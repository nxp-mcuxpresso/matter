/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if ENABLE_VIT_ASR || ENABLE_DSMT_ASR

#include "stddef.h"
#include "string.h"
#include "sln_flash_files.h"
#include "sln_voice_demo.h"
#include "FreeRTOSConfig.h"
#include "IndexCommands.h"

#if ENABLE_DSMT_ASR
#include "sln_local_voice_dsmt.h"
extern sln_voice_demo_t *all_voice_demos_dsmt;
sln_voice_demo_t **all_voice_demos = &all_voice_demos_dsmt;
#elif ENABLE_VIT_ASR
#include "sln_local_voice_vit.h"
extern sln_voice_demo_t *all_voice_demos_vit;
sln_voice_demo_t **all_voice_demos = &all_voice_demos_vit;
#endif /* ENABLE_DSMT_ASR */

static sln_voice_demo_t *get_voice_demo(asr_language_t asrLang, asr_inference_t infCMDType)
{
    sln_voice_demo_t **demo_iterator = all_voice_demos;
    sln_voice_demo_t *demo_ptr       = NULL;

    while (*demo_iterator)
    {
        demo_ptr = *demo_iterator;

        if ((demo_ptr->language_id == asrLang) &&
            (demo_ptr->demo_id == infCMDType))
            break;

        demo_iterator++;
    }

    return *demo_iterator;
}

uint16_t get_cmd_number(asr_language_t asrLang, asr_inference_t infCMDType)
{
    uint16_t cmd_number = 0;

    sln_voice_demo_t *demo = get_voice_demo(asrLang, infCMDType);
    if (demo)
    {
        cmd_number = demo->num_cmd_strings;
    }

    return cmd_number;
}

uint16_t get_ww_number(asr_language_t asrLang, asr_inference_t infCMDType)
{
    uint16_t ww_number = 0;

    sln_voice_demo_t *demo = get_voice_demo(asrLang, infCMDType);
    if (demo)
    {
        ww_number = demo->num_ww_strings;
    }

    return ww_number;
}

char **get_cmd_strings(asr_language_t asrLang, asr_inference_t infCMDType)
{
    char **retString = NULL;

    sln_voice_demo_t *demo = get_voice_demo(asrLang, infCMDType);
    if (demo)
    {
        retString = (char **)demo->cmd_strings;
    }

    return retString;
}

uint16_t get_action_from_keyword(asr_language_t asrLang, asr_inference_t infCMDType, uint8_t keywordId)
{
    uint16_t action_index = 0;

    sln_voice_demo_t *demo = get_voice_demo(asrLang, infCMDType);
    if (demo)
    {
        action_index = demo->cmd_actions[keywordId];
    }

    return action_index;
}

void *get_prompt_from_keyword(asr_language_t asrLang, asr_inference_t infCMDType, uint8_t keywordId)
{
    char *prompt = NULL;

    sln_voice_demo_t *demo = get_voice_demo(asrLang, infCMDType);
    if (demo)
    {
        prompt = (char *)demo->cmd_prompts[keywordId];
    }

    return prompt;
}

char *get_demo_prompt(asr_inference_t demoType, asr_language_t asrLang)
{
    char *prompt               = NULL;
    asr_language_t asrLangUsed = UNDEFINED_LANGUAGE;

#if MULTILINGUAL
    asrLangUsed = active_languages_get_first(asrLang);
#else
    asrLangUsed = asrLang;
#endif /* MULTILINGUAL */

    if (asrLangUsed == UNDEFINED_LANGUAGE)
    {
        asrLangUsed = DEFAULT_ASR_LANGUAGE;
    }

    sln_voice_demo_t *demo = get_voice_demo(asrLangUsed, demoType);
    if (demo)
    {
        prompt = (char *)demo->demo_prompt;
    }

    return prompt;
}

char **get_ww_strings(asr_language_t asrLang)
{
    char **retString = NULL;

    sln_voice_demo_t *demo = get_voice_demo(asrLang, DEFAULT_ASR_CMD_DEMO);
    if (demo)
    {
        retString = (char **)demo->ww_strings;
    }

    return retString;
}

void *get_demo_model(asr_language_t asrLang, asr_inference_t infCMDType)
{
    void *model               = NULL;

    sln_voice_demo_t *demo = get_voice_demo(asrLang, infCMDType);
    if (demo)
    {
        model = (char *)demo->model;
    }

    return model;
}

