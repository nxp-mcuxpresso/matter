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

#include "streamer.h"
#include <app/server/Server.h>
#include <lib/shell/Engine.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/CommissionableDataProvider.h>
#include <platform/nxp/mw320/ConnectivityUtils.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "wlan.h"
#include "network_flash_storage.h"
void test_wlan_scan(int argc, char ** argv);
}
#include "AppTask.h"
#include "mw320_ota.h"

using chip::FormatCHIPError;
using chip::Shell::Engine;
using chip::Shell::streamer_get;

namespace {

intptr_t ReadLine(char * buffer, size_t max)
{
    ssize_t read = 0;
    bool done    = false;
    char * inptr = buffer;

    // Read in characters until we get a new line or we hit our max size.
    while (((inptr - buffer) < static_cast<int>(max)) && !done)
    {
        if (read == 0)
        {
            read = streamer_read(streamer_get(), inptr, 1);
        }

        // Process any characters we just read in.
        while (read > 0)
        {
            switch (*inptr)
            {
            case '\r':
            case '\n':
                streamer_printf(streamer_get(), "\r\n");
                *inptr = 0; // null terminate
                done   = true;
                break;
            case 0x7F:
                // delete backspace character + 1 more
                inptr -= 2;
                if (inptr >= buffer - 1)
                {
                    streamer_printf(streamer_get(), "\b \b");
                }
                else
                {
                    inptr = buffer - 1;
                }
                break;
            default:
                if (isprint(static_cast<int>(*inptr)) || *inptr == '\t')
                {
                    streamer_printf(streamer_get(), "%c", *inptr);
                }
                else
                {
                    inptr--;
                }
                break;
            }

            inptr++;
            read--;
        }
    }

    return inptr - buffer;
}

bool IsSeparator(char ch)
{
    return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
}

bool IsEscape(char ch)
{
    return (ch == '\\');
}

bool IsEscapable(char ch)
{
    return IsSeparator(ch) || IsEscape(ch);
}

int TokenizeLine(char * buffer, char ** tokens, int max_tokens)
{
    size_t len = strlen(buffer);
    int cursor = 0;
    size_t i   = 0;

    // Strip leading spaces
    while (buffer[i] && buffer[i] == ' ')
    {
        i++;
    }

    VerifyOrExit((len - i) > 0, cursor = 0);

    // The first token starts at the beginning.
    tokens[cursor++] = &buffer[i];

    for (; i < len && cursor < max_tokens; i++)
    {
        if (IsEscape(buffer[i]) && IsEscapable(buffer[i + 1]))
        {
            // include the null terminator: strlen(cmd) = strlen(cmd + 1) + 1
            memmove(&buffer[i], &buffer[i + 1], strlen(&buffer[i]));
        }
        else if (IsSeparator(buffer[i]))
        {
            buffer[i] = 0;
            if (!IsSeparator(buffer[i + 1]))
            {
                tokens[cursor++] = &buffer[i + 1];
            }
        }
    }

    tokens[cursor] = nullptr;

exit:
    return cursor;
}

} // namespace

