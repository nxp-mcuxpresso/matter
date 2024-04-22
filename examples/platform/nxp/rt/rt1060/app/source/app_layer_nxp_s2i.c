/*
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_NXP_OOBE
#if ENABLE_S2I_ASR

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

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

#define INTENT_NAME(x, i)   (x->pIntent[i])
#define TAG_NAME(x, i)      (x->pSlot_Tag[i])
#define TAG_VALUE_CNT(x, i) (x->Slot_Tag_Value_count[i] - 1)
#define TAG_VALUE(x, i, j)  (x->pSlot_Tag_Value[(i * MAX_NUMBER_WORDS_PER_SLOT_TAG_VALUE) + \
                             TAG_VALUE_CNT(x, i) - j])

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern QueueHandle_t xMatterActionsQueue;

extern app_asr_shell_commands_t appAsrShellCommands;
extern oob_demo_control_t oob_demo_control;
extern TaskHandle_t appTaskHandle;
extern VIT_Intent_st SpeechIntent;

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
    RGB_LED_SetHSL();
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
    RGB_LED_SetHSL();
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

static void APP_LAYER_VIT_ParseHomeIntent(void)
{
    VIT_Intent_st *pSpeechIntent = NULL;
    PL_INT16 tagsCnt;
    char *prompt = NULL;

    pSpeechIntent = &SpeechIntent;
    tagsCnt = pSpeechIntent->Slot_Tag_count - 1;

    /* LIGHT BRIGHTNESS */
    if (!strcmp("LightBrightness", INTENT_NAME(pSpeechIntent, tagsCnt)))
    {
        if (pSpeechIntent->Slot_Tag_count == 1)
        {
            if (!strcmp("level", TAG_NAME(pSpeechIntent, tagsCnt)))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.location = "all";
                valueToSend.action = kMatterActionBrightnessChange;

                if (!strcmp("maximum", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("max", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "max";
                    valueToSend.value = (void *)99;
                    prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_HOME;
                }
                else if (!strcmp("minimum", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("min", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "min";
                    valueToSend.value = (void *)2;
                    prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_HOME;
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
        else if (pSpeechIntent->Slot_Tag_count == 2)
        {
            if (!strcmp("level", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                !strcmp("location", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBrightnessChange;

                if (!strcmp("maximum", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("max", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "max";
                    valueToSend.value = (void *)99;

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("minimum", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("min", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "min";
                    valueToSend.value = (void *)2;

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
            else if (!strcmp("location", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                     !strcmp("level", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBrightnessChange;

                if (!strcmp("maximum", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)) ||
                    !strcmp("max", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.command = "max";
                    valueToSend.value = (void *)99;

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MAXIMUM_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("minimum", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)) ||
                         !strcmp("min", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.command = "min";
                    valueToSend.value = (void *)2;

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_SET_BRIGHTNESS_TO_MINIMUM_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
            else if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                     !strcmp("level", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBrightnessChange;
                valueToSend.location = "all";

                if (!strcmp("increase", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("up", TAG_VALUE(pSpeechIntent, tagsCnt, 1)) ||
                    !strcmp("raise", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("more", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("brighter", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "increase";

                    if (!strcmp("ten", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)10;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_HOME;
                    }
                    else if (!strcmp("twenty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)20;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_HOME;
                    }
                    else if (!strcmp("thirty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)30;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_HOME;
                    }
                    else if (!strcmp("forty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)40;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_HOME;
                    }
                    else if (!strcmp("fifty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)50;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_HOME;
                    }
                    else if (!strcmp("sixty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)60;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_HOME;
                    }
                    else if (!strcmp("seventy", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)70;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_HOME;
                    }
                    else if (!strcmp("eighty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)80;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_HOME;
                    }
                    else if (!strcmp("ninety", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)90;
                        prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_HOME;
                    }
                }
                else if (!strcmp("decrease", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("down", TAG_VALUE(pSpeechIntent, tagsCnt, 1)) ||
                         !strcmp("reduce", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("lower", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("darker", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("dim", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "decrease";

                    if (!strcmp("ten", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)10;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_HOME;
                    }
                    else if (!strcmp("twenty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)20;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_HOME;
                    }
                    else if (!strcmp("thirty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)30;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_HOME;
                    }
                    else if (!strcmp("forty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)40;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_HOME;
                    }
                    else if (!strcmp("fifty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)50;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_HOME;
                    }
                    else if (!strcmp("sixty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)60;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_HOME;
                    }
                    else if (!strcmp("seventy", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)70;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_HOME;
                    }
                    else if (!strcmp("eighty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)80;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_HOME;
                    }
                    else if (!strcmp("ninety", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)90;
                        prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
        else if (pSpeechIntent->Slot_Tag_count == 3)
        {
            if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                !strcmp("level", TAG_NAME(pSpeechIntent, (tagsCnt - 1))) &&
                !strcmp("location", TAG_NAME(pSpeechIntent, (tagsCnt - 2))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBrightnessChange;

                if (!strcmp("increase", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("up", TAG_VALUE(pSpeechIntent, tagsCnt, 1)) ||
                    !strcmp("raise", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("more", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("brighter", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "increase";

                    if (!strcmp("ten", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)10;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("twenty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)20;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("thirty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)30;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("forty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)40;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("fifty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)50;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("sixty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)60;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("seventy", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)70;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("eighty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)80;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("ninety", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)90;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
                }
                else if (!strcmp("decrease", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("down", TAG_VALUE(pSpeechIntent, tagsCnt, 1)) ||
                         !strcmp("reduce", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("lower", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("darker", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("dim", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "decrease";

                    if (!strcmp("ten", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)10;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("twenty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)20;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("thirty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)30;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("forty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)40;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("fifty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)50;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("sixty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)60;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("seventy", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)70;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("eighty", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)80;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("ninety", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)90;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
                }
            }
            else if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                     !strcmp("location", TAG_NAME(pSpeechIntent, (tagsCnt - 1))) &&
                     !strcmp("level", TAG_NAME(pSpeechIntent, (tagsCnt - 2))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBrightnessChange;

                if (!strcmp("increase", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("up", TAG_VALUE(pSpeechIntent, tagsCnt, 1)) ||
                    !strcmp("raise", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("more", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("brighter", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "increase";

                    if (!strcmp("ten", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)10;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_10_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("twenty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)20;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_20_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("thirty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)30;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_30_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("forty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)40;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_40_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("fifty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)50;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_50_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("sixty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)60;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_60_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("seventy", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)70;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_70_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("eighty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)80;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_80_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("ninety", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)90;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_INCREASE_BRIGHTNESS_BY_90_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                }
                else if (!strcmp("decrease", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("down", TAG_VALUE(pSpeechIntent, tagsCnt, 1)) ||
                         !strcmp("reduce", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("lower", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("darker", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("dim", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "decrease";

                    if (!strcmp("ten", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)10;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_10_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("twenty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)20;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_20_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("thirty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)30;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_30_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("forty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)40;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_40_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("fifty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)50;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_50_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("sixty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)60;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_60_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("seventy", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)70;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_70_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("eighty", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)80;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_80_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                    else if (!strcmp("ninety", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                    {
                        valueToSend.value = (void *)90;

                        if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "kitchen";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_IN_THE_KITCHEN_HOME;
                        }
                        else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bedroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_IN_THE_BEDROOM_HOME;
                        }
                        else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "bathroom";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_IN_THE_BATHROOM_HOME;
                        }
                        else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                        {
                            valueToSend.location = "living";
                            prompt = AUDIO_OK_DECREASE_BRIGHTNESS_BY_90_IN_THE_LIVING_ROOM_HOME;
                        }
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
    }
    /* LIGHT COLOR */
    else if (!strcmp("LightColor", INTENT_NAME(pSpeechIntent, tagsCnt)))
    {
        if (pSpeechIntent->Slot_Tag_count == 1)
        {
            if (!strcmp("color", TAG_NAME(pSpeechIntent, tagsCnt)))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.location = "all";
                valueToSend.action = kMatterActionColorChange;

                if (!strcmp("blue", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "blue";
                    prompt = AUDIO_OK_BLUE_LIGHTS_HOME;
                }
                else if (!strcmp("red", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "red";
                    prompt = AUDIO_OK_RED_LIGHTS_HOME;
                }
                else if (!strcmp("pink", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "pink";
                    prompt = AUDIO_OK_PINK_LIGHTS_HOME;
                }
                else if (!strcmp("green", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "green";
                    prompt = AUDIO_OK_GREEN_LIGHTS_HOME;
                }
                else if (!strcmp("purple", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "purple";
                    prompt = AUDIO_OK_PURPLE_LIGHTS_HOME;
                }
                else if (!strcmp("yellow", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "yellow";
                    prompt = AUDIO_OK_YELLOW_LIGHTS_HOME;
                }
                else if (!strcmp("orange", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "orange";
                    prompt = AUDIO_OK_ORANGE_LIGHTS_HOME;
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
        else if (pSpeechIntent->Slot_Tag_count == 2)
        {
            if (!strcmp("color", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                !strcmp("location", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionColorChange;

                if (!strcmp("blue", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "blue";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_BLUE_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_BLUE_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_BLUE_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_BLUE_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("red", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "red";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_RED_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_RED_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_RED_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_RED_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("pink", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "pink";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_PINK_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_PINK_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_PINK_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_PINK_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("green", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "green";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_GREEN_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_GREEN_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_GREEN_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_GREEN_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("purple", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "purple";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_PURPLE_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_PURPLE_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_PURPLE_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_PURPLE_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("yellow", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "yellow";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_YELLOW_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_YELLOW_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_YELLOW_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_YELLOW_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("orange", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.value = "orange";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_ORANGE_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_ORANGE_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_ORANGE_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_ORANGE_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
            if (!strcmp("location", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                !strcmp("color", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionColorChange;

                if (!strcmp("blue", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.value = "blue";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_BLUE_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_BLUE_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_BLUE_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_BLUE_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("red", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.value = "red";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_RED_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_RED_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_RED_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_RED_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("pink", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.value = "pink";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_PINK_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_PINK_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_PINK_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_PINK_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("green", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.value = "green";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_GREEN_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_GREEN_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_GREEN_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_GREEN_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("purple", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.value = "purple";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_PURPLE_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_PURPLE_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_PURPLE_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_PURPLE_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("yellow", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.value = "yellow";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_YELLOW_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_YELLOW_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_YELLOW_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_YELLOW_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("orange", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.value = "orange";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_ORANGE_LIGHTS_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_ORANGE_LIGHTS_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_ORANGE_LIGHTS_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_ORANGE_LIGHTS_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
    }
    /* LIGHT POWER */
    else if (!strcmp("LightPower", INTENT_NAME(pSpeechIntent, tagsCnt)))
    {
        if (pSpeechIntent->Slot_Tag_count == 1)
        {
            if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.location = "all";
                valueToSend.action = kMatterActionOnOff;

                if (!strcmp("on", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "on";
                    prompt = AUDIO_OK_LIGHTS_ON_HOME;
                }
                else if (!strcmp("off", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "off";
                    prompt = AUDIO_OK_LIGHTS_OFF_HOME;
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
        else if (pSpeechIntent->Slot_Tag_count == 2)
        {
            if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                !strcmp("location", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionOnOff;

                if (!strcmp("on", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "on";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_LIGHTS_ON_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_LIGHTS_ON_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_LIGHTS_ON_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_LIGHTS_ON_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("off", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "off";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_LIGHTS_OFF_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_LIGHTS_OFF_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_LIGHTS_OFF_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_LIGHTS_OFF_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
            else if (!strcmp("location", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                     !strcmp("action", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionOnOff;

                if (!strcmp("on", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.command = "on";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_LIGHTS_ON_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_LIGHTS_ON_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_LIGHTS_ON_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_LIGHTS_ON_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                else if (!strcmp("off", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                {
                    valueToSend.command = "off";

                    if (!strcmp("kitchen", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "kitchen";
                        prompt = AUDIO_OK_LIGHTS_OFF_IN_THE_KITCHEN_HOME;
                    }
                    else if (!strcmp("bedroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bedroom";
                        prompt = AUDIO_OK_LIGHTS_OFF_IN_THE_BEDROOM_HOME;
                    }
                    else if (!strcmp("bathroom", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "bathroom";
                        prompt = AUDIO_OK_LIGHTS_OFF_IN_THE_BATHROOM_HOME;
                    }
                    else if (!strcmp("living", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                    {
                        valueToSend.location = "living";
                        prompt = AUDIO_OK_LIGHTS_OFF_IN_THE_LIVING_ROOM_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
    }
    /* SHADES */
    else if (!strcmp("Shades", INTENT_NAME(pSpeechIntent, tagsCnt)))
    {
        if (pSpeechIntent->Slot_Tag_count == 1)
        {
            if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBlindsControl;

                if (!strcmp("open", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("up", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("lift", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("raise", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "lift";
                    valueToSend.value = (void *)100;
                    valueToSend.location = "central";
                    prompt = AUDIO_OK_RAISE_WINDOW_SHADES_HOME;
                }
                else if (!strcmp("down", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("lower", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("close", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("shut", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "close";
                    valueToSend.value = (void *)0;
                    valueToSend.location = "central";
                    prompt = AUDIO_OK_LOWER_WINDOW_SHADES_HOME;
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
        else if (pSpeechIntent->Slot_Tag_count == 2)
        {
            if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                !strcmp("side", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBlindsControl;

                if (!strcmp("open", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("up", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("lift", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("raise", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    if (!strcmp("left", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        prompt = AUDIO_OK_RAISE_LEFT_WINDOW_SHADES_HOME;
                    }
                    else if (!strcmp("right", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        prompt = AUDIO_OK_RAISE_RIGHT_WINDOW_SHADES_HOME;
                    }
                    else if (!strcmp("middle", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        prompt = AUDIO_OK_RAISE_MIDDLE_WINDOW_SHADES_HOME;
                    }
                    else if (!strcmp("all", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.command = "lift";
                        valueToSend.value = (void *)100;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_RAISE_ALL_WINDOW_SHADES_HOME;
                    }
                }
                else if (!strcmp("down", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("lower", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("close", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("shut", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    if (!strcmp("left", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        prompt = AUDIO_OK_LOWER_LEFT_WINDOW_SHADES_HOME;
                    }
                    else if (!strcmp("right", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        prompt = AUDIO_OK_LOWER_RIGHT_WINDOW_SHADES_HOME;
                    }
                    else if (!strcmp("middle", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        prompt = AUDIO_OK_LOWER_MIDDLE_WINDOW_SHADES_HOME;
                    }
                    else if (!strcmp("all", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.command = "lift";
                        valueToSend.value = (void *)0;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_LOWER_ALL_WINDOW_SHADES_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
            else if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                     !strcmp("level", TAG_NAME(pSpeechIntent, (tagsCnt - 1))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBlindsControl;

                if (!strcmp("open", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("up", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("lift", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("raise", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "lift";

                    if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)) ||
                        !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)50;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_RAISE_WINDOW_SHADES_HALFWAY_HOME;
                    }
                    else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)100;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_RAISE_WINDOW_SHADES_COMPLETELY_HOME;
                    }
                    else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)25;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_RAISE_WINDOW_SHADES_SLIGHTLY_HOME;
                    }
                    /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                    else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 1] > 1)
                    {
                        valueToSend.value = (void *)10;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_RAISE_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                    }
                }
                else if (!strcmp("down", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("lower", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("close", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("shut", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "close";

                    if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)) ||
                        !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)50;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_LOWER_WINDOW_SHADES_HALFWAY_HOME;
                    }
                    else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)100;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_LOWER_WINDOW_SHADES_COMPLETELY_HOME;
                    }
                    else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        valueToSend.value = (void *)25;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_LOWER_WINDOW_SHADES_SLIGHTLY_HOME;
                    }
                    /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                    else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 1] > 1)
                    {
                        valueToSend.value = (void *)10;
                        valueToSend.location = "central";
                        prompt = AUDIO_OK_LOWER_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
        else if (pSpeechIntent->Slot_Tag_count == 3)
        {
            if (!strcmp("action", TAG_NAME(pSpeechIntent, tagsCnt)) &&
                !strcmp("side", TAG_NAME(pSpeechIntent, (tagsCnt - 1))) &&
                !strcmp("level", TAG_NAME(pSpeechIntent, (tagsCnt - 2))))
            {
                matterActionStruct valueToSend = {0};
                valueToSend.action = kMatterActionBlindsControl;

                if (!strcmp("open", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("up", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("lift", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("raise", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "lift";

                    if (!strcmp("left", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)) ||
                            !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_LEFT_WINDOW_SHADES_HALFWAY_HOME;
                        }
                        else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_LEFT_WINDOW_SHADES_COMPLETELY_HOME;
                        }
                        else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_LEFT_WINDOW_SHADES_SLIGHTLY_HOME;
                        }
                        /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                        else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 2] > 1)
                        {
                            prompt = AUDIO_OK_RAISE_LEFT_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                        }
                    }
                    else if (!strcmp("right", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)) ||
                            !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_RIGHT_WINDOW_SHADES_HALFWAY_HOME;
                        }
                        else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_RIGHT_WINDOW_SHADES_COMPLETELY_HOME;
                        }
                        else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_RIGHT_WINDOW_SHADES_SLIGHTLY_HOME;
                        }
                        /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                        else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 2] > 1)
                        {
                            prompt = AUDIO_OK_RAISE_RIGHT_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                        }
                    }
                    else if (!strcmp("middle", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)) ||
                            !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_MIDDLE_WINDOW_SHADES_HALFWAY_HOME;
                        }
                        else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_MIDDLE_WINDOW_SHADES_COMPLETELY_HOME;
                        }
                        else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_RAISE_MIDDLE_WINDOW_SHADES_SLIGHTLY_HOME;
                        }
                        /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                        else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 2] > 1)
                        {
                            prompt = AUDIO_OK_RAISE_MIDDLE_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                        }
                    }
                    else if (!strcmp("all", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)) ||
                            !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.value = (void *)50;
                            valueToSend.location = "central";
                            prompt = AUDIO_OK_RAISE_ALL_WINDOW_SHADES_HALFWAY_HOME;
                        }
                        else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.value = (void *)100;
                            valueToSend.location = "central";
                            prompt = AUDIO_OK_RAISE_ALL_WINDOW_SHADES_COMPLETELY_HOME;
                        }
                        else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.value = (void *)25;
                            valueToSend.location = "central";
                            prompt = AUDIO_OK_RAISE_ALL_WINDOW_SHADES_SLIGHTLY_HOME;
                        }
                        /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                        else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 2] > 1)
                        {
                            valueToSend.value = (void *)10;
                            valueToSend.location = "central";
                            prompt = AUDIO_OK_RAISE_ALL_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                        }
                    }
                }
                else if (!strcmp("down", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("lower", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("close", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                         !strcmp("shut", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    valueToSend.command = "close";

                    if (!strcmp("left", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)) ||
                            !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_LEFT_WINDOW_SHADES_HALFWAY_HOME;
                        }
                        else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_LEFT_WINDOW_SHADES_COMPLETELY_HOME;
                        }
                        else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_LEFT_WINDOW_SHADES_SLIGHTLY_HOME;
                        }
                        /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                        else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 2] > 1)
                        {
                            prompt = AUDIO_OK_LOWER_LEFT_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                        }
                    }
                    else if (!strcmp("right", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)) ||
                            !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_RIGHT_WINDOW_SHADES_HALFWAY_HOME;
                        }
                        else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_RIGHT_WINDOW_SHADES_COMPLETELY_HOME;
                        }
                        else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_RIGHT_WINDOW_SHADES_SLIGHTLY_HOME;
                        }
                        /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                        else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 2] > 1)
                        {
                            prompt = AUDIO_OK_LOWER_RIGHT_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                        }
                    }
                    else if (!strcmp("middle", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)) ||
                            !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_MIDDLE_WINDOW_SHADES_HALFWAY_HOME;
                        }
                        else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_MIDDLE_WINDOW_SHADES_COMPLETELY_HOME;
                        }
                        else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            prompt = AUDIO_OK_LOWER_MIDDLE_WINDOW_SHADES_SLIGHTLY_HOME;
                        }
                        /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                        else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 2] > 1)
                        {
                            prompt = AUDIO_OK_LOWER_MIDDLE_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                        }
                    }
                    else if (!strcmp("all", TAG_VALUE(pSpeechIntent, (tagsCnt - 1), 0)))
                    {
                        if (!strcmp("halfway", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)) ||
                            !strcmp("partially", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.value = (void *)50;
                            valueToSend.location = "central";
                            prompt = AUDIO_OK_LOWER_ALL_WINDOW_SHADES_HALFWAY_HOME;
                        }
                        else if (!strcmp("completely", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.value = (void *)100;
                            valueToSend.location = "central";
                            prompt = AUDIO_OK_LOWER_ALL_WINDOW_SHADES_COMPLETELY_HOME;
                        }
                        else if (!strcmp("slightly", TAG_VALUE(pSpeechIntent, (tagsCnt - 2), 0)))
                        {
                            valueToSend.value = (void *)25;
                            valueToSend.location = "central";
                            prompt = AUDIO_OK_LOWER_ALL_WINDOW_SHADES_SLIGHTLY_HOME;
                        }
                        /* A LITTLE, A LITTLE BIT, JUST A LITTLE BIT */
                        else if (pSpeechIntent->Slot_Tag_Value_count[tagsCnt - 2] > 1)
                        {
                            valueToSend.value = (void *)10;
                            valueToSend.location = "central";
                            prompt = AUDIO_OK_LOWER_ALL_WINDOW_SHADES_A_LITTLE_BIT_HOME;
                        }
                    }
                }
                xQueueSend(xMatterActionsQueue, &valueToSend, portMAX_DELAY);
            }
        }
    }
    /* CUSTOM ADJUST BRIGHTNESS*/
    else if (!strcmp("CustomAdjustBrightness", INTENT_NAME(pSpeechIntent, tagsCnt)))
    {
        if (!strcmp("light_on", TAG_NAME(pSpeechIntent, tagsCnt)))
        {
            if (!strcmp("dark", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
            {
                prompt = AUDIO_OK_INCREASING_BRIGHTNESS_HOME;
            }
        }
        else if (!strcmp("light_off", TAG_NAME(pSpeechIntent, tagsCnt)))
        {
            if (!strcmp("bright", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
            {
                prompt = AUDIO_OK_DECREASING_BRIGHTNESS_HOME;
            }
        }
    }

    configPRINTF(("%s \r\n", prompt));

#if ENABLE_STREAMER
    if (prompt != NULL)
    {
        APP_LAYER_PlayAudioFromFileSystem(prompt);
    }
    else
    {
        configPRINTF(("Intent processed successfully but no audio prompt associated to it found.\r\n"));
    }
#endif /* ENABLE_STREAMER */
}

static void APP_LAYER_VIT_ParseSpeechIntent(void)
{
    APP_LAYER_VIT_ParseHomeIntent();
}

/*******************************************************************************
 * API
 ******************************************************************************/

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

status_t APP_LAYER_ProcessIntent()
{
    status_t status = kStatus_Success;

    APP_LAYER_LedCommandDetected();
    APP_LAYER_VIT_ParseSpeechIntent();

    return status;
}

bool APP_LAYER_FilterIntent()
{
    bool filterDetection = false;
    VIT_Intent_st *pSpeechIntent = NULL;
    PL_INT16 tagsCnt;

    pSpeechIntent = &SpeechIntent;
    tagsCnt       = pSpeechIntent->Slot_Tag_count - 1;

    if (oob_demo_control.changeDemoFlow)
    {
        if (strcmp("ChangeDemo", INTENT_NAME(pSpeechIntent, tagsCnt)))
        {
            filterDetection = true;
        }
    }
    else
    {
        if(!strcmp("ChangeDemo", INTENT_NAME(pSpeechIntent, tagsCnt)))
        {
            if (!strcmp("demo", TAG_NAME(pSpeechIntent, tagsCnt)))
            {
                if (!strcmp("hvac", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("oven", TAG_VALUE(pSpeechIntent, tagsCnt, 0)) ||
                    !strcmp("smart", TAG_VALUE(pSpeechIntent, tagsCnt, 0)))
                {
                    filterDetection = true;
                }
            }
        }
    }

    return filterDetection;
}

status_t APP_LAYER_ProcessTimeout(oob_demo_control_t *commandConfig)
{
    status_t status = kStatus_Success;

    if (commandConfig != NULL)
    {
        /* appAsrShellCommands.demo contains current demo. */
        switch (appAsrShellCommands.demo)
        {
            case ASR_S2I_HOME:
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

void APP_LAYER_HandleFirstBoardBoot(void)
{
    if ((appAsrShellCommands.demo == BOOT_ASR_CMD_DEMO) &&
        (BOOT_ASR_CMD_DEMO != DEFAULT_ASR_CMD_DEMO))
    {
    	APP_LAYER_VIT_ParseHomeIntent();
    }
}

#endif /* ENABLE_S2I_ASR */
#endif /* ENABLE_NXP_OOBE */
