/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include "AudioOutputManager.h"

using namespace std;
using namespace chip::app;
using namespace chip::app::Clusters::AudioOutput;

AudioOutputManager::AudioOutputManager()
{
    mCurrentOutput = 1;
    if (mAudioOutput.Init()) {
        ChipLogError(NotSpecified, "Audio Output Device failed to Init");
        return;
    }

    std::vector<std::string> sinks = mAudioOutput.GetSinks();
    uint8_t index = 0;
    for (const auto& sink : sinks) {
        OutputInfoType outputInfo;
        if (sink.find("bt") != std::string::npos) {
            outputInfo.outputType = chip::app::Clusters::AudioOutput::OutputTypeEnum::kBt;
            outputInfo.name = chip::CharSpan::fromCharString("Bluetooth");
        } else if (sink.find("wm8") != std::string::npos) {
            outputInfo.name = chip::CharSpan::fromCharString("Audio Jack");
            outputInfo.outputType = chip::app::Clusters::AudioOutput::OutputTypeEnum::kOther;
        } else {
            outputInfo.name = chip::CharSpan::fromCharString("Other");
            outputInfo.outputType = chip::app::Clusters::AudioOutput::OutputTypeEnum::kOther;
        }
        outputInfo.index = static_cast<uint8_t>(index);
        mOutputs.push_back(outputInfo);
        index++;
    }
}

uint8_t AudioOutputManager::HandleGetCurrentOutput()
{
    return mCurrentOutput;
}

CHIP_ERROR AudioOutputManager::HandleGetOutputList(AttributeValueEncoder & aEncoder)
{
    // TODO: Insert code here
    return aEncoder.EncodeList([this](const auto & encoder) -> CHIP_ERROR {
        for (auto const & outputInfo : this->mOutputs)
        {
            ReturnErrorOnFailure(encoder.Encode(outputInfo));
        }
        return CHIP_NO_ERROR;
    });
}

bool AudioOutputManager::HandleRenameOutput(const uint8_t & index, const chip::CharSpan & name)
{
    // TODO: Insert code here
    bool audioOutputRenamed = false;

    for (OutputInfoType & output : mOutputs)
    {
        if (output.index == index)
        {
            audioOutputRenamed = true;
            memcpy(this->Data(index), name.data(), name.size());
            output.name = chip::CharSpan(this->Data(index), name.size());
        }
    }

    return audioOutputRenamed;
}

bool AudioOutputManager::HandleSelectOutput(const uint8_t & index)
{
    // TODO: Insert code here
    bool audioOutputSelected = false;
    for (OutputInfoType & output : mOutputs)
    {
        if (output.index == index)
        {
            audioOutputSelected = true;
            mCurrentOutput      = index;
            if (mAudioOutput.Select(index)) {
                ChipLogError(NotSpecified,"Failed to select output:%s", output.name);
                return false;
            }
        }
    }

    return audioOutputSelected;
}
