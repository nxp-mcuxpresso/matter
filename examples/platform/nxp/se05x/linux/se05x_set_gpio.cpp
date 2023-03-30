/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <lib/support/logging/CHIPLogging.h>
#include "se05x_set_gpio.h"

//#define SE05X_PWR_PIN 508 // 496 (base of gpio expansion) + 12 (pin number) (= EXP_IO10 = pin 15 of j1003)
#define SE05X_PWR_PIN 4

#define SLEEP_SECONDS 0
#define SLEEP_NS 5000000 // 5_000_000 ns (2 ms relais switch, 1 ms se05x startup, 2 ms spare)

int se05x_export_pin(int pin);
int se05x_unexport_pin(int pin);
int se05x_set_pin_to_output(int pin);
int se05x_set_pin_value(int pin, int value);

extern int se05x_session_open;

int se05x_set_pin(int value)
{
    const struct timespec ts = { SLEEP_SECONDS, SLEEP_NS };
    struct timespec ts_left;

    int res;
    res = se05x_export_pin(SE05X_PWR_PIN);
    if (res != 0)
    {
        return res;
    }

    res = se05x_set_pin_to_output(SE05X_PWR_PIN);
    if (res != 0)
    {
        return res;
    }

    res = se05x_set_pin_value(SE05X_PWR_PIN, value);
    if (res != 0)
    {
        return res;
    }

#if 0
    res = se05x_unexport_pin(SE05X_PWR_PIN);
    if (res != 0)
    {
        return res;
    }
#endif

    nanosleep(&ts, &ts_left);

    if (value == 0)
    {
        // to make sure that the session is reopend
        se05x_session_open = 0;
    }

    // And exit
    return 0;
}

int se05x_export_pin(int pin)
{
    char pin_directory[30];
    sprintf(pin_directory, "/sys/class/gpio/gpio%d", pin);

    DIR * dir = opendir(pin_directory);
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
        return 0;
    }
    else if (ENOENT != errno)
    {
        ChipLogProgress(NotSpecified, "Error trying to open /sys/class/gpio/gpio-PIN");
        return 1;
    }

    // Export the desired pin by writing to /sys/class/gpio/export
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1)
    {
        ChipLogProgress(NotSpecified, "Unable to open /sys/class/gpio/export");
        return 1;
    }

    char pin_str[10];
    sprintf(pin_str, "%d", pin);

    if (write(fd, pin_str, strlen(pin_str)) != (ssize_t) strlen(pin_str))
    {
        ChipLogProgress(NotSpecified, "Error writing to /sys/class/gpio/export");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int se05x_unexport_pin(int pin)
{
    // Unexport the pin by writing to /sys/class/gpio/unexport
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd == -1)
    {
        ChipLogProgress(NotSpecified, "Unable to open /sys/class/gpio/unexport");
        return 1;
    }

    char pin_str[10];
    sprintf(pin_str, "%d", pin);

    if (write(fd, pin_str, strlen(pin_str)) != (ssize_t) strlen(pin_str))
    {
        ChipLogProgress(NotSpecified, "Error writing to /sys/class/gpio/unexport");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int se05x_set_pin_to_output(int pin)
{
    char file_name[50];
    sprintf(file_name, "/sys/class/gpio/gpio%d/direction", pin);

    // Set the pin to be an output
    int fd = open(file_name, O_WRONLY);
    if (fd == -1)
    {
        ChipLogProgress(NotSpecified, "Unable to open /sys/class/gpio/gpioPIN/direction");
        return 1;
    }

    if (write(fd, "out", 3) != 3)
    {
        ChipLogProgress(NotSpecified, "Error writing to /sys/class/gpio/gpioPIN/direction");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int se05x_set_pin_value(int pin, int value)
{
    char file_name[50];
    char value_str[10];
    sprintf(file_name, "/sys/class/gpio/gpio%d/value", pin);

    int fd = open(file_name, O_WRONLY);
    if (fd == -1)
    {
        ChipLogProgress(NotSpecified, "Unable to open /sys/class/gpio/gpioPIN/value");
        return 1;
    }

    sprintf(value_str, "%d", value);

    if (write(fd, value_str, strlen(value_str)) != (ssize_t) strlen(value_str))
    {
        ChipLogProgress(NotSpecified, "Error writing to /sys/class/gpio/gpioPIN/value");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
