/*
 * Copyright 2021 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_SAFE_STRLEN_H_
#define _SLN_SAFE_STRLEN_H_

#include <stddef.h>
#ifndef __REDLIB__
#include <string.h>
#endif /* __REDLIB__ */

/**
 * @brief Determine the length of a fixed-size string.
 *        For __REDLIB__, determine the length manually.
 *        For NOT __REDLIB__, call strnlen from <string.h>.
 *
 * @param ptr Pointer to the data.
 * @param max Maximum length to be counted.
 *
 * @return Number of bytes in the string pointed by 'ptr', but at most 'max'.
 */
__attribute__((weak)) size_t safe_strlen(const char *ptr, size_t max)
{
#ifdef __REDLIB__
    /* REDLIB Has no strnlen call; This serves as a placeholder for developer to implement own function. */
    size_t len;

    for (len = 0; len < max; len++, ptr++)
    {
        if (*ptr == 0)
        {
            break;
        }
    }

    return len;
#else
    return strnlen(ptr, max);
#endif /* __REDLIB__ */
}

#endif /* _SLN_SAFE_STRLEN_H_ */
