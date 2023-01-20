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

#include <src/app/clusters/ota-requestor/OTARequestorInterface.h>
#include <app/clusters/ota-requestor/OTADownloader.h>

#include "OTAImageProcessorImpl.h"

#include "OtaSupport.h"

extern "C" void HAL_ResetMCU(void);

#define ResetMCU HAL_ResetMCU

using namespace chip::DeviceLayer;
using namespace ::chip::DeviceLayer::Internal;

namespace chip {

CHIP_ERROR OTAImageProcessorImpl::PrepareDownload()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandlePrepareDownload, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Finalize()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleFinalize, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Apply()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleApply, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Abort()
{    
    if (mImageFile == nullptr)
    {
        ChipLogError(SoftwareUpdate, "Invalid output image file supplied");
        return CHIP_ERROR_INTERNAL;
    }

    DeviceLayer::PlatformMgr().ScheduleWork(HandleAbort, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::ProcessBlock(ByteSpan & block)
{
    if ((block.data() == nullptr) || block.empty())
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    /* Store block data for HandleProcessBlock to access */
    CHIP_ERROR err = SetBlock(block);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Cannot set block data: %" CHIP_ERROR_FORMAT, err.Format());
    }

    DeviceLayer::PlatformMgr().ScheduleWork(HandleProcessBlock, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

void OTAImageProcessorImpl::TriggerNewRequestForData()
{
    if (this->mDownloader)
    {
        PlatformMgr().LockChipStack();
        this->mDownloader->FetchNextData();
        PlatformMgr().UnlockChipStack();
    }
}

void OTAImageProcessorImpl::HandlePrepareDownload(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }
    else if (imageProcessor->mDownloader == nullptr)
    {
        ChipLogError(SoftwareUpdate, "mDownloader is null");
        return;
    }
    
    /* Initialize OTA External Storage Memory */
    imageProcessor->mOTAPartition = PARTITION_START;
    imageProcessor->mOTAPartitionSize = PARTITION_SIZE;

    if(OTA_SelectExternalStoragePartition(imageProcessor->mOTAPartition, imageProcessor->mOTAPartitionSize) != gOtaSuccess_c)
    {
        ChipLogError(SoftwareUpdate, "Failed to select valid External Flash partition");
    }
    
    /* Initialize OTA service for posted operations */
    if(gOtaSuccess_c == OTA_ServiceInit(&imageProcessor->mPostedOperationsStorage[0], NB_PENDING_TRANSACTIONS*TRANSACTION_SZ))
    {
        imageProcessor->mHeaderParser.Init();
        imageProcessor->mDownloader->OnPreparedForDownload(CHIP_NO_ERROR);
    }
}

CHIP_ERROR OTAImageProcessorImpl::ProcessHeader(ByteSpan & block)
{
    OTAImageHeader header;
    CHIP_ERROR error = mHeaderParser.AccumulateAndDecode(block, header);

    /* Needs more data to decode the header */
    ReturnErrorCodeIf(error == CHIP_ERROR_BUFFER_TOO_SMALL, CHIP_NO_ERROR);
    ReturnErrorOnFailure(error);

    mParams.totalFileBytes = header.mPayloadSize;
    mSoftwareVersion = header.mSoftwareVersion;
    mHeaderParser.Clear();

    return CHIP_NO_ERROR;
}

void OTAImageProcessorImpl::HandleAbort(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        return;
    }

    remove(imageProcessor->mImageFile);
    imageProcessor->ReleaseBlock();
}

void OTAImageProcessorImpl::HandleProcessBlock(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    static uint32_t ulEraseAddr = 0;
    static uint32_t ulEraseLen = 0;
    static uint32_t ulCrtAddr = 0;

    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }
    else if (imageProcessor->mDownloader == nullptr)
    {
        ChipLogError(SoftwareUpdate, "mDownloader is null");
        return;
    }
    CHIP_ERROR error = CHIP_NO_ERROR;

    /* Process Header of the received OTA block if mHeaderParser is in Initialized state */
    if (imageProcessor->mHeaderParser.IsInitialized())
    {
        ByteSpan block = ByteSpan(imageProcessor->mBlock.data(), imageProcessor->mBlock.size());

        error = imageProcessor->ProcessHeader(block);

        if (error == CHIP_NO_ERROR)
        {
            /* Start the OTA Image writing session */
            if (gOtaSuccess_c == OTA_StartImage(imageProcessor->mParams.totalFileBytes))
            {
                uint8_t *ptr = static_cast<uint8_t *>(chip::Platform::MemoryAlloc(block.size()));
                if (ptr != nullptr)
                {
                    MutableByteSpan mutableBlock = MutableByteSpan(ptr, block.size());
                    error = CopySpanToMutableSpan(block, mutableBlock);

                    if (error == CHIP_NO_ERROR)
                    {
                        imageProcessor->ReleaseBlock();
                        imageProcessor->mBlock = MutableByteSpan(mutableBlock.data(), mutableBlock.size());
                    }
                }
                else
                {
                    error = CHIP_ERROR_NO_MEMORY;
                }
            }
            else
            {
                error = CHIP_ERROR_INTERNAL;
            }
        }
    }
    /* Stop the downloading process if header isn't valid */
    if(error != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Failed to process OTA image header");
        imageProcessor->mDownloader->EndDownload(error);
        return;
    }

    // For testing purpose
    if(imageProcessor->mBlock.data()!=nullptr)
    {
        ChipLogProgress(SoftwareUpdate, "OTA Block received, preparing for writing in flash...");
        ChipLogProgress(SoftwareUpdate, "OTA Block size : %d", imageProcessor->mBlock.size());
    }

    ulCrtAddr += imageProcessor->mBlock.size();

    if ( ulCrtAddr >= ulEraseLen )
    { // need to erase
        ulEraseAddr = ulEraseLen;
        ulEraseLen += 0x1000; // flash sector size

        ota_op_completion_cb_t callback = HandleBlockEraseComplete;

        /* Erase enough space to store the block before writing
           HandleBlockEraseComplete Callback is called after data is written and transaction Queue is empty */
        if (gOtaSuccess_c != OTA_MakeHeadRoomForNextBlock(imageProcessor->mBlock.size(), callback, (uint32_t) imageProcessor))
        {
            ChipLogProgress(SoftwareUpdate, "OTA erase error");
        }
    }
    else
    {
        HandleBlockEraseComplete((uint32_t) imageProcessor);
    }
    
    /* Send block to be written in external flash */
    if (gOtaSuccess_c == OTA_PushImageChunk(imageProcessor->mBlock.data(), (uint16_t) imageProcessor->mBlock.size(), NULL, NULL))
    {
        imageProcessor->mParams.downloadedBytes += imageProcessor->mBlock.size();
        // For test purpose
        ChipLogProgress(SoftwareUpdate, "OTA Block writing transaction successfully submitted esd=%lu crt=%lu", ulEraseAddr, ulCrtAddr);
        return;
    }
    else
    {
        ChipLogProgress(SoftwareUpdate, "OTA Block error writing transaction NOT submitted");
    }
}

bool OTAImageProcessorImpl::IsFirstImageRun()
{
    OTARequestorInterface * requestor = chip::GetRequestorInstance();
    if (requestor == nullptr)
    {
        return false;
    }

    return requestor->GetCurrentUpdateState() == OTARequestorInterface::OTAUpdateStateEnum::kApplying;
}

CHIP_ERROR OTAImageProcessorImpl::ConfirmCurrentImage()
{
    OTARequestorInterface * requestor = chip::GetRequestorInstance();
    if (requestor == nullptr)
    {
        return CHIP_ERROR_INTERNAL;
    }

    uint32_t currentVersion;
    uint32_t targetVersion = requestor->GetTargetVersion();
    ReturnErrorOnFailure(DeviceLayer::ConfigurationMgr().GetSoftwareVersion(currentVersion));
    if (currentVersion != targetVersion)
    {
        ChipLogError(SoftwareUpdate,
            "Current sw version %" PRIu32 " is different than the expected sw version = %" PRIu32,
            currentVersion, targetVersion);
        return CHIP_ERROR_INCORRECT_STATE;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::SetBlock(ByteSpan & block)
{
    if (!IsSpanUsable(block))
    {
        ReleaseBlock();
        return CHIP_NO_ERROR;
    }
    if (mBlock.size() < block.size())
    {
        if (!mBlock.empty())
        {
            ReleaseBlock();
        }
        /* Allocate memory for block data */
        uint8_t * mBlock_ptr = static_cast<uint8_t *>(chip::Platform::MemoryAlloc(block.size()));
        if (mBlock_ptr == nullptr)
        {
            return CHIP_ERROR_NO_MEMORY;
        }
        mBlock = MutableByteSpan(mBlock_ptr, block.size());
    }
    /* Store the actual block data */
    CHIP_ERROR err = CopySpanToMutableSpan(block, mBlock);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Cannot copy block data: %" CHIP_ERROR_FORMAT, err.Format());
        return err;
    }
    return CHIP_NO_ERROR;
}

void OTAImageProcessorImpl::HandleFinalize(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        return;
    }

    imageProcessor->ReleaseBlock();

    ChipLogProgress(SoftwareUpdate, "OTA Image download complete");
}

void OTAImageProcessorImpl::HandleApply(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);

    if (imageProcessor == nullptr)
    {
        return;
    }

    ChipLogProgress(SoftwareUpdate, "OTA Handle Apply prepare for RESET");

    /* Finalize the writing of the OTA image in flash */
    OTA_CommitImage(NULL);

    ChipLogProgress(SoftwareUpdate, "OTA image authentication success. Device will reboot with the new image!");
    // Set the necessary information to inform the SSBL that a new image is available
    // and trigger the actual device reboot after some time, to take into account
    // queued actions, e.g. sending events to a subscription
    SystemLayer().StartTimer(
        chip::System::Clock::Milliseconds32(CHIP_DEVICE_LAYER_OTA_REBOOT_DELAY),
        [](chip::System::Layer *, void *) {
            OTA_SetNewImageFlag ();
            ResetMCU ();
        },
        nullptr
    );
}

CHIP_ERROR OTAImageProcessorImpl::ReleaseBlock()
{
    if (mBlock.data() != nullptr)
    {
        chip::Platform::MemoryFree(mBlock.data());
    }

    mBlock = MutableByteSpan();
    return CHIP_NO_ERROR;
}

void OTAImageProcessorImpl::HandleBlockEraseComplete(uint32_t param)
{
    CHIP_ERROR error = CHIP_NO_ERROR;

    ChipDeviceEvent otaChange;
    otaChange.Type                     = DeviceEventType::kOtaStateChanged;
    otaChange.OtaStateChanged.newState = kOtaSpaceAvailable;
    error                              = PlatformMgr().PostEvent(&otaChange);

    ChipLogProgress(SoftwareUpdate, "OTA posted event");

    if (error != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Error while posting OtaChange event");
    }
}

}// namespace chip