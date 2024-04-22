/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_NXP_OOBE
#if ENABLE_ASR_VIT || ENABLE_ASR_DSMT

#include "FreeRTOS.h"
#include "task.h"

#include "stdint.h"
#include "fsl_common.h"
#include "FreeRTOSConfig.h"
#include "sln_amplifier.h"
#include "sln_local_voice_common.h"
#include "sln_rgb_led_driver.h"
#include "local_sounds_task.h"
#include "sln_flash_files.h"

#include "demo_actions.h"
#include "IndexCommands.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern app_asr_shell_commands_t appAsrShellCommands;
extern oob_demo_control_t oob_demo_control;
extern TaskHandle_t appTaskHandle;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void APP_LAYER_LedCommandDetected(void)
{
    RGB_LED_SetColor(LED_COLOR_OFF);
    RGB_LED_SetColor(LED_COLOR_GREEN);
    vTaskDelay(200);
    RGB_LED_SetColor(LED_COLOR_OFF);
    if ((appAsrShellCommands.asrMode == ASR_MODE_WW_AND_MUL_CMD) || (appAsrShellCommands.asrMode == ASR_MODE_CMD_ONLY))
    {
        vTaskDelay(200);
        RGB_LED_SetColor(LED_COLOR_BLUE);
    }
}

static void APP_LAYER_LedListening(void)
{
    RGB_LED_SetColor(LED_COLOR_OFF);
    RGB_LED_SetColor(LED_COLOR_BLUE);
    if (appAsrShellCommands.asrMode == ASR_MODE_WW_ONLY)
    {
        vTaskDelay(500);
        RGB_LED_SetColor(LED_COLOR_OFF);
    }
}

static void APP_LAYER_LedTimeout(void)
{
    RGB_LED_SetColor(LED_COLOR_OFF);
    RGB_LED_SetColor(LED_COLOR_PURPLE);
    vTaskDelay(200);
    RGB_LED_SetColor(LED_COLOR_OFF);
}

static void APP_LAYER_LedError(void)
{
    RGB_LED_SetColor(LED_COLOR_OFF);
    RGB_LED_SetColor(LED_COLOR_RED);
}

static void APP_LAYER_LedChangeDemoCommand(void)
{
    RGB_LED_SetColor(LED_COLOR_OFF);
    RGB_LED_SetColor(LED_COLOR_ORANGE);
}

#if ENABLE_VIT_ASR
static void APP_LAYER_LedChangeLanguageCommand(void)
{
    RGB_LED_SetColor(LED_COLOR_OFF);
    RGB_LED_SetColor(LED_COLOR_YELLOW);
}
#endif /* ENABLE_VIT_ASR */

static void APP_LAYER_LedChangeDemoAction(void)
{
    RGB_LED_SetColor(LED_COLOR_OFF);
    vTaskDelay(200);
    RGB_LED_SetColor(LED_COLOR_ORANGE);
    vTaskDelay(200);
    RGB_LED_SetColor(LED_COLOR_OFF);
}

#if ENABLE_VIT_ASR
static void APP_LAYER_LedChangeLanguageAction(void)
{
    RGB_LED_SetColor(LED_COLOR_OFF);
    vTaskDelay(200);
    RGB_LED_SetColor(LED_COLOR_YELLOW);
    vTaskDelay(200);
    RGB_LED_SetColor(LED_COLOR_OFF);
}
#endif /* ENABLE_VIT_ASR */

#if ENABLE_STREAMER
static status_t APP_LAYER_PlayAudioFromFileSystem(char *filename)
{
    status_t status = kStatus_Success;

    /* Make sure that speaker is not currently playing another audio. */
    while (LOCAL_SOUNDS_isPlaying())
    {
        vTaskDelay(10);
    }

    LOCAL_SOUNDS_PlayAudioFile(filename, appAsrShellCommands.volume);

    return status;
}
#endif /* ENABLE_STREAMER */

static void APP_LAYER_ProcessChangeDemoCommand(void)
{
#if ENABLE_DSMT_ASR
    appAsrShellCommands.demo   = ASR_CMD_CHANGE_DEMO;
#endif /* ENABLE_DSMT_ASR */
    oob_demo_control.skipWW = 1;
    oob_demo_control.changeDemoFlow = 1;

    APP_LAYER_LedChangeDemoCommand();
}

