/*
 * Copyright 2021-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_DSMT_ASR

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Include file system */
#include "sln_flash.h"
#include "sln_flash_files.h"
#include "sln_flash_fs_ops.h"

/* Drivers, settings */
#include "sln_rgb_led_driver.h"
#include "sln_amplifier.h"
#include "sln_mic_config.h"
#include "sln_i2s_mic.h"

/* Structures, API */
#include "sln_local_voice_common.h"
#include "sln_local_voice_dsmt.h"
#include "IndexCommands.h"
#include "audio_processing_task.h"
#include "demo_actions.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NUM_SAMPLES_AFE_OUTPUT            (480)
#if USE_DSMT_EVALUATION_MODE
#define DSMT_EVALUATION_DETECTIONS_LIMIT  (100)
#endif /* USE_DSMT_EVALUATION_MODE */

#if USE_DSMT_STATIC_POOLS

#if MULTILINGUAL
#define WAKE_WORD_MEMPOOL_SIZE    (50 * 1024)
#else
/* allocate a buffer big enough to hold a CN with Tone recognition model as well.
 * in case this is not used, mempool can be reduced to 70KB for DSMT Level 4 and
 * to 50KB for DSMT Level 1  */
#define WAKE_WORD_MEMPOOL_SIZE    (90 * 1024)
#endif /* !MULTILINGUAL */

#define CN_WAKE_WORD_MEMPOOL_SIZE (90 * 1024)
#define COMMAND_MEMPOOL_SIZE      (90 * 1024)

typedef struct pool_entry_t_
{
    uint8_t *pool_ptr;
    uint32_t pool_size;
    bool     pool_used;
} pool_entry_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
SDK_ALIGN(uint8_t __attribute__((section(".bss.$SRAM_DTC"))) g_memPoolWLang0[WAKE_WORD_MEMPOOL_SIZE], 8);

#if MULTILINGUAL
/* Put commands static pool in non cacheable ocram as we don't have any more space in cacheable
 * when MULTILINGUAL is on */
SDK_ALIGN(uint8_t __attribute__((section(".bss.$SRAM_DTC"))) g_memPoolCmd[COMMAND_MEMPOOL_SIZE], 8);
#else
SDK_ALIGN(uint8_t __attribute__((section(".bss.$SRAM_DTC"))) g_memPoolCmd[COMMAND_MEMPOOL_SIZE], 8);
#endif /* MULTILINGUAL */

#if MULTILINGUAL
/* NOTE: Chinese with Tone Recognition model takes larger memory pool than the other languages.
 * Make sure Chinese model is placed in g_memPoolWLang3 with CN_WAKE_WORD_MEMPOOL_SIZE.
 * Also, make sure Chinese model is installed in the same order for install_language() and install_inference_engine(). */
SDK_ALIGN(uint8_t __attribute__((section(".bss.$SRAM_DTC"))) g_memPoolWLang1[WAKE_WORD_MEMPOOL_SIZE], 8);
SDK_ALIGN(uint8_t __attribute__((section(".bss.$SRAM_DTC"))) g_memPoolWLang2[WAKE_WORD_MEMPOOL_SIZE], 8);
SDK_ALIGN(uint8_t __attribute__((section(".bss.$SRAM_DTC"))) g_memPoolWLang3[CN_WAKE_WORD_MEMPOOL_SIZE], 8);

/* NOTE: must be in pool size order, from lower to higher pool size! */
pool_entry_t g_memPoolWw[MAX_CONCURRENT_LANGUAGES] =      {{g_memPoolWLang0, sizeof(g_memPoolWLang0), false},
                                                           {g_memPoolWLang1, sizeof(g_memPoolWLang1), false},
                                                           {g_memPoolWLang2, sizeof(g_memPoolWLang2), false},
                                                           {g_memPoolWLang3, sizeof(g_memPoolWLang3), false}};

#else
pool_entry_t g_memPoolWw[MAX_CONCURRENT_LANGUAGES] =      {{g_memPoolWLang0, sizeof(g_memPoolWLang0), false}};
#endif /* MULTILINGUAL */

#endif /* USE_DSMT_STATIC_POOLS */

extern QueueHandle_t g_xSampleQueue;
extern volatile uint32_t g_wakeWordLength;
extern TaskHandle_t appTaskHandle;
extern oob_demo_control_t oob_demo_control;
extern bool g_SW1Pressed;
extern EventGroupHandle_t xBindingGroup;
extern QueueHandle_t xMatterActionsQueue;

struct asr_language_model g_asrLangModel[MAX_CONCURRENT_LANGUAGES] = {};
struct asr_inference_engine g_asrInfWW[MAX_CONCURRENT_LANGUAGES]   = {};
struct asr_inference_engine g_asrInfCMD                            = {};
asr_control_t g_asrControl                                         = {};
app_asr_shell_commands_t appAsrShellCommands                       = {};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Utility function to extract indices from bitwise variable. this is used for asr_inference_t.
 */
