/*
 * Copyright 2018-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_SHELL

#include "sln_shell.h"
#include "IndexCommands.h"
#include "sln_local_voice_common.h"
#include "sln_afe.h"
#include "sln_app_fwupdate.h"
#include "ctype.h"
#include "sln_flash_fs_ops.h"
#include "flash_ica_driver.h"
#include "local_sounds_task.h"
#include "perf.h"

#if ENABLE_USB_AUDIO_DUMP
#include "composite.h"
#include "audio_dump.h"
#endif /* ENABLE_USB_AUDIO_DUMP */

#if ENABLE_VAD
#include "audio_processing_task.h"
#endif /* ENABLE_VAD */

#if ENABLE_WIFI
#include "wifi_connection.h"
#include "wifi_credentials.h"
#endif /* ENABLE_WIFI */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
#define SLN_SERIAL_MANAGER_RECEIVE_BUFFER_LEN 2048U
#endif

#define BANK_A_STRING "BankA"
#define BANK_B_STRING "BankB"
#define BANK_U_STRING "BankUnknown"

/* Max number of arguments for a shell command */
#define MAX_ARGC          5
/* Max argv string size, string terminator included */
#define MAX_ARGV_STR_SIZE 32
/* A longer argv string size, string terminator included */
#define MAX_ARGV_LONG_STR_SIZE 100

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void *pvPortCalloc(size_t nmemb, size_t xSize);

static shell_status_t sln_reset_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t sln_factoryreset_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t sln_changebank_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t sln_commands_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#ifdef SHELL_SELECTABLE_DEMOS
static shell_status_t sln_changedemo_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif /* SHELL_SELECTABLE_DEMOS */
#if ENABLE_AMPLIFIER
static shell_status_t sln_volume_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#if ENABLE_STREAMER
static shell_status_t sln_playprompt_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif /* ENABLE_STREAMER */
#endif /* ENABLE_AMPLIFIER */
static shell_status_t sln_mics_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t sln_timeout_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t sln_asrmode_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#ifdef SHELL_SELECTABLE_LANGUAGES
static shell_status_t sln_changelang_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif /* SHELL_SELECTABLE_LANGUAGES */
#if ENABLE_DSMT_ASR
static shell_status_t sln_cmdresults_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif /* ENABLE_DSMT_ASR */
static shell_status_t sln_updateotw_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t sln_version_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#if ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER
static shell_status_t sln_usb_aec_align_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif /* ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER */
#if SLN_TRACE_CPU_USAGE
static shell_status_t sln_cpuview_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif /* SLN_TRACE_CPU_USAGE */
static shell_status_t sln_memview_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t sln_heapview_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t sln_stacksview_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#if ENABLE_WIFI
static shell_status_t sln_wifi_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif /* ENABLE_WIFI */
#if ENABLE_AEC
static shell_status_t sln_aecmode_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif /* ENABLE_AEC */

/*******************************************************************************
 * Variables
 ******************************************************************************/
SHELL_COMMAND_DEFINE(reset, "\r\n\"reset\": Resets the MCU.\r\n", sln_reset_handler, 0);

SHELL_COMMAND_DEFINE(factoryreset, "\r\n\"factoryreset\": Reset the board to the original settings\r\n", sln_factoryreset_handler, 0);

SHELL_COMMAND_DEFINE(changebank,
                     "\r\n\"changebank\": Change the active bank. If Bank A is active, this command will change the boot settings to Bank B and vice versa.\r\n",
                     sln_changebank_handler,
                     0);

SHELL_COMMAND_DEFINE(commands,
                     "\r\n\"commands\": List available voice commands for selected demo.\r\n",
                     sln_commands_handler,
                     0);

#ifdef SHELL_SELECTABLE_DEMOS
SHELL_COMMAND_DEFINE(changedemo,
                     "\r\n\"changedemo\": Change the command set. Save in flash memory.\r\n"
                     "         Usage:\r\n"
                     "            changedemo <param> \r\n"
                     "            when called without parameters, it will display the current demo\r\n"
                     "         Parameters\r\n"
                     "            one of the following demos: "
                     SHELL_SELECTABLE_DEMOS
                     "\r\n",
                     sln_changedemo_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);
#endif /* SHELL_SELECTABLE_DEMOS */

#ifdef SHELL_SELECTABLE_LANGUAGES
SHELL_COMMAND_DEFINE(changelang,
                     "\r\n\"changelang\": Change language(s). Save in flash memory.\r\n"
                     "         Usage:\r\n"
#if MULTILINGUAL
                     "            changelang language_code1 up to language_code4\r\n"
#else
                     "            changelang language_code\r\n"
#endif
                     "            when called without parameters, it will display the current active language(s)\r\n"
                     "         Parameters\r\n"
#if MULTILINGUAL
                     "            one or more of the following languages, separated by spaces: "
#else
                     "            only one of the following languages: "
#endif
                     SHELL_SELECTABLE_LANGUAGES
                     "\r\n",
                     sln_changelang_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);
#endif /* SHELL_SELECTABLE_LANGUAGES */

#if ENABLE_AMPLIFIER
SHELL_COMMAND_DEFINE(volume,
                     "\r\n\"volume\": Set speaker volume (0 - 100). Save in flash memory.\r\n"
                     "         Usage:\r\n"
                     "            volume N \r\n"
                     "            when called without parameters, it will display the current volume\r\n"
                     "         Parameters\r\n"
                     "            N between 0 and 100\r\n",
                     sln_volume_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);

#if ENABLE_STREAMER
SHELL_COMMAND_DEFINE(playprompt,
                     "\r\n\"playprompt\": Play prompt from file system.\r\n"
                     "         Usage:\r\n"
                     "            playprompt PATH \r\n"
                     "         Parameters\r\n"
                     "            PATH (e.g. Additional_Prompts/EN/smart_home_demo_en.opus) is the path of audio prompt in file system\r\n",
                     sln_playprompt_handler,
                     1);
#endif /* ENABLE_STREAMER */
#endif /* ENABLE_AMPLIFIER */

