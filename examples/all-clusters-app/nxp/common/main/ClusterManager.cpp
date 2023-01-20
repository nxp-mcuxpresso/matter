/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
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

#include "ClusterManager.h"

#include "AppTask.h"
#include "FreeRTOS.h"

#include "application_config.h"

ClusterManager ClusterManager::sCluster;

TimerHandle_t sClusterTimer; // FreeRTOS app sw timer.

CHIP_ERROR ClusterManager::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    // Create FreeRTOS sw timer for Cluster timer.

    sClusterTimer = xTimerCreate("ClusterTmr",       // Just a text name, not used by the RTOS kernel
                               1,                // == default timer period (mS)
                               false,            // no timer reload (==one-shot)
                               (void *) this,    // init timer id = Cluster obj context
                               TimerEventHandler // timer callback handler
    );

    if (sClusterTimer == NULL)
    {
        ChipLogProgress(DeviceLayer, "Cluster timer create failed");
        assert(0);
    }

    mState                = kState_TurnOffCompleted;
    mAutoTurnOnTimerArmed = false;
    mAutoTurnOn           = false;
    mAutoTurnOnDuration   = 0;

    return err;
}

void ClusterManager::SetCallbacks(Callback_fn_initiated aActionInitiated_CB, Callback_fn_completed aActionCompleted_CB)
{
    mActionInitiated_CB = aActionInitiated_CB;
    mActionCompleted_CB = aActionCompleted_CB;
}

bool ClusterManager::IsActionInProgress()
{
    return (mState == kState_TurnOnInitiated || mState == kState_TurnOffInitiated) ? true : false;
}

bool ClusterManager::IsTurnedOff()
{
    return (mState == kState_TurnOffCompleted) ? true : false;
}

void ClusterManager::EnableAutoTurnOn(bool aOn)
{
    mAutoTurnOn = aOn;
}

void ClusterManager::SetAutoTurnOnDuration(uint32_t aDurationInSecs)
{
    mAutoTurnOnDuration = aDurationInSecs;
}

bool ClusterManager::InitiateAction(int32_t aActor, Action_t aAction)
{
    bool action_initiated = false;
    State_t new_state;

    // Initiate ON/OFF Action only when the previous one is complete.
    if (mState == kState_TurnOnCompleted && aAction == TURNOFF_ACTION)
    {
        action_initiated = true;

        new_state = kState_TurnOffInitiated;
    }
    else if (mState == kState_TurnOffCompleted && aAction == TURNON_ACTION)
    {
        action_initiated = true;

        new_state = kState_TurnOnInitiated;
    }

    if (action_initiated)
    {
        if (mAutoTurnOnTimerArmed && new_state == kState_TurnOnInitiated)
        {
            // If auto turnon timer has been armed and someone initiates turn on the ligth,
            // cancel the timer and continue as normal.
            mAutoTurnOnTimerArmed = false;

            CancelTimer();
        }

        StartTimer(ACTUATOR_MOVEMENT_PERIOS_MS);

        // Since the timer started successfully, update the state and trigger callback
        mState = new_state;

        if (mActionInitiated_CB)
        {
            mActionInitiated_CB(aAction, aActor);
        }
    }

    return action_initiated;
}

void ClusterManager::StartTimer(uint32_t aTimeoutMs)
{
    if (xTimerIsTimerActive(sClusterTimer))
    {
        ChipLogProgress(DeviceLayer, "Cluster timer already started!");
        // appError(err);
        CancelTimer();
    }

    // timer is not active, change its period to required value.
    // This also causes the timer to start.  FreeRTOS- Block for a maximum of
    // 100 ticks if the  change period command cannot immediately be sent to the
    // timer command queue.
    if (xTimerChangePeriod(sClusterTimer, aTimeoutMs / portTICK_PERIOD_MS, 100) != pdPASS)
    {
        ChipLogProgress(DeviceLayer, "Cluster timer start() failed");
        // appError(err);
    }
}

void ClusterManager::CancelTimer(void)
{
    if (xTimerStop(sClusterTimer, 0) == pdFAIL)
    {
        ChipLogProgress(DeviceLayer, "Cluster timer stop() failed");
        // appError(err);
    }
}

void ClusterManager::TimerEventHandler(TimerHandle_t xTimer)
{
    // Get Cluster obj context from timer id.
    ClusterManager * Cluster = static_cast<ClusterManager *>(pvTimerGetTimerID(xTimer));

    // The timer event handler will be called in the context of the timer task
    // once sClusterTimer expires. Post an event to apptask queue with the actual handler
    // so that the event can be handled in the context of the apptask.
    AppEvent event;
    event.Type               = AppEvent::kEventType_Timer;
    event.TimerEvent.Context = Cluster;

    if (Cluster->mAutoTurnOnTimerArmed)
    {
        event.Handler = AutoReTurnOnTimerEventHandler;
        GetAppTask().PostEvent(&event);
    }
    else
    {
        event.Handler = ActuatorMovementTimerEventHandler;
        GetAppTask().PostEvent(&event);
    }
}

void ClusterManager::AutoReTurnOnTimerEventHandler(AppEvent * aEvent)
{
    ClusterManager * Cluster = static_cast<ClusterManager *>(aEvent->TimerEvent.Context);
    int32_t actor           = 0;

    // Make sure auto Cluster timer is still armed.
    if (!Cluster->mAutoTurnOnTimerArmed)
    {
        return;
    }

    Cluster->mAutoTurnOnTimerArmed = false;

    ChipLogProgress(DeviceLayer, "Auto Turn On has been triggered!");

    Cluster->InitiateAction(actor, TURNON_ACTION);
}

void ClusterManager::ActuatorMovementTimerEventHandler(AppEvent * aEvent)
{
    Action_t actionCompleted = INVALID_ACTION;

    ClusterManager * Cluster = static_cast<ClusterManager *>(aEvent->TimerEvent.Context);

    if (Cluster->mState == kState_TurnOnInitiated)
    {
        Cluster->mState   = kState_TurnOnCompleted;
        actionCompleted = TURNON_ACTION;
    }
    else if (Cluster->mState == kState_TurnOffInitiated)
    {
        Cluster->mState   = kState_TurnOffCompleted;
        actionCompleted = TURNOFF_ACTION;
    }

    if (actionCompleted != INVALID_ACTION)
    {
        if (Cluster->mActionCompleted_CB)
        {
            Cluster->mActionCompleted_CB(actionCompleted);
        }

        if (Cluster->mAutoTurnOn && actionCompleted == TURNOFF_ACTION)
        {
            // Start the timer for auto turn on
            Cluster->StartTimer(Cluster->mAutoTurnOnDuration * 1000);

            Cluster->mAutoTurnOnTimerArmed = true;

            ChipLogProgress(DeviceLayer, "Auto Turn On enabled. Will be triggered in %lu seconds", Cluster->mAutoTurnOnDuration);
        }
    }
}