static unsigned int decode_bitshift(unsigned int x)
{
    unsigned int y = 1; // starting from index 1 (not 0)
    while (x >>= 1)
        y++;
    return y;
}

/*!
 * @brief Utility function to break model binary pack into multiple groups where each group will represent an
 * inference engine combined with base model.
 */
static signed int unpackBin(unsigned char lpbyBin[], unsigned char *lppbyModel[], int32_t nMaxNumModel)
{
    unsigned int *lpnBin     = (unsigned int *)lpbyBin;
    signed int nNumBin       = lpnBin[0];
    unsigned int *lpnBinSize = lpnBin + 1;
    signed int i;

    lppbyModel[0] = (unsigned char *)(lpnBinSize + nNumBin);
    for (i = 1; i < nNumBin; i++)
    {
        if (i >= nMaxNumModel)
            break;
        lppbyModel[i] = lppbyModel[i - 1] + lpnBinSize[i - 1];
    }

    return i;
}

/*!
 * @brief Language model installation.
 */
int32_t install_language(asr_control_t *pAsrCtrl,
                         struct asr_language_model *pLangModel,
                         asr_language_t lang,
                         unsigned char *pAddrBin,
                         uint8_t nGroups)
{
    sln_asr_local_states_t status = kAsrLocalSuccess;
    uint8_t nGroupsMapID          = nGroups - 2; // -1 cause there is no MapID group for base model,
                                        // -1 cause the nGroups contains 1 more section with the no of mapID binaries

    if (lang && pAddrBin && nGroups)
    {
        pLangModel->iWhoAmI = lang;
        pLangModel->addrBin = pAddrBin;
        pLangModel->nGroups = nGroups;
        pLangModel->next    = pAsrCtrl->langModel;
        pAsrCtrl->langModel = pLangModel;

        if ((status = unpackBin(pAddrBin, pLangModel->addrGroup, nGroups)) <
            nGroups) // unpack group addresses from model binary
        {
            configPRINTF(("Invalid bin. Error Code: %d.\r\n", status));
        }
        else
        {
            if ((status = unpackBin(pLangModel->addrGroup[nGroups - 1], pLangModel->addrGroupMapID, nGroupsMapID)) <
                (nGroupsMapID)) // unpack group addresses from mapID binary
            {
                configPRINTF(("Invalid bin. Error Code: %d.\r\n", status));
            }
        }
    }
    else
        status = 1;

    return status;
}

/*!
 * @brief Inference engine installation.
 */
static uint32_t install_inference_engine(asr_control_t *pAsrCtrl,
                                         struct asr_inference_engine *pInfEngine,
                                         asr_language_t lang,
                                         asr_inference_t infType,
                                         char **idToString)
{
    sln_asr_local_states_t status = kAsrLocalSuccess;
    uint32_t sizeMemPool          = 0;
    uint32_t memUsage             = 0;
    void *addrMemPool             = NULL;

    struct asr_language_model *pLang = NULL;
    int idx                          = decode_bitshift(infType);

    if (pAsrCtrl && pInfEngine && lang && infType)
    {
        for (pLang = pAsrCtrl->langModel; pLang != NULL; pLang = pLang->next)
        {
            if (pLang->iWhoAmI == lang)
                break;
        }
    }

    if (pLang)
    {
        /* find out how big the pool needs to be */
        memUsage = (uint32_t) SLN_ASR_LOCAL_Verify(pLang->addrGroup[0],
                                                   (unsigned char **)&pLang->addrGroup[idx],
                                                   1,
                                                   MAX_COMMAND_FRAMES);

#if USE_DSMT_STATIC_POOLS
        if (infType == ASR_WW)
        {
            /* find the first pool that fits the needs; pools are expected to be
             * ordered by size when they are declared */
            for (int i = 0; i < MAX_CONCURRENT_LANGUAGES; i++)
            {
                if ((memUsage <= g_memPoolWw[i].pool_size) && (g_memPoolWw[i].pool_used == false))
                {
                    addrMemPool = g_memPoolWw[i].pool_ptr;
                    sizeMemPool = g_memPoolWw[i].pool_size;
                    g_memPoolWw[i].pool_used = true;
                    break;
                }
            }
        }
        else
        {
            /* there's only one pool for commands */
            addrMemPool = &g_memPoolCmd[0];
            sizeMemPool = COMMAND_MEMPOOL_SIZE;
        }
#else
        /* Check if initial allocation or reallocation needed */
        if ((NULL == pInfEngine->memPool) || (memUsage > pInfEngine->memPoolSize))
        {
            if (pInfEngine->memPool)
            {
                vPortFree(pInfEngine->memPool);
                pInfEngine->memPool = NULL;
            }

            addrMemPool = pvPortMalloc(memUsage);
            sizeMemPool = memUsage;
        }
        else
        {
            addrMemPool = pInfEngine->memPool;
            sizeMemPool = pInfEngine->memPoolSize;
        }
#endif /* USE_DSMT_STATIC_POOLS */

        /* stop here if couldn't allocate pool */
        if (NULL == addrMemPool)
        {
            configPRINTF(("Failed to allocate a memory pool for the inference engine!\r\n"));
            status = kAsrLocalOutOfMemory;
            RGB_LED_SetColor(LED_COLOR_ORANGE);

            while (1)
            {
                vTaskDelay(10000);
            }
        }
    }

    if (addrMemPool && sizeMemPool)
    {
        pInfEngine->iWhoAmI_inf  = infType;
        pInfEngine->iWhoAmI_lang = lang;
        pInfEngine->handler      = NULL;
        pInfEngine->nGroups      = 2;
        pInfEngine->idToKeyword  = idToString;
        pInfEngine->memPool      = addrMemPool;
        pInfEngine->memPoolSize  = sizeMemPool;
        if (infType == ASR_WW)
        {                                                  // linked list for WW engines
            pInfEngine->next      = pAsrCtrl->infEngineWW; // the end of pInfEngine->next should be NULL
            pAsrCtrl->infEngineWW = pInfEngine;
        }
        else
        {   // linked list for CMD engines. Dialog demo needs a linked list of CMD engines.
            pInfEngine->next       = pAsrCtrl->infEngineCMD;
            pAsrCtrl->infEngineCMD = pInfEngine;
        }
    }
    else
        status = kAsrLocalInstallFailed;

    return status;
}