#if ENABLE_VIT_ASR
static void APP_LAYER_ProcessChangeLanguageCommand(void)
{
    oob_demo_control.skipWW = 1;
    oob_demo_control.changeLanguageFlow = 1;

    APP_LAYER_LedChangeLanguageCommand();
}

bool APP_LAYER_FilterVitDetection(unsigned short commandId, asr_inference_t activeDemo)
{
    bool filterDetection = false;

    switch (activeDemo)
    {
        case ASR_CMD_ELEVATOR:
        {
            switch (commandId)
            {
                case kElevator_FirstFloor:
                case kElevator_SecondFloor:
                case kElevator_ThirdFloor:
                case kElevator_FourthFloor:
                case kElevator_FifthFloor:
                case kElevator_MainLobby:
                case kElevator_GroundFloor:
                case kElevator_BasementFloor:
                case kElevator_OpenDoor:
                case kElevator_CloseDoor:
                case kElevator_ChangeDemo:
                case kElevator_ChangeLanguage:
                {
                    if (oob_demo_control.changeLanguageFlow ||
                            oob_demo_control.changeDemoFlow)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                case kElevator_Elevator:
                case kElevator_WashingMachine:
                case kElevator_SmartHome:
                {
                    if (oob_demo_control.changeDemoFlow == 0)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                case kElevator_English:
                case kElevator_French:
                case kElevator_German:
                case kElevator_Chinese:
                {
                    if (oob_demo_control.changeLanguageFlow == 0)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                default:
                {
                    configPRINTF(("Filter error, unsupported elevator command %d\r\n", commandId));
                    filterDetection = true;
                }
            }

            break;
        }

        case ASR_CMD_WASHING_MACHINE:
        {
            switch (commandId)
            {
                case kWashingMachine_Delicate:
                case kWashingMachine_Normal:
                case kWashingMachine_HeavyDuty:
                case kWashingMachine_Whites:
                case kWashingMachine_Start:
                case kWashingMachine_Cancel:
                case kWashingMachine_ChangeDemo:
                case kWashingMachine_ChangeLanguage:
                {
                    if (oob_demo_control.changeLanguageFlow ||
                        oob_demo_control.changeDemoFlow)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                case kWashingMachine_Elevator:
                case kWashingMachine_WashingMachine:
                case kWashingMachine_SmartHome:
                {
                    if (oob_demo_control.changeDemoFlow == 0)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                case kWashingMachine_English:
                case kWashingMachine_French:
                case kWashingMachine_German:
                case kWashingMachine_Chinese:
                {
                    if (oob_demo_control.changeLanguageFlow == 0)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                default:
                {
                    configPRINTF(("Filter error, unsupported washing machine command %d\r\n", commandId));
                    filterDetection = true;
                }
            }

            break;
        }

        case ASR_CMD_SMART_HOME:
        {
            switch (commandId)
            {
                case kSmartHome_TurnOnTheLights:
                case kSmartHome_TurnOffTheLights:
                case kSmartHome_TemperatureHigher:
                case kSmartHome_TemperatureLower:
                case kSmartHome_OpenTheWindow:
                case kSmartHome_CloseTheWindow:
                case kSmartHome_MakeItBrighter:
                case kSmartHome_MakeItDarker:
                case kSmartHome_ChangeDemo:
                case kSmartHome_ChangeLanguage:
                {
                    if (oob_demo_control.changeLanguageFlow ||
                        oob_demo_control.changeDemoFlow)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                case kSmartHome_Elevator:
                case kSmartHome_WashingMachine:
                case kSmartHome_SmartHome:
                {
                    if (oob_demo_control.changeDemoFlow == 0)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                case kSmartHome_English:
                case kSmartHome_French:
                case kSmartHome_German:
                case kSmartHome_Chinese:
                {
                    if (oob_demo_control.changeLanguageFlow == 0)
                    {
                        filterDetection = true;
                    }
                    break;
                }
                default:
                {
                    configPRINTF(("Filter error, unsupported smart home command %d\r\n", commandId));
                    filterDetection = true;
                }
            }

            break;
        }
        default:
        {
            configPRINTF(("Filter error, unsupported demo id %d\r\n", activeDemo));
            filterDetection = true;
        }
    }

    return filterDetection;
}
#endif /* ENABLE_VIT_ASR */

static status_t APP_LAYER_SwitchToNewDemo(asr_inference_t newDemo)
{
    status_t status = kStatus_Success;

    if (oob_demo_control.changeDemoFlow)
    {
        switch (newDemo)
        {
            case ASR_CMD_ELEVATOR:
            {
                configPRINTF(("Changing demo to Elevator\r\n"));
                break;
            }
            case ASR_CMD_WASHING_MACHINE:
            {
                configPRINTF(("Changing demo to Washing Machine\r\n"));
                break;
            }
            case ASR_CMD_SMART_HOME:
            {
                configPRINTF(("Changing demo to Smart Home\r\n"));
                break;
            }
            default:
            {
                configPRINTF(("Error, unsupported new demo %d\r\n", newDemo));
                newDemo = DEFAULT_ASR_CMD_DEMO;
                status  = kStatus_InvalidArgument;
            }
        }

        appAsrShellCommands.demo   = newDemo;
        appAsrShellCommands.status = WRITE_READY;
        appAsrShellCommands.asrCfg = ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED;
        oob_demo_control.changeDemoFlow = 0;

        APP_LAYER_LedChangeDemoAction();
    }

    return status;
}

/*******************************************************************************
 * API
 ******************************************************************************/

#if ENABLE_VIT_ASR
static status_t APP_LAYER_SwitchToNewLanguage(asr_language_t newLanguage)
{
    status_t status = kStatus_Success;

    if (oob_demo_control.changeLanguageFlow)
    {
        switch (newLanguage)
        {
            case ASR_ENGLISH:
            {
                configPRINTF(("Changing language to English\r\n"));
                break;
            }
            case ASR_FRENCH:
            {
                configPRINTF(("Changing language to French\r\n"));
                break;
            }
            case ASR_GERMAN:
            {
                configPRINTF(("Changing language to German\r\n"));
                break;
            }
            case ASR_CHINESE:
            {
                configPRINTF(("Changing language to Chinese\r\n"));
                break;
            }
            default:
            {
                configPRINTF(("Error, unsupported language %d\r\n", newLanguage));
                newLanguage = DEFAULT_ASR_LANGUAGE;
                status  = kStatus_InvalidArgument;
            }
        }

        appAsrShellCommands.activeLanguage   = newLanguage;
        appAsrShellCommands.status = WRITE_READY;
        appAsrShellCommands.asrCfg = ASR_CFG_DEMO_LANGUAGE_CHANGED;
        oob_demo_control.changeLanguageFlow = 0;

        oob_demo_control.language   = newLanguage;

        APP_LAYER_LedChangeLanguageAction();
    }

    return status;
}
#endif

status_t APP_LAYER_ProcessWakeWord(oob_demo_control_t *commandConfig)
{
    status_t status = kStatus_Success;

#if ENABLE_STREAMER
    APP_LAYER_PlayAudioFromFileSystem(AUDIO_WW_DETECTED);
#endif /* ENABLE_STREAMER */

    if (status == kStatus_Success)
    {
        APP_LAYER_LedListening();
    }
    else
    {
        APP_LAYER_LedError();
    }

    return status;
}

status_t APP_LAYER_ProcessVoiceCommand(oob_demo_control_t *commandConfig)
{
    status_t status = kStatus_Success;
    uint16_t action = 0;

    if (commandConfig != NULL)
    {
        action = get_action_from_keyword(commandConfig->language, commandConfig->commandSet, commandConfig->commandId);

#if ENABLE_STREAMER
        void *prompt = NULL;

        /* Play prompt for the active language */
        prompt = get_prompt_from_keyword(commandConfig->language, commandConfig->commandSet, commandConfig->commandId);

        if (NULL != prompt)
        {
            APP_LAYER_PlayAudioFromFileSystem((char*)prompt);
        }
#endif /* ENABLE_STREAMER */

        switch (commandConfig->commandSet)
        {
#if ENABLE_DSMT_ASR
            case ASR_CMD_CHANGE_DEMO:
            {
                switch (action)
                {
                    case kChangeDemo_Elevator:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_ELEVATOR);
                        break;
                    }
                    case kChangeDemo_WashingMachine:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_WASHING_MACHINE);
                        break;
                    }
                    case kChangeDemo_SmartHome:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_SMART_HOME);
                        break;
                    }
                    default:
                    {
                        configPRINTF(("Error: Unknown demo %d\r\n", commandConfig->commandId));
                        APP_LAYER_SwitchToNewDemo(ASR_CMD_ELEVATOR);
                        status = kStatus_Fail;
                    }
                }

                break;
            }
#endif /* ENABLE_DSMT_ASR */

            case ASR_CMD_ELEVATOR:
            {
                switch (action)
                {
                    case kElevator_ChangeDemo:
                    {
                        APP_LAYER_ProcessChangeDemoCommand();
                        break;
                    }
#if ENABLE_VIT_ASR
                    case kElevator_ChangeLanguage:
                    {
                        APP_LAYER_ProcessChangeLanguageCommand();
                        break;
                    }
                    case kElevator_Elevator:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_ELEVATOR);
                        break;
                    }
                    case kElevator_WashingMachine:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_WASHING_MACHINE);
                        break;
                    }
                    case kElevator_SmartHome:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_SMART_HOME);
                        break;
                    }
                    case kElevator_English:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_ENGLISH);
                        break;
                    }
                    case kElevator_French:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_FRENCH);
                        break;
                    }
                    case kElevator_German:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_GERMAN);
                        break;
                    }
                    case kElevator_Chinese:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_CHINESE);
                        break;
                    }