SHELL_COMMAND_DEFINE(mics,
                     "\r\n\"mics\": Set microphones state (on / off). Save in flash memory.\r\n"
                     "         Usage:\r\n"
                     "            mics on (or off)\r\n"
                     "            when called without parameters, it will display the current state of the microphones\r\n"
                     "         Parameters\r\n"
                     "            on or off\r\n",
                     sln_mics_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(timeout,
                     "\r\n\"timeout\": Set command waiting time (in ms). Save in flash memory.\r\n"
                     "         Usage:\r\n"
                     "            timeout N \r\n"
                     "            when called without parameters, it will display the current timeout\r\n"
                     "         Parameters\r\n"
                     "            N milliseconds\r\n",
                     sln_timeout_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(asrmode,
                     "\r\n\"asrmode\": Set ASR mode (wwcmd / wwmulcmd / ww / cmd / ptt). Save in flash memory.\r\n"
                     "         Usage:\r\n"
                     "            asrmode wwcmd / wwmulcmd / ww / cmd / ptt\r\n"
                     "            When called without parameters, it will display the current active asrmode\r\n"
                     "         Parameters\r\n"
                     "            One of the following: wwcmd / wwmulcmd / ww / cmd / ptt\r\n"
                     "            wwcmd: ASR processes wake-word + command pairs\r\n"
                     "            wwmulcmd: ASR processes wake-word + multiple commands\r\n"
                     "            ww: ASR processes only wake-words\r\n"
                     "            cmd: ASR processes only commands\r\n"
                     "            ptt: ASR processes commands after push-to-talk action (press on SW3)\r\n",
                     sln_asrmode_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);

#if ENABLE_DSMT_ASR
SHELL_COMMAND_DEFINE(cmdresults,
                     "\r\n\"cmdresults\": Print the command detection results in console.\r\n"
                     "         Usage:\r\n"
                     "            cmdresults on (or off) \r\n"
                     "            when called without parameters, it will display the status of printing command detection results\r\n"
                     "         Parameters\r\n"
                     "            on or off\r\n",
                     sln_cmdresults_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);
#endif /* ENABLE_DSMT_ASR */

SHELL_COMMAND_DEFINE(updateotw,
                     "\r\n\"updateotw\": Restarts the board in the OTW update mode.\r\n",
                     sln_updateotw_handler,
                     0);

SHELL_COMMAND_DEFINE(version, "\r\n\"version\": Print firmware version\r\n", sln_version_handler, 0);

#if ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER
SHELL_COMMAND_DEFINE(usb_aec_align,
                     "\r\n\"usb_aec_align\": Starts playing the aec alignment sound.\r\n",
                     sln_usb_aec_align_handler,
                     0);
#endif /* ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER */

#if SLN_TRACE_CPU_USAGE
SHELL_COMMAND_DEFINE(cpuview, "\r\n\"cpuview\": Print the CPU usage info\r\n", sln_cpuview_handler, 0);
#endif /* SLN_TRACE_CPU_USAGE */

SHELL_COMMAND_DEFINE(memview, "\r\n\"memview\": View available FreeRTOS heap\r\n", sln_memview_handler, 0);
SHELL_COMMAND_DEFINE(heapview, "\r\n\"heapview\": Print FreeRTOS heap consumption\r\n", sln_heapview_handler, 0);
SHELL_COMMAND_DEFINE(stacksview, "\r\n\"stacksview\": Print FreeRTOS tasks stack consumption\r\n", sln_stacksview_handler, 0);

#if ENABLE_WIFI
SHELL_COMMAND_DEFINE(wifi,
                     "\r\n\"wifi\": Setup, erase or print the WiFi Network Credentials\r\n"
                     "           Setup the WiFi Network Credentials:\r\n"
                     "              Usage:\r\n"
                     "                 wifi setup SSID [PASSWORD]\r\n"
                     "              Parameters:\r\n"
                     "              SSID:       The wireless network name\r\n"
                     "              PASSWORD:   The password for the wireless network\r\n"
                     "                          For open networks it is not needed\r\n"
                     "           Erase the current WiFi Network credentials from flash:\r\n"
                     "              Usage:\r\n"
                     "                 wifi erase\r\n"
                     "           Print the WiFi Network Credentials currently stored in flash:\r\n"
                     "              Usage:\r\n"
                     "                 wifi print\r\n",
                     /* if more than two parameters, it'll take just the first two of them */
                     sln_wifi_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);
#endif /* ENABLE_WIFI */

#if ENABLE_AEC
SHELL_COMMAND_DEFINE(aecmode,
                     "\r\n\"aecmode\": Set the aecmode to true or false. \r\n"
                     "                 This setting can only be changed at runtime and it is not persistent,\r\n"
                     "                 after reboot aecmode will be true again.\r\n"
                     "         Usage:\r\n"
                     "            aecmode on (or off) \r\n"
                     "            when called without parameters, it will display the status of aecmode\r\n"
                     "         Parameters\r\n"
                     "            on or off\r\n",
                     sln_aecmode_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);
#endif /* ENABLE_AEC */

extern app_asr_shell_commands_t appAsrShellCommands;
extern TaskHandle_t appTaskHandle;

static uint8_t __attribute__((section(".bss.$SRAM_OC_NON_CACHEABLE")))s_shellHandleBuffer[SHELL_HANDLE_SIZE];
static shell_handle_t s_shellHandle;

static uint8_t s_serialHandleBuffer[SERIAL_MANAGER_HANDLE_SIZE];
static serial_handle_t s_serialHandle = &s_serialHandleBuffer[0];

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
__attribute__((section(".bss.$SRAM_OC_NON_CACHEABLE"))) __attribute__((aligned(8)))
uint8_t readRingBuffer[SLN_SERIAL_MANAGER_RECEIVE_BUFFER_LEN];
#endif

static EventGroupHandle_t s_ShellEventGroup;
static shell_heap_trace_t s_heap_trace = {0};

static uint8_t s_argc = 0;
static char __attribute__((section(".bss.$SRAM_OC_NON_CACHEABLE")))s_argv[MAX_ARGC + 1][MAX_ARGV_STR_SIZE + 1] = {0};
static char __attribute__((section(".bss.$SRAM_OC_NON_CACHEABLE")))s_argvLong[MAX_ARGV_LONG_STR_SIZE + 1]      = {0};

#if ENABLE_WIFI
static wifi_cred_t s_wifi_cred = {0};
#endif /* ENABLE_WIFI */

extern oob_demo_control_t oob_demo_control;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get current running bank */
static char *getActiveBank(void)
{
    char *activeBank = BANK_U_STRING;

#if ENABLE_REMAP
    /* If remap is enabled find the current running bank by checking
     * the remap offset register */
    if (REMAP_DISABLED_OFFSET == IOMUXC_GPR->GPR32)
    {
        activeBank = BANK_A_STRING;
    }
    else if (REMAP_ENABLED_OFFSET == IOMUXC_GPR->GPR32)
    {
        activeBank = BANK_B_STRING;
    }
#else
    /* If remap is not enabled, find the current running bank by checking
     * the ResetISR Address in the vector table (which is loaded into DTC). */
    uint32_t runningFromBankA =
        (((*(uint32_t *)(APPLICATION_RESET_ISR_ADDRESS)) & APP_VECTOR_TABLE_APP_A) == APP_VECTOR_TABLE_APP_A);
    uint32_t runningFromBankb =
        (((*(uint32_t *)(APPLICATION_RESET_ISR_ADDRESS)) & APP_VECTOR_TABLE_APP_B) == APP_VECTOR_TABLE_APP_B);

    if (runningFromBankA)
    {
        activeBank = BANK_A_STRING;
    }
    else if (runningFromBankb)
    {
        activeBank = BANK_B_STRING;
    }
#endif /* ENABLE_REMAP */

    return activeBank;
}

static shell_status_t isNumber(char *arg)
{
    int32_t status = kStatus_SHELL_Success;
    uint32_t i;

    for (i = 0; arg[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)arg[i]))
        {
            status = kStatus_SHELL_Error;
        }
    }

    return status;
}

#if ENABLE_USB_SHELL
static void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    if (CONTROLLER_ID == kSerialManager_UsbControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
}
#endif /* ENABLE_USB_SHELL */

/*******************************************************************************
 * Code - Commands implementation
 ******************************************************************************/

/* reset command */
/*****************/
static void sln_reset_cmd_action(void)
{
    NVIC_SystemReset();
}

static shell_status_t sln_reset_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, RESET_EVENT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_reset_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

/* factory reset command */
/*************************/
static void sln_factoryreset_cmd_action(void)
{
    uint32_t statusFlash  = SLN_FLASH_FS_OK;

    statusFlash = sln_flash_fs_ops_erase(ASR_SHELL_COMMANDS_FILE_NAME);

    if (statusFlash != SLN_FLASH_FS_OK)
    {
        SHELL_Printf(s_shellHandle, "Failed deleting local demo configuration from flash memory.\r\n\r\n");
    }
    else
    {
        SHELL_Printf(s_shellHandle, "Resetting the board to original settings.\r\n\r\n");
        NVIC_SystemReset();
    }
}