/*!
 * @brief Checks memory pool size for WW / CMD engines.
 */
void verify_inference_handler(struct asr_inference_engine *p)
{
    sln_asr_local_states_t status = kAsrLocalSuccess;
    int32_t mem_usage;

    mem_usage = SLN_ASR_LOCAL_Verify(p->addrGroup[0], (unsigned char **)&p->addrGroup[1], 1, MAX_COMMAND_FRAMES);

    if ((p->iWhoAmI_inf == ASR_WW) && (mem_usage > p->memPoolSize))
    {
        configPRINTF(("Memory size %d for WW exceeds the memory pool %d!\r\n", mem_usage, p->memPoolSize));
        status = kAsrLocalOutOfMemory;
    }
    else if ((p->iWhoAmI_inf != ASR_WW) && (mem_usage > p->memPoolSize))
    {
        configPRINTF(("Memory size %d for CMD exceeds the memory pool %d!\r\n", mem_usage, p->memPoolSize));
        status = kAsrLocalOutOfMemory;
    }

    if (status != kAsrLocalSuccess)
    {
        RGB_LED_SetColor(LED_COLOR_ORANGE);
        while (1)
        {
            vTaskDelay(10000);
        }
    }
}

/*!
 * @brief Handler should be set with valid
 *  p->addrGroup[0] (base model address) and
 *  p->addrGroup[1] (application group such as WW, CMD for IoT, etc)
 */
void set_inference_handler(struct asr_inference_engine *p)
{
    int status = kAsrLocalSuccess;

#if !USE_DSMT_STATIC_POOLS
    int32_t mem_usage = 0;
    void *mem_pool    = p->memPool;

    /* find out how big the pool needs to be */
    mem_usage = SLN_ASR_LOCAL_Verify(p->addrGroup[0],
                                     (unsigned char **)&p->addrGroup[1],
                                     1,
                                     MAX_COMMAND_FRAMES);

    /* Check if initial allocation or reallocation needed */
    if ((NULL == p->memPool) || (mem_usage > p->memPoolSize))
    {
        if (p->memPool)
        {
            vPortFree(p->memPool);
            p->memPool = NULL;
        }

        mem_pool = pvPortMalloc(mem_usage);

        if (NULL == mem_pool)
        {
            configPRINTF(("Failed to allocate %d bytes for inference engine memory pool!\r\n", mem_usage));
            status = kAsrLocalOutOfMemory;
            RGB_LED_SetColor(LED_COLOR_ORANGE);

            while (1)
            {
                vTaskDelay(10000);
            }
        }
        else
        {
            p->memPool = mem_pool;
            p->memPoolSize = mem_usage;
        }
    }
#endif /* USE_DSMT_STATIC_POOLS */

    p->handler = SLN_ASR_LOCAL_Init(p->addrGroup[0], (unsigned char **)&p->addrGroup[1], 1, MAX_COMMAND_FRAMES, p->memPool,
                                    p->memPoolSize, (int32_t *)&status);

    if (status != kAsrLocalSuccess)
    {
        configPRINTF(("Could not initialize ASR engine. Please check language settings or license limitations!\r\n"));
        RGB_LED_SetColor(LED_COLOR_RED);
        while (1)
        {
            vTaskDelay(1000);
        }
    }

    if ((status = SLN_ASR_LOCAL_Set_CmdMapID(p->handler, &p->addrGroupMapID, 1)) != kAsrLocalSuccess)
    {
        configPRINT_STRING("Fail to set map id! - %d\r\n", status);
    }
}

/*!
 * @brief Handler should be reset after WW or CMDs are detected.
 */
void reset_inference_handler(struct asr_inference_engine *p)
{
    SLN_ASR_LOCAL_Reset(p->handler);
}

