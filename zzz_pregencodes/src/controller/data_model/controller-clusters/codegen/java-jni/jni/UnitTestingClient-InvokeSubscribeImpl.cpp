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

JNI_METHOD(jlong, UnitTestingCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, UnitTestingCluster, 
  test)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::Test::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;

  
    std::unique_ptr<CHIPDefaultSuccessCallback, void (*)(CHIPDefaultSuccessCallback *)> onSuccess(
        Platform::New<CHIPDefaultSuccessCallback>(callback), Platform::Delete<CHIPDefaultSuccessCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPDefaultSuccessCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testNotHandled)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestNotHandled::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;

  
    std::unique_ptr<CHIPDefaultSuccessCallback, void (*)(CHIPDefaultSuccessCallback *)> onSuccess(
        Platform::New<CHIPDefaultSuccessCallback>(callback), Platform::Delete<CHIPDefaultSuccessCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPDefaultSuccessCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testSpecific)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestSpecific::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;

  
    std::unique_ptr<CHIPUnitTestingClusterTestSpecificResponseCallback, void (*)(CHIPUnitTestingClusterTestSpecificResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestSpecificResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestSpecificResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestSpecificResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testUnknownCommand)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestUnknownCommand::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;

  
    std::unique_ptr<CHIPDefaultSuccessCallback, void (*)(CHIPDefaultSuccessCallback *)> onSuccess(
        Platform::New<CHIPDefaultSuccessCallback>(callback), Platform::Delete<CHIPDefaultSuccessCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPDefaultSuccessCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testAddArguments)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject arg2,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestAddArguments::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.arg1 = static_cast<std::remove_reference_t<decltype(request.arg1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1));
    request.arg2 = static_cast<std::remove_reference_t<decltype(request.arg2)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg2));
    

  
    std::unique_ptr<CHIPUnitTestingClusterTestAddArgumentsResponseCallback, void (*)(CHIPUnitTestingClusterTestAddArgumentsResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestAddArgumentsResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestAddArgumentsResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestAddArgumentsResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testSimpleArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestSimpleArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.arg1 = static_cast<std::remove_reference_t<decltype(request.arg1)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg1));
    

  
    std::unique_ptr<CHIPUnitTestingClusterTestSimpleArgumentResponseCallback, void (*)(CHIPUnitTestingClusterTestSimpleArgumentResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestSimpleArgumentResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestSimpleArgumentResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestSimpleArgumentResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testStructArrayArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject arg2,jobject arg3,jobject arg4,jobject arg5,jobject arg6,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestStructArrayArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    using ListType_0 = std::remove_reference_t<decltype(request.arg1)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint arg1Size;
    chip::JniReferences::GetInstance().GetListSize(arg1, arg1Size);
    if (arg1Size != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(arg1Size);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < arg1Size; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(arg1, i_0, element_0);
        
          jobject element_0_aItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "a", "Ljava/lang/Integer;", element_0_aItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].a = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_aItem_1));
    
          jobject element_0_bItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "b", "Ljava/lang/Boolean;", element_0_bItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].b = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_0_bItem_1));
    
          jobject element_0_cItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "c", "Lchip/devicecontroller/ChipStructs$UnitTestingClusterSimpleStruct;", element_0_cItem_1);
          
          jobject element_0_cItem_1_aItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "a", "Ljava/lang/Integer;", element_0_cItem_1_aItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.a = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_cItem_1_aItem_2));
    
          jobject element_0_cItem_1_bItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "b", "Ljava/lang/Boolean;", element_0_cItem_1_bItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.b = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_0_cItem_1_bItem_2));
    
          jobject element_0_cItem_1_cItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "c", "Ljava/lang/Integer;", element_0_cItem_1_cItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.c = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_cItem_1_cItem_2));
          jobject element_0_cItem_1_dItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "d", "[B", element_0_cItem_1_dItem_2);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_0_cItem_1_dItem_2)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].c.d = cleanupByteArrays.back()->byteSpan();
          jobject element_0_cItem_1_eItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "e", "Ljava/lang/String;", element_0_cItem_1_eItem_2);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_0_cItem_1_eItem_2)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].c.e = cleanupStrings.back()->charSpan();
          jobject element_0_cItem_1_fItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "f", "Ljava/lang/Integer;", element_0_cItem_1_fItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.f.SetRaw(static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_cItem_1_fItem_2)));
    
          jobject element_0_cItem_1_gItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "g", "Ljava/lang/Float;", element_0_cItem_1_gItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.g = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(element_0_cItem_1_gItem_2));
    
          jobject element_0_cItem_1_hItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "h", "Ljava/lang/Double;", element_0_cItem_1_hItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.h = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(element_0_cItem_1_hItem_2));
    
          jobject element_0_dItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "d", "Ljava/util/ArrayList;", element_0_dItem_1);
          {
    using ListType_2 = std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].d)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint element_0_dItem_1Size;
    chip::JniReferences::GetInstance().GetListSize(element_0_dItem_1, element_0_dItem_1Size);
    if (element_0_dItem_1Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(element_0_dItem_1Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < element_0_dItem_1Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(element_0_dItem_1, i_2, element_2);
        
          jobject element_2_aItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "a", "Ljava/lang/Integer;", element_2_aItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].a = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2_aItem_3));
    
          jobject element_2_bItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "b", "Ljava/lang/Boolean;", element_2_bItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].b = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_2_bItem_3));
    
          jobject element_2_cItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "c", "Ljava/lang/Integer;", element_2_cItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].c = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2_cItem_3));
          jobject element_2_dItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "d", "[B", element_2_dItem_3);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_2_dItem_3)));
        listHolder_2->mList[static_cast<uint32_t>(i_2)].d = cleanupByteArrays.back()->byteSpan();
          jobject element_2_eItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "e", "Ljava/lang/String;", element_2_eItem_3);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_2_eItem_3)));
        listHolder_2->mList[static_cast<uint32_t>(i_2)].e = cleanupStrings.back()->charSpan();
          jobject element_2_fItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "f", "Ljava/lang/Integer;", element_2_fItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].f.SetRaw(static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2_fItem_3)));
    
          jobject element_2_gItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "g", "Ljava/lang/Float;", element_2_gItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].g = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(element_2_gItem_3));
    
          jobject element_2_hItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "h", "Ljava/lang/Double;", element_2_hItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].h = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(element_2_hItem_3));
    
      }
      listHolder_0->mList[static_cast<uint32_t>(i_0)].d = ListType_2(listHolder_2->mList, element_0_dItem_1Size);
    } else {
      listHolder_0->mList[static_cast<uint32_t>(i_0)].d = ListType_2();
    }
  }
          jobject element_0_eItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "e", "Ljava/util/ArrayList;", element_0_eItem_1);
          {
    using ListType_2 = std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].e)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint element_0_eItem_1Size;
    chip::JniReferences::GetInstance().GetListSize(element_0_eItem_1, element_0_eItem_1Size);
    if (element_0_eItem_1Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(element_0_eItem_1Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < element_0_eItem_1Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(element_0_eItem_1, i_2, element_2);
        listHolder_2->mList[static_cast<uint32_t>(i_2)] = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)])>>(chip::JniReferences::GetInstance().LongToPrimitive(element_2));
    
      }
      listHolder_0->mList[static_cast<uint32_t>(i_0)].e = ListType_2(listHolder_2->mList, element_0_eItem_1Size);
    } else {
      listHolder_0->mList[static_cast<uint32_t>(i_0)].e = ListType_2();
    }
  }
          jobject element_0_fItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "f", "Ljava/util/ArrayList;", element_0_fItem_1);
          {
    using ListType_2 = std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].f)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint element_0_fItem_1Size;
    chip::JniReferences::GetInstance().GetListSize(element_0_fItem_1, element_0_fItem_1Size);
    if (element_0_fItem_1Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(element_0_fItem_1Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < element_0_fItem_1Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(element_0_fItem_1, i_2, element_2);
        cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_2)));
        listHolder_2->mList[static_cast<uint32_t>(i_2)] = cleanupByteArrays.back()->byteSpan();
      }
      listHolder_0->mList[static_cast<uint32_t>(i_0)].f = ListType_2(listHolder_2->mList, element_0_fItem_1Size);
    } else {
      listHolder_0->mList[static_cast<uint32_t>(i_0)].f = ListType_2();
    }
  }
          jobject element_0_gItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "g", "Ljava/util/ArrayList;", element_0_gItem_1);
          {
    using ListType_2 = std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].g)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint element_0_gItem_1Size;
    chip::JniReferences::GetInstance().GetListSize(element_0_gItem_1, element_0_gItem_1Size);
    if (element_0_gItem_1Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(element_0_gItem_1Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < element_0_gItem_1Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(element_0_gItem_1, i_2, element_2);
        listHolder_2->mList[static_cast<uint32_t>(i_2)] = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2));
    
      }
      listHolder_0->mList[static_cast<uint32_t>(i_0)].g = ListType_2(listHolder_2->mList, element_0_gItem_1Size);
    } else {
      listHolder_0->mList[static_cast<uint32_t>(i_0)].g = ListType_2();
    }
  }
      }
      request.arg1 = ListType_0(listHolder_0->mList, arg1Size);
    } else {
      request.arg1 = ListType_0();
    }
  }{
    using ListType_0 = std::remove_reference_t<decltype(request.arg2)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint arg2Size;
    chip::JniReferences::GetInstance().GetListSize(arg2, arg2Size);
    if (arg2Size != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(arg2Size);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < arg2Size; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(arg2, i_0, element_0);
        
          jobject element_0_aItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "a", "Ljava/lang/Integer;", element_0_aItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].a = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_aItem_1));
    
          jobject element_0_bItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "b", "Ljava/lang/Boolean;", element_0_bItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].b = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_0_bItem_1));
    
          jobject element_0_cItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "c", "Ljava/lang/Integer;", element_0_cItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_cItem_1));
          jobject element_0_dItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "d", "[B", element_0_dItem_1);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_0_dItem_1)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].d = cleanupByteArrays.back()->byteSpan();
          jobject element_0_eItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "e", "Ljava/lang/String;", element_0_eItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_0_eItem_1)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].e = cleanupStrings.back()->charSpan();
          jobject element_0_fItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "f", "Ljava/lang/Integer;", element_0_fItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].f.SetRaw(static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_fItem_1)));
    
          jobject element_0_gItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "g", "Ljava/lang/Float;", element_0_gItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].g = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(element_0_gItem_1));
    
          jobject element_0_hItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "h", "Ljava/lang/Double;", element_0_hItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].h = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(element_0_hItem_1));
    
      }
      request.arg2 = ListType_0(listHolder_0->mList, arg2Size);
    } else {
      request.arg2 = ListType_0();
    }
  }{
    using ListType_0 = std::remove_reference_t<decltype(request.arg3)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint arg3Size;
    chip::JniReferences::GetInstance().GetListSize(arg3, arg3Size);
    if (arg3Size != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(arg3Size);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < arg3Size; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(arg3, i_0, element_0);
        listHolder_0->mList[static_cast<uint32_t>(i_0)] = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0));
      }
      request.arg3 = ListType_0(listHolder_0->mList, arg3Size);
    } else {
      request.arg3 = ListType_0();
    }
  }{
    using ListType_0 = std::remove_reference_t<decltype(request.arg4)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint arg4Size;
    chip::JniReferences::GetInstance().GetListSize(arg4, arg4Size);
    if (arg4Size != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(arg4Size);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < arg4Size; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(arg4, i_0, element_0);
        listHolder_0->mList[static_cast<uint32_t>(i_0)] = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)])>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_0));
    
      }
      request.arg4 = ListType_0(listHolder_0->mList, arg4Size);
    } else {
      request.arg4 = ListType_0();
    }
  }request.arg5 = static_cast<std::remove_reference_t<decltype(request.arg5)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg5));request.arg6 = static_cast<std::remove_reference_t<decltype(request.arg6)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg6));
    

  
    std::unique_ptr<CHIPUnitTestingClusterTestStructArrayArgumentResponseCallback, void (*)(CHIPUnitTestingClusterTestStructArrayArgumentResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestStructArrayArgumentResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestStructArrayArgumentResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestStructArrayArgumentResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testStructArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestStructArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;
          jobject arg1_aItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "a", "Ljava/lang/Integer;", arg1_aItem_0);
          request.arg1.a = static_cast<std::remove_reference_t<decltype(request.arg1.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_aItem_0));
    
          jobject arg1_bItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "b", "Ljava/lang/Boolean;", arg1_bItem_0);
          request.arg1.b = static_cast<std::remove_reference_t<decltype(request.arg1.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg1_bItem_0));
    
          jobject arg1_cItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "c", "Ljava/lang/Integer;", arg1_cItem_0);
          request.arg1.c = static_cast<std::remove_reference_t<decltype(request.arg1.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_cItem_0));
          jobject arg1_dItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "d", "[B", arg1_dItem_0);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(arg1_dItem_0)));
        request.arg1.d = cleanupByteArrays.back()->byteSpan();
          jobject arg1_eItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "e", "Ljava/lang/String;", arg1_eItem_0);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(arg1_eItem_0)));
        request.arg1.e = cleanupStrings.back()->charSpan();
          jobject arg1_fItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "f", "Ljava/lang/Integer;", arg1_fItem_0);
          request.arg1.f.SetRaw(static_cast<std::remove_reference_t<decltype(request.arg1.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_fItem_0)));
    
          jobject arg1_gItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "g", "Ljava/lang/Float;", arg1_gItem_0);
          request.arg1.g = static_cast<std::remove_reference_t<decltype(request.arg1.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(arg1_gItem_0));
    
          jobject arg1_hItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "h", "Ljava/lang/Double;", arg1_hItem_0);
          request.arg1.h = static_cast<std::remove_reference_t<decltype(request.arg1.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(arg1_hItem_0));
    

  
    std::unique_ptr<CHIPUnitTestingClusterBooleanResponseCallback, void (*)(CHIPUnitTestingClusterBooleanResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterBooleanResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterBooleanResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBooleanResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testNestedStructArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestNestedStructArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;
          jobject arg1_aItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "a", "Ljava/lang/Integer;", arg1_aItem_0);
          request.arg1.a = static_cast<std::remove_reference_t<decltype(request.arg1.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_aItem_0));
    
          jobject arg1_bItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "b", "Ljava/lang/Boolean;", arg1_bItem_0);
          request.arg1.b = static_cast<std::remove_reference_t<decltype(request.arg1.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg1_bItem_0));
    
          jobject arg1_cItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "c", "Lchip/devicecontroller/ChipStructs$UnitTestingClusterSimpleStruct;", arg1_cItem_0);
          
          jobject arg1_cItem_0_aItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "a", "Ljava/lang/Integer;", arg1_cItem_0_aItem_1);
          request.arg1.c.a = static_cast<std::remove_reference_t<decltype(request.arg1.c.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_cItem_0_aItem_1));
    
          jobject arg1_cItem_0_bItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "b", "Ljava/lang/Boolean;", arg1_cItem_0_bItem_1);
          request.arg1.c.b = static_cast<std::remove_reference_t<decltype(request.arg1.c.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg1_cItem_0_bItem_1));
    
          jobject arg1_cItem_0_cItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "c", "Ljava/lang/Integer;", arg1_cItem_0_cItem_1);
          request.arg1.c.c = static_cast<std::remove_reference_t<decltype(request.arg1.c.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_cItem_0_cItem_1));
          jobject arg1_cItem_0_dItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "d", "[B", arg1_cItem_0_dItem_1);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(arg1_cItem_0_dItem_1)));
        request.arg1.c.d = cleanupByteArrays.back()->byteSpan();
          jobject arg1_cItem_0_eItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "e", "Ljava/lang/String;", arg1_cItem_0_eItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(arg1_cItem_0_eItem_1)));
        request.arg1.c.e = cleanupStrings.back()->charSpan();
          jobject arg1_cItem_0_fItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "f", "Ljava/lang/Integer;", arg1_cItem_0_fItem_1);
          request.arg1.c.f.SetRaw(static_cast<std::remove_reference_t<decltype(request.arg1.c.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_cItem_0_fItem_1)));
    
          jobject arg1_cItem_0_gItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "g", "Ljava/lang/Float;", arg1_cItem_0_gItem_1);
          request.arg1.c.g = static_cast<std::remove_reference_t<decltype(request.arg1.c.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(arg1_cItem_0_gItem_1));
    
          jobject arg1_cItem_0_hItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "h", "Ljava/lang/Double;", arg1_cItem_0_hItem_1);
          request.arg1.c.h = static_cast<std::remove_reference_t<decltype(request.arg1.c.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(arg1_cItem_0_hItem_1));
    

  
    std::unique_ptr<CHIPUnitTestingClusterBooleanResponseCallback, void (*)(CHIPUnitTestingClusterBooleanResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterBooleanResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterBooleanResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBooleanResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testListStructArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestListStructArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    using ListType_0 = std::remove_reference_t<decltype(request.arg1)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint arg1Size;
    chip::JniReferences::GetInstance().GetListSize(arg1, arg1Size);
    if (arg1Size != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(arg1Size);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < arg1Size; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(arg1, i_0, element_0);
        
          jobject element_0_aItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "a", "Ljava/lang/Integer;", element_0_aItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].a = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_aItem_1));
    
          jobject element_0_bItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "b", "Ljava/lang/Boolean;", element_0_bItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].b = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_0_bItem_1));
    
          jobject element_0_cItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "c", "Ljava/lang/Integer;", element_0_cItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_cItem_1));
          jobject element_0_dItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "d", "[B", element_0_dItem_1);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_0_dItem_1)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].d = cleanupByteArrays.back()->byteSpan();
          jobject element_0_eItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "e", "Ljava/lang/String;", element_0_eItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_0_eItem_1)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].e = cleanupStrings.back()->charSpan();
          jobject element_0_fItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "f", "Ljava/lang/Integer;", element_0_fItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].f.SetRaw(static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_fItem_1)));
    
          jobject element_0_gItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "g", "Ljava/lang/Float;", element_0_gItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].g = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(element_0_gItem_1));
    
          jobject element_0_hItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "h", "Ljava/lang/Double;", element_0_hItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].h = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(element_0_hItem_1));
    
      }
      request.arg1 = ListType_0(listHolder_0->mList, arg1Size);
    } else {
      request.arg1 = ListType_0();
    }
  }

  
    std::unique_ptr<CHIPUnitTestingClusterBooleanResponseCallback, void (*)(CHIPUnitTestingClusterBooleanResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterBooleanResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterBooleanResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBooleanResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testListInt8UArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestListInt8UArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    using ListType_0 = std::remove_reference_t<decltype(request.arg1)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint arg1Size;
    chip::JniReferences::GetInstance().GetListSize(arg1, arg1Size);
    if (arg1Size != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(arg1Size);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < arg1Size; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(arg1, i_0, element_0);
        listHolder_0->mList[static_cast<uint32_t>(i_0)] = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0));
    
      }
      request.arg1 = ListType_0(listHolder_0->mList, arg1Size);
    } else {
      request.arg1 = ListType_0();
    }
  }

  
    std::unique_ptr<CHIPUnitTestingClusterBooleanResponseCallback, void (*)(CHIPUnitTestingClusterBooleanResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterBooleanResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterBooleanResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBooleanResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testNestedStructListArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestNestedStructListArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;
          jobject arg1_aItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "a", "Ljava/lang/Integer;", arg1_aItem_0);
          request.arg1.a = static_cast<std::remove_reference_t<decltype(request.arg1.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_aItem_0));
    
          jobject arg1_bItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "b", "Ljava/lang/Boolean;", arg1_bItem_0);
          request.arg1.b = static_cast<std::remove_reference_t<decltype(request.arg1.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg1_bItem_0));
    
          jobject arg1_cItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "c", "Lchip/devicecontroller/ChipStructs$UnitTestingClusterSimpleStruct;", arg1_cItem_0);
          
          jobject arg1_cItem_0_aItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "a", "Ljava/lang/Integer;", arg1_cItem_0_aItem_1);
          request.arg1.c.a = static_cast<std::remove_reference_t<decltype(request.arg1.c.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_cItem_0_aItem_1));
    
          jobject arg1_cItem_0_bItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "b", "Ljava/lang/Boolean;", arg1_cItem_0_bItem_1);
          request.arg1.c.b = static_cast<std::remove_reference_t<decltype(request.arg1.c.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg1_cItem_0_bItem_1));
    
          jobject arg1_cItem_0_cItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "c", "Ljava/lang/Integer;", arg1_cItem_0_cItem_1);
          request.arg1.c.c = static_cast<std::remove_reference_t<decltype(request.arg1.c.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_cItem_0_cItem_1));
          jobject arg1_cItem_0_dItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "d", "[B", arg1_cItem_0_dItem_1);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(arg1_cItem_0_dItem_1)));
        request.arg1.c.d = cleanupByteArrays.back()->byteSpan();
          jobject arg1_cItem_0_eItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "e", "Ljava/lang/String;", arg1_cItem_0_eItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(arg1_cItem_0_eItem_1)));
        request.arg1.c.e = cleanupStrings.back()->charSpan();
          jobject arg1_cItem_0_fItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "f", "Ljava/lang/Integer;", arg1_cItem_0_fItem_1);
          request.arg1.c.f.SetRaw(static_cast<std::remove_reference_t<decltype(request.arg1.c.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_cItem_0_fItem_1)));
    
          jobject arg1_cItem_0_gItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "g", "Ljava/lang/Float;", arg1_cItem_0_gItem_1);
          request.arg1.c.g = static_cast<std::remove_reference_t<decltype(request.arg1.c.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(arg1_cItem_0_gItem_1));
    
          jobject arg1_cItem_0_hItem_1;
          chip::JniReferences::GetInstance().GetObjectField(arg1_cItem_0, "h", "Ljava/lang/Double;", arg1_cItem_0_hItem_1);
          request.arg1.c.h = static_cast<std::remove_reference_t<decltype(request.arg1.c.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(arg1_cItem_0_hItem_1));
    
          jobject arg1_dItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "d", "Ljava/util/ArrayList;", arg1_dItem_0);
          {
    using ListType_1 = std::remove_reference_t<decltype(request.arg1.d)>;
    using ListMemberType_1 = ListMemberTypeGetter<ListType_1>::Type;
    jint arg1_dItem_0Size;
    chip::JniReferences::GetInstance().GetListSize(arg1_dItem_0, arg1_dItem_0Size);
    if (arg1_dItem_0Size != 0) {
      auto * listHolder_1 = new ListHolder<ListMemberType_1>(arg1_dItem_0Size);
      listFreer.add(listHolder_1);

      for (jint i_1 = 0; i_1 < arg1_dItem_0Size; ++i_1) {
        jobject element_1;
        chip::JniReferences::GetInstance().GetListItem(arg1_dItem_0, i_1, element_1);
        
          jobject element_1_aItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "a", "Ljava/lang/Integer;", element_1_aItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].a = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1_aItem_2));
    
          jobject element_1_bItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "b", "Ljava/lang/Boolean;", element_1_bItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].b = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_1_bItem_2));
    
          jobject element_1_cItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "c", "Ljava/lang/Integer;", element_1_cItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].c = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1_cItem_2));
          jobject element_1_dItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "d", "[B", element_1_dItem_2);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_1_dItem_2)));
        listHolder_1->mList[static_cast<uint32_t>(i_1)].d = cleanupByteArrays.back()->byteSpan();
          jobject element_1_eItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "e", "Ljava/lang/String;", element_1_eItem_2);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_1_eItem_2)));
        listHolder_1->mList[static_cast<uint32_t>(i_1)].e = cleanupStrings.back()->charSpan();
          jobject element_1_fItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "f", "Ljava/lang/Integer;", element_1_fItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].f.SetRaw(static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1_fItem_2)));
    
          jobject element_1_gItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "g", "Ljava/lang/Float;", element_1_gItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].g = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(element_1_gItem_2));
    
          jobject element_1_hItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "h", "Ljava/lang/Double;", element_1_hItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].h = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(element_1_hItem_2));
    
      }
      request.arg1.d = ListType_1(listHolder_1->mList, arg1_dItem_0Size);
    } else {
      request.arg1.d = ListType_1();
    }
  }
          jobject arg1_eItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "e", "Ljava/util/ArrayList;", arg1_eItem_0);
          {
    using ListType_1 = std::remove_reference_t<decltype(request.arg1.e)>;
    using ListMemberType_1 = ListMemberTypeGetter<ListType_1>::Type;
    jint arg1_eItem_0Size;
    chip::JniReferences::GetInstance().GetListSize(arg1_eItem_0, arg1_eItem_0Size);
    if (arg1_eItem_0Size != 0) {
      auto * listHolder_1 = new ListHolder<ListMemberType_1>(arg1_eItem_0Size);
      listFreer.add(listHolder_1);

      for (jint i_1 = 0; i_1 < arg1_eItem_0Size; ++i_1) {
        jobject element_1;
        chip::JniReferences::GetInstance().GetListItem(arg1_eItem_0, i_1, element_1);
        listHolder_1->mList[static_cast<uint32_t>(i_1)] = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)])>>(chip::JniReferences::GetInstance().LongToPrimitive(element_1));
    
      }
      request.arg1.e = ListType_1(listHolder_1->mList, arg1_eItem_0Size);
    } else {
      request.arg1.e = ListType_1();
    }
  }
          jobject arg1_fItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "f", "Ljava/util/ArrayList;", arg1_fItem_0);
          {
    using ListType_1 = std::remove_reference_t<decltype(request.arg1.f)>;
    using ListMemberType_1 = ListMemberTypeGetter<ListType_1>::Type;
    jint arg1_fItem_0Size;
    chip::JniReferences::GetInstance().GetListSize(arg1_fItem_0, arg1_fItem_0Size);
    if (arg1_fItem_0Size != 0) {
      auto * listHolder_1 = new ListHolder<ListMemberType_1>(arg1_fItem_0Size);
      listFreer.add(listHolder_1);

      for (jint i_1 = 0; i_1 < arg1_fItem_0Size; ++i_1) {
        jobject element_1;
        chip::JniReferences::GetInstance().GetListItem(arg1_fItem_0, i_1, element_1);
        cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_1)));
        listHolder_1->mList[static_cast<uint32_t>(i_1)] = cleanupByteArrays.back()->byteSpan();
      }
      request.arg1.f = ListType_1(listHolder_1->mList, arg1_fItem_0Size);
    } else {
      request.arg1.f = ListType_1();
    }
  }
          jobject arg1_gItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "g", "Ljava/util/ArrayList;", arg1_gItem_0);
          {
    using ListType_1 = std::remove_reference_t<decltype(request.arg1.g)>;
    using ListMemberType_1 = ListMemberTypeGetter<ListType_1>::Type;
    jint arg1_gItem_0Size;
    chip::JniReferences::GetInstance().GetListSize(arg1_gItem_0, arg1_gItem_0Size);
    if (arg1_gItem_0Size != 0) {
      auto * listHolder_1 = new ListHolder<ListMemberType_1>(arg1_gItem_0Size);
      listFreer.add(listHolder_1);

      for (jint i_1 = 0; i_1 < arg1_gItem_0Size; ++i_1) {
        jobject element_1;
        chip::JniReferences::GetInstance().GetListItem(arg1_gItem_0, i_1, element_1);
        listHolder_1->mList[static_cast<uint32_t>(i_1)] = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1));
    
      }
      request.arg1.g = ListType_1(listHolder_1->mList, arg1_gItem_0Size);
    } else {
      request.arg1.g = ListType_1();
    }
  }

  
    std::unique_ptr<CHIPUnitTestingClusterBooleanResponseCallback, void (*)(CHIPUnitTestingClusterBooleanResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterBooleanResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterBooleanResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBooleanResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testListNestedStructListArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestListNestedStructListArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    using ListType_0 = std::remove_reference_t<decltype(request.arg1)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint arg1Size;
    chip::JniReferences::GetInstance().GetListSize(arg1, arg1Size);
    if (arg1Size != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(arg1Size);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < arg1Size; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(arg1, i_0, element_0);
        
          jobject element_0_aItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "a", "Ljava/lang/Integer;", element_0_aItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].a = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_aItem_1));
    
          jobject element_0_bItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "b", "Ljava/lang/Boolean;", element_0_bItem_1);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].b = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_0_bItem_1));
    
          jobject element_0_cItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "c", "Lchip/devicecontroller/ChipStructs$UnitTestingClusterSimpleStruct;", element_0_cItem_1);
          
          jobject element_0_cItem_1_aItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "a", "Ljava/lang/Integer;", element_0_cItem_1_aItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.a = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_cItem_1_aItem_2));
    
          jobject element_0_cItem_1_bItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "b", "Ljava/lang/Boolean;", element_0_cItem_1_bItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.b = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_0_cItem_1_bItem_2));
    
          jobject element_0_cItem_1_cItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "c", "Ljava/lang/Integer;", element_0_cItem_1_cItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.c = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_cItem_1_cItem_2));
          jobject element_0_cItem_1_dItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "d", "[B", element_0_cItem_1_dItem_2);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_0_cItem_1_dItem_2)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].c.d = cleanupByteArrays.back()->byteSpan();
          jobject element_0_cItem_1_eItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "e", "Ljava/lang/String;", element_0_cItem_1_eItem_2);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_0_cItem_1_eItem_2)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].c.e = cleanupStrings.back()->charSpan();
          jobject element_0_cItem_1_fItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "f", "Ljava/lang/Integer;", element_0_cItem_1_fItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.f.SetRaw(static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0_cItem_1_fItem_2)));
    
          jobject element_0_cItem_1_gItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "g", "Ljava/lang/Float;", element_0_cItem_1_gItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.g = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(element_0_cItem_1_gItem_2));
    
          jobject element_0_cItem_1_hItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_0_cItem_1, "h", "Ljava/lang/Double;", element_0_cItem_1_hItem_2);
          listHolder_0->mList[static_cast<uint32_t>(i_0)].c.h = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].c.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(element_0_cItem_1_hItem_2));
    
          jobject element_0_dItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "d", "Ljava/util/ArrayList;", element_0_dItem_1);
          {
    using ListType_2 = std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].d)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint element_0_dItem_1Size;
    chip::JniReferences::GetInstance().GetListSize(element_0_dItem_1, element_0_dItem_1Size);
    if (element_0_dItem_1Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(element_0_dItem_1Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < element_0_dItem_1Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(element_0_dItem_1, i_2, element_2);
        
          jobject element_2_aItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "a", "Ljava/lang/Integer;", element_2_aItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].a = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2_aItem_3));
    
          jobject element_2_bItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "b", "Ljava/lang/Boolean;", element_2_bItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].b = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(element_2_bItem_3));
    
          jobject element_2_cItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "c", "Ljava/lang/Integer;", element_2_cItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].c = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2_cItem_3));
          jobject element_2_dItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "d", "[B", element_2_dItem_3);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_2_dItem_3)));
        listHolder_2->mList[static_cast<uint32_t>(i_2)].d = cleanupByteArrays.back()->byteSpan();
          jobject element_2_eItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "e", "Ljava/lang/String;", element_2_eItem_3);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_2_eItem_3)));
        listHolder_2->mList[static_cast<uint32_t>(i_2)].e = cleanupStrings.back()->charSpan();
          jobject element_2_fItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "f", "Ljava/lang/Integer;", element_2_fItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].f.SetRaw(static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2_fItem_3)));
    
          jobject element_2_gItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "g", "Ljava/lang/Float;", element_2_gItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].g = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(element_2_gItem_3));
    
          jobject element_2_hItem_3;
          chip::JniReferences::GetInstance().GetObjectField(element_2, "h", "Ljava/lang/Double;", element_2_hItem_3);
          listHolder_2->mList[static_cast<uint32_t>(i_2)].h = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)].h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(element_2_hItem_3));
    
      }
      listHolder_0->mList[static_cast<uint32_t>(i_0)].d = ListType_2(listHolder_2->mList, element_0_dItem_1Size);
    } else {
      listHolder_0->mList[static_cast<uint32_t>(i_0)].d = ListType_2();
    }
  }
          jobject element_0_eItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "e", "Ljava/util/ArrayList;", element_0_eItem_1);
          {
    using ListType_2 = std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].e)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint element_0_eItem_1Size;
    chip::JniReferences::GetInstance().GetListSize(element_0_eItem_1, element_0_eItem_1Size);
    if (element_0_eItem_1Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(element_0_eItem_1Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < element_0_eItem_1Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(element_0_eItem_1, i_2, element_2);
        listHolder_2->mList[static_cast<uint32_t>(i_2)] = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)])>>(chip::JniReferences::GetInstance().LongToPrimitive(element_2));
    
      }
      listHolder_0->mList[static_cast<uint32_t>(i_0)].e = ListType_2(listHolder_2->mList, element_0_eItem_1Size);
    } else {
      listHolder_0->mList[static_cast<uint32_t>(i_0)].e = ListType_2();
    }
  }
          jobject element_0_fItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "f", "Ljava/util/ArrayList;", element_0_fItem_1);
          {
    using ListType_2 = std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].f)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint element_0_fItem_1Size;
    chip::JniReferences::GetInstance().GetListSize(element_0_fItem_1, element_0_fItem_1Size);
    if (element_0_fItem_1Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(element_0_fItem_1Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < element_0_fItem_1Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(element_0_fItem_1, i_2, element_2);
        cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(element_2)));
        listHolder_2->mList[static_cast<uint32_t>(i_2)] = cleanupByteArrays.back()->byteSpan();
      }
      listHolder_0->mList[static_cast<uint32_t>(i_0)].f = ListType_2(listHolder_2->mList, element_0_fItem_1Size);
    } else {
      listHolder_0->mList[static_cast<uint32_t>(i_0)].f = ListType_2();
    }
  }
          jobject element_0_gItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "g", "Ljava/util/ArrayList;", element_0_gItem_1);
          {
    using ListType_2 = std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)].g)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint element_0_gItem_1Size;
    chip::JniReferences::GetInstance().GetListSize(element_0_gItem_1, element_0_gItem_1Size);
    if (element_0_gItem_1Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(element_0_gItem_1Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < element_0_gItem_1Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(element_0_gItem_1, i_2, element_2);
        listHolder_2->mList[static_cast<uint32_t>(i_2)] = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2));
    
      }
      listHolder_0->mList[static_cast<uint32_t>(i_0)].g = ListType_2(listHolder_2->mList, element_0_gItem_1Size);
    } else {
      listHolder_0->mList[static_cast<uint32_t>(i_0)].g = ListType_2();
    }
  }
      }
      request.arg1 = ListType_0(listHolder_0->mList, arg1Size);
    } else {
      request.arg1 = ListType_0();
    }
  }

  
    std::unique_ptr<CHIPUnitTestingClusterBooleanResponseCallback, void (*)(CHIPUnitTestingClusterBooleanResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterBooleanResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterBooleanResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBooleanResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testListInt8UReverseRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestListInt8UReverseRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    using ListType_0 = std::remove_reference_t<decltype(request.arg1)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint arg1Size;
    chip::JniReferences::GetInstance().GetListSize(arg1, arg1Size);
    if (arg1Size != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(arg1Size);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < arg1Size; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(arg1, i_0, element_0);
        listHolder_0->mList[static_cast<uint32_t>(i_0)] = static_cast<std::remove_reference_t<decltype(listHolder_0->mList[static_cast<uint32_t>(i_0)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_0));
    
      }
      request.arg1 = ListType_0(listHolder_0->mList, arg1Size);
    } else {
      request.arg1 = ListType_0();
    }
  }

  
    std::unique_ptr<CHIPUnitTestingClusterTestListInt8UReverseResponseCallback, void (*)(CHIPUnitTestingClusterTestListInt8UReverseResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestListInt8UReverseResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestListInt8UReverseResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestListInt8UReverseResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testEnumsRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject arg2,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestEnumsRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.arg1 = static_cast<std::remove_reference_t<decltype(request.arg1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1));
    request.arg2 = static_cast<std::remove_reference_t<decltype(request.arg2)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg2));

  
    std::unique_ptr<CHIPUnitTestingClusterTestEnumsResponseCallback, void (*)(CHIPUnitTestingClusterTestEnumsResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestEnumsResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestEnumsResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestEnumsResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testNullableOptionalRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestNullableOptionalRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(arg1, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.arg1.Emplace();
      if (optionalValue_0 == nullptr) {
    definedValue_0.SetNull();
  } else {
    auto & nonNullValue_1 = definedValue_0.SetNonNull();
    nonNullValue_1 = static_cast<std::remove_reference_t<decltype(nonNullValue_1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0));
    
  }
    }
  }

  
    std::unique_ptr<CHIPUnitTestingClusterTestNullableOptionalResponseCallback, void (*)(CHIPUnitTestingClusterTestNullableOptionalResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestNullableOptionalResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestNullableOptionalResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestNullableOptionalResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testComplexNullableOptionalRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject nullableInt,jobject optionalInt,jobject nullableOptionalInt,jstring nullableString,jobject optionalString,jobject nullableOptionalString,jobject nullableStruct,jobject optionalStruct,jobject nullableOptionalStruct,jobject nullableList,jobject optionalList,jobject nullableOptionalList,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestComplexNullableOptionalRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;if (nullableInt == nullptr) {
    request.nullableInt.SetNull();
  } else {
    auto & nonNullValue_0 = request.nullableInt.SetNonNull();
    nonNullValue_0 = static_cast<std::remove_reference_t<decltype(nonNullValue_0)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(nullableInt));
    
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalInt, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.optionalInt.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0));
    
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(nullableOptionalInt, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.nullableOptionalInt.Emplace();
      if (optionalValue_0 == nullptr) {
    definedValue_0.SetNull();
  } else {
    auto & nonNullValue_1 = definedValue_0.SetNonNull();
    nonNullValue_1 = static_cast<std::remove_reference_t<decltype(nonNullValue_1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0));
    
  }
    }
  }if (nullableString == nullptr) {
    request.nullableString.SetNull();
  } else {
    auto & nonNullValue_0 = request.nullableString.SetNonNull();
    cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(nullableString)));
        nonNullValue_0 = cleanupStrings.back()->charSpan();
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalString, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.optionalString.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_0)));
        definedValue_0 = cleanupStrings.back()->charSpan();
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(nullableOptionalString, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.nullableOptionalString.Emplace();
      if (optionalValue_0 == nullptr) {
    definedValue_0.SetNull();
  } else {
    auto & nonNullValue_1 = definedValue_0.SetNonNull();
    cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_0)));
        nonNullValue_1 = cleanupStrings.back()->charSpan();
  }
    }
  }if (nullableStruct == nullptr) {
    request.nullableStruct.SetNull();
  } else {
    auto & nonNullValue_0 = request.nullableStruct.SetNonNull();
    
          jobject nullableStruct_aItem_1;
          chip::JniReferences::GetInstance().GetObjectField(nullableStruct, "a", "Ljava/lang/Integer;", nullableStruct_aItem_1);
          nonNullValue_0.a = static_cast<std::remove_reference_t<decltype(nonNullValue_0.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(nullableStruct_aItem_1));
    
          jobject nullableStruct_bItem_1;
          chip::JniReferences::GetInstance().GetObjectField(nullableStruct, "b", "Ljava/lang/Boolean;", nullableStruct_bItem_1);
          nonNullValue_0.b = static_cast<std::remove_reference_t<decltype(nonNullValue_0.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(nullableStruct_bItem_1));
    
          jobject nullableStruct_cItem_1;
          chip::JniReferences::GetInstance().GetObjectField(nullableStruct, "c", "Ljava/lang/Integer;", nullableStruct_cItem_1);
          nonNullValue_0.c = static_cast<std::remove_reference_t<decltype(nonNullValue_0.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(nullableStruct_cItem_1));
          jobject nullableStruct_dItem_1;
          chip::JniReferences::GetInstance().GetObjectField(nullableStruct, "d", "[B", nullableStruct_dItem_1);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(nullableStruct_dItem_1)));
        nonNullValue_0.d = cleanupByteArrays.back()->byteSpan();
          jobject nullableStruct_eItem_1;
          chip::JniReferences::GetInstance().GetObjectField(nullableStruct, "e", "Ljava/lang/String;", nullableStruct_eItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(nullableStruct_eItem_1)));
        nonNullValue_0.e = cleanupStrings.back()->charSpan();
          jobject nullableStruct_fItem_1;
          chip::JniReferences::GetInstance().GetObjectField(nullableStruct, "f", "Ljava/lang/Integer;", nullableStruct_fItem_1);
          nonNullValue_0.f.SetRaw(static_cast<std::remove_reference_t<decltype(nonNullValue_0.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(nullableStruct_fItem_1)));
    
          jobject nullableStruct_gItem_1;
          chip::JniReferences::GetInstance().GetObjectField(nullableStruct, "g", "Ljava/lang/Float;", nullableStruct_gItem_1);
          nonNullValue_0.g = static_cast<std::remove_reference_t<decltype(nonNullValue_0.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(nullableStruct_gItem_1));
    
          jobject nullableStruct_hItem_1;
          chip::JniReferences::GetInstance().GetObjectField(nullableStruct, "h", "Ljava/lang/Double;", nullableStruct_hItem_1);
          nonNullValue_0.h = static_cast<std::remove_reference_t<decltype(nonNullValue_0.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(nullableStruct_hItem_1));
    
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalStruct, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.optionalStruct.Emplace();
      
          jobject optionalValue_0_aItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "a", "Ljava/lang/Integer;", optionalValue_0_aItem_1);
          definedValue_0.a = static_cast<std::remove_reference_t<decltype(definedValue_0.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0_aItem_1));
    
          jobject optionalValue_0_bItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "b", "Ljava/lang/Boolean;", optionalValue_0_bItem_1);
          definedValue_0.b = static_cast<std::remove_reference_t<decltype(definedValue_0.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(optionalValue_0_bItem_1));
    
          jobject optionalValue_0_cItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "c", "Ljava/lang/Integer;", optionalValue_0_cItem_1);
          definedValue_0.c = static_cast<std::remove_reference_t<decltype(definedValue_0.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0_cItem_1));
          jobject optionalValue_0_dItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "d", "[B", optionalValue_0_dItem_1);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(optionalValue_0_dItem_1)));
        definedValue_0.d = cleanupByteArrays.back()->byteSpan();
          jobject optionalValue_0_eItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "e", "Ljava/lang/String;", optionalValue_0_eItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_0_eItem_1)));
        definedValue_0.e = cleanupStrings.back()->charSpan();
          jobject optionalValue_0_fItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "f", "Ljava/lang/Integer;", optionalValue_0_fItem_1);
          definedValue_0.f.SetRaw(static_cast<std::remove_reference_t<decltype(definedValue_0.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0_fItem_1)));
    
          jobject optionalValue_0_gItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "g", "Ljava/lang/Float;", optionalValue_0_gItem_1);
          definedValue_0.g = static_cast<std::remove_reference_t<decltype(definedValue_0.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(optionalValue_0_gItem_1));
    
          jobject optionalValue_0_hItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "h", "Ljava/lang/Double;", optionalValue_0_hItem_1);
          definedValue_0.h = static_cast<std::remove_reference_t<decltype(definedValue_0.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_0_hItem_1));
    
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(nullableOptionalStruct, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.nullableOptionalStruct.Emplace();
      if (optionalValue_0 == nullptr) {
    definedValue_0.SetNull();
  } else {
    auto & nonNullValue_1 = definedValue_0.SetNonNull();
    
          jobject optionalValue_0_aItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "a", "Ljava/lang/Integer;", optionalValue_0_aItem_2);
          nonNullValue_1.a = static_cast<std::remove_reference_t<decltype(nonNullValue_1.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0_aItem_2));
    
          jobject optionalValue_0_bItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "b", "Ljava/lang/Boolean;", optionalValue_0_bItem_2);
          nonNullValue_1.b = static_cast<std::remove_reference_t<decltype(nonNullValue_1.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(optionalValue_0_bItem_2));
    
          jobject optionalValue_0_cItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "c", "Ljava/lang/Integer;", optionalValue_0_cItem_2);
          nonNullValue_1.c = static_cast<std::remove_reference_t<decltype(nonNullValue_1.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0_cItem_2));
          jobject optionalValue_0_dItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "d", "[B", optionalValue_0_dItem_2);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(optionalValue_0_dItem_2)));
        nonNullValue_1.d = cleanupByteArrays.back()->byteSpan();
          jobject optionalValue_0_eItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "e", "Ljava/lang/String;", optionalValue_0_eItem_2);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_0_eItem_2)));
        nonNullValue_1.e = cleanupStrings.back()->charSpan();
          jobject optionalValue_0_fItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "f", "Ljava/lang/Integer;", optionalValue_0_fItem_2);
          nonNullValue_1.f.SetRaw(static_cast<std::remove_reference_t<decltype(nonNullValue_1.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0_fItem_2)));
    
          jobject optionalValue_0_gItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "g", "Ljava/lang/Float;", optionalValue_0_gItem_2);
          nonNullValue_1.g = static_cast<std::remove_reference_t<decltype(nonNullValue_1.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(optionalValue_0_gItem_2));
    
          jobject optionalValue_0_hItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "h", "Ljava/lang/Double;", optionalValue_0_hItem_2);
          nonNullValue_1.h = static_cast<std::remove_reference_t<decltype(nonNullValue_1.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_0_hItem_2));
    
  }
    }
  }if (nullableList == nullptr) {
    request.nullableList.SetNull();
  } else {
    auto & nonNullValue_0 = request.nullableList.SetNonNull();
    {
    using ListType_1 = std::remove_reference_t<decltype(nonNullValue_0)>;
    using ListMemberType_1 = ListMemberTypeGetter<ListType_1>::Type;
    jint nullableListSize;
    chip::JniReferences::GetInstance().GetListSize(nullableList, nullableListSize);
    if (nullableListSize != 0) {
      auto * listHolder_1 = new ListHolder<ListMemberType_1>(nullableListSize);
      listFreer.add(listHolder_1);

      for (jint i_1 = 0; i_1 < nullableListSize; ++i_1) {
        jobject element_1;
        chip::JniReferences::GetInstance().GetListItem(nullableList, i_1, element_1);
        listHolder_1->mList[static_cast<uint32_t>(i_1)] = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1));
      }
      nonNullValue_0 = ListType_1(listHolder_1->mList, nullableListSize);
    } else {
      nonNullValue_0 = ListType_1();
    }
  }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalList, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.optionalList.Emplace();
      {
    using ListType_1 = std::remove_reference_t<decltype(definedValue_0)>;
    using ListMemberType_1 = ListMemberTypeGetter<ListType_1>::Type;
    jint optionalValue_0Size;
    chip::JniReferences::GetInstance().GetListSize(optionalValue_0, optionalValue_0Size);
    if (optionalValue_0Size != 0) {
      auto * listHolder_1 = new ListHolder<ListMemberType_1>(optionalValue_0Size);
      listFreer.add(listHolder_1);

      for (jint i_1 = 0; i_1 < optionalValue_0Size; ++i_1) {
        jobject element_1;
        chip::JniReferences::GetInstance().GetListItem(optionalValue_0, i_1, element_1);
        listHolder_1->mList[static_cast<uint32_t>(i_1)] = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1));
      }
      definedValue_0 = ListType_1(listHolder_1->mList, optionalValue_0Size);
    } else {
      definedValue_0 = ListType_1();
    }
  }
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(nullableOptionalList, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.nullableOptionalList.Emplace();
      if (optionalValue_0 == nullptr) {
    definedValue_0.SetNull();
  } else {
    auto & nonNullValue_1 = definedValue_0.SetNonNull();
    {
    using ListType_2 = std::remove_reference_t<decltype(nonNullValue_1)>;
    using ListMemberType_2 = ListMemberTypeGetter<ListType_2>::Type;
    jint optionalValue_0Size;
    chip::JniReferences::GetInstance().GetListSize(optionalValue_0, optionalValue_0Size);
    if (optionalValue_0Size != 0) {
      auto * listHolder_2 = new ListHolder<ListMemberType_2>(optionalValue_0Size);
      listFreer.add(listHolder_2);

      for (jint i_2 = 0; i_2 < optionalValue_0Size; ++i_2) {
        jobject element_2;
        chip::JniReferences::GetInstance().GetListItem(optionalValue_0, i_2, element_2);
        listHolder_2->mList[static_cast<uint32_t>(i_2)] = static_cast<std::remove_reference_t<decltype(listHolder_2->mList[static_cast<uint32_t>(i_2)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_2));
      }
      nonNullValue_1 = ListType_2(listHolder_2->mList, optionalValue_0Size);
    } else {
      nonNullValue_1 = ListType_2();
    }
  }
  }
    }
  }

  
    std::unique_ptr<CHIPUnitTestingClusterTestComplexNullableOptionalResponseCallback, void (*)(CHIPUnitTestingClusterTestComplexNullableOptionalResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestComplexNullableOptionalResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestComplexNullableOptionalResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestComplexNullableOptionalResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  simpleStructEchoRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::SimpleStructEchoRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;
          jobject arg1_aItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "a", "Ljava/lang/Integer;", arg1_aItem_0);
          request.arg1.a = static_cast<std::remove_reference_t<decltype(request.arg1.a)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_aItem_0));
    
          jobject arg1_bItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "b", "Ljava/lang/Boolean;", arg1_bItem_0);
          request.arg1.b = static_cast<std::remove_reference_t<decltype(request.arg1.b)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg1_bItem_0));
    
          jobject arg1_cItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "c", "Ljava/lang/Integer;", arg1_cItem_0);
          request.arg1.c = static_cast<std::remove_reference_t<decltype(request.arg1.c)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_cItem_0));
          jobject arg1_dItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "d", "[B", arg1_dItem_0);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(arg1_dItem_0)));
        request.arg1.d = cleanupByteArrays.back()->byteSpan();
          jobject arg1_eItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "e", "Ljava/lang/String;", arg1_eItem_0);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(arg1_eItem_0)));
        request.arg1.e = cleanupStrings.back()->charSpan();
          jobject arg1_fItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "f", "Ljava/lang/Integer;", arg1_fItem_0);
          request.arg1.f.SetRaw(static_cast<std::remove_reference_t<decltype(request.arg1.f)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1_fItem_0)));
    
          jobject arg1_gItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "g", "Ljava/lang/Float;", arg1_gItem_0);
          request.arg1.g = static_cast<std::remove_reference_t<decltype(request.arg1.g)>>(chip::JniReferences::GetInstance().FloatToPrimitive(arg1_gItem_0));
    
          jobject arg1_hItem_0;
          chip::JniReferences::GetInstance().GetObjectField(arg1, "h", "Ljava/lang/Double;", arg1_hItem_0);
          request.arg1.h = static_cast<std::remove_reference_t<decltype(request.arg1.h)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(arg1_hItem_0));
    

  
    std::unique_ptr<CHIPUnitTestingClusterSimpleStructResponseCallback, void (*)(CHIPUnitTestingClusterSimpleStructResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterSimpleStructResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterSimpleStructResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterSimpleStructResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  timedInvokeRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TimedInvokeRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;

  
    std::unique_ptr<CHIPDefaultSuccessCallback, void (*)(CHIPDefaultSuccessCallback *)> onSuccess(
        Platform::New<CHIPDefaultSuccessCallback>(callback), Platform::Delete<CHIPDefaultSuccessCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPDefaultSuccessCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

  err = cppCluster->InvokeCommand(request, onSuccess->mContext, successFn->mCall, failureFn->mCall, chip::JniReferences::GetInstance().IntegerToPrimitive(timedInvokeTimeoutMs));
    VerifyOrReturn(err == CHIP_NO_ERROR, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error invoking command", CHIP_ERROR_INCORRECT_STATE));

    onSuccess.release();
    onFailure.release();
}
JNI_METHOD(void, UnitTestingCluster, 
  testSimpleOptionalArgumentRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestSimpleOptionalArgumentRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(arg1, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.arg1.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(optionalValue_0));
    
    }
  }

  
    std::unique_ptr<CHIPDefaultSuccessCallback, void (*)(CHIPDefaultSuccessCallback *)> onSuccess(
        Platform::New<CHIPDefaultSuccessCallback>(callback), Platform::Delete<CHIPDefaultSuccessCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPDefaultSuccessCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testEmitTestEventRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject arg2,jobject arg3,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestEmitTestEventRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.arg1 = static_cast<std::remove_reference_t<decltype(request.arg1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1));
    request.arg2 = static_cast<std::remove_reference_t<decltype(request.arg2)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg2));request.arg3 = static_cast<std::remove_reference_t<decltype(request.arg3)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(arg3));
    

  
    std::unique_ptr<CHIPUnitTestingClusterTestEmitTestEventResponseCallback, void (*)(CHIPUnitTestingClusterTestEmitTestEventResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestEmitTestEventResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestEmitTestEventResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestEmitTestEventResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testEmitTestFabricScopedEventRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject arg1,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestEmitTestFabricScopedEventRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.arg1 = static_cast<std::remove_reference_t<decltype(request.arg1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(arg1));
    

  
    std::unique_ptr<CHIPUnitTestingClusterTestEmitTestFabricScopedEventResponseCallback, void (*)(CHIPUnitTestingClusterTestEmitTestFabricScopedEventResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestEmitTestFabricScopedEventResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestEmitTestFabricScopedEventResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestEmitTestFabricScopedEventResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testBatchHelperRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject sleepBeforeResponseTimeMs,jobject sizeOfResponseBuffer,jobject fillCharacter,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestBatchHelperRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.sleepBeforeResponseTimeMs = static_cast<std::remove_reference_t<decltype(request.sleepBeforeResponseTimeMs)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(sleepBeforeResponseTimeMs));
    request.sizeOfResponseBuffer = static_cast<std::remove_reference_t<decltype(request.sizeOfResponseBuffer)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(sizeOfResponseBuffer));
    request.fillCharacter = static_cast<std::remove_reference_t<decltype(request.fillCharacter)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(fillCharacter));
    

  
    std::unique_ptr<CHIPUnitTestingClusterTestBatchHelperResponseCallback, void (*)(CHIPUnitTestingClusterTestBatchHelperResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestBatchHelperResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestBatchHelperResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestBatchHelperResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, 
  testSecondBatchHelperRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject sleepBeforeResponseTimeMs,jobject sizeOfResponseBuffer,jobject fillCharacter,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::UnitTesting::Commands::TestSecondBatchHelperRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.sleepBeforeResponseTimeMs = static_cast<std::remove_reference_t<decltype(request.sleepBeforeResponseTimeMs)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(sleepBeforeResponseTimeMs));
    request.sizeOfResponseBuffer = static_cast<std::remove_reference_t<decltype(request.sizeOfResponseBuffer)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(sizeOfResponseBuffer));
    request.fillCharacter = static_cast<std::remove_reference_t<decltype(request.fillCharacter)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(fillCharacter));
    

  
    std::unique_ptr<CHIPUnitTestingClusterTestBatchHelperResponseCallback, void (*)(CHIPUnitTestingClusterTestBatchHelperResponseCallback *)> onSuccess(
        Platform::New<CHIPUnitTestingClusterTestBatchHelperResponseCallback>(callback), Platform::Delete<CHIPUnitTestingClusterTestBatchHelperResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTestBatchHelperResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, UnitTestingCluster, subscribeBooleanAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPBooleanAttributeCallback, void (*)(CHIPBooleanAttributeCallback *)> onSuccess(Platform::New<CHIPBooleanAttributeCallback>(callback, true), chip::Platform::Delete<CHIPBooleanAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Boolean::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBooleanAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPBooleanAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeBitmap8Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Bitmap8::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBitmap8AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeBitmap16Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Bitmap16::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBitmap16AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeBitmap32Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Bitmap32::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBitmap32AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeBitmap64Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64uAttributeCallback, void (*)(CHIPInt64uAttributeCallback *)> onSuccess(Platform::New<CHIPInt64uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Bitmap64::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterBitmap64AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt8uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int8u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt8uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt16uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int16u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt16uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt24uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int24u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt24uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt32uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int32u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt32uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt40uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64uAttributeCallback, void (*)(CHIPInt64uAttributeCallback *)> onSuccess(Platform::New<CHIPInt64uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int40u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt40uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt48uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64uAttributeCallback, void (*)(CHIPInt64uAttributeCallback *)> onSuccess(Platform::New<CHIPInt64uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int48u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt48uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt56uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64uAttributeCallback, void (*)(CHIPInt64uAttributeCallback *)> onSuccess(Platform::New<CHIPInt64uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int56u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt56uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt64uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64uAttributeCallback, void (*)(CHIPInt64uAttributeCallback *)> onSuccess(Platform::New<CHIPInt64uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int64u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt64uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt8sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8sAttributeCallback, void (*)(CHIPInt8sAttributeCallback *)> onSuccess(Platform::New<CHIPInt8sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int8s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt8sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt16sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(Platform::New<CHIPInt16sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int16s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt16sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt24sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32sAttributeCallback, void (*)(CHIPInt32sAttributeCallback *)> onSuccess(Platform::New<CHIPInt32sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int24s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt24sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt32sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32sAttributeCallback, void (*)(CHIPInt32sAttributeCallback *)> onSuccess(Platform::New<CHIPInt32sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int32s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt32sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt40sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64sAttributeCallback, void (*)(CHIPInt64sAttributeCallback *)> onSuccess(Platform::New<CHIPInt64sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int40s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt40sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt48sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64sAttributeCallback, void (*)(CHIPInt64sAttributeCallback *)> onSuccess(Platform::New<CHIPInt64sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int48s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt48sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt56sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64sAttributeCallback, void (*)(CHIPInt64sAttributeCallback *)> onSuccess(Platform::New<CHIPInt64sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int56s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt56sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeInt64sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64sAttributeCallback, void (*)(CHIPInt64sAttributeCallback *)> onSuccess(Platform::New<CHIPInt64sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Int64s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterInt64sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeEnum8Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Enum8::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterEnum8AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeEnum16Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Enum16::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterEnum16AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeFloatSingleAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPFloatAttributeCallback, void (*)(CHIPFloatAttributeCallback *)> onSuccess(Platform::New<CHIPFloatAttributeCallback>(callback, true), chip::Platform::Delete<CHIPFloatAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::FloatSingle::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterFloatSingleAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPFloatAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeFloatDoubleAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDoubleAttributeCallback, void (*)(CHIPDoubleAttributeCallback *)> onSuccess(Platform::New<CHIPDoubleAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDoubleAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::FloatDouble::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterFloatDoubleAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDoubleAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeOctetStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPOctetStringAttributeCallback, void (*)(CHIPOctetStringAttributeCallback *)> onSuccess(Platform::New<CHIPOctetStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPOctetStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::OctetString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterOctetStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPOctetStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeListInt8uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingListInt8uAttributeCallback, void (*)(CHIPUnitTestingListInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingListInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingListInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::ListInt8u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterListInt8uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingListInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeListOctetStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingListOctetStringAttributeCallback, void (*)(CHIPUnitTestingListOctetStringAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingListOctetStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingListOctetStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::ListOctetString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterListOctetStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingListOctetStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeListStructOctetStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingListStructOctetStringAttributeCallback, void (*)(CHIPUnitTestingListStructOctetStringAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingListStructOctetStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingListStructOctetStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::ListStructOctetString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterListStructOctetStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingListStructOctetStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeLongOctetStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPOctetStringAttributeCallback, void (*)(CHIPOctetStringAttributeCallback *)> onSuccess(Platform::New<CHIPOctetStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPOctetStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::LongOctetString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterLongOctetStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPOctetStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeCharStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPCharStringAttributeCallback, void (*)(CHIPCharStringAttributeCallback *)> onSuccess(Platform::New<CHIPCharStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPCharStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::CharString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterCharStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPCharStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeLongCharStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPCharStringAttributeCallback, void (*)(CHIPCharStringAttributeCallback *)> onSuccess(Platform::New<CHIPCharStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPCharStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::LongCharString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterLongCharStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPCharStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeEpochUsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt64uAttributeCallback, void (*)(CHIPInt64uAttributeCallback *)> onSuccess(Platform::New<CHIPInt64uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt64uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::EpochUs::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterEpochUsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt64uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeEpochSAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::EpochS::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterEpochSAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeVendorIdAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::VendorId::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterVendorIdAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeListNullablesAndOptionalsStructAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingListNullablesAndOptionalsStructAttributeCallback, void (*)(CHIPUnitTestingListNullablesAndOptionalsStructAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingListNullablesAndOptionalsStructAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingListNullablesAndOptionalsStructAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::ListNullablesAndOptionalsStruct::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterListNullablesAndOptionalsStructAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingListNullablesAndOptionalsStructAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeEnumAttrAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::EnumAttr::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterEnumAttrAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeRangeRestrictedInt8uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::RangeRestrictedInt8u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterRangeRestrictedInt8uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeRangeRestrictedInt8sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8sAttributeCallback, void (*)(CHIPInt8sAttributeCallback *)> onSuccess(Platform::New<CHIPInt8sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::RangeRestrictedInt8s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterRangeRestrictedInt8sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeRangeRestrictedInt16uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::RangeRestrictedInt16u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterRangeRestrictedInt16uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeRangeRestrictedInt16sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(Platform::New<CHIPInt16sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::RangeRestrictedInt16s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterRangeRestrictedInt16sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeListLongOctetStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingListLongOctetStringAttributeCallback, void (*)(CHIPUnitTestingListLongOctetStringAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingListLongOctetStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingListLongOctetStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::ListLongOctetString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterListLongOctetStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingListLongOctetStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeListFabricScopedAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingListFabricScopedAttributeCallback, void (*)(CHIPUnitTestingListFabricScopedAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingListFabricScopedAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingListFabricScopedAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::ListFabricScoped::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterListFabricScopedAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingListFabricScopedAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeTimedWriteBooleanAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPBooleanAttributeCallback, void (*)(CHIPBooleanAttributeCallback *)> onSuccess(Platform::New<CHIPBooleanAttributeCallback>(callback, true), chip::Platform::Delete<CHIPBooleanAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::TimedWriteBoolean::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterTimedWriteBooleanAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPBooleanAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeGeneralErrorBooleanAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPBooleanAttributeCallback, void (*)(CHIPBooleanAttributeCallback *)> onSuccess(Platform::New<CHIPBooleanAttributeCallback>(callback, true), chip::Platform::Delete<CHIPBooleanAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::GeneralErrorBoolean::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterGeneralErrorBooleanAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPBooleanAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeClusterErrorBooleanAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPBooleanAttributeCallback, void (*)(CHIPBooleanAttributeCallback *)> onSuccess(Platform::New<CHIPBooleanAttributeCallback>(callback, true), chip::Platform::Delete<CHIPBooleanAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::ClusterErrorBoolean::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterClusterErrorBooleanAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPBooleanAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeUnsupportedAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPBooleanAttributeCallback, void (*)(CHIPBooleanAttributeCallback *)> onSuccess(Platform::New<CHIPBooleanAttributeCallback>(callback, true), chip::Platform::Delete<CHIPBooleanAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::Unsupported::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterUnsupportedAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPBooleanAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableBooleanAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableBooleanAttributeCallback, void (*)(CHIPUnitTestingNullableBooleanAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableBooleanAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableBooleanAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableBoolean::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableBooleanAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableBooleanAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableBitmap8Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableBitmap8AttributeCallback, void (*)(CHIPUnitTestingNullableBitmap8AttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableBitmap8AttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableBitmap8AttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableBitmap8::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableBitmap8AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableBitmap8AttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableBitmap16Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableBitmap16AttributeCallback, void (*)(CHIPUnitTestingNullableBitmap16AttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableBitmap16AttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableBitmap16AttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableBitmap16::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableBitmap16AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableBitmap16AttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableBitmap32Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableBitmap32AttributeCallback, void (*)(CHIPUnitTestingNullableBitmap32AttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableBitmap32AttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableBitmap32AttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableBitmap32::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableBitmap32AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableBitmap32AttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableBitmap64Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableBitmap64AttributeCallback, void (*)(CHIPUnitTestingNullableBitmap64AttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableBitmap64AttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableBitmap64AttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableBitmap64::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableBitmap64AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableBitmap64AttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt8uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt8uAttributeCallback, void (*)(CHIPUnitTestingNullableInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt8u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt8uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt16uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt16uAttributeCallback, void (*)(CHIPUnitTestingNullableInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt16u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt16uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt24uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt24uAttributeCallback, void (*)(CHIPUnitTestingNullableInt24uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt24uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt24uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt24u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt24uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt24uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt32uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt32uAttributeCallback, void (*)(CHIPUnitTestingNullableInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt32u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt32uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt40uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt40uAttributeCallback, void (*)(CHIPUnitTestingNullableInt40uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt40uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt40uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt40u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt40uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt40uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt48uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt48uAttributeCallback, void (*)(CHIPUnitTestingNullableInt48uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt48uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt48uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt48u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt48uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt48uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt56uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt56uAttributeCallback, void (*)(CHIPUnitTestingNullableInt56uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt56uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt56uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt56u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt56uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt56uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt64uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt64uAttributeCallback, void (*)(CHIPUnitTestingNullableInt64uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt64uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt64uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt64u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt64uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt64uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt8sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt8sAttributeCallback, void (*)(CHIPUnitTestingNullableInt8sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt8sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt8s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt8sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt8sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt16sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt16sAttributeCallback, void (*)(CHIPUnitTestingNullableInt16sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt16sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt16s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt16sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt16sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt24sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt24sAttributeCallback, void (*)(CHIPUnitTestingNullableInt24sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt24sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt24sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt24s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt24sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt24sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt32sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt32sAttributeCallback, void (*)(CHIPUnitTestingNullableInt32sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt32sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt32sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt32s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt32sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt32sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt40sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt40sAttributeCallback, void (*)(CHIPUnitTestingNullableInt40sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt40sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt40sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt40s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt40sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt40sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt48sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt48sAttributeCallback, void (*)(CHIPUnitTestingNullableInt48sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt48sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt48sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt48s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt48sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt48sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt56sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt56sAttributeCallback, void (*)(CHIPUnitTestingNullableInt56sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt56sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt56sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt56s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt56sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt56sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableInt64sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableInt64sAttributeCallback, void (*)(CHIPUnitTestingNullableInt64sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableInt64sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableInt64sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableInt64s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableInt64sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableInt64sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableEnum8Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableEnum8AttributeCallback, void (*)(CHIPUnitTestingNullableEnum8AttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableEnum8AttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableEnum8AttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableEnum8::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableEnum8AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableEnum8AttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableEnum16Attribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableEnum16AttributeCallback, void (*)(CHIPUnitTestingNullableEnum16AttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableEnum16AttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableEnum16AttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableEnum16::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableEnum16AttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableEnum16AttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableFloatSingleAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableFloatSingleAttributeCallback, void (*)(CHIPUnitTestingNullableFloatSingleAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableFloatSingleAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableFloatSingleAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableFloatSingle::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableFloatSingleAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableFloatSingleAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableFloatDoubleAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableFloatDoubleAttributeCallback, void (*)(CHIPUnitTestingNullableFloatDoubleAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableFloatDoubleAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableFloatDoubleAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableFloatDouble::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableFloatDoubleAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableFloatDoubleAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableOctetStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableOctetStringAttributeCallback, void (*)(CHIPUnitTestingNullableOctetStringAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableOctetStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableOctetStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableOctetString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableOctetStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableOctetStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableCharStringAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableCharStringAttributeCallback, void (*)(CHIPUnitTestingNullableCharStringAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableCharStringAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableCharStringAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableCharString::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableCharStringAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableCharStringAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableEnumAttrAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableEnumAttrAttributeCallback, void (*)(CHIPUnitTestingNullableEnumAttrAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableEnumAttrAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableEnumAttrAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableEnumAttr::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableEnumAttrAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableEnumAttrAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableRangeRestrictedInt8uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableRangeRestrictedInt8uAttributeCallback, void (*)(CHIPUnitTestingNullableRangeRestrictedInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableRangeRestrictedInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableRangeRestrictedInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableRangeRestrictedInt8u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableRangeRestrictedInt8uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableRangeRestrictedInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableRangeRestrictedInt8sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableRangeRestrictedInt8sAttributeCallback, void (*)(CHIPUnitTestingNullableRangeRestrictedInt8sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableRangeRestrictedInt8sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableRangeRestrictedInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableRangeRestrictedInt8s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableRangeRestrictedInt8sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableRangeRestrictedInt8sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableRangeRestrictedInt16uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableRangeRestrictedInt16uAttributeCallback, void (*)(CHIPUnitTestingNullableRangeRestrictedInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableRangeRestrictedInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableRangeRestrictedInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableRangeRestrictedInt16u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableRangeRestrictedInt16uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableRangeRestrictedInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeNullableRangeRestrictedInt16sAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingNullableRangeRestrictedInt16sAttributeCallback, void (*)(CHIPUnitTestingNullableRangeRestrictedInt16sAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingNullableRangeRestrictedInt16sAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingNullableRangeRestrictedInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::NullableRangeRestrictedInt16s::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterNullableRangeRestrictedInt16sAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingNullableRangeRestrictedInt16sAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeWriteOnlyInt8uAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::WriteOnlyInt8u::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterWriteOnlyInt8uAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingGeneratedCommandListAttributeCallback, void (*)(CHIPUnitTestingGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingAcceptedCommandListAttributeCallback, void (*)(CHIPUnitTestingAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingEventListAttributeCallback, void (*)(CHIPUnitTestingEventListAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPUnitTestingAttributeListAttributeCallback, void (*)(CHIPUnitTestingAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPUnitTestingAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPUnitTestingAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPUnitTestingAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, UnitTestingCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::UnitTesting::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPUnitTestingClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