static shell_status_t sln_factoryreset_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, FACTORY_RESET_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_factoryreset_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

/* changebank command */
/**********************/
static void sln_changebank_cmd_action(void)
{
    int32_t oldImgType;
    uint32_t status = SLN_FLASH_NO_ERROR;

    status = FICA_GetCurAppStartType(&oldImgType);
    if (status == SLN_FLASH_NO_ERROR)
    {
        if (FICA_IMG_TYPE_APP_A == oldImgType)
        {
            FICA_SetCurAppStartType(FICA_IMG_TYPE_APP_B);
            SHELL_Printf(s_shellHandle, "Application bank switched from A to B\r\n");
        }
        else
        {
            FICA_SetCurAppStartType(FICA_IMG_TYPE_APP_A);
            SHELL_Printf(s_shellHandle, "Application bank switched from B to A\r\n");
        }
    }
    else
    {
        SHELL_Printf(s_shellHandle, "[Error] Switching application bank failed %d\r\n", status);
    }

    SHELL_Printf(s_shellHandle, "\r\nRebooting...\r\n");
    vTaskDelay(200);
    NVIC_SystemReset();
}

static shell_status_t sln_changebank_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, CHANGE_BANK_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_changebank_cmd_action();
#endif /* ENABLE_USB_SHELL */
    return kStatus_SHELL_Success;
}

/* commands command */
/********************/
static void sln_commands_cmd_action(void)
{
#if ENABLE_VIT_ASR || ENABLE_DSMT_ASR
    uint16_t cmd_number     = 0;
    uint16_t ww_number      = 0;
    asr_language_t lang         = ASR_FIRST_LANGUAGE;
    asr_language_t activeLanguage = appAsrShellCommands.activeLanguage;

    SHELL_Printf(s_shellHandle, "Available commands in selected language(s):\r\n\r\n");

    while (activeLanguage != UNDEFINED_LANGUAGE)
    {
        if ((activeLanguage & lang) != UNDEFINED_LANGUAGE)
        {
            SHELL_Printf(s_shellHandle, "Demo:      %s\r\n", get_demo_string(lang, appAsrShellCommands.demo));
            SHELL_Printf(s_shellHandle, "Language:  %s\r\n", get_language_str_from_id(lang));
            SHELL_Printf(s_shellHandle, "Wake Words:\r\n");

            ww_number = get_ww_number((asr_language_t)lang, appAsrShellCommands.demo);
            char **wwString = get_ww_strings(lang);

            for (unsigned int i = 0; i < ww_number; i++)
            {
                if (strlen(wwString[i]) > 1)
                {
                    if (wwString[i][strlen(wwString[i]) - 2] == '^')
                    {
                        /* Skip duplicated wake words. Duplicated wake words have '^' at the end. */
                        continue;
                    }
                }

                SHELL_Printf(s_shellHandle, "  %s\r\n", wwString[i]);
            }

            SHELL_Printf(s_shellHandle, "Commands:\r\n");

            cmd_number = get_cmd_number((asr_language_t)lang, appAsrShellCommands.demo);
            char **cmdString = get_cmd_strings(lang, appAsrShellCommands.demo);

            for (unsigned int i = 0; i < cmd_number; i++)
            {
                if (strlen(cmdString[i]) > 1)
                {
                    if (cmdString[i][strlen(cmdString[i]) - 2] == '^')
                    {
                        /* Skip duplicated commands. Duplicated commands have '^' at the end. */
                        continue;
                    }
                }

                SHELL_Printf(s_shellHandle, "  %s\r\n", cmdString[i]);
            }
            activeLanguage &= ~lang;

            SHELL_Printf(s_shellHandle, "\r\n");
        }
        lang <<= 1U;
    }
#elif ENABLE_S2I_ASR
    SHELL_Printf(s_shellHandle, "\r\nDemo:      ");

    switch (appAsrShellCommands.demo)
    {
        case ASR_S2I_HVAC:
            SHELL_Printf(s_shellHandle, "hvac\r\n");
            break;
        case ASR_S2I_OVEN:
            SHELL_Printf(s_shellHandle, "oven\r\n");
            break;
        case ASR_S2I_HOME:
            SHELL_Printf(s_shellHandle, "home\r\n");
            break;
    }

    SHELL_Printf(s_shellHandle, "Language:  en\r\n");
    SHELL_Printf(s_shellHandle, "Wake Words: Hey NXP\r\n\r\n");

    SHELL_Printf(s_shellHandle, "Example expressions (full list in expressions.txt):\r\n");

    switch (appAsrShellCommands.demo)
    {
        case ASR_S2I_HVAC:
            SHELL_Printf(s_shellHandle, "i am feeling really cold today\r\n");
            SHELL_Printf(s_shellHandle, "i get quite warm now\r\n");
            SHELL_Printf(s_shellHandle, "it is very heated now\r\n");
            SHELL_Printf(s_shellHandle, "i am very freezing today\r\n");
            SHELL_Printf(s_shellHandle, "set fan speed to low\r\n");
            SHELL_Printf(s_shellHandle, "turn speed to auto\r\n");
            SHELL_Printf(s_shellHandle, "set the temperature to fifteen degrees\r\n");
            SHELL_Printf(s_shellHandle, "increase the temperature by three\r\n");
            SHELL_Printf(s_shellHandle, "lower temperature by five degrees\r\n");
            SHELL_Printf(s_shellHandle, "set a timer for two hours and ten minutes\r\n");
            break;
        case ASR_S2I_OVEN:
            SHELL_Printf(s_shellHandle, "set the convection function\r\n");
            SHELL_Printf(s_shellHandle, "activate the defrost\r\n");
            SHELL_Printf(s_shellHandle, "preheat the oven to one hundred twenty degrees\r\n");
            SHELL_Printf(s_shellHandle, "bake at one hundred ninety five\r\n");
            SHELL_Printf(s_shellHandle, "raise temperature by five degrees\r\n");
            SHELL_Printf(s_shellHandle, "reduce temperature by ten\r\n");
            SHELL_Printf(s_shellHandle, "decrease the heat to seventy five degrees\r\n");
            SHELL_Printf(s_shellHandle, "put to two hundred degrees\r\n");
            SHELL_Printf(s_shellHandle, "update the temperature at one hundred seventy\r\n");
            SHELL_Printf(s_shellHandle, "launch a timer to one hour\r\n");
           break;
        case ASR_S2I_HOME:
            SHELL_Printf(s_shellHandle, "set the brightness level to max in the kitchen\r\n");
            SHELL_Printf(s_shellHandle, "turn up light brightness by twenty percent in the bedroom\r\n");
            SHELL_Printf(s_shellHandle, "set the bedroom lamps to blue\r\n");
            SHELL_Printf(s_shellHandle, "change the living room light to orange\r\n");
            SHELL_Printf(s_shellHandle, "switch the bathroom lights to pink\r\n");
            SHELL_Printf(s_shellHandle, "set lamps to purple in the bedroom\r\n");
            SHELL_Printf(s_shellHandle, "turn lighting in the bathroom to green\r\n");
            SHELL_Printf(s_shellHandle, "yellow lights in the kitchen\r\n");
            SHELL_Printf(s_shellHandle, "open middle window shades\r\n");
            SHELL_Printf(s_shellHandle, "pull up the shades halfway\r\n");
            break;
    }
#endif
}

static shell_status_t sln_commands_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, PRINT_COMMANDS_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_commands_cmd_action();
#endif /* ENABLE_USB_SHELL */
    return kStatus_SHELL_Success;
}