/*!
 * @brief Initialize WW inference engine from the installed language models.
 *  After, pInfEngine should be a linked list of the installed languages.
 */
void init_WW_engine(asr_control_t *pAsrCtrl, asr_inference_t infType)
{
    struct asr_inference_engine *pInfEngine;
    struct asr_language_model *pLang;
    int idx       = decode_bitshift(ASR_WW); // decode the bitwise ASR_WW which is 1.
    int idx_mapID = idx - 1;                 // the index for mapIDs starts from 0 instead of 1

    pInfEngine = pAsrCtrl->infEngineWW;
    for (pLang = pAsrCtrl->langModel; pLang != NULL; pLang = pLang->next)
    {
        pInfEngine->addrGroup[0]   = pLang->addrGroup[0];              // language model's base
        pInfEngine->addrGroup[1]   = pLang->addrGroup[idx];            // language model's wake word group
        pInfEngine->addrGroupMapID = pLang->addrGroupMapID[idx_mapID]; // language model's wake word mapID group

        verify_inference_handler(pInfEngine); // verify inference handler, checking mem pool size
        set_inference_handler(pInfEngine);    // set inf engine to ww mode for each language.

        pInfEngine = pInfEngine->next; // the end of pInfEngine->next should be NULL.
    }
}

/*!
 * @brief Initialize CMD inference engine from the installed language models.
 */
void init_CMD_engine(asr_control_t *pAsrCtrl, asr_inference_t infType)
{
    struct asr_inference_engine *pInfEngine;
    struct asr_language_model *pLang;
    int idx       = decode_bitshift(infType); // decode the bitwise infType variable.
    int idx_mapID = idx - 1;                  // the index for mapIDs starts from 0 instead of 1

    pInfEngine = pAsrCtrl->infEngineCMD;

    pLang                      = pAsrCtrl->langModel;              // langModel for CMD inf engine is selected when WW is detected.
    pInfEngine->addrGroup[0]   = pLang->addrGroup[0];              // the selected language model's base
    pInfEngine->addrGroup[1]   = pLang->addrGroup[idx];            // the selected language model's infType group
    pInfEngine->addrGroupMapID = pLang->addrGroupMapID[idx_mapID]; // the selected language model's mapID group

    verify_inference_handler(pInfEngine); // verify inference handler, checking mem pool size
    set_inference_handler(pInfEngine);    // set inf engine to ww mode for each language.
}

/*!
 * @brief Set language WW recognition engines.
 */
void set_WW_engine(asr_control_t *pAsrCtrl)
{
    struct asr_inference_engine *pInf = pAsrCtrl->infEngineWW;

    for (pInf = pAsrCtrl->infEngineWW; pInf != NULL; pInf = pInf->next)
    {
        set_inference_handler(pInf);
    }
}

/*!
 * @brief Reset language WW recognition engines.
 */
void reset_WW_engine(asr_control_t *pAsrCtrl)
{
    struct asr_inference_engine *pInf = pAsrCtrl->infEngineWW;

    for (pInf = pAsrCtrl->infEngineWW; pInf != NULL; pInf = pInf->next)
    {
        reset_inference_handler(pInf);
    }
}

/*!
 * @brief Set specific language CMD recognition engine, post WW detection.
 */
void set_CMD_engine(asr_control_t *pAsrCtrl, asr_language_t langType, asr_inference_t infCMDType, char **cmdString)
{
    struct asr_language_model *pLang;
    struct asr_inference_engine *pInf = pAsrCtrl->infEngineCMD;
    int idx                           = decode_bitshift(infCMDType); // decode the bitwise infType variable
    int idx_mapID                     = idx - 1;                     // the index for mapIDs starts from 0 instead of 1

    for (pLang = pAsrCtrl->langModel; pLang != NULL; pLang = pLang->next)
    {
        if (pLang->iWhoAmI == langType)
        {
            pInf->iWhoAmI_inf  = infCMDType;
            pInf->iWhoAmI_lang = langType;
            pInf->addrGroup[0] = pLang->addrGroup[0]; // base model. should be same with WW's base
            if (pLang->addrGroup[idx] != NULL)
            {
                pInf->addrGroup[1]   = pLang->addrGroup[idx];
                pInf->addrGroupMapID = pLang->addrGroupMapID[idx_mapID];
            }
            set_inference_handler(pInf);
            pInf->idToKeyword = cmdString;
            break; // exit for loop, once pInf is set with the intended language
        }
    }
}

/*!
 * @brief Reset specific language CMD recognition engine.
 */
void reset_CMD_engine(asr_control_t *pAsrCtrl)
{
    struct asr_inference_engine *pInf = pAsrCtrl->infEngineCMD;

    reset_inference_handler(pInf);
}

/*!
 * @brief Process audio stream to detect wake words or commands.
 */
