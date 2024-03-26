
#include <jni/CHIPReadCallbacks.h>

#include <app-common/zap-generated/cluster-objects.h>
#include <controller/CHIPCluster.h>

#include <controller/java/AndroidClusterExceptions.h>
#include <controller/java/CHIPDefaultCallbacks.h>
#include <jni.h>
#include <lib/support/CodeUtils.h>
#include <platform/PlatformManager.h>

#define JNI_METHOD(RETURN, CLASS_NAME, METHOD_NAME)                                                                                \
    extern "C" JNIEXPORT RETURN JNICALL Java_chip_devicecontroller_ChipClusters_00024##CLASS_NAME##_##METHOD_NAME
JNI_METHOD(void, EnergyPreferenceCluster, readEnergyBalancesAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::EnergyBalances::TypeInfo;
    std::unique_ptr<CHIPEnergyPreferenceEnergyBalancesAttributeCallback, void (*)(CHIPEnergyPreferenceEnergyBalancesAttributeCallback *)> onSuccess(chip::Platform::New<CHIPEnergyPreferenceEnergyBalancesAttributeCallback>(callback, false), chip::Platform::Delete<CHIPEnergyPreferenceEnergyBalancesAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterEnergyBalancesAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readCurrentEnergyBalanceAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::CurrentEnergyBalance::TypeInfo;
    std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterCurrentEnergyBalanceAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readEnergyPrioritiesAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::EnergyPriorities::TypeInfo;
    std::unique_ptr<CHIPEnergyPreferenceEnergyPrioritiesAttributeCallback, void (*)(CHIPEnergyPreferenceEnergyPrioritiesAttributeCallback *)> onSuccess(chip::Platform::New<CHIPEnergyPreferenceEnergyPrioritiesAttributeCallback>(callback, false), chip::Platform::Delete<CHIPEnergyPreferenceEnergyPrioritiesAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterEnergyPrioritiesAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readLowPowerModeSensitivitiesAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::LowPowerModeSensitivities::TypeInfo;
    std::unique_ptr<CHIPEnergyPreferenceLowPowerModeSensitivitiesAttributeCallback, void (*)(CHIPEnergyPreferenceLowPowerModeSensitivitiesAttributeCallback *)> onSuccess(chip::Platform::New<CHIPEnergyPreferenceLowPowerModeSensitivitiesAttributeCallback>(callback, false), chip::Platform::Delete<CHIPEnergyPreferenceLowPowerModeSensitivitiesAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterLowPowerModeSensitivitiesAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readCurrentLowPowerModeSensitivityAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::CurrentLowPowerModeSensitivity::TypeInfo;
    std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterCurrentLowPowerModeSensitivityAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::GeneratedCommandList::TypeInfo;
    std::unique_ptr<CHIPEnergyPreferenceGeneratedCommandListAttributeCallback, void (*)(CHIPEnergyPreferenceGeneratedCommandListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPEnergyPreferenceGeneratedCommandListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPEnergyPreferenceGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::AcceptedCommandList::TypeInfo;
    std::unique_ptr<CHIPEnergyPreferenceAcceptedCommandListAttributeCallback, void (*)(CHIPEnergyPreferenceAcceptedCommandListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPEnergyPreferenceAcceptedCommandListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPEnergyPreferenceAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::EventList::TypeInfo;
    std::unique_ptr<CHIPEnergyPreferenceEventListAttributeCallback, void (*)(CHIPEnergyPreferenceEventListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPEnergyPreferenceEventListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPEnergyPreferenceEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::AttributeList::TypeInfo;
    std::unique_ptr<CHIPEnergyPreferenceAttributeListAttributeCallback, void (*)(CHIPEnergyPreferenceAttributeListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPEnergyPreferenceAttributeListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPEnergyPreferenceAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::FeatureMap::TypeInfo;
    std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, EnergyPreferenceCluster, readClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::EnergyPreference::Attributes::ClusterRevision::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPEnergyPreferenceClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