#ifdef SHELL_SELECTABLE_DEMOS
/* changedemo command */
/**********************/
static void sln_changedemo_cmd_action(void)
{
#if ENABLE_S2I_ASR
    char *str;
    uint16_t demo_id              = UNDEFINED_INFERENCE;

    if (s_argc > 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else
    {
        if (s_argc == 1)
        {
            switch (appAsrShellCommands.demo)
               {
                   case ASR_S2I_HVAC:
                       SHELL_Printf(s_shellHandle, "Demo set to hvac.\r\n");
                       break;
                   case ASR_S2I_OVEN:
                       SHELL_Printf(s_shellHandle, "Demo set to oven.\r\n");
                       break;
                   case ASR_S2I_HOME:
                       SHELL_Printf(s_shellHandle, "Demo set to home.\r\n");
                       break;
               }
        }
        else
        {
            str = s_argv[1];

            if (!strcmp("hvac", str))
            {
                demo_id = ASR_S2I_HVAC;
            }
            else if (!strcmp("oven", str))
            {
                demo_id = ASR_S2I_OVEN;
            }
            else if (!strcmp("home", str))
            {
                demo_id = ASR_S2I_HOME;
            }

            if (demo_id != UNDEFINED_INFERENCE)
            {
                appAsrShellCommands.demo   = demo_id;
                appAsrShellCommands.status = WRITE_READY;
                appAsrShellCommands.asrCfg = ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED;
                SHELL_Printf(s_shellHandle, "Changing to %s commands demo.\r\n", str);

#if ENABLE_VAD
                audio_processing_force_vad_event();
#endif /* ENABLE_VAD */

                xTaskNotifyFromISR(appTaskHandle, kDefault, eSetBits, NULL);
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Invalid input.\r\n");
            }
        }
    }
#else
    char *str;
    uint16_t demo_id              = UNDEFINED_INFERENCE;
    asr_language_t lang           = ASR_FIRST_LANGUAGE;
    asr_language_t activeLanguage = appAsrShellCommands.activeLanguage;

    if (s_argc > 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else
    {
        if (s_argc == 1)
        {
            while (activeLanguage != UNDEFINED_LANGUAGE)
            {
                if ((activeLanguage & lang) != UNDEFINED_LANGUAGE)
                {
                    SHELL_Printf(s_shellHandle, "Demo set to %s.\r\n", get_demo_string(lang, appAsrShellCommands.demo));
                    activeLanguage &= ~lang;

                    break;
                }
                lang <<= 1U;
            }
        }
        else
        {
            str = s_argv[1];

            demo_id   = get_demo_id_from_str(str);

            if (demo_id != UNDEFINED_INFERENCE)
            {
                appAsrShellCommands.demo   = demo_id;
                appAsrShellCommands.status = WRITE_READY;
                appAsrShellCommands.asrCfg = ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED;
                SHELL_Printf(s_shellHandle, "Changing to %s commands demo.\r\n", str);

#if ENABLE_VAD
                audio_processing_force_vad_event();
#endif /* ENABLE_VAD */

                xTaskNotifyFromISR(appTaskHandle, kDefault, eSetBits, NULL);
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Invalid input.\r\n");
            }
        }
    }
#endif /* ENABLE_S2I_ASR */
}

static shell_status_t sln_changedemo_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    strncpy(s_argv[1], argv[1], MAX_ARGV_STR_SIZE);

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, CHANGE_DEMO_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_changedemo_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}
#endif /* SHELL_SELECTABLE_DEMOS */

#ifdef SHELL_SELECTABLE_LANGUAGES
/* changelang command */
/**********************/
static void sln_changelang_cmd_action(void)
{
    int32_t status                  = kStatus_SHELL_Success;
    int32_t language_limit          = 0;
    asr_language_t activeLanguage   = UNDEFINED_LANGUAGE;

    int idx;

    if ((s_argc - 1) > MAX_CONCURRENT_LANGUAGES)
    {
        SHELL_Printf(s_shellHandle,
                     "\r\nIncorrect command parameter(s). Please enter up to %d languages at a time.\r\n\r\n",
                     MAX_CONCURRENT_LANGUAGES);
    }
    else
    {
        /* when called without arguments, we will print the active languages */
        if (s_argc == 1)
        {
            activeLanguage = appAsrShellCommands.activeLanguage;

            SHELL_Printf(s_shellHandle, "Enabled language(s):");
            print_active_languages_str(appAsrShellCommands.activeLanguage, appAsrShellCommands.demo);
            SHELL_Printf(s_shellHandle, ".\r\n");
        }
        else
        {
            activeLanguage = UNDEFINED_LANGUAGE;

#if MULTILINGUAL
            if ((appAsrShellCommands.asrMode == ASR_MODE_PTT) &&
                (s_argc > 2))
            {
                /* during PTT only one language will work */
                SHELL_Printf(s_shellHandle, "Only one language supported during Push-to-Talk mode\r\n");
            }
            else
#endif /* MULTILINGUAL */
            {
                SHELL_Printf(s_shellHandle, "Enabling ");

                language_limit = s_argc;

                for (idx = 1; idx < language_limit; idx++)
                {
                    uint16_t current_lang_id = get_language_id_from_str(s_argv[idx]);

                    if ((current_lang_id == UNDEFINED_LANGUAGE) ||
                        (activeLanguage & current_lang_id))
                    {
                        SHELL_Printf(s_shellHandle, "\r\nERROR(arg: %s) ", s_argv[idx]);
                        status = kStatus_SHELL_Error;
                        break;
                    }
                    else
                    {
                        activeLanguage |= current_lang_id;
                        SHELL_Printf(s_shellHandle, "%s ", s_argv[idx]);
                    }
                }

                if (status == kStatus_SHELL_Success)
                {
                    SHELL_Printf(s_shellHandle, "language(s).\r\n");
                }

                if (status == kStatus_SHELL_Success)
                {
                    appAsrShellCommands.activeLanguage = activeLanguage;
                    appAsrShellCommands.status         = WRITE_READY;
                    appAsrShellCommands.asrCfg         = ASR_CFG_DEMO_LANGUAGE_CHANGED;

                    if (s_argc == 2)
                    {
                        /* if only one language sent, let's change the oob demo as well
                         * to have the prompts in this language */
                        oob_demo_control.language = activeLanguage;
                    }
                    else
                    {
                        /* if multiple languages sent, let's use the first one for prompts initially */
                        oob_demo_control.language = get_language_id_from_str(s_argv[1]);
                    }

#if ENABLE_VAD
                    audio_processing_force_vad_event();
#endif /* ENABLE_VAD */

                    xTaskNotifyFromISR(appTaskHandle, kDefault, eSetBits, NULL);
                }
                else
                {
                    SHELL_Printf(s_shellHandle,
                                 "\r\nIncorrect/duplicated command parameter(s). Enter \"help\" to view a list of "
                                 "available commands.\r\n\r\n");
                }
            }
        }
    }
}

static shell_status_t sln_changelang_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    if (s_argc <= MAX_ARGC)
    {
        for (uint8_t i = 0; i < argc; i++)
        {
            strncpy(s_argv[i], argv[i], MAX_ARGV_STR_SIZE);
        }
    }

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, CHANGE_LANG_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_changelang_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}
#endif /* SHELL_SELECTABLE_LANGUAGES */

#if ENABLE_AMPLIFIER
/* volume command */
/******************/
static void sln_volume_cmd_action(void)
{
    if (s_argc > 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else
    {
        if (s_argc == 1)
        {
            SHELL_Printf(s_shellHandle, "Speaker volume set to %d.\r\n", appAsrShellCommands.volume);
        }
        else if (s_argc == 2 && (isNumber(s_argv[1]) == kStatus_SHELL_Success) && atoi(s_argv[1]) >= 0 &&
                 atoi(s_argv[1]) <= 100)
        {
            appAsrShellCommands.volume = (uint32_t)atoi(s_argv[1]);
            appAsrShellCommands.status = WRITE_READY;
            SHELL_Printf(s_shellHandle, "Setting speaker volume to %d.\r\n", appAsrShellCommands.volume);

            xTaskNotifyFromISR(appTaskHandle, kVolumeUpdate, eSetBits, NULL);
        }
        else
        {
            SHELL_Printf(s_shellHandle, "Invalid volume value. Set between 0 to 100.\r\n", appAsrShellCommands.volume);
        }
    }
}

static shell_status_t sln_volume_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    strncpy(s_argv[1], argv[1], MAX_ARGV_STR_SIZE);

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, SET_VOLUME_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_volume_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

#if ENABLE_STREAMER
/* playprompt command */
/**********************/
static void sln_playprompt_cmd_action(void)
{
    char *promptPath;
    uint32_t len          = 0;
    uint32_t statusFlash  = 0;

    promptPath = s_argvLong;

    statusFlash = sln_flash_fs_ops_read((const char *)promptPath, NULL, 0, &len);

    if (s_argc != 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else if (statusFlash == SLN_FLASH_FS_OK)
    {
        /* Make sure that speaker is not currently playing another audio. */
        while (LOCAL_SOUNDS_isPlaying())
        {
            vTaskDelay(10);
        }

#if ENABLE_VAD
        audio_processing_force_vad_event();
#endif /* ENABLE_VAD */

        SHELL_Printf(s_shellHandle, "Playing audio prompt %s.\r\n", promptPath);
        LOCAL_SOUNDS_PlayAudioFile(promptPath, appAsrShellCommands.volume);
    }
    else
    {
        SHELL_Printf(s_shellHandle, "Invalid path %s.\r\n", promptPath);
    }
}

static shell_status_t sln_playprompt_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    strncpy(s_argvLong, argv[1], MAX_ARGV_LONG_STR_SIZE);

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, PLAY_PROMPT_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_playprompt_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}
#endif /* ENABLE_STREAMER */
#endif /* ENABLE_AMPLIFIER */


/* asrmode command */
/*******************/
static void sln_asrmode_cmd_action(void)
{
    char *str;

    if (s_argc == 1)
    {
        SHELL_Printf(s_shellHandle,"Current ASR mode: ");
        if (appAsrShellCommands.asrMode == ASR_MODE_WW_AND_CMD)
        {
            SHELL_Printf(s_shellHandle,"wwcmd\r\n");
        }
        else if (appAsrShellCommands.asrMode == ASR_MODE_WW_AND_MUL_CMD)
        {
            SHELL_Printf(s_shellHandle,"wwmulcmd\r\n");
        }
        else if (appAsrShellCommands.asrMode == ASR_MODE_WW_ONLY)
        {
            SHELL_Printf(s_shellHandle,"ww\r\n");
        }
        else if (appAsrShellCommands.asrMode == ASR_MODE_CMD_ONLY)
        {
            SHELL_Printf(s_shellHandle,"cmd\r\n");
        }
        else if (appAsrShellCommands.asrMode == ASR_MODE_PTT)
        {
            SHELL_Printf(s_shellHandle,"ptt\r\n");
        }
    }
    else if (s_argc == 2)
    {
        str = s_argv[1];

        if (strcmp(str, "wwcmd") == 0)
        {
            appAsrShellCommands.asrMode = ASR_MODE_WW_AND_CMD;
            appAsrShellCommands.status  = WRITE_READY;
            appAsrShellCommands.asrCfg |= ASR_CFG_MODE_CHANGED;
            SHELL_Printf(s_shellHandle, "Setting ASR mode to wwcmd.\r\n");
        }
        else if (strcmp(str, "wwmulcmd") == 0)
        {
            appAsrShellCommands.asrMode = ASR_MODE_WW_AND_MUL_CMD;
            appAsrShellCommands.status  = WRITE_READY;
            appAsrShellCommands.asrCfg |= ASR_CFG_MODE_CHANGED;
            SHELL_Printf(s_shellHandle, "Setting ASR mode to wwmulcmd.\r\n");
        }
        else if (strcmp(str, "ww") == 0)
        {
            appAsrShellCommands.asrMode = ASR_MODE_WW_ONLY;
            appAsrShellCommands.status  = WRITE_READY;
            appAsrShellCommands.asrCfg |= ASR_CFG_MODE_CHANGED;
            SHELL_Printf(s_shellHandle, "Setting ASR mode to ww.\r\n");
        }
        else if (strcmp(str, "cmd") == 0)
        {
            appAsrShellCommands.asrMode = ASR_MODE_CMD_ONLY;
            appAsrShellCommands.status  = WRITE_READY;
            appAsrShellCommands.asrCfg |= ASR_CFG_MODE_CHANGED;
#if MULTILINGUAL
            uint16_t prevLanguage              = appAsrShellCommands.activeLanguage;
            appAsrShellCommands.activeLanguage = active_languages_get_first(appAsrShellCommands.activeLanguage);
            oob_demo_control.language          = appAsrShellCommands.activeLanguage;

            if (appAsrShellCommands.activeLanguage != prevLanguage)
            {
                appAsrShellCommands.asrCfg    |= ASR_CFG_DEMO_LANGUAGE_CHANGED;
            }

            SHELL_Printf(s_shellHandle, "Setting ASR mode to cmd. %s language active.\r\n",
                          get_language_str_from_id(active_languages_get_first(appAsrShellCommands.activeLanguage)));
#else
#if ENABLE_S2I_ASR
            SHELL_Printf(s_shellHandle, "Setting ASR mode to cmd.\r\n");
#else
            SHELL_Printf(s_shellHandle, "Setting ASR mode to cmd. %s language active.\r\n",
                          get_language_str_from_id(appAsrShellCommands.activeLanguage));
#endif /* ENABLE_S2I_ASR */
#endif /* MULTILINGUAL */
        }
        else if (strcmp(str, "ptt") == 0)
        {
            if (appAsrShellCommands.micsState == ASR_MICS_ON)
            {
                appAsrShellCommands.asrMode = ASR_MODE_PTT;
                appAsrShellCommands.status  = WRITE_READY;
#if MULTILINGUAL
                uint16_t prevLanguage              = appAsrShellCommands.activeLanguage;
                appAsrShellCommands.activeLanguage = active_languages_get_first(appAsrShellCommands.activeLanguage);
                oob_demo_control.language          = appAsrShellCommands.activeLanguage;

                if (appAsrShellCommands.activeLanguage != prevLanguage)
                {
                    appAsrShellCommands.asrCfg         = ASR_CFG_DEMO_LANGUAGE_CHANGED;
                }

                SHELL_Printf(s_shellHandle, "Setting ASR Push-To-Talk mode on. %s language active.\r\n",
                              get_language_str_from_id(active_languages_get_first(appAsrShellCommands.activeLanguage)));
#else
#if ENABLE_S2I_ASR
            SHELL_Printf(s_shellHandle, "Setting ASR Push-To-Talk mode on.\r\n");
#else
            SHELL_Printf(s_shellHandle, "Setting ASR Push-To-Talk mode on. %s language active.\r\n",
                          get_language_str_from_id(appAsrShellCommands.activeLanguage));
#endif /* ENABLE_S2I_ASR */
#endif /* MULTILINGUAL */

                xTaskNotifyFromISR(appTaskHandle, kMicUpdate, eSetBits, NULL);
            }
            else
            {
                /* cannot activate PTT when mics are off */
                SHELL_Printf(s_shellHandle, "Mics are turned off! Turn mics on before activating ASR Push-To-Talk mode.\r\n");
            }
        }
        else
        {
            SHELL_Printf(s_shellHandle, "Invalid input.\r\n");
        }

        xTaskNotifyFromISR(appTaskHandle, kAsrModeChanged, eSetBits, NULL);

#if ENABLE_VAD
        audio_processing_force_vad_event();
#endif /* ENABLE_VAD */
    }
    else
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
}

static shell_status_t sln_asrmode_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    strncpy(s_argv[1], argv[1], MAX_ARGV_STR_SIZE);

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, ASR_MODE_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_asrmode_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

/* timeout command */
/*******************/
static void sln_timeout_cmd_action(void)
{
    if (s_argc > 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else
    {
        if (s_argc == 1)
        {
            SHELL_Printf(s_shellHandle, "Timeout value set to %d.\r\n", appAsrShellCommands.timeout);
        }
        else if (s_argc == 2 && (isNumber(s_argv[1]) == kStatus_SHELL_Success) && atoi(s_argv[1]) >= 0)
        {
            appAsrShellCommands.timeout = (uint32_t)atoi(s_argv[1]);
            appAsrShellCommands.status  = WRITE_READY;
            SHELL_Printf(s_shellHandle, "Setting command waiting time to %d ms.\r\n", appAsrShellCommands.timeout);

            xTaskNotifyFromISR(appTaskHandle, kDefault, eSetBits, NULL);
        }
        else
        {
            SHELL_Printf(s_shellHandle, "Invalid waiting time %d ms.\r\n", appAsrShellCommands.timeout);
        }
    }
}

static shell_status_t sln_timeout_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    strncpy(s_argv[1], argv[1], MAX_ARGV_STR_SIZE);

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, SET_TIMEOUT_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_timeout_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

#if ENABLE_WIFI
static shell_status_t sln_wifi_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    s_argc = argc;
    if (s_argc <= MAX_ARGC)
    {
        for (uint8_t i = 0; i < argc; i++)
        {
            strncpy(s_argv[i], argv[i], MAX_ARGV_STR_SIZE);
        }
    }

    xEventGroupSetBitsFromISR(s_ShellEventGroup, WIFI_EVT, &xHigherPriorityTaskWoken);
    return kStatus_SHELL_Success;
}

static void sln_wifi_action(void)
{
    shell_status_t status             = kStatus_SHELL_Success;
    sln_flash_fs_status_t flashStatus = SLN_FLASH_FS_OK;
    wifi_cred_t wifi_cred             = {0};
    char *str;

    if (s_argc < 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else
    {
        str = s_argv[1];

        if (strcmp(str, "setup") == 0)
        {
            if (s_argc < 3)
            {
                SHELL_Printf(
                    s_shellHandle,
                    "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
            }
            else
            {
                char *kWiFiName     = NULL;
                char *kWiFiPassword = NULL;

                kWiFiName = s_argv[2];

                if (s_argc > 3)
                {
                    kWiFiPassword = s_argv[3];
                }

                uint32_t name_len = strlen(kWiFiName);
                uint32_t pass_len = kWiFiPassword ? strlen(kWiFiPassword) : 0;

                if (name_len + 1 <= sizeof(s_wifi_cred.ssid.value))
                {
                    memcpy(s_wifi_cred.ssid.value, kWiFiName, name_len);
                    s_wifi_cred.ssid.length = name_len;
                }
                else
                {
                    status = kStatus_SHELL_Error;
                }

                if (kStatus_SHELL_Success == status)
                {
                    if (pass_len + 1 <= sizeof(s_wifi_cred.password.value))
                    {
                        if (pass_len != 0)
                        {
                            memcpy(s_wifi_cred.password.value, kWiFiPassword, pass_len + 1);
                        }
                        else
                        {
                            s_wifi_cred.password.value[0] = '\0';
                        }
                        s_wifi_cred.password.length = pass_len;
                    }
                    else
                    {
                        status = kStatus_SHELL_Error;
                    }
                }

                if (kStatus_SHELL_Success == status)
                {
                    flashStatus = wifi_credentials_flash_set(&s_wifi_cred);
                }

                if (SLN_FLASH_FS_OK == flashStatus)
                {
                    SHELL_Printf(s_shellHandle, "Credentials saved\r\n");
                    NVIC_SystemReset();
                }
            }
        }
        else if (strcmp(str, "print") == 0)
        {
            if (s_argc != 2)
            {
                SHELL_Printf(
                    s_shellHandle,
                    "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
            }
            else
            {
                flashStatus = wifi_credentials_flash_get(&wifi_cred);

                if (SLN_FLASH_FS_OK == flashStatus)
                {
                    SHELL_Printf(s_shellHandle, "Wifi name: %s\r\n", wifi_cred.ssid.value);
                    SHELL_Printf(s_shellHandle, "Wifi password: %s\r\n", wifi_cred.password.value);
                    SHELL_Printf(s_shellHandle, "SHELL>> ");
                }
            }
        }
        else if (strcmp(str, "erase") == 0)
        {
            if (s_argc != 2)
            {
                SHELL_Printf(
                    s_shellHandle,
                    "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
            }
            else
            {
                flashStatus = wifi_credentials_flash_reset();

                if (SLN_FLASH_FS_OK == flashStatus)
                {
                    SHELL_Printf(s_shellHandle, "Credentials erased\r\n");
                    NVIC_SystemReset();
                }
            }
        }
        else if (strcmp(str, "status") == 0)
        {
            if (s_argc != 2)
            {
                SHELL_Printf(
                    s_shellHandle,
                    "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
            }
            else
            {
                get_wifi_status();
            }
        }
        else
        {
            SHELL_Printf(
                s_shellHandle,
                "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
        }
    }
}
#endif /* ENABLE_WIFI */

/* mics command */
/****************/
static void sln_mics_cmd_action(void)
{
    char *str;

    if (s_argc > 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else
    {
        if (s_argc == 1)
        {
            if (appAsrShellCommands.micsState == ASR_MICS_OFF)
            {
                SHELL_Printf(s_shellHandle, "Mics set to off.\r\n");
            }
            else if (appAsrShellCommands.micsState == ASR_MICS_ON)
            {
                SHELL_Printf(s_shellHandle, "Mics set to on.\r\n");
            }
        }
        else
        {
            str = s_argv[1];

            if (strcmp(str, "on") == 0)
            {
                appAsrShellCommands.micsState   = ASR_MICS_ON;
                appAsrShellCommands.status = WRITE_READY;
                SHELL_Printf(s_shellHandle, "Setting mics on.\r\n");

                xTaskNotifyFromISR(appTaskHandle, kMicUpdate, eSetBits, NULL);
            }
            else if (strcmp(str, "off") == 0)
            {
                appAsrShellCommands.micsState   = ASR_MICS_OFF;
                appAsrShellCommands.status = WRITE_READY;
                SHELL_Printf(s_shellHandle, "Setting mics off.\r\n");

                if (appAsrShellCommands.asrMode == ASR_MODE_PTT)
                {
                    /* deactivate PTT mode when mics are off */
                    appAsrShellCommands.asrMode &= ~ASR_MODE_PTT;
                    SHELL_Printf(s_shellHandle, "Disabling ASR Push-To-Talk mode.\r\n");
                }

                xTaskNotifyFromISR(appTaskHandle, kMicUpdate, eSetBits, NULL);
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Invalid input.\r\n");
            }
        }
    }
}

static shell_status_t sln_mics_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    strncpy(s_argv[1], argv[1], MAX_ARGV_STR_SIZE);

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, MICS_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_mics_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

#if ENABLE_DSMT_ASR

/* cmdresults command */
/**********************/
static void sln_cmdresults_cmd_action(void)
{
    char *str;

    if (s_argc > 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else
    {
        if (s_argc == 1)
        {
            if (appAsrShellCommands.cmdresults == ASR_CMD_RES_OFF)
            {
                SHELL_Printf(s_shellHandle, "Command detection results printing set to off.\r\n");
            }
            else if (appAsrShellCommands.cmdresults == ASR_CMD_RES_ON)
            {
                SHELL_Printf(s_shellHandle, "Command detection results printing set to on.\r\n");
            }
        }
        else
        {
            str = s_argv[1];

            if (strcmp(str, "on") == 0)
            {
                appAsrShellCommands.cmdresults = ASR_CMD_RES_ON;
                SHELL_Printf(s_shellHandle, "Setting command results printing to on.\r\n");
            }
            else if (strcmp(str, "off") == 0)
            {
                appAsrShellCommands.cmdresults = ASR_CMD_RES_OFF;
                SHELL_Printf(s_shellHandle, "Setting command results printing to off.\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Invalid input.\r\n");
            }
        }
    }
}

static shell_status_t sln_cmdresults_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    strncpy(s_argv[1], argv[1], MAX_ARGV_STR_SIZE);

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, PRINT_CMD_RES_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_cmdresults_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}
#endif /* ENABLE_DSMT_ASR */


/* updateotw command */
/*********************/
static void sln_updateotw_cmd_action(void)
{
    FWUpdate_set_SLN_OTW();

    SHELL_Printf(s_shellHandle, "\r\nReseting the board in OTW update mode\r\n");
    NVIC_SystemReset();
}

static shell_status_t sln_updateotw_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, OTW_UPDATE_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_updateotw_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

/* version command */
/*********************/
static void sln_version_cmd_action(void)
{
    /* Print firmware version and active bank */
    SHELL_Printf(s_shellHandle, "Firmware version: %d.%d.%d, Current Bank: %s, ", APP_MAJ_VER, APP_MIN_VER,
                 APP_BLD_VER, getActiveBank());

    /* Print ASR and its version */
    print_asr_version();

    /* Print AFE and its version */
    uint32_t ver_major, ver_minor, ver_patch;
    SLN_AFE_Get_Version(&ver_major, &ver_minor, &ver_patch);
    SHELL_Printf(s_shellHandle, "AFE: Voiceseeker version %d.%d.%d\r\n", ver_major, ver_minor, ver_patch);
}

static shell_status_t sln_version_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, VERSION_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_version_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

#if ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER

/* usb_aec_align command */
/*********************/
static void sln_usb_aec_align_cmd_action(void)
{
    static bool test_sound_on = false;
    if (test_sound_on)
    {
        SHELL_Printf(s_shellHandle, "Board stopped playing the test sound!\r\n");
        AUDIO_DUMP_AecAlignSoundStop();
        test_sound_on = false;
    }
    else
    {
        SHELL_Printf(s_shellHandle, "Board will start playing a test sound!\r\n");
        AUDIO_DUMP_AecAlignSoundStart();
        test_sound_on = true;
    }
}

static shell_status_t sln_usb_aec_align_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, AUDIO_DUMP_TEST_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_usb_aec_align_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}
#endif /* ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER */

#if SLN_TRACE_CPU_USAGE

/* cpuview command */
/*********************/
static void sln_cpuview_cmd_action(void)
{
    char *cpuLoad = NULL;

    cpuLoad = PERF_GetCPULoad();
    if (cpuLoad != NULL)
    {
        SHELL_Printf(s_shellHandle, "\r\n\r\nCPU usage:\r\n%s\r\n", cpuLoad);
    }
    else
    {
        SHELL_Printf(s_shellHandle, "Could NOT get the CPU usage\r\n");
    }
}

static shell_status_t sln_cpuview_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, TRACE_CPU_USAGE_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_cpuview_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}
#endif /* SLN_TRACE_CPU_USAGE */


/* memview command */
/*******************/
static void sln_memview_cmd_action(void)
{
    /* Print out available bytes in the FreeRTOS heap */
    SHELL_Printf(s_shellHandle, "Available Heap: %d\r\n", xPortGetFreeHeapSize());
}

static shell_status_t sln_memview_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, MEM_VIEW_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_memview_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

/* heapview command */
/********************/
static void sln_heapview_cmd_action(void)
{
    PERF_PrintHeap();
}

static shell_status_t sln_heapview_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, HEAP_VIEW_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_heapview_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

/* stacksview command */
/**********************/
static void sln_stacksview_cmd_action(void)
{
    PERF_PrintStacks();
}

static shell_status_t sln_stacksview_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, STACKS_VIEW_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_stacksview_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}

#if ENABLE_AEC

/* aecmode command */
/**********************/
static void sln_aecmode_cmd_action(void)
{
    char *str;

    if (s_argc > 2)
    {
        SHELL_Printf(
            s_shellHandle,
            "\r\nIncorrect command parameter(s). Enter \"help\" to view a list of available commands.\r\n\r\n");
    }
    else
    {
        if (s_argc == 1)
        {
            if (audio_processing_get_bypass_aec() == false)
            {
                SHELL_Printf(s_shellHandle, "AEC mode set to on.\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "AEC mode set to off.\r\n");
            }
        }
        else
        {
            str = s_argv[1];

            if (strcmp(str, "on") == 0)
            {
                audio_processing_set_bypass_aec(false);
                SHELL_Printf(s_shellHandle, "Setting AEC mode to on.\r\n");
            }
            else if (strcmp(str, "off") == 0)
            {
                audio_processing_set_bypass_aec(true);
                SHELL_Printf(s_shellHandle, "Setting AEC mode to off.\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Invalid input.\r\n");
            }
        }
    }
}

static shell_status_t sln_aecmode_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    s_argc = argc;
    strncpy(s_argv[1], argv[1], MAX_ARGV_STR_SIZE);

#if ENABLE_USB_SHELL
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(s_ShellEventGroup, AEC_MODE_EVT, &xHigherPriorityTaskWoken);
#elif ENABLE_UART_SHELL
    sln_cmdresults_cmd_action();
#endif /* ENABLE_USB_SHELL */

    return kStatus_SHELL_Success;
}
#endif /* ENABLE_AEC */

int log_shell_printf(const char *formatString, ...)
{
    va_list ap;
    char logbuf[configLOGGING_MAX_MESSAGE_LENGTH] = {0};

    va_start(ap, formatString);
    vsnprintf(logbuf, configLOGGING_MAX_MESSAGE_LENGTH, formatString, ap);

    va_end(ap);

    return SHELL_Write(s_shellHandle, logbuf, strlen(logbuf));
}

int sln_shell_init(void)
{
    status_t status = 0;
    serial_manager_config_t serialConfig = {0};
    void *portConfig = NULL;
    serial_port_type_t serialPortType = kSerialPort_None;

    s_ShellEventGroup = xEventGroupCreate();

#if ENABLE_USB_SHELL
    serial_port_usb_cdc_config_t usbCdcConfig = {
        .controllerIndex = (serial_port_usb_cdc_controller_index_t)CONTROLLER_ID,
    };
    portConfig = &usbCdcConfig;
    serialPortType = kSerialPort_UsbCdc;

    static volatile uint8_t usb_clock_initialized = 0;
    if (!usb_clock_initialized)
    {
        usb_clock_initialized = 1;
        USB_DeviceClockInit();
    }

#if ENABLE_USB_AUDIO_DUMP
    USB_DeviceApplicationInit();
#endif /* ENABLE_USB_AUDIO_DUMP */

#elif ENABLE_UART_SHELL
    serial_port_uart_config_t uartConfig = {.clockRate = BOARD_DebugConsoleSrcFreq(),
                                            .baudRate = BOARD_DEBUG_UART_BAUDRATE,
                                            .enableRx = 1,
                                            .enableTx = 1,
                                            .instance = BOARD_DEBUG_UART_INSTANCE};

    portConfig = &uartConfig;
    serialPortType = kSerialPort_Uart;
#endif /* ENABLE_USB_SHELL */

    /* Init Serial Manager */
    serialConfig.type = serialPortType;
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    serialConfig.ringBuffer     = &readRingBuffer[0];
    serialConfig.ringBufferSize = SLN_SERIAL_MANAGER_RECEIVE_BUFFER_LEN;
#endif
    serialConfig.portConfig = portConfig;

    status = SerialManager_Init(s_serialHandle, &serialConfig);
    if (status != kStatus_SerialManager_Success)
    {
        return (int32_t)status;
    }

    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, s_serialHandle, "SHELL>> ");

    /* Add the SLN commands to the commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(reset));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(factoryreset));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(changebank));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(commands));
#ifdef SHELL_SELECTABLE_DEMOS
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(changedemo));
#endif /* SHELL_SELECTABLE_DEMOS */
#ifdef SHELL_SELECTABLE_LANGUAGES
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(changelang));
#endif /* SHELL_SELECTABLE_LANGUAGES */
#if ENABLE_AMPLIFIER
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(volume));
#if ENABLE_STREAMER
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(playprompt));
#endif /* ENABLE_STREAMER */
#endif /* ENABLE_AMPLIFIER */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(asrmode));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(timeout));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(mics));
#if ENABLE_DSMT_ASR
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(cmdresults));
#endif /* ENABLE_DSMT_ASR */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(updateotw));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(version));
#if ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(usb_aec_align));
#endif /* ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER */
#if SLN_TRACE_CPU_USAGE
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(cpuview));
#endif /* SLN_TRACE_CPU_USAGE */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(memview));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(heapview));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(stacksview));
#if ENABLE_WIFI
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(wifi));
#endif /* ENABLE_WIFI */
#if ENABLE_AEC
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(aecmode));
#endif /* ENABLE_AEC */

    return status;
}