int asr_process_audio_buffer(void *handler, int16_t *audBuff, uint16_t bufSize, asr_inference_t infType)
{
    int status = 0;
    // reset values
    g_asrControl.result.keywordID[0] = 0xFFFF;
    g_asrControl.result.keywordID[1] = 0xFFFF;
    g_asrControl.result.cmdMapID     = 0xFF;

    status = SLN_ASR_LOCAL_Process(handler, audBuff, bufSize, &g_asrControl.result);

    return status;
}

char *asr_get_string_by_id(struct asr_inference_engine *pInfEngine, int32_t id)
{
    return pInfEngine->idToKeyword[id];
}

void print_asr_version(void)
{
    const char *version = SLN_ASR_LOCAL_Ver();
    configPRINT_STRING("ASR engine: DSMT version %s\r\n", version);
}

void initialize_asr(void)
{
    asr_inference_t demoType                     = appAsrShellCommands.demo;
    asr_language_t lang[MAX_CONCURRENT_LANGUAGES]           = {UNDEFINED_LANGUAGE};
    asr_language_t langShell                     = 0;

    if (appAsrShellCommands.activeLanguage == UNDEFINED_LANGUAGE)
    {
        appAsrShellCommands.activeLanguage       = DEFAULT_ASR_LANGUAGE;
        oob_demo_control.language                = DEFAULT_ASR_LANGUAGE;
    }

    /* oob_demo_control will be updated after first detected WW */
    if (oob_demo_control.language == UNDEFINED_LANGUAGE)
    {
        oob_demo_control.language = DEFAULT_ASR_LANGUAGE;
    }

    if (oob_demo_control.commandSet == UNDEFINED_INFERENCE)
    {
        oob_demo_control.commandSet = DEFAULT_ASR_CMD_DEMO;
    }

    langShell = appAsrShellCommands.activeLanguage;

    for (int i = 0; i < MAX_CONCURRENT_LANGUAGES; i++)
    {
        for (int j = 0; j < NUM_LANGUAGES; j++)
        {
            if (langShell & (ASR_FIRST_LANGUAGE << j))
            {
                lang[i] = langShell & (ASR_FIRST_LANGUAGE << j);
                langShell &= ~(ASR_FIRST_LANGUAGE << j);
                break;
            }
        }
    }

    // NULL to ensure the end of linked list.
    g_asrControl.langModel    = NULL;
    g_asrControl.infEngineWW  = NULL;
    g_asrControl.infEngineCMD = NULL;

    for (int i = MAX_CONCURRENT_LANGUAGES - 1; i >= 0; i--)
    {
        install_language(&g_asrControl,
                         &g_asrLangModel[i],
                         lang[i],
                         (unsigned char *)get_demo_model(lang[i], demoType),
                         NUM_GROUPS);
    }

#if USE_DSMT_STATIC_POOLS
    /* Reset all used WW memory pools */
    for (int i = MAX_CONCURRENT_LANGUAGES - 1; i >= 0; i--)
    {
        g_memPoolWw[i].pool_used = false;
    }
#endif /* USE_DSMT_STATIC_POOLS */

    for (int i = MAX_CONCURRENT_LANGUAGES - 1; i >= 0; i--)
    {
        install_inference_engine(&g_asrControl,
                                 &g_asrInfWW[i],
                                 lang[i],
                                 ASR_WW,
                                 get_ww_strings(lang[i]));
    }

    // CMD inference engine will be reset with detected language after WW is detected
    install_inference_engine(&g_asrControl,
                             &g_asrInfCMD,
                             active_languages_get_first(appAsrShellCommands.activeLanguage),
                             demoType,
                             get_cmd_strings(active_languages_get_first(appAsrShellCommands.activeLanguage), demoType));

    // init
    init_WW_engine(&g_asrControl, demoType);
    init_CMD_engine(&g_asrControl, demoType);
}

void print_asr_session(int status)
{
    switch (status)
    {
        case ASR_SESSION_STARTED:
            configPRINTF(("[ASR] Session started\r\n"));
            break;
        case ASR_SESSION_ENDED:
            configPRINTF(("[ASR] Session ended\r\n"));
            break;
        case ASR_SESSION_TIMEOUT:
            configPRINTF(("[ASR] Timed out waiting for response\r\n"));
            break;
    }
}

/*!
 * @brief ASR main task
 */
