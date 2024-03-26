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

JNI_METHOD(jlong, ChannelCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, ChannelCluster, 
  changeChannel)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jstring match,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::Channel::Commands::ChangeChannel::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(match)));
        request.match = cleanupStrings.back()->charSpan();

  
    std::unique_ptr<CHIPChannelClusterChangeChannelResponseCallback, void (*)(CHIPChannelClusterChangeChannelResponseCallback *)> onSuccess(
        Platform::New<CHIPChannelClusterChangeChannelResponseCallback>(callback), Platform::Delete<CHIPChannelClusterChangeChannelResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPChannelClusterChangeChannelResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, ChannelCluster, 
  changeChannelByNumber)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject majorNumber,jobject minorNumber,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::Channel::Commands::ChangeChannelByNumber::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.majorNumber = static_cast<std::remove_reference_t<decltype(request.majorNumber)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(majorNumber));
    request.minorNumber = static_cast<std::remove_reference_t<decltype(request.minorNumber)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(minorNumber));
    

  
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
JNI_METHOD(void, ChannelCluster, 
  skipChannel)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject count,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::Channel::Commands::SkipChannel::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;request.count = static_cast<std::remove_reference_t<decltype(request.count)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(count));
    

  
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
JNI_METHOD(void, ChannelCluster, 
  getProgramGuide)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject startTime,jobject endTime,jobject channelList,jobject pageToken,jobject recordingFlag,jobject externalIDList,jobject data,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::Channel::Commands::GetProgramGuide::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(startTime, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.startTime.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0));
    
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(endTime, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.endTime.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0));
    
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(channelList, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.channelList.Emplace();
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
        
          jobject element_1_majorNumberItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "majorNumber", "Ljava/lang/Integer;", element_1_majorNumberItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].majorNumber = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].majorNumber)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1_majorNumberItem_2));
    
          jobject element_1_minorNumberItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "minorNumber", "Ljava/lang/Integer;", element_1_minorNumberItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].minorNumber = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].minorNumber)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1_minorNumberItem_2));
    
          jobject element_1_nameItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "name", "Ljava/util/Optional;", element_1_nameItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_nameItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].name.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_3)));
        definedValue_3 = cleanupStrings.back()->charSpan();
    }
  }
          jobject element_1_callSignItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "callSign", "Ljava/util/Optional;", element_1_callSignItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_callSignItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].callSign.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_3)));
        definedValue_3 = cleanupStrings.back()->charSpan();
    }
  }
          jobject element_1_affiliateCallSignItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "affiliateCallSign", "Ljava/util/Optional;", element_1_affiliateCallSignItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_affiliateCallSignItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].affiliateCallSign.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_3)));
        definedValue_3 = cleanupStrings.back()->charSpan();
    }
  }
          jobject element_1_identifierItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "identifier", "Ljava/util/Optional;", element_1_identifierItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_identifierItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].identifier.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_3)));
        definedValue_3 = cleanupStrings.back()->charSpan();
    }
  }
          jobject element_1_typeItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "type", "Ljava/util/Optional;", element_1_typeItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_typeItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].type.Emplace();
      definedValue_3 = static_cast<std::remove_reference_t<decltype(definedValue_3)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_3));
    }
  }
      }
      definedValue_0 = ListType_1(listHolder_1->mList, optionalValue_0Size);
    } else {
      definedValue_0 = ListType_1();
    }
  }
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(pageToken, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.pageToken.Emplace();
      
          jobject optionalValue_0_limitItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "limit", "Ljava/util/Optional;", optionalValue_0_limitItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_limitItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.limit.Emplace();
      definedValue_2 = static_cast<std::remove_reference_t<decltype(definedValue_2)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_2));
    
    }
  }
          jobject optionalValue_0_afterItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "after", "Ljava/util/Optional;", optionalValue_0_afterItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_afterItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.after.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_2)));
        definedValue_2 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_0_beforeItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "before", "Ljava/util/Optional;", optionalValue_0_beforeItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_beforeItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.before.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_2)));
        definedValue_2 = cleanupStrings.back()->charSpan();
    }
  }
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(recordingFlag, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.recordingFlag.Emplace();
      definedValue_0.SetRaw(static_cast<std::remove_reference_t<decltype(definedValue_0)>::IntegerType>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0)));
    
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(externalIDList, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.externalIDList.Emplace();
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
        
          jobject element_1_nameItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "name", "Ljava/lang/String;", element_1_nameItem_2);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_1_nameItem_2)));
        listHolder_1->mList[static_cast<uint32_t>(i_1)].name = cleanupStrings.back()->charSpan();
          jobject element_1_valueItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "value", "Ljava/lang/String;", element_1_valueItem_2);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_1_valueItem_2)));
        listHolder_1->mList[static_cast<uint32_t>(i_1)].value = cleanupStrings.back()->charSpan();
      }
      definedValue_0 = ListType_1(listHolder_1->mList, optionalValue_0Size);
    } else {
      definedValue_0 = ListType_1();
    }
  }
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(data, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.data.Emplace();
      cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(optionalValue_0)));
        definedValue_0 = cleanupByteArrays.back()->byteSpan();
    }
  }

  
    std::unique_ptr<CHIPChannelClusterProgramGuideResponseCallback, void (*)(CHIPChannelClusterProgramGuideResponseCallback *)> onSuccess(
        Platform::New<CHIPChannelClusterProgramGuideResponseCallback>(callback), Platform::Delete<CHIPChannelClusterProgramGuideResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPChannelClusterProgramGuideResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, ChannelCluster, 
  recordProgram)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jstring programIdentifier,jobject shouldRecordSeries,jobject externalIDList,jbyteArray data,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::Channel::Commands::RecordProgram::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(programIdentifier)));
        request.programIdentifier = cleanupStrings.back()->charSpan();request.shouldRecordSeries = static_cast<std::remove_reference_t<decltype(request.shouldRecordSeries)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(shouldRecordSeries));
    {
    using ListType_0 = std::remove_reference_t<decltype(request.externalIDList)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint externalIDListSize;
    chip::JniReferences::GetInstance().GetListSize(externalIDList, externalIDListSize);
    if (externalIDListSize != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(externalIDListSize);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < externalIDListSize; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(externalIDList, i_0, element_0);
        
          jobject element_0_nameItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "name", "Ljava/lang/String;", element_0_nameItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_0_nameItem_1)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].name = cleanupStrings.back()->charSpan();
          jobject element_0_valueItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "value", "Ljava/lang/String;", element_0_valueItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_0_valueItem_1)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].value = cleanupStrings.back()->charSpan();
      }
      request.externalIDList = ListType_0(listHolder_0->mList, externalIDListSize);
    } else {
      request.externalIDList = ListType_0();
    }
  }cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(data)));
        request.data = cleanupByteArrays.back()->byteSpan();

  
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
JNI_METHOD(void, ChannelCluster, 
  cancelRecordProgram)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jstring programIdentifier,jobject shouldRecordSeries,jobject externalIDList,jbyteArray data,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::Channel::Commands::CancelRecordProgram::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(programIdentifier)));
        request.programIdentifier = cleanupStrings.back()->charSpan();request.shouldRecordSeries = static_cast<std::remove_reference_t<decltype(request.shouldRecordSeries)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(shouldRecordSeries));
    {
    using ListType_0 = std::remove_reference_t<decltype(request.externalIDList)>;
    using ListMemberType_0 = ListMemberTypeGetter<ListType_0>::Type;
    jint externalIDListSize;
    chip::JniReferences::GetInstance().GetListSize(externalIDList, externalIDListSize);
    if (externalIDListSize != 0) {
      auto * listHolder_0 = new ListHolder<ListMemberType_0>(externalIDListSize);
      listFreer.add(listHolder_0);

      for (jint i_0 = 0; i_0 < externalIDListSize; ++i_0) {
        jobject element_0;
        chip::JniReferences::GetInstance().GetListItem(externalIDList, i_0, element_0);
        
          jobject element_0_nameItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "name", "Ljava/lang/String;", element_0_nameItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_0_nameItem_1)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].name = cleanupStrings.back()->charSpan();
          jobject element_0_valueItem_1;
          chip::JniReferences::GetInstance().GetObjectField(element_0, "value", "Ljava/lang/String;", element_0_valueItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_0_valueItem_1)));
        listHolder_0->mList[static_cast<uint32_t>(i_0)].value = cleanupStrings.back()->charSpan();
      }
      request.externalIDList = ListType_0(listHolder_0->mList, externalIDListSize);
    } else {
      request.externalIDList = ListType_0();
    }
  }cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(data)));
        request.data = cleanupByteArrays.back()->byteSpan();

  
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
JNI_METHOD(void, ChannelCluster, subscribeChannelListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPChannelChannelListAttributeCallback, void (*)(CHIPChannelChannelListAttributeCallback *)> onSuccess(Platform::New<CHIPChannelChannelListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPChannelChannelListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Channel::Attributes::ChannelList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPChannelClusterChannelListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPChannelChannelListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ChannelCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPChannelGeneratedCommandListAttributeCallback, void (*)(CHIPChannelGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPChannelGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPChannelGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Channel::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPChannelClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPChannelGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ChannelCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPChannelAcceptedCommandListAttributeCallback, void (*)(CHIPChannelAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPChannelAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPChannelAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Channel::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPChannelClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPChannelAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ChannelCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPChannelEventListAttributeCallback, void (*)(CHIPChannelEventListAttributeCallback *)> onSuccess(Platform::New<CHIPChannelEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPChannelEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Channel::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPChannelClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPChannelEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ChannelCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPChannelAttributeListAttributeCallback, void (*)(CHIPChannelAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPChannelAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPChannelAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Channel::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPChannelClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPChannelAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ChannelCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Channel::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPChannelClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ChannelCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::Channel::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPChannelClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