void sln_shell_task(void *arg)
{
    SHELL_Printf(s_shellHandle, "Howdy! Type \"help\" to see what this shell can do!\r\n");
    SHELL_Printf(s_shellHandle, "SHELL>> ");

    while (1)
    {
#if ENABLE_UART_SHELL
        SHELL_Task(s_shellHandle);
#else
        volatile EventBits_t shellEvents = 0U;

        shellEvents = xEventGroupWaitBits(s_ShellEventGroup, 0x00FFFFFF, pdTRUE, pdFALSE, portMAX_DELAY);

        if (shellEvents & RESET_EVENT)
        {
            sln_reset_cmd_action();
        }

        if (shellEvents & FACTORY_RESET_EVT)
        {
            sln_factoryreset_cmd_action();
        }

        if (shellEvents & CHANGE_BANK_EVT)
        {
            sln_changebank_cmd_action();
        }

        if (shellEvents & PRINT_COMMANDS_EVT)
        {
            sln_commands_cmd_action();
        }

#ifdef SHELL_SELECTABLE_DEMOS
        if (shellEvents & CHANGE_DEMO_EVT)
        {
            sln_changedemo_cmd_action();
        }
#endif /* SHELL_SELECTABLE_DEMOS */

#ifdef SHELL_SELECTABLE_LANGUAGES
        if (shellEvents & CHANGE_LANG_EVT)
        {
            sln_changelang_cmd_action();
        }
#endif /* SHELL_SELECTABLE_LANGUAGES */

#if ENABLE_AMPLIFIER
        if (shellEvents & SET_VOLUME_EVT)
        {
            sln_volume_cmd_action();
        }
#if ENABLE_STREAMER
        if (shellEvents & PLAY_PROMPT_EVT)
        {
            sln_playprompt_cmd_action();
        }
#endif /* ENABLE_STREAMER */
#endif /* ENABLE_AMPLIFIER */

        if (shellEvents & ASR_MODE_EVT)
        {
            sln_asrmode_cmd_action();
        }

        if (shellEvents & SET_TIMEOUT_EVT)
        {
            sln_timeout_cmd_action();
        }

        if (shellEvents & MICS_EVT)
        {
            sln_mics_cmd_action();
        }

#if ENABLE_DSMT_ASR
        if (shellEvents & PRINT_CMD_RES_EVT)
        {
            sln_cmdresults_cmd_action();
        }
#endif /* ENABLE_DSMT_ASR */

        if (shellEvents & OTW_UPDATE_EVT)
        {
            sln_updateotw_cmd_action();
        }

        if (shellEvents & VERSION_EVT)
        {
            sln_version_cmd_action();
        }

#if ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER
        if (shellEvents & AUDIO_DUMP_TEST_EVT)
        {
            sln_usb_aec_align_cmd_action();
        }
#endif /* ENABLE_USB_AUDIO_DUMP && ENABLE_AMPLIFIER */

#if SLN_TRACE_CPU_USAGE
        if (shellEvents & TRACE_CPU_USAGE_EVT)
        {
            sln_cpuview_cmd_action();
        }
#endif /* SLN_TRACE_CPU_USAGE */

        if (shellEvents & MEM_VIEW_EVT)
        {
            sln_memview_cmd_action();
        }

        if (shellEvents & HEAP_VIEW_EVT)
        {
            sln_heapview_cmd_action();
        }

        if (shellEvents & STACKS_VIEW_EVT)
        {
            sln_stacksview_cmd_action();
        }

#if ENABLE_WIFI
        if (shellEvents & WIFI_EVT)
        {
            sln_wifi_action();
        }
#endif /* ENABLE_WIFI */

#if ENABLE_AEC
        if (shellEvents & AEC_MODE_EVT)
        {
            sln_aecmode_cmd_action();
        }
#endif /* ENABLE_AEC */

#endif /* ENABLE_UART_SHELL */
    }
}

