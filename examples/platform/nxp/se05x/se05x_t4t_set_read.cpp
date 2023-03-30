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

#include <lib/support/logging/CHIPLogging.h>
#include "se05x_t4t_set_read.h"
#include "ex_sss_ports.h"
#include <crypto/hsm/nxp/CHIPCryptoPALHsm_SE05X_utils.h>
#include <fsl_sss_se05x_types.h>
#include <se05x_APDU_apis.h>
#include <se05x_APDU.h>

extern int se05x_session_open;

int se05x_lock_read(void)
{
#if SSS_HAVE_APPLET_SE051_H
    sss_se05x_session_t *pCtx = (sss_se05x_session_t *)&gex_sss_chip_ctx.session;
    smStatus_t smStatus       = SM_NOT_OK;

	se05x_sessionOpen();
    if(gex_sss_chip_ctx.ks.session == NULL){
    	return 1;
    }

    smStatus = Se05x_T4T_API_SelectT4TApplet(&(pCtx->s_ctx));
    if (smStatus != SM_OK){
    	return 1;
    }

    // Selecting t4t applet will deselect the opt applet. So need session open again
    se05x_session_open = 0;

    smStatus = Se05x_T4T_API_ConfigureAccessCtrl(
        &(pCtx->s_ctx), kSE05x_T4T_Interface_Contactless, kSE05x_T4T_Operation_Read, kSE05x_T4T_AccessCtrl_Denied);
    if (smStatus != SM_OK){
    	return 1;
    }
#endif
    return 0;
}

int se05x_unlock_read(void)
{
#if SSS_HAVE_APPLET_SE051_H
    sss_se05x_session_t *pCtx = (sss_se05x_session_t *)&gex_sss_chip_ctx.session;
    smStatus_t smStatus       = SM_NOT_OK;

	se05x_sessionOpen();
    if(gex_sss_chip_ctx.ks.session == NULL){
    	return 1;
    }

    smStatus = Se05x_T4T_API_SelectT4TApplet(&(pCtx->s_ctx));
    if (smStatus != SM_OK){
    	return 1;
    }

    // Selecting t4t applet will deselect the opt applet. So need session open again
    se05x_session_open = 0;

    smStatus = Se05x_T4T_API_ConfigureAccessCtrl(
        &(pCtx->s_ctx), kSE05x_T4T_Interface_Contactless, kSE05x_T4T_Operation_Read, kSE05x_T4T_AccessCtrl_Granted);
    if (smStatus != SM_OK){
    	return 1;
    }
#endif
    return 0;
}