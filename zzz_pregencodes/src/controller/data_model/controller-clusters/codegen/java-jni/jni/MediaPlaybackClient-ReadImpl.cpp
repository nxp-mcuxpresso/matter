
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
JNI_METHOD(void, MediaPlaybackCluster, readCurrentStateAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::CurrentState::TypeInfo;
    std::unique_ptr<CHIPInt8uAttributeCallback, void (*)(CHIPInt8uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt8uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt8uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterCurrentStateAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readStartTimeAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::StartTime::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackStartTimeAttributeCallback, void (*)(CHIPMediaPlaybackStartTimeAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackStartTimeAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackStartTimeAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterStartTimeAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readDurationAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::Duration::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackDurationAttributeCallback, void (*)(CHIPMediaPlaybackDurationAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackDurationAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackDurationAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterDurationAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readPlaybackSpeedAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::PlaybackSpeed::TypeInfo;
    std::unique_ptr<CHIPFloatAttributeCallback, void (*)(CHIPFloatAttributeCallback *)> onSuccess(chip::Platform::New<CHIPFloatAttributeCallback>(callback, false), chip::Platform::Delete<CHIPFloatAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterPlaybackSpeedAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readSeekRangeEndAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::SeekRangeEnd::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackSeekRangeEndAttributeCallback, void (*)(CHIPMediaPlaybackSeekRangeEndAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackSeekRangeEndAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackSeekRangeEndAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterSeekRangeEndAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readSeekRangeStartAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::SeekRangeStart::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackSeekRangeStartAttributeCallback, void (*)(CHIPMediaPlaybackSeekRangeStartAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackSeekRangeStartAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackSeekRangeStartAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterSeekRangeStartAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readAvailableAudioTracksAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::AvailableAudioTracks::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackAvailableAudioTracksAttributeCallback, void (*)(CHIPMediaPlaybackAvailableAudioTracksAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackAvailableAudioTracksAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackAvailableAudioTracksAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterAvailableAudioTracksAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readAvailableTextTracksAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::AvailableTextTracks::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackAvailableTextTracksAttributeCallback, void (*)(CHIPMediaPlaybackAvailableTextTracksAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackAvailableTextTracksAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackAvailableTextTracksAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterAvailableTextTracksAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readGeneratedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::GeneratedCommandList::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackGeneratedCommandListAttributeCallback, void (*)(CHIPMediaPlaybackGeneratedCommandListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackGeneratedCommandListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackGeneratedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterGeneratedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readAcceptedCommandListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::AcceptedCommandList::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackAcceptedCommandListAttributeCallback, void (*)(CHIPMediaPlaybackAcceptedCommandListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackAcceptedCommandListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackAcceptedCommandListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterAcceptedCommandListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readEventListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::EventList::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackEventListAttributeCallback, void (*)(CHIPMediaPlaybackEventListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackEventListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackEventListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterEventListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readAttributeListAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::AttributeList::TypeInfo;
    std::unique_ptr<CHIPMediaPlaybackAttributeListAttributeCallback, void (*)(CHIPMediaPlaybackAttributeListAttributeCallback *)> onSuccess(chip::Platform::New<CHIPMediaPlaybackAttributeListAttributeCallback>(callback, false), chip::Platform::Delete<CHIPMediaPlaybackAttributeListAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterAttributeListAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readFeatureMapAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::FeatureMap::TypeInfo;
    std::unique_ptr<CHIPInt32uAttributeCallback, void (*)(CHIPInt32uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt32uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt32uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterFeatureMapAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


JNI_METHOD(void, MediaPlaybackCluster, readClusterRevisionAttribute)(JNIEnv * env, jobject self, jlong clusterPtr, jobject callback)
{
    chip::DeviceLayer::StackLock lock;
    using TypeInfo = chip::app::Clusters::MediaPlayback::Attributes::ClusterRevision::TypeInfo;
    std::unique_ptr<CHIPInt16uAttributeCallback, void (*)(CHIPInt16uAttributeCallback *)> onSuccess(chip::Platform::New<CHIPInt16uAttributeCallback>(callback, false), chip::Platform::Delete<CHIPInt16uAttributeCallback>);
    VerifyOrReturn(onSuccess.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native success callback", CHIP_ERROR_NO_MEMORY));

    std::unique_ptr<chip::CHIPDefaultFailureCallback, void (*)(chip::CHIPDefaultFailureCallback *)> onFailure(chip::Platform::New<chip::CHIPDefaultFailureCallback>(callback), chip::Platform::Delete<chip::CHIPDefaultFailureCallback>);
    VerifyOrReturn(onFailure.get() != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error creating native failure callback", CHIP_ERROR_NO_MEMORY));

    CHIP_ERROR err = CHIP_NO_ERROR;
    chip::Controller::ClusterBase * cppCluster = reinterpret_cast<chip::Controller::ClusterBase *>(clusterPtr);
    VerifyOrReturn(cppCluster != nullptr, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Could not get native cluster", CHIP_ERROR_INCORRECT_STATE));

    auto successFn = chip::Callback::Callback<CHIPMediaPlaybackClusterClusterRevisionAttributeCallbackType>::FromCancelable(onSuccess->Cancel());
    auto failureFn = chip::Callback::Callback<CHIPDefaultFailureCallbackType>::FromCancelable(onFailure->Cancel());
    err = cppCluster->ReadAttribute<TypeInfo>(onSuccess->mContext, successFn->mCall, failureFn->mCall);
    VerifyOrReturn(err == CHIP_NO_ERROR, chip::AndroidClusterExceptions::GetInstance().ReturnIllegalStateException(env, callback, "Error reading attribute", err));

    onSuccess.release();
    onFailure.release();
}