extern const char * mw320_get_verstr(void);
namespace chip {
namespace Shell {

// ++++
static void AtExitShell(void);

static CHIP_ERROR ShutdownHandler(int argc, char ** argv)
{
    streamer_printf(streamer_get(), "Shutdown and Goodbye\r\n");
    Server::GetInstance().GenerateShutDownEvent();
    // TODO: This is assuming that we did (on a different thread from this one)
    // RunEventLoop(), not StartEventLoopTask().  It will not work correctly
    // with StartEventLoopTask().
    DeviceLayer::PlatformMgr().ScheduleWork([](intptr_t) { DeviceLayer::PlatformMgr().StopEventLoopTask(); });
    AtExitShell();
    exit(0);
    return CHIP_NO_ERROR;
}

static void AtExitShell(void)
{
    PRINTF("%s(), PlatformMgr().Shutdown() \r\n", __FUNCTION__);
    chip::DeviceLayer::PlatformMgr().Shutdown();
}

static CHIP_ERROR VersionHandler(int argc, char ** argv)
{
    streamer_printf(streamer_get(), "mw320 firmware: %s\r\n", mw320_get_verstr());
    return CHIP_NO_ERROR;
}

static CHIP_ERROR SetPinCodeHandler(int argc, char ** argv)
{
    VerifyOrReturnError(argc == 1, CHIP_ERROR_INVALID_ARGUMENT);
    uint32_t setupPinCode = strtoull(argv[0], nullptr, 10);

    ReturnErrorOnFailure(DeviceLayer::GetCommissionableDataProvider()->SetSetupPasscode(setupPinCode));

    return CHIP_NO_ERROR;
}

static CHIP_ERROR SetDefAPHandler(int argc, char ** argv)
{
    VerifyOrReturnError(argc == 2, CHIP_ERROR_INVALID_ARGUMENT);
    PRINTF("[%s], [%s] \r\n", argv[0], argv[1]);
    GetAppTask().SaveNetwork(argv[0], argv[1]);

    return CHIP_NO_ERROR;
}

static CHIP_ERROR WlanStateHandler(int argc, char ** argv)
{
    enum wlan_connection_state state;
    int result;
    result = wlan_get_connection_state(&state);
    if (result != WM_SUCCESS)
    {
        streamer_printf(streamer_get(), "Unknown WiFi State\r\n");
        return CHIP_ERROR_INCORRECT_STATE;
    }
    switch (state)
    {
    case WLAN_DISCONNECTED:
        streamer_printf(streamer_get(), "Wi-Fi: Disconnected\r\n");
        break;
    case WLAN_CONNECTING:
        streamer_printf(streamer_get(), "Wi-Fi: connecting \r\n");
        break;
    case WLAN_ASSOCIATED:
        streamer_printf(streamer_get(), "Wi-Fi: associated \r\n");
        break;
    case WLAN_CONNECTED:
        streamer_printf(streamer_get(), "Wi-Fi: connected \r\n");
        break;
    case WLAN_SCANNING:
        streamer_printf(streamer_get(), "Wi-Fi: scanning \r\n");
        break;
    default:
        streamer_printf(streamer_get(), "Unknown WiFi State [%d] \r\n", (int) state);
    }

    return CHIP_NO_ERROR;
}

static CHIP_ERROR WlanAbortHandler(int argc, char ** argv)
{
#ifdef WIFI_CONN_ABORT_SUPPORT
    wlan_abort_connect();
#endif // WIFI_CONN_ABORT_SUPPORT
    return CHIP_NO_ERROR;
}

static CHIP_ERROR WlanConnHandler(int argc, char ** argv)
{
    VerifyOrReturnError(argc == 2, CHIP_ERROR_INVALID_ARGUMENT);
    PRINTF("[%s], [%s] \r\n", argv[0], argv[1]);
    //::connect_wifi_network(argv[0], argv[1]);
    chip::DeviceLayer::Internal::ConnectivityUtils::ConnectWiFiNetwork(argv[0], argv[1]);
    return CHIP_NO_ERROR;
}

static CHIP_ERROR WlanScanHandler(int argc, char ** argv)
{
    test_wlan_scan(0, NULL);
    return CHIP_NO_ERROR;
}

static CHIP_ERROR FactoryRstHandler(int argc, char **argv)
{
    // Eraseing the saved parameters
    ::erase_all_params();
    // Do factory reset from Matter Stack
    chip::Server::GetInstance().ScheduleFactoryReset();
    if (argc == 1) {
        if (!strcmp(argv[0], "1")) {
            // Set default AP
            GetAppTask().SaveNetwork((char*)DEFAP_SSID, (char*)DEFAP_PWD);
            // Set op_state to working
            GetAppTask().SetOpState(work_state);
        }
    }
    // Reboot the device
    ::mw320_dev_reset(1000);

    return CHIP_NO_ERROR;
}

static CHIP_ERROR SetOpStatHandler(int argc, char ** argv)
{
    op_state_t opstat;
    VerifyOrReturnError(argc == 1, CHIP_ERROR_INVALID_ARGUMENT);
    opstat = (op_state_t)atoi(argv[0]);
    VerifyOrReturnError(opstat < max_op_state, CHIP_ERROR_INVALID_ARGUMENT);
    PRINTF("Operation State: %d\r\n", (int)opstat);
    GetAppTask().SetOpState(opstat);
    return CHIP_NO_ERROR;
}

static CHIP_ERROR GetOpStatHandler(int argc, char ** argv)
{
    op_state_t opstat = GetAppTask().GetOpState();
    PRINTF("Current operation state: %d \r\n", (int)opstat);
    return CHIP_NO_ERROR;
}

static CHIP_ERROR CmdRebootHandler(int argc, char ** argv)
{
    ::mw320_dev_reset(1000);
    return CHIP_NO_ERROR;
}


static void RegisterMetaCommands(void)
{
    static shell_command_t sCmds[] = {
        { &ShutdownHandler, "shutdown", "Exit the shell application" },
        { &VersionHandler, "version", "Output the software version" },
        { &SetPinCodeHandler, "pincode", "Set the pin code" },
        { &SetDefAPHandler, "set-defap", "Set default AP SSID/PWD" },
        { &WlanStateHandler, "wlan-stat", "Check the wifi status" },
        { &WlanAbortHandler, "wlan-abort", "Abort the scan/reconnect" },
        { &WlanConnHandler, "wlan-connect", "Connect to AP" },
        { &WlanScanHandler, "wlan-scan", "Scan the environment"},
        { &FactoryRstHandler, "factory-reset", "Do factory reset"},
        { &SetOpStatHandler, "set-opstate", "Set the Operation State [0:factory_rest|1:work]"},
        { &GetOpStatHandler, "get-opstate", "Get the Operation State [0:factory_rest|1:work]"},
        { &CmdRebootHandler, "reboot", "Reboot the device"},
    };

    std::atexit(AtExitShell);

    Engine::Root().RegisterCommands(sCmds, ArraySize(sCmds));
}

// ----

void Engine::RunMainLoop()
{
    CHIP_ERROR retval;
    int argc;
    char * argv[CHIP_SHELL_MAX_TOKENS];
    char line[CHIP_SHELL_MAX_LINE_SIZE];

    Engine::Root().RegisterDefaultCommands();
    RegisterMetaCommands();

    while (true)
    {
        streamer_printf(streamer_get(), CHIP_SHELL_PROMPT);

        ReadLine(line, sizeof(line));
        argc = TokenizeLine(line, argv, CHIP_SHELL_MAX_TOKENS);

        if (argc > 0)
        {
            retval = Engine::Root().ExecCommand(argc, argv);

            if (retval != CHIP_NO_ERROR)
            {
                char errorStr[160];
                bool errorStrFound = FormatCHIPError(errorStr, sizeof(errorStr), retval);
                if (!errorStrFound)
                {
                    errorStr[0] = 0;
                }
                streamer_printf(streamer_get(), "Error %s: %s\r\n", argv[0], errorStr);
            }
            else
            {
                streamer_printf(streamer_get(), "Done\r\n", argv[0]);
            }
        }
        else
        {
            // Empty input has no output -- just display prompt
        }
    }
}

} // namespace Shell
} // namespace chip
