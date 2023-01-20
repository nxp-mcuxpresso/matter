/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
 *    All rights reserved.
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

#pragma once

#include <lib/core/OTAImageHeader.h>
#include <app/clusters/ota-requestor/OTADownloader.h>
#include <include/platform/CHIPDeviceLayer.h>
#include <include/platform/OTAImageProcessor.h>
#include "OtaSupport.h"

/* Posted Operations Size Info */
#define NB_PENDING_TRANSACTIONS 12
#define TRANSACTION_SZ (sizeof(FLASH_TransactionOpNode_t))

/* External Flash Partition info */
#define PARTITION_START ((uint32_t) 0) /* Start of OTA Image partition */
#define PARTITION_SIZE ((uint32_t)(1024*1024)) /* Size of OTA Image partition */

namespace chip {

class OTAImageProcessorImpl : public OTAImageProcessorInterface
{
public:
    //////////// OTAImageProcessorInterface Implementation ///////////////
    CHIP_ERROR PrepareDownload() override;
    CHIP_ERROR Finalize() override;
    CHIP_ERROR Apply() override;
    CHIP_ERROR Abort() override;
    CHIP_ERROR ProcessBlock(ByteSpan & block) override;
    bool IsFirstImageRun() override;
    CHIP_ERROR ConfirmCurrentImage() override;

    void TriggerNewRequestForData();
    void SetOTADownloader(OTADownloader * downloader) { mDownloader = downloader; }
    void SetOTAImageFile(const char * imageFile) { mImageFile = imageFile; }

private:
    //////////// Actual handlers for the OTAImageProcessorInterface ///////////////
    static void HandlePrepareDownload(intptr_t context);
    CHIP_ERROR ProcessHeader(ByteSpan & block);
    static void HandleFinalize(intptr_t context);
    static void HandleAbort(intptr_t context);
    static void HandleProcessBlock(intptr_t context);
    static void HandleApply(intptr_t context);
    static void HandleBlockEraseComplete(uint32_t param);

    /**
     * Called to allocate memory for mBlock if necessary and set it to block
     */
    CHIP_ERROR SetBlock(ByteSpan & block);

    /**
     * Called to release allocated memory for mBlock
     */
    CHIP_ERROR ReleaseBlock();

    OTADownloader * mDownloader;
    OTAImageHeaderParser mHeaderParser;
    MutableByteSpan mBlock;
    const char * mImageFile = nullptr;
    uint32_t mSoftwareVersion;

    /** Partition Info  (can be replaced by mcuboot's defined struct partition_t) **/
    uint32_t mOTAPartition; /* Flash partition for OTA Image storage */
    uint32_t mOTAPartitionSize; /* Size of partition */

    /* Buffer used for transaction storage */
    uint8_t mPostedOperationsStorage[NB_PENDING_TRANSACTIONS*TRANSACTION_SZ];
};

} // namespace chip