#endif /* ENABLE_VIT_ASR */
                }

                break;
            }

            case ASR_CMD_WASHING_MACHINE:
            {
                switch (action)
                {
                    case kWashingMachine_ChangeDemo:
                    {
                        APP_LAYER_ProcessChangeDemoCommand();
                        break;
                    }
#if ENABLE_VIT_ASR
                    case kWashingMachine_ChangeLanguage:
                    {
                        APP_LAYER_ProcessChangeLanguageCommand();
                        break;
                    }
                    case kWashingMachine_Elevator:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_ELEVATOR);
                        break;
                    }
                    case kWashingMachine_WashingMachine:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_WASHING_MACHINE);
                        break;
                    }
                    case kWashingMachine_SmartHome:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_SMART_HOME);
                        break;
                    }
                    case kWashingMachine_English:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_ENGLISH);
                        break;
                    }
                    case kWashingMachine_French:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_FRENCH);
                        break;
                    }
                    case kWashingMachine_German:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_GERMAN);
                        break;
                    }
                    case kWashingMachine_Chinese:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_CHINESE);
                        break;
                    }
#endif /* ENABLE_VIT_ASR */
                }

                break;
            }

            case ASR_CMD_SMART_HOME:
            {
                switch (action)
                {
                    case kSmartHome_ChangeDemo:
                    {
                        APP_LAYER_ProcessChangeDemoCommand();
                        break;
                    }
#if ENABLE_VIT_ASR
                    case kSmartHome_ChangeLanguage:
                    {
                        APP_LAYER_ProcessChangeLanguageCommand();
                        break;
                    }
                    case kSmartHome_Elevator:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_ELEVATOR);
                        break;
                    }
                    case kSmartHome_WashingMachine:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_WASHING_MACHINE);
                        break;
                    }
                    case kSmartHome_SmartHome:
                    {
                        status = APP_LAYER_SwitchToNewDemo(ASR_CMD_SMART_HOME);
                        break;
                    }
                    case kSmartHome_English:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_ENGLISH);
                        break;
                    }
                    case kSmartHome_French:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_FRENCH);
                        break;
                    }
                    case kSmartHome_German:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_GERMAN);
                        break;
                    }
                    case kSmartHome_Chinese:
                    {
                        status = APP_LAYER_SwitchToNewLanguage(ASR_CHINESE);
                        break;
                    }