void sln_shell_trace_malloc(void *ptr, size_t size)
{
    if (s_shellHandle)
    {
        if (s_heap_trace.enable)
        {
            if (size >= s_heap_trace.threshold)
            {
                SHELL_Printf(s_shellHandle, "[TRACE] Allocated %d bytes to 0x%X\r\n", size, (int)ptr);
                SHELL_Printf(s_shellHandle, "SHELL>> ");
            }
        }
    }
}

void sln_shell_trace_free(void *ptr, size_t size)
{
    if (s_shellHandle)
    {
        if (s_heap_trace.enable)
        {
            if (size >= s_heap_trace.threshold)
            {
                SHELL_Printf(s_shellHandle, "[TRACE] De-allocated %d bytes from 0x%X\r\n", size, (int)ptr);
                SHELL_Printf(s_shellHandle, "SHELL>> ");
            }
        }
    }
}
#if !(SDK_DEBUGCONSOLE)
int DbgConsole_Printf(const char *formatString, ...)
{
    va_list ap;
    char logbuf[configLOGGING_MAX_MESSAGE_LENGTH] = {0};

    va_start(ap, formatString);
    vsnprintf(logbuf, configLOGGING_MAX_MESSAGE_LENGTH, formatString, ap);

    va_end(ap);

    return 0;
}
#endif

#endif /* ENABLE_SHELL */
