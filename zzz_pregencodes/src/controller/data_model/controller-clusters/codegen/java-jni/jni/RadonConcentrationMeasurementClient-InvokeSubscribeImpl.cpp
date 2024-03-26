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

JNI_METHOD(jlong, RadonConcentrationMeasurementCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeMeasuredValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementMeasuredValueAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementMeasuredValueAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementMeasuredValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementMeasuredValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::MeasuredValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterMeasuredValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementMeasuredValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeMinMeasuredValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementMinMeasuredValueAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementMinMeasuredValueAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementMinMeasuredValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementMinMeasuredValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::MinMeasuredValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterMinMeasuredValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementMinMeasuredValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeMaxMeasuredValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementMaxMeasuredValueAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementMaxMeasuredValueAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementMaxMeasuredValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementMaxMeasuredValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::MaxMeasuredValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterMaxMeasuredValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementMaxMeasuredValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribePeakMeasuredValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementPeakMeasuredValueAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementPeakMeasuredValueAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementPeakMeasuredValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementPeakMeasuredValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::PeakMeasuredValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterPeakMeasuredValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementPeakMeasuredValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribePeakMeasuredValueWindowAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::PeakMeasuredValueWindow::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterPeakMeasuredValueWindowAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeAverageMeasuredValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementAverageMeasuredValueAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementAverageMeasuredValueAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementAverageMeasuredValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementAverageMeasuredValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::AverageMeasuredValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterAverageMeasuredValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementAverageMeasuredValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeAverageMeasuredValueWindowAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::AverageMeasuredValueWindow::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterAverageMeasuredValueWindowAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeUncertaintyAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPFloatAttributeCallback, void (*)(CHIPFloatAttributeCallback *)> onSuccess(Platform::New<CHIPFloatAttributeCallback>(callback, true), chip::Platform::Delete<CHIPFloatAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::Uncertainty::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterUncertaintyAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPFloatAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeMeasurementUnitAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::MeasurementUnit::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterMeasurementUnitAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeMeasurementMediumAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::MeasurementMedium::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterMeasurementMediumAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeLevelValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::LevelValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterLevelValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementGeneratedCommandListAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementAcceptedCommandListAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementEventListAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementEventListAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPRadonConcentrationMeasurementAttributeListAttributeCallback, void (*)(CHIPRadonConcentrationMeasurementAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPRadonConcentrationMeasurementAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPRadonConcentrationMeasurementAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPRadonConcentrationMeasurementAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, RadonConcentrationMeasurementCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::RadonConcentrationMeasurement::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPRadonConcentrationMeasurementClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
