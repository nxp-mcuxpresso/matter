/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
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

#include "Device.h"

#include <cstdio>
#include <time.h>
#include <platform/CHIPDeviceLayer.h>

#include "zcb.h"

using namespace chip::app::Clusters::Actions;

Device::Device(const char * szDeviceName, std::string szLocation)
{
    chip::Platform::CopyString(mName, szDeviceName);
    mLocation   = szLocation;
    mReachable  = false;
    mEndpointId = 0;

    device_mutex = PTHREAD_MUTEX_INITIALIZER;
    device_cond =  PTHREAD_COND_INITIALIZER;
}

bool Device::IsReachable()
{
    return mReachable;
}

void Device::SetReachable(bool aReachable)
{
    bool changed = (mReachable != aReachable);

    mReachable = aReachable;

    if (aReachable)
    {
        ChipLogProgress(DeviceLayer, "Device[%s]: ONLINE", mName);
    }
    else
    {
        ChipLogProgress(DeviceLayer, "Device[%s]: OFFLINE", mName);
    }

    if (changed)
    {
        HandleDeviceChange(this, kChanged_Reachable);
    }
}

void Device::SetName(const char * szName)
{
    bool changed = (strncmp(mName, szName, sizeof(mName)) != 0);

    ChipLogProgress(DeviceLayer, "Device[%s]: New Name=\"%s\"", mName, szName);

    chip::Platform::CopyString(mName, szName);

    if (changed)
    {
        HandleDeviceChange(this, kChanged_Name);
    }
}

void Device::SetLocation(std::string szLocation)
{
    bool changed = (mLocation.compare(szLocation) != 0);

    mLocation = szLocation;

    ChipLogProgress(DeviceLayer, "Device[%s]: Location=\"%s\"", mName, mLocation.c_str());

    if (changed)
    {
        HandleDeviceChange(this, kChanged_Location);
    }
}

DeviceOnOff::DeviceOnOff(const char * szDeviceName, std::string szLocation) : Device(szDeviceName, szLocation)
{
    mOn = true;
}

bool DeviceOnOff::IsOn()
{
    return mOn;
}

void DeviceOnOff::SetOnOff(bool aOn)
{
    int result;
    bool changed;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    // timeout time is 2s
    ts.tv_sec += 2;

    changed = aOn ^ mOn;

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_OnOff);
        if(eOnOff(uint16_t(this->GetZigbeeSaddr()), aOn) != 0)
        {
            ChipLogProgress(DeviceLayer, "Send Device[%s]: %s cmd Failed", mName, aOn ? "ON" : "OFF");
            mOn = !mOn;
            goto done;
        }

        pthread_mutex_lock(&device_mutex);
        while(mOn != aOn) {
            result = pthread_cond_timedwait(&device_cond, &device_mutex, &ts);
            if(result == ETIMEDOUT) {
                ChipLogProgress(DeviceLayer, "Timeout waiting for response from Device[%s]", mName);
                SetReachable(false);
                break;
            } else {
                ChipLogProgress(DeviceLayer, "Set Device[%s]: Success", mName);
                mOn = aOn;
            }
        }
        pthread_mutex_unlock(&device_mutex);
    }

done:
    ChipLogProgress(DeviceLayer, "Device[%s]: %s", mName, mOn ? "ON" : "OFF");
}

void DeviceOnOff::SyncOnOff(bool aOn)
{
    pthread_mutex_lock(&device_mutex);
    mOn     = aOn;
    pthread_cond_signal(&device_cond);
    pthread_mutex_unlock(&device_mutex);

}

void DeviceOnOff::GetOnOff()
{
    newdb_zcb_t zcb;
    newDbGetZcbSaddr(this->GetZigbeeSaddr(), &zcb);

    if( eReadOnoff( uint16_t(zcb.saddr)) == 0)
    {
        this->SetReachable(true);
        if ( strcmp(zcb.info, "off") == 0) {
            mOn = false;
        } else {
            mOn = true;
        }
    } else {
        this->SetReachable(false);
    }
}

void DeviceOnOff::Toggle()
{
    bool aOn = !IsOn();
    SetOnOff(aOn);
}

void DeviceOnOff::SetChangeCallback(DeviceCallback_fn aChanged_CB)
{
    mChanged_CB = aChanged_CB;
}

void DeviceOnOff::HandleDeviceChange(Device * device, Device::Changed_t changeMask)
{
    if (mChanged_CB)
    {
        mChanged_CB(this, (DeviceOnOff::Changed_t) changeMask);
    }
}

DeviceSwitch::DeviceSwitch(const char * szDeviceName, std::string szLocation, uint32_t aFeatureMap) :
    Device(szDeviceName, szLocation)
{
    mNumberOfPositions = 2;
    mCurrentPosition   = 0;
    mMultiPressMax     = 2;
    mFeatureMap        = aFeatureMap;
}

void DeviceSwitch::SetNumberOfPositions(uint8_t aNumberOfPositions)
{
    bool changed;

    changed            = aNumberOfPositions != mNumberOfPositions;
    mNumberOfPositions = aNumberOfPositions;

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_NumberOfPositions);
    }
}

void DeviceSwitch::SetCurrentPosition(uint8_t aCurrentPosition)
{
    bool changed;

    changed          = aCurrentPosition != mCurrentPosition;
    mCurrentPosition = aCurrentPosition;

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_CurrentPosition);
    }
}

void DeviceSwitch::SetMultiPressMax(uint8_t aMultiPressMax)
{
    bool changed;

    changed        = aMultiPressMax != mMultiPressMax;
    mMultiPressMax = aMultiPressMax;

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_MultiPressMax);
    }
}