void local_voice_task(void *arg)
{
    int16_t pi16Sample[NUM_SAMPLES_AFE_OUTPUT];
    uint32_t len            = 0;
    uint32_t statusFlash    = 0;
    asr_events_t asrEvent   = ASR_SESSION_ENDED;
    struct asr_inference_engine *pInfWW;
    struct asr_inference_engine *pInfCMD;
    char **cmdString;
#if USE_DSMT_EVALUATION_MODE
    static uint32_t s_detectionsCounter = 0;
#endif /* USE_DSMT_EVALUATION_MODE */

#if ENABLE_VAD
    /* VAD will suspend / resume this task accordingly */
    audio_processing_set_local_voice_task_handle(xTaskGetCurrentTaskHandle());
#endif /* ENABLE_VAD */

    /* wait until mics are active */
    while (!SLN_MIC_GET_STATE())
    {
        vTaskDelay(1);
    }

    // Read Shell Commands Parameters from flash memory. If not available, initialize and write into flash memory.
    statusFlash = sln_flash_fs_ops_read(ASR_SHELL_COMMANDS_FILE_NAME, NULL, 0, &len);
    if (statusFlash != SLN_FLASH_FS_OK)
    {
        configPRINTF(("Failed reading local demo configuration from flash memory.\r\n"));
    }
    else
    {
        statusFlash = sln_flash_fs_ops_read(ASR_SHELL_COMMANDS_FILE_NAME, (uint8_t *)&appAsrShellCommands, 0, &len);

        if ((statusFlash != SLN_FLASH_FS_OK) || len != sizeof(app_asr_shell_commands_t))
        {
            configPRINTF(("Failed reading local demo configuration from flash memory.\r\n"));
        }
    }

    if ((appAsrShellCommands.status != WRITE_SUCCESS) ||
        (false == validate_all_active_languages(appAsrShellCommands.activeLanguage, appAsrShellCommands.demo)) ||
        (appAsrShellCommands.version != APP_ASR_SHELL_VERSION) ||
        appAsrShellCommands.vitActive)
    {
        appAsrShellCommands.demo         = BOOT_ASR_CMD_DEMO;
        appAsrShellCommands.asrMode      = ASR_MODE_WW_AND_CMD;
#if MULTILINGUAL
        appAsrShellCommands.activeLanguage = DEFAULT_ASR_MULTILANGUAGE;
#else
        appAsrShellCommands.activeLanguage = DEFAULT_ASR_LANGUAGE;
#endif
        appAsrShellCommands.micsState    = ASR_MICS_ON;
        appAsrShellCommands.timeout      = TIMEOUT_TIME_IN_MS;
        appAsrShellCommands.volume       = DEFAULT_SPEAKER_VOLUME;
        appAsrShellCommands.status       = WRITE_SUCCESS;
        appAsrShellCommands.asrCfg       = ASR_CFG_DEMO_NO_CHANGE;
        appAsrShellCommands.vitActive    = 0;
        appAsrShellCommands.version      = APP_ASR_SHELL_VERSION;

        sln_flash_fs_ops_erase(ASR_SHELL_COMMANDS_FILE_NAME);
        statusFlash = sln_flash_fs_ops_save(ASR_SHELL_COMMANDS_FILE_NAME, (uint8_t *)&appAsrShellCommands,
                                          sizeof(app_asr_shell_commands_t));
        if (statusFlash != SLN_FLASH_FS_OK)
        {
            configPRINTF(("Failed writing local demo configuration in flash memory.\r\n"));
        }
    }

    initialize_asr();

    // We need to reset asrCfg state so we won't remember an unprocessed demo change that was saved in flash
    appAsrShellCommands.asrCfg = ASR_CFG_DEMO_NO_CHANGE;

    while (!g_xSampleQueue)
        vTaskDelay(10);

    while (1)
    {
        if (xQueueReceive(g_xSampleQueue, pi16Sample, portMAX_DELAY) != pdPASS)
        {
            configPRINTF(("Could not receive from the queue\r\n"));
        }

        // push-to-talk
        if ((g_SW1Pressed == true) && (asrEvent == ASR_SESSION_ENDED) && (appAsrShellCommands.asrMode == ASR_MODE_PTT))
        {
            g_SW1Pressed             = false;
            g_asrControl.sampleCount = 0;

            // only one language CMD
            cmdString = get_cmd_strings(appAsrShellCommands.activeLanguage, appAsrShellCommands.demo);
            set_CMD_engine(&g_asrControl, appAsrShellCommands.activeLanguage, appAsrShellCommands.demo, cmdString);
            oob_demo_control.language = appAsrShellCommands.activeLanguage;

            asrEvent = ASR_SESSION_STARTED;
            print_asr_session(asrEvent); // print ASR session status

            xTaskNotify(appTaskHandle, kWakeWordDetected, eSetBits);
        }

        /* Skip Wake Word phase and go directly to Voice Command phase.
         * As language will be selected last detected language.
         * As demo will be selected currently enabled demo. */
        if (oob_demo_control.skipWW == 1)
        {
            g_asrControl.sampleCount = 0;
            asrEvent                 = ASR_SESSION_STARTED;
            print_asr_session(asrEvent);

            cmdString = get_cmd_strings(oob_demo_control.language, appAsrShellCommands.demo);
            set_CMD_engine(&g_asrControl, oob_demo_control.language, appAsrShellCommands.demo, cmdString);

            reset_WW_engine(&g_asrControl);

            oob_demo_control.skipWW = 0;
        }

        // continue listening to wake words in the selected languages. pInfWW is language specific.
        if ((asrEvent == ASR_SESSION_ENDED) && (appAsrShellCommands.asrMode != ASR_MODE_PTT))
        {
            for (pInfWW = g_asrControl.infEngineWW; pInfWW != NULL; pInfWW = pInfWW->next)
            {
                if (asr_process_audio_buffer(pInfWW->handler, pi16Sample, NUM_SAMPLES_AFE_OUTPUT,
                                             pInfWW->iWhoAmI_inf) == kAsrLocalDetected)
                {
                    if (asr_get_string_by_id(pInfWW, g_asrControl.result.keywordID[0]) != NULL)
                    {
                        g_wakeWordLength = SLN_ASR_LOCAL_GetDetectedCommandDuration(pInfWW->handler);
                        if ((appAsrShellCommands.asrMode == ASR_MODE_WW_AND_CMD) ||
                            (appAsrShellCommands.asrMode == ASR_MODE_WW_AND_MUL_CMD))
                        {
                            asrEvent = ASR_SESSION_STARTED;
                            print_asr_session(asrEvent);
                        }
#if USE_DSMT_EVALUATION_MODE
                        s_detectionsCounter = s_detectionsCounter + 1;
#endif /* USE_DSMT_EVALUATION_MODE */
                        configPRINTF(("[ASR] Wake Word: %s(%d) - MapID(%d)\r\n",
                                      asr_get_string_by_id(pInfWW, g_asrControl.result.keywordID[0]),
                                      g_asrControl.result.keywordID[0], g_asrControl.result.cmdMapID));
                        if (appAsrShellCommands.cmdresults == ASR_CMD_RES_ON)
                        {
                            configPRINTF(("      Trust: %d, SGDiff: %d\r\n", g_asrControl.result.trustScore,
                                          g_asrControl.result.SGDiffScore));
                        }

                        cmdString = get_cmd_strings(pInfWW->iWhoAmI_lang, appAsrShellCommands.demo);
                        set_CMD_engine(&g_asrControl, pInfWW->iWhoAmI_lang, appAsrShellCommands.demo, cmdString);

                        oob_demo_control.language   = pInfWW->iWhoAmI_lang;
                        oob_demo_control.commandSet = appAsrShellCommands.demo;

                        reset_WW_engine(&g_asrControl);

                        // Notify App Task Wake Word Detected
                        xTaskNotify(appTaskHandle, kWakeWordDetected, eSetBits);
                        break; // exit for loop
                    }          // end of if (asr_get_string_by_id(pInfWW, g_asrControl.keywordID[0]) != NULL)
                } // end of if (asr_process_audio_buffer(pInfWW->handler, pi16Sample, NUM_SAMPLES_AFE_OUTPUT,
                  // pInfWW->iWhoAmI_inf))
            }     // end of for (pInfWW = g_asrControl.infEngineWW; pInfWW != NULL; pInfWW = pInfWW->next)
        }         // end of if (asrEvent == ASR_SESSION_ENDED)
        // now we are getting into command detection. It must detect a command within the waiting time.
        else if (asrEvent == ASR_SESSION_STARTED)
        {
            pInfCMD = g_asrControl.infEngineCMD;

            if (asr_process_audio_buffer(pInfCMD->handler, pi16Sample, NUM_SAMPLES_AFE_OUTPUT, pInfCMD->iWhoAmI_inf) ==
                kAsrLocalDetected)
            {
                if (asr_get_string_by_id(pInfCMD, g_asrControl.result.keywordID[1]) != NULL)
                {
#if USE_DSMT_EVALUATION_MODE
                    s_detectionsCounter = s_detectionsCounter + 1;
#endif /* USE_DSMT_EVALUATION_MODE */
                    configPRINTF(("[ASR] Command: %s(%d) - MapID(%d)\r\n",
                                  asr_get_string_by_id(pInfCMD, g_asrControl.result.keywordID[1]),
                                  g_asrControl.result.keywordID[1], g_asrControl.result.cmdMapID));
                    if (appAsrShellCommands.cmdresults == ASR_CMD_RES_ON)
                    {
                        configPRINTF(("      Trust: %d, SGDiff: %d\r\n", g_asrControl.result.trustScore,
                                      g_asrControl.result.SGDiffScore));
                    }

                    matterActionStruct valueToSend = {0};
                    valueToSend.location = "all";
                    if (xMatterActionsQueue != 0)
                    {
                        if (g_asrControl.result.keywordID[1] == 0 || g_asrControl.result.keywordID[1] == 1 ||
                            g_asrControl.result.keywordID[1] == 2 || g_asrControl.result.keywordID[1] == 3 ||
                            g_asrControl.result.keywordID[1] == 4 || g_asrControl.result.keywordID[1] == 5)
                        {
                            valueToSend.action = kMatterActionOnOff;
                            valueToSend.command = "on";
                        }
                        else if (g_asrControl.result.keywordID[1] == 6 || g_asrControl.result.keywordID[1] == 7 ||
                                g_asrControl.result.keywordID[1] == 8)
                        {
                            valueToSend.action = kMatterActionOnOff;
                            valueToSend.command = "off";
                        }
                        else if (g_asrControl.result.keywordID[1] == 9 || g_asrControl.result.keywordID[1] == 10 ||
                                g_asrControl.result.keywordID[1] == 11)
                        {
                            valueToSend.action = kMatterActionBlindsControl;
                            valueToSend.command = "lift";
                            valueToSend.value = (void *)100;
                            valueToSend.location = "central";
                        }
                        else if (g_asrControl.result.keywordID[1] == 12 || g_asrControl.result.keywordID[1] == 13 ||
                                g_asrControl.result.keywordID[1] == 14 || g_asrControl.result.keywordID[1] == 15 ||
                                g_asrControl.result.keywordID[1] == 16 || g_asrControl.result.keywordID[1] == 17)
                        {
                            valueToSend.action = kMatterActionBlindsControl;
                            valueToSend.command = "close";
                            valueToSend.value = (void *)0;
                            valueToSend.location = "central";
                        }
                        else if (g_asrControl.result.keywordID[1] == 18)
                        {
                            valueToSend.action = kMatterActionBrightnessChange;
                            valueToSend.command = "increase";
                            valueToSend.value = (void *)20;
                        }
                        else if (g_asrControl.result.keywordID[1] == 19)
                        {
                            valueToSend.action = kMatterActionBrightnessChange;
                            valueToSend.command = "decrease";
                            valueToSend.value = (void *)20;
                        }

                        xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
                    }

                    g_asrControl.sampleCount = 0;

                    if (appAsrShellCommands.asrMode == ASR_MODE_WW_AND_MUL_CMD)
                    {
                        asrEvent = ASR_SESSION_STARTED;
                    }
                    else if (appAsrShellCommands.asrMode == ASR_MODE_WW_AND_CMD)
                    {
                        asrEvent = ASR_SESSION_ENDED;
                    }

                    oob_demo_control.language   = pInfCMD->iWhoAmI_lang;
                    oob_demo_control.commandSet = pInfCMD->iWhoAmI_inf;
                    oob_demo_control.commandId  = g_asrControl.result.keywordID[1];
                    xTaskNotify(appTaskHandle, kVoiceCommandDetected, eSetBits);

                    if (asrEvent == ASR_SESSION_ENDED)
                    {
                        print_asr_session(asrEvent);
                    }

                    reset_CMD_engine(&g_asrControl);
                    set_WW_engine(&g_asrControl);

                } // end of asr_get_string()
            }     // end of asr_process_audio_buffer()

            // calculate waiting time.
            g_asrControl.sampleCount += NUM_SAMPLES_AFE_OUTPUT;

            // No timeout when ASR is in cmd only mode
            if ((g_asrControl.sampleCount > 16000 / 1000 * appAsrShellCommands.timeout) && (appAsrShellCommands.asrMode != ASR_MODE_CMD_ONLY))
            {
                g_asrControl.sampleCount = 0;

                reset_CMD_engine(&g_asrControl);

                asrEvent = ASR_SESSION_ENDED;
                print_asr_session(ASR_SESSION_TIMEOUT);
                print_asr_session(asrEvent);

                set_WW_engine(&g_asrControl);

                // Notify App Task Timeout
                xTaskNotify(appTaskHandle, kTimeOut, eSetBits);
            }
        } // end of else if (asrEvent == ASR_SESSION_STARTED)

        // reinitialize the ASR engine if language set or command set was changed
        if (appAsrShellCommands.asrCfg & (ASR_CFG_DEMO_LANGUAGE_CHANGED | ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED))
        {
            g_asrControl.sampleCount = 0;
            reset_CMD_engine(&g_asrControl);
            set_WW_engine(&g_asrControl);

            initialize_asr();
            appAsrShellCommands.asrCfg &= ~(ASR_CFG_DEMO_LANGUAGE_CHANGED | ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED);

            asrEvent = ASR_SESSION_ENDED;

            xTaskNotify(appTaskHandle, kAsrModelChanged, eSetBits);
        }

        if (appAsrShellCommands.asrCfg & ASR_CFG_MODE_CHANGED)
        {
            appAsrShellCommands.asrCfg &= ~ASR_CFG_MODE_CHANGED;

            if (appAsrShellCommands.asrMode == ASR_MODE_CMD_ONLY)
            {
                asrEvent = ASR_SESSION_STARTED;
            }
            else
            {
                asrEvent = ASR_SESSION_ENDED;
            }
        }

#if USE_DSMT_EVALUATION_MODE
        /* DSMT lib is limited to 100 detections. When this limit is reached the board will not detect
         * any commands. Power reset the board to reset the counter.
         */
        if (s_detectionsCounter == DSMT_EVALUATION_DETECTIONS_LIMIT)
        {
            while (1)
            {
                configPRINTF(("[ASR] DSMT detections limit reached (%d commands). Please power reset the board.\r\n",
                        DSMT_EVALUATION_DETECTIONS_LIMIT));
                vTaskDelay(5000);
            }
        }
#endif /* USE_DSMT_EVALUATION_MODE */
    } // end of while
}

#endif /* ENABLE_DSMT_ASR */
