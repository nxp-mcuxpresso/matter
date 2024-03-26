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

JNI_METHOD(jlong, FixedLabelCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, FixedLabelCluster, subscribeLabelListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPFixedLabelLabelListAttributeCallback, void (*)(CHIPFixedLabelLabelListAttributeCallback *)> onSuccess(Platform::New<CHIPFixedLabelLabelListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPFixedLabelLabelListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::FixedLabel::Attributes::LabelList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPFixedLabelClusterLabelListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPFixedLabelLabelListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, FixedLabelCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPFixedLabelGeneratedCommandListAttributeCallback, void (*)(CHIPFixedLabelGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPFixedLabelGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPFixedLabelGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::FixedLabel::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPFixedLabelClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPFixedLabelGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, FixedLabelCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPFixedLabelAcceptedCommandListAttributeCallback, void (*)(CHIPFixedLabelAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPFixedLabelAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPFixedLabelAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::FixedLabel::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPFixedLabelClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPFixedLabelAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, FixedLabelCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPFixedLabelEventListAttributeCallback, void (*)(CHIPFixedLabelEventListAttributeCallback *)> onSuccess(Platform::New<CHIPFixedLabelEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPFixedLabelEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::FixedLabel::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPFixedLabelClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPFixedLabelEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, FixedLabelCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPFixedLabelAttributeListAttributeCallback, void (*)(CHIPFixedLabelAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPFixedLabelAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPFixedLabelAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::FixedLabel::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPFixedLabelClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPFixedLabelAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, FixedLabelCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::FixedLabel::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPFixedLabelClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, FixedLabelCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::FixedLabel::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPFixedLabelClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