#endif /* ENABLE_VIT_ASR */
                }

                break;
            }

            default:
            {
                configPRINTF(("Error: Unsupported command set %d\r\n", commandConfig->commandSet));
                status = kStatus_Fail;
                break;
            }
        }
    }
    else
    {
        configPRINTF(("Error, commandConfig is NULL\r\n"));
        status = kStatus_InvalidArgument;
    }

    if (status == kStatus_Success)
    {
        if (!(oob_demo_control.changeDemoFlow     ||
              oob_demo_control.changeLanguageFlow ||
              appAsrShellCommands.asrCfg == ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED ||
              appAsrShellCommands.asrCfg == ASR_CFG_DEMO_LANGUAGE_CHANGED))
        {
            APP_LAYER_LedCommandDetected();
        }
    }
    else
    {
        APP_LAYER_LedError();
    }

    return status;
}

status_t APP_LAYER_ProcessTimeout(oob_demo_control_t *commandConfig)
{
    status_t status = kStatus_Success;

    if (commandConfig != NULL)
    {
        /* appAsrShellCommands.demo contains current demo. */
        switch (appAsrShellCommands.demo)
        {
            case ASR_CMD_CHANGE_DEMO:
            {
                /* commandConfig->commandSet contains demo corresponded to the latest detected voice command. */
                status = APP_LAYER_SwitchToNewDemo(commandConfig->commandSet);

                break;
            }

            case ASR_CMD_ELEVATOR:
            case ASR_CMD_WASHING_MACHINE:
            case ASR_CMD_SMART_HOME:
            {
                if (oob_demo_control.changeDemoFlow ||
                    oob_demo_control.changeLanguageFlow)
                {
                    /* appTask plays the demo prompt when notified that the model changed
                     * reuse that behavior here to play the demo prompt after a timeout
                     * in this way the user will be reminded the demo and the language that are active */
                    xTaskNotify(appTaskHandle, kAsrModelChanged, eSetBits);
                }
                break;
            }

            default:
            {
                configPRINTF(("Error: Unsupported command set %d\r\n", commandConfig->commandSet));
                status = kStatus_Fail;
                break;
            }
        }
    }
    else
    {
        configPRINTF(("Error, commandConfig is NULL\r\n"));
        status = kStatus_InvalidArgument;
    }

    if (oob_demo_control.changeLanguageFlow)
    {
        oob_demo_control.changeLanguageFlow = 0;
    }

    if (oob_demo_control.changeDemoFlow)
    {
        oob_demo_control.changeDemoFlow = 0;
    }

    if (oob_demo_control.skipWW)
    {
        oob_demo_control.skipWW = 0;
    }

    if (status == kStatus_Success)
    {
#if ENABLE_STREAMER
        APP_LAYER_PlayAudioFromFileSystem(AUDIO_TONE_TIMEOUT);
#endif /* ENABLE_STREAMER */
        APP_LAYER_LedTimeout();
    }
    else
    {
        APP_LAYER_LedError();
    }

    return status;
}

void APP_LAYER_SwitchToNextDemo(void)
{
    if (!(appAsrShellCommands.asrCfg & ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED) &&
        !(appAsrShellCommands.asrCfg & ASR_CFG_DEMO_LANGUAGE_CHANGED) &&
        !oob_demo_control.changeDemoFlow &&
        !oob_demo_control.changeLanguageFlow)
    {
        switch (appAsrShellCommands.demo)
        {
            case ASR_CMD_CHANGE_DEMO:
                appAsrShellCommands.demo = DEFAULT_ASR_CMD_DEMO;
                break;
            case ASR_CMD_ELEVATOR:
                appAsrShellCommands.demo = ASR_CMD_WASHING_MACHINE;
                break;
            case ASR_CMD_WASHING_MACHINE:
                appAsrShellCommands.demo = ASR_CMD_SMART_HOME;
                break;
            case ASR_CMD_SMART_HOME:
                appAsrShellCommands.demo = ASR_CMD_ELEVATOR;
                break;
            default:
                configPRINTF(("Error: unsupported demo %d\r\n", appAsrShellCommands.demo));
                appAsrShellCommands.demo = DEFAULT_ASR_CMD_DEMO;
                break;
        }

        appAsrShellCommands.status = WRITE_READY;
        appAsrShellCommands.asrCfg |= ASR_CFG_CMD_INFERENCE_ENGINE_CHANGED;

        /* In case no voice control interaction with the board yet, just use what we have saved in flash */
        if (UNDEFINED_LANGUAGE == oob_demo_control.language)
        {
            oob_demo_control.language   = appAsrShellCommands.activeLanguage;
        }
    }
}

void APP_LAYER_HandleFirstBoardBoot(void)
{
    if ((appAsrShellCommands.demo == BOOT_ASR_CMD_DEMO) &&
        (BOOT_ASR_CMD_DEMO != DEFAULT_ASR_CMD_DEMO))
    {
#if ENABLE_VIT_ASR
#if ENABLE_STREAMER
        APP_LAYER_PlayAudioFromFileSystem(AUDIO_DEMO_NAME_TEST_EN);
#endif /* ENABLE_STREAMER */
        appAsrShellCommands.demo = DEFAULT_ASR_CMD_DEMO;
        appAsrShellCommands.status = WRITE_READY;

        /* notify app task to allow it to save
         * the demo type in flash */
        xTaskNotify(appTaskHandle, kDefault, eSetBits);
#endif /* ENABLE_VIT_ASR */

        oob_demo_control.skipWW         = 1;
        oob_demo_control.changeDemoFlow = 1;
    }
}

#endif /* ENABLE_ASR_VIT || ENABLE_ASR_DSMT */
#endif /* ENABLE_NXP_OOBE */
