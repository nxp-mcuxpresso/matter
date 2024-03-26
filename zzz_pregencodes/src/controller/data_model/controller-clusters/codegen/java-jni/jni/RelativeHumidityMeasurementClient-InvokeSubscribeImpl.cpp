#include <jni/CHIPCallbackTypes.h>
#include <jni/CHIPReadCallbacks.h>
#include <controller/CHIPCluster.h>
#include <controller/java/zap-generated/CHIPInvokeCallbacks.h>

#include <app-common/zap-generated/cluster-objects.h>

#include <controller/java/zap-generated/CHIPClientCallbacks.h>
#include <controller/java/AndroidCallbacks.h>
#include <controller/java/AndroidClusterExceptions.h>
#include <controller/java/CHIPDefaultCallbacks.h>
#include <jni.h>
#include <lib/support/CHIPListUtils.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/JniReferences.h>
#include <lib/support/JniTypeWrappers.h>
#include <lib/support/Span.h>
#include <platform/PlatformManager.h>
#include <vector>

#include <jni/CHIPCallbackTypes.h>

#define JNI_METHOD(RETURN, CLASS_NAME, METHOD_NAME)                                                                                \
    extern "C" JNIEXPORT RETURN JNICALL Java_chip_devicecontroller_ChipClusters_00024##CLASS_NAME##_##METHOD_NAME

using namespace chip;
using chip::Controller::ClusterBase;

JNI_METHOD(jlong, RelativeHumidityMeasurementCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeMeasuredValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRelativeHumidityMeasurementMeasuredValueAttributeCallback, void (*)(CHIPRelativeHumidityMeasurementMeasuredValueAttributeCallback *)> onSuccess(Platform::New<CHIPRelativeHumidityMeasurementMeasuredValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRelativeHumidityMeasurementMeasuredValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::MeasuredValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterMeasuredValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRelativeHumidityMeasurementMeasuredValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeMinMeasuredValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRelativeHumidityMeasurementMinMeasuredValueAttributeCallback, void (*)(CHIPRelativeHumidityMeasurementMinMeasuredValueAttributeCallback *)> onSuccess(Platform::New<CHIPRelativeHumidityMeasurementMinMeasuredValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRelativeHumidityMeasurementMinMeasuredValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::MinMeasuredValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterMinMeasuredValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRelativeHumidityMeasurementMinMeasuredValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeMaxMeasuredValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRelativeHumidityMeasurementMaxMeasuredValueAttributeCallback, void (*)(CHIPRelativeHumidityMeasurementMaxMeasuredValueAttributeCallback *)> onSuccess(Platform::New<CHIPRelativeHumidityMeasurementMaxMeasuredValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRelativeHumidityMeasurementMaxMeasuredValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::MaxMeasuredValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterMaxMeasuredValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRelativeHumidityMeasurementMaxMeasuredValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeToleranceAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::Tolerance::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterToleranceAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRelativeHumidityMeasurementGeneratedCommandListAttributeCallback, void (*)(CHIPRelativeHumidityMeasurementGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPRelativeHumidityMeasurementGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRelativeHumidityMeasurementGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRelativeHumidityMeasurementGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRelativeHumidityMeasurementAcceptedCommandListAttributeCallback, void (*)(CHIPRelativeHumidityMeasurementAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPRelativeHumidityMeasurementAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRelativeHumidityMeasurementAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRelativeHumidityMeasurementAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRelativeHumidityMeasurementEventListAttributeCallback, void (*)(CHIPRelativeHumidityMeasurementEventListAttributeCallback *)> onSuccess(Platform::New<CHIPRelativeHumidityMeasurementEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRelativeHumidityMeasurementEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRelativeHumidityMeasurementEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRelativeHumidityMeasurementAttributeListAttributeCallback, void (*)(CHIPRelativeHumidityMeasurementAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPRelativeHumidityMeasurementAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRelativeHumidityMeasurementAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRelativeHumidityMeasurementAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RelativeHumidityMeasurementCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RelativeHumidityMeasurement::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRelativeHumidityMeasurementClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
