
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
JNI_METHOD(void, ElectricalMeasurementCluster, readMeasurementTypeAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::MeasurementType::TypeInfo;
    std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasurementTypeAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcVoltageAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcVoltage::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcVoltageAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcVoltageMinAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcVoltageMin::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcVoltageMinAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcVoltageMaxAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcVoltageMax::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcVoltageMaxAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcCurrentMinAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcCurrentMin::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcCurrentMinAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcCurrentMaxAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcCurrentMax::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcCurrentMaxAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcPowerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcPower::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcPowerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcPowerMinAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcPowerMin::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcPowerMinAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcPowerMaxAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcPowerMax::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcPowerMaxAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcVoltageMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcVoltageMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcVoltageMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcVoltageDivisorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcVoltageDivisor::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcVoltageDivisorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcCurrentMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcCurrentMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcCurrentMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcCurrentDivisorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcCurrentDivisor::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcCurrentDivisorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcPowerMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcPowerMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcPowerMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readDcPowerDivisorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::DcPowerDivisor::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterDcPowerDivisorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcFrequencyAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcFrequency::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcFrequencyAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcFrequencyMinAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcFrequencyMin::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcFrequencyMinAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcFrequencyMaxAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcFrequencyMax::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcFrequencyMaxAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readNeutralCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::NeutralCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterNeutralCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readTotalActivePowerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::TotalActivePower::TypeInfo;
    std::unique_ptr<CHIPInt32sAttributeCallback, void (*)(CHIPInt32sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterTotalActivePowerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readTotalReactivePowerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::TotalReactivePower::TypeInfo;
    std::unique_ptr<CHIPInt32sAttributeCallback, void (*)(CHIPInt32sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterTotalReactivePowerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readTotalApparentPowerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::TotalApparentPower::TypeInfo;
    std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterTotalApparentPowerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasured1stHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::Measured1stHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasured1stHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasured3rdHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::Measured3rdHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasured3rdHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasured5thHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::Measured5thHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasured5thHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasured7thHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::Measured7thHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasured7thHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasured9thHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::Measured9thHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasured9thHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasured11thHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::Measured11thHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasured11thHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasuredPhase1stHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::MeasuredPhase1stHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasuredPhase1stHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasuredPhase3rdHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::MeasuredPhase3rdHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasuredPhase3rdHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasuredPhase5thHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::MeasuredPhase5thHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasuredPhase5thHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasuredPhase7thHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::MeasuredPhase7thHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasuredPhase7thHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasuredPhase9thHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::MeasuredPhase9thHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasuredPhase9thHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readMeasuredPhase11thHarmonicCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::MeasuredPhase11thHarmonicCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterMeasuredPhase11thHarmonicCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcFrequencyMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcFrequencyMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcFrequencyMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcFrequencyDivisorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcFrequencyDivisor::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcFrequencyDivisorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readPowerMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::PowerMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterPowerMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readPowerDivisorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::PowerDivisor::TypeInfo;
    std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterPowerDivisorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readHarmonicCurrentMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::HarmonicCurrentMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt8sAttributeCallback, void (*)(CHIPInt8sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterHarmonicCurrentMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readPhaseHarmonicCurrentMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::PhaseHarmonicCurrentMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt8sAttributeCallback, void (*)(CHIPInt8sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterPhaseHarmonicCurrentMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readInstantaneousVoltageAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::InstantaneousVoltage::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterInstantaneousVoltageAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readInstantaneousLineCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::InstantaneousLineCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterInstantaneousLineCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readInstantaneousActiveCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::InstantaneousActiveCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterInstantaneousActiveCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readInstantaneousReactiveCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::InstantaneousReactiveCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterInstantaneousReactiveCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readInstantaneousPowerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::InstantaneousPower::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterInstantaneousPowerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltage::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageMinAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageMin::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageMinAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageMaxAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageMax::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageMaxAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrent::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentMinAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrentMin::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentMinAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentMaxAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrentMax::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentMaxAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePower::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerMinAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePowerMin::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerMinAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerMaxAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePowerMax::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerMaxAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readReactivePowerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ReactivePower::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterReactivePowerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readApparentPowerAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ApparentPower::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterApparentPowerAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readPowerFactorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::PowerFactor::TypeInfo;
    std::unique_ptr<CHIPInt8sAttributeCallback, void (*)(CHIPInt8sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterPowerFactorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsVoltageMeasurementPeriodAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsVoltageMeasurementPeriod::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsVoltageMeasurementPeriodAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsUnderVoltageCounterAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsUnderVoltageCounter::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsUnderVoltageCounterAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsExtremeOverVoltagePeriodAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsExtremeOverVoltagePeriod::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsExtremeOverVoltagePeriodAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsExtremeUnderVoltagePeriodAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsExtremeUnderVoltagePeriod::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsExtremeUnderVoltagePeriodAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageSagPeriodAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageSagPeriod::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageSagPeriodAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageSwellPeriodAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageSwellPeriod::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageSwellPeriodAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcVoltageMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcVoltageMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcVoltageMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcVoltageDivisorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcVoltageDivisor::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcVoltageDivisorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcCurrentMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcCurrentMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcCurrentMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcCurrentDivisorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcCurrentDivisor::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcCurrentDivisorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcPowerMultiplierAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcPowerMultiplier::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcPowerMultiplierAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcPowerDivisorAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcPowerDivisor::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcPowerDivisorAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readOverloadAlarmsMaskAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::OverloadAlarmsMask::TypeInfo;
    std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterOverloadAlarmsMaskAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readVoltageOverloadAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::VoltageOverload::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterVoltageOverloadAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readCurrentOverloadAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::CurrentOverload::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterCurrentOverloadAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcOverloadAlarmsMaskAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcOverloadAlarmsMask::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcOverloadAlarmsMaskAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcVoltageOverloadAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcVoltageOverload::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcVoltageOverloadAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcCurrentOverloadAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcCurrentOverload::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcCurrentOverloadAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcActivePowerOverloadAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcActivePowerOverload::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcActivePowerOverloadAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcReactivePowerOverloadAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcReactivePowerOverload::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcReactivePowerOverloadAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsOverVoltageAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsOverVoltage::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsOverVoltageAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsUnderVoltageAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsUnderVoltage::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsUnderVoltageAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsExtremeOverVoltageAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsExtremeOverVoltage::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsExtremeOverVoltageAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsExtremeUnderVoltageAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsExtremeUnderVoltage::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsExtremeUnderVoltageAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageSagAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageSag::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageSagAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageSwellAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageSwell::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageSwellAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readLineCurrentPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::LineCurrentPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterLineCurrentPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActiveCurrentPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActiveCurrentPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActiveCurrentPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readReactiveCurrentPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ReactiveCurrentPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterReactiveCurrentPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltagePhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltagePhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltagePhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageMinPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageMinPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageMinPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageMaxPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageMaxPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageMaxPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrentPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentMinPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrentMinPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentMinPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentMaxPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrentMaxPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentMaxPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePowerPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerMinPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePowerMinPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerMinPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerMaxPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePowerMaxPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerMaxPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readReactivePowerPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ReactivePowerPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterReactivePowerPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readApparentPowerPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ApparentPowerPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterApparentPowerPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readPowerFactorPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::PowerFactorPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt8sAttributeCallback, void (*)(CHIPInt8sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterPowerFactorPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsVoltageMeasurementPeriodPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsVoltageMeasurementPeriodPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsVoltageMeasurementPeriodPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsOverVoltageCounterPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsOverVoltageCounterPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsOverVoltageCounterPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsUnderVoltageCounterPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsUnderVoltageCounterPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsUnderVoltageCounterPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsExtremeOverVoltagePeriodPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsExtremeOverVoltagePeriodPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsExtremeOverVoltagePeriodPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsExtremeUnderVoltagePeriodPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsExtremeUnderVoltagePeriodPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsExtremeUnderVoltagePeriodPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageSagPeriodPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageSagPeriodPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageSagPeriodPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageSwellPeriodPhaseBAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageSwellPeriodPhaseB::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageSwellPeriodPhaseBAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readLineCurrentPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::LineCurrentPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterLineCurrentPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActiveCurrentPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActiveCurrentPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActiveCurrentPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readReactiveCurrentPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ReactiveCurrentPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterReactiveCurrentPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltagePhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltagePhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltagePhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageMinPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageMinPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageMinPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageMaxPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageMaxPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageMaxPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrentPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentMinPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrentMinPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentMinPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsCurrentMaxPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsCurrentMaxPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsCurrentMaxPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePowerPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerMinPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePowerMinPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerMinPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readActivePowerMaxPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ActivePowerMaxPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterActivePowerMaxPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readReactivePowerPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ReactivePowerPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16sAttributeCallback, void (*)(CHIPInt16sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterReactivePowerPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readApparentPowerPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ApparentPowerPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterApparentPowerPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readPowerFactorPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::PowerFactorPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt8sAttributeCallback, void (*)(CHIPInt8sAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8sAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8sAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterPowerFactorPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsVoltageMeasurementPeriodPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsVoltageMeasurementPeriodPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsVoltageMeasurementPeriodPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsOverVoltageCounterPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsOverVoltageCounterPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsOverVoltageCounterPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAverageRmsUnderVoltageCounterPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AverageRmsUnderVoltageCounterPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAverageRmsUnderVoltageCounterPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsExtremeOverVoltagePeriodPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsExtremeOverVoltagePeriodPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsExtremeOverVoltagePeriodPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsExtremeUnderVoltagePeriodPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsExtremeUnderVoltagePeriodPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsExtremeUnderVoltagePeriodPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageSagPeriodPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageSagPeriodPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageSagPeriodPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readRmsVoltageSwellPeriodPhaseCAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::RmsVoltageSwellPeriodPhaseC::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterRmsVoltageSwellPeriodPhaseCAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::GeneratedCommandList::TypeInfo;
    std::unique_ptr<CHIPElectricalMeasurementGeneratedCommandListAttributeCallback, void (*)(CHIPElectricalMeasurementGeneratedCommandListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPElectricalMeasurementGeneratedCommandListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPElectricalMeasurementGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AcceptedCommandList::TypeInfo;
    std::unique_ptr<CHIPElectricalMeasurementAcceptedCommandListAttributeCallback, void (*)(CHIPElectricalMeasurementAcceptedCommandListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPElectricalMeasurementAcceptedCommandListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPElectricalMeasurementAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::EventList::TypeInfo;
    std::unique_ptr<CHIPElectricalMeasurementEventListAttributeCallback, void (*)(CHIPElectricalMeasurementEventListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPElectricalMeasurementEventListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPElectricalMeasurementEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::AttributeList::TypeInfo;
    std::unique_ptr<CHIPElectricalMeasurementAttributeListAttributeCallback, void (*)(CHIPElectricalMeasurementAttributeListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPElectricalMeasurementAttributeListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPElectricalMeasurementAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::FeatureMap::TypeInfo;
    std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, ElectricalMeasurementCluster, readClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::ElectricalMeasurement::Attributes::ClusterRevision::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPElectricalMeasurementClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


