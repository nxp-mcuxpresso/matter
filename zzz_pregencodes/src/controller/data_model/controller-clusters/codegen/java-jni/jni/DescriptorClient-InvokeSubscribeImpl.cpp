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

JNI_METHOD(jlong, DescriptorCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, DescriptorCluster, subscribeDeviceTypeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorDeviceTypeListAttributeCallback, void (*)(CHIPDescriptorDeviceTypeListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorDeviceTypeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorDeviceTypeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::DeviceTypeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterDeviceTypeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorDeviceTypeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeServerListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorServerListAttributeCallback, void (*)(CHIPDescriptorServerListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorServerListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorServerListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::ServerList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterServerListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorServerListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeClientListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorClientListAttributeCallback, void (*)(CHIPDescriptorClientListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorClientListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorClientListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::ClientList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterClientListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorClientListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribePartsListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorPartsListAttributeCallback, void (*)(CHIPDescriptorPartsListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorPartsListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorPartsListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::PartsList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterPartsListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorPartsListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeTagListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorTagListAttributeCallback, void (*)(CHIPDescriptorTagListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorTagListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorTagListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::TagList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterTagListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorTagListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorGeneratedCommandListAttributeCallback, void (*)(CHIPDescriptorGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorAcceptedCommandListAttributeCallback, void (*)(CHIPDescriptorAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorEventListAttributeCallback, void (*)(CHIPDescriptorEventListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDescriptorAttributeListAttributeCallback, void (*)(CHIPDescriptorAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPDescriptorAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDescriptorAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDescriptorAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DescriptorCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Descriptor::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDescriptorClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
