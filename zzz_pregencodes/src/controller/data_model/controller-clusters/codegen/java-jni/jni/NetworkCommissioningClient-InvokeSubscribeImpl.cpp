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

JNI_METHOD(jlong, NetworkCommissioningCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, NetworkCommissioningCluster, 
  scanNetworks)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject ssid,jobject breadcrumb,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::NetworkCommissioning::Commands::ScanNetworks::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(ssid, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.ssid.Emplace();
      if (optionalValue_0 == nullptr) {
    definedValue_0.SetNull();
  } else {
    auto & nonNullValue_1 = definedValue_0.SetNonNull();
    cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(optionalValue_0)));
        nonNullValue_1 = cleanupByteArrays.back()->byteSpan();
  }
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(breadcrumb, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.breadcrumb.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0));
    
    }
  }

  
    std::unique_ptr<CHIPNetworkCommissioningClusterScanNetworksResponseCallback, void (*)(CHIPNetworkCommissioningClusterScanNetworksResponseCallback *)> onSuccess(
        Platform::New<CHIPNetworkCommissioningClusterScanNetworksResponseCallback>(callback), Platform::Delete<CHIPNetworkCommissioningClusterScanNetworksResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterScanNetworksResponseCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

  if (timedInvokeTimeoutMs == nullptr) {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall);
    } else {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall, chip::JniReferences::GetInstance().IntegerToPrimitive(timedInvokeTimeoutMs));
    }
    VerifyOrReturn(err == CHIP_NO_ERROR, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error invoking command", CHIP_ERROR_INCORRECT_STATE));

    onSuccess.release();
    onFailure.release();
}
JNI_METHOD(void, NetworkCommissioningCluster, 
  addOrUpdateWiFiNetwork)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jbyteArray ssid,jbyteArray credentials,jobject breadcrumb,jobject networkIdentity,jobject clientIdentifier,jobject possessionNonce,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::NetworkCommissioning::Commands::AddOrUpdateWiFiNetwork::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(ssid)));
        request.ssid = cleanupByteArrays.back()->byteSpan();cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(credentials)));
        request.credentials = cleanupByteArrays.back()->byteSpan();{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(breadcrumb, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.breadcrumb.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0));
    
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(networkIdentity, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.networkIdentity.Emplace();
      cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(optionalValue_0)));
        definedValue_0 = cleanupByteArrays.back()->byteSpan();
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(clientIdentifier, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.clientIdentifier.Emplace();
      cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(optionalValue_0)));
        definedValue_0 = cleanupByteArrays.back()->byteSpan();
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(possessionNonce, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.possessionNonce.Emplace();
      cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(optionalValue_0)));
        definedValue_0 = cleanupByteArrays.back()->byteSpan();
    }
  }

  
    std::unique_ptr<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback, void (*)(CHIPNetworkCommissioningClusterNetworkConfigResponseCallback *)> onSuccess(
        Platform::New<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback>(callback), Platform::Delete<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterNetworkConfigResponseCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

  if (timedInvokeTimeoutMs == nullptr) {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall);
    } else {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall, chip::JniReferences::GetInstance().IntegerToPrimitive(timedInvokeTimeoutMs));
    }
    VerifyOrReturn(err == CHIP_NO_ERROR, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error invoking command", CHIP_ERROR_INCORRECT_STATE));

    onSuccess.release();
    onFailure.release();
}
JNI_METHOD(void, NetworkCommissioningCluster, 
  addOrUpdateThreadNetwork)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jbyteArray operationalDataset,jobject breadcrumb,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::NetworkCommissioning::Commands::AddOrUpdateThreadNetwork::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(operationalDataset)));
        request.operationalDataset = cleanupByteArrays.back()->byteSpan();{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(breadcrumb, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.breadcrumb.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0));
    
    }
  }

  
    std::unique_ptr<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback, void (*)(CHIPNetworkCommissioningClusterNetworkConfigResponseCallback *)> onSuccess(
        Platform::New<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback>(callback), Platform::Delete<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterNetworkConfigResponseCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

  if (timedInvokeTimeoutMs == nullptr) {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall);
    } else {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall, chip::JniReferences::GetInstance().IntegerToPrimitive(timedInvokeTimeoutMs));
    }
    VerifyOrReturn(err == CHIP_NO_ERROR, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error invoking command", CHIP_ERROR_INCORRECT_STATE));

    onSuccess.release();
    onFailure.release();
}
JNI_METHOD(void, NetworkCommissioningCluster, 
  removeNetwork)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jbyteArray networkID,jobject breadcrumb,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::NetworkCommissioning::Commands::RemoveNetwork::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(networkID)));
        request.networkID = cleanupByteArrays.back()->byteSpan();{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(breadcrumb, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.breadcrumb.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0));
    
    }
  }

  
    std::unique_ptr<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback, void (*)(CHIPNetworkCommissioningClusterNetworkConfigResponseCallback *)> onSuccess(
        Platform::New<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback>(callback), Platform::Delete<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterNetworkConfigResponseCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

  if (timedInvokeTimeoutMs == nullptr) {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall);
    } else {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall, chip::JniReferences::GetInstance().IntegerToPrimitive(timedInvokeTimeoutMs));
    }
    VerifyOrReturn(err == CHIP_NO_ERROR, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error invoking command", CHIP_ERROR_INCORRECT_STATE));

    onSuccess.release();
    onFailure.release();
}
JNI_METHOD(void, NetworkCommissioningCluster, 
  connectNetwork)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jbyteArray networkID,jobject breadcrumb,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::NetworkCommissioning::Commands::ConnectNetwork::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(networkID)));
        request.networkID = cleanupByteArrays.back()->byteSpan();{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(breadcrumb, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.breadcrumb.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0));
    
    }
  }

  
    std::unique_ptr<CHIPNetworkCommissioningClusterConnectNetworkResponseCallback, void (*)(CHIPNetworkCommissioningClusterConnectNetworkResponseCallback *)> onSuccess(
        Platform::New<CHIPNetworkCommissioningClusterConnectNetworkResponseCallback>(callback), Platform::Delete<CHIPNetworkCommissioningClusterConnectNetworkResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterConnectNetworkResponseCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

  if (timedInvokeTimeoutMs == nullptr) {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall);
    } else {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall, chip::JniReferences::GetInstance().IntegerToPrimitive(timedInvokeTimeoutMs));
    }
    VerifyOrReturn(err == CHIP_NO_ERROR, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error invoking command", CHIP_ERROR_INCORRECT_STATE));

    onSuccess.release();
    onFailure.release();
}
JNI_METHOD(void, NetworkCommissioningCluster, 
  reorderNetwork)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jbyteArray networkID,jobject networkIndex,jobject breadcrumb,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::NetworkCommissioning::Commands::ReorderNetwork::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(networkID)));
        request.networkID = cleanupByteArrays.back()->byteSpan();request.networkIndex = static_cast<std::remove_reference_t<decltype(request.networkIndex)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(networkIndex));
    {
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(breadcrumb, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.breadcrumb.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0));
    
    }
  }

  
    std::unique_ptr<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback, void (*)(CHIPNetworkCommissioningClusterNetworkConfigResponseCallback *)> onSuccess(
        Platform::New<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback>(callback), Platform::Delete<CHIPNetworkCommissioningClusterNetworkConfigResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterNetworkConfigResponseCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

  if (timedInvokeTimeoutMs == nullptr) {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall);
    } else {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall, chip::JniReferences::GetInstance().IntegerToPrimitive(timedInvokeTimeoutMs));
    }
    VerifyOrReturn(err == CHIP_NO_ERROR, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error invoking command", CHIP_ERROR_INCORRECT_STATE));

    onSuccess.release();
    onFailure.release();
}
JNI_METHOD(void, NetworkCommissioningCluster, 
  queryIdentity)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jbyteArray keyIdentifier,jobject possessionNonce,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::NetworkCommissioning::Commands::QueryIdentity::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(keyIdentifier)));
        request.keyIdentifier = cleanupByteArrays.back()->byteSpan();{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(possessionNonce, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.possessionNonce.Emplace();
      cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(optionalValue_0)));
        definedValue_0 = cleanupByteArrays.back()->byteSpan();
    }
  }

  
    std::unique_ptr<CHIPNetworkCommissioningClusterQueryIdentityResponseCallback, void (*)(CHIPNetworkCommissioningClusterQueryIdentityResponseCallback *)> onSuccess(
        Platform::New<CHIPNetworkCommissioningClusterQueryIdentityResponseCallback>(callback), Platform::Delete<CHIPNetworkCommissioningClusterQueryIdentityResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterQueryIdentityResponseCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

  if (timedInvokeTimeoutMs == nullptr) {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall);
    } else {
        err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall, chip::JniReferences::GetInstance().IntegerToPrimitive(timedInvokeTimeoutMs));
    }
    VerifyOrReturn(err == CHIP_NO_ERROR, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error invoking command", CHIP_ERROR_INCORRECT_STATE));

    onSuccess.release();
    onFailure.release();
}
JNI_METHOD(void, NetworkCommissioningCluster, subscribeMaxNetworksAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::MaxNetworks::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterMaxNetworksAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeNetworksAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningNetworksAttributeCallback, void (*)(CHIPNetworkCommissioningNetworksAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningNetworksAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningNetworksAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::Networks::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterNetworksAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningNetworksAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeScanMaxTimeSecondsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::ScanMaxTimeSeconds::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterScanMaxTimeSecondsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeConnectMaxTimeSecondsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::ConnectMaxTimeSeconds::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterConnectMaxTimeSecondsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeInterfaceEnabledAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPBooleanAttributeCallback, void (*)(CHIPBooleanAttributeCallback *)> onSuccess(Platform::New<CHIPBooleanAttributeCallback>(callback, true), chip::Platform::Delete<CHIPBooleanAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::InterfaceEnabled::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterInterfaceEnabledAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPBooleanAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeLastNetworkingStatusAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningLastNetworkingStatusAttributeCallback, void (*)(CHIPNetworkCommissioningLastNetworkingStatusAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningLastNetworkingStatusAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningLastNetworkingStatusAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::LastNetworkingStatus::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterLastNetworkingStatusAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningLastNetworkingStatusAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeLastNetworkIDAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningLastNetworkIDAttributeCallback, void (*)(CHIPNetworkCommissioningLastNetworkIDAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningLastNetworkIDAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningLastNetworkIDAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::LastNetworkID::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterLastNetworkIDAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningLastNetworkIDAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeLastConnectErrorValueAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningLastConnectErrorValueAttributeCallback, void (*)(CHIPNetworkCommissioningLastConnectErrorValueAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningLastConnectErrorValueAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningLastConnectErrorValueAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::LastConnectErrorValue::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterLastConnectErrorValueAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningLastConnectErrorValueAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeSupportedWiFiBandsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningSupportedWiFiBandsAttributeCallback, void (*)(CHIPNetworkCommissioningSupportedWiFiBandsAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningSupportedWiFiBandsAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningSupportedWiFiBandsAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::SupportedWiFiBands::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterSupportedWiFiBandsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningSupportedWiFiBandsAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeSupportedThreadFeaturesAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::SupportedThreadFeatures::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterSupportedThreadFeaturesAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeThreadVersionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::ThreadVersion::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterThreadVersionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningGeneratedCommandListAttributeCallback, void (*)(CHIPNetworkCommissioningGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningAcceptedCommandListAttributeCallback, void (*)(CHIPNetworkCommissioningAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningEventListAttributeCallback, void (*)(CHIPNetworkCommissioningEventListAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPNetworkCommissioningAttributeListAttributeCallback, void (*)(CHIPNetworkCommissioningAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPNetworkCommissioningAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPNetworkCommissioningAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPNetworkCommissioningAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, NetworkCommissioningCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::NetworkCommissioning::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPNetworkCommissioningClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
