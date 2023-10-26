/*
 *  Copyright 2022 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __TCPDOWNLOAD_H__
#define __TCPDOWNLOAD_H__

/* Creates a dedicated Task responsible for downloading a file and calculating its hash.
 * The download is performed over TCP on a port defined by TCP_DOWNLOAD_PORT. */
void EnableTcpDownloadComponent(void);

#endif /* __TCPDOWNLOAD_H__ */
