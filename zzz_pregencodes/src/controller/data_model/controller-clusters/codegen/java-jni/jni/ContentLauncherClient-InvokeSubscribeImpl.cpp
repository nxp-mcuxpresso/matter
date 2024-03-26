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

JNI_METHOD(jlong, ContentLauncherCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, ContentLauncherCluster, 
  launchContent)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject search,jobject autoPlay,jobject data,jobject playbackPreferences,jobject useCurrentContext,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::ContentLauncher::Commands::LaunchContent::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;
          jobject search_parameterListItem_0;
          chip::JniReferences::GetInstance().GetObjectField(search, "parameterList", "Ljava/util/ArrayList;", search_parameterListItem_0);
          {
    using ListType_1 = std::remove_reference_t<decltype(request.search.parameterList)>;
    using ListMemberType_1 = ListMemberTypeGetter<ListType_1>::Type;
    jint search_parameterListItem_0Size;
    chip::JniReferences::GetInstance().GetListSize(search_parameterListItem_0, search_parameterListItem_0Size);
    if (search_parameterListItem_0Size != 0) {
      auto * listHolder_1 = new ListHolder<ListMemberType_1>(search_parameterListItem_0Size);
      listFreer.add(listHolder_1);

      for (jint i_1 = 0; i_1 < search_parameterListItem_0Size; ++i_1) {
        jobject element_1;
        chip::JniReferences::GetInstance().GetListItem(search_parameterListItem_0, i_1, element_1);
        
          jobject element_1_typeItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "type", "Ljava/lang/Integer;", element_1_typeItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].type = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].type)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1_typeItem_2));
          jobject element_1_valueItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "value", "Ljava/lang/String;", element_1_valueItem_2);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_1_valueItem_2)));
        listHolder_1->mList[static_cast<uint32_t>(i_1)].value = cleanupStrings.back()->charSpan();
          jobject element_1_externalIDListItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "externalIDList", "Ljava/util/Optional;", element_1_externalIDListItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_externalIDListItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].externalIDList.Emplace();
      {
    using ListType_4 = std::remove_reference_t<decltype(definedValue_3)>;
    using ListMemberType_4 = ListMemberTypeGetter<ListType_4>::Type;
    jint optionalValue_3Size;
    chip::JniReferences::GetInstance().GetListSize(optionalValue_3, optionalValue_3Size);
    if (optionalValue_3Size != 0) {
      auto * listHolder_4 = new ListHolder<ListMemberType_4>(optionalValue_3Size);
      listFreer.add(listHolder_4);

      for (jint i_4 = 0; i_4 < optionalValue_3Size; ++i_4) {
        jobject element_4;
        chip::JniReferences::GetInstance().GetListItem(optionalValue_3, i_4, element_4);
        
          jobject element_4_nameItem_5;
          chip::JniReferences::GetInstance().GetObjectField(element_4, "name", "Ljava/lang/String;", element_4_nameItem_5);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_4_nameItem_5)));
        listHolder_4->mList[static_cast<uint32_t>(i_4)].name = cleanupStrings.back()->charSpan();
          jobject element_4_valueItem_5;
          chip::JniReferences::GetInstance().GetObjectField(element_4, "value", "Ljava/lang/String;", element_4_valueItem_5);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_4_valueItem_5)));
        listHolder_4->mList[static_cast<uint32_t>(i_4)].value = cleanupStrings.back()->charSpan();
      }
      definedValue_3 = ListType_4(listHolder_4->mList, optionalValue_3Size);
    } else {
      definedValue_3 = ListType_4();
    }
  }
    }
  }
      }
      request.search.parameterList = ListType_1(listHolder_1->mList, search_parameterListItem_0Size);
    } else {
      request.search.parameterList = ListType_1();
    }
  }request.autoPlay = static_cast<std::remove_reference_t<decltype(request.autoPlay)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(autoPlay));
    {
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(data, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.data.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_0)));
        definedValue_0 = cleanupStrings.back()->charSpan();
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(playbackPreferences, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.playbackPreferences.Emplace();
      
          jobject optionalValue_0_playbackPositionItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "playbackPosition", "Ljava/lang/Long;", optionalValue_0_playbackPositionItem_1);
          definedValue_0.playbackPosition = static_cast<std::remove_reference_t<decltype(definedValue_0.playbackPosition)>>(chip::JniReferences::GetInstance().LongToPrimitive(optionalValue_0_playbackPositionItem_1));
    
          jobject optionalValue_0_textTrackItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "textTrack", "Lchip/devicecontroller/ChipStructs$ContentLauncherClusterTrackPreferenceStruct;", optionalValue_0_textTrackItem_1);
          
          jobject optionalValue_0_textTrackItem_1_languageCodeItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0_textTrackItem_1, "languageCode", "Ljava/lang/String;", optionalValue_0_textTrackItem_1_languageCodeItem_2);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_0_textTrackItem_1_languageCodeItem_2)));
        definedValue_0.textTrack.languageCode = cleanupStrings.back()->charSpan();
          jobject optionalValue_0_textTrackItem_1_characteristicsItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0_textTrackItem_1, "characteristics", "Ljava/util/Optional;", optionalValue_0_textTrackItem_1_characteristicsItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_textTrackItem_1_characteristicsItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = definedValue_0.textTrack.characteristics.Emplace();
      {
    using ListType_4 = std::remove_reference_t<decltype(definedValue_3)>;
    using ListMemberType_4 = ListMemberTypeGetter<ListType_4>::Type;
    jint optionalValue_3Size;
    chip::JniReferences::GetInstance().GetListSize(optionalValue_3, optionalValue_3Size);
    if (optionalValue_3Size != 0) {
      auto * listHolder_4 = new ListHolder<ListMemberType_4>(optionalValue_3Size);
      listFreer.add(listHolder_4);

      for (jint i_4 = 0; i_4 < optionalValue_3Size; ++i_4) {
        jobject element_4;
        chip::JniReferences::GetInstance().GetListItem(optionalValue_3, i_4, element_4);
        listHolder_4->mList[static_cast<uint32_t>(i_4)] = static_cast<std::remove_reference_t<decltype(listHolder_4->mList[static_cast<uint32_t>(i_4)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_4));
      }
      definedValue_3 = ListType_4(listHolder_4->mList, optionalValue_3Size);
    } else {
      definedValue_3 = ListType_4();
    }
  }
    }
  }
          jobject optionalValue_0_textTrackItem_1_audioOutputIndexItem_2;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0_textTrackItem_1, "audioOutputIndex", "Ljava/lang/Integer;", optionalValue_0_textTrackItem_1_audioOutputIndexItem_2);
          definedValue_0.textTrack.audioOutputIndex = static_cast<std::remove_reference_t<decltype(definedValue_0.textTrack.audioOutputIndex)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_0_textTrackItem_1_audioOutputIndexItem_2));
    
          jobject optionalValue_0_audioTracksItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "audioTracks", "Ljava/util/Optional;", optionalValue_0_audioTracksItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_audioTracksItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.audioTracks.Emplace();
      {
    using ListType_3 = std::remove_reference_t<decltype(definedValue_2)>;
    using ListMemberType_3 = ListMemberTypeGetter<ListType_3>::Type;
    jint optionalValue_2Size;
    chip::JniReferences::GetInstance().GetListSize(optionalValue_2, optionalValue_2Size);
    if (optionalValue_2Size != 0) {
      auto * listHolder_3 = new ListHolder<ListMemberType_3>(optionalValue_2Size);
      listFreer.add(listHolder_3);

      for (jint i_3 = 0; i_3 < optionalValue_2Size; ++i_3) {
        jobject element_3;
        chip::JniReferences::GetInstance().GetListItem(optionalValue_2, i_3, element_3);
        
          jobject element_3_languageCodeItem_4;
          chip::JniReferences::GetInstance().GetObjectField(element_3, "languageCode", "Ljava/lang/String;", element_3_languageCodeItem_4);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(element_3_languageCodeItem_4)));
        listHolder_3->mList[static_cast<uint32_t>(i_3)].languageCode = cleanupStrings.back()->charSpan();
          jobject element_3_characteristicsItem_4;
          chip::JniReferences::GetInstance().GetObjectField(element_3, "characteristics", "Ljava/util/Optional;", element_3_characteristicsItem_4);
          {
    jobject optionalValue_5 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_3_characteristicsItem_4, optionalValue_5);
    if (optionalValue_5) {
      auto & definedValue_5 = listHolder_3->mList[static_cast<uint32_t>(i_3)].characteristics.Emplace();
      {
    using ListType_6 = std::remove_reference_t<decltype(definedValue_5)>;
    using ListMemberType_6 = ListMemberTypeGetter<ListType_6>::Type;
    jint optionalValue_5Size;
    chip::JniReferences::GetInstance().GetListSize(optionalValue_5, optionalValue_5Size);
    if (optionalValue_5Size != 0) {
      auto * listHolder_6 = new ListHolder<ListMemberType_6>(optionalValue_5Size);
      listFreer.add(listHolder_6);

      for (jint i_6 = 0; i_6 < optionalValue_5Size; ++i_6) {
        jobject element_6;
        chip::JniReferences::GetInstance().GetListItem(optionalValue_5, i_6, element_6);
        listHolder_6->mList[static_cast<uint32_t>(i_6)] = static_cast<std::remove_reference_t<decltype(listHolder_6->mList[static_cast<uint32_t>(i_6)])>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_6));
      }
      definedValue_5 = ListType_6(listHolder_6->mList, optionalValue_5Size);
    } else {
      definedValue_5 = ListType_6();
    }
  }
    }
  }
          jobject element_3_audioOutputIndexItem_4;
          chip::JniReferences::GetInstance().GetObjectField(element_3, "audioOutputIndex", "Ljava/lang/Integer;", element_3_audioOutputIndexItem_4);
          listHolder_3->mList[static_cast<uint32_t>(i_3)].audioOutputIndex = static_cast<std::remove_reference_t<decltype(listHolder_3->mList[static_cast<uint32_t>(i_3)].audioOutputIndex)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_3_audioOutputIndexItem_4));
    
      }
      definedValue_2 = ListType_3(listHolder_3->mList, optionalValue_2Size);
    } else {
      definedValue_2 = ListType_3();
    }
  }
    }
  }
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(useCurrentContext, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.useCurrentContext.Emplace();
      definedValue_0 = static_cast<std::remove_reference_t<decltype(definedValue_0)>>(chip::JniReferences::GetInstance().BooleanToPrimitive(optionalValue_0));
    
    }
  }

  
    std::unique_ptr<CHIPContentLauncherClusterLauncherResponseCallback, void (*)(CHIPContentLauncherClusterLauncherResponseCallback *)> onSuccess(
        Platform::New<CHIPContentLauncherClusterLauncherResponseCallback>(callback), Platform::Delete<CHIPContentLauncherClusterLauncherResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterLauncherResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, ContentLauncherCluster, 
  launchURL)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jstring contentURL,jobject displayString,jobject brandingInformation,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::ContentLauncher::Commands::LaunchURL::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(contentURL)));
        request.contentURL = cleanupStrings.back()->charSpan();{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(displayString, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.displayString.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_0)));
        definedValue_0 = cleanupStrings.back()->charSpan();
    }
  }{
    jobject optionalValue_0 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(brandingInformation, optionalValue_0);
    if (optionalValue_0) {
      auto & definedValue_0 = request.brandingInformation.Emplace();
      
          jobject optionalValue_0_providerNameItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "providerName", "Ljava/lang/String;", optionalValue_0_providerNameItem_1);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_0_providerNameItem_1)));
        definedValue_0.providerName = cleanupStrings.back()->charSpan();
          jobject optionalValue_0_backgroundItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "background", "Ljava/util/Optional;", optionalValue_0_backgroundItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_backgroundItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.background.Emplace();
      
          jobject optionalValue_2_imageURLItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "imageURL", "Ljava/util/Optional;", optionalValue_2_imageURLItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_imageURLItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.imageURL.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_colorItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "color", "Ljava/util/Optional;", optionalValue_2_colorItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_colorItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.color.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_sizeItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "size", "Ljava/util/Optional;", optionalValue_2_sizeItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_sizeItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.size.Emplace();
      
          jobject optionalValue_4_widthItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "width", "Ljava/lang/Double;", optionalValue_4_widthItem_5);
          definedValue_4.width = static_cast<std::remove_reference_t<decltype(definedValue_4.width)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_widthItem_5));
    
          jobject optionalValue_4_heightItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "height", "Ljava/lang/Double;", optionalValue_4_heightItem_5);
          definedValue_4.height = static_cast<std::remove_reference_t<decltype(definedValue_4.height)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_heightItem_5));
    
          jobject optionalValue_4_metricItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "metric", "Ljava/lang/Integer;", optionalValue_4_metricItem_5);
          definedValue_4.metric = static_cast<std::remove_reference_t<decltype(definedValue_4.metric)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_4_metricItem_5));
    }
  }
    }
  }
          jobject optionalValue_0_logoItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "logo", "Ljava/util/Optional;", optionalValue_0_logoItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_logoItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.logo.Emplace();
      
          jobject optionalValue_2_imageURLItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "imageURL", "Ljava/util/Optional;", optionalValue_2_imageURLItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_imageURLItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.imageURL.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_colorItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "color", "Ljava/util/Optional;", optionalValue_2_colorItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_colorItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.color.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_sizeItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "size", "Ljava/util/Optional;", optionalValue_2_sizeItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_sizeItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.size.Emplace();
      
          jobject optionalValue_4_widthItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "width", "Ljava/lang/Double;", optionalValue_4_widthItem_5);
          definedValue_4.width = static_cast<std::remove_reference_t<decltype(definedValue_4.width)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_widthItem_5));
    
          jobject optionalValue_4_heightItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "height", "Ljava/lang/Double;", optionalValue_4_heightItem_5);
          definedValue_4.height = static_cast<std::remove_reference_t<decltype(definedValue_4.height)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_heightItem_5));
    
          jobject optionalValue_4_metricItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "metric", "Ljava/lang/Integer;", optionalValue_4_metricItem_5);
          definedValue_4.metric = static_cast<std::remove_reference_t<decltype(definedValue_4.metric)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_4_metricItem_5));
    }
  }
    }
  }
          jobject optionalValue_0_progressBarItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "progressBar", "Ljava/util/Optional;", optionalValue_0_progressBarItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_progressBarItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.progressBar.Emplace();
      
          jobject optionalValue_2_imageURLItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "imageURL", "Ljava/util/Optional;", optionalValue_2_imageURLItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_imageURLItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.imageURL.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_colorItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "color", "Ljava/util/Optional;", optionalValue_2_colorItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_colorItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.color.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_sizeItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "size", "Ljava/util/Optional;", optionalValue_2_sizeItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_sizeItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.size.Emplace();
      
          jobject optionalValue_4_widthItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "width", "Ljava/lang/Double;", optionalValue_4_widthItem_5);
          definedValue_4.width = static_cast<std::remove_reference_t<decltype(definedValue_4.width)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_widthItem_5));
    
          jobject optionalValue_4_heightItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "height", "Ljava/lang/Double;", optionalValue_4_heightItem_5);
          definedValue_4.height = static_cast<std::remove_reference_t<decltype(definedValue_4.height)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_heightItem_5));
    
          jobject optionalValue_4_metricItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "metric", "Ljava/lang/Integer;", optionalValue_4_metricItem_5);
          definedValue_4.metric = static_cast<std::remove_reference_t<decltype(definedValue_4.metric)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_4_metricItem_5));
    }
  }
    }
  }
          jobject optionalValue_0_splashItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "splash", "Ljava/util/Optional;", optionalValue_0_splashItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_splashItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.splash.Emplace();
      
          jobject optionalValue_2_imageURLItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "imageURL", "Ljava/util/Optional;", optionalValue_2_imageURLItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_imageURLItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.imageURL.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_colorItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "color", "Ljava/util/Optional;", optionalValue_2_colorItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_colorItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.color.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_sizeItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "size", "Ljava/util/Optional;", optionalValue_2_sizeItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_sizeItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.size.Emplace();
      
          jobject optionalValue_4_widthItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "width", "Ljava/lang/Double;", optionalValue_4_widthItem_5);
          definedValue_4.width = static_cast<std::remove_reference_t<decltype(definedValue_4.width)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_widthItem_5));
    
          jobject optionalValue_4_heightItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "height", "Ljava/lang/Double;", optionalValue_4_heightItem_5);
          definedValue_4.height = static_cast<std::remove_reference_t<decltype(definedValue_4.height)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_heightItem_5));
    
          jobject optionalValue_4_metricItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "metric", "Ljava/lang/Integer;", optionalValue_4_metricItem_5);
          definedValue_4.metric = static_cast<std::remove_reference_t<decltype(definedValue_4.metric)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_4_metricItem_5));
    }
  }
    }
  }
          jobject optionalValue_0_waterMarkItem_1;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_0, "waterMark", "Ljava/util/Optional;", optionalValue_0_waterMarkItem_1);
          {
    jobject optionalValue_2 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_0_waterMarkItem_1, optionalValue_2);
    if (optionalValue_2) {
      auto & definedValue_2 = definedValue_0.waterMark.Emplace();
      
          jobject optionalValue_2_imageURLItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "imageURL", "Ljava/util/Optional;", optionalValue_2_imageURLItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_imageURLItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.imageURL.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_colorItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "color", "Ljava/util/Optional;", optionalValue_2_colorItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_colorItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.color.Emplace();
      cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(optionalValue_4)));
        definedValue_4 = cleanupStrings.back()->charSpan();
    }
  }
          jobject optionalValue_2_sizeItem_3;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_2, "size", "Ljava/util/Optional;", optionalValue_2_sizeItem_3);
          {
    jobject optionalValue_4 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_2_sizeItem_3, optionalValue_4);
    if (optionalValue_4) {
      auto & definedValue_4 = definedValue_2.size.Emplace();
      
          jobject optionalValue_4_widthItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "width", "Ljava/lang/Double;", optionalValue_4_widthItem_5);
          definedValue_4.width = static_cast<std::remove_reference_t<decltype(definedValue_4.width)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_widthItem_5));
    
          jobject optionalValue_4_heightItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "height", "Ljava/lang/Double;", optionalValue_4_heightItem_5);
          definedValue_4.height = static_cast<std::remove_reference_t<decltype(definedValue_4.height)>>(chip::JniReferences::GetInstance().DoubleToPrimitive(optionalValue_4_heightItem_5));
    
          jobject optionalValue_4_metricItem_5;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_4, "metric", "Ljava/lang/Integer;", optionalValue_4_metricItem_5);
          definedValue_4.metric = static_cast<std::remove_reference_t<decltype(definedValue_4.metric)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_4_metricItem_5));
    }
  }
    }
  }
    }
  }

  
    std::unique_ptr<CHIPContentLauncherClusterLauncherResponseCallback, void (*)(CHIPContentLauncherClusterLauncherResponseCallback *)> onSuccess(
        Platform::New<CHIPContentLauncherClusterLauncherResponseCallback>(callback), Platform::Delete<CHIPContentLauncherClusterLauncherResponseCallback>);
    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));
    VerifyOrReturn(onFailure.get() != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native callback", CHIP_ERROR_NO_MEMORY));

    cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error getting native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterLauncherResponseCallbackType>::FromCancelable(onSuccess->Cancel());
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
JNI_METHOD(void, ContentLauncherCluster, subscribeAcceptHeaderAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPContentLauncherAcceptHeaderAttributeCallback, void (*)(CHIPContentLauncherAcceptHeaderAttributeCallback *)> onSuccess(Platform::New<CHIPContentLauncherAcceptHeaderAttributeCallback>(callback, true), chip::Platform::Delete<CHIPContentLauncherAcceptHeaderAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::ContentLauncher::Attributes::AcceptHeader::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterAcceptHeaderAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPContentLauncherAcceptHeaderAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ContentLauncherCluster, subscribeSupportedStreamingProtocolsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::ContentLauncher::Attributes::SupportedStreamingProtocols::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterSupportedStreamingProtocolsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ContentLauncherCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPContentLauncherGeneratedCommandListAttributeCallback, void (*)(CHIPContentLauncherGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPContentLauncherGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPContentLauncherGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::ContentLauncher::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPContentLauncherGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ContentLauncherCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPContentLauncherAcceptedCommandListAttributeCallback, void (*)(CHIPContentLauncherAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPContentLauncherAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPContentLauncherAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::ContentLauncher::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPContentLauncherAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ContentLauncherCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPContentLauncherEventListAttributeCallback, void (*)(CHIPContentLauncherEventListAttributeCallback *)> onSuccess(Platform::New<CHIPContentLauncherEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPContentLauncherEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::ContentLauncher::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPContentLauncherEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ContentLauncherCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPContentLauncherAttributeListAttributeCallback, void (*)(CHIPContentLauncherAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPContentLauncherAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPContentLauncherAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::ContentLauncher::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPContentLauncherAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ContentLauncherCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::ContentLauncher::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, ContentLauncherCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::ContentLauncher::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPContentLauncherClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
