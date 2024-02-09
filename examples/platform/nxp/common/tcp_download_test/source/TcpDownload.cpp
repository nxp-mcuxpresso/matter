/*
 *  Copyright 2022-2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */
#include "TcpDownload.h"
#include "DeviceCallbacks.h"
#include "lwip/api.h"
#include <lib/support/Span.h>
#include <platform/nxp/common/NetworkCommissioningDriver.h>

using namespace ::chip::DeviceLayer;

/* TCP Download task configuration */
#define TCP_DOWNLOAD_TASK_NAME "tcp_download"
#define TCP_DOWNLOAD_TASK_SIZE 1024
#define TCP_DOWNLOAD_TASK_PRIO (configMAX_PRIORITIES - 4)

/* TCP Download task port */
#define TCP_DOWNLOAD_PORT 80

/* Set to 1 in order to connect to a dedicated Wi-Fi network.
 * Specify WIFI_SSID and WIFI_PASSWORD. */
#ifndef WIFI_CONNECT
#define WIFI_CONNECT 0
#endif

#if WIFI_CONNECT

#ifndef WIFI_SSID
#define WIFI_SSID "my_ssid"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "my_password"
#endif

#endif /* WIFI_CONNECT */
bool networkAdded = false;

void _tcpDownload(void * pvParameters)
{
#if WIFI_CONNECT
    CHIP_ERROR chip_err;
    NetworkCommissioning::Status status;

    const chip::ByteSpan ssid(reinterpret_cast<const uint8_t *>(WIFI_SSID), strlen(WIFI_SSID));
    const chip::ByteSpan password(reinterpret_cast<const uint8_t *>(WIFI_PASSWORD), strlen(WIFI_PASSWORD));
    chip::MutableCharSpan debugText;
    uint8_t networkIndex;

    ChipLogProgress(DeviceLayer,
                    "[TCP_DOWNLOAD] Connecting to Wi-Fi network: SSID = '" WIFI_SSID "' and PASSWORD = '" WIFI_PASSWORD "'");

    chip_err = NetworkCommissioning::NXPWiFiDriver::GetInstance().Init(nullptr);
    if (chip_err == CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND)
    {
        ChipLogError(DeviceLayer, "[TCP_DOWNLOAD] Error: Init: SSID and/or PASSWORD not found in persistent storage");

        status = NetworkCommissioning::NXPWiFiDriver::GetInstance().AddOrUpdateNetwork(ssid, password, debugText, networkIndex);
        if (status != NetworkCommissioning::Status::kSuccess)
        {
            ChipLogError(DeviceLayer, "[TCP_DOWNLOAD] Error: AddOrUpdateNetwork: %u", (uint8_t) status);
        }

        NetworkCommissioning::NXPWiFiDriver::GetInstance().ConnectNetwork(ssid, nullptr);
        networkAdded = true;
        // TODO should wait for connection establishment or failure (but the socket below will start accepting once WiFi is
        // connected)
    }
    else if (chip_err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "[TCP_DOWNLOAD] Error: Init: %" CHIP_ERROR_FORMAT, chip_err.Format());
    }
    else
    {
        networkAdded = true;
    }

#endif

    if (networkAdded == true)
    {
        err_t err                = ERR_OK;
        struct netconn * conn    = NULL;
        struct netconn * newconn = NULL;
        uint16_t tcpDownloadPort = TCP_DOWNLOAD_PORT;

        conn = netconn_new(NETCONN_TCP);
        if (conn == NULL)
        {
            err = ERR_ARG;
            ChipLogError(DeviceLayer, "[TCP_DOWNLOAD] ERROR: netconn_new failed");
        }

        if (err == ERR_OK)
        {
            err = netconn_bind(conn, IP_ADDR_ANY, tcpDownloadPort);
            if (err != ERR_OK)
            {
                ChipLogError(DeviceLayer, "[TCP_DOWNLOAD] ERROR: netconn_bind failed %d", err);
            }
        }

        if (err == ERR_OK)
        {
            err = netconn_listen(conn);
            if (err != ERR_OK)
            {
                ChipLogError(DeviceLayer, "[TCP_DOWNLOAD] ERROR: netconn_listen failed %d", err);
            }
        }

        if (err == ERR_OK)
        {
            while (1)
            {
                ChipLogProgress(DeviceLayer, "[TCP_DOWNLOAD] TCP component is listening on port %d", tcpDownloadPort);

                /* Wait for a new connection. */
                err = netconn_accept(conn, &newconn);
                if (err == ERR_OK)
                {
                    uint32_t fileHash = 0;
                    struct netbuf * buf;
                    void * data;
                    u16_t len;

                    /* Process the new connection. */
                    ChipLogProgress(DeviceLayer, "[TCP_DOWNLOAD] New connection opened");

                    while ((err = netconn_recv(newconn, &buf)) == ERR_OK)
                    {
                        do
                        {
                            netbuf_data(buf, &data, &len);

                            /* Compute the hash */
                            for (uint32_t i = 0; i < len; i++)
                            {
                                fileHash = (fileHash + ((uint8_t *) data)[i]) % 256;
                            }
                        } while (netbuf_next(buf) >= 0);

                        netbuf_delete(buf);
                    }

                    ChipLogProgress(DeviceLayer, "[TCP_DOWNLOAD] Received file HASH = %ld", fileHash);

                    /* Close connection and discard connection identifier. */
                    netconn_close(newconn);
                    netconn_delete(newconn);
                    ChipLogProgress(DeviceLayer, "[TCP_DOWNLOAD] Connection closed\r\n\r\n");
                }
                else
                {
                    ChipLogError(DeviceLayer, "[TCP_DOWNLOAD] Error netconn_accept failed %d\r\n\r\n", err);
                }

                vTaskDelay(1000);
            }
        }
    }

    vTaskDelete(NULL);
}

void chip::NXP::App::EnableTcpDownloadComponent(void)
{
    TaskHandle_t taskHandle;

    if (xTaskCreate(&_tcpDownload, TCP_DOWNLOAD_TASK_NAME, TCP_DOWNLOAD_TASK_SIZE, NULL, TCP_DOWNLOAD_TASK_PRIO, &taskHandle) !=
        pdPASS)
    {
        ChipLogError(DeviceLayer, "Failed to start tcp_download task");
        assert(false);
    }
}