void DeviceSwitch::SetChangeCallback(DeviceCallback_fn aChanged_CB)
{
    mChanged_CB = aChanged_CB;
}

void DeviceSwitch::HandleDeviceChange(Device * device, Device::Changed_t changeMask)
{
    if (mChanged_CB)
    {
        mChanged_CB(this, (DeviceSwitch::Changed_t) changeMask);
    }
}

void* DeviceTempSensor::Monitor(void *context)
{
    DeviceTempSensor *Dev = (DeviceTempSensor*)context;

    int result;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += Dev->timeout;

    while( 1 ) {
        pthread_mutex_lock(&Dev->device_mutex);
        while(Dev->mReachable == true) {
            result = pthread_cond_timedwait(&Dev->device_cond, &Dev->device_mutex, &ts);
            if(result == ETIMEDOUT) {
                Dev->SetReachable(false);
            } else {
                clock_gettime(CLOCK_REALTIME, &ts);
                ts.tv_sec += Dev->timeout;
            }
        }

        while(Dev->mReachable == false) {
            result = pthread_cond_wait(&Dev->device_cond, &Dev->device_mutex);
            if(result == 0) {
                Dev->SetReachable(true);
                clock_gettime(CLOCK_REALTIME, &ts);
                ts.tv_sec += Dev->timeout;
            }
        }
        pthread_mutex_unlock(&Dev->device_mutex);
    }

}

DeviceTempSensor::DeviceTempSensor(const char * szDeviceName, std::string szLocation, int16_t min, int16_t max,
                                   int16_t measuredValue) :
    Device(szDeviceName, szLocation),
    mMin(min), mMax(max), mMeasurement(measuredValue)
{
    timeout = 10; // second
}

int DeviceTempSensor::StartMonitor()
{
    int res = pthread_create(&Monitor_thread, nullptr, Monitor, (void*)this);
    if (res)
    {
        printf("Error creating TempSensorDevice[%s] Monitor: %d\n", mName, res);
        return -1;
    }

    return 0;
}

int DeviceTempSensor::DestoryMonitor()
{
    pthread_cancel(Monitor_thread);
    if(pthread_join(Monitor_thread, NULL) == 0)
    {
        printf("[%s] Monitor exit !\n", mName);
    }

    return 0;
}

void DeviceTempSensor::SetMeasuredValue(int16_t measurement)
{
    // Limit measurement based on the min and max.
    if (measurement < mMin)
    {
        measurement = mMin;
    }
    else if (measurement > mMax)
    {
        measurement = mMax;
    }

    bool changed = mMeasurement != measurement;

    ChipLogProgress(DeviceLayer, "TempSensorDevice[%s]: New measurement=\"%d\"", mName, measurement);

    mMeasurement = measurement;

    if (changed && mChanged_CB)
    {
        mChanged_CB(this, kChanged_MeasurementValue);
    }
}

void DeviceTempSensor::SetChangeCallback(DeviceCallback_fn aChanged_CB)
{
    mChanged_CB = aChanged_CB;
}

void DeviceTempSensor::HandleDeviceChange(Device * device, Device::Changed_t changeMask)
{
    if (mChanged_CB)
    {
        mChanged_CB(this, (DeviceTempSensor::Changed_t) changeMask);
    }
}

void ComposedDevice::HandleDeviceChange(Device * device, Device::Changed_t changeMask)
{
    if (mChanged_CB)
    {
        mChanged_CB(this, (ComposedDevice::Changed_t) changeMask);
    }
}

void DevicePowerSource::HandleDeviceChange(Device * device, Device::Changed_t changeMask)
{
    if (mChanged_CB)
    {
        mChanged_CB(this, (DevicePowerSource::Changed_t) changeMask);
    }
}

void DevicePowerSource::SetBatChargeLevel(uint8_t aBatChargeLevel)
{
    bool changed;

    changed         = aBatChargeLevel != mBatChargeLevel;
    mBatChargeLevel = aBatChargeLevel;

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_BatLevel);
    }
}

void DevicePowerSource::SetDescription(std::string aDescription)
{
    bool changed;

    changed      = aDescription != mDescription;
    mDescription = aDescription;

    if ((changed) && (mChanged_CB))
    {
        mChanged_CB(this, kChanged_Description);
    }
}

EndpointListInfo::EndpointListInfo(uint16_t endpointListId, std::string name, EndpointListTypeEnum type)
{
    mEndpointListId = endpointListId;
    mName           = name;
    mType           = type;
}

EndpointListInfo::EndpointListInfo(uint16_t endpointListId, std::string name, EndpointListTypeEnum type,
                                   chip::EndpointId endpointId)
{
    mEndpointListId = endpointListId;
    mName           = name;
    mType           = type;
    mEndpoints.push_back(endpointId);
}

void EndpointListInfo::AddEndpointId(chip::EndpointId endpointId)
{
    mEndpoints.push_back(endpointId);
}

Room::Room(std::string name, uint16_t endpointListId, EndpointListTypeEnum type, bool isVisible)
{
    mName           = name;
    mEndpointListId = endpointListId;
    mType           = type;
    mIsVisible      = isVisible;
}

Action::Action() {}

Action::Action(uint16_t actionId, std::string name, ActionTypeEnum type, uint16_t endpointListId, uint16_t supportedCommands,
               ActionStateEnum status, bool isVisible)
{
    mActionId          = actionId;
    mName              = name;
    mType              = type;
    mEndpointListId    = endpointListId;
    mSupportedCommands = supportedCommands;
    mStatus            = status;
    mIsVisible         = isVisible;
}