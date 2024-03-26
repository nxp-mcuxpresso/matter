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

JNI_METHOD(jlong, DemandResponseLoadControlCluster, initWithDevice)(JNIEnv * env, jobject self, jlong devicePtr, jint endpointId)
{
    chip::DeviceLayer::StackLock lock;
    DeviceProxy * device = reinterpret_cast<DeviceProxy *>(devicePtr);
    ClusterBase * cppCluster = new ClusterBase(*device->GetExchangeManager(), device->GetSecureSession().Value(), endpointId);
    return reinterpret_cast<jlong>(cppCluster);
}

JNI_METHOD(void, DemandResponseLoadControlCluster, 
  registerLoadControlProgramRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject loadControlProgram,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::DemandResponseLoadControl::Commands::RegisterLoadControlProgramRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;
          jobject loadControlProgram_programIDItem_0;
          chip::JniReferences::GetInstance().GetObjectField(loadControlProgram, "programID", "[B", loadControlProgram_programIDItem_0);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(loadControlProgram_programIDItem_0)));
        request.loadControlProgram.programID = cleanupByteArrays.back()->byteSpan();
          jobject loadControlProgram_nameItem_0;
          chip::JniReferences::GetInstance().GetObjectField(loadControlProgram, "name", "Ljava/lang/String;", loadControlProgram_nameItem_0);
          cleanupStrings.push_back(chip::Platform::MakeUnique<chip::JniUtfString>(env, static_cast<jstring>(loadControlProgram_nameItem_0)));
        request.loadControlProgram.name = cleanupStrings.back()->charSpan();
          jobject loadControlProgram_enrollmentGroupItem_0;
          chip::JniReferences::GetInstance().GetObjectField(loadControlProgram, "enrollmentGroup", "Ljava/lang/Integer;", loadControlProgram_enrollmentGroupItem_0);
          if (loadControlProgram_enrollmentGroupItem_0 == nullptr) {
    request.loadControlProgram.enrollmentGroup.SetNull();
  } else {
    auto & nonNullValue_1 = request.loadControlProgram.enrollmentGroup.SetNonNull();
    nonNullValue_1 = static_cast<std::remove_reference_t<decltype(nonNullValue_1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(loadControlProgram_enrollmentGroupItem_0));
    
  }
          jobject loadControlProgram_randomStartMinutesItem_0;
          chip::JniReferences::GetInstance().GetObjectField(loadControlProgram, "randomStartMinutes", "Ljava/lang/Integer;", loadControlProgram_randomStartMinutesItem_0);
          if (loadControlProgram_randomStartMinutesItem_0 == nullptr) {
    request.loadControlProgram.randomStartMinutes.SetNull();
  } else {
    auto & nonNullValue_1 = request.loadControlProgram.randomStartMinutes.SetNonNull();
    nonNullValue_1 = static_cast<std::remove_reference_t<decltype(nonNullValue_1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(loadControlProgram_randomStartMinutesItem_0));
    
  }
          jobject loadControlProgram_randomDurationMinutesItem_0;
          chip::JniReferences::GetInstance().GetObjectField(loadControlProgram, "randomDurationMinutes", "Ljava/lang/Integer;", loadControlProgram_randomDurationMinutesItem_0);
          if (loadControlProgram_randomDurationMinutesItem_0 == nullptr) {
    request.loadControlProgram.randomDurationMinutes.SetNull();
  } else {
    auto & nonNullValue_1 = request.loadControlProgram.randomDurationMinutes.SetNonNull();
    nonNullValue_1 = static_cast<std::remove_reference_t<decltype(nonNullValue_1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(loadControlProgram_randomDurationMinutesItem_0));
    
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
JNI_METHOD(void, DemandResponseLoadControlCluster, 
  unregisterLoadControlProgramRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jbyteArray loadControlProgramID,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::DemandResponseLoadControl::Commands::UnregisterLoadControlProgramRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(loadControlProgramID)));
        request.loadControlProgramID = cleanupByteArrays.back()->byteSpan();

  
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
JNI_METHOD(void, DemandResponseLoadControlCluster, 
  addLoadControlEventRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject event,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::DemandResponseLoadControl::Commands::AddLoadControlEventRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;
          jobject event_eventIDItem_0;
          chip::JniReferences::GetInstance().GetObjectField(event, "eventID", "[B", event_eventIDItem_0);
          cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(event_eventIDItem_0)));
        request.event.eventID = cleanupByteArrays.back()->byteSpan();
          jobject event_programIDItem_0;
          chip::JniReferences::GetInstance().GetObjectField(event, "programID", "[B", event_programIDItem_0);
          if (event_programIDItem_0 == nullptr) {
    request.event.programID.SetNull();
  } else {
    auto & nonNullValue_1 = request.event.programID.SetNonNull();
    cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(event_programIDItem_0)));
        nonNullValue_1 = cleanupByteArrays.back()->byteSpan();
  }
          jobject event_controlItem_0;
          chip::JniReferences::GetInstance().GetObjectField(event, "control", "Ljava/lang/Integer;", event_controlItem_0);
          request.event.control.SetRaw(static_cast<std::remove_reference_t<decltype(request.event.control)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(event_controlItem_0)));
    
          jobject event_deviceClassItem_0;
          chip::JniReferences::GetInstance().GetObjectField(event, "deviceClass", "Ljava/lang/Long;", event_deviceClassItem_0);
          request.event.deviceClass.SetRaw(static_cast<std::remove_reference_t<decltype(request.event.deviceClass)>::IntegerType>(chip::JniReferences::GetInstance().LongToPrimitive(event_deviceClassItem_0)));
    
          jobject event_enrollmentGroupItem_0;
          chip::JniReferences::GetInstance().GetObjectField(event, "enrollmentGroup", "Ljava/util/Optional;", event_enrollmentGroupItem_0);
          {
    jobject optionalValue_1 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(event_enrollmentGroupItem_0, optionalValue_1);
    if (optionalValue_1) {
      auto & definedValue_1 = request.event.enrollmentGroup.Emplace();
      definedValue_1 = static_cast<std::remove_reference_t<decltype(definedValue_1)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_1));
    
    }
  }
          jobject event_criticalityItem_0;
          chip::JniReferences::GetInstance().GetObjectField(event, "criticality", "Ljava/lang/Integer;", event_criticalityItem_0);
          request.event.criticality = static_cast<std::remove_reference_t<decltype(request.event.criticality)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(event_criticalityItem_0));
          jobject event_startTimeItem_0;
          chip::JniReferences::GetInstance().GetObjectField(event, "startTime", "Ljava/lang/Long;", event_startTimeItem_0);
          if (event_startTimeItem_0 == nullptr) {
    request.event.startTime.SetNull();
  } else {
    auto & nonNullValue_1 = request.event.startTime.SetNonNull();
    nonNullValue_1 = static_cast<std::remove_reference_t<decltype(nonNullValue_1)>>(chip::JniReferences::GetInstance().LongToPrimitive(event_startTimeItem_0));
    
  }
          jobject event_transitionsItem_0;
          chip::JniReferences::GetInstance().GetObjectField(event, "transitions", "Ljava/util/ArrayList;", event_transitionsItem_0);
          {
    using ListType_1 = std::remove_reference_t<decltype(request.event.transitions)>;
    using ListMemberType_1 = ListMemberTypeGetter<ListType_1>::Type;
    jint event_transitionsItem_0Size;
    chip::JniReferences::GetInstance().GetListSize(event_transitionsItem_0, event_transitionsItem_0Size);
    if (event_transitionsItem_0Size != 0) {
      auto * listHolder_1 = new ListHolder<ListMemberType_1>(event_transitionsItem_0Size);
      listFreer.add(listHolder_1);

      for (jint i_1 = 0; i_1 < event_transitionsItem_0Size; ++i_1) {
        jobject element_1;
        chip::JniReferences::GetInstance().GetListItem(event_transitionsItem_0, i_1, element_1);
        
          jobject element_1_durationItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "duration", "Ljava/lang/Integer;", element_1_durationItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].duration = static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].duration)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1_durationItem_2));
    
          jobject element_1_controlItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "control", "Ljava/lang/Integer;", element_1_controlItem_2);
          listHolder_1->mList[static_cast<uint32_t>(i_1)].control.SetRaw(static_cast<std::remove_reference_t<decltype(listHolder_1->mList[static_cast<uint32_t>(i_1)].control)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(element_1_controlItem_2)));
    
          jobject element_1_temperatureControlItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "temperatureControl", "Ljava/util/Optional;", element_1_temperatureControlItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_temperatureControlItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].temperatureControl.Emplace();
      
          jobject optionalValue_3_coolingTempOffsetItem_4;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_3, "coolingTempOffset", "Ljava/util/Optional;", optionalValue_3_coolingTempOffsetItem_4);
          {
    jobject optionalValue_5 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_3_coolingTempOffsetItem_4, optionalValue_5);
    if (optionalValue_5) {
      auto & definedValue_5 = definedValue_3.coolingTempOffset.Emplace();
      if (optionalValue_5 == nullptr) {
    definedValue_5.SetNull();
  } else {
    auto & nonNullValue_6 = definedValue_5.SetNonNull();
    nonNullValue_6 = static_cast<std::remove_reference_t<decltype(nonNullValue_6)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_5));
    
  }
    }
  }
          jobject optionalValue_3_heatingtTempOffsetItem_4;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_3, "heatingtTempOffset", "Ljava/util/Optional;", optionalValue_3_heatingtTempOffsetItem_4);
          {
    jobject optionalValue_5 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_3_heatingtTempOffsetItem_4, optionalValue_5);
    if (optionalValue_5) {
      auto & definedValue_5 = definedValue_3.heatingtTempOffset.Emplace();
      if (optionalValue_5 == nullptr) {
    definedValue_5.SetNull();
  } else {
    auto & nonNullValue_6 = definedValue_5.SetNonNull();
    nonNullValue_6 = static_cast<std::remove_reference_t<decltype(nonNullValue_6)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_5));
    
  }
    }
  }
          jobject optionalValue_3_coolingTempSetpointItem_4;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_3, "coolingTempSetpoint", "Ljava/util/Optional;", optionalValue_3_coolingTempSetpointItem_4);
          {
    jobject optionalValue_5 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_3_coolingTempSetpointItem_4, optionalValue_5);
    if (optionalValue_5) {
      auto & definedValue_5 = definedValue_3.coolingTempSetpoint.Emplace();
      if (optionalValue_5 == nullptr) {
    definedValue_5.SetNull();
  } else {
    auto & nonNullValue_6 = definedValue_5.SetNonNull();
    nonNullValue_6 = static_cast<std::remove_reference_t<decltype(nonNullValue_6)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_5));
    
  }
    }
  }
          jobject optionalValue_3_heatingTempSetpointItem_4;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_3, "heatingTempSetpoint", "Ljava/util/Optional;", optionalValue_3_heatingTempSetpointItem_4);
          {
    jobject optionalValue_5 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(optionalValue_3_heatingTempSetpointItem_4, optionalValue_5);
    if (optionalValue_5) {
      auto & definedValue_5 = definedValue_3.heatingTempSetpoint.Emplace();
      if (optionalValue_5 == nullptr) {
    definedValue_5.SetNull();
  } else {
    auto & nonNullValue_6 = definedValue_5.SetNonNull();
    nonNullValue_6 = static_cast<std::remove_reference_t<decltype(nonNullValue_6)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_5));
    
  }
    }
  }
    }
  }
          jobject element_1_averageLoadControlItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "averageLoadControl", "Ljava/util/Optional;", element_1_averageLoadControlItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_averageLoadControlItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].averageLoadControl.Emplace();
      
          jobject optionalValue_3_loadAdjustmentItem_4;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_3, "loadAdjustment", "Ljava/lang/Integer;", optionalValue_3_loadAdjustmentItem_4);
          definedValue_3.loadAdjustment = static_cast<std::remove_reference_t<decltype(definedValue_3.loadAdjustment)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_3_loadAdjustmentItem_4));
    
    }
  }
          jobject element_1_dutyCycleControlItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "dutyCycleControl", "Ljava/util/Optional;", element_1_dutyCycleControlItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_dutyCycleControlItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].dutyCycleControl.Emplace();
      
          jobject optionalValue_3_dutyCycleItem_4;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_3, "dutyCycle", "Ljava/lang/Integer;", optionalValue_3_dutyCycleItem_4);
          definedValue_3.dutyCycle = static_cast<std::remove_reference_t<decltype(definedValue_3.dutyCycle)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_3_dutyCycleItem_4));
    
    }
  }
          jobject element_1_powerSavingsControlItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "powerSavingsControl", "Ljava/util/Optional;", element_1_powerSavingsControlItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_powerSavingsControlItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].powerSavingsControl.Emplace();
      
          jobject optionalValue_3_powerSavingsItem_4;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_3, "powerSavings", "Ljava/lang/Integer;", optionalValue_3_powerSavingsItem_4);
          definedValue_3.powerSavings = static_cast<std::remove_reference_t<decltype(definedValue_3.powerSavings)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_3_powerSavingsItem_4));
    
    }
  }
          jobject element_1_heatingSourceControlItem_2;
          chip::JniReferences::GetInstance().GetObjectField(element_1, "heatingSourceControl", "Ljava/util/Optional;", element_1_heatingSourceControlItem_2);
          {
    jobject optionalValue_3 = nullptr;
    chip::JniReferences::GetInstance().GetOptionalValue(element_1_heatingSourceControlItem_2, optionalValue_3);
    if (optionalValue_3) {
      auto & definedValue_3 = listHolder_1->mList[static_cast<uint32_t>(i_1)].heatingSourceControl.Emplace();
      
          jobject optionalValue_3_heatingSourceItem_4;
          chip::JniReferences::GetInstance().GetObjectField(optionalValue_3, "heatingSource", "Ljava/lang/Integer;", optionalValue_3_heatingSourceItem_4);
          definedValue_3.heatingSource = static_cast<std::remove_reference_t<decltype(definedValue_3.heatingSource)>>(chip::JniReferences::GetInstance().IntegerToPrimitive(optionalValue_3_heatingSourceItem_4));
    }
  }
      }
      request.event.transitions = ListType_1(listHolder_1->mList, event_transitionsItem_0Size);
    } else {
      request.event.transitions = ListType_1();
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
JNI_METHOD(void, DemandResponseLoadControlCluster, 
  removeLoadControlEventRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jbyteArray eventID,jobject cancelControl,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::DemandResponseLoadControl::Commands::RemoveLoadControlEventRequest::Type request;

    std::vector<Platform::UniquePtr<JniByteArray>> cleanupByteArrays;
    std::vector<Platform::UniquePtr<JniUtfString>> cleanupStrings;cleanupByteArrays.push_back(chip::Platform::MakeUnique<chip::JniByteArray>(env, static_cast<jbyteArray>(eventID)));
        request.eventID = cleanupByteArrays.back()->byteSpan();request.cancelControl.SetRaw(static_cast<std::remove_reference_t<decltype(request.cancelControl)>::IntegerType>(chip::JniReferences::GetInstance().IntegerToPrimitive(cancelControl)));
    

  
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
JNI_METHOD(void, DemandResponseLoadControlCluster, 
  clearLoadControlEventsRequest)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback,jobject timedInvokeTimeoutMs)
{
    chip::DeviceLayer::StackLock lock;
    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster;

    ListFreer listFreer;
    chip::app::Clusters::DemandResponseLoadControl::Commands::ClearLoadControlEventsRequest::Type request;

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
JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeLoadControlProgramsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDemandResponseLoadControlLoadControlProgramsAttributeCallback, void (*)(CHIPDemandResponseLoadControlLoadControlProgramsAttributeCallback *)> onSuccess(Platform::New<CHIPDemandResponseLoadControlLoadControlProgramsAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDemandResponseLoadControlLoadControlProgramsAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::LoadControlPrograms::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterLoadControlProgramsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDemandResponseLoadControlLoadControlProgramsAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeNumberOfLoadControlProgramsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::NumberOfLoadControlPrograms::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterNumberOfLoadControlProgramsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeEventsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDemandResponseLoadControlEventsAttributeCallback, void (*)(CHIPDemandResponseLoadControlEventsAttributeCallback *)> onSuccess(Platform::New<CHIPDemandResponseLoadControlEventsAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDemandResponseLoadControlEventsAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::Events::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterEventsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDemandResponseLoadControlEventsAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeActiveEventsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDemandResponseLoadControlActiveEventsAttributeCallback, void (*)(CHIPDemandResponseLoadControlActiveEventsAttributeCallback *)> onSuccess(Platform::New<CHIPDemandResponseLoadControlActiveEventsAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDemandResponseLoadControlActiveEventsAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::ActiveEvents::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterActiveEventsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDemandResponseLoadControlActiveEventsAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeNumberOfEventsPerProgramAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::NumberOfEventsPerProgram::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterNumberOfEventsPerProgramAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeNumberOfTransitionsAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::NumberOfTransitions::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterNumberOfTransitionsAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeDefaultRandomStartAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::DefaultRandomStart::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterDefaultRandomStartAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeDefaultRandomDurationAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(Platform::New<CHIPInt8uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::DefaultRandomDuration::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterDefaultRandomDurationAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt8uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDemandResponseLoadControlGeneratedCommandListAttributeCallback, void (*)(CHIPDemandResponseLoadControlGeneratedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPDemandResponseLoadControlGeneratedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDemandResponseLoadControlGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::GeneratedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDemandResponseLoadControlGeneratedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDemandResponseLoadControlAcceptedCommandListAttributeCallback, void (*)(CHIPDemandResponseLoadControlAcceptedCommandListAttributeCallback *)> onSuccess(Platform::New<CHIPDemandResponseLoadControlAcceptedCommandListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDemandResponseLoadControlAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::AcceptedCommandList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDemandResponseLoadControlAcceptedCommandListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDemandResponseLoadControlEventListAttributeCallback, void (*)(CHIPDemandResponseLoadControlEventListAttributeCallback *)> onSuccess(Platform::New<CHIPDemandResponseLoadControlEventListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDemandResponseLoadControlEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::EventList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDemandResponseLoadControlEventListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPDemandResponseLoadControlAttributeListAttributeCallback, void (*)(CHIPDemandResponseLoadControlAttributeListAttributeCallback *)> onSuccess(Platform::New<CHIPDemandResponseLoadControlAttributeListAttributeCallback>(callback, true), chip::Platform::Delete<CHIPDemandResponseLoadControlAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::AttributeList::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPDemandResponseLoadControlAttributeListAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(Platform::New<CHIPInt32uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::FeatureMap::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt32uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}JNI_METHOD(void, DemandResponseLoadControlCluster, subscribeClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback, jint minInterval, jint maxInterval)
{
    chip::DeviceLayer::StackLock lock;std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(Platform::New<CHIPInt16uAttributeCallback>(callback, true), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<CHIPDefaultFailureCallback, void (*)(CHIPDefaultFailureCallback *)> onFailure(Platform::New<CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    ClusterBase * cppCluster = reinterpret_cast<ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    using TypeInfo = chip::app::Clusters::DemandResponseLoadControl::Attributes::ClusterRevision::TypeInfo;
    auto successFn = chip::Callback::Callback<CHIPDemandResponseLoadControlClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());

    err = cppCluster->SubscribeAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall, static_cast<uint16_t>(minInterval), static_cast<uint16_t>(maxInterval), CHIPInt16uAttributeCallback::OnSubscriptionEstablished);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error subscribing to attribute", err));

    onSuccess.release();
    onFailure.release();
}
