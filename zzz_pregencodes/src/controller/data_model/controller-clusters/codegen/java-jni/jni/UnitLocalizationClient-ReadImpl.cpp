
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
JNI_METHOD(void, UnitLocalizationCluster, readTemperatureUnitAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::UnitLocalization::Attributes::TemperatureUnit::TypeInfo;
    std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitLocalizationClusterTemperatureUnitAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, UnitLocalizationCluster, readGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::UnitLocalization::Attributes::GeneratedCommandList::TypeInfo;
    std::unique_ptr<CHIPUnitLocalizationGeneratedCommandListAttributeCallback, void (*)(CHIPUnitLocalizationGeneratedCommandListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPUnitLocalizationGeneratedCommandListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPUnitLocalizationGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitLocalizationClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, UnitLocalizationCluster, readAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::UnitLocalization::Attributes::AcceptedCommandList::TypeInfo;
    std::unique_ptr<CHIPUnitLocalizationAcceptedCommandListAttributeCallback, void (*)(CHIPUnitLocalizationAcceptedCommandListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPUnitLocalizationAcceptedCommandListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPUnitLocalizationAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitLocalizationClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, UnitLocalizationCluster, readEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::UnitLocalization::Attributes::EventList::TypeInfo;
    std::unique_ptr<CHIPUnitLocalizationEventListAttributeCallback, void (*)(CHIPUnitLocalizationEventListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPUnitLocalizationEventListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPUnitLocalizationEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitLocalizationClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, UnitLocalizationCluster, readAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::UnitLocalization::Attributes::AttributeList::TypeInfo;
    std::unique_ptr<CHIPUnitLocalizationAttributeListAttributeCallback, void (*)(CHIPUnitLocalizationAttributeListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPUnitLocalizationAttributeListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPUnitLocalizationAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitLocalizationClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, UnitLocalizationCluster, readFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::UnitLocalization::Attributes::FeatureMap::TypeInfo;
    std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitLocalizationClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, UnitLocalizationCluster, readClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::UnitLocalization::Attributes::ClusterRevision::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitLocalizationClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


