/**
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

#include "MediaPlaybackManager.h"
#include "MediaIPCHelper.h"
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/util/config.h>
#include <protocols/interaction_model/StatusCode.h>
#include <protocols/interaction_model/Constants.h>

using namespace std;
using namespace chip::app::DataModel;
using namespace chip::app::Clusters::MediaPlayback;
using namespace chip::Uint8;
using chip::CharSpan;
using chip::Protocols::InteractionModel::Status;

#define CHECKSERVICE() \
    do { \
        if (gMediaIPCHelper->PlayerStatus() != ServiceActiveState::Active) { \
            helper.Failure(Status::Failure); \
            return; \
        } \
    } while(0)

#define CHECKRESULT(val) \
    do { \
        if (val) { \
            gMediaIPCHelper->StopPlayer(); \
            gMediaIPCHelper->StartPlayer(); \
            helper.Failure(Status::Failure); \
            return; \
        } \
    } while(0)


PlaybackStateEnum MediaPlaybackManager::HandleGetCurrentState()
{
    if (gMediaIPCHelper->PlayerStatus() != ServiceActiveState::Active)
        return PlaybackStateEnum::kUnknownEnumValue;

    mCurrentState = gMediaIPCHelper->GetCurrentStatus();
    return mCurrentState;
}

uint64_t MediaPlaybackManager::HandleGetStartTime()
{
    return 0;
}

uint64_t MediaPlaybackManager::HandleGetDuration()
{
    if (gMediaIPCHelper->PlayerStatus() != ServiceActiveState::Active)
        return 0;

    mDuration = gMediaIPCHelper->GetDuration();
    return mDuration;
}

CHIP_ERROR MediaPlaybackManager::HandleGetSampledPosition(AttributeValueEncoder & aEncoder)
{
    uint64_t position;
    if (gMediaIPCHelper->PlayerStatus() != ServiceActiveState::Active) {
        position = 0;
    }
    else {
        position = gMediaIPCHelper->GetPosition();
    }

    mPlaybackPosition.position = chip::app::DataModel::Nullable<uint64_t>(position);
    return aEncoder.Encode(mPlaybackPosition);
}

float MediaPlaybackManager::HandleGetPlaybackSpeed()
{
    if (gMediaIPCHelper->PlayerStatus() != ServiceActiveState::Active)
        return 0;

    mPlaybackSpeed = gMediaIPCHelper->GetPlaybackSpeed();
    return mPlaybackSpeed;
}

uint64_t MediaPlaybackManager::HandleGetSeekRangeStart()
{
    return 0;
}

uint64_t MediaPlaybackManager::HandleGetSeekRangeEnd()
{
    if (gMediaIPCHelper->PlayerStatus() != ServiceActiveState::Active)
        return 0;

    mDuration = gMediaIPCHelper->GetDuration();
    return mDuration;
}
CHIP_ERROR MediaPlaybackManager::HandleGetActiveAudioTrack(AttributeValueEncoder & aEncoder)
{
    return aEncoder.Encode(mActiveAudioTrack);
}

CHIP_ERROR MediaPlaybackManager::HandleGetAvailableAudioTracks(AttributeValueEncoder & aEncoder)
{
    // TODO: Insert code here
    return aEncoder.EncodeList([this](const auto & encoder) -> CHIP_ERROR {
        for (auto const & audioTrack : mAvailableAudioTracks)
        {
            ReturnErrorOnFailure(encoder.Encode(audioTrack));
        }
        return CHIP_NO_ERROR;
    });
}

CHIP_ERROR MediaPlaybackManager::HandleGetActiveTextTrack(AttributeValueEncoder & aEncoder)
{
    return aEncoder.Encode(mActiveTextTrack);
}

CHIP_ERROR MediaPlaybackManager::HandleGetAvailableTextTracks(AttributeValueEncoder & aEncoder)
{
    // TODO: Insert code here
    return aEncoder.EncodeList([this](const auto & encoder) -> CHIP_ERROR {
        for (auto const & textTrack : mAvailableTextTracks)
        {
            ReturnErrorOnFailure(encoder.Encode(textTrack));
        }
        return CHIP_NO_ERROR;
    });
}
void MediaPlaybackManager::HandlePlay(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper)
{
    CHECKSERVICE();
    // TODO: Insert code here
    mPlaybackSpeed = 1;
    CHECKRESULT(gMediaIPCHelper->Notify("c 1"));
    mCurrentState = gMediaIPCHelper->GetCurrentStatus();

    if (mCurrentState == chip::app::Clusters::MediaPlayback::PlaybackStateEnum::kPaused) {
       CHECKRESULT(gMediaIPCHelper->Notify("a"));
    }else {
       CHECKRESULT(gMediaIPCHelper->Notify("p"));
    }

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandlePause(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper)
{
    CHECKSERVICE();
    // TODO: Insert code here
    mPlaybackSpeed = 0;
    CHECKRESULT(gMediaIPCHelper->Notify("c 1"));
    CHECKRESULT(gMediaIPCHelper->Notify("a"));

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandleStop(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper)
{
    CHECKSERVICE();
    // TODO: Insert code here
    mPlaybackSpeed    = 0;
    mPlaybackPosition = { 0, chip::app::DataModel::Nullable<uint64_t>(0) };
    CHECKRESULT(gMediaIPCHelper->Notify("c 1"));
    CHECKRESULT(gMediaIPCHelper->Notify("s"));

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandleFastForward(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper,
                                             const chip::Optional<bool> & audioAdvanceUnmuted)
{
    CHECKSERVICE();
    // TODO: Insert code here
    char buf[10] = {0};
    mPlaybackSpeed = gMediaIPCHelper->GetPlaybackSpeed();
    if (mPlaybackSpeed >= kPlaybackMaxForwardSpeed)
    {
        // if already at max speed, return error
        Commands::PlaybackResponse::Type response;
        response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
        response.status = StatusEnum::kSpeedOutOfRange;
        helper.Success(response);
        return;
    }

    mPlaybackSpeed = (mPlaybackSpeed <= 0 ? 1 : mPlaybackSpeed * 2);
    if (mPlaybackSpeed > kPlaybackMaxForwardSpeed)
    {
        // don't exceed max speed
        mPlaybackSpeed = kPlaybackMaxForwardSpeed;
    }
    sprintf(buf, "c %1.2f", mPlaybackSpeed);
    CHECKRESULT(gMediaIPCHelper->Notify(buf));

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandlePrevious(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper)
{
    CHECKSERVICE();
    // TODO: Insert code here
    mPlaybackSpeed    = 1;
    CHECKRESULT(gMediaIPCHelper->Notify("<"));
    mPlaybackPosition = { 0, chip::app::DataModel::Nullable<uint64_t>(0) };

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandleRewind(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper,
                                        const chip::Optional<bool> & audioAdvanceUnmuted)
{
    CHECKSERVICE();
    // TODO: Insert code here
    char buf[10] = {0};
    mPlaybackSpeed = gMediaIPCHelper->GetPlaybackSpeed();
    if (mPlaybackSpeed <= kPlaybackMaxRewindSpeed)
    {
        // if already at max speed in reverse, return error
        Commands::PlaybackResponse::Type response;
        response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
        response.status = StatusEnum::kSpeedOutOfRange;
        helper.Success(response);
        return;
    }

    mPlaybackSpeed = (mPlaybackSpeed >= 0 ? -1 : mPlaybackSpeed * 2);
    if (mPlaybackSpeed < kPlaybackMaxRewindSpeed)
    {
        // don't exceed max rewind speed
        mPlaybackSpeed = kPlaybackMaxRewindSpeed;
    }
    sprintf(buf, "c %1.2f", mPlaybackSpeed);
    CHECKRESULT(gMediaIPCHelper->Notify(buf));

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandleSkipBackward(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper,
                                              const uint64_t & deltaPositionMilliseconds)
{
    CHECKSERVICE();
    // TODO: Insert code here
    auto position = gMediaIPCHelper->GetPosition();
    mPlaybackPosition.position = chip::app::DataModel::Nullable<uint64_t>(position);
    uint64_t newPosition = (mPlaybackPosition.position.Value() > deltaPositionMilliseconds
                                ? mPlaybackPosition.position.Value() - deltaPositionMilliseconds
                                : 0);
    mPlaybackPosition    = { 0, chip::app::DataModel::Nullable<uint64_t>(newPosition) };
    uint64_t positionSeconds = newPosition/1000;
    char buf[80] = {0};

    sprintf(buf, "e 0 t%ld", positionSeconds);
    CHECKRESULT(gMediaIPCHelper->Notify(buf));

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandleSkipForward(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper,
                                             const uint64_t & deltaPositionMilliseconds)
{
    CHECKSERVICE();
    // TODO: Insert code here
    auto position = gMediaIPCHelper->GetPosition();
    mPlaybackPosition.position = chip::app::DataModel::Nullable<uint64_t>(position);
    uint64_t newPosition = mPlaybackPosition.position.Value() + deltaPositionMilliseconds;
    auto duration = gMediaIPCHelper->GetDuration();
    newPosition          = newPosition > duration ? duration : newPosition;
    mPlaybackPosition    = { 0, chip::app::DataModel::Nullable<uint64_t>(newPosition) };

    uint64_t positionSeconds = newPosition/1000;
    char buf[80] = {0};
    sprintf(buf, "e 0 t%ld", positionSeconds);
    CHECKRESULT(gMediaIPCHelper->Notify(buf));

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandleSeek(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper,
                                      const uint64_t & positionMilliseconds)
{
    CHECKSERVICE();
    // TODO: Insert code here
    mDuration = gMediaIPCHelper->GetDuration();
    if (positionMilliseconds > mDuration)
    {
        Commands::PlaybackResponse::Type response;
        response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
        response.status = StatusEnum::kSeekOutOfRange;
        helper.Success(response);
    }
    else
    {
        mPlaybackPosition = { 0, chip::app::DataModel::Nullable<uint64_t>(positionMilliseconds) };
        uint64_t positionSeconds = positionMilliseconds/1000;
        char buf[80] = {0};

        sprintf(buf, "e 0 t%ld", positionSeconds);
        CHECKRESULT(gMediaIPCHelper->Notify(buf));

        Commands::PlaybackResponse::Type response;
        response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
        response.status = StatusEnum::kSuccess;
        helper.Success(response);
    }
}

void MediaPlaybackManager::HandleNext(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper)
{
    CHECKSERVICE();
    // TODO: Insert code here
    mPlaybackSpeed    = 1;
    CHECKRESULT(gMediaIPCHelper->Notify("c 1"));
    mPlaybackPosition = { 0, chip::app::DataModel::Nullable<uint64_t>(0) };

    CHECKRESULT(gMediaIPCHelper->Notify(">"));

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

void MediaPlaybackManager::HandleStartOver(CommandResponseHelper<Commands::PlaybackResponse::Type> & helper)
{
    CHECKSERVICE();
    // TODO: Insert code here
    mPlaybackPosition = { 0, chip::app::DataModel::Nullable<uint64_t>(0) };
    CHECKRESULT(gMediaIPCHelper->Notify("s"));
    CHECKRESULT(gMediaIPCHelper->Notify("p"));

    Commands::PlaybackResponse::Type response;
    response.data   = chip::MakeOptional(CharSpan::fromCharString("data response"));
    response.status = StatusEnum::kSuccess;
    helper.Success(response);
}

bool MediaPlaybackManager::HandleActivateAudioTrack(const chip::CharSpan & trackId, const uint8_t & audioOutputIndex)
{
    std::string idString(trackId.data(), trackId.size());
    for (auto const & availableAudioTrack : mAvailableAudioTracks)
    {
        std::string nextIdString(availableAudioTrack.id.data(), availableAudioTrack.id.size());
        if (nextIdString == idString)
        {
            mActiveAudioTrack = availableAudioTrack;
            return true;
        }
    }

    return false;
}

bool MediaPlaybackManager::HandleActivateTextTrack(const chip::CharSpan & trackId)
{
    std::string idString(trackId.data(), trackId.size());
    for (auto const & availableTextTrack : mAvailableTextTracks)
    {
        std::string nextIdString(availableTextTrack.id.data(), availableTextTrack.id.size());
        if (nextIdString == idString)
        {
            mActiveTextTrack = availableTextTrack;
            return true;
        }
    }

    return false;
}

bool MediaPlaybackManager::HandleDeactivateTextTrack()
{
    // Handle Deactivate Text Track
    if (mActiveTextTrack.id.data() != nullptr)
    {
        mActiveTextTrack = {};
    }
    return true;
}

uint32_t MediaPlaybackManager::GetFeatureMap(chip::EndpointId endpoint)
{
    if (endpoint >= MATTER_DM_CONTENT_LAUNCHER_CLUSTER_SERVER_ENDPOINT_COUNT)
    {
        return kEndpointFeatureMap;
    }

    uint32_t featureMap = 0;
    Attributes::FeatureMap::Get(endpoint, &featureMap);
    return featureMap;
}

uint16_t MediaPlaybackManager::GetClusterRevision(chip::EndpointId endpoint)
{
    if (endpoint >= MATTER_DM_CONTENT_LAUNCHER_CLUSTER_SERVER_ENDPOINT_COUNT)
    {
        return kClusterRevision;
    }

    uint16_t clusterRevision = 0;
    bool success =
        (Attributes::ClusterRevision::Get(endpoint, &clusterRevision) == chip::Protocols::InteractionModel::Status::Success);
    if (!success)
    {
        ChipLogError(Zcl, "MediaPlaybackManager::GetClusterRevision error reading cluster revision");
    }
    return clusterRevision;
}