const char *get_demo_string(asr_language_t asrLang, asr_inference_t infCMDType)
{
    char *demo_str = NULL;

    sln_voice_demo_t *demo = get_voice_demo(asrLang, infCMDType);
    if (demo)
    {
        demo_str = (char *)demo->demo_str;
    }

    return demo_str;
}

uint16_t get_demo_id_from_str(char *demo_str)
{
    uint16_t demo_id = UNDEFINED_INFERENCE;

    if (NULL != demo_str)
    {
        sln_voice_demo_t **demo_iterator = all_voice_demos;
        sln_voice_demo_t *demo_ptr       = NULL;

        while (*demo_iterator)
        {
            demo_ptr = *demo_iterator;

            if (!strcmp(demo_ptr->demo_str, demo_str))
            {
                demo_id = demo_ptr->demo_id;
                break;
            }

            demo_iterator++;
        }
    }

    return demo_id;
}

uint16_t get_language_id_from_str(char *language_str)
{
    uint16_t language_id = UNDEFINED_LANGUAGE;

    if (NULL != language_str)
    {
        sln_voice_demo_t **demo_iterator = all_voice_demos;
        sln_voice_demo_t *demo_ptr       = NULL;

        while (*demo_iterator)
        {
            demo_ptr = *demo_iterator;

            if (!strcmp(demo_ptr->language_str, language_str))
            {
                language_id = demo_ptr->language_id;
                break;
            }

            demo_iterator++;
        }
    }

    return language_id;
}

char *get_language_str_from_id(uint16_t language_id)
{
    char *language_string = NULL;

    if (UNDEFINED_LANGUAGE != language_id)
    {
        sln_voice_demo_t **demo_iterator = all_voice_demos;
        sln_voice_demo_t *demo_ptr       = NULL;

        while (*demo_iterator)
        {
            demo_ptr = *demo_iterator;

            if (demo_ptr->language_id == language_id)
            {
                language_string = (char *)demo_ptr->language_str;
                break;
            }

            demo_iterator++;
        }
    }

    return language_string;
}

void print_active_languages_str(uint16_t active_languages, uint16_t demo_id)
{
    if (UNDEFINED_LANGUAGE != active_languages)
    {
        sln_voice_demo_t **demo_iterator = all_voice_demos;
        sln_voice_demo_t *demo_ptr       = NULL;

        while (*demo_iterator)
        {
            demo_ptr = *demo_iterator;

            if ((demo_ptr->demo_id == demo_id) && (active_languages & demo_ptr->language_id))
            {
                configPRINT_STRING(" %s", demo_ptr->language_str);
            }

            demo_iterator++;
        }
    }
}

bool validate_all_active_languages(asr_language_t asrLang, asr_inference_t infCMDType)
{
    asr_language_t lang           = ASR_FIRST_LANGUAGE;
    asr_language_t activeLanguage = asrLang;
    bool ret = true;
    int16_t num_languages = 0;

    if (asrLang > ASR_ALL_LANG || asrLang == UNDEFINED_LANGUAGE)
    {
        ret = false;
    }

    if (ret == true)
    {
        while (activeLanguage != UNDEFINED_LANGUAGE)
        {
            if ((activeLanguage & lang) != UNDEFINED_LANGUAGE)
            {
                num_languages++;

                if ((NULL == get_voice_demo(lang, infCMDType)) || (num_languages > MAX_CONCURRENT_LANGUAGES))
                {
                    ret = false;
                    break;
                }
                activeLanguage &= ~lang;
            }
            lang <<= 1U;
        }
    }

    return ret;
}

uint16_t active_languages_get_first(asr_language_t asrLang)
{
    uint16_t first_active_lang = UNDEFINED_LANGUAGE;
    asr_language_t lang           = ASR_FIRST_LANGUAGE;
    asr_language_t activeLanguage = asrLang;

    if (asrLang <= ASR_ALL_LANG && asrLang != UNDEFINED_LANGUAGE)
    {
        while (activeLanguage != UNDEFINED_LANGUAGE)
        {
            if ((activeLanguage & lang) != UNDEFINED_LANGUAGE)
            {
                first_active_lang = lang;
                break;
            }

            activeLanguage &= ~lang;
            lang <<= 1U;
        }
    }

    return first_active_lang;
}

#endif /* ENABLE_VIT_ASR || ENABLE_DSMT_ASR */
