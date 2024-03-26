#include <tlv/meta/clusters_meta.h>

namespace chip {
namespace TLVMeta {
namespace {

using namespace chip::FlatTree;
using namespace chip::TLV;

const Entry<ItemInfo> _Identify[] = {
  { { AttributeTag(0), "identifyTime", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::int16u
  { { AttributeTag(1), "identifyType", ItemType::kEnum }, 7 }, // Identify::IdentifyTypeEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Identify::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Identify::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Identify::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Identify::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::int16u
  { { CommandTag(0), "Identify", ItemType::kDefault }, 3 }, // Identify::Identify::IdentifyRequest
  { { CommandTag(64), "TriggerEffect", ItemType::kDefault }, 4 }, // Identify::TriggerEffect::TriggerEffectRequest
};

const Entry<ItemInfo> _Identify_IdentifyRequest[] = {
  { { ContextTag(0), "identifyTime", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::int16u
};

const Entry<ItemInfo> _Identify_TriggerEffectRequest[] = {
  { { ContextTag(0), "effectIdentifier", ItemType::kEnum }, 5 }, // Identify::EffectIdentifierEnum
  { { ContextTag(1), "effectVariant", ItemType::kEnum }, 6 }, // Identify::EffectVariantEnum
};

const Entry<ItemInfo> _Identify_EffectIdentifierEnum[] = {
  { { ConstantValueTag(0x0), "kBlink", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::EffectIdentifierEnum::kBlink
  { { ConstantValueTag(0x1), "kBreathe", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::EffectIdentifierEnum::kBreathe
  { { ConstantValueTag(0x2), "kOkay", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::EffectIdentifierEnum::kOkay
  { { ConstantValueTag(0xB), "kChannelChange", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::EffectIdentifierEnum::kChannelChange
  { { ConstantValueTag(0xFE), "kFinishEffect", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::EffectIdentifierEnum::kFinishEffect
  { { ConstantValueTag(0xFF), "kStopEffect", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::EffectIdentifierEnum::kStopEffect
};

const Entry<ItemInfo> _Identify_EffectVariantEnum[] = {
  { { ConstantValueTag(0x0), "kDefault", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::EffectVariantEnum::kDefault
};

const Entry<ItemInfo> _Identify_IdentifyTypeEnum[] = {
  { { ConstantValueTag(0x0), "kNone", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::IdentifyTypeEnum::kNone
  { { ConstantValueTag(0x1), "kLightOutput", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::IdentifyTypeEnum::kLightOutput
  { { ConstantValueTag(0x2), "kVisibleIndicator", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::IdentifyTypeEnum::kVisibleIndicator
  { { ConstantValueTag(0x3), "kAudibleBeep", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::IdentifyTypeEnum::kAudibleBeep
  { { ConstantValueTag(0x4), "kDisplay", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::IdentifyTypeEnum::kDisplay
  { { ConstantValueTag(0x5), "kActuator", ItemType::kDefault }, kInvalidNodeIndex }, // Identify::IdentifyTypeEnum::kActuator
};

const Entry<ItemInfo> _Groups[] = {
  { { AttributeTag(0), "nameSupport", ItemType::kBitmap }, 19 }, // Groups::NameSupportBitmap
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Groups::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Groups::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Groups::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Groups::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 18 }, // Groups::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::int16u
  { { CommandTag(0), "AddGroup", ItemType::kDefault }, 9 }, // Groups::AddGroup::AddGroupRequest
  { { CommandTag(1), "ViewGroup", ItemType::kDefault }, 11 }, // Groups::ViewGroup::ViewGroupRequest
  { { CommandTag(2), "GetGroupMembership", ItemType::kDefault }, 13 }, // Groups::GetGroupMembership::GetGroupMembershipRequest
  { { CommandTag(3), "RemoveGroup", ItemType::kDefault }, 15 }, // Groups::RemoveGroup::RemoveGroupRequest
  { { CommandTag(4), "RemoveAllGroups", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::RemoveAllGroups::()
  { { CommandTag(5), "AddGroupIfIdentifying", ItemType::kDefault }, 17 }, // Groups::AddGroupIfIdentifying::AddGroupIfIdentifyingRequest
  { { CommandTag(0), "AddGroupResponse", ItemType::kDefault }, 10 }, // Groups::AddGroupResponse
  { { CommandTag(1), "ViewGroupResponse", ItemType::kDefault }, 12 }, // Groups::ViewGroupResponse
  { { CommandTag(2), "GetGroupMembershipResponse", ItemType::kDefault }, 14 }, // Groups::GetGroupMembershipResponse
  { { CommandTag(3), "RemoveGroupResponse", ItemType::kDefault }, 16 }, // Groups::RemoveGroupResponse
};

const Entry<ItemInfo> _Groups_AddGroupRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::group_id
  { { ContextTag(1), "groupName", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::char_string
};

const Entry<ItemInfo> _Groups_AddGroupResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::enum8
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::group_id
};

const Entry<ItemInfo> _Groups_ViewGroupRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::group_id
};

const Entry<ItemInfo> _Groups_ViewGroupResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::enum8
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::group_id
  { { ContextTag(2), "groupName", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::char_string
};

const Entry<ItemInfo> _Groups_GetGroupMembershipRequest[] = {
  { { ContextTag(0), "groupList", ItemType::kList }, 1 }, // Groups::group_id[]
};

const Entry<ItemInfo> _Groups_GetGroupMembershipResponse[] = {
  { { ContextTag(0), "capacity", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::int8u
  { { ContextTag(1), "groupList", ItemType::kList }, 1 }, // Groups::group_id[]
};

const Entry<ItemInfo> _Groups_RemoveGroupRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::group_id
};

const Entry<ItemInfo> _Groups_RemoveGroupResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::enum8
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::group_id
};

const Entry<ItemInfo> _Groups_AddGroupIfIdentifyingRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::group_id
  { { ContextTag(1), "groupName", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::char_string
};

const Entry<ItemInfo> _Groups_Feature[] = {
  { { ConstantValueTag(0x1), "kGroupNames", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::Feature::kGroupNames
};

const Entry<ItemInfo> _Groups_NameSupportBitmap[] = {
  { { ConstantValueTag(0x80), "kGroupNames", ItemType::kDefault }, kInvalidNodeIndex }, // Groups::NameSupportBitmap::kGroupNames
};

const Entry<ItemInfo> _Scenes[] = {
  { { AttributeTag(0), "sceneCount", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { AttributeTag(1), "currentScene", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { AttributeTag(2), "currentGroup", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { AttributeTag(3), "sceneValid", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::boolean
  { { AttributeTag(4), "nameSupport", ItemType::kBitmap }, 48 }, // Scenes::NameSupportBitmap
  { { AttributeTag(5), "lastConfiguredBy", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::node_id
  { { AttributeTag(6), "sceneTableSize", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int16u
  { { AttributeTag(7), "fabricSceneInfo", ItemType::kList }, 43 }, // Scenes::SceneInfoStruct[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Scenes::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Scenes::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Scenes::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Scenes::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 47 }, // Scenes::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int16u
  { { CommandTag(0), "AddScene", ItemType::kDefault }, 24 }, // Scenes::AddScene::AddSceneRequest
  { { CommandTag(1), "ViewScene", ItemType::kDefault }, 26 }, // Scenes::ViewScene::ViewSceneRequest
  { { CommandTag(2), "RemoveScene", ItemType::kDefault }, 28 }, // Scenes::RemoveScene::RemoveSceneRequest
  { { CommandTag(3), "RemoveAllScenes", ItemType::kDefault }, 30 }, // Scenes::RemoveAllScenes::RemoveAllScenesRequest
  { { CommandTag(4), "StoreScene", ItemType::kDefault }, 32 }, // Scenes::StoreScene::StoreSceneRequest
  { { CommandTag(5), "RecallScene", ItemType::kDefault }, 34 }, // Scenes::RecallScene::RecallSceneRequest
  { { CommandTag(6), "GetSceneMembership", ItemType::kDefault }, 35 }, // Scenes::GetSceneMembership::GetSceneMembershipRequest
  { { CommandTag(64), "EnhancedAddScene", ItemType::kDefault }, 37 }, // Scenes::EnhancedAddScene::EnhancedAddSceneRequest
  { { CommandTag(65), "EnhancedViewScene", ItemType::kDefault }, 39 }, // Scenes::EnhancedViewScene::EnhancedViewSceneRequest
  { { CommandTag(66), "CopyScene", ItemType::kDefault }, 41 }, // Scenes::CopyScene::CopySceneRequest
  { { CommandTag(0), "AddSceneResponse", ItemType::kDefault }, 25 }, // Scenes::AddSceneResponse
  { { CommandTag(1), "ViewSceneResponse", ItemType::kDefault }, 27 }, // Scenes::ViewSceneResponse
  { { CommandTag(2), "RemoveSceneResponse", ItemType::kDefault }, 29 }, // Scenes::RemoveSceneResponse
  { { CommandTag(3), "RemoveAllScenesResponse", ItemType::kDefault }, 31 }, // Scenes::RemoveAllScenesResponse
  { { CommandTag(4), "StoreSceneResponse", ItemType::kDefault }, 33 }, // Scenes::StoreSceneResponse
  { { CommandTag(6), "GetSceneMembershipResponse", ItemType::kDefault }, 36 }, // Scenes::GetSceneMembershipResponse
  { { CommandTag(64), "EnhancedAddSceneResponse", ItemType::kDefault }, 38 }, // Scenes::EnhancedAddSceneResponse
  { { CommandTag(65), "EnhancedViewSceneResponse", ItemType::kDefault }, 40 }, // Scenes::EnhancedViewSceneResponse
  { { CommandTag(66), "CopySceneResponse", ItemType::kDefault }, 42 }, // Scenes::CopySceneResponse
};

const Entry<ItemInfo> _Scenes_AttributeValuePair[] = {
  { { ContextTag(0), "attributeID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::attrib_id
  { { ContextTag(1), "attributeValue", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int32u
};

const Entry<ItemInfo> _Scenes_ExtensionFieldSet[] = {
  { { ContextTag(0), "clusterID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::cluster_id
  { { ContextTag(1), "attributeValueList", ItemType::kList }, 44 }, // Scenes::AttributeValuePair[]
};

const Entry<ItemInfo> _Scenes_SceneInfoStruct[] = {
  { { ContextTag(0), "sceneCount", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(1), "currentScene", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(2), "currentGroup", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(3), "sceneValid", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::boolean
  { { ContextTag(4), "remainingCapacity", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::fabric_idx
};

const Entry<ItemInfo> _Scenes_AddSceneRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(1), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int16u
  { { ContextTag(3), "sceneName", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::char_string
  { { ContextTag(4), "extensionFieldSets", ItemType::kList }, 45 }, // Scenes::ExtensionFieldSet[]
};

const Entry<ItemInfo> _Scenes_AddSceneResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(2), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_ViewSceneRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(1), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_ViewSceneResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(2), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(3), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int16u
  { { ContextTag(4), "sceneName", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::char_string
  { { ContextTag(5), "extensionFieldSets", ItemType::kList }, 45 }, // Scenes::ExtensionFieldSet[]
};

const Entry<ItemInfo> _Scenes_RemoveSceneRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(1), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_RemoveSceneResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(2), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_RemoveAllScenesRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
};

const Entry<ItemInfo> _Scenes_RemoveAllScenesResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
};

const Entry<ItemInfo> _Scenes_StoreSceneRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(1), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_StoreSceneResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(2), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_RecallSceneRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(1), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int16u
};

const Entry<ItemInfo> _Scenes_GetSceneMembershipRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
};

const Entry<ItemInfo> _Scenes_GetSceneMembershipResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "capacity", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(2), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(3), "sceneList", ItemType::kList }, 1 }, // Scenes::int8u[]
};

const Entry<ItemInfo> _Scenes_EnhancedAddSceneRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(1), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int16u
  { { ContextTag(3), "sceneName", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::char_string
  { { ContextTag(4), "extensionFieldSets", ItemType::kList }, 45 }, // Scenes::ExtensionFieldSet[]
};

const Entry<ItemInfo> _Scenes_EnhancedAddSceneResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(2), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_EnhancedViewSceneRequest[] = {
  { { ContextTag(0), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(1), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_EnhancedViewSceneResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "groupID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(2), "sceneID", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(3), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int16u
  { { ContextTag(4), "sceneName", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::char_string
  { { ContextTag(5), "extensionFieldSets", ItemType::kList }, 45 }, // Scenes::ExtensionFieldSet[]
};

const Entry<ItemInfo> _Scenes_CopySceneRequest[] = {
  { { ContextTag(0), "mode", ItemType::kBitmap }, 46 }, // Scenes::CopyModeBitmap
  { { ContextTag(1), "groupIdentifierFrom", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(2), "sceneIdentifierFrom", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
  { { ContextTag(3), "groupIdentifierTo", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(4), "sceneIdentifierTo", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_CopySceneResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::status
  { { ContextTag(1), "groupIdentifierFrom", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::group_id
  { { ContextTag(2), "sceneIdentifierFrom", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::int8u
};

const Entry<ItemInfo> _Scenes_SceneInfoStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 23 }, // Scenes_SceneInfoStruct[]
};

const Entry<ItemInfo> _Scenes_AttributeValuePair_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 21 }, // Scenes_AttributeValuePair[]
};

const Entry<ItemInfo> _Scenes_ExtensionFieldSet_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 22 }, // Scenes_ExtensionFieldSet[]
};

const Entry<ItemInfo> _Scenes_CopyModeBitmap[] = {
  { { ConstantValueTag(0x1), "kCopyAllScenes", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::CopyModeBitmap::kCopyAllScenes
};

const Entry<ItemInfo> _Scenes_Feature[] = {
  { { ConstantValueTag(0x1), "kSceneNames", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::Feature::kSceneNames
  { { ConstantValueTag(0x2), "kExplicit", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::Feature::kExplicit
  { { ConstantValueTag(0x4), "kTableSize", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::Feature::kTableSize
  { { ConstantValueTag(0x8), "kFabricScenes", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::Feature::kFabricScenes
};

const Entry<ItemInfo> _Scenes_NameSupportBitmap[] = {
  { { ConstantValueTag(0x80), "kSceneNames", ItemType::kDefault }, kInvalidNodeIndex }, // Scenes::NameSupportBitmap::kSceneNames
};

const Entry<ItemInfo> _OnOff[] = {
  { { AttributeTag(0), "onOff", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::boolean
  { { AttributeTag(16384), "globalSceneControl", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::boolean
  { { AttributeTag(16385), "onTime", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::int16u
  { { AttributeTag(16386), "offWaitTime", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::int16u
  { { AttributeTag(16387), "startUpOnOff", ItemType::kEnum }, 55 }, // OnOff::StartUpOnOffEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OnOff::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OnOff::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OnOff::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OnOff::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 56 }, // OnOff::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::int16u
  { { CommandTag(0), "Off", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::Off::()
  { { CommandTag(1), "On", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::On::()
  { { CommandTag(2), "Toggle", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::Toggle::()
  { { CommandTag(64), "OffWithEffect", ItemType::kDefault }, 50 }, // OnOff::OffWithEffect::OffWithEffectRequest
  { { CommandTag(65), "OnWithRecallGlobalScene", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::OnWithRecallGlobalScene::()
  { { CommandTag(66), "OnWithTimedOff", ItemType::kDefault }, 51 }, // OnOff::OnWithTimedOff::OnWithTimedOffRequest
};

const Entry<ItemInfo> _OnOff_OffWithEffectRequest[] = {
  { { ContextTag(0), "effectIdentifier", ItemType::kEnum }, 54 }, // OnOff::EffectIdentifierEnum
  { { ContextTag(1), "effectVariant", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::enum8
};

const Entry<ItemInfo> _OnOff_OnWithTimedOffRequest[] = {
  { { ContextTag(0), "onOffControl", ItemType::kBitmap }, 57 }, // OnOff::OnOffControlBitmap
  { { ContextTag(1), "onTime", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::int16u
  { { ContextTag(2), "offWaitTime", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::int16u
};

const Entry<ItemInfo> _OnOff_DelayedAllOffEffectVariantEnum[] = {
  { { ConstantValueTag(0x0), "kDelayedOffFastFade", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::DelayedAllOffEffectVariantEnum::kDelayedOffFastFade
  { { ConstantValueTag(0x1), "kNoFade", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::DelayedAllOffEffectVariantEnum::kNoFade
  { { ConstantValueTag(0x2), "kDelayedOffSlowFade", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::DelayedAllOffEffectVariantEnum::kDelayedOffSlowFade
};

const Entry<ItemInfo> _OnOff_DyingLightEffectVariantEnum[] = {
  { { ConstantValueTag(0x0), "kDyingLightFadeOff", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::DyingLightEffectVariantEnum::kDyingLightFadeOff
};

const Entry<ItemInfo> _OnOff_EffectIdentifierEnum[] = {
  { { ConstantValueTag(0x0), "kDelayedAllOff", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::EffectIdentifierEnum::kDelayedAllOff
  { { ConstantValueTag(0x1), "kDyingLight", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::EffectIdentifierEnum::kDyingLight
};

const Entry<ItemInfo> _OnOff_StartUpOnOffEnum[] = {
  { { ConstantValueTag(0x0), "kOff", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::StartUpOnOffEnum::kOff
  { { ConstantValueTag(0x1), "kOn", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::StartUpOnOffEnum::kOn
  { { ConstantValueTag(0x2), "kToggle", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::StartUpOnOffEnum::kToggle
};

const Entry<ItemInfo> _OnOff_Feature[] = {
  { { ConstantValueTag(0x1), "kLighting", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::Feature::kLighting
  { { ConstantValueTag(0x2), "kDeadFrontBehavior", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::Feature::kDeadFrontBehavior
  { { ConstantValueTag(0x4), "kOffOnly", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::Feature::kOffOnly
};

const Entry<ItemInfo> _OnOff_OnOffControlBitmap[] = {
  { { ConstantValueTag(0x1), "kAcceptOnlyWhenOn", ItemType::kDefault }, kInvalidNodeIndex }, // OnOff::OnOffControlBitmap::kAcceptOnlyWhenOn
};

const Entry<ItemInfo> _OnOffSwitchConfiguration[] = {
  { { AttributeTag(0), "switchType", ItemType::kDefault }, kInvalidNodeIndex }, // OnOffSwitchConfiguration::enum8
  { { AttributeTag(16), "switchActions", ItemType::kDefault }, kInvalidNodeIndex }, // OnOffSwitchConfiguration::enum8
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OnOffSwitchConfiguration::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OnOffSwitchConfiguration::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OnOffSwitchConfiguration::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OnOffSwitchConfiguration::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // OnOffSwitchConfiguration::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OnOffSwitchConfiguration::int16u
};

const Entry<ItemInfo> _LevelControl[] = {
  { { AttributeTag(0), "currentLevel", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { AttributeTag(1), "remainingTime", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { AttributeTag(2), "minLevel", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { AttributeTag(3), "maxLevel", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { AttributeTag(4), "currentFrequency", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { AttributeTag(5), "minFrequency", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { AttributeTag(6), "maxFrequency", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { AttributeTag(15), "options", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { AttributeTag(16), "onOffTransitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { AttributeTag(17), "onLevel", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { AttributeTag(18), "onTransitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { AttributeTag(19), "offTransitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { AttributeTag(20), "defaultMoveRate", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { AttributeTag(16384), "startUpCurrentLevel", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // LevelControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // LevelControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // LevelControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // LevelControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 71 }, // LevelControl::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { CommandTag(0), "MoveToLevel", ItemType::kDefault }, 60 }, // LevelControl::MoveToLevel::MoveToLevelRequest
  { { CommandTag(1), "Move", ItemType::kDefault }, 61 }, // LevelControl::Move::MoveRequest
  { { CommandTag(2), "Step", ItemType::kDefault }, 62 }, // LevelControl::Step::StepRequest
  { { CommandTag(3), "Stop", ItemType::kDefault }, 63 }, // LevelControl::Stop::StopRequest
  { { CommandTag(4), "MoveToLevelWithOnOff", ItemType::kDefault }, 64 }, // LevelControl::MoveToLevelWithOnOff::MoveToLevelWithOnOffRequest
  { { CommandTag(5), "MoveWithOnOff", ItemType::kDefault }, 65 }, // LevelControl::MoveWithOnOff::MoveWithOnOffRequest
  { { CommandTag(6), "StepWithOnOff", ItemType::kDefault }, 66 }, // LevelControl::StepWithOnOff::StepWithOnOffRequest
  { { CommandTag(7), "StopWithOnOff", ItemType::kDefault }, 67 }, // LevelControl::StopWithOnOff::StopWithOnOffRequest
  { { CommandTag(8), "MoveToClosestFrequency", ItemType::kDefault }, 68 }, // LevelControl::MoveToClosestFrequency::MoveToClosestFrequencyRequest
};

const Entry<ItemInfo> _LevelControl_MoveToLevelRequest[] = {
  { { ContextTag(0), "level", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { ContextTag(1), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { ContextTag(2), "optionsMask", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { ContextTag(3), "optionsOverride", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
};

const Entry<ItemInfo> _LevelControl_MoveRequest[] = {
  { { ContextTag(0), "moveMode", ItemType::kEnum }, 69 }, // LevelControl::MoveModeEnum
  { { ContextTag(1), "rate", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { ContextTag(2), "optionsMask", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { ContextTag(3), "optionsOverride", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
};

const Entry<ItemInfo> _LevelControl_StepRequest[] = {
  { { ContextTag(0), "stepMode", ItemType::kEnum }, 70 }, // LevelControl::StepModeEnum
  { { ContextTag(1), "stepSize", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { ContextTag(4), "optionsOverride", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
};

const Entry<ItemInfo> _LevelControl_StopRequest[] = {
  { { ContextTag(0), "optionsMask", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { ContextTag(1), "optionsOverride", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
};

const Entry<ItemInfo> _LevelControl_MoveToLevelWithOnOffRequest[] = {
  { { ContextTag(0), "level", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { ContextTag(1), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { ContextTag(2), "optionsMask", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { ContextTag(3), "optionsOverride", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
};

const Entry<ItemInfo> _LevelControl_MoveWithOnOffRequest[] = {
  { { ContextTag(0), "moveMode", ItemType::kEnum }, 69 }, // LevelControl::MoveModeEnum
  { { ContextTag(1), "rate", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { ContextTag(2), "optionsMask", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { ContextTag(3), "optionsOverride", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
};

const Entry<ItemInfo> _LevelControl_StepWithOnOffRequest[] = {
  { { ContextTag(0), "stepMode", ItemType::kEnum }, 70 }, // LevelControl::StepModeEnum
  { { ContextTag(1), "stepSize", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { ContextTag(4), "optionsOverride", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
};

const Entry<ItemInfo> _LevelControl_StopWithOnOffRequest[] = {
  { { ContextTag(0), "optionsMask", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
  { { ContextTag(1), "optionsOverride", ItemType::kBitmap }, 72 }, // LevelControl::OptionsBitmap
};

const Entry<ItemInfo> _LevelControl_MoveToClosestFrequencyRequest[] = {
  { { ContextTag(0), "frequency", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::int16u
};

const Entry<ItemInfo> _LevelControl_MoveModeEnum[] = {
  { { ConstantValueTag(0x0), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::MoveModeEnum::kUp
  { { ConstantValueTag(0x1), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::MoveModeEnum::kDown
};

const Entry<ItemInfo> _LevelControl_StepModeEnum[] = {
  { { ConstantValueTag(0x0), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::StepModeEnum::kUp
  { { ConstantValueTag(0x1), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::StepModeEnum::kDown
};

const Entry<ItemInfo> _LevelControl_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::Feature::kOnOff
  { { ConstantValueTag(0x2), "kLighting", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::Feature::kLighting
  { { ConstantValueTag(0x4), "kFrequency", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::Feature::kFrequency
};

const Entry<ItemInfo> _LevelControl_OptionsBitmap[] = {
  { { ConstantValueTag(0x1), "kExecuteIfOff", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::OptionsBitmap::kExecuteIfOff
  { { ConstantValueTag(0x2), "kCoupleColorTempToLevel", ItemType::kDefault }, kInvalidNodeIndex }, // LevelControl::OptionsBitmap::kCoupleColorTempToLevel
};

const Entry<ItemInfo> _BinaryInputBasic[] = {
  { { AttributeTag(4), "activeText", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::char_string
  { { AttributeTag(28), "description", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::char_string
  { { AttributeTag(46), "inactiveText", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::char_string
  { { AttributeTag(81), "outOfService", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::boolean
  { { AttributeTag(84), "polarity", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::enum8
  { { AttributeTag(85), "presentValue", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::boolean
  { { AttributeTag(103), "reliability", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::enum8
  { { AttributeTag(111), "statusFlags", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::bitmap8
  { { AttributeTag(256), "applicationType", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::int32u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // BinaryInputBasic::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // BinaryInputBasic::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // BinaryInputBasic::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // BinaryInputBasic::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // BinaryInputBasic::int16u
};

const Entry<ItemInfo> _PulseWidthModulation[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // PulseWidthModulation::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // PulseWidthModulation::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // PulseWidthModulation::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // PulseWidthModulation::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // PulseWidthModulation::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // PulseWidthModulation::int16u
};

const Entry<ItemInfo> _Descriptor[] = {
  { { AttributeTag(0), "deviceTypeList", ItemType::kList }, 79 }, // Descriptor::DeviceTypeStruct[]
  { { AttributeTag(1), "serverList", ItemType::kList }, 1 }, // Descriptor::cluster_id[]
  { { AttributeTag(2), "clientList", ItemType::kList }, 1 }, // Descriptor::cluster_id[]
  { { AttributeTag(3), "partsList", ItemType::kList }, 1 }, // Descriptor::endpoint_no[]
  { { AttributeTag(4), "tagList", ItemType::kList }, 78 }, // Descriptor::SemanticTagStruct[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Descriptor::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Descriptor::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Descriptor::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Descriptor::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 80 }, // Descriptor::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Descriptor::int16u
};

const Entry<ItemInfo> _Descriptor_DeviceTypeStruct[] = {
  { { ContextTag(0), "deviceType", ItemType::kDefault }, kInvalidNodeIndex }, // Descriptor::devtype_id
  { { ContextTag(1), "revision", ItemType::kDefault }, kInvalidNodeIndex }, // Descriptor::int16u
};

const Entry<ItemInfo> _Descriptor_SemanticTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // Descriptor::vendor_id
  { { ContextTag(1), "namespaceID", ItemType::kDefault }, kInvalidNodeIndex }, // Descriptor::enum8
  { { ContextTag(2), "tag", ItemType::kDefault }, kInvalidNodeIndex }, // Descriptor::enum8
  { { ContextTag(3), "label", ItemType::kDefault }, kInvalidNodeIndex }, // Descriptor::char_string
};

const Entry<ItemInfo> _Descriptor_SemanticTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 77 }, // Descriptor_SemanticTagStruct[]
};

const Entry<ItemInfo> _Descriptor_DeviceTypeStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 76 }, // Descriptor_DeviceTypeStruct[]
};

const Entry<ItemInfo> _Descriptor_Feature[] = {
  { { ConstantValueTag(0x1), "kTagList", ItemType::kDefault }, kInvalidNodeIndex }, // Descriptor::Feature::kTagList
};

const Entry<ItemInfo> _Binding[] = {
  { { AttributeTag(0), "binding", ItemType::kList }, 83 }, // Binding::TargetStruct[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Binding::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Binding::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Binding::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Binding::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // Binding::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Binding::int16u
};

const Entry<ItemInfo> _Binding_TargetStruct[] = {
  { { ContextTag(1), "node", ItemType::kDefault }, kInvalidNodeIndex }, // Binding::node_id
  { { ContextTag(2), "group", ItemType::kDefault }, kInvalidNodeIndex }, // Binding::group_id
  { { ContextTag(3), "endpoint", ItemType::kDefault }, kInvalidNodeIndex }, // Binding::endpoint_no
  { { ContextTag(4), "cluster", ItemType::kDefault }, kInvalidNodeIndex }, // Binding::cluster_id
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // Binding::fabric_idx
};

const Entry<ItemInfo> _Binding_TargetStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 82 }, // Binding_TargetStruct[]
};

const Entry<ItemInfo> _AccessControl[] = {
  { { AttributeTag(0), "acl", ItemType::kList }, 92 }, // AccessControl::AccessControlEntryStruct[]
  { { AttributeTag(1), "extension", ItemType::kList }, 91 }, // AccessControl::AccessControlExtensionStruct[]
  { { AttributeTag(2), "subjectsPerAccessControlEntry", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::int16u
  { { AttributeTag(3), "targetsPerAccessControlEntry", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::int16u
  { { AttributeTag(4), "accessControlEntriesPerFabric", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // AccessControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // AccessControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // AccessControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // AccessControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::int16u
  { { EventTag(0), "AccessControlEntryChanged", ItemType::kDefault }, 88 }, // AccessControl::AccessControlEntryChanged
  { { EventTag(1), "AccessControlExtensionChanged", ItemType::kDefault }, 89 }, // AccessControl::AccessControlExtensionChanged
};

const Entry<ItemInfo> _AccessControl_AccessControlTargetStruct[] = {
  { { ContextTag(0), "cluster", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::cluster_id
  { { ContextTag(1), "endpoint", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::endpoint_no
  { { ContextTag(2), "deviceType", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::devtype_id
};

const Entry<ItemInfo> _AccessControl_AccessControlEntryStruct[] = {
  { { ContextTag(1), "privilege", ItemType::kEnum }, 94 }, // AccessControl::AccessControlEntryPrivilegeEnum
  { { ContextTag(2), "authMode", ItemType::kEnum }, 93 }, // AccessControl::AccessControlEntryAuthModeEnum
  { { ContextTag(3), "subjects", ItemType::kList }, 1 }, // AccessControl::int64u[]
  { { ContextTag(4), "targets", ItemType::kList }, 90 }, // AccessControl::AccessControlTargetStruct[]
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::fabric_idx
};

const Entry<ItemInfo> _AccessControl_AccessControlExtensionStruct[] = {
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::octet_string
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::fabric_idx
};

const Entry<ItemInfo> _AccessControl_AccessControlEntryChanged[] = {
  { { ContextTag(1), "adminNodeID", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::node_id
  { { ContextTag(2), "adminPasscodeID", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::int16u
  { { ContextTag(3), "changeType", ItemType::kEnum }, 95 }, // AccessControl::ChangeTypeEnum
  { { ContextTag(4), "latestValue", ItemType::kDefault }, 86 }, // AccessControl::AccessControlEntryStruct
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::fabric_idx
};

const Entry<ItemInfo> _AccessControl_AccessControlExtensionChanged[] = {
  { { ContextTag(1), "adminNodeID", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::node_id
  { { ContextTag(2), "adminPasscodeID", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::int16u
  { { ContextTag(3), "changeType", ItemType::kEnum }, 95 }, // AccessControl::ChangeTypeEnum
  { { ContextTag(4), "latestValue", ItemType::kDefault }, 87 }, // AccessControl::AccessControlExtensionStruct
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::fabric_idx
};

const Entry<ItemInfo> _AccessControl_AccessControlTargetStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 85 }, // AccessControl_AccessControlTargetStruct[]
};

const Entry<ItemInfo> _AccessControl_AccessControlExtensionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 87 }, // AccessControl_AccessControlExtensionStruct[]
};

const Entry<ItemInfo> _AccessControl_AccessControlEntryStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 86 }, // AccessControl_AccessControlEntryStruct[]
};

const Entry<ItemInfo> _AccessControl_AccessControlEntryAuthModeEnum[] = {
  { { ConstantValueTag(0x1), "kPASE", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::AccessControlEntryAuthModeEnum::kPASE
  { { ConstantValueTag(0x2), "kCASE", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::AccessControlEntryAuthModeEnum::kCASE
  { { ConstantValueTag(0x3), "kGroup", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::AccessControlEntryAuthModeEnum::kGroup
};

const Entry<ItemInfo> _AccessControl_AccessControlEntryPrivilegeEnum[] = {
  { { ConstantValueTag(0x1), "kView", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::AccessControlEntryPrivilegeEnum::kView
  { { ConstantValueTag(0x2), "kProxyView", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::AccessControlEntryPrivilegeEnum::kProxyView
  { { ConstantValueTag(0x3), "kOperate", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::AccessControlEntryPrivilegeEnum::kOperate
  { { ConstantValueTag(0x4), "kManage", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::AccessControlEntryPrivilegeEnum::kManage
  { { ConstantValueTag(0x5), "kAdminister", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::AccessControlEntryPrivilegeEnum::kAdminister
};

const Entry<ItemInfo> _AccessControl_ChangeTypeEnum[] = {
  { { ConstantValueTag(0x0), "kChanged", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::ChangeTypeEnum::kChanged
  { { ConstantValueTag(0x1), "kAdded", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::ChangeTypeEnum::kAdded
  { { ConstantValueTag(0x2), "kRemoved", ItemType::kDefault }, kInvalidNodeIndex }, // AccessControl::ChangeTypeEnum::kRemoved
};

const Entry<ItemInfo> _Actions[] = {
  { { AttributeTag(0), "actionList", ItemType::kList }, 113 }, // Actions::ActionStruct[]
  { { AttributeTag(1), "endpointLists", ItemType::kList }, 114 }, // Actions::EndpointListStruct[]
  { { AttributeTag(2), "setupURL", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::long_char_string
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Actions::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Actions::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Actions::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Actions::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { EventTag(0), "StateChanged", ItemType::kDefault }, 111 }, // Actions::StateChanged
  { { EventTag(1), "ActionFailed", ItemType::kDefault }, 112 }, // Actions::ActionFailed
  { { CommandTag(0), "InstantAction", ItemType::kDefault }, 99 }, // Actions::InstantAction::InstantActionRequest
  { { CommandTag(1), "InstantActionWithTransition", ItemType::kDefault }, 100 }, // Actions::InstantActionWithTransition::InstantActionWithTransitionRequest
  { { CommandTag(2), "StartAction", ItemType::kDefault }, 101 }, // Actions::StartAction::StartActionRequest
  { { CommandTag(3), "StartActionWithDuration", ItemType::kDefault }, 102 }, // Actions::StartActionWithDuration::StartActionWithDurationRequest
  { { CommandTag(4), "StopAction", ItemType::kDefault }, 103 }, // Actions::StopAction::StopActionRequest
  { { CommandTag(5), "PauseAction", ItemType::kDefault }, 104 }, // Actions::PauseAction::PauseActionRequest
  { { CommandTag(6), "PauseActionWithDuration", ItemType::kDefault }, 105 }, // Actions::PauseActionWithDuration::PauseActionWithDurationRequest
  { { CommandTag(7), "ResumeAction", ItemType::kDefault }, 106 }, // Actions::ResumeAction::ResumeActionRequest
  { { CommandTag(8), "EnableAction", ItemType::kDefault }, 107 }, // Actions::EnableAction::EnableActionRequest
  { { CommandTag(9), "EnableActionWithDuration", ItemType::kDefault }, 108 }, // Actions::EnableActionWithDuration::EnableActionWithDurationRequest
  { { CommandTag(10), "DisableAction", ItemType::kDefault }, 109 }, // Actions::DisableAction::DisableActionRequest
  { { CommandTag(11), "DisableActionWithDuration", ItemType::kDefault }, 110 }, // Actions::DisableActionWithDuration::DisableActionWithDurationRequest
};

const Entry<ItemInfo> _Actions_ActionStruct[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::char_string
  { { ContextTag(2), "type", ItemType::kEnum }, 117 }, // Actions::ActionTypeEnum
  { { ContextTag(3), "endpointListID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(4), "supportedCommands", ItemType::kBitmap }, 119 }, // Actions::CommandBits
  { { ContextTag(5), "state", ItemType::kEnum }, 116 }, // Actions::ActionStateEnum
};

const Entry<ItemInfo> _Actions_EndpointListStruct[] = {
  { { ContextTag(0), "endpointListID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::char_string
  { { ContextTag(2), "type", ItemType::kEnum }, 118 }, // Actions::EndpointListTypeEnum
  { { ContextTag(3), "endpoints", ItemType::kList }, 1 }, // Actions::endpoint_no[]
};

const Entry<ItemInfo> _Actions_InstantActionRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_InstantActionWithTransitionRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
};

const Entry<ItemInfo> _Actions_StartActionRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_StartActionWithDurationRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
  { { ContextTag(2), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_StopActionRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_PauseActionRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_PauseActionWithDurationRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
  { { ContextTag(2), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_ResumeActionRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_EnableActionRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_EnableActionWithDurationRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
  { { ContextTag(2), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_DisableActionRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_DisableActionWithDurationRequest[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
  { { ContextTag(2), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
};

const Entry<ItemInfo> _Actions_StateChanged[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
  { { ContextTag(2), "newState", ItemType::kEnum }, 116 }, // Actions::ActionStateEnum
};

const Entry<ItemInfo> _Actions_ActionFailed[] = {
  { { ContextTag(0), "actionID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int16u
  { { ContextTag(1), "invokeID", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::int32u
  { { ContextTag(2), "newState", ItemType::kEnum }, 116 }, // Actions::ActionStateEnum
  { { ContextTag(3), "error", ItemType::kEnum }, 115 }, // Actions::ActionErrorEnum
};

const Entry<ItemInfo> _Actions_ActionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 97 }, // Actions_ActionStruct[]
};

const Entry<ItemInfo> _Actions_EndpointListStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 98 }, // Actions_EndpointListStruct[]
};

const Entry<ItemInfo> _Actions_ActionErrorEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionErrorEnum::kUnknown
  { { ConstantValueTag(0x1), "kInterrupted", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionErrorEnum::kInterrupted
};

const Entry<ItemInfo> _Actions_ActionStateEnum[] = {
  { { ConstantValueTag(0x0), "kInactive", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionStateEnum::kInactive
  { { ConstantValueTag(0x1), "kActive", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionStateEnum::kActive
  { { ConstantValueTag(0x2), "kPaused", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionStateEnum::kPaused
  { { ConstantValueTag(0x3), "kDisabled", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionStateEnum::kDisabled
};

const Entry<ItemInfo> _Actions_ActionTypeEnum[] = {
  { { ConstantValueTag(0x0), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionTypeEnum::kOther
  { { ConstantValueTag(0x1), "kScene", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionTypeEnum::kScene
  { { ConstantValueTag(0x2), "kSequence", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionTypeEnum::kSequence
  { { ConstantValueTag(0x3), "kAutomation", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionTypeEnum::kAutomation
  { { ConstantValueTag(0x4), "kException", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionTypeEnum::kException
  { { ConstantValueTag(0x5), "kNotification", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionTypeEnum::kNotification
  { { ConstantValueTag(0x6), "kAlarm", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::ActionTypeEnum::kAlarm
};

const Entry<ItemInfo> _Actions_EndpointListTypeEnum[] = {
  { { ConstantValueTag(0x0), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::EndpointListTypeEnum::kOther
  { { ConstantValueTag(0x1), "kRoom", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::EndpointListTypeEnum::kRoom
  { { ConstantValueTag(0x2), "kZone", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::EndpointListTypeEnum::kZone
};

const Entry<ItemInfo> _Actions_CommandBits[] = {
  { { ConstantValueTag(0x1), "kInstantAction", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kInstantAction
  { { ConstantValueTag(0x2), "kInstantActionWithTransition", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kInstantActionWithTransition
  { { ConstantValueTag(0x4), "kStartAction", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kStartAction
  { { ConstantValueTag(0x8), "kStartActionWithDuration", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kStartActionWithDuration
  { { ConstantValueTag(0x10), "kStopAction", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kStopAction
  { { ConstantValueTag(0x20), "kPauseAction", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kPauseAction
  { { ConstantValueTag(0x40), "kPauseActionWithDuration", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kPauseActionWithDuration
  { { ConstantValueTag(0x80), "kResumeAction", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kResumeAction
  { { ConstantValueTag(0x100), "kEnableAction", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kEnableAction
  { { ConstantValueTag(0x200), "kEnableActionWithDuration", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kEnableActionWithDuration
  { { ConstantValueTag(0x400), "kDisableAction", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kDisableAction
  { { ConstantValueTag(0x800), "kDisableActionWithDuration", ItemType::kDefault }, kInvalidNodeIndex }, // Actions::CommandBits::kDisableActionWithDuration
};

const Entry<ItemInfo> _BasicInformation[] = {
  { { AttributeTag(0), "dataModelRevision", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int16u
  { { AttributeTag(1), "vendorName", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(2), "vendorID", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::vendor_id
  { { AttributeTag(3), "productName", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(4), "productID", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int16u
  { { AttributeTag(5), "nodeLabel", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(6), "location", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(7), "hardwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int16u
  { { AttributeTag(8), "hardwareVersionString", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(9), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int32u
  { { AttributeTag(10), "softwareVersionString", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(11), "manufacturingDate", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(12), "partNumber", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(13), "productURL", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::long_char_string
  { { AttributeTag(14), "productLabel", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(15), "serialNumber", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(16), "localConfigDisabled", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::boolean
  { { AttributeTag(17), "reachable", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::boolean
  { { AttributeTag(18), "uniqueID", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::char_string
  { { AttributeTag(19), "capabilityMinima", ItemType::kDefault }, 121 }, // BasicInformation::CapabilityMinimaStruct
  { { AttributeTag(20), "productAppearance", ItemType::kDefault }, 122 }, // BasicInformation::ProductAppearanceStruct
  { { AttributeTag(21), "specificationVersion", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int32u
  { { AttributeTag(22), "maxPathsPerInvoke", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // BasicInformation::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // BasicInformation::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // BasicInformation::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // BasicInformation::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int16u
  { { EventTag(0), "StartUp", ItemType::kDefault }, 123 }, // BasicInformation::StartUp
  { { EventTag(2), "Leave", ItemType::kDefault }, 124 }, // BasicInformation::Leave
  { { EventTag(3), "ReachableChanged", ItemType::kDefault }, 125 }, // BasicInformation::ReachableChanged
  { { CommandTag(0), "MfgSpecificPing", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::MfgSpecificPing::()
};

const Entry<ItemInfo> _BasicInformation_CapabilityMinimaStruct[] = {
  { { ContextTag(0), "caseSessionsPerFabric", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int16u
  { { ContextTag(1), "subscriptionsPerFabric", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int16u
};

const Entry<ItemInfo> _BasicInformation_ProductAppearanceStruct[] = {
  { { ContextTag(0), "finish", ItemType::kEnum }, 127 }, // BasicInformation::ProductFinishEnum
  { { ContextTag(1), "primaryColor", ItemType::kEnum }, 126 }, // BasicInformation::ColorEnum
};

const Entry<ItemInfo> _BasicInformation_StartUp[] = {
  { { ContextTag(0), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::int32u
};

const Entry<ItemInfo> _BasicInformation_Leave[] = {
  { { ContextTag(0), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::fabric_idx
};

const Entry<ItemInfo> _BasicInformation_ReachableChanged[] = {
  { { ContextTag(0), "reachableNewValue", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::boolean
};

const Entry<ItemInfo> _BasicInformation_ColorEnum[] = {
  { { ConstantValueTag(0x0), "kBlack", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kBlack
  { { ConstantValueTag(0x1), "kNavy", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kNavy
  { { ConstantValueTag(0x2), "kGreen", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kGreen
  { { ConstantValueTag(0x3), "kTeal", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kTeal
  { { ConstantValueTag(0x4), "kMaroon", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kMaroon
  { { ConstantValueTag(0x5), "kPurple", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kPurple
  { { ConstantValueTag(0x6), "kOlive", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kOlive
  { { ConstantValueTag(0x7), "kGray", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kGray
  { { ConstantValueTag(0x8), "kBlue", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kBlue
  { { ConstantValueTag(0x9), "kLime", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kLime
  { { ConstantValueTag(0xA), "kAqua", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kAqua
  { { ConstantValueTag(0xB), "kRed", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kRed
  { { ConstantValueTag(0xC), "kFuchsia", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kFuchsia
  { { ConstantValueTag(0xD), "kYellow", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kYellow
  { { ConstantValueTag(0xE), "kWhite", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kWhite
  { { ConstantValueTag(0xF), "kNickel", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kNickel
  { { ConstantValueTag(0x10), "kChrome", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kChrome
  { { ConstantValueTag(0x11), "kBrass", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kBrass
  { { ConstantValueTag(0x12), "kCopper", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kCopper
  { { ConstantValueTag(0x13), "kSilver", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kSilver
  { { ConstantValueTag(0x14), "kGold", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ColorEnum::kGold
};

const Entry<ItemInfo> _BasicInformation_ProductFinishEnum[] = {
  { { ConstantValueTag(0x0), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ProductFinishEnum::kOther
  { { ConstantValueTag(0x1), "kMatte", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ProductFinishEnum::kMatte
  { { ConstantValueTag(0x2), "kSatin", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ProductFinishEnum::kSatin
  { { ConstantValueTag(0x3), "kPolished", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ProductFinishEnum::kPolished
  { { ConstantValueTag(0x4), "kRugged", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ProductFinishEnum::kRugged
  { { ConstantValueTag(0x5), "kFabric", ItemType::kDefault }, kInvalidNodeIndex }, // BasicInformation::ProductFinishEnum::kFabric
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OtaSoftwareUpdateProvider::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OtaSoftwareUpdateProvider::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OtaSoftwareUpdateProvider::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OtaSoftwareUpdateProvider::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int16u
  { { CommandTag(0), "QueryImage", ItemType::kDefault }, 129 }, // OtaSoftwareUpdateProvider::QueryImage::QueryImageRequest
  { { CommandTag(2), "ApplyUpdateRequest", ItemType::kDefault }, 131 }, // OtaSoftwareUpdateProvider::ApplyUpdateRequest::ApplyUpdateRequestRequest
  { { CommandTag(4), "NotifyUpdateApplied", ItemType::kDefault }, 133 }, // OtaSoftwareUpdateProvider::NotifyUpdateApplied::NotifyUpdateAppliedRequest
  { { CommandTag(1), "QueryImageResponse", ItemType::kDefault }, 130 }, // OtaSoftwareUpdateProvider::QueryImageResponse
  { { CommandTag(3), "ApplyUpdateResponse", ItemType::kDefault }, 132 }, // OtaSoftwareUpdateProvider::ApplyUpdateResponse
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_QueryImageRequest[] = {
  { { ContextTag(0), "vendorID", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::vendor_id
  { { ContextTag(1), "productID", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int16u
  { { ContextTag(2), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int32u
  { { ContextTag(3), "protocolsSupported", ItemType::kList }, 134 }, // OtaSoftwareUpdateProvider::DownloadProtocolEnum[]
  { { ContextTag(4), "hardwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int16u
  { { ContextTag(5), "location", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::char_string
  { { ContextTag(6), "requestorCanConsent", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::boolean
  { { ContextTag(7), "metadataForProvider", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::octet_string
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_QueryImageResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 137 }, // OtaSoftwareUpdateProvider::StatusEnum
  { { ContextTag(1), "delayedActionTime", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int32u
  { { ContextTag(2), "imageURI", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::char_string
  { { ContextTag(3), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int32u
  { { ContextTag(4), "softwareVersionString", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::char_string
  { { ContextTag(5), "updateToken", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::octet_string
  { { ContextTag(6), "userConsentNeeded", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::boolean
  { { ContextTag(7), "metadataForRequestor", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::octet_string
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_ApplyUpdateRequestRequest[] = {
  { { ContextTag(0), "updateToken", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::octet_string
  { { ContextTag(1), "newVersion", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int32u
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_ApplyUpdateResponse[] = {
  { { ContextTag(0), "action", ItemType::kEnum }, 135 }, // OtaSoftwareUpdateProvider::ApplyUpdateActionEnum
  { { ContextTag(1), "delayedActionTime", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int32u
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_NotifyUpdateAppliedRequest[] = {
  { { ContextTag(0), "updateToken", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::octet_string
  { { ContextTag(1), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::int32u
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_DownloadProtocolEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 136 }, // OtaSoftwareUpdateProvider_DownloadProtocolEnum[]
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_ApplyUpdateActionEnum[] = {
  { { ConstantValueTag(0x0), "kProceed", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::ApplyUpdateActionEnum::kProceed
  { { ConstantValueTag(0x1), "kAwaitNextAction", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::ApplyUpdateActionEnum::kAwaitNextAction
  { { ConstantValueTag(0x2), "kDiscontinue", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::ApplyUpdateActionEnum::kDiscontinue
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_DownloadProtocolEnum[] = {
  { { ConstantValueTag(0x0), "kBDXSynchronous", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::DownloadProtocolEnum::kBDXSynchronous
  { { ConstantValueTag(0x1), "kBDXAsynchronous", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::DownloadProtocolEnum::kBDXAsynchronous
  { { ConstantValueTag(0x2), "kHTTPS", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::DownloadProtocolEnum::kHTTPS
  { { ConstantValueTag(0x3), "kVendorSpecific", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::DownloadProtocolEnum::kVendorSpecific
};

const Entry<ItemInfo> _OtaSoftwareUpdateProvider_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kUpdateAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::StatusEnum::kUpdateAvailable
  { { ConstantValueTag(0x1), "kBusy", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::StatusEnum::kBusy
  { { ConstantValueTag(0x2), "kNotAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::StatusEnum::kNotAvailable
  { { ConstantValueTag(0x3), "kDownloadProtocolNotSupported", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateProvider::StatusEnum::kDownloadProtocolNotSupported
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor[] = {
  { { AttributeTag(0), "defaultOTAProviders", ItemType::kList }, 144 }, // OtaSoftwareUpdateRequestor::ProviderLocation[]
  { { AttributeTag(1), "updatePossible", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::boolean
  { { AttributeTag(2), "updateState", ItemType::kEnum }, 147 }, // OtaSoftwareUpdateRequestor::UpdateStateEnum
  { { AttributeTag(3), "updateStateProgress", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OtaSoftwareUpdateRequestor::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OtaSoftwareUpdateRequestor::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OtaSoftwareUpdateRequestor::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OtaSoftwareUpdateRequestor::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int16u
  { { EventTag(0), "StateTransition", ItemType::kDefault }, 141 }, // OtaSoftwareUpdateRequestor::StateTransition
  { { EventTag(1), "VersionApplied", ItemType::kDefault }, 142 }, // OtaSoftwareUpdateRequestor::VersionApplied
  { { EventTag(2), "DownloadError", ItemType::kDefault }, 143 }, // OtaSoftwareUpdateRequestor::DownloadError
  { { CommandTag(0), "AnnounceOTAProvider", ItemType::kDefault }, 140 }, // OtaSoftwareUpdateRequestor::AnnounceOTAProvider::AnnounceOTAProviderRequest
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_ProviderLocation[] = {
  { { ContextTag(1), "providerNodeID", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::node_id
  { { ContextTag(2), "endpoint", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::endpoint_no
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::fabric_idx
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_AnnounceOTAProviderRequest[] = {
  { { ContextTag(0), "providerNodeID", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::node_id
  { { ContextTag(1), "vendorID", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::vendor_id
  { { ContextTag(2), "announcementReason", ItemType::kEnum }, 145 }, // OtaSoftwareUpdateRequestor::AnnouncementReasonEnum
  { { ContextTag(3), "metadataForNode", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::octet_string
  { { ContextTag(4), "endpoint", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::endpoint_no
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_StateTransition[] = {
  { { ContextTag(0), "previousState", ItemType::kEnum }, 147 }, // OtaSoftwareUpdateRequestor::UpdateStateEnum
  { { ContextTag(1), "newState", ItemType::kEnum }, 147 }, // OtaSoftwareUpdateRequestor::UpdateStateEnum
  { { ContextTag(2), "reason", ItemType::kEnum }, 146 }, // OtaSoftwareUpdateRequestor::ChangeReasonEnum
  { { ContextTag(3), "targetSoftwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int32u
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_VersionApplied[] = {
  { { ContextTag(0), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int32u
  { { ContextTag(1), "productID", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int16u
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_DownloadError[] = {
  { { ContextTag(0), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int32u
  { { ContextTag(1), "bytesDownloaded", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int64u
  { { ContextTag(2), "progressPercent", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int8u
  { { ContextTag(3), "platformCode", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::int64s
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_ProviderLocation_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 139 }, // OtaSoftwareUpdateRequestor_ProviderLocation[]
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_AnnouncementReasonEnum[] = {
  { { ConstantValueTag(0x0), "kSimpleAnnouncement", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::AnnouncementReasonEnum::kSimpleAnnouncement
  { { ConstantValueTag(0x1), "kUpdateAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::AnnouncementReasonEnum::kUpdateAvailable
  { { ConstantValueTag(0x2), "kUrgentUpdateAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::AnnouncementReasonEnum::kUrgentUpdateAvailable
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_ChangeReasonEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::ChangeReasonEnum::kUnknown
  { { ConstantValueTag(0x1), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::ChangeReasonEnum::kSuccess
  { { ConstantValueTag(0x2), "kFailure", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::ChangeReasonEnum::kFailure
  { { ConstantValueTag(0x3), "kTimeOut", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::ChangeReasonEnum::kTimeOut
  { { ConstantValueTag(0x4), "kDelayByProvider", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::ChangeReasonEnum::kDelayByProvider
};

const Entry<ItemInfo> _OtaSoftwareUpdateRequestor_UpdateStateEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kUnknown
  { { ConstantValueTag(0x1), "kIdle", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kIdle
  { { ConstantValueTag(0x2), "kQuerying", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kQuerying
  { { ConstantValueTag(0x3), "kDelayedOnQuery", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kDelayedOnQuery
  { { ConstantValueTag(0x4), "kDownloading", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kDownloading
  { { ConstantValueTag(0x5), "kApplying", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kApplying
  { { ConstantValueTag(0x6), "kDelayedOnApply", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kDelayedOnApply
  { { ConstantValueTag(0x7), "kRollingBack", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kRollingBack
  { { ConstantValueTag(0x8), "kDelayedOnUserConsent", ItemType::kDefault }, kInvalidNodeIndex }, // OtaSoftwareUpdateRequestor::UpdateStateEnum::kDelayedOnUserConsent
};

const Entry<ItemInfo> _LocalizationConfiguration[] = {
  { { AttributeTag(0), "activeLocale", ItemType::kDefault }, kInvalidNodeIndex }, // LocalizationConfiguration::char_string
  { { AttributeTag(1), "supportedLocales", ItemType::kList }, 1 }, // LocalizationConfiguration::char_string[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // LocalizationConfiguration::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // LocalizationConfiguration::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // LocalizationConfiguration::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // LocalizationConfiguration::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // LocalizationConfiguration::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // LocalizationConfiguration::int16u
};

const Entry<ItemInfo> _TimeFormatLocalization[] = {
  { { AttributeTag(0), "hourFormat", ItemType::kEnum }, 152 }, // TimeFormatLocalization::HourFormatEnum
  { { AttributeTag(1), "activeCalendarType", ItemType::kEnum }, 151 }, // TimeFormatLocalization::CalendarTypeEnum
  { { AttributeTag(2), "supportedCalendarTypes", ItemType::kList }, 150 }, // TimeFormatLocalization::CalendarTypeEnum[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // TimeFormatLocalization::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // TimeFormatLocalization::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // TimeFormatLocalization::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // TimeFormatLocalization::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 153 }, // TimeFormatLocalization::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::int16u
};

const Entry<ItemInfo> _TimeFormatLocalization_CalendarTypeEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 151 }, // TimeFormatLocalization_CalendarTypeEnum[]
};

const Entry<ItemInfo> _TimeFormatLocalization_CalendarTypeEnum[] = {
  { { ConstantValueTag(0x0), "kBuddhist", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kBuddhist
  { { ConstantValueTag(0x1), "kChinese", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kChinese
  { { ConstantValueTag(0x2), "kCoptic", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kCoptic
  { { ConstantValueTag(0x3), "kEthiopian", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kEthiopian
  { { ConstantValueTag(0x4), "kGregorian", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kGregorian
  { { ConstantValueTag(0x5), "kHebrew", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kHebrew
  { { ConstantValueTag(0x6), "kIndian", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kIndian
  { { ConstantValueTag(0x7), "kIslamic", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kIslamic
  { { ConstantValueTag(0x8), "kJapanese", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kJapanese
  { { ConstantValueTag(0x9), "kKorean", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kKorean
  { { ConstantValueTag(0xA), "kPersian", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kPersian
  { { ConstantValueTag(0xB), "kTaiwanese", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kTaiwanese
  { { ConstantValueTag(0xFF), "kUseActiveLocale", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::CalendarTypeEnum::kUseActiveLocale
};

const Entry<ItemInfo> _TimeFormatLocalization_HourFormatEnum[] = {
  { { ConstantValueTag(0x0), "k12hr", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::HourFormatEnum::k12hr
  { { ConstantValueTag(0x1), "k24hr", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::HourFormatEnum::k24hr
  { { ConstantValueTag(0xFF), "kUseActiveLocale", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::HourFormatEnum::kUseActiveLocale
};

const Entry<ItemInfo> _TimeFormatLocalization_Feature[] = {
  { { ConstantValueTag(0x1), "kCalendarFormat", ItemType::kDefault }, kInvalidNodeIndex }, // TimeFormatLocalization::Feature::kCalendarFormat
};

const Entry<ItemInfo> _UnitLocalization[] = {
  { { AttributeTag(0), "temperatureUnit", ItemType::kEnum }, 155 }, // UnitLocalization::TempUnitEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // UnitLocalization::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // UnitLocalization::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // UnitLocalization::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // UnitLocalization::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 156 }, // UnitLocalization::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // UnitLocalization::int16u
};

const Entry<ItemInfo> _UnitLocalization_TempUnitEnum[] = {
  { { ConstantValueTag(0x0), "kFahrenheit", ItemType::kDefault }, kInvalidNodeIndex }, // UnitLocalization::TempUnitEnum::kFahrenheit
  { { ConstantValueTag(0x1), "kCelsius", ItemType::kDefault }, kInvalidNodeIndex }, // UnitLocalization::TempUnitEnum::kCelsius
  { { ConstantValueTag(0x2), "kKelvin", ItemType::kDefault }, kInvalidNodeIndex }, // UnitLocalization::TempUnitEnum::kKelvin
};

const Entry<ItemInfo> _UnitLocalization_Feature[] = {
  { { ConstantValueTag(0x1), "kTemperatureUnit", ItemType::kDefault }, kInvalidNodeIndex }, // UnitLocalization::Feature::kTemperatureUnit
};

const Entry<ItemInfo> _PowerSourceConfiguration[] = {
  { { AttributeTag(0), "sources", ItemType::kList }, 1 }, // PowerSourceConfiguration::endpoint_no[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // PowerSourceConfiguration::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // PowerSourceConfiguration::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // PowerSourceConfiguration::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // PowerSourceConfiguration::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSourceConfiguration::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSourceConfiguration::int16u
};

const Entry<ItemInfo> _PowerSource[] = {
  { { AttributeTag(0), "status", ItemType::kEnum }, 175 }, // PowerSource::PowerSourceStatusEnum
  { { AttributeTag(1), "order", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int8u
  { { AttributeTag(2), "description", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::char_string
  { { AttributeTag(3), "wiredAssessedInputVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(4), "wiredAssessedInputFrequency", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int16u
  { { AttributeTag(5), "wiredCurrentType", ItemType::kEnum }, 176 }, // PowerSource::WiredCurrentTypeEnum
  { { AttributeTag(6), "wiredAssessedCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(7), "wiredNominalVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(8), "wiredMaximumCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(9), "wiredPresent", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::boolean
  { { AttributeTag(10), "activeWiredFaults", ItemType::kList }, 166 }, // PowerSource::WiredFaultEnum[]
  { { AttributeTag(11), "batVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(12), "batPercentRemaining", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int8u
  { { AttributeTag(13), "batTimeRemaining", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(14), "batChargeLevel", ItemType::kEnum }, 170 }, // PowerSource::BatChargeLevelEnum
  { { AttributeTag(15), "batReplacementNeeded", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::boolean
  { { AttributeTag(16), "batReplaceability", ItemType::kEnum }, 174 }, // PowerSource::BatReplaceabilityEnum
  { { AttributeTag(17), "batPresent", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::boolean
  { { AttributeTag(18), "activeBatFaults", ItemType::kList }, 167 }, // PowerSource::BatFaultEnum[]
  { { AttributeTag(19), "batReplacementDescription", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::char_string
  { { AttributeTag(20), "batCommonDesignation", ItemType::kEnum }, 172 }, // PowerSource::BatCommonDesignationEnum
  { { AttributeTag(21), "batANSIDesignation", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::char_string
  { { AttributeTag(22), "batIECDesignation", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::char_string
  { { AttributeTag(23), "batApprovedChemistry", ItemType::kEnum }, 168 }, // PowerSource::BatApprovedChemistryEnum
  { { AttributeTag(24), "batCapacity", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(25), "batQuantity", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int8u
  { { AttributeTag(26), "batChargeState", ItemType::kEnum }, 171 }, // PowerSource::BatChargeStateEnum
  { { AttributeTag(27), "batTimeToFullCharge", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(28), "batFunctionalWhileCharging", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::boolean
  { { AttributeTag(29), "batChargingCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int32u
  { { AttributeTag(30), "activeBatChargeFaults", ItemType::kList }, 165 }, // PowerSource::BatChargeFaultEnum[]
  { { AttributeTag(31), "endpointList", ItemType::kList }, 1 }, // PowerSource::endpoint_no[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // PowerSource::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // PowerSource::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // PowerSource::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // PowerSource::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 178 }, // PowerSource::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::int16u
  { { EventTag(0), "WiredFaultChange", ItemType::kDefault }, 162 }, // PowerSource::WiredFaultChange
  { { EventTag(1), "BatFaultChange", ItemType::kDefault }, 163 }, // PowerSource::BatFaultChange
  { { EventTag(2), "BatChargeFaultChange", ItemType::kDefault }, 164 }, // PowerSource::BatChargeFaultChange
};

const Entry<ItemInfo> _PowerSource_BatChargeFaultChangeType[] = {
  { { ContextTag(0), "current", ItemType::kList }, 165 }, // PowerSource::BatChargeFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 165 }, // PowerSource::BatChargeFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_BatFaultChangeType[] = {
  { { ContextTag(0), "current", ItemType::kList }, 167 }, // PowerSource::BatFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 167 }, // PowerSource::BatFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_WiredFaultChangeType[] = {
  { { ContextTag(0), "current", ItemType::kList }, 166 }, // PowerSource::WiredFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 166 }, // PowerSource::WiredFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_WiredFaultChange[] = {
  { { ContextTag(0), "current", ItemType::kList }, 166 }, // PowerSource::WiredFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 166 }, // PowerSource::WiredFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_BatFaultChange[] = {
  { { ContextTag(0), "current", ItemType::kList }, 167 }, // PowerSource::BatFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 167 }, // PowerSource::BatFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_BatChargeFaultChange[] = {
  { { ContextTag(0), "current", ItemType::kList }, 165 }, // PowerSource::BatChargeFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 165 }, // PowerSource::BatChargeFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_BatChargeFaultEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 169 }, // PowerSource_BatChargeFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_WiredFaultEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 177 }, // PowerSource_WiredFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_BatFaultEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 173 }, // PowerSource_BatFaultEnum[]
};

const Entry<ItemInfo> _PowerSource_BatApprovedChemistryEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kUnspecified
  { { ConstantValueTag(0x1), "kAlkaline", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kAlkaline
  { { ConstantValueTag(0x2), "kLithiumCarbonFluoride", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumCarbonFluoride
  { { ConstantValueTag(0x3), "kLithiumChromiumOxide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumChromiumOxide
  { { ConstantValueTag(0x4), "kLithiumCopperOxide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumCopperOxide
  { { ConstantValueTag(0x5), "kLithiumIronDisulfide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumIronDisulfide
  { { ConstantValueTag(0x6), "kLithiumManganeseDioxide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumManganeseDioxide
  { { ConstantValueTag(0x7), "kLithiumThionylChloride", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumThionylChloride
  { { ConstantValueTag(0x8), "kMagnesium", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kMagnesium
  { { ConstantValueTag(0x9), "kMercuryOxide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kMercuryOxide
  { { ConstantValueTag(0xA), "kNickelOxyhydride", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kNickelOxyhydride
  { { ConstantValueTag(0xB), "kSilverOxide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kSilverOxide
  { { ConstantValueTag(0xC), "kZincAir", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kZincAir
  { { ConstantValueTag(0xD), "kZincCarbon", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kZincCarbon
  { { ConstantValueTag(0xE), "kZincChloride", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kZincChloride
  { { ConstantValueTag(0xF), "kZincManganeseDioxide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kZincManganeseDioxide
  { { ConstantValueTag(0x10), "kLeadAcid", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLeadAcid
  { { ConstantValueTag(0x11), "kLithiumCobaltOxide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumCobaltOxide
  { { ConstantValueTag(0x12), "kLithiumIon", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumIon
  { { ConstantValueTag(0x13), "kLithiumIonPolymer", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumIonPolymer
  { { ConstantValueTag(0x14), "kLithiumIronPhosphate", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumIronPhosphate
  { { ConstantValueTag(0x15), "kLithiumSulfur", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumSulfur
  { { ConstantValueTag(0x16), "kLithiumTitanate", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kLithiumTitanate
  { { ConstantValueTag(0x17), "kNickelCadmium", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kNickelCadmium
  { { ConstantValueTag(0x18), "kNickelHydrogen", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kNickelHydrogen
  { { ConstantValueTag(0x19), "kNickelIron", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kNickelIron
  { { ConstantValueTag(0x1A), "kNickelMetalHydride", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kNickelMetalHydride
  { { ConstantValueTag(0x1B), "kNickelZinc", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kNickelZinc
  { { ConstantValueTag(0x1C), "kSilverZinc", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kSilverZinc
  { { ConstantValueTag(0x1D), "kSodiumIon", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kSodiumIon
  { { ConstantValueTag(0x1E), "kSodiumSulfur", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kSodiumSulfur
  { { ConstantValueTag(0x1F), "kZincBromide", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kZincBromide
  { { ConstantValueTag(0x20), "kZincCerium", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatApprovedChemistryEnum::kZincCerium
};

const Entry<ItemInfo> _PowerSource_BatChargeFaultEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kUnspecified
  { { ConstantValueTag(0x1), "kAmbientTooHot", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kAmbientTooHot
  { { ConstantValueTag(0x2), "kAmbientTooCold", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kAmbientTooCold
  { { ConstantValueTag(0x3), "kBatteryTooHot", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kBatteryTooHot
  { { ConstantValueTag(0x4), "kBatteryTooCold", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kBatteryTooCold
  { { ConstantValueTag(0x5), "kBatteryAbsent", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kBatteryAbsent
  { { ConstantValueTag(0x6), "kBatteryOverVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kBatteryOverVoltage
  { { ConstantValueTag(0x7), "kBatteryUnderVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kBatteryUnderVoltage
  { { ConstantValueTag(0x8), "kChargerOverVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kChargerOverVoltage
  { { ConstantValueTag(0x9), "kChargerUnderVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kChargerUnderVoltage
  { { ConstantValueTag(0xA), "kSafetyTimeout", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeFaultEnum::kSafetyTimeout
};

const Entry<ItemInfo> _PowerSource_BatChargeLevelEnum[] = {
  { { ConstantValueTag(0x0), "kOK", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeLevelEnum::kOK
  { { ConstantValueTag(0x1), "kWarning", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeLevelEnum::kWarning
  { { ConstantValueTag(0x2), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeLevelEnum::kCritical
};

const Entry<ItemInfo> _PowerSource_BatChargeStateEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeStateEnum::kUnknown
  { { ConstantValueTag(0x1), "kIsCharging", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeStateEnum::kIsCharging
  { { ConstantValueTag(0x2), "kIsAtFullCharge", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeStateEnum::kIsAtFullCharge
  { { ConstantValueTag(0x3), "kIsNotCharging", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatChargeStateEnum::kIsNotCharging
};

const Entry<ItemInfo> _PowerSource_BatCommonDesignationEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kUnspecified
  { { ConstantValueTag(0x1), "kAAA", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kAAA
  { { ConstantValueTag(0x2), "kAA", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kAA
  { { ConstantValueTag(0x3), "kC", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kC
  { { ConstantValueTag(0x4), "kD", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kD
  { { ConstantValueTag(0x5), "k4v5", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k4v5
  { { ConstantValueTag(0x6), "k6v0", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k6v0
  { { ConstantValueTag(0x7), "k9v0", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k9v0
  { { ConstantValueTag(0x8), "k12AA", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k12AA
  { { ConstantValueTag(0x9), "kAAAA", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kAAAA
  { { ConstantValueTag(0xA), "kA", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kA
  { { ConstantValueTag(0xB), "kB", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kB
  { { ConstantValueTag(0xC), "kF", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kF
  { { ConstantValueTag(0xD), "kN", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kN
  { { ConstantValueTag(0xE), "kNo6", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kNo6
  { { ConstantValueTag(0xF), "kSubC", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSubC
  { { ConstantValueTag(0x10), "kA23", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kA23
  { { ConstantValueTag(0x11), "kA27", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kA27
  { { ConstantValueTag(0x12), "kBA5800", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kBA5800
  { { ConstantValueTag(0x13), "kDuplex", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kDuplex
  { { ConstantValueTag(0x14), "k4SR44", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k4SR44
  { { ConstantValueTag(0x15), "k523", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k523
  { { ConstantValueTag(0x16), "k531", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k531
  { { ConstantValueTag(0x17), "k15v0", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k15v0
  { { ConstantValueTag(0x18), "k22v5", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k22v5
  { { ConstantValueTag(0x19), "k30v0", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k30v0
  { { ConstantValueTag(0x1A), "k45v0", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k45v0
  { { ConstantValueTag(0x1B), "k67v5", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k67v5
  { { ConstantValueTag(0x1C), "kJ", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kJ
  { { ConstantValueTag(0x1D), "kCR123A", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kCR123A
  { { ConstantValueTag(0x1E), "kCR2", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kCR2
  { { ConstantValueTag(0x1F), "k2CR5", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k2CR5
  { { ConstantValueTag(0x20), "kCRP2", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kCRP2
  { { ConstantValueTag(0x21), "kCRV3", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kCRV3
  { { ConstantValueTag(0x22), "kSR41", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR41
  { { ConstantValueTag(0x23), "kSR43", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR43
  { { ConstantValueTag(0x24), "kSR44", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR44
  { { ConstantValueTag(0x25), "kSR45", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR45
  { { ConstantValueTag(0x26), "kSR48", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR48
  { { ConstantValueTag(0x27), "kSR54", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR54
  { { ConstantValueTag(0x28), "kSR55", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR55
  { { ConstantValueTag(0x29), "kSR57", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR57
  { { ConstantValueTag(0x2A), "kSR58", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR58
  { { ConstantValueTag(0x2B), "kSR59", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR59
  { { ConstantValueTag(0x2C), "kSR60", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR60
  { { ConstantValueTag(0x2D), "kSR63", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR63
  { { ConstantValueTag(0x2E), "kSR64", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR64
  { { ConstantValueTag(0x2F), "kSR65", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR65
  { { ConstantValueTag(0x30), "kSR66", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR66
  { { ConstantValueTag(0x31), "kSR67", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR67
  { { ConstantValueTag(0x32), "kSR68", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR68
  { { ConstantValueTag(0x33), "kSR69", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR69
  { { ConstantValueTag(0x34), "kSR516", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR516
  { { ConstantValueTag(0x35), "kSR731", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR731
  { { ConstantValueTag(0x36), "kSR712", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kSR712
  { { ConstantValueTag(0x37), "kLR932", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kLR932
  { { ConstantValueTag(0x38), "kA5", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kA5
  { { ConstantValueTag(0x39), "kA10", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kA10
  { { ConstantValueTag(0x3A), "kA13", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kA13
  { { ConstantValueTag(0x3B), "kA312", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kA312
  { { ConstantValueTag(0x3C), "kA675", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kA675
  { { ConstantValueTag(0x3D), "kAC41E", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kAC41E
  { { ConstantValueTag(0x3E), "k10180", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k10180
  { { ConstantValueTag(0x3F), "k10280", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k10280
  { { ConstantValueTag(0x40), "k10440", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k10440
  { { ConstantValueTag(0x41), "k14250", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k14250
  { { ConstantValueTag(0x42), "k14430", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k14430
  { { ConstantValueTag(0x43), "k14500", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k14500
  { { ConstantValueTag(0x44), "k14650", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k14650
  { { ConstantValueTag(0x45), "k15270", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k15270
  { { ConstantValueTag(0x46), "k16340", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k16340
  { { ConstantValueTag(0x47), "kRCR123A", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::kRCR123A
  { { ConstantValueTag(0x48), "k17500", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k17500
  { { ConstantValueTag(0x49), "k17670", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k17670
  { { ConstantValueTag(0x4A), "k18350", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k18350
  { { ConstantValueTag(0x4B), "k18500", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k18500
  { { ConstantValueTag(0x4C), "k18650", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k18650
  { { ConstantValueTag(0x4D), "k19670", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k19670
  { { ConstantValueTag(0x4E), "k25500", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k25500
  { { ConstantValueTag(0x4F), "k26650", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k26650
  { { ConstantValueTag(0x50), "k32600", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatCommonDesignationEnum::k32600
};

const Entry<ItemInfo> _PowerSource_BatFaultEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatFaultEnum::kUnspecified
  { { ConstantValueTag(0x1), "kOverTemp", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatFaultEnum::kOverTemp
  { { ConstantValueTag(0x2), "kUnderTemp", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatFaultEnum::kUnderTemp
};

const Entry<ItemInfo> _PowerSource_BatReplaceabilityEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatReplaceabilityEnum::kUnspecified
  { { ConstantValueTag(0x1), "kNotReplaceable", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatReplaceabilityEnum::kNotReplaceable
  { { ConstantValueTag(0x2), "kUserReplaceable", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatReplaceabilityEnum::kUserReplaceable
  { { ConstantValueTag(0x3), "kFactoryReplaceable", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::BatReplaceabilityEnum::kFactoryReplaceable
};

const Entry<ItemInfo> _PowerSource_PowerSourceStatusEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::PowerSourceStatusEnum::kUnspecified
  { { ConstantValueTag(0x1), "kActive", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::PowerSourceStatusEnum::kActive
  { { ConstantValueTag(0x2), "kStandby", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::PowerSourceStatusEnum::kStandby
  { { ConstantValueTag(0x3), "kUnavailable", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::PowerSourceStatusEnum::kUnavailable
};

const Entry<ItemInfo> _PowerSource_WiredCurrentTypeEnum[] = {
  { { ConstantValueTag(0x0), "kAC", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::WiredCurrentTypeEnum::kAC
  { { ConstantValueTag(0x1), "kDC", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::WiredCurrentTypeEnum::kDC
};

const Entry<ItemInfo> _PowerSource_WiredFaultEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::WiredFaultEnum::kUnspecified
  { { ConstantValueTag(0x1), "kOverVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::WiredFaultEnum::kOverVoltage
  { { ConstantValueTag(0x2), "kUnderVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::WiredFaultEnum::kUnderVoltage
};

const Entry<ItemInfo> _PowerSource_Feature[] = {
  { { ConstantValueTag(0x1), "kWired", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::Feature::kWired
  { { ConstantValueTag(0x2), "kBattery", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::Feature::kBattery
  { { ConstantValueTag(0x4), "kRechargeable", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::Feature::kRechargeable
  { { ConstantValueTag(0x8), "kReplaceable", ItemType::kDefault }, kInvalidNodeIndex }, // PowerSource::Feature::kReplaceable
};

const Entry<ItemInfo> _GeneralCommissioning[] = {
  { { AttributeTag(0), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::int64u
  { { AttributeTag(1), "basicCommissioningInfo", ItemType::kDefault }, 180 }, // GeneralCommissioning::BasicCommissioningInfo
  { { AttributeTag(2), "regulatoryConfig", ItemType::kEnum }, 187 }, // GeneralCommissioning::RegulatoryLocationTypeEnum
  { { AttributeTag(3), "locationCapability", ItemType::kEnum }, 187 }, // GeneralCommissioning::RegulatoryLocationTypeEnum
  { { AttributeTag(4), "supportsConcurrentConnection", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::boolean
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // GeneralCommissioning::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // GeneralCommissioning::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // GeneralCommissioning::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // GeneralCommissioning::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::int16u
  { { CommandTag(0), "ArmFailSafe", ItemType::kDefault }, 181 }, // GeneralCommissioning::ArmFailSafe::ArmFailSafeRequest
  { { CommandTag(2), "SetRegulatoryConfig", ItemType::kDefault }, 183 }, // GeneralCommissioning::SetRegulatoryConfig::SetRegulatoryConfigRequest
  { { CommandTag(4), "CommissioningComplete", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::CommissioningComplete::()
  { { CommandTag(1), "ArmFailSafeResponse", ItemType::kDefault }, 182 }, // GeneralCommissioning::ArmFailSafeResponse
  { { CommandTag(3), "SetRegulatoryConfigResponse", ItemType::kDefault }, 184 }, // GeneralCommissioning::SetRegulatoryConfigResponse
  { { CommandTag(5), "CommissioningCompleteResponse", ItemType::kDefault }, 185 }, // GeneralCommissioning::CommissioningCompleteResponse
};

const Entry<ItemInfo> _GeneralCommissioning_BasicCommissioningInfo[] = {
  { { ContextTag(0), "failSafeExpiryLengthSeconds", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::int16u
  { { ContextTag(1), "maxCumulativeFailsafeSeconds", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::int16u
};

const Entry<ItemInfo> _GeneralCommissioning_ArmFailSafeRequest[] = {
  { { ContextTag(0), "expiryLengthSeconds", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::int16u
  { { ContextTag(1), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::int64u
};

const Entry<ItemInfo> _GeneralCommissioning_ArmFailSafeResponse[] = {
  { { ContextTag(0), "errorCode", ItemType::kEnum }, 186 }, // GeneralCommissioning::CommissioningErrorEnum
  { { ContextTag(1), "debugText", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::char_string
};

const Entry<ItemInfo> _GeneralCommissioning_SetRegulatoryConfigRequest[] = {
  { { ContextTag(0), "newRegulatoryConfig", ItemType::kEnum }, 187 }, // GeneralCommissioning::RegulatoryLocationTypeEnum
  { { ContextTag(1), "countryCode", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::char_string
  { { ContextTag(2), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::int64u
};

const Entry<ItemInfo> _GeneralCommissioning_SetRegulatoryConfigResponse[] = {
  { { ContextTag(0), "errorCode", ItemType::kEnum }, 186 }, // GeneralCommissioning::CommissioningErrorEnum
  { { ContextTag(1), "debugText", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::char_string
};

const Entry<ItemInfo> _GeneralCommissioning_CommissioningCompleteResponse[] = {
  { { ContextTag(0), "errorCode", ItemType::kEnum }, 186 }, // GeneralCommissioning::CommissioningErrorEnum
  { { ContextTag(1), "debugText", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::char_string
};

const Entry<ItemInfo> _GeneralCommissioning_CommissioningErrorEnum[] = {
  { { ConstantValueTag(0x0), "kOK", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::CommissioningErrorEnum::kOK
  { { ConstantValueTag(0x1), "kValueOutsideRange", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::CommissioningErrorEnum::kValueOutsideRange
  { { ConstantValueTag(0x2), "kInvalidAuthentication", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::CommissioningErrorEnum::kInvalidAuthentication
  { { ConstantValueTag(0x3), "kNoFailSafe", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::CommissioningErrorEnum::kNoFailSafe
  { { ConstantValueTag(0x4), "kBusyWithOtherAdmin", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::CommissioningErrorEnum::kBusyWithOtherAdmin
};

const Entry<ItemInfo> _GeneralCommissioning_RegulatoryLocationTypeEnum[] = {
  { { ConstantValueTag(0x0), "kIndoor", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::RegulatoryLocationTypeEnum::kIndoor
  { { ConstantValueTag(0x1), "kOutdoor", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::RegulatoryLocationTypeEnum::kOutdoor
  { { ConstantValueTag(0x2), "kIndoorOutdoor", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralCommissioning::RegulatoryLocationTypeEnum::kIndoorOutdoor
};

const Entry<ItemInfo> _NetworkCommissioning[] = {
  { { AttributeTag(0), "maxNetworks", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8u
  { { AttributeTag(1), "networks", ItemType::kList }, 205 }, // NetworkCommissioning::NetworkInfoStruct[]
  { { AttributeTag(2), "scanMaxTimeSeconds", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8u
  { { AttributeTag(3), "connectMaxTimeSeconds", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8u
  { { AttributeTag(4), "interfaceEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::boolean
  { { AttributeTag(5), "lastNetworkingStatus", ItemType::kEnum }, 207 }, // NetworkCommissioning::NetworkCommissioningStatusEnum
  { { AttributeTag(6), "lastNetworkID", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { AttributeTag(7), "lastConnectErrorValue", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int32s
  { { AttributeTag(8), "supportedWiFiBands", ItemType::kList }, 204 }, // NetworkCommissioning::WiFiBandEnum[]
  { { AttributeTag(9), "supportedThreadFeatures", ItemType::kBitmap }, 210 }, // NetworkCommissioning::ThreadCapabilitiesBitmap
  { { AttributeTag(10), "threadVersion", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // NetworkCommissioning::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // NetworkCommissioning::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // NetworkCommissioning::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // NetworkCommissioning::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 209 }, // NetworkCommissioning::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int16u
  { { CommandTag(0), "ScanNetworks", ItemType::kDefault }, 192 }, // NetworkCommissioning::ScanNetworks::ScanNetworksRequest
  { { CommandTag(2), "AddOrUpdateWiFiNetwork", ItemType::kDefault }, 194 }, // NetworkCommissioning::AddOrUpdateWiFiNetwork::AddOrUpdateWiFiNetworkRequest
  { { CommandTag(3), "AddOrUpdateThreadNetwork", ItemType::kDefault }, 195 }, // NetworkCommissioning::AddOrUpdateThreadNetwork::AddOrUpdateThreadNetworkRequest
  { { CommandTag(4), "RemoveNetwork", ItemType::kDefault }, 196 }, // NetworkCommissioning::RemoveNetwork::RemoveNetworkRequest
  { { CommandTag(6), "ConnectNetwork", ItemType::kDefault }, 198 }, // NetworkCommissioning::ConnectNetwork::ConnectNetworkRequest
  { { CommandTag(8), "ReorderNetwork", ItemType::kDefault }, 200 }, // NetworkCommissioning::ReorderNetwork::ReorderNetworkRequest
  { { CommandTag(9), "QueryIdentity", ItemType::kDefault }, 201 }, // NetworkCommissioning::QueryIdentity::QueryIdentityRequest
  { { CommandTag(1), "ScanNetworksResponse", ItemType::kDefault }, 193 }, // NetworkCommissioning::ScanNetworksResponse
  { { CommandTag(5), "NetworkConfigResponse", ItemType::kDefault }, 197 }, // NetworkCommissioning::NetworkConfigResponse
  { { CommandTag(7), "ConnectNetworkResponse", ItemType::kDefault }, 199 }, // NetworkCommissioning::ConnectNetworkResponse
  { { CommandTag(10), "QueryIdentityResponse", ItemType::kDefault }, 202 }, // NetworkCommissioning::QueryIdentityResponse
};

const Entry<ItemInfo> _NetworkCommissioning_NetworkInfoStruct[] = {
  { { ContextTag(0), "networkID", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "connected", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::boolean
  { { ContextTag(2), "networkIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(3), "clientIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
};

const Entry<ItemInfo> _NetworkCommissioning_ThreadInterfaceScanResultStruct[] = {
  { { ContextTag(0), "panId", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int16u
  { { ContextTag(1), "extendedPanId", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int64u
  { { ContextTag(2), "networkName", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::char_string
  { { ContextTag(3), "channel", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int16u
  { { ContextTag(4), "version", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8u
  { { ContextTag(5), "extendedAddress", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(6), "rssi", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8s
  { { ContextTag(7), "lqi", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8u
};

const Entry<ItemInfo> _NetworkCommissioning_WiFiInterfaceScanResultStruct[] = {
  { { ContextTag(0), "security", ItemType::kBitmap }, 211 }, // NetworkCommissioning::WiFiSecurityBitmap
  { { ContextTag(1), "ssid", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(2), "bssid", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(3), "channel", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int16u
  { { ContextTag(4), "wiFiBand", ItemType::kEnum }, 208 }, // NetworkCommissioning::WiFiBandEnum
  { { ContextTag(5), "rssi", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8s
};

const Entry<ItemInfo> _NetworkCommissioning_ScanNetworksRequest[] = {
  { { ContextTag(0), "ssid", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int64u
};

const Entry<ItemInfo> _NetworkCommissioning_ScanNetworksResponse[] = {
  { { ContextTag(0), "networkingStatus", ItemType::kEnum }, 207 }, // NetworkCommissioning::NetworkCommissioningStatusEnum
  { { ContextTag(1), "debugText", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::char_string
  { { ContextTag(2), "wiFiScanResults", ItemType::kList }, 203 }, // NetworkCommissioning::WiFiInterfaceScanResultStruct[]
  { { ContextTag(3), "threadScanResults", ItemType::kList }, 206 }, // NetworkCommissioning::ThreadInterfaceScanResultStruct[]
};

const Entry<ItemInfo> _NetworkCommissioning_AddOrUpdateWiFiNetworkRequest[] = {
  { { ContextTag(0), "ssid", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "credentials", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(2), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int64u
  { { ContextTag(3), "networkIdentity", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(4), "clientIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(5), "possessionNonce", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
};

const Entry<ItemInfo> _NetworkCommissioning_AddOrUpdateThreadNetworkRequest[] = {
  { { ContextTag(0), "operationalDataset", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int64u
};

const Entry<ItemInfo> _NetworkCommissioning_RemoveNetworkRequest[] = {
  { { ContextTag(0), "networkID", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int64u
};

const Entry<ItemInfo> _NetworkCommissioning_NetworkConfigResponse[] = {
  { { ContextTag(0), "networkingStatus", ItemType::kEnum }, 207 }, // NetworkCommissioning::NetworkCommissioningStatusEnum
  { { ContextTag(1), "debugText", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::char_string
  { { ContextTag(2), "networkIndex", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8u
  { { ContextTag(3), "clientIdentity", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(4), "possessionSignature", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
};

const Entry<ItemInfo> _NetworkCommissioning_ConnectNetworkRequest[] = {
  { { ContextTag(0), "networkID", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int64u
};

const Entry<ItemInfo> _NetworkCommissioning_ConnectNetworkResponse[] = {
  { { ContextTag(0), "networkingStatus", ItemType::kEnum }, 207 }, // NetworkCommissioning::NetworkCommissioningStatusEnum
  { { ContextTag(1), "debugText", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::char_string
  { { ContextTag(2), "errorValue", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int32s
};

const Entry<ItemInfo> _NetworkCommissioning_ReorderNetworkRequest[] = {
  { { ContextTag(0), "networkID", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "networkIndex", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int8u
  { { ContextTag(2), "breadcrumb", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::int64u
};

const Entry<ItemInfo> _NetworkCommissioning_QueryIdentityRequest[] = {
  { { ContextTag(0), "keyIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "possessionNonce", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
};

const Entry<ItemInfo> _NetworkCommissioning_QueryIdentityResponse[] = {
  { { ContextTag(0), "identity", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
  { { ContextTag(1), "possessionSignature", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::octet_string
};

const Entry<ItemInfo> _NetworkCommissioning_WiFiInterfaceScanResultStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 191 }, // NetworkCommissioning_WiFiInterfaceScanResultStruct[]
};

const Entry<ItemInfo> _NetworkCommissioning_WiFiBandEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 208 }, // NetworkCommissioning_WiFiBandEnum[]
};

const Entry<ItemInfo> _NetworkCommissioning_NetworkInfoStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 189 }, // NetworkCommissioning_NetworkInfoStruct[]
};

const Entry<ItemInfo> _NetworkCommissioning_ThreadInterfaceScanResultStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 190 }, // NetworkCommissioning_ThreadInterfaceScanResultStruct[]
};

const Entry<ItemInfo> _NetworkCommissioning_NetworkCommissioningStatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kOutOfRange", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kOutOfRange
  { { ConstantValueTag(0x2), "kBoundsExceeded", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kBoundsExceeded
  { { ConstantValueTag(0x3), "kNetworkIDNotFound", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kNetworkIDNotFound
  { { ConstantValueTag(0x4), "kDuplicateNetworkID", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kDuplicateNetworkID
  { { ConstantValueTag(0x5), "kNetworkNotFound", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kNetworkNotFound
  { { ConstantValueTag(0x6), "kRegulatoryError", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kRegulatoryError
  { { ConstantValueTag(0x7), "kAuthFailure", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kAuthFailure
  { { ConstantValueTag(0x8), "kUnsupportedSecurity", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kUnsupportedSecurity
  { { ConstantValueTag(0x9), "kOtherConnectionFailure", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kOtherConnectionFailure
  { { ConstantValueTag(0xA), "kIPV6Failed", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kIPV6Failed
  { { ConstantValueTag(0xB), "kIPBindFailed", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kIPBindFailed
  { { ConstantValueTag(0xC), "kUnknownError", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::NetworkCommissioningStatusEnum::kUnknownError
};

const Entry<ItemInfo> _NetworkCommissioning_WiFiBandEnum[] = {
  { { ConstantValueTag(0x0), "k2G4", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiBandEnum::k2G4
  { { ConstantValueTag(0x1), "k3G65", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiBandEnum::k3G65
  { { ConstantValueTag(0x2), "k5G", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiBandEnum::k5G
  { { ConstantValueTag(0x3), "k6G", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiBandEnum::k6G
  { { ConstantValueTag(0x4), "k60G", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiBandEnum::k60G
  { { ConstantValueTag(0x5), "k1G", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiBandEnum::k1G
};

const Entry<ItemInfo> _NetworkCommissioning_Feature[] = {
  { { ConstantValueTag(0x1), "kWiFiNetworkInterface", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::Feature::kWiFiNetworkInterface
  { { ConstantValueTag(0x2), "kThreadNetworkInterface", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::Feature::kThreadNetworkInterface
  { { ConstantValueTag(0x4), "kEthernetNetworkInterface", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::Feature::kEthernetNetworkInterface
  { { ConstantValueTag(0x8), "kPerDeviceCredentials", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::Feature::kPerDeviceCredentials
};

const Entry<ItemInfo> _NetworkCommissioning_ThreadCapabilitiesBitmap[] = {
  { { ConstantValueTag(0x1), "kIsBorderRouterCapable", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::ThreadCapabilitiesBitmap::kIsBorderRouterCapable
  { { ConstantValueTag(0x2), "kIsRouterCapable", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::ThreadCapabilitiesBitmap::kIsRouterCapable
  { { ConstantValueTag(0x4), "kIsSleepyEndDeviceCapable", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::ThreadCapabilitiesBitmap::kIsSleepyEndDeviceCapable
  { { ConstantValueTag(0x8), "kIsFullThreadDevice", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::ThreadCapabilitiesBitmap::kIsFullThreadDevice
  { { ConstantValueTag(0x10), "kIsSynchronizedSleepyEndDeviceCapable", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::ThreadCapabilitiesBitmap::kIsSynchronizedSleepyEndDeviceCapable
};

const Entry<ItemInfo> _NetworkCommissioning_WiFiSecurityBitmap[] = {
  { { ConstantValueTag(0x1), "kUnencrypted", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiSecurityBitmap::kUnencrypted
  { { ConstantValueTag(0x2), "kWEP", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiSecurityBitmap::kWEP
  { { ConstantValueTag(0x4), "kWPAPersonal", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiSecurityBitmap::kWPAPersonal
  { { ConstantValueTag(0x8), "kWPA2Personal", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiSecurityBitmap::kWPA2Personal
  { { ConstantValueTag(0x10), "kWPA3Personal", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiSecurityBitmap::kWPA3Personal
  { { ConstantValueTag(0x20), "kWPA3MatterPDC", ItemType::kDefault }, kInvalidNodeIndex }, // NetworkCommissioning::WiFiSecurityBitmap::kWPA3MatterPDC
};

const Entry<ItemInfo> _DiagnosticLogs[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // DiagnosticLogs::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // DiagnosticLogs::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // DiagnosticLogs::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // DiagnosticLogs::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::int16u
  { { CommandTag(0), "RetrieveLogsRequest", ItemType::kDefault }, 213 }, // DiagnosticLogs::RetrieveLogsRequest::RetrieveLogsRequestRequest
  { { CommandTag(1), "RetrieveLogsResponse", ItemType::kDefault }, 214 }, // DiagnosticLogs::RetrieveLogsResponse
};

const Entry<ItemInfo> _DiagnosticLogs_RetrieveLogsRequestRequest[] = {
  { { ContextTag(0), "intent", ItemType::kEnum }, 215 }, // DiagnosticLogs::IntentEnum
  { { ContextTag(1), "requestedProtocol", ItemType::kEnum }, 217 }, // DiagnosticLogs::TransferProtocolEnum
  { { ContextTag(2), "transferFileDesignator", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::char_string
};

const Entry<ItemInfo> _DiagnosticLogs_RetrieveLogsResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 216 }, // DiagnosticLogs::StatusEnum
  { { ContextTag(1), "logContent", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::long_octet_string
  { { ContextTag(2), "UTCTimeStamp", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::epoch_us
  { { ContextTag(3), "timeSinceBoot", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::systime_us
};

const Entry<ItemInfo> _DiagnosticLogs_IntentEnum[] = {
  { { ConstantValueTag(0x0), "kEndUserSupport", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::IntentEnum::kEndUserSupport
  { { ConstantValueTag(0x1), "kNetworkDiag", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::IntentEnum::kNetworkDiag
  { { ConstantValueTag(0x2), "kCrashLogs", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::IntentEnum::kCrashLogs
};

const Entry<ItemInfo> _DiagnosticLogs_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::StatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kExhausted", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::StatusEnum::kExhausted
  { { ConstantValueTag(0x2), "kNoLogs", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::StatusEnum::kNoLogs
  { { ConstantValueTag(0x3), "kBusy", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::StatusEnum::kBusy
  { { ConstantValueTag(0x4), "kDenied", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::StatusEnum::kDenied
};

const Entry<ItemInfo> _DiagnosticLogs_TransferProtocolEnum[] = {
  { { ConstantValueTag(0x0), "kResponsePayload", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::TransferProtocolEnum::kResponsePayload
  { { ConstantValueTag(0x1), "kBDX", ItemType::kDefault }, kInvalidNodeIndex }, // DiagnosticLogs::TransferProtocolEnum::kBDX
};

const Entry<ItemInfo> _GeneralDiagnostics[] = {
  { { AttributeTag(0), "networkInterfaces", ItemType::kList }, 226 }, // GeneralDiagnostics::NetworkInterface[]
  { { AttributeTag(1), "rebootCount", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::int16u
  { { AttributeTag(2), "upTime", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::int64u
  { { AttributeTag(3), "totalOperationalHours", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::int32u
  { { AttributeTag(4), "bootReason", ItemType::kEnum }, 230 }, // GeneralDiagnostics::BootReasonEnum
  { { AttributeTag(5), "activeHardwareFaults", ItemType::kList }, 228 }, // GeneralDiagnostics::HardwareFaultEnum[]
  { { AttributeTag(6), "activeRadioFaults", ItemType::kList }, 229 }, // GeneralDiagnostics::RadioFaultEnum[]
  { { AttributeTag(7), "activeNetworkFaults", ItemType::kList }, 227 }, // GeneralDiagnostics::NetworkFaultEnum[]
  { { AttributeTag(8), "testEventTriggersEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::boolean
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // GeneralDiagnostics::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // GeneralDiagnostics::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // GeneralDiagnostics::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // GeneralDiagnostics::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::int16u
  { { EventTag(0), "HardwareFaultChange", ItemType::kDefault }, 222 }, // GeneralDiagnostics::HardwareFaultChange
  { { EventTag(1), "RadioFaultChange", ItemType::kDefault }, 223 }, // GeneralDiagnostics::RadioFaultChange
  { { EventTag(2), "NetworkFaultChange", ItemType::kDefault }, 224 }, // GeneralDiagnostics::NetworkFaultChange
  { { EventTag(3), "BootReason", ItemType::kDefault }, 225 }, // GeneralDiagnostics::BootReason
  { { CommandTag(0), "TestEventTrigger", ItemType::kDefault }, 220 }, // GeneralDiagnostics::TestEventTrigger::TestEventTriggerRequest
  { { CommandTag(1), "TimeSnapshot", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::TimeSnapshot::()
  { { CommandTag(2), "TimeSnapshotResponse", ItemType::kDefault }, 221 }, // GeneralDiagnostics::TimeSnapshotResponse
};

const Entry<ItemInfo> _GeneralDiagnostics_NetworkInterface[] = {
  { { ContextTag(0), "name", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::char_string
  { { ContextTag(1), "isOperational", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::boolean
  { { ContextTag(2), "offPremiseServicesReachableIPv4", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::boolean
  { { ContextTag(3), "offPremiseServicesReachableIPv6", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::boolean
  { { ContextTag(4), "hardwareAddress", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::octet_string
  { { ContextTag(5), "IPv4Addresses", ItemType::kList }, 1 }, // GeneralDiagnostics::octet_string[]
  { { ContextTag(6), "IPv6Addresses", ItemType::kList }, 1 }, // GeneralDiagnostics::octet_string[]
  { { ContextTag(7), "type", ItemType::kEnum }, 232 }, // GeneralDiagnostics::InterfaceTypeEnum
};

const Entry<ItemInfo> _GeneralDiagnostics_TestEventTriggerRequest[] = {
  { { ContextTag(0), "enableKey", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::octet_string
  { { ContextTag(1), "eventTrigger", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::int64u
};

const Entry<ItemInfo> _GeneralDiagnostics_TimeSnapshotResponse[] = {
  { { ContextTag(0), "systemTimeMs", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::systime_ms
  { { ContextTag(1), "posixTimeMs", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::posix_ms
};

const Entry<ItemInfo> _GeneralDiagnostics_HardwareFaultChange[] = {
  { { ContextTag(0), "current", ItemType::kList }, 228 }, // GeneralDiagnostics::HardwareFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 228 }, // GeneralDiagnostics::HardwareFaultEnum[]
};

const Entry<ItemInfo> _GeneralDiagnostics_RadioFaultChange[] = {
  { { ContextTag(0), "current", ItemType::kList }, 229 }, // GeneralDiagnostics::RadioFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 229 }, // GeneralDiagnostics::RadioFaultEnum[]
};

const Entry<ItemInfo> _GeneralDiagnostics_NetworkFaultChange[] = {
  { { ContextTag(0), "current", ItemType::kList }, 227 }, // GeneralDiagnostics::NetworkFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 227 }, // GeneralDiagnostics::NetworkFaultEnum[]
};

const Entry<ItemInfo> _GeneralDiagnostics_BootReason[] = {
  { { ContextTag(0), "bootReason", ItemType::kEnum }, 230 }, // GeneralDiagnostics::BootReasonEnum
};

const Entry<ItemInfo> _GeneralDiagnostics_NetworkInterface_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 219 }, // GeneralDiagnostics_NetworkInterface[]
};

const Entry<ItemInfo> _GeneralDiagnostics_NetworkFaultEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 233 }, // GeneralDiagnostics_NetworkFaultEnum[]
};

const Entry<ItemInfo> _GeneralDiagnostics_HardwareFaultEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 231 }, // GeneralDiagnostics_HardwareFaultEnum[]
};

const Entry<ItemInfo> _GeneralDiagnostics_RadioFaultEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 234 }, // GeneralDiagnostics_RadioFaultEnum[]
};

const Entry<ItemInfo> _GeneralDiagnostics_BootReasonEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::BootReasonEnum::kUnspecified
  { { ConstantValueTag(0x1), "kPowerOnReboot", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::BootReasonEnum::kPowerOnReboot
  { { ConstantValueTag(0x2), "kBrownOutReset", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::BootReasonEnum::kBrownOutReset
  { { ConstantValueTag(0x3), "kSoftwareWatchdogReset", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::BootReasonEnum::kSoftwareWatchdogReset
  { { ConstantValueTag(0x4), "kHardwareWatchdogReset", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::BootReasonEnum::kHardwareWatchdogReset
  { { ConstantValueTag(0x5), "kSoftwareUpdateCompleted", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::BootReasonEnum::kSoftwareUpdateCompleted
  { { ConstantValueTag(0x6), "kSoftwareReset", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::BootReasonEnum::kSoftwareReset
};

const Entry<ItemInfo> _GeneralDiagnostics_HardwareFaultEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kUnspecified
  { { ConstantValueTag(0x1), "kRadio", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kRadio
  { { ConstantValueTag(0x2), "kSensor", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kSensor
  { { ConstantValueTag(0x3), "kResettableOverTemp", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kResettableOverTemp
  { { ConstantValueTag(0x4), "kNonResettableOverTemp", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kNonResettableOverTemp
  { { ConstantValueTag(0x5), "kPowerSource", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kPowerSource
  { { ConstantValueTag(0x6), "kVisualDisplayFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kVisualDisplayFault
  { { ConstantValueTag(0x7), "kAudioOutputFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kAudioOutputFault
  { { ConstantValueTag(0x8), "kUserInterfaceFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kUserInterfaceFault
  { { ConstantValueTag(0x9), "kNonVolatileMemoryError", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kNonVolatileMemoryError
  { { ConstantValueTag(0xA), "kTamperDetected", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::HardwareFaultEnum::kTamperDetected
};

const Entry<ItemInfo> _GeneralDiagnostics_InterfaceTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::InterfaceTypeEnum::kUnspecified
  { { ConstantValueTag(0x1), "kWiFi", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::InterfaceTypeEnum::kWiFi
  { { ConstantValueTag(0x2), "kEthernet", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::InterfaceTypeEnum::kEthernet
  { { ConstantValueTag(0x3), "kCellular", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::InterfaceTypeEnum::kCellular
  { { ConstantValueTag(0x4), "kThread", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::InterfaceTypeEnum::kThread
};

const Entry<ItemInfo> _GeneralDiagnostics_NetworkFaultEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::NetworkFaultEnum::kUnspecified
  { { ConstantValueTag(0x1), "kHardwareFailure", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::NetworkFaultEnum::kHardwareFailure
  { { ConstantValueTag(0x2), "kNetworkJammed", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::NetworkFaultEnum::kNetworkJammed
  { { ConstantValueTag(0x3), "kConnectionFailed", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::NetworkFaultEnum::kConnectionFailed
};

const Entry<ItemInfo> _GeneralDiagnostics_RadioFaultEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::RadioFaultEnum::kUnspecified
  { { ConstantValueTag(0x1), "kWiFiFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::RadioFaultEnum::kWiFiFault
  { { ConstantValueTag(0x2), "kCellularFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::RadioFaultEnum::kCellularFault
  { { ConstantValueTag(0x3), "kThreadFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::RadioFaultEnum::kThreadFault
  { { ConstantValueTag(0x4), "kNFCFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::RadioFaultEnum::kNFCFault
  { { ConstantValueTag(0x5), "kBLEFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::RadioFaultEnum::kBLEFault
  { { ConstantValueTag(0x6), "kEthernetFault", ItemType::kDefault }, kInvalidNodeIndex }, // GeneralDiagnostics::RadioFaultEnum::kEthernetFault
};

const Entry<ItemInfo> _SoftwareDiagnostics[] = {
  { { AttributeTag(0), "threadMetrics", ItemType::kList }, 238 }, // SoftwareDiagnostics::ThreadMetricsStruct[]
  { { AttributeTag(1), "currentHeapFree", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int64u
  { { AttributeTag(2), "currentHeapUsed", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int64u
  { { AttributeTag(3), "currentHeapHighWatermark", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int64u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // SoftwareDiagnostics::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // SoftwareDiagnostics::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // SoftwareDiagnostics::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // SoftwareDiagnostics::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 239 }, // SoftwareDiagnostics::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int16u
  { { EventTag(0), "SoftwareFault", ItemType::kDefault }, 237 }, // SoftwareDiagnostics::SoftwareFault
  { { CommandTag(0), "ResetWatermarks", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::ResetWatermarks::()
};

const Entry<ItemInfo> _SoftwareDiagnostics_ThreadMetricsStruct[] = {
  { { ContextTag(0), "id", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int64u
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::char_string
  { { ContextTag(2), "stackFreeCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int32u
  { { ContextTag(3), "stackFreeMinimum", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int32u
  { { ContextTag(4), "stackSize", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int32u
};

const Entry<ItemInfo> _SoftwareDiagnostics_SoftwareFault[] = {
  { { ContextTag(0), "id", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::int64u
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::char_string
  { { ContextTag(2), "faultRecording", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::octet_string
};

const Entry<ItemInfo> _SoftwareDiagnostics_ThreadMetricsStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 236 }, // SoftwareDiagnostics_ThreadMetricsStruct[]
};

const Entry<ItemInfo> _SoftwareDiagnostics_Feature[] = {
  { { ConstantValueTag(0x1), "kWatermarks", ItemType::kDefault }, kInvalidNodeIndex }, // SoftwareDiagnostics::Feature::kWatermarks
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics[] = {
  { { AttributeTag(0), "channel", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(1), "routingRole", ItemType::kEnum }, 252 }, // ThreadNetworkDiagnostics::RoutingRoleEnum
  { { AttributeTag(2), "networkName", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::char_string
  { { AttributeTag(3), "panId", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(4), "extendedPanId", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int64u
  { { AttributeTag(5), "meshLocalPrefix", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::octet_string
  { { AttributeTag(6), "overrunCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int64u
  { { AttributeTag(7), "neighborTable", ItemType::kList }, 249 }, // ThreadNetworkDiagnostics::NeighborTableStruct[]
  { { AttributeTag(8), "routeTable", ItemType::kList }, 247 }, // ThreadNetworkDiagnostics::RouteTableStruct[]
  { { AttributeTag(9), "partitionId", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(10), "weighting", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(11), "dataVersion", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(12), "stableDataVersion", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(13), "leaderRouterId", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { AttributeTag(14), "detachedRoleCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(15), "childRoleCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(16), "routerRoleCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(17), "leaderRoleCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(18), "attachAttemptCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(19), "partitionIdChangeCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(20), "betterPartitionAttachAttemptCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(21), "parentChangeCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { AttributeTag(22), "txTotalCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(23), "txUnicastCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(24), "txBroadcastCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(25), "txAckRequestedCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(26), "txAckedCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(27), "txNoAckRequestedCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(28), "txDataCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(29), "txDataPollCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(30), "txBeaconCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(31), "txBeaconRequestCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(32), "txOtherCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(33), "txRetryCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(34), "txDirectMaxRetryExpiryCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(35), "txIndirectMaxRetryExpiryCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(36), "txErrCcaCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(37), "txErrAbortCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(38), "txErrBusyChannelCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(39), "rxTotalCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(40), "rxUnicastCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(41), "rxBroadcastCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(42), "rxDataCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(43), "rxDataPollCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(44), "rxBeaconCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(45), "rxBeaconRequestCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(46), "rxOtherCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(47), "rxAddressFilteredCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(48), "rxDestAddrFilteredCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(49), "rxDuplicatedCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(50), "rxErrNoFrameCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(51), "rxErrUnknownNeighborCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(52), "rxErrInvalidSrcAddrCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(53), "rxErrSecCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(54), "rxErrFcsCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(55), "rxErrOtherCount", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(56), "activeTimestamp", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int64u
  { { AttributeTag(57), "pendingTimestamp", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int64u
  { { AttributeTag(58), "delay", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { AttributeTag(59), "securityPolicy", ItemType::kDefault }, 244 }, // ThreadNetworkDiagnostics::SecurityPolicy
  { { AttributeTag(60), "channelPage0Mask", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::octet_string
  { { AttributeTag(61), "operationalDatasetComponents", ItemType::kDefault }, 242 }, // ThreadNetworkDiagnostics::OperationalDatasetComponents
  { { AttributeTag(62), "activeNetworkFaultsList", ItemType::kList }, 248 }, // ThreadNetworkDiagnostics::NetworkFaultEnum[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ThreadNetworkDiagnostics::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ThreadNetworkDiagnostics::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ThreadNetworkDiagnostics::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ThreadNetworkDiagnostics::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 253 }, // ThreadNetworkDiagnostics::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { EventTag(0), "ConnectionStatus", ItemType::kDefault }, 245 }, // ThreadNetworkDiagnostics::ConnectionStatus
  { { EventTag(1), "NetworkFaultChange", ItemType::kDefault }, 246 }, // ThreadNetworkDiagnostics::NetworkFaultChange
  { { CommandTag(0), "ResetCounts", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::ResetCounts::()
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_NeighborTableStruct[] = {
  { { ContextTag(0), "extAddress", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int64u
  { { ContextTag(1), "age", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { ContextTag(2), "rloc16", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { ContextTag(3), "linkFrameCounter", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { ContextTag(4), "mleFrameCounter", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int32u
  { { ContextTag(5), "lqi", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(6), "averageRssi", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8s
  { { ContextTag(7), "lastRssi", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8s
  { { ContextTag(8), "frameErrorRate", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(9), "messageErrorRate", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(10), "rxOnWhenIdle", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(11), "fullThreadDevice", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(12), "fullNetworkData", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(13), "isChild", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_OperationalDatasetComponents[] = {
  { { ContextTag(0), "activeTimestampPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(1), "pendingTimestampPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(2), "masterKeyPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(3), "networkNamePresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(4), "extendedPanIdPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(5), "meshLocalPrefixPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(6), "delayPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(7), "panIdPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(8), "channelPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(9), "pskcPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(10), "securityPolicyPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(11), "channelMaskPresent", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_RouteTableStruct[] = {
  { { ContextTag(0), "extAddress", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int64u
  { { ContextTag(1), "rloc16", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { ContextTag(2), "routerId", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(3), "nextHop", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(4), "pathCost", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(5), "LQIIn", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(6), "LQIOut", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(7), "age", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int8u
  { { ContextTag(8), "allocated", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
  { { ContextTag(9), "linkEstablished", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::boolean
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_SecurityPolicy[] = {
  { { ContextTag(0), "rotationTime", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
  { { ContextTag(1), "flags", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::int16u
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_ConnectionStatus[] = {
  { { ContextTag(0), "connectionStatus", ItemType::kEnum }, 250 }, // ThreadNetworkDiagnostics::ConnectionStatusEnum
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_NetworkFaultChange[] = {
  { { ContextTag(0), "current", ItemType::kList }, 248 }, // ThreadNetworkDiagnostics::NetworkFaultEnum[]
  { { ContextTag(1), "previous", ItemType::kList }, 248 }, // ThreadNetworkDiagnostics::NetworkFaultEnum[]
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_RouteTableStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 243 }, // ThreadNetworkDiagnostics_RouteTableStruct[]
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_NetworkFaultEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 251 }, // ThreadNetworkDiagnostics_NetworkFaultEnum[]
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_NeighborTableStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 241 }, // ThreadNetworkDiagnostics_NeighborTableStruct[]
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_ConnectionStatusEnum[] = {
  { { ConstantValueTag(0x0), "kConnected", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::ConnectionStatusEnum::kConnected
  { { ConstantValueTag(0x1), "kNotConnected", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::ConnectionStatusEnum::kNotConnected
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_NetworkFaultEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::NetworkFaultEnum::kUnspecified
  { { ConstantValueTag(0x1), "kLinkDown", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::NetworkFaultEnum::kLinkDown
  { { ConstantValueTag(0x2), "kHardwareFailure", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::NetworkFaultEnum::kHardwareFailure
  { { ConstantValueTag(0x3), "kNetworkJammed", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::NetworkFaultEnum::kNetworkJammed
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_RoutingRoleEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::RoutingRoleEnum::kUnspecified
  { { ConstantValueTag(0x1), "kUnassigned", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::RoutingRoleEnum::kUnassigned
  { { ConstantValueTag(0x2), "kSleepyEndDevice", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::RoutingRoleEnum::kSleepyEndDevice
  { { ConstantValueTag(0x3), "kEndDevice", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::RoutingRoleEnum::kEndDevice
  { { ConstantValueTag(0x4), "kREED", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::RoutingRoleEnum::kREED
  { { ConstantValueTag(0x5), "kRouter", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::RoutingRoleEnum::kRouter
  { { ConstantValueTag(0x6), "kLeader", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::RoutingRoleEnum::kLeader
};

const Entry<ItemInfo> _ThreadNetworkDiagnostics_Feature[] = {
  { { ConstantValueTag(0x1), "kPacketCounts", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::Feature::kPacketCounts
  { { ConstantValueTag(0x2), "kErrorCounts", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::Feature::kErrorCounts
  { { ConstantValueTag(0x4), "kMLECounts", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::Feature::kMLECounts
  { { ConstantValueTag(0x8), "kMACCounts", ItemType::kDefault }, kInvalidNodeIndex }, // ThreadNetworkDiagnostics::Feature::kMACCounts
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics[] = {
  { { AttributeTag(0), "bssid", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::octet_string
  { { AttributeTag(1), "securityType", ItemType::kEnum }, 260 }, // WiFiNetworkDiagnostics::SecurityTypeEnum
  { { AttributeTag(2), "wiFiVersion", ItemType::kEnum }, 261 }, // WiFiNetworkDiagnostics::WiFiVersionEnum
  { { AttributeTag(3), "channelNumber", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int16u
  { { AttributeTag(4), "rssi", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int8s
  { { AttributeTag(5), "beaconLostCount", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int32u
  { { AttributeTag(6), "beaconRxCount", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int32u
  { { AttributeTag(7), "packetMulticastRxCount", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int32u
  { { AttributeTag(8), "packetMulticastTxCount", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int32u
  { { AttributeTag(9), "packetUnicastRxCount", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int32u
  { { AttributeTag(10), "packetUnicastTxCount", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int32u
  { { AttributeTag(11), "currentMaxRate", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int64u
  { { AttributeTag(12), "overrunCount", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int64u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // WiFiNetworkDiagnostics::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // WiFiNetworkDiagnostics::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // WiFiNetworkDiagnostics::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // WiFiNetworkDiagnostics::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 262 }, // WiFiNetworkDiagnostics::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int16u
  { { EventTag(0), "Disconnection", ItemType::kDefault }, 255 }, // WiFiNetworkDiagnostics::Disconnection
  { { EventTag(1), "AssociationFailure", ItemType::kDefault }, 256 }, // WiFiNetworkDiagnostics::AssociationFailure
  { { EventTag(2), "ConnectionStatus", ItemType::kDefault }, 257 }, // WiFiNetworkDiagnostics::ConnectionStatus
  { { CommandTag(0), "ResetCounts", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::ResetCounts::()
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics_Disconnection[] = {
  { { ContextTag(0), "reasonCode", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int16u
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics_AssociationFailure[] = {
  { { ContextTag(0), "associationFailureCause", ItemType::kEnum }, 258 }, // WiFiNetworkDiagnostics::AssociationFailureCauseEnum
  { { ContextTag(1), "status", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::int16u
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics_ConnectionStatus[] = {
  { { ContextTag(0), "connectionStatus", ItemType::kEnum }, 259 }, // WiFiNetworkDiagnostics::ConnectionStatusEnum
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics_AssociationFailureCauseEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::AssociationFailureCauseEnum::kUnknown
  { { ConstantValueTag(0x1), "kAssociationFailed", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::AssociationFailureCauseEnum::kAssociationFailed
  { { ConstantValueTag(0x2), "kAuthenticationFailed", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::AssociationFailureCauseEnum::kAuthenticationFailed
  { { ConstantValueTag(0x3), "kSsidNotFound", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::AssociationFailureCauseEnum::kSsidNotFound
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics_ConnectionStatusEnum[] = {
  { { ConstantValueTag(0x0), "kConnected", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::ConnectionStatusEnum::kConnected
  { { ConstantValueTag(0x1), "kNotConnected", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::ConnectionStatusEnum::kNotConnected
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics_SecurityTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::SecurityTypeEnum::kUnspecified
  { { ConstantValueTag(0x1), "kNone", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::SecurityTypeEnum::kNone
  { { ConstantValueTag(0x2), "kWEP", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::SecurityTypeEnum::kWEP
  { { ConstantValueTag(0x3), "kWPA", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::SecurityTypeEnum::kWPA
  { { ConstantValueTag(0x4), "kWPA2", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::SecurityTypeEnum::kWPA2
  { { ConstantValueTag(0x5), "kWPA3", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::SecurityTypeEnum::kWPA3
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics_WiFiVersionEnum[] = {
  { { ConstantValueTag(0x0), "kA", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::WiFiVersionEnum::kA
  { { ConstantValueTag(0x1), "kB", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::WiFiVersionEnum::kB
  { { ConstantValueTag(0x2), "kG", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::WiFiVersionEnum::kG
  { { ConstantValueTag(0x3), "kN", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::WiFiVersionEnum::kN
  { { ConstantValueTag(0x4), "kAc", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::WiFiVersionEnum::kAc
  { { ConstantValueTag(0x5), "kAx", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::WiFiVersionEnum::kAx
  { { ConstantValueTag(0x6), "kAh", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::WiFiVersionEnum::kAh
};

const Entry<ItemInfo> _WiFiNetworkDiagnostics_Feature[] = {
  { { ConstantValueTag(0x1), "kPacketCounts", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::Feature::kPacketCounts
  { { ConstantValueTag(0x2), "kErrorCounts", ItemType::kDefault }, kInvalidNodeIndex }, // WiFiNetworkDiagnostics::Feature::kErrorCounts
};

const Entry<ItemInfo> _EthernetNetworkDiagnostics[] = {
  { { AttributeTag(0), "PHYRate", ItemType::kEnum }, 264 }, // EthernetNetworkDiagnostics::PHYRateEnum
  { { AttributeTag(1), "fullDuplex", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::boolean
  { { AttributeTag(2), "packetRxCount", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::int64u
  { { AttributeTag(3), "packetTxCount", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::int64u
  { { AttributeTag(4), "txErrCount", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::int64u
  { { AttributeTag(5), "collisionCount", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::int64u
  { { AttributeTag(6), "overrunCount", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::int64u
  { { AttributeTag(7), "carrierDetect", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::boolean
  { { AttributeTag(8), "timeSinceReset", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::int64u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // EthernetNetworkDiagnostics::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // EthernetNetworkDiagnostics::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // EthernetNetworkDiagnostics::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // EthernetNetworkDiagnostics::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 265 }, // EthernetNetworkDiagnostics::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::int16u
  { { CommandTag(0), "ResetCounts", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::ResetCounts::()
};

const Entry<ItemInfo> _EthernetNetworkDiagnostics_PHYRateEnum[] = {
  { { ConstantValueTag(0x0), "kRate10M", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate10M
  { { ConstantValueTag(0x1), "kRate100M", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate100M
  { { ConstantValueTag(0x2), "kRate1G", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate1G
  { { ConstantValueTag(0x3), "kRate25G", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate25G
  { { ConstantValueTag(0x4), "kRate5G", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate5G
  { { ConstantValueTag(0x5), "kRate10G", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate10G
  { { ConstantValueTag(0x6), "kRate40G", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate40G
  { { ConstantValueTag(0x7), "kRate100G", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate100G
  { { ConstantValueTag(0x8), "kRate200G", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate200G
  { { ConstantValueTag(0x9), "kRate400G", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::PHYRateEnum::kRate400G
};

const Entry<ItemInfo> _EthernetNetworkDiagnostics_Feature[] = {
  { { ConstantValueTag(0x1), "kPacketCounts", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::Feature::kPacketCounts
  { { ConstantValueTag(0x2), "kErrorCounts", ItemType::kDefault }, kInvalidNodeIndex }, // EthernetNetworkDiagnostics::Feature::kErrorCounts
};

const Entry<ItemInfo> _TimeSynchronization[] = {
  { { AttributeTag(0), "UTCTime", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::epoch_us
  { { AttributeTag(1), "granularity", ItemType::kEnum }, 281 }, // TimeSynchronization::GranularityEnum
  { { AttributeTag(2), "timeSource", ItemType::kEnum }, 283 }, // TimeSynchronization::TimeSourceEnum
  { { AttributeTag(3), "trustedTimeSource", ItemType::kDefault }, 270 }, // TimeSynchronization::TrustedTimeSourceStruct
  { { AttributeTag(4), "defaultNTP", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::char_string
  { { AttributeTag(5), "timeZone", ItemType::kList }, 280 }, // TimeSynchronization::TimeZoneStruct[]
  { { AttributeTag(6), "DSTOffset", ItemType::kList }, 279 }, // TimeSynchronization::DSTOffsetStruct[]
  { { AttributeTag(7), "localTime", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::epoch_us
  { { AttributeTag(8), "timeZoneDatabase", ItemType::kEnum }, 284 }, // TimeSynchronization::TimeZoneDatabaseEnum
  { { AttributeTag(9), "NTPServerAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::boolean
  { { AttributeTag(10), "timeZoneListMaxSize", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::int8u
  { { AttributeTag(11), "DSTOffsetListMaxSize", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::int8u
  { { AttributeTag(12), "supportsDNSResolve", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::boolean
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // TimeSynchronization::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // TimeSynchronization::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // TimeSynchronization::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // TimeSynchronization::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 285 }, // TimeSynchronization::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::int16u
  { { EventTag(1), "DSTStatus", ItemType::kDefault }, 277 }, // TimeSynchronization::DSTStatus
  { { EventTag(2), "TimeZoneStatus", ItemType::kDefault }, 278 }, // TimeSynchronization::TimeZoneStatus
  { { CommandTag(0), "SetUTCTime", ItemType::kDefault }, 271 }, // TimeSynchronization::SetUTCTime::SetUTCTimeRequest
  { { CommandTag(1), "SetTrustedTimeSource", ItemType::kDefault }, 272 }, // TimeSynchronization::SetTrustedTimeSource::SetTrustedTimeSourceRequest
  { { CommandTag(2), "SetTimeZone", ItemType::kDefault }, 273 }, // TimeSynchronization::SetTimeZone::SetTimeZoneRequest
  { { CommandTag(4), "SetDSTOffset", ItemType::kDefault }, 275 }, // TimeSynchronization::SetDSTOffset::SetDSTOffsetRequest
  { { CommandTag(5), "SetDefaultNTP", ItemType::kDefault }, 276 }, // TimeSynchronization::SetDefaultNTP::SetDefaultNTPRequest
  { { CommandTag(3), "SetTimeZoneResponse", ItemType::kDefault }, 274 }, // TimeSynchronization::SetTimeZoneResponse
};

const Entry<ItemInfo> _TimeSynchronization_DSTOffsetStruct[] = {
  { { ContextTag(0), "offset", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::int32s
  { { ContextTag(1), "validStarting", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::epoch_us
  { { ContextTag(2), "validUntil", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::epoch_us
};

const Entry<ItemInfo> _TimeSynchronization_FabricScopedTrustedTimeSourceStruct[] = {
  { { ContextTag(0), "nodeID", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::node_id
  { { ContextTag(1), "endpoint", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::endpoint_no
};

const Entry<ItemInfo> _TimeSynchronization_TimeZoneStruct[] = {
  { { ContextTag(0), "offset", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::int32s
  { { ContextTag(1), "validAt", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::epoch_us
  { { ContextTag(2), "name", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::char_string
};

const Entry<ItemInfo> _TimeSynchronization_TrustedTimeSourceStruct[] = {
  { { ContextTag(0), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::fabric_idx
  { { ContextTag(1), "nodeID", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::node_id
  { { ContextTag(2), "endpoint", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::endpoint_no
};

const Entry<ItemInfo> _TimeSynchronization_SetUTCTimeRequest[] = {
  { { ContextTag(0), "UTCTime", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::epoch_us
  { { ContextTag(1), "granularity", ItemType::kEnum }, 281 }, // TimeSynchronization::GranularityEnum
  { { ContextTag(2), "timeSource", ItemType::kEnum }, 283 }, // TimeSynchronization::TimeSourceEnum
};

const Entry<ItemInfo> _TimeSynchronization_SetTrustedTimeSourceRequest[] = {
  { { ContextTag(0), "trustedTimeSource", ItemType::kDefault }, 268 }, // TimeSynchronization::FabricScopedTrustedTimeSourceStruct
};

const Entry<ItemInfo> _TimeSynchronization_SetTimeZoneRequest[] = {
  { { ContextTag(0), "timeZone", ItemType::kList }, 280 }, // TimeSynchronization::TimeZoneStruct[]
};

const Entry<ItemInfo> _TimeSynchronization_SetTimeZoneResponse[] = {
  { { ContextTag(0), "DSTOffsetRequired", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::boolean
};

const Entry<ItemInfo> _TimeSynchronization_SetDSTOffsetRequest[] = {
  { { ContextTag(0), "DSTOffset", ItemType::kList }, 279 }, // TimeSynchronization::DSTOffsetStruct[]
};

const Entry<ItemInfo> _TimeSynchronization_SetDefaultNTPRequest[] = {
  { { ContextTag(0), "defaultNTP", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::char_string
};

const Entry<ItemInfo> _TimeSynchronization_DSTStatus[] = {
  { { ContextTag(0), "DSTOffsetActive", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::boolean
};

const Entry<ItemInfo> _TimeSynchronization_TimeZoneStatus[] = {
  { { ContextTag(0), "offset", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::int32s
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::char_string
};

const Entry<ItemInfo> _TimeSynchronization_DSTOffsetStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 267 }, // TimeSynchronization_DSTOffsetStruct[]
};

const Entry<ItemInfo> _TimeSynchronization_TimeZoneStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 269 }, // TimeSynchronization_TimeZoneStruct[]
};

const Entry<ItemInfo> _TimeSynchronization_GranularityEnum[] = {
  { { ConstantValueTag(0x0), "kNoTimeGranularity", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::GranularityEnum::kNoTimeGranularity
  { { ConstantValueTag(0x1), "kMinutesGranularity", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::GranularityEnum::kMinutesGranularity
  { { ConstantValueTag(0x2), "kSecondsGranularity", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::GranularityEnum::kSecondsGranularity
  { { ConstantValueTag(0x3), "kMillisecondsGranularity", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::GranularityEnum::kMillisecondsGranularity
  { { ConstantValueTag(0x4), "kMicrosecondsGranularity", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::GranularityEnum::kMicrosecondsGranularity
};

const Entry<ItemInfo> _TimeSynchronization_StatusCode[] = {
  { { ConstantValueTag(0x2), "kTimeNotAccepted", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::StatusCode::kTimeNotAccepted
};

const Entry<ItemInfo> _TimeSynchronization_TimeSourceEnum[] = {
  { { ConstantValueTag(0x0), "kNone", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kNone
  { { ConstantValueTag(0x1), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kUnknown
  { { ConstantValueTag(0x2), "kAdmin", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kAdmin
  { { ConstantValueTag(0x3), "kNodeTimeCluster", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kNodeTimeCluster
  { { ConstantValueTag(0x4), "kNonMatterSNTP", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kNonMatterSNTP
  { { ConstantValueTag(0x5), "kNonMatterNTP", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kNonMatterNTP
  { { ConstantValueTag(0x6), "kMatterSNTP", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kMatterSNTP
  { { ConstantValueTag(0x7), "kMatterNTP", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kMatterNTP
  { { ConstantValueTag(0x8), "kMixedNTP", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kMixedNTP
  { { ConstantValueTag(0x9), "kNonMatterSNTPNTS", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kNonMatterSNTPNTS
  { { ConstantValueTag(0xA), "kNonMatterNTPNTS", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kNonMatterNTPNTS
  { { ConstantValueTag(0xB), "kMatterSNTPNTS", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kMatterSNTPNTS
  { { ConstantValueTag(0xC), "kMatterNTPNTS", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kMatterNTPNTS
  { { ConstantValueTag(0xD), "kMixedNTPNTS", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kMixedNTPNTS
  { { ConstantValueTag(0xE), "kCloudSource", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kCloudSource
  { { ConstantValueTag(0xF), "kPTP", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kPTP
  { { ConstantValueTag(0x10), "kGNSS", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeSourceEnum::kGNSS
};

const Entry<ItemInfo> _TimeSynchronization_TimeZoneDatabaseEnum[] = {
  { { ConstantValueTag(0x0), "kFull", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeZoneDatabaseEnum::kFull
  { { ConstantValueTag(0x1), "kPartial", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeZoneDatabaseEnum::kPartial
  { { ConstantValueTag(0x2), "kNone", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::TimeZoneDatabaseEnum::kNone
};

const Entry<ItemInfo> _TimeSynchronization_Feature[] = {
  { { ConstantValueTag(0x1), "kTimeZone", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::Feature::kTimeZone
  { { ConstantValueTag(0x2), "kNTPClient", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::Feature::kNTPClient
  { { ConstantValueTag(0x4), "kNTPServer", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::Feature::kNTPServer
  { { ConstantValueTag(0x8), "kTimeSyncClient", ItemType::kDefault }, kInvalidNodeIndex }, // TimeSynchronization::Feature::kTimeSyncClient
};

const Entry<ItemInfo> _BridgedDeviceBasicInformation[] = {
  { { AttributeTag(1), "vendorName", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(2), "vendorID", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::vendor_id
  { { AttributeTag(3), "productName", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(5), "nodeLabel", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(7), "hardwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::int16u
  { { AttributeTag(8), "hardwareVersionString", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(9), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::int32u
  { { AttributeTag(10), "softwareVersionString", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(11), "manufacturingDate", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(12), "partNumber", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(13), "productURL", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::long_char_string
  { { AttributeTag(14), "productLabel", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(15), "serialNumber", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(17), "reachable", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::boolean
  { { AttributeTag(18), "uniqueID", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::char_string
  { { AttributeTag(20), "productAppearance", ItemType::kDefault }, 287 }, // BridgedDeviceBasicInformation::ProductAppearanceStruct
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // BridgedDeviceBasicInformation::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // BridgedDeviceBasicInformation::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // BridgedDeviceBasicInformation::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // BridgedDeviceBasicInformation::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::int16u
  { { EventTag(0), "StartUp", ItemType::kDefault }, 288 }, // BridgedDeviceBasicInformation::StartUp
  { { EventTag(3), "ReachableChanged", ItemType::kDefault }, 289 }, // BridgedDeviceBasicInformation::ReachableChanged
};

const Entry<ItemInfo> _BridgedDeviceBasicInformation_ProductAppearanceStruct[] = {
  { { ContextTag(0), "finish", ItemType::kEnum }, 291 }, // BridgedDeviceBasicInformation::ProductFinishEnum
  { { ContextTag(1), "primaryColor", ItemType::kEnum }, 290 }, // BridgedDeviceBasicInformation::ColorEnum
};

const Entry<ItemInfo> _BridgedDeviceBasicInformation_StartUp[] = {
  { { ContextTag(0), "softwareVersion", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::int32u
};

const Entry<ItemInfo> _BridgedDeviceBasicInformation_ReachableChanged[] = {
  { { ContextTag(0), "reachableNewValue", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::boolean
};

const Entry<ItemInfo> _BridgedDeviceBasicInformation_ColorEnum[] = {
  { { ConstantValueTag(0x0), "kBlack", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kBlack
  { { ConstantValueTag(0x1), "kNavy", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kNavy
  { { ConstantValueTag(0x2), "kGreen", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kGreen
  { { ConstantValueTag(0x3), "kTeal", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kTeal
  { { ConstantValueTag(0x4), "kMaroon", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kMaroon
  { { ConstantValueTag(0x5), "kPurple", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kPurple
  { { ConstantValueTag(0x6), "kOlive", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kOlive
  { { ConstantValueTag(0x7), "kGray", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kGray
  { { ConstantValueTag(0x8), "kBlue", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kBlue
  { { ConstantValueTag(0x9), "kLime", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kLime
  { { ConstantValueTag(0xA), "kAqua", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kAqua
  { { ConstantValueTag(0xB), "kRed", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kRed
  { { ConstantValueTag(0xC), "kFuchsia", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kFuchsia
  { { ConstantValueTag(0xD), "kYellow", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kYellow
  { { ConstantValueTag(0xE), "kWhite", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kWhite
  { { ConstantValueTag(0xF), "kNickel", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kNickel
  { { ConstantValueTag(0x10), "kChrome", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kChrome
  { { ConstantValueTag(0x11), "kBrass", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kBrass
  { { ConstantValueTag(0x12), "kCopper", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kCopper
  { { ConstantValueTag(0x13), "kSilver", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kSilver
  { { ConstantValueTag(0x14), "kGold", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ColorEnum::kGold
};

const Entry<ItemInfo> _BridgedDeviceBasicInformation_ProductFinishEnum[] = {
  { { ConstantValueTag(0x0), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ProductFinishEnum::kOther
  { { ConstantValueTag(0x1), "kMatte", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ProductFinishEnum::kMatte
  { { ConstantValueTag(0x2), "kSatin", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ProductFinishEnum::kSatin
  { { ConstantValueTag(0x3), "kPolished", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ProductFinishEnum::kPolished
  { { ConstantValueTag(0x4), "kRugged", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ProductFinishEnum::kRugged
  { { ConstantValueTag(0x5), "kFabric", ItemType::kDefault }, kInvalidNodeIndex }, // BridgedDeviceBasicInformation::ProductFinishEnum::kFabric
};

const Entry<ItemInfo> _Switch[] = {
  { { AttributeTag(0), "numberOfPositions", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
  { { AttributeTag(1), "currentPosition", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
  { { AttributeTag(2), "multiPressMax", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Switch::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Switch::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Switch::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Switch::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 300 }, // Switch::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int16u
  { { EventTag(0), "SwitchLatched", ItemType::kDefault }, 293 }, // Switch::SwitchLatched
  { { EventTag(1), "InitialPress", ItemType::kDefault }, 294 }, // Switch::InitialPress
  { { EventTag(2), "LongPress", ItemType::kDefault }, 295 }, // Switch::LongPress
  { { EventTag(3), "ShortRelease", ItemType::kDefault }, 296 }, // Switch::ShortRelease
  { { EventTag(4), "LongRelease", ItemType::kDefault }, 297 }, // Switch::LongRelease
  { { EventTag(5), "MultiPressOngoing", ItemType::kDefault }, 298 }, // Switch::MultiPressOngoing
  { { EventTag(6), "MultiPressComplete", ItemType::kDefault }, 299 }, // Switch::MultiPressComplete
};

const Entry<ItemInfo> _Switch_SwitchLatched[] = {
  { { ContextTag(0), "newPosition", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
};

const Entry<ItemInfo> _Switch_InitialPress[] = {
  { { ContextTag(0), "newPosition", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
};

const Entry<ItemInfo> _Switch_LongPress[] = {
  { { ContextTag(0), "newPosition", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
};

const Entry<ItemInfo> _Switch_ShortRelease[] = {
  { { ContextTag(0), "previousPosition", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
};

const Entry<ItemInfo> _Switch_LongRelease[] = {
  { { ContextTag(0), "previousPosition", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
};

const Entry<ItemInfo> _Switch_MultiPressOngoing[] = {
  { { ContextTag(0), "newPosition", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
  { { ContextTag(1), "currentNumberOfPressesCounted", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
};

const Entry<ItemInfo> _Switch_MultiPressComplete[] = {
  { { ContextTag(0), "previousPosition", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
  { { ContextTag(1), "totalNumberOfPressesCounted", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::int8u
};

const Entry<ItemInfo> _Switch_Feature[] = {
  { { ConstantValueTag(0x1), "kLatchingSwitch", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::Feature::kLatchingSwitch
  { { ConstantValueTag(0x2), "kMomentarySwitch", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::Feature::kMomentarySwitch
  { { ConstantValueTag(0x4), "kMomentarySwitchRelease", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::Feature::kMomentarySwitchRelease
  { { ConstantValueTag(0x8), "kMomentarySwitchLongPress", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::Feature::kMomentarySwitchLongPress
  { { ConstantValueTag(0x10), "kMomentarySwitchMultiPress", ItemType::kDefault }, kInvalidNodeIndex }, // Switch::Feature::kMomentarySwitchMultiPress
};

const Entry<ItemInfo> _AdministratorCommissioning[] = {
  { { AttributeTag(0), "windowStatus", ItemType::kEnum }, 304 }, // AdministratorCommissioning::CommissioningWindowStatusEnum
  { { AttributeTag(1), "adminFabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::fabric_idx
  { { AttributeTag(2), "adminVendorId", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::vendor_id
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // AdministratorCommissioning::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // AdministratorCommissioning::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // AdministratorCommissioning::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // AdministratorCommissioning::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 306 }, // AdministratorCommissioning::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::int16u
  { { CommandTag(0), "OpenCommissioningWindow", ItemType::kDefault }, 302 }, // AdministratorCommissioning::OpenCommissioningWindow::OpenCommissioningWindowRequest
  { { CommandTag(1), "OpenBasicCommissioningWindow", ItemType::kDefault }, 303 }, // AdministratorCommissioning::OpenBasicCommissioningWindow::OpenBasicCommissioningWindowRequest
  { { CommandTag(2), "RevokeCommissioning", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::RevokeCommissioning::()
};

const Entry<ItemInfo> _AdministratorCommissioning_OpenCommissioningWindowRequest[] = {
  { { ContextTag(0), "commissioningTimeout", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::int16u
  { { ContextTag(1), "PAKEPasscodeVerifier", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::octet_string
  { { ContextTag(2), "discriminator", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::int16u
  { { ContextTag(3), "iterations", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::int32u
  { { ContextTag(4), "salt", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::octet_string
};

const Entry<ItemInfo> _AdministratorCommissioning_OpenBasicCommissioningWindowRequest[] = {
  { { ContextTag(0), "commissioningTimeout", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::int16u
};

const Entry<ItemInfo> _AdministratorCommissioning_CommissioningWindowStatusEnum[] = {
  { { ConstantValueTag(0x0), "kWindowNotOpen", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::CommissioningWindowStatusEnum::kWindowNotOpen
  { { ConstantValueTag(0x1), "kEnhancedWindowOpen", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::CommissioningWindowStatusEnum::kEnhancedWindowOpen
  { { ConstantValueTag(0x2), "kBasicWindowOpen", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::CommissioningWindowStatusEnum::kBasicWindowOpen
};

const Entry<ItemInfo> _AdministratorCommissioning_StatusCode[] = {
  { { ConstantValueTag(0x2), "kBusy", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::StatusCode::kBusy
  { { ConstantValueTag(0x3), "kPAKEParameterError", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::StatusCode::kPAKEParameterError
  { { ConstantValueTag(0x4), "kWindowNotOpen", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::StatusCode::kWindowNotOpen
};

const Entry<ItemInfo> _AdministratorCommissioning_Feature[] = {
  { { ConstantValueTag(0x1), "kBasic", ItemType::kDefault }, kInvalidNodeIndex }, // AdministratorCommissioning::Feature::kBasic
};

const Entry<ItemInfo> _OperationalCredentials[] = {
  { { AttributeTag(0), "NOCs", ItemType::kList }, 323 }, // OperationalCredentials::NOCStruct[]
  { { AttributeTag(1), "fabrics", ItemType::kList }, 322 }, // OperationalCredentials::FabricDescriptorStruct[]
  { { AttributeTag(2), "supportedFabrics", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::int8u
  { { AttributeTag(3), "commissionedFabrics", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::int8u
  { { AttributeTag(4), "trustedRootCertificates", ItemType::kList }, 1 }, // OperationalCredentials::octet_string[]
  { { AttributeTag(5), "currentFabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OperationalCredentials::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OperationalCredentials::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OperationalCredentials::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OperationalCredentials::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::int16u
  { { CommandTag(0), "AttestationRequest", ItemType::kDefault }, 310 }, // OperationalCredentials::AttestationRequest::AttestationRequestRequest
  { { CommandTag(2), "CertificateChainRequest", ItemType::kDefault }, 312 }, // OperationalCredentials::CertificateChainRequest::CertificateChainRequestRequest
  { { CommandTag(4), "CSRRequest", ItemType::kDefault }, 314 }, // OperationalCredentials::CSRRequest::CSRRequestRequest
  { { CommandTag(6), "AddNOC", ItemType::kDefault }, 316 }, // OperationalCredentials::AddNOC::AddNOCRequest
  { { CommandTag(7), "UpdateNOC", ItemType::kDefault }, 317 }, // OperationalCredentials::UpdateNOC::UpdateNOCRequest
  { { CommandTag(9), "UpdateFabricLabel", ItemType::kDefault }, 319 }, // OperationalCredentials::UpdateFabricLabel::UpdateFabricLabelRequest
  { { CommandTag(10), "RemoveFabric", ItemType::kDefault }, 320 }, // OperationalCredentials::RemoveFabric::RemoveFabricRequest
  { { CommandTag(11), "AddTrustedRootCertificate", ItemType::kDefault }, 321 }, // OperationalCredentials::AddTrustedRootCertificate::AddTrustedRootCertificateRequest
  { { CommandTag(1), "AttestationResponse", ItemType::kDefault }, 311 }, // OperationalCredentials::AttestationResponse
  { { CommandTag(3), "CertificateChainResponse", ItemType::kDefault }, 313 }, // OperationalCredentials::CertificateChainResponse
  { { CommandTag(5), "CSRResponse", ItemType::kDefault }, 315 }, // OperationalCredentials::CSRResponse
  { { CommandTag(8), "NOCResponse", ItemType::kDefault }, 318 }, // OperationalCredentials::NOCResponse
};

const Entry<ItemInfo> _OperationalCredentials_FabricDescriptorStruct[] = {
  { { ContextTag(1), "rootPublicKey", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(2), "vendorID", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::vendor_id
  { { ContextTag(3), "fabricID", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::fabric_id
  { { ContextTag(4), "nodeID", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::node_id
  { { ContextTag(5), "label", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::char_string
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::fabric_idx
};

const Entry<ItemInfo> _OperationalCredentials_NOCStruct[] = {
  { { ContextTag(1), "noc", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(2), "icac", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::fabric_idx
};

const Entry<ItemInfo> _OperationalCredentials_AttestationRequestRequest[] = {
  { { ContextTag(0), "attestationNonce", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
};

const Entry<ItemInfo> _OperationalCredentials_AttestationResponse[] = {
  { { ContextTag(0), "attestationElements", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(1), "attestationSignature", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
};

const Entry<ItemInfo> _OperationalCredentials_CertificateChainRequestRequest[] = {
  { { ContextTag(0), "certificateType", ItemType::kEnum }, 324 }, // OperationalCredentials::CertificateChainTypeEnum
};

const Entry<ItemInfo> _OperationalCredentials_CertificateChainResponse[] = {
  { { ContextTag(0), "certificate", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
};

const Entry<ItemInfo> _OperationalCredentials_CSRRequestRequest[] = {
  { { ContextTag(0), "CSRNonce", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(1), "isForUpdateNOC", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::boolean
};

const Entry<ItemInfo> _OperationalCredentials_CSRResponse[] = {
  { { ContextTag(0), "NOCSRElements", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(1), "attestationSignature", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
};

const Entry<ItemInfo> _OperationalCredentials_AddNOCRequest[] = {
  { { ContextTag(0), "NOCValue", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(1), "ICACValue", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(2), "IPKValue", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(3), "caseAdminSubject", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::int64u
  { { ContextTag(4), "adminVendorId", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::vendor_id
};

const Entry<ItemInfo> _OperationalCredentials_UpdateNOCRequest[] = {
  { { ContextTag(0), "NOCValue", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
  { { ContextTag(1), "ICACValue", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
};

const Entry<ItemInfo> _OperationalCredentials_NOCResponse[] = {
  { { ContextTag(0), "statusCode", ItemType::kEnum }, 325 }, // OperationalCredentials::NodeOperationalCertStatusEnum
  { { ContextTag(1), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::fabric_idx
  { { ContextTag(2), "debugText", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::char_string
};

const Entry<ItemInfo> _OperationalCredentials_UpdateFabricLabelRequest[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::char_string
};

const Entry<ItemInfo> _OperationalCredentials_RemoveFabricRequest[] = {
  { { ContextTag(0), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::fabric_idx
};

const Entry<ItemInfo> _OperationalCredentials_AddTrustedRootCertificateRequest[] = {
  { { ContextTag(0), "rootCACertificate", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::octet_string
};

const Entry<ItemInfo> _OperationalCredentials_FabricDescriptorStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 308 }, // OperationalCredentials_FabricDescriptorStruct[]
};

const Entry<ItemInfo> _OperationalCredentials_NOCStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 309 }, // OperationalCredentials_NOCStruct[]
};

const Entry<ItemInfo> _OperationalCredentials_CertificateChainTypeEnum[] = {
  { { ConstantValueTag(0x1), "kDACCertificate", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::CertificateChainTypeEnum::kDACCertificate
  { { ConstantValueTag(0x2), "kPAICertificate", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::CertificateChainTypeEnum::kPAICertificate
};

const Entry<ItemInfo> _OperationalCredentials_NodeOperationalCertStatusEnum[] = {
  { { ConstantValueTag(0x0), "kOK", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kOK
  { { ConstantValueTag(0x1), "kInvalidPublicKey", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kInvalidPublicKey
  { { ConstantValueTag(0x2), "kInvalidNodeOpId", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kInvalidNodeOpId
  { { ConstantValueTag(0x3), "kInvalidNOC", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kInvalidNOC
  { { ConstantValueTag(0x4), "kMissingCsr", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kMissingCsr
  { { ConstantValueTag(0x5), "kTableFull", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kTableFull
  { { ConstantValueTag(0x6), "kInvalidAdminSubject", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kInvalidAdminSubject
  { { ConstantValueTag(0x9), "kFabricConflict", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kFabricConflict
  { { ConstantValueTag(0xA), "kLabelConflict", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kLabelConflict
  { { ConstantValueTag(0xB), "kInvalidFabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalCredentials::NodeOperationalCertStatusEnum::kInvalidFabricIndex
};

const Entry<ItemInfo> _GroupKeyManagement[] = {
  { { AttributeTag(0), "groupKeyMap", ItemType::kList }, 336 }, // GroupKeyManagement::GroupKeyMapStruct[]
  { { AttributeTag(1), "groupTable", ItemType::kList }, 335 }, // GroupKeyManagement::GroupInfoMapStruct[]
  { { AttributeTag(2), "maxGroupsPerFabric", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::int16u
  { { AttributeTag(3), "maxGroupKeysPerFabric", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // GroupKeyManagement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // GroupKeyManagement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // GroupKeyManagement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // GroupKeyManagement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 338 }, // GroupKeyManagement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::int16u
  { { CommandTag(0), "KeySetWrite", ItemType::kDefault }, 330 }, // GroupKeyManagement::KeySetWrite::KeySetWriteRequest
  { { CommandTag(1), "KeySetRead", ItemType::kDefault }, 331 }, // GroupKeyManagement::KeySetRead::KeySetReadRequest
  { { CommandTag(3), "KeySetRemove", ItemType::kDefault }, 333 }, // GroupKeyManagement::KeySetRemove::KeySetRemoveRequest
  { { CommandTag(4), "KeySetReadAllIndices", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::KeySetReadAllIndices::()
  { { CommandTag(2), "KeySetReadResponse", ItemType::kDefault }, 332 }, // GroupKeyManagement::KeySetReadResponse
  { { CommandTag(5), "KeySetReadAllIndicesResponse", ItemType::kDefault }, 334 }, // GroupKeyManagement::KeySetReadAllIndicesResponse
};

const Entry<ItemInfo> _GroupKeyManagement_GroupInfoMapStruct[] = {
  { { ContextTag(1), "groupId", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::group_id
  { { ContextTag(2), "endpoints", ItemType::kList }, 1 }, // GroupKeyManagement::endpoint_no[]
  { { ContextTag(3), "groupName", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::char_string
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::fabric_idx
};

const Entry<ItemInfo> _GroupKeyManagement_GroupKeyMapStruct[] = {
  { { ContextTag(1), "groupId", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::group_id
  { { ContextTag(2), "groupKeySetID", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::int16u
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::fabric_idx
};

const Entry<ItemInfo> _GroupKeyManagement_GroupKeySetStruct[] = {
  { { ContextTag(0), "groupKeySetID", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::int16u
  { { ContextTag(1), "groupKeySecurityPolicy", ItemType::kEnum }, 337 }, // GroupKeyManagement::GroupKeySecurityPolicyEnum
  { { ContextTag(2), "epochKey0", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::octet_string
  { { ContextTag(3), "epochStartTime0", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::epoch_us
  { { ContextTag(4), "epochKey1", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::octet_string
  { { ContextTag(5), "epochStartTime1", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::epoch_us
  { { ContextTag(6), "epochKey2", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::octet_string
  { { ContextTag(7), "epochStartTime2", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::epoch_us
};

const Entry<ItemInfo> _GroupKeyManagement_KeySetWriteRequest[] = {
  { { ContextTag(0), "groupKeySet", ItemType::kDefault }, 329 }, // GroupKeyManagement::GroupKeySetStruct
};

const Entry<ItemInfo> _GroupKeyManagement_KeySetReadRequest[] = {
  { { ContextTag(0), "groupKeySetID", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::int16u
};

const Entry<ItemInfo> _GroupKeyManagement_KeySetReadResponse[] = {
  { { ContextTag(0), "groupKeySet", ItemType::kDefault }, 329 }, // GroupKeyManagement::GroupKeySetStruct
};

const Entry<ItemInfo> _GroupKeyManagement_KeySetRemoveRequest[] = {
  { { ContextTag(0), "groupKeySetID", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::int16u
};

const Entry<ItemInfo> _GroupKeyManagement_KeySetReadAllIndicesResponse[] = {
  { { ContextTag(0), "groupKeySetIDs", ItemType::kList }, 1 }, // GroupKeyManagement::int16u[]
};

const Entry<ItemInfo> _GroupKeyManagement_GroupInfoMapStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 327 }, // GroupKeyManagement_GroupInfoMapStruct[]
};

const Entry<ItemInfo> _GroupKeyManagement_GroupKeyMapStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 328 }, // GroupKeyManagement_GroupKeyMapStruct[]
};

const Entry<ItemInfo> _GroupKeyManagement_GroupKeySecurityPolicyEnum[] = {
  { { ConstantValueTag(0x0), "kTrustFirst", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::GroupKeySecurityPolicyEnum::kTrustFirst
  { { ConstantValueTag(0x1), "kCacheAndSync", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::GroupKeySecurityPolicyEnum::kCacheAndSync
};

const Entry<ItemInfo> _GroupKeyManagement_Feature[] = {
  { { ConstantValueTag(0x1), "kCacheAndSync", ItemType::kDefault }, kInvalidNodeIndex }, // GroupKeyManagement::Feature::kCacheAndSync
};

const Entry<ItemInfo> _FixedLabel[] = {
  { { AttributeTag(0), "labelList", ItemType::kList }, 341 }, // FixedLabel::LabelStruct[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // FixedLabel::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // FixedLabel::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // FixedLabel::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // FixedLabel::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // FixedLabel::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // FixedLabel::int16u
};

const Entry<ItemInfo> _FixedLabel_LabelStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // FixedLabel::char_string
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // FixedLabel::char_string
};

const Entry<ItemInfo> _FixedLabel_LabelStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 340 }, // FixedLabel_LabelStruct[]
};

const Entry<ItemInfo> _UserLabel[] = {
  { { AttributeTag(0), "labelList", ItemType::kList }, 344 }, // UserLabel::LabelStruct[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // UserLabel::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // UserLabel::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // UserLabel::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // UserLabel::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // UserLabel::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // UserLabel::int16u
};

const Entry<ItemInfo> _UserLabel_LabelStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // UserLabel::char_string
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // UserLabel::char_string
};

const Entry<ItemInfo> _UserLabel_LabelStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 343 }, // UserLabel_LabelStruct[]
};

const Entry<ItemInfo> _ProxyConfiguration[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ProxyConfiguration::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ProxyConfiguration::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ProxyConfiguration::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ProxyConfiguration::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // ProxyConfiguration::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ProxyConfiguration::int16u
};

const Entry<ItemInfo> _ProxyDiscovery[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ProxyDiscovery::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ProxyDiscovery::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ProxyDiscovery::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ProxyDiscovery::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // ProxyDiscovery::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ProxyDiscovery::int16u
};

const Entry<ItemInfo> _ProxyValid[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ProxyValid::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ProxyValid::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ProxyValid::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ProxyValid::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // ProxyValid::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ProxyValid::int16u
};

const Entry<ItemInfo> _BooleanState[] = {
  { { AttributeTag(0), "stateValue", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanState::boolean
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // BooleanState::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // BooleanState::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // BooleanState::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // BooleanState::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanState::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanState::int16u
  { { EventTag(0), "StateChange", ItemType::kDefault }, 349 }, // BooleanState::StateChange
};

const Entry<ItemInfo> _BooleanState_StateChange[] = {
  { { ContextTag(0), "stateValue", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanState::boolean
};

const Entry<ItemInfo> _IcdManagement[] = {
  { { AttributeTag(0), "idleModeDuration", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int32u
  { { AttributeTag(1), "activeModeDuration", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int32u
  { { AttributeTag(2), "activeModeThreshold", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int16u
  { { AttributeTag(3), "registeredClients", ItemType::kList }, 356 }, // IcdManagement::MonitoringRegistrationStruct[]
  { { AttributeTag(4), "ICDCounter", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int32u
  { { AttributeTag(5), "clientsSupportedPerFabric", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int16u
  { { AttributeTag(6), "userActiveModeTriggerHint", ItemType::kBitmap }, 359 }, // IcdManagement::UserActiveModeTriggerBitmap
  { { AttributeTag(7), "userActiveModeTriggerInstruction", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::char_string
  { { AttributeTag(8), "operatingMode", ItemType::kEnum }, 357 }, // IcdManagement::OperatingModeEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // IcdManagement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // IcdManagement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // IcdManagement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // IcdManagement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 358 }, // IcdManagement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int16u
  { { CommandTag(0), "RegisterClient", ItemType::kDefault }, 352 }, // IcdManagement::RegisterClient::RegisterClientRequest
  { { CommandTag(2), "UnregisterClient", ItemType::kDefault }, 354 }, // IcdManagement::UnregisterClient::UnregisterClientRequest
  { { CommandTag(3), "StayActiveRequest", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::StayActiveRequest::()
  { { CommandTag(1), "RegisterClientResponse", ItemType::kDefault }, 353 }, // IcdManagement::RegisterClientResponse
  { { CommandTag(4), "StayActiveResponse", ItemType::kDefault }, 355 }, // IcdManagement::StayActiveResponse
};

const Entry<ItemInfo> _IcdManagement_MonitoringRegistrationStruct[] = {
  { { ContextTag(1), "checkInNodeID", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::node_id
  { { ContextTag(2), "monitoredSubject", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int64u
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::fabric_idx
};

const Entry<ItemInfo> _IcdManagement_RegisterClientRequest[] = {
  { { ContextTag(0), "checkInNodeID", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::node_id
  { { ContextTag(1), "monitoredSubject", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int64u
  { { ContextTag(2), "key", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::octet_string
  { { ContextTag(3), "verificationKey", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::octet_string
};

const Entry<ItemInfo> _IcdManagement_RegisterClientResponse[] = {
  { { ContextTag(0), "ICDCounter", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int32u
};

const Entry<ItemInfo> _IcdManagement_UnregisterClientRequest[] = {
  { { ContextTag(0), "checkInNodeID", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::node_id
  { { ContextTag(1), "verificationKey", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::octet_string
};

const Entry<ItemInfo> _IcdManagement_StayActiveResponse[] = {
  { { ContextTag(0), "promisedActiveDuration", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::int32u
};

const Entry<ItemInfo> _IcdManagement_MonitoringRegistrationStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 351 }, // IcdManagement_MonitoringRegistrationStruct[]
};

const Entry<ItemInfo> _IcdManagement_OperatingModeEnum[] = {
  { { ConstantValueTag(0x0), "kSIT", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::OperatingModeEnum::kSIT
  { { ConstantValueTag(0x1), "kLIT", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::OperatingModeEnum::kLIT
};

const Entry<ItemInfo> _IcdManagement_Feature[] = {
  { { ConstantValueTag(0x1), "kCheckInProtocolSupport", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::Feature::kCheckInProtocolSupport
  { { ConstantValueTag(0x2), "kUserActiveModeTrigger", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::Feature::kUserActiveModeTrigger
  { { ConstantValueTag(0x4), "kLongIdleTimeSupport", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::Feature::kLongIdleTimeSupport
};

const Entry<ItemInfo> _IcdManagement_UserActiveModeTriggerBitmap[] = {
  { { ConstantValueTag(0x1), "kPowerCycle", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kPowerCycle
  { { ConstantValueTag(0x2), "kSettingsMenu", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kSettingsMenu
  { { ConstantValueTag(0x4), "kCustomInstruction", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kCustomInstruction
  { { ConstantValueTag(0x8), "kDeviceManual", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kDeviceManual
  { { ConstantValueTag(0x10), "kActuateSensor", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kActuateSensor
  { { ConstantValueTag(0x20), "kActuateSensorSeconds", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kActuateSensorSeconds
  { { ConstantValueTag(0x40), "kActuateSensorTimes", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kActuateSensorTimes
  { { ConstantValueTag(0x80), "kActuateSensorLightsBlink", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kActuateSensorLightsBlink
  { { ConstantValueTag(0x100), "kResetButton", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kResetButton
  { { ConstantValueTag(0x200), "kResetButtonLightsBlink", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kResetButtonLightsBlink
  { { ConstantValueTag(0x400), "kResetButtonSeconds", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kResetButtonSeconds
  { { ConstantValueTag(0x800), "kResetButtonTimes", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kResetButtonTimes
  { { ConstantValueTag(0x1000), "kSetupButton", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kSetupButton
  { { ConstantValueTag(0x2000), "kSetupButtonSeconds", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kSetupButtonSeconds
  { { ConstantValueTag(0x4000), "kSetupButtonLightsBlink", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kSetupButtonLightsBlink
  { { ConstantValueTag(0x8000), "kSetupButtonTimes", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kSetupButtonTimes
  { { ConstantValueTag(0x10000), "kAppDefinedButton", ItemType::kDefault }, kInvalidNodeIndex }, // IcdManagement::UserActiveModeTriggerBitmap::kAppDefinedButton
};

const Entry<ItemInfo> _Timer[] = {
  { { AttributeTag(0), "setTime", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::elapsed_s
  { { AttributeTag(1), "timeRemaining", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::elapsed_s
  { { AttributeTag(2), "timerState", ItemType::kEnum }, 364 }, // Timer::TimerStatusEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Timer::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Timer::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Timer::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Timer::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 365 }, // Timer::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::int16u
  { { CommandTag(0), "SetTimer", ItemType::kDefault }, 361 }, // Timer::SetTimer::SetTimerRequest
  { { CommandTag(1), "ResetTimer", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::ResetTimer::()
  { { CommandTag(2), "AddTime", ItemType::kDefault }, 362 }, // Timer::AddTime::AddTimeRequest
  { { CommandTag(3), "ReduceTime", ItemType::kDefault }, 363 }, // Timer::ReduceTime::ReduceTimeRequest
};

const Entry<ItemInfo> _Timer_SetTimerRequest[] = {
  { { ContextTag(0), "newTime", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::elapsed_s
};

const Entry<ItemInfo> _Timer_AddTimeRequest[] = {
  { { ContextTag(0), "additionalTime", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::elapsed_s
};

const Entry<ItemInfo> _Timer_ReduceTimeRequest[] = {
  { { ContextTag(0), "timeReduction", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::elapsed_s
};

const Entry<ItemInfo> _Timer_TimerStatusEnum[] = {
  { { ConstantValueTag(0x0), "kRunning", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::TimerStatusEnum::kRunning
  { { ConstantValueTag(0x1), "kPaused", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::TimerStatusEnum::kPaused
  { { ConstantValueTag(0x2), "kExpired", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::TimerStatusEnum::kExpired
  { { ConstantValueTag(0x3), "kReady", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::TimerStatusEnum::kReady
};

const Entry<ItemInfo> _Timer_Feature[] = {
  { { ConstantValueTag(0x1), "kReset", ItemType::kDefault }, kInvalidNodeIndex }, // Timer::Feature::kReset
};

const Entry<ItemInfo> _OvenCavityOperationalState[] = {
  { { AttributeTag(0), "phaseList", ItemType::kList }, 1 }, // OvenCavityOperationalState::char_string[]
  { { AttributeTag(1), "currentPhase", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::int8u
  { { AttributeTag(2), "countdownTime", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::elapsed_s
  { { AttributeTag(3), "operationalStateList", ItemType::kList }, 372 }, // OvenCavityOperationalState::OperationalStateStruct[]
  { { AttributeTag(4), "operationalState", ItemType::kEnum }, 374 }, // OvenCavityOperationalState::OperationalStateEnum
  { { AttributeTag(5), "operationalError", ItemType::kDefault }, 367 }, // OvenCavityOperationalState::ErrorStateStruct
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OvenCavityOperationalState::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OvenCavityOperationalState::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OvenCavityOperationalState::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OvenCavityOperationalState::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::int16u
  { { EventTag(0), "OperationalError", ItemType::kDefault }, 370 }, // OvenCavityOperationalState::OperationalError
  { { EventTag(1), "OperationCompletion", ItemType::kDefault }, 371 }, // OvenCavityOperationalState::OperationCompletion
  { { CommandTag(0), "Pause", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::Pause::()
  { { CommandTag(1), "Stop", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::Stop::()
  { { CommandTag(2), "Start", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::Start::()
  { { CommandTag(3), "Resume", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::Resume::()
  { { CommandTag(4), "OperationalCommandResponse", ItemType::kDefault }, 369 }, // OvenCavityOperationalState::OperationalCommandResponse
};

const Entry<ItemInfo> _OvenCavityOperationalState_ErrorStateStruct[] = {
  { { ContextTag(0), "errorStateID", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::enum8
  { { ContextTag(1), "errorStateLabel", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::char_string
  { { ContextTag(2), "errorStateDetails", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::char_string
};

const Entry<ItemInfo> _OvenCavityOperationalState_OperationalStateStruct[] = {
  { { ContextTag(0), "operationalStateID", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::enum8
  { { ContextTag(1), "operationalStateLabel", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::char_string
};

const Entry<ItemInfo> _OvenCavityOperationalState_OperationalCommandResponse[] = {
  { { ContextTag(0), "commandResponseState", ItemType::kDefault }, 367 }, // OvenCavityOperationalState::ErrorStateStruct
};

const Entry<ItemInfo> _OvenCavityOperationalState_OperationalError[] = {
  { { ContextTag(0), "errorState", ItemType::kDefault }, 367 }, // OvenCavityOperationalState::ErrorStateStruct
};

const Entry<ItemInfo> _OvenCavityOperationalState_OperationCompletion[] = {
  { { ContextTag(0), "completionErrorCode", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::enum8
  { { ContextTag(1), "totalOperationalTime", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::elapsed_s
  { { ContextTag(2), "pausedTime", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::elapsed_s
};

const Entry<ItemInfo> _OvenCavityOperationalState_OperationalStateStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 368 }, // OvenCavityOperationalState_OperationalStateStruct[]
};

const Entry<ItemInfo> _OvenCavityOperationalState_ErrorStateEnum[] = {
  { { ConstantValueTag(0x0), "kNoError", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::ErrorStateEnum::kNoError
  { { ConstantValueTag(0x1), "kUnableToStartOrResume", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::ErrorStateEnum::kUnableToStartOrResume
  { { ConstantValueTag(0x2), "kUnableToCompleteOperation", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::ErrorStateEnum::kUnableToCompleteOperation
  { { ConstantValueTag(0x3), "kCommandInvalidInState", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::ErrorStateEnum::kCommandInvalidInState
};

const Entry<ItemInfo> _OvenCavityOperationalState_OperationalStateEnum[] = {
  { { ConstantValueTag(0x0), "kStopped", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::OperationalStateEnum::kStopped
  { { ConstantValueTag(0x1), "kRunning", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::OperationalStateEnum::kRunning
  { { ConstantValueTag(0x2), "kPaused", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::OperationalStateEnum::kPaused
  { { ConstantValueTag(0x3), "kError", ItemType::kDefault }, kInvalidNodeIndex }, // OvenCavityOperationalState::OperationalStateEnum::kError
};

const Entry<ItemInfo> _OvenMode[] = {
  { { AttributeTag(0), "supportedModes", ItemType::kList }, 380 }, // OvenMode::ModeOptionStruct[]
  { { AttributeTag(1), "currentMode", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::int8u
  { { AttributeTag(2), "startUpMode", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::int8u
  { { AttributeTag(3), "onMode", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OvenMode::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OvenMode::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OvenMode::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OvenMode::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 383 }, // OvenMode::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::int16u
  { { CommandTag(0), "ChangeToMode", ItemType::kDefault }, 378 }, // OvenMode::ChangeToMode::ChangeToModeRequest
  { { CommandTag(1), "ChangeToModeResponse", ItemType::kDefault }, 379 }, // OvenMode::ChangeToModeResponse
};

const Entry<ItemInfo> _OvenMode_ModeTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::vendor_id
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::enum16
};

const Entry<ItemInfo> _OvenMode_ModeOptionStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::char_string
  { { ContextTag(1), "mode", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::int8u
  { { ContextTag(2), "modeTags", ItemType::kList }, 381 }, // OvenMode::ModeTagStruct[]
};

const Entry<ItemInfo> _OvenMode_ChangeToModeRequest[] = {
  { { ContextTag(0), "newMode", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::int8u
};

const Entry<ItemInfo> _OvenMode_ChangeToModeResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::enum8
  { { ContextTag(1), "statusText", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::char_string
};

const Entry<ItemInfo> _OvenMode_ModeOptionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 377 }, // OvenMode_ModeOptionStruct[]
};

const Entry<ItemInfo> _OvenMode_ModeTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 376 }, // OvenMode_ModeTagStruct[]
};

const Entry<ItemInfo> _OvenMode_ModeTag[] = {
  { { ConstantValueTag(0x4000), "kBake", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kBake
  { { ConstantValueTag(0x4001), "kConvection", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kConvection
  { { ConstantValueTag(0x4002), "kGrill", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kGrill
  { { ConstantValueTag(0x4003), "kRoast", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kRoast
  { { ConstantValueTag(0x4004), "kClean", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kClean
  { { ConstantValueTag(0x4005), "kConvectionBake", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kConvectionBake
  { { ConstantValueTag(0x4006), "kConvectionRoast", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kConvectionRoast
  { { ConstantValueTag(0x4007), "kWarming", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kWarming
  { { ConstantValueTag(0x4008), "kProofing", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::ModeTag::kProofing
};

const Entry<ItemInfo> _OvenMode_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // OvenMode::Feature::kOnOff
};

const Entry<ItemInfo> _LaundryDryerControls[] = {
  { { AttributeTag(0), "supportedDrynessLevels", ItemType::kList }, 385 }, // LaundryDryerControls::DrynessLevelEnum[]
  { { AttributeTag(1), "selectedDrynessLevel", ItemType::kEnum }, 386 }, // LaundryDryerControls::DrynessLevelEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // LaundryDryerControls::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // LaundryDryerControls::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // LaundryDryerControls::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // LaundryDryerControls::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryDryerControls::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryDryerControls::int16u
};

const Entry<ItemInfo> _LaundryDryerControls_DrynessLevelEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 386 }, // LaundryDryerControls_DrynessLevelEnum[]
};

const Entry<ItemInfo> _LaundryDryerControls_DrynessLevelEnum[] = {
  { { ConstantValueTag(0x0), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryDryerControls::DrynessLevelEnum::kLow
  { { ConstantValueTag(0x1), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryDryerControls::DrynessLevelEnum::kNormal
  { { ConstantValueTag(0x2), "kExtra", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryDryerControls::DrynessLevelEnum::kExtra
  { { ConstantValueTag(0x3), "kMax", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryDryerControls::DrynessLevelEnum::kMax
};

const Entry<ItemInfo> _ModeSelect[] = {
  { { AttributeTag(0), "description", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::char_string
  { { AttributeTag(1), "standardNamespace", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::enum16
  { { AttributeTag(2), "supportedModes", ItemType::kList }, 392 }, // ModeSelect::ModeOptionStruct[]
  { { AttributeTag(3), "currentMode", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::int8u
  { { AttributeTag(4), "startUpMode", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::int8u
  { { AttributeTag(5), "onMode", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ModeSelect::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ModeSelect::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ModeSelect::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ModeSelect::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 393 }, // ModeSelect::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::int16u
  { { CommandTag(0), "ChangeToMode", ItemType::kDefault }, 390 }, // ModeSelect::ChangeToMode::ChangeToModeRequest
};

const Entry<ItemInfo> _ModeSelect_SemanticTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::vendor_id
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::enum16
};

const Entry<ItemInfo> _ModeSelect_ModeOptionStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::char_string
  { { ContextTag(1), "mode", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::int8u
  { { ContextTag(2), "semanticTags", ItemType::kList }, 391 }, // ModeSelect::SemanticTagStruct[]
};

const Entry<ItemInfo> _ModeSelect_ChangeToModeRequest[] = {
  { { ContextTag(0), "newMode", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::int8u
};

const Entry<ItemInfo> _ModeSelect_SemanticTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 388 }, // ModeSelect_SemanticTagStruct[]
};

const Entry<ItemInfo> _ModeSelect_ModeOptionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 389 }, // ModeSelect_ModeOptionStruct[]
};

const Entry<ItemInfo> _ModeSelect_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // ModeSelect::Feature::kOnOff
};

const Entry<ItemInfo> _LaundryWasherMode[] = {
  { { AttributeTag(0), "supportedModes", ItemType::kList }, 399 }, // LaundryWasherMode::ModeOptionStruct[]
  { { AttributeTag(1), "currentMode", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::int8u
  { { AttributeTag(2), "startUpMode", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::int8u
  { { AttributeTag(3), "onMode", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // LaundryWasherMode::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // LaundryWasherMode::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // LaundryWasherMode::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // LaundryWasherMode::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 402 }, // LaundryWasherMode::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::int16u
  { { CommandTag(0), "ChangeToMode", ItemType::kDefault }, 397 }, // LaundryWasherMode::ChangeToMode::ChangeToModeRequest
  { { CommandTag(1), "ChangeToModeResponse", ItemType::kDefault }, 398 }, // LaundryWasherMode::ChangeToModeResponse
};

const Entry<ItemInfo> _LaundryWasherMode_ModeTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::vendor_id
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::enum16
};

const Entry<ItemInfo> _LaundryWasherMode_ModeOptionStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::char_string
  { { ContextTag(1), "mode", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::int8u
  { { ContextTag(2), "modeTags", ItemType::kList }, 400 }, // LaundryWasherMode::ModeTagStruct[]
};

const Entry<ItemInfo> _LaundryWasherMode_ChangeToModeRequest[] = {
  { { ContextTag(0), "newMode", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::int8u
};

const Entry<ItemInfo> _LaundryWasherMode_ChangeToModeResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::enum8
  { { ContextTag(1), "statusText", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::char_string
};

const Entry<ItemInfo> _LaundryWasherMode_ModeOptionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 396 }, // LaundryWasherMode_ModeOptionStruct[]
};

const Entry<ItemInfo> _LaundryWasherMode_ModeTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 395 }, // LaundryWasherMode_ModeTagStruct[]
};

const Entry<ItemInfo> _LaundryWasherMode_ModeTag[] = {
  { { ConstantValueTag(0x4000), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::ModeTag::kNormal
  { { ConstantValueTag(0x4001), "kDelicate", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::ModeTag::kDelicate
  { { ConstantValueTag(0x4002), "kHeavy", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::ModeTag::kHeavy
  { { ConstantValueTag(0x4003), "kWhites", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::ModeTag::kWhites
};

const Entry<ItemInfo> _LaundryWasherMode_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherMode::Feature::kOnOff
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode[] = {
  { { AttributeTag(0), "supportedModes", ItemType::kList }, 408 }, // RefrigeratorAndTemperatureControlledCabinetMode::ModeOptionStruct[]
  { { AttributeTag(1), "currentMode", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::int8u
  { { AttributeTag(2), "startUpMode", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::int8u
  { { AttributeTag(3), "onMode", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // RefrigeratorAndTemperatureControlledCabinetMode::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // RefrigeratorAndTemperatureControlledCabinetMode::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // RefrigeratorAndTemperatureControlledCabinetMode::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // RefrigeratorAndTemperatureControlledCabinetMode::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 411 }, // RefrigeratorAndTemperatureControlledCabinetMode::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::int16u
  { { CommandTag(0), "ChangeToMode", ItemType::kDefault }, 406 }, // RefrigeratorAndTemperatureControlledCabinetMode::ChangeToMode::ChangeToModeRequest
  { { CommandTag(1), "ChangeToModeResponse", ItemType::kDefault }, 407 }, // RefrigeratorAndTemperatureControlledCabinetMode::ChangeToModeResponse
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode_ModeTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::vendor_id
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::enum16
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode_ModeOptionStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::char_string
  { { ContextTag(1), "mode", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::int8u
  { { ContextTag(2), "modeTags", ItemType::kList }, 409 }, // RefrigeratorAndTemperatureControlledCabinetMode::ModeTagStruct[]
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode_ChangeToModeRequest[] = {
  { { ContextTag(0), "newMode", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::int8u
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode_ChangeToModeResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::enum8
  { { ContextTag(1), "statusText", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::char_string
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode_ModeOptionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 405 }, // RefrigeratorAndTemperatureControlledCabinetMode_ModeOptionStruct[]
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode_ModeTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 404 }, // RefrigeratorAndTemperatureControlledCabinetMode_ModeTagStruct[]
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode_ModeTag[] = {
  { { ConstantValueTag(0x4000), "kRapidCool", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::ModeTag::kRapidCool
  { { ConstantValueTag(0x4001), "kRapidFreeze", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::ModeTag::kRapidFreeze
};

const Entry<ItemInfo> _RefrigeratorAndTemperatureControlledCabinetMode_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAndTemperatureControlledCabinetMode::Feature::kOnOff
};

const Entry<ItemInfo> _LaundryWasherControls[] = {
  { { AttributeTag(0), "spinSpeeds", ItemType::kList }, 1 }, // LaundryWasherControls::char_string[]
  { { AttributeTag(1), "spinSpeedCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherControls::int8u
  { { AttributeTag(2), "numberOfRinses", ItemType::kEnum }, 414 }, // LaundryWasherControls::NumberOfRinsesEnum
  { { AttributeTag(3), "supportedRinses", ItemType::kList }, 413 }, // LaundryWasherControls::NumberOfRinsesEnum[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // LaundryWasherControls::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // LaundryWasherControls::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // LaundryWasherControls::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // LaundryWasherControls::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 415 }, // LaundryWasherControls::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherControls::int16u
};

const Entry<ItemInfo> _LaundryWasherControls_NumberOfRinsesEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 414 }, // LaundryWasherControls_NumberOfRinsesEnum[]
};

const Entry<ItemInfo> _LaundryWasherControls_NumberOfRinsesEnum[] = {
  { { ConstantValueTag(0x0), "kNone", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherControls::NumberOfRinsesEnum::kNone
  { { ConstantValueTag(0x1), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherControls::NumberOfRinsesEnum::kNormal
  { { ConstantValueTag(0x2), "kExtra", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherControls::NumberOfRinsesEnum::kExtra
  { { ConstantValueTag(0x3), "kMax", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherControls::NumberOfRinsesEnum::kMax
};

const Entry<ItemInfo> _LaundryWasherControls_Feature[] = {
  { { ConstantValueTag(0x1), "kSpin", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherControls::Feature::kSpin
  { { ConstantValueTag(0x2), "kRinse", ItemType::kDefault }, kInvalidNodeIndex }, // LaundryWasherControls::Feature::kRinse
};

const Entry<ItemInfo> _RvcRunMode[] = {
  { { AttributeTag(0), "supportedModes", ItemType::kList }, 421 }, // RvcRunMode::ModeOptionStruct[]
  { { AttributeTag(1), "currentMode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::int8u
  { { AttributeTag(3), "onMode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // RvcRunMode::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // RvcRunMode::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // RvcRunMode::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // RvcRunMode::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 425 }, // RvcRunMode::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::int16u
  { { CommandTag(0), "ChangeToMode", ItemType::kDefault }, 419 }, // RvcRunMode::ChangeToMode::ChangeToModeRequest
  { { CommandTag(1), "ChangeToModeResponse", ItemType::kDefault }, 420 }, // RvcRunMode::ChangeToModeResponse
};

const Entry<ItemInfo> _RvcRunMode_ModeTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::vendor_id
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::enum16
};

const Entry<ItemInfo> _RvcRunMode_ModeOptionStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::char_string
  { { ContextTag(1), "mode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::int8u
  { { ContextTag(2), "modeTags", ItemType::kList }, 422 }, // RvcRunMode::ModeTagStruct[]
};

const Entry<ItemInfo> _RvcRunMode_ChangeToModeRequest[] = {
  { { ContextTag(0), "newMode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::int8u
};

const Entry<ItemInfo> _RvcRunMode_ChangeToModeResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::enum8
  { { ContextTag(1), "statusText", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::char_string
};

const Entry<ItemInfo> _RvcRunMode_ModeOptionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 418 }, // RvcRunMode_ModeOptionStruct[]
};

const Entry<ItemInfo> _RvcRunMode_ModeTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 417 }, // RvcRunMode_ModeTagStruct[]
};

const Entry<ItemInfo> _RvcRunMode_ModeTag[] = {
  { { ConstantValueTag(0x4000), "kIdle", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::ModeTag::kIdle
  { { ConstantValueTag(0x4001), "kCleaning", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::ModeTag::kCleaning
};

const Entry<ItemInfo> _RvcRunMode_StatusCode[] = {
  { { ConstantValueTag(0x41), "kStuck", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::StatusCode::kStuck
  { { ConstantValueTag(0x42), "kDustBinMissing", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::StatusCode::kDustBinMissing
  { { ConstantValueTag(0x43), "kDustBinFull", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::StatusCode::kDustBinFull
  { { ConstantValueTag(0x44), "kWaterTankEmpty", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::StatusCode::kWaterTankEmpty
  { { ConstantValueTag(0x45), "kWaterTankMissing", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::StatusCode::kWaterTankMissing
  { { ConstantValueTag(0x46), "kWaterTankLidOpen", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::StatusCode::kWaterTankLidOpen
  { { ConstantValueTag(0x47), "kMopCleaningPadMissing", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::StatusCode::kMopCleaningPadMissing
  { { ConstantValueTag(0x48), "kBatteryLow", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::StatusCode::kBatteryLow
};

const Entry<ItemInfo> _RvcRunMode_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // RvcRunMode::Feature::kOnOff
};

const Entry<ItemInfo> _RvcCleanMode[] = {
  { { AttributeTag(0), "supportedModes", ItemType::kList }, 431 }, // RvcCleanMode::ModeOptionStruct[]
  { { AttributeTag(1), "currentMode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::int8u
  { { AttributeTag(3), "onMode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // RvcCleanMode::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // RvcCleanMode::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // RvcCleanMode::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // RvcCleanMode::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 435 }, // RvcCleanMode::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::int16u
  { { CommandTag(0), "ChangeToMode", ItemType::kDefault }, 429 }, // RvcCleanMode::ChangeToMode::ChangeToModeRequest
  { { CommandTag(1), "ChangeToModeResponse", ItemType::kDefault }, 430 }, // RvcCleanMode::ChangeToModeResponse
};

const Entry<ItemInfo> _RvcCleanMode_ModeTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::vendor_id
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::enum16
};

const Entry<ItemInfo> _RvcCleanMode_ModeOptionStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::char_string
  { { ContextTag(1), "mode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::int8u
  { { ContextTag(2), "modeTags", ItemType::kList }, 432 }, // RvcCleanMode::ModeTagStruct[]
};

const Entry<ItemInfo> _RvcCleanMode_ChangeToModeRequest[] = {
  { { ContextTag(0), "newMode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::int8u
};

const Entry<ItemInfo> _RvcCleanMode_ChangeToModeResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::enum8
  { { ContextTag(1), "statusText", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::char_string
};

const Entry<ItemInfo> _RvcCleanMode_ModeOptionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 428 }, // RvcCleanMode_ModeOptionStruct[]
};

const Entry<ItemInfo> _RvcCleanMode_ModeTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 427 }, // RvcCleanMode_ModeTagStruct[]
};

const Entry<ItemInfo> _RvcCleanMode_ModeTag[] = {
  { { ConstantValueTag(0x4000), "kDeepClean", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::ModeTag::kDeepClean
  { { ConstantValueTag(0x4001), "kVacuum", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::ModeTag::kVacuum
  { { ConstantValueTag(0x4002), "kMop", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::ModeTag::kMop
};

const Entry<ItemInfo> _RvcCleanMode_StatusCode[] = {
  { { ConstantValueTag(0x40), "kCleaningInProgress", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::StatusCode::kCleaningInProgress
};

const Entry<ItemInfo> _RvcCleanMode_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // RvcCleanMode::Feature::kOnOff
};

const Entry<ItemInfo> _TemperatureControl[] = {
  { { AttributeTag(0), "temperatureSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::temperature
  { { AttributeTag(1), "minTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::temperature
  { { AttributeTag(2), "maxTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::temperature
  { { AttributeTag(3), "step", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::temperature
  { { AttributeTag(4), "selectedTemperatureLevel", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::int8u
  { { AttributeTag(5), "supportedTemperatureLevels", ItemType::kList }, 1 }, // TemperatureControl::char_string[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // TemperatureControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // TemperatureControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // TemperatureControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // TemperatureControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 438 }, // TemperatureControl::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::int16u
  { { CommandTag(0), "SetTemperature", ItemType::kDefault }, 437 }, // TemperatureControl::SetTemperature::SetTemperatureRequest
};

const Entry<ItemInfo> _TemperatureControl_SetTemperatureRequest[] = {
  { { ContextTag(0), "targetTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::temperature
  { { ContextTag(1), "targetTemperatureLevel", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::int8u
};

const Entry<ItemInfo> _TemperatureControl_Feature[] = {
  { { ConstantValueTag(0x1), "kTemperatureNumber", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::Feature::kTemperatureNumber
  { { ConstantValueTag(0x2), "kTemperatureLevel", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::Feature::kTemperatureLevel
  { { ConstantValueTag(0x4), "kTemperatureStep", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureControl::Feature::kTemperatureStep
};

const Entry<ItemInfo> _RefrigeratorAlarm[] = {
  { { AttributeTag(0), "mask", ItemType::kBitmap }, 441 }, // RefrigeratorAlarm::AlarmMap
  { { AttributeTag(2), "state", ItemType::kBitmap }, 441 }, // RefrigeratorAlarm::AlarmMap
  { { AttributeTag(3), "supported", ItemType::kBitmap }, 441 }, // RefrigeratorAlarm::AlarmMap
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // RefrigeratorAlarm::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // RefrigeratorAlarm::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // RefrigeratorAlarm::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // RefrigeratorAlarm::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAlarm::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAlarm::int16u
  { { EventTag(0), "Notify", ItemType::kDefault }, 440 }, // RefrigeratorAlarm::Notify
};

const Entry<ItemInfo> _RefrigeratorAlarm_Notify[] = {
  { { ContextTag(0), "active", ItemType::kBitmap }, 441 }, // RefrigeratorAlarm::AlarmMap
  { { ContextTag(1), "inactive", ItemType::kBitmap }, 441 }, // RefrigeratorAlarm::AlarmMap
  { { ContextTag(2), "state", ItemType::kBitmap }, 441 }, // RefrigeratorAlarm::AlarmMap
  { { ContextTag(3), "mask", ItemType::kBitmap }, 441 }, // RefrigeratorAlarm::AlarmMap
};

const Entry<ItemInfo> _RefrigeratorAlarm_AlarmMap[] = {
  { { ConstantValueTag(0x1), "kDoorOpen", ItemType::kDefault }, kInvalidNodeIndex }, // RefrigeratorAlarm::AlarmMap::kDoorOpen
};

const Entry<ItemInfo> _DishwasherMode[] = {
  { { AttributeTag(0), "supportedModes", ItemType::kList }, 447 }, // DishwasherMode::ModeOptionStruct[]
  { { AttributeTag(1), "currentMode", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::int8u
  { { AttributeTag(2), "startUpMode", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::int8u
  { { AttributeTag(3), "onMode", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // DishwasherMode::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // DishwasherMode::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // DishwasherMode::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // DishwasherMode::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 450 }, // DishwasherMode::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::int16u
  { { CommandTag(0), "ChangeToMode", ItemType::kDefault }, 445 }, // DishwasherMode::ChangeToMode::ChangeToModeRequest
  { { CommandTag(1), "ChangeToModeResponse", ItemType::kDefault }, 446 }, // DishwasherMode::ChangeToModeResponse
};

const Entry<ItemInfo> _DishwasherMode_ModeTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::vendor_id
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::enum16
};

const Entry<ItemInfo> _DishwasherMode_ModeOptionStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::char_string
  { { ContextTag(1), "mode", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::int8u
  { { ContextTag(2), "modeTags", ItemType::kList }, 448 }, // DishwasherMode::ModeTagStruct[]
};

const Entry<ItemInfo> _DishwasherMode_ChangeToModeRequest[] = {
  { { ContextTag(0), "newMode", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::int8u
};

const Entry<ItemInfo> _DishwasherMode_ChangeToModeResponse[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::enum8
  { { ContextTag(1), "statusText", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::char_string
};

const Entry<ItemInfo> _DishwasherMode_ModeOptionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 444 }, // DishwasherMode_ModeOptionStruct[]
};

const Entry<ItemInfo> _DishwasherMode_ModeTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 443 }, // DishwasherMode_ModeTagStruct[]
};

const Entry<ItemInfo> _DishwasherMode_ModeTag[] = {
  { { ConstantValueTag(0x4000), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::ModeTag::kNormal
  { { ConstantValueTag(0x4001), "kHeavy", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::ModeTag::kHeavy
  { { ConstantValueTag(0x4002), "kLight", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::ModeTag::kLight
};

const Entry<ItemInfo> _DishwasherMode_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherMode::Feature::kOnOff
};

const Entry<ItemInfo> _AirQuality[] = {
  { { AttributeTag(0), "airQuality", ItemType::kEnum }, 452 }, // AirQuality::AirQualityEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // AirQuality::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // AirQuality::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // AirQuality::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // AirQuality::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 453 }, // AirQuality::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::int16u
};

const Entry<ItemInfo> _AirQuality_AirQualityEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::AirQualityEnum::kUnknown
  { { ConstantValueTag(0x1), "kGood", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::AirQualityEnum::kGood
  { { ConstantValueTag(0x2), "kFair", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::AirQualityEnum::kFair
  { { ConstantValueTag(0x3), "kModerate", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::AirQualityEnum::kModerate
  { { ConstantValueTag(0x4), "kPoor", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::AirQualityEnum::kPoor
  { { ConstantValueTag(0x5), "kVeryPoor", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::AirQualityEnum::kVeryPoor
  { { ConstantValueTag(0x6), "kExtremelyPoor", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::AirQualityEnum::kExtremelyPoor
};

const Entry<ItemInfo> _AirQuality_Feature[] = {
  { { ConstantValueTag(0x1), "kFair", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::Feature::kFair
  { { ConstantValueTag(0x2), "kModerate", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::Feature::kModerate
  { { ConstantValueTag(0x4), "kVeryPoor", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::Feature::kVeryPoor
  { { ConstantValueTag(0x8), "kExtremelyPoor", ItemType::kDefault }, kInvalidNodeIndex }, // AirQuality::Feature::kExtremelyPoor
};

const Entry<ItemInfo> _SmokeCoAlarm[] = {
  { { AttributeTag(0), "expressedState", ItemType::kEnum }, 463 }, // SmokeCoAlarm::ExpressedStateEnum
  { { AttributeTag(1), "smokeState", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
  { { AttributeTag(2), "COState", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
  { { AttributeTag(3), "batteryAlert", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
  { { AttributeTag(4), "deviceMuted", ItemType::kEnum }, 464 }, // SmokeCoAlarm::MuteStateEnum
  { { AttributeTag(5), "testInProgress", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::boolean
  { { AttributeTag(6), "hardwareFaultAlert", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::boolean
  { { AttributeTag(7), "endOfServiceAlert", ItemType::kEnum }, 462 }, // SmokeCoAlarm::EndOfServiceEnum
  { { AttributeTag(8), "interconnectSmokeAlarm", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
  { { AttributeTag(9), "interconnectCOAlarm", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
  { { AttributeTag(10), "contaminationState", ItemType::kEnum }, 461 }, // SmokeCoAlarm::ContaminationStateEnum
  { { AttributeTag(11), "smokeSensitivityLevel", ItemType::kEnum }, 465 }, // SmokeCoAlarm::SensitivityEnum
  { { AttributeTag(12), "expiryDate", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::epoch_s
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // SmokeCoAlarm::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // SmokeCoAlarm::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // SmokeCoAlarm::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // SmokeCoAlarm::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 466 }, // SmokeCoAlarm::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::int16u
  { { EventTag(0), "SmokeAlarm", ItemType::kDefault }, 455 }, // SmokeCoAlarm::SmokeAlarm
  { { EventTag(1), "COAlarm", ItemType::kDefault }, 456 }, // SmokeCoAlarm::COAlarm
  { { EventTag(2), "LowBattery", ItemType::kDefault }, 457 }, // SmokeCoAlarm::LowBattery
  { { EventTag(8), "InterconnectSmokeAlarm", ItemType::kDefault }, 458 }, // SmokeCoAlarm::InterconnectSmokeAlarm
  { { EventTag(9), "InterconnectCOAlarm", ItemType::kDefault }, 459 }, // SmokeCoAlarm::InterconnectCOAlarm
  { { CommandTag(0), "SelfTestRequest", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::SelfTestRequest::()
};

const Entry<ItemInfo> _SmokeCoAlarm_SmokeAlarm[] = {
  { { ContextTag(0), "alarmSeverityLevel", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
};

const Entry<ItemInfo> _SmokeCoAlarm_COAlarm[] = {
  { { ContextTag(0), "alarmSeverityLevel", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
};

const Entry<ItemInfo> _SmokeCoAlarm_LowBattery[] = {
  { { ContextTag(0), "alarmSeverityLevel", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
};

const Entry<ItemInfo> _SmokeCoAlarm_InterconnectSmokeAlarm[] = {
  { { ContextTag(0), "alarmSeverityLevel", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
};

const Entry<ItemInfo> _SmokeCoAlarm_InterconnectCOAlarm[] = {
  { { ContextTag(0), "alarmSeverityLevel", ItemType::kEnum }, 460 }, // SmokeCoAlarm::AlarmStateEnum
};

const Entry<ItemInfo> _SmokeCoAlarm_AlarmStateEnum[] = {
  { { ConstantValueTag(0x0), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::AlarmStateEnum::kNormal
  { { ConstantValueTag(0x1), "kWarning", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::AlarmStateEnum::kWarning
  { { ConstantValueTag(0x2), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::AlarmStateEnum::kCritical
};

const Entry<ItemInfo> _SmokeCoAlarm_ContaminationStateEnum[] = {
  { { ConstantValueTag(0x0), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ContaminationStateEnum::kNormal
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ContaminationStateEnum::kLow
  { { ConstantValueTag(0x2), "kWarning", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ContaminationStateEnum::kWarning
  { { ConstantValueTag(0x3), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ContaminationStateEnum::kCritical
};

const Entry<ItemInfo> _SmokeCoAlarm_EndOfServiceEnum[] = {
  { { ConstantValueTag(0x0), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::EndOfServiceEnum::kNormal
  { { ConstantValueTag(0x1), "kExpired", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::EndOfServiceEnum::kExpired
};

const Entry<ItemInfo> _SmokeCoAlarm_ExpressedStateEnum[] = {
  { { ConstantValueTag(0x0), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kNormal
  { { ConstantValueTag(0x1), "kSmokeAlarm", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kSmokeAlarm
  { { ConstantValueTag(0x2), "kCOAlarm", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kCOAlarm
  { { ConstantValueTag(0x3), "kBatteryAlert", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kBatteryAlert
  { { ConstantValueTag(0x4), "kTesting", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kTesting
  { { ConstantValueTag(0x5), "kHardwareFault", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kHardwareFault
  { { ConstantValueTag(0x6), "kEndOfService", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kEndOfService
  { { ConstantValueTag(0x7), "kInterconnectSmoke", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kInterconnectSmoke
  { { ConstantValueTag(0x8), "kInterconnectCO", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::ExpressedStateEnum::kInterconnectCO
};

const Entry<ItemInfo> _SmokeCoAlarm_MuteStateEnum[] = {
  { { ConstantValueTag(0x0), "kNotMuted", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::MuteStateEnum::kNotMuted
  { { ConstantValueTag(0x1), "kMuted", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::MuteStateEnum::kMuted
};

const Entry<ItemInfo> _SmokeCoAlarm_SensitivityEnum[] = {
  { { ConstantValueTag(0x0), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::SensitivityEnum::kHigh
  { { ConstantValueTag(0x1), "kStandard", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::SensitivityEnum::kStandard
  { { ConstantValueTag(0x2), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::SensitivityEnum::kLow
};

const Entry<ItemInfo> _SmokeCoAlarm_Feature[] = {
  { { ConstantValueTag(0x1), "kSmokeAlarm", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::Feature::kSmokeAlarm
  { { ConstantValueTag(0x2), "kCOAlarm", ItemType::kDefault }, kInvalidNodeIndex }, // SmokeCoAlarm::Feature::kCOAlarm
};

const Entry<ItemInfo> _DishwasherAlarm[] = {
  { { AttributeTag(0), "mask", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
  { { AttributeTag(1), "latch", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
  { { AttributeTag(2), "state", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
  { { AttributeTag(3), "supported", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // DishwasherAlarm::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // DishwasherAlarm::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // DishwasherAlarm::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // DishwasherAlarm::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 472 }, // DishwasherAlarm::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherAlarm::int16u
  { { EventTag(0), "Notify", ItemType::kDefault }, 470 }, // DishwasherAlarm::Notify
  { { CommandTag(0), "Reset", ItemType::kDefault }, 468 }, // DishwasherAlarm::Reset::ResetRequest
  { { CommandTag(1), "ModifyEnabledAlarms", ItemType::kDefault }, 469 }, // DishwasherAlarm::ModifyEnabledAlarms::ModifyEnabledAlarmsRequest
};

const Entry<ItemInfo> _DishwasherAlarm_ResetRequest[] = {
  { { ContextTag(0), "alarms", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
};

const Entry<ItemInfo> _DishwasherAlarm_ModifyEnabledAlarmsRequest[] = {
  { { ContextTag(0), "mask", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
};

const Entry<ItemInfo> _DishwasherAlarm_Notify[] = {
  { { ContextTag(0), "active", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
  { { ContextTag(1), "inactive", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
  { { ContextTag(2), "state", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
  { { ContextTag(3), "mask", ItemType::kBitmap }, 471 }, // DishwasherAlarm::AlarmMap
};

const Entry<ItemInfo> _DishwasherAlarm_AlarmMap[] = {
  { { ConstantValueTag(0x1), "kInflowError", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherAlarm::AlarmMap::kInflowError
  { { ConstantValueTag(0x2), "kDrainError", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherAlarm::AlarmMap::kDrainError
  { { ConstantValueTag(0x4), "kDoorError", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherAlarm::AlarmMap::kDoorError
  { { ConstantValueTag(0x8), "kTempTooLow", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherAlarm::AlarmMap::kTempTooLow
  { { ConstantValueTag(0x10), "kTempTooHigh", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherAlarm::AlarmMap::kTempTooHigh
  { { ConstantValueTag(0x20), "kWaterLevelError", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherAlarm::AlarmMap::kWaterLevelError
};

const Entry<ItemInfo> _DishwasherAlarm_Feature[] = {
  { { ConstantValueTag(0x1), "kReset", ItemType::kDefault }, kInvalidNodeIndex }, // DishwasherAlarm::Feature::kReset
};

const Entry<ItemInfo> _MicrowaveOvenMode[] = {
  { { AttributeTag(0), "supportedModes", ItemType::kList }, 477 }, // MicrowaveOvenMode::ModeOptionStruct[]
  { { AttributeTag(1), "currentMode", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // MicrowaveOvenMode::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // MicrowaveOvenMode::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // MicrowaveOvenMode::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // MicrowaveOvenMode::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 479 }, // MicrowaveOvenMode::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::int16u
};

const Entry<ItemInfo> _MicrowaveOvenMode_ModeTagStruct[] = {
  { { ContextTag(0), "mfgCode", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::vendor_id
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::enum16
};

const Entry<ItemInfo> _MicrowaveOvenMode_ModeOptionStruct[] = {
  { { ContextTag(0), "label", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::char_string
  { { ContextTag(1), "mode", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::int8u
  { { ContextTag(2), "modeTags", ItemType::kList }, 476 }, // MicrowaveOvenMode::ModeTagStruct[]
};

const Entry<ItemInfo> _MicrowaveOvenMode_ModeTagStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 474 }, // MicrowaveOvenMode_ModeTagStruct[]
};

const Entry<ItemInfo> _MicrowaveOvenMode_ModeOptionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 475 }, // MicrowaveOvenMode_ModeOptionStruct[]
};

const Entry<ItemInfo> _MicrowaveOvenMode_ModeTag[] = {
  { { ConstantValueTag(0x4000), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::ModeTag::kNormal
  { { ConstantValueTag(0x4001), "kDefrost", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::ModeTag::kDefrost
};

const Entry<ItemInfo> _MicrowaveOvenMode_Feature[] = {
  { { ConstantValueTag(0x1), "kOnOff", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenMode::Feature::kOnOff
};

const Entry<ItemInfo> _MicrowaveOvenControl[] = {
  { { AttributeTag(1), "cookTime", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::elapsed_s
  { { AttributeTag(2), "powerSetting", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::int8u
  { { AttributeTag(3), "minPower", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::int8u
  { { AttributeTag(4), "maxPower", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::int8u
  { { AttributeTag(5), "powerStep", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // MicrowaveOvenControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // MicrowaveOvenControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // MicrowaveOvenControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // MicrowaveOvenControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::int16u
  { { CommandTag(0), "SetCookingParameters", ItemType::kDefault }, 481 }, // MicrowaveOvenControl::SetCookingParameters::SetCookingParametersRequest
  { { CommandTag(1), "AddMoreTime", ItemType::kDefault }, 482 }, // MicrowaveOvenControl::AddMoreTime::AddMoreTimeRequest
};

const Entry<ItemInfo> _MicrowaveOvenControl_SetCookingParametersRequest[] = {
  { { ContextTag(0), "cookMode", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::int8u
  { { ContextTag(1), "cookTime", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::elapsed_s
  { { ContextTag(2), "powerSetting", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::int8u
};

const Entry<ItemInfo> _MicrowaveOvenControl_AddMoreTimeRequest[] = {
  { { ContextTag(0), "timeToAdd", ItemType::kDefault }, kInvalidNodeIndex }, // MicrowaveOvenControl::elapsed_s
};

const Entry<ItemInfo> _OperationalState[] = {
  { { AttributeTag(0), "phaseList", ItemType::kList }, 1 }, // OperationalState::char_string[]
  { { AttributeTag(1), "currentPhase", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::int8u
  { { AttributeTag(2), "countdownTime", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::elapsed_s
  { { AttributeTag(3), "operationalStateList", ItemType::kList }, 489 }, // OperationalState::OperationalStateStruct[]
  { { AttributeTag(4), "operationalState", ItemType::kEnum }, 491 }, // OperationalState::OperationalStateEnum
  { { AttributeTag(5), "operationalError", ItemType::kDefault }, 484 }, // OperationalState::ErrorStateStruct
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OperationalState::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OperationalState::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OperationalState::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OperationalState::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::int16u
  { { EventTag(0), "OperationalError", ItemType::kDefault }, 487 }, // OperationalState::OperationalError
  { { EventTag(1), "OperationCompletion", ItemType::kDefault }, 488 }, // OperationalState::OperationCompletion
  { { CommandTag(0), "Pause", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::Pause::()
  { { CommandTag(1), "Stop", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::Stop::()
  { { CommandTag(2), "Start", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::Start::()
  { { CommandTag(3), "Resume", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::Resume::()
  { { CommandTag(4), "OperationalCommandResponse", ItemType::kDefault }, 486 }, // OperationalState::OperationalCommandResponse
};

const Entry<ItemInfo> _OperationalState_ErrorStateStruct[] = {
  { { ContextTag(0), "errorStateID", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::enum8
  { { ContextTag(1), "errorStateLabel", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::char_string
  { { ContextTag(2), "errorStateDetails", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::char_string
};

const Entry<ItemInfo> _OperationalState_OperationalStateStruct[] = {
  { { ContextTag(0), "operationalStateID", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::enum8
  { { ContextTag(1), "operationalStateLabel", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::char_string
};

const Entry<ItemInfo> _OperationalState_OperationalCommandResponse[] = {
  { { ContextTag(0), "commandResponseState", ItemType::kDefault }, 484 }, // OperationalState::ErrorStateStruct
};

const Entry<ItemInfo> _OperationalState_OperationalError[] = {
  { { ContextTag(0), "errorState", ItemType::kDefault }, 484 }, // OperationalState::ErrorStateStruct
};

const Entry<ItemInfo> _OperationalState_OperationCompletion[] = {
  { { ContextTag(0), "completionErrorCode", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::enum8
  { { ContextTag(1), "totalOperationalTime", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::elapsed_s
  { { ContextTag(2), "pausedTime", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::elapsed_s
};

const Entry<ItemInfo> _OperationalState_OperationalStateStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 485 }, // OperationalState_OperationalStateStruct[]
};

const Entry<ItemInfo> _OperationalState_ErrorStateEnum[] = {
  { { ConstantValueTag(0x0), "kNoError", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::ErrorStateEnum::kNoError
  { { ConstantValueTag(0x1), "kUnableToStartOrResume", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::ErrorStateEnum::kUnableToStartOrResume
  { { ConstantValueTag(0x2), "kUnableToCompleteOperation", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::ErrorStateEnum::kUnableToCompleteOperation
  { { ConstantValueTag(0x3), "kCommandInvalidInState", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::ErrorStateEnum::kCommandInvalidInState
};

const Entry<ItemInfo> _OperationalState_OperationalStateEnum[] = {
  { { ConstantValueTag(0x0), "kStopped", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::OperationalStateEnum::kStopped
  { { ConstantValueTag(0x1), "kRunning", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::OperationalStateEnum::kRunning
  { { ConstantValueTag(0x2), "kPaused", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::OperationalStateEnum::kPaused
  { { ConstantValueTag(0x3), "kError", ItemType::kDefault }, kInvalidNodeIndex }, // OperationalState::OperationalStateEnum::kError
};

const Entry<ItemInfo> _RvcOperationalState[] = {
  { { AttributeTag(0), "phaseList", ItemType::kList }, 1 }, // RvcOperationalState::char_string[]
  { { AttributeTag(1), "currentPhase", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::int8u
  { { AttributeTag(2), "countdownTime", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::elapsed_s
  { { AttributeTag(3), "operationalStateList", ItemType::kList }, 498 }, // RvcOperationalState::OperationalStateStruct[]
  { { AttributeTag(4), "operationalState", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::enum8
  { { AttributeTag(5), "operationalError", ItemType::kDefault }, 493 }, // RvcOperationalState::ErrorStateStruct
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // RvcOperationalState::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // RvcOperationalState::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // RvcOperationalState::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // RvcOperationalState::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::int16u
  { { EventTag(0), "OperationalError", ItemType::kDefault }, 496 }, // RvcOperationalState::OperationalError
  { { EventTag(1), "OperationCompletion", ItemType::kDefault }, 497 }, // RvcOperationalState::OperationCompletion
  { { CommandTag(0), "Pause", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::Pause::()
  { { CommandTag(1), "Stop", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::Stop::()
  { { CommandTag(2), "Start", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::Start::()
  { { CommandTag(3), "Resume", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::Resume::()
  { { CommandTag(4), "OperationalCommandResponse", ItemType::kDefault }, 495 }, // RvcOperationalState::OperationalCommandResponse
};

const Entry<ItemInfo> _RvcOperationalState_ErrorStateStruct[] = {
  { { ContextTag(0), "errorStateID", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::enum8
  { { ContextTag(1), "errorStateLabel", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::char_string
  { { ContextTag(2), "errorStateDetails", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::char_string
};

const Entry<ItemInfo> _RvcOperationalState_OperationalStateStruct[] = {
  { { ContextTag(0), "operationalStateID", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::enum8
  { { ContextTag(1), "operationalStateLabel", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::char_string
};

const Entry<ItemInfo> _RvcOperationalState_OperationalCommandResponse[] = {
  { { ContextTag(0), "commandResponseState", ItemType::kDefault }, 493 }, // RvcOperationalState::ErrorStateStruct
};

const Entry<ItemInfo> _RvcOperationalState_OperationalError[] = {
  { { ContextTag(0), "errorState", ItemType::kDefault }, 493 }, // RvcOperationalState::ErrorStateStruct
};

const Entry<ItemInfo> _RvcOperationalState_OperationCompletion[] = {
  { { ContextTag(0), "completionErrorCode", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::enum8
  { { ContextTag(1), "totalOperationalTime", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::elapsed_s
  { { ContextTag(2), "pausedTime", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::elapsed_s
};

const Entry<ItemInfo> _RvcOperationalState_OperationalStateStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 494 }, // RvcOperationalState_OperationalStateStruct[]
};

const Entry<ItemInfo> _RvcOperationalState_ErrorStateEnum[] = {
  { { ConstantValueTag(0x40), "kFailedToFindChargingDock", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::ErrorStateEnum::kFailedToFindChargingDock
  { { ConstantValueTag(0x41), "kStuck", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::ErrorStateEnum::kStuck
  { { ConstantValueTag(0x42), "kDustBinMissing", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::ErrorStateEnum::kDustBinMissing
  { { ConstantValueTag(0x43), "kDustBinFull", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::ErrorStateEnum::kDustBinFull
  { { ConstantValueTag(0x44), "kWaterTankEmpty", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::ErrorStateEnum::kWaterTankEmpty
  { { ConstantValueTag(0x45), "kWaterTankMissing", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::ErrorStateEnum::kWaterTankMissing
  { { ConstantValueTag(0x46), "kWaterTankLidOpen", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::ErrorStateEnum::kWaterTankLidOpen
  { { ConstantValueTag(0x47), "kMopCleaningPadMissing", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::ErrorStateEnum::kMopCleaningPadMissing
};

const Entry<ItemInfo> _RvcOperationalState_OperationalStateEnum[] = {
  { { ConstantValueTag(0x40), "kSeekingCharger", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::OperationalStateEnum::kSeekingCharger
  { { ConstantValueTag(0x41), "kCharging", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::OperationalStateEnum::kCharging
  { { ConstantValueTag(0x42), "kDocked", ItemType::kDefault }, kInvalidNodeIndex }, // RvcOperationalState::OperationalStateEnum::kDocked
};

const Entry<ItemInfo> _HepaFilterMonitoring[] = {
  { { AttributeTag(0), "condition", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::percent
  { { AttributeTag(1), "degradationDirection", ItemType::kEnum }, 505 }, // HepaFilterMonitoring::DegradationDirectionEnum
  { { AttributeTag(2), "changeIndication", ItemType::kEnum }, 504 }, // HepaFilterMonitoring::ChangeIndicationEnum
  { { AttributeTag(3), "inPlaceIndicator", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::boolean
  { { AttributeTag(4), "lastChangedTime", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::epoch_s
  { { AttributeTag(5), "replacementProductList", ItemType::kList }, 503 }, // HepaFilterMonitoring::ReplacementProductStruct[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // HepaFilterMonitoring::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // HepaFilterMonitoring::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // HepaFilterMonitoring::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // HepaFilterMonitoring::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 507 }, // HepaFilterMonitoring::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::int16u
  { { CommandTag(0), "ResetCondition", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ResetCondition::()
};

const Entry<ItemInfo> _HepaFilterMonitoring_ReplacementProductStruct[] = {
  { { ContextTag(0), "productIdentifierType", ItemType::kEnum }, 506 }, // HepaFilterMonitoring::ProductIdentifierTypeEnum
  { { ContextTag(1), "productIdentifierValue", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::char_string
};

const Entry<ItemInfo> _HepaFilterMonitoring_ReplacementProductStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 502 }, // HepaFilterMonitoring_ReplacementProductStruct[]
};

const Entry<ItemInfo> _HepaFilterMonitoring_ChangeIndicationEnum[] = {
  { { ConstantValueTag(0x0), "kOK", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ChangeIndicationEnum::kOK
  { { ConstantValueTag(0x1), "kWarning", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ChangeIndicationEnum::kWarning
  { { ConstantValueTag(0x2), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ChangeIndicationEnum::kCritical
};

const Entry<ItemInfo> _HepaFilterMonitoring_DegradationDirectionEnum[] = {
  { { ConstantValueTag(0x0), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::DegradationDirectionEnum::kUp
  { { ConstantValueTag(0x1), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::DegradationDirectionEnum::kDown
};

const Entry<ItemInfo> _HepaFilterMonitoring_ProductIdentifierTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUPC", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ProductIdentifierTypeEnum::kUPC
  { { ConstantValueTag(0x1), "kGTIN8", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ProductIdentifierTypeEnum::kGTIN8
  { { ConstantValueTag(0x2), "kEAN", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ProductIdentifierTypeEnum::kEAN
  { { ConstantValueTag(0x3), "kGTIN14", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ProductIdentifierTypeEnum::kGTIN14
  { { ConstantValueTag(0x4), "kOEM", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::ProductIdentifierTypeEnum::kOEM
};

const Entry<ItemInfo> _HepaFilterMonitoring_Feature[] = {
  { { ConstantValueTag(0x1), "kCondition", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::Feature::kCondition
  { { ConstantValueTag(0x2), "kWarning", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::Feature::kWarning
  { { ConstantValueTag(0x4), "kReplacementProductList", ItemType::kDefault }, kInvalidNodeIndex }, // HepaFilterMonitoring::Feature::kReplacementProductList
};

const Entry<ItemInfo> _ActivatedCarbonFilterMonitoring[] = {
  { { AttributeTag(0), "condition", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::percent
  { { AttributeTag(1), "degradationDirection", ItemType::kEnum }, 512 }, // ActivatedCarbonFilterMonitoring::DegradationDirectionEnum
  { { AttributeTag(2), "changeIndication", ItemType::kEnum }, 511 }, // ActivatedCarbonFilterMonitoring::ChangeIndicationEnum
  { { AttributeTag(3), "inPlaceIndicator", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::boolean
  { { AttributeTag(4), "lastChangedTime", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::epoch_s
  { { AttributeTag(5), "replacementProductList", ItemType::kList }, 510 }, // ActivatedCarbonFilterMonitoring::ReplacementProductStruct[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ActivatedCarbonFilterMonitoring::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ActivatedCarbonFilterMonitoring::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ActivatedCarbonFilterMonitoring::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ActivatedCarbonFilterMonitoring::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 514 }, // ActivatedCarbonFilterMonitoring::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::int16u
  { { CommandTag(0), "ResetCondition", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ResetCondition::()
};

const Entry<ItemInfo> _ActivatedCarbonFilterMonitoring_ReplacementProductStruct[] = {
  { { ContextTag(0), "productIdentifierType", ItemType::kEnum }, 513 }, // ActivatedCarbonFilterMonitoring::ProductIdentifierTypeEnum
  { { ContextTag(1), "productIdentifierValue", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::char_string
};

const Entry<ItemInfo> _ActivatedCarbonFilterMonitoring_ReplacementProductStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 509 }, // ActivatedCarbonFilterMonitoring_ReplacementProductStruct[]
};

const Entry<ItemInfo> _ActivatedCarbonFilterMonitoring_ChangeIndicationEnum[] = {
  { { ConstantValueTag(0x0), "kOK", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ChangeIndicationEnum::kOK
  { { ConstantValueTag(0x1), "kWarning", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ChangeIndicationEnum::kWarning
  { { ConstantValueTag(0x2), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ChangeIndicationEnum::kCritical
};

const Entry<ItemInfo> _ActivatedCarbonFilterMonitoring_DegradationDirectionEnum[] = {
  { { ConstantValueTag(0x0), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::DegradationDirectionEnum::kUp
  { { ConstantValueTag(0x1), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::DegradationDirectionEnum::kDown
};

const Entry<ItemInfo> _ActivatedCarbonFilterMonitoring_ProductIdentifierTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUPC", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ProductIdentifierTypeEnum::kUPC
  { { ConstantValueTag(0x1), "kGTIN8", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ProductIdentifierTypeEnum::kGTIN8
  { { ConstantValueTag(0x2), "kEAN", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ProductIdentifierTypeEnum::kEAN
  { { ConstantValueTag(0x3), "kGTIN14", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ProductIdentifierTypeEnum::kGTIN14
  { { ConstantValueTag(0x4), "kOEM", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::ProductIdentifierTypeEnum::kOEM
};

const Entry<ItemInfo> _ActivatedCarbonFilterMonitoring_Feature[] = {
  { { ConstantValueTag(0x1), "kCondition", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::Feature::kCondition
  { { ConstantValueTag(0x2), "kWarning", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::Feature::kWarning
  { { ConstantValueTag(0x4), "kReplacementProductList", ItemType::kDefault }, kInvalidNodeIndex }, // ActivatedCarbonFilterMonitoring::Feature::kReplacementProductList
};

const Entry<ItemInfo> _BooleanStateConfiguration[] = {
  { { AttributeTag(0), "currentSensitivityLevel", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::int8u
  { { AttributeTag(1), "supportedSensitivityLevels", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::int8u
  { { AttributeTag(2), "defaultSensitivityLevel", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::int8u
  { { AttributeTag(3), "alarmsActive", ItemType::kBitmap }, 520 }, // BooleanStateConfiguration::AlarmModeBitmap
  { { AttributeTag(4), "alarmsSuppressed", ItemType::kBitmap }, 520 }, // BooleanStateConfiguration::AlarmModeBitmap
  { { AttributeTag(5), "alarmsEnabled", ItemType::kBitmap }, 520 }, // BooleanStateConfiguration::AlarmModeBitmap
  { { AttributeTag(6), "alarmsSupported", ItemType::kBitmap }, 520 }, // BooleanStateConfiguration::AlarmModeBitmap
  { { AttributeTag(7), "sensorFault", ItemType::kBitmap }, 522 }, // BooleanStateConfiguration::SensorFaultBitmap
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // BooleanStateConfiguration::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // BooleanStateConfiguration::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // BooleanStateConfiguration::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // BooleanStateConfiguration::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 521 }, // BooleanStateConfiguration::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::int16u
  { { EventTag(0), "AlarmsStateChanged", ItemType::kDefault }, 518 }, // BooleanStateConfiguration::AlarmsStateChanged
  { { EventTag(1), "SensorFault", ItemType::kDefault }, 519 }, // BooleanStateConfiguration::SensorFault
  { { CommandTag(0), "SuppressAlarm", ItemType::kDefault }, 516 }, // BooleanStateConfiguration::SuppressAlarm::SuppressAlarmRequest
  { { CommandTag(1), "EnableDisableAlarm", ItemType::kDefault }, 517 }, // BooleanStateConfiguration::EnableDisableAlarm::EnableDisableAlarmRequest
};

const Entry<ItemInfo> _BooleanStateConfiguration_SuppressAlarmRequest[] = {
  { { ContextTag(0), "alarmsToSuppress", ItemType::kBitmap }, 520 }, // BooleanStateConfiguration::AlarmModeBitmap
};

const Entry<ItemInfo> _BooleanStateConfiguration_EnableDisableAlarmRequest[] = {
  { { ContextTag(0), "alarmsToEnableDisable", ItemType::kBitmap }, 520 }, // BooleanStateConfiguration::AlarmModeBitmap
};

const Entry<ItemInfo> _BooleanStateConfiguration_AlarmsStateChanged[] = {
  { { ContextTag(0), "alarmsActive", ItemType::kBitmap }, 520 }, // BooleanStateConfiguration::AlarmModeBitmap
  { { ContextTag(1), "alarmsSuppressed", ItemType::kBitmap }, 520 }, // BooleanStateConfiguration::AlarmModeBitmap
};

const Entry<ItemInfo> _BooleanStateConfiguration_SensorFault[] = {
  { { ContextTag(0), "sensorFault", ItemType::kBitmap }, 522 }, // BooleanStateConfiguration::SensorFaultBitmap
};

const Entry<ItemInfo> _BooleanStateConfiguration_AlarmModeBitmap[] = {
  { { ConstantValueTag(0x1), "kVisual", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::AlarmModeBitmap::kVisual
  { { ConstantValueTag(0x2), "kAudible", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::AlarmModeBitmap::kAudible
};

const Entry<ItemInfo> _BooleanStateConfiguration_Feature[] = {
  { { ConstantValueTag(0x1), "kVisual", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::Feature::kVisual
  { { ConstantValueTag(0x2), "kAudible", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::Feature::kAudible
  { { ConstantValueTag(0x4), "kAlarmSuppress", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::Feature::kAlarmSuppress
  { { ConstantValueTag(0x8), "kSensitivityLevel", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::Feature::kSensitivityLevel
};

const Entry<ItemInfo> _BooleanStateConfiguration_SensorFaultBitmap[] = {
  { { ConstantValueTag(0x1), "kGeneralFault", ItemType::kDefault }, kInvalidNodeIndex }, // BooleanStateConfiguration::SensorFaultBitmap::kGeneralFault
};

const Entry<ItemInfo> _ValveConfigurationAndControl[] = {
  { { AttributeTag(0), "openDuration", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::elapsed_s
  { { AttributeTag(1), "defaultOpenDuration", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::elapsed_s
  { { AttributeTag(2), "autoCloseTime", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::epoch_us
  { { AttributeTag(3), "remainingDuration", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::elapsed_s
  { { AttributeTag(4), "currentState", ItemType::kEnum }, 528 }, // ValveConfigurationAndControl::ValveStateEnum
  { { AttributeTag(5), "targetState", ItemType::kEnum }, 528 }, // ValveConfigurationAndControl::ValveStateEnum
  { { AttributeTag(6), "currentLevel", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::percent
  { { AttributeTag(7), "targetLevel", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::percent
  { { AttributeTag(8), "defaultOpenLevel", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::percent
  { { AttributeTag(9), "valveFault", ItemType::kBitmap }, 530 }, // ValveConfigurationAndControl::ValveFaultBitmap
  { { AttributeTag(10), "levelStep", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ValveConfigurationAndControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ValveConfigurationAndControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ValveConfigurationAndControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ValveConfigurationAndControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 529 }, // ValveConfigurationAndControl::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::int16u
  { { EventTag(0), "ValveStateChanged", ItemType::kDefault }, 525 }, // ValveConfigurationAndControl::ValveStateChanged
  { { EventTag(1), "ValveFault", ItemType::kDefault }, 526 }, // ValveConfigurationAndControl::ValveFault
  { { CommandTag(0), "Open", ItemType::kDefault }, 524 }, // ValveConfigurationAndControl::Open::OpenRequest
  { { CommandTag(1), "Close", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::Close::()
};

const Entry<ItemInfo> _ValveConfigurationAndControl_OpenRequest[] = {
  { { ContextTag(0), "openDuration", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::elapsed_s
  { { ContextTag(1), "targetLevel", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::percent
};

const Entry<ItemInfo> _ValveConfigurationAndControl_ValveStateChanged[] = {
  { { ContextTag(0), "valveState", ItemType::kEnum }, 528 }, // ValveConfigurationAndControl::ValveStateEnum
  { { ContextTag(1), "valveLevel", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::percent
};

const Entry<ItemInfo> _ValveConfigurationAndControl_ValveFault[] = {
  { { ContextTag(0), "valveFault", ItemType::kBitmap }, 530 }, // ValveConfigurationAndControl::ValveFaultBitmap
};

const Entry<ItemInfo> _ValveConfigurationAndControl_StatusCodeEnum[] = {
  { { ConstantValueTag(0x2), "kFailureDueToFault", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::StatusCodeEnum::kFailureDueToFault
};

const Entry<ItemInfo> _ValveConfigurationAndControl_ValveStateEnum[] = {
  { { ConstantValueTag(0x0), "kClosed", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveStateEnum::kClosed
  { { ConstantValueTag(0x1), "kOpen", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveStateEnum::kOpen
  { { ConstantValueTag(0x2), "kTransitioning", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveStateEnum::kTransitioning
};

const Entry<ItemInfo> _ValveConfigurationAndControl_Feature[] = {
  { { ConstantValueTag(0x1), "kTimeSync", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::Feature::kTimeSync
  { { ConstantValueTag(0x2), "kLevel", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::Feature::kLevel
};

const Entry<ItemInfo> _ValveConfigurationAndControl_ValveFaultBitmap[] = {
  { { ConstantValueTag(0x1), "kGeneralFault", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveFaultBitmap::kGeneralFault
  { { ConstantValueTag(0x2), "kBlocked", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveFaultBitmap::kBlocked
  { { ConstantValueTag(0x4), "kLeaking", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveFaultBitmap::kLeaking
  { { ConstantValueTag(0x8), "kNotConnected", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveFaultBitmap::kNotConnected
  { { ConstantValueTag(0x10), "kShortCircuit", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveFaultBitmap::kShortCircuit
  { { ConstantValueTag(0x20), "kCurrentExceeded", ItemType::kDefault }, kInvalidNodeIndex }, // ValveConfigurationAndControl::ValveFaultBitmap::kCurrentExceeded
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement[] = {
  { { AttributeTag(0), "accuracy", ItemType::kDefault }, 533 }, // ElectricalEnergyMeasurement::MeasurementAccuracyStruct
  { { AttributeTag(1), "cumulativeEnergyImported", ItemType::kDefault }, 534 }, // ElectricalEnergyMeasurement::EnergyMeasurementStruct
  { { AttributeTag(2), "cumulativeEnergyExported", ItemType::kDefault }, 534 }, // ElectricalEnergyMeasurement::EnergyMeasurementStruct
  { { AttributeTag(3), "periodicEnergyImported", ItemType::kDefault }, 534 }, // ElectricalEnergyMeasurement::EnergyMeasurementStruct
  { { AttributeTag(4), "periodicEnergyExported", ItemType::kDefault }, 534 }, // ElectricalEnergyMeasurement::EnergyMeasurementStruct
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ElectricalEnergyMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ElectricalEnergyMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ElectricalEnergyMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ElectricalEnergyMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 539 }, // ElectricalEnergyMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int16u
  { { EventTag(0), "CumulativeEnergyMeasured", ItemType::kDefault }, 535 }, // ElectricalEnergyMeasurement::CumulativeEnergyMeasured
  { { EventTag(1), "PeriodicEnergyMeasured", ItemType::kDefault }, 536 }, // ElectricalEnergyMeasurement::PeriodicEnergyMeasured
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement_MeasurementAccuracyRangeStruct[] = {
  { { ContextTag(0), "rangeMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int64s
  { { ContextTag(1), "rangeMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int64s
  { { ContextTag(2), "percentMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::percent100ths
  { { ContextTag(3), "percentMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::percent100ths
  { { ContextTag(4), "percentTypical", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::percent100ths
  { { ContextTag(5), "fixedMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int64u
  { { ContextTag(6), "fixedMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int64u
  { { ContextTag(7), "fixedTypical", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int64u
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement_MeasurementAccuracyStruct[] = {
  { { ContextTag(0), "measurementType", ItemType::kEnum }, 538 }, // ElectricalEnergyMeasurement::MeasurementTypeEnum
  { { ContextTag(1), "measured", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::boolean
  { { ContextTag(2), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int64s
  { { ContextTag(3), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int64s
  { { ContextTag(4), "accuracyRanges", ItemType::kList }, 537 }, // ElectricalEnergyMeasurement::MeasurementAccuracyRangeStruct[]
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement_EnergyMeasurementStruct[] = {
  { { ContextTag(0), "energy", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::int64s
  { { ContextTag(1), "startTimestamp", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::epoch_s
  { { ContextTag(2), "endTimestamp", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::epoch_s
  { { ContextTag(3), "startSystime", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::systime_ms
  { { ContextTag(4), "endSystime", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::systime_ms
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement_CumulativeEnergyMeasured[] = {
  { { ContextTag(0), "energyImported", ItemType::kDefault }, 534 }, // ElectricalEnergyMeasurement::EnergyMeasurementStruct
  { { ContextTag(1), "energyExported", ItemType::kDefault }, 534 }, // ElectricalEnergyMeasurement::EnergyMeasurementStruct
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement_PeriodicEnergyMeasured[] = {
  { { ContextTag(0), "energyImported", ItemType::kDefault }, 534 }, // ElectricalEnergyMeasurement::EnergyMeasurementStruct
  { { ContextTag(1), "energyExported", ItemType::kDefault }, 534 }, // ElectricalEnergyMeasurement::EnergyMeasurementStruct
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement_MeasurementAccuracyRangeStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 532 }, // ElectricalEnergyMeasurement_MeasurementAccuracyRangeStruct[]
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement_MeasurementTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kUnspecified
  { { ConstantValueTag(0x1), "kVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kVoltage
  { { ConstantValueTag(0x2), "kActiveCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kActiveCurrent
  { { ConstantValueTag(0x3), "kReactiveCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kReactiveCurrent
  { { ConstantValueTag(0x4), "kApparentCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kApparentCurrent
  { { ConstantValueTag(0x5), "kActivePower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kActivePower
  { { ConstantValueTag(0x6), "kReactivePower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kReactivePower
  { { ConstantValueTag(0x7), "kApparentPower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kApparentPower
  { { ConstantValueTag(0x8), "kRMSVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kRMSVoltage
  { { ConstantValueTag(0x9), "kRMSCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kRMSCurrent
  { { ConstantValueTag(0xA), "kRMSPower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kRMSPower
  { { ConstantValueTag(0xB), "kFrequency", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kFrequency
  { { ConstantValueTag(0xC), "kPowerFactor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kPowerFactor
  { { ConstantValueTag(0xD), "kNeutralCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kNeutralCurrent
  { { ConstantValueTag(0xE), "kElectricalEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::MeasurementTypeEnum::kElectricalEnergy
};

const Entry<ItemInfo> _ElectricalEnergyMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kImportedEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::Feature::kImportedEnergy
  { { ConstantValueTag(0x2), "kExportedEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::Feature::kExportedEnergy
  { { ConstantValueTag(0x4), "kCumulativeEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::Feature::kCumulativeEnergy
  { { ConstantValueTag(0x8), "kPeriodicEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalEnergyMeasurement::Feature::kPeriodicEnergy
};

const Entry<ItemInfo> _DemandResponseLoadControl[] = {
  { { AttributeTag(0), "loadControlPrograms", ItemType::kList }, 555 }, // DemandResponseLoadControl::LoadControlProgramStruct[]
  { { AttributeTag(1), "numberOfLoadControlPrograms", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { AttributeTag(2), "events", ItemType::kList }, 554 }, // DemandResponseLoadControl::LoadControlEventStruct[]
  { { AttributeTag(3), "activeEvents", ItemType::kList }, 554 }, // DemandResponseLoadControl::LoadControlEventStruct[]
  { { AttributeTag(4), "numberOfEventsPerProgram", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { AttributeTag(5), "numberOfTransitions", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { AttributeTag(6), "defaultRandomStart", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { AttributeTag(7), "defaultRandomDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // DemandResponseLoadControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // DemandResponseLoadControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // DemandResponseLoadControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // DemandResponseLoadControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 565 }, // DemandResponseLoadControl::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int16u
  { { EventTag(0), "LoadControlEventStatusChange", ItemType::kDefault }, 553 }, // DemandResponseLoadControl::LoadControlEventStatusChange
  { { CommandTag(0), "RegisterLoadControlProgramRequest", ItemType::kDefault }, 549 }, // DemandResponseLoadControl::RegisterLoadControlProgramRequest::RegisterLoadControlProgramRequestRequest
  { { CommandTag(1), "UnregisterLoadControlProgramRequest", ItemType::kDefault }, 550 }, // DemandResponseLoadControl::UnregisterLoadControlProgramRequest::UnregisterLoadControlProgramRequestRequest
  { { CommandTag(2), "AddLoadControlEventRequest", ItemType::kDefault }, 551 }, // DemandResponseLoadControl::AddLoadControlEventRequest::AddLoadControlEventRequestRequest
  { { CommandTag(3), "RemoveLoadControlEventRequest", ItemType::kDefault }, 552 }, // DemandResponseLoadControl::RemoveLoadControlEventRequest::RemoveLoadControlEventRequestRequest
  { { CommandTag(4), "ClearLoadControlEventsRequest", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::ClearLoadControlEventsRequest::()
};

const Entry<ItemInfo> _DemandResponseLoadControl_HeatingSourceControlStruct[] = {
  { { ContextTag(0), "heatingSource", ItemType::kEnum }, 558 }, // DemandResponseLoadControl::HeatingSourceEnum
};

const Entry<ItemInfo> _DemandResponseLoadControl_PowerSavingsControlStruct[] = {
  { { ContextTag(0), "powerSavings", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::percent
};

const Entry<ItemInfo> _DemandResponseLoadControl_DutyCycleControlStruct[] = {
  { { ContextTag(0), "dutyCycle", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::percent
};

const Entry<ItemInfo> _DemandResponseLoadControl_AverageLoadControlStruct[] = {
  { { ContextTag(0), "loadAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8s
};

const Entry<ItemInfo> _DemandResponseLoadControl_TemperatureControlStruct[] = {
  { { ContextTag(0), "coolingTempOffset", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int16u
  { { ContextTag(1), "heatingtTempOffset", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int16u
  { { ContextTag(2), "coolingTempSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::temperature
  { { ContextTag(3), "heatingTempSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::temperature
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlEventTransitionStruct[] = {
  { { ContextTag(0), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int16u
  { { ContextTag(1), "control", ItemType::kBitmap }, 564 }, // DemandResponseLoadControl::EventTransitionControlBitmap
  { { ContextTag(2), "temperatureControl", ItemType::kDefault }, 545 }, // DemandResponseLoadControl::TemperatureControlStruct
  { { ContextTag(3), "averageLoadControl", ItemType::kDefault }, 544 }, // DemandResponseLoadControl::AverageLoadControlStruct
  { { ContextTag(4), "dutyCycleControl", ItemType::kDefault }, 543 }, // DemandResponseLoadControl::DutyCycleControlStruct
  { { ContextTag(5), "powerSavingsControl", ItemType::kDefault }, 542 }, // DemandResponseLoadControl::PowerSavingsControlStruct
  { { ContextTag(6), "heatingSourceControl", ItemType::kDefault }, 541 }, // DemandResponseLoadControl::HeatingSourceControlStruct
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlEventStruct[] = {
  { { ContextTag(0), "eventID", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::octet_string
  { { ContextTag(1), "programID", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::octet_string
  { { ContextTag(2), "control", ItemType::kBitmap }, 563 }, // DemandResponseLoadControl::EventControlBitmap
  { { ContextTag(3), "deviceClass", ItemType::kBitmap }, 562 }, // DemandResponseLoadControl::DeviceClassBitmap
  { { ContextTag(4), "enrollmentGroup", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { ContextTag(5), "criticality", ItemType::kEnum }, 557 }, // DemandResponseLoadControl::CriticalityLevelEnum
  { { ContextTag(6), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::epoch_s
  { { ContextTag(7), "transitions", ItemType::kList }, 556 }, // DemandResponseLoadControl::LoadControlEventTransitionStruct[]
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlProgramStruct[] = {
  { { ContextTag(0), "programID", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::octet_string
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::long_char_string
  { { ContextTag(2), "enrollmentGroup", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { ContextTag(3), "randomStartMinutes", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { ContextTag(4), "randomDurationMinutes", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
};

const Entry<ItemInfo> _DemandResponseLoadControl_RegisterLoadControlProgramRequestRequest[] = {
  { { ContextTag(0), "loadControlProgram", ItemType::kDefault }, 548 }, // DemandResponseLoadControl::LoadControlProgramStruct
};

const Entry<ItemInfo> _DemandResponseLoadControl_UnregisterLoadControlProgramRequestRequest[] = {
  { { ContextTag(0), "loadControlProgramID", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::octet_string
};

const Entry<ItemInfo> _DemandResponseLoadControl_AddLoadControlEventRequestRequest[] = {
  { { ContextTag(0), "event", ItemType::kDefault }, 547 }, // DemandResponseLoadControl::LoadControlEventStruct
};

const Entry<ItemInfo> _DemandResponseLoadControl_RemoveLoadControlEventRequestRequest[] = {
  { { ContextTag(0), "eventID", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::octet_string
  { { ContextTag(1), "cancelControl", ItemType::kBitmap }, 561 }, // DemandResponseLoadControl::CancelControlBitmap
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlEventStatusChange[] = {
  { { ContextTag(0), "eventID", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::octet_string
  { { ContextTag(1), "transitionIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::int8u
  { { ContextTag(2), "status", ItemType::kEnum }, 560 }, // DemandResponseLoadControl::LoadControlEventStatusEnum
  { { ContextTag(3), "criticality", ItemType::kEnum }, 557 }, // DemandResponseLoadControl::CriticalityLevelEnum
  { { ContextTag(4), "control", ItemType::kBitmap }, 563 }, // DemandResponseLoadControl::EventControlBitmap
  { { ContextTag(5), "temperatureControl", ItemType::kDefault }, 545 }, // DemandResponseLoadControl::TemperatureControlStruct
  { { ContextTag(6), "averageLoadControl", ItemType::kDefault }, 544 }, // DemandResponseLoadControl::AverageLoadControlStruct
  { { ContextTag(7), "dutyCycleControl", ItemType::kDefault }, 543 }, // DemandResponseLoadControl::DutyCycleControlStruct
  { { ContextTag(8), "powerSavingsControl", ItemType::kDefault }, 542 }, // DemandResponseLoadControl::PowerSavingsControlStruct
  { { ContextTag(9), "heatingSourceControl", ItemType::kDefault }, 541 }, // DemandResponseLoadControl::HeatingSourceControlStruct
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlEventStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 547 }, // DemandResponseLoadControl_LoadControlEventStruct[]
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlProgramStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 548 }, // DemandResponseLoadControl_LoadControlProgramStruct[]
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlEventTransitionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 546 }, // DemandResponseLoadControl_LoadControlEventTransitionStruct[]
};

const Entry<ItemInfo> _DemandResponseLoadControl_CriticalityLevelEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kUnknown
  { { ConstantValueTag(0x1), "kGreen", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kGreen
  { { ConstantValueTag(0x2), "kLevel1", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kLevel1
  { { ConstantValueTag(0x3), "kLevel2", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kLevel2
  { { ConstantValueTag(0x4), "kLevel3", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kLevel3
  { { ConstantValueTag(0x5), "kLevel4", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kLevel4
  { { ConstantValueTag(0x6), "kLevel5", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kLevel5
  { { ConstantValueTag(0x7), "kEmergency", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kEmergency
  { { ConstantValueTag(0x8), "kPlannedOutage", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kPlannedOutage
  { { ConstantValueTag(0x9), "kServiceDisconnect", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CriticalityLevelEnum::kServiceDisconnect
};

const Entry<ItemInfo> _DemandResponseLoadControl_HeatingSourceEnum[] = {
  { { ConstantValueTag(0x0), "kAny", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::HeatingSourceEnum::kAny
  { { ConstantValueTag(0x1), "kElectric", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::HeatingSourceEnum::kElectric
  { { ConstantValueTag(0x2), "kNonElectric", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::HeatingSourceEnum::kNonElectric
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlEventChangeSourceEnum[] = {
  { { ConstantValueTag(0x0), "kAutomatic", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventChangeSourceEnum::kAutomatic
  { { ConstantValueTag(0x1), "kUserAction", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventChangeSourceEnum::kUserAction
};

const Entry<ItemInfo> _DemandResponseLoadControl_LoadControlEventStatusEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kUnknown
  { { ConstantValueTag(0x1), "kReceived", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kReceived
  { { ConstantValueTag(0x2), "kInProgress", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kInProgress
  { { ConstantValueTag(0x3), "kCompleted", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kCompleted
  { { ConstantValueTag(0x4), "kOptedOut", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kOptedOut
  { { ConstantValueTag(0x5), "kOptedIn", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kOptedIn
  { { ConstantValueTag(0x6), "kCanceled", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kCanceled
  { { ConstantValueTag(0x7), "kSuperseded", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kSuperseded
  { { ConstantValueTag(0x8), "kPartialOptedOut", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kPartialOptedOut
  { { ConstantValueTag(0x9), "kPartialOptedIn", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kPartialOptedIn
  { { ConstantValueTag(0xA), "kNoParticipation", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kNoParticipation
  { { ConstantValueTag(0xB), "kUnavailable", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kUnavailable
  { { ConstantValueTag(0xC), "kFailed", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::LoadControlEventStatusEnum::kFailed
};

const Entry<ItemInfo> _DemandResponseLoadControl_CancelControlBitmap[] = {
  { { ConstantValueTag(0x1), "kRandomEnd", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::CancelControlBitmap::kRandomEnd
};

const Entry<ItemInfo> _DemandResponseLoadControl_DeviceClassBitmap[] = {
  { { ConstantValueTag(0x1), "kHVAC", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kHVAC
  { { ConstantValueTag(0x2), "kStripHeater", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kStripHeater
  { { ConstantValueTag(0x4), "kWaterHeater", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kWaterHeater
  { { ConstantValueTag(0x8), "kPoolPump", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kPoolPump
  { { ConstantValueTag(0x10), "kSmartAppliance", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kSmartAppliance
  { { ConstantValueTag(0x20), "kIrrigationPump", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kIrrigationPump
  { { ConstantValueTag(0x40), "kCommercialLoad", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kCommercialLoad
  { { ConstantValueTag(0x80), "kResidentialLoad", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kResidentialLoad
  { { ConstantValueTag(0x100), "kExteriorLighting", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kExteriorLighting
  { { ConstantValueTag(0x200), "kInteriorLighting", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kInteriorLighting
  { { ConstantValueTag(0x400), "kEV", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kEV
  { { ConstantValueTag(0x800), "kGenerationSystem", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kGenerationSystem
  { { ConstantValueTag(0x1000), "kSmartInverter", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kSmartInverter
  { { ConstantValueTag(0x2000), "kEVSE", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kEVSE
  { { ConstantValueTag(0x4000), "kRESU", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kRESU
  { { ConstantValueTag(0x8000), "kEMS", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kEMS
  { { ConstantValueTag(0x10000), "kSEM", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::DeviceClassBitmap::kSEM
};

const Entry<ItemInfo> _DemandResponseLoadControl_EventControlBitmap[] = {
  { { ConstantValueTag(0x1), "kRandomStart", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::EventControlBitmap::kRandomStart
};

const Entry<ItemInfo> _DemandResponseLoadControl_EventTransitionControlBitmap[] = {
  { { ConstantValueTag(0x1), "kRandomDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::EventTransitionControlBitmap::kRandomDuration
  { { ConstantValueTag(0x2), "kIgnoreOptOut", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::EventTransitionControlBitmap::kIgnoreOptOut
};

const Entry<ItemInfo> _DemandResponseLoadControl_Feature[] = {
  { { ConstantValueTag(0x1), "kEnrollmentGroups", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::Feature::kEnrollmentGroups
  { { ConstantValueTag(0x2), "kTemperatureOffset", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::Feature::kTemperatureOffset
  { { ConstantValueTag(0x4), "kTemperatureSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::Feature::kTemperatureSetpoint
  { { ConstantValueTag(0x8), "kLoadAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::Feature::kLoadAdjustment
  { { ConstantValueTag(0x10), "kDutyCycle", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::Feature::kDutyCycle
  { { ConstantValueTag(0x20), "kPowerSavings", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::Feature::kPowerSavings
  { { ConstantValueTag(0x40), "kHeatingSource", ItemType::kDefault }, kInvalidNodeIndex }, // DemandResponseLoadControl::Feature::kHeatingSource
};

const Entry<ItemInfo> _DeviceEnergyManagement[] = {
  { { AttributeTag(0), "ESAType", ItemType::kEnum }, 587 }, // DeviceEnergyManagement::ESATypeEnum
  { { AttributeTag(1), "ESACanGenerate", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::boolean
  { { AttributeTag(2), "ESAState", ItemType::kEnum }, 586 }, // DeviceEnergyManagement::ESAStateEnum
  { { AttributeTag(3), "absMinPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { AttributeTag(4), "absMaxPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { AttributeTag(5), "powerAdjustmentCapability", ItemType::kList }, 581 }, // DeviceEnergyManagement::PowerAdjustStruct[]
  { { AttributeTag(6), "forecast", ItemType::kDefault }, 569 }, // DeviceEnergyManagement::ForecastStruct
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // DeviceEnergyManagement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // DeviceEnergyManagement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // DeviceEnergyManagement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // DeviceEnergyManagement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 588 }, // DeviceEnergyManagement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int16u
  { { EventTag(1), "PowerAdjustEnd", ItemType::kDefault }, 578 }, // DeviceEnergyManagement::PowerAdjustEnd
  { { CommandTag(0), "PowerAdjustRequest", ItemType::kDefault }, 573 }, // DeviceEnergyManagement::PowerAdjustRequest::PowerAdjustRequestRequest
  { { CommandTag(1), "CancelPowerAdjustRequest", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CancelPowerAdjustRequest::()
  { { CommandTag(2), "StartTimeAdjustRequest", ItemType::kDefault }, 574 }, // DeviceEnergyManagement::StartTimeAdjustRequest::StartTimeAdjustRequestRequest
  { { CommandTag(3), "PauseRequest", ItemType::kDefault }, 575 }, // DeviceEnergyManagement::PauseRequest::PauseRequestRequest
  { { CommandTag(4), "ResumeRequest", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ResumeRequest::()
  { { CommandTag(5), "ModifyForecastRequest", ItemType::kDefault }, 576 }, // DeviceEnergyManagement::ModifyForecastRequest::ModifyForecastRequestRequest
  { { CommandTag(6), "RequestConstraintBasedForecast", ItemType::kDefault }, 577 }, // DeviceEnergyManagement::RequestConstraintBasedForecast::RequestConstraintBasedForecastRequest
};

const Entry<ItemInfo> _DeviceEnergyManagement_CostStruct[] = {
  { { ContextTag(0), "costType", ItemType::kEnum }, 585 }, // DeviceEnergyManagement::CostTypeEnum
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int32s
  { { ContextTag(2), "decimalPoints", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int8u
  { { ContextTag(3), "currency", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int16u
};

const Entry<ItemInfo> _DeviceEnergyManagement_SlotStruct[] = {
  { { ContextTag(0), "minDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(1), "maxDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(2), "defaultDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(3), "elapsedSlotTime", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(4), "remainingSlotTime", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(5), "slotIsPauseable", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::boolean
  { { ContextTag(6), "minPauseDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(7), "maxPauseDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(8), "manufacturerESAState", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int16u
  { { ContextTag(9), "nominalPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(10), "minPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(11), "maxPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(12), "nominalEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::energy_mwh
  { { ContextTag(13), "costs", ItemType::kList }, 580 }, // DeviceEnergyManagement::CostStruct[]
  { { ContextTag(14), "minPowerAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(15), "maxPowerAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(16), "minDurationAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(17), "maxDurationAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
};

const Entry<ItemInfo> _DeviceEnergyManagement_ForecastStruct[] = {
  { { ContextTag(0), "forecastId", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int16u
  { { ContextTag(1), "activeSlotNumber", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int16u
  { { ContextTag(2), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::epoch_s
  { { ContextTag(3), "endTime", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::epoch_s
  { { ContextTag(4), "earliestStartTime", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::epoch_s
  { { ContextTag(5), "latestEndTime", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::epoch_s
  { { ContextTag(6), "isPauseable", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::boolean
  { { ContextTag(7), "slots", ItemType::kList }, 583 }, // DeviceEnergyManagement::SlotStruct[]
};

const Entry<ItemInfo> _DeviceEnergyManagement_ConstraintsStruct[] = {
  { { ContextTag(0), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::epoch_s
  { { ContextTag(1), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(2), "nominalPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(3), "maximumEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::energy_mwh
  { { ContextTag(4), "loadControl", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int8s
};

const Entry<ItemInfo> _DeviceEnergyManagement_PowerAdjustStruct[] = {
  { { ContextTag(0), "minPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(1), "maxPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(2), "minDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(3), "maxDuration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
};

const Entry<ItemInfo> _DeviceEnergyManagement_SlotAdjustmentStruct[] = {
  { { ContextTag(0), "slotIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int8u
  { { ContextTag(1), "nominalPower", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(2), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
};

const Entry<ItemInfo> _DeviceEnergyManagement_PowerAdjustRequestRequest[] = {
  { { ContextTag(0), "power", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::power_mw
  { { ContextTag(1), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
};

const Entry<ItemInfo> _DeviceEnergyManagement_StartTimeAdjustRequestRequest[] = {
  { { ContextTag(0), "requestedStartTime", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::epoch_s
};

const Entry<ItemInfo> _DeviceEnergyManagement_PauseRequestRequest[] = {
  { { ContextTag(0), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
};

const Entry<ItemInfo> _DeviceEnergyManagement_ModifyForecastRequestRequest[] = {
  { { ContextTag(0), "forecastId", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::int32u
  { { ContextTag(1), "slotAdjustments", ItemType::kList }, 582 }, // DeviceEnergyManagement::SlotAdjustmentStruct[]
};

const Entry<ItemInfo> _DeviceEnergyManagement_RequestConstraintBasedForecastRequest[] = {
  { { ContextTag(0), "constraints", ItemType::kList }, 579 }, // DeviceEnergyManagement::ConstraintsStruct[]
};

const Entry<ItemInfo> _DeviceEnergyManagement_PowerAdjustEnd[] = {
  { { ContextTag(0), "cause", ItemType::kEnum }, 584 }, // DeviceEnergyManagement::CauseEnum
  { { ContextTag(1), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::elapsed_s
  { { ContextTag(2), "energyUse", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::energy_mwh
};

const Entry<ItemInfo> _DeviceEnergyManagement_ConstraintsStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 570 }, // DeviceEnergyManagement_ConstraintsStruct[]
};

const Entry<ItemInfo> _DeviceEnergyManagement_CostStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 567 }, // DeviceEnergyManagement_CostStruct[]
};

const Entry<ItemInfo> _DeviceEnergyManagement_PowerAdjustStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 571 }, // DeviceEnergyManagement_PowerAdjustStruct[]
};

const Entry<ItemInfo> _DeviceEnergyManagement_SlotAdjustmentStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 572 }, // DeviceEnergyManagement_SlotAdjustmentStruct[]
};

const Entry<ItemInfo> _DeviceEnergyManagement_SlotStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 568 }, // DeviceEnergyManagement_SlotStruct[]
};

const Entry<ItemInfo> _DeviceEnergyManagement_CauseEnum[] = {
  { { ConstantValueTag(0x0), "kNormalCompletion", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CauseEnum::kNormalCompletion
  { { ConstantValueTag(0x1), "kOffline", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CauseEnum::kOffline
  { { ConstantValueTag(0x2), "kFault", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CauseEnum::kFault
  { { ConstantValueTag(0x3), "kUserOptOut", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CauseEnum::kUserOptOut
};

const Entry<ItemInfo> _DeviceEnergyManagement_CostTypeEnum[] = {
  { { ConstantValueTag(0x0), "kFinancial", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CostTypeEnum::kFinancial
  { { ConstantValueTag(0x1), "kGHGEmissions", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CostTypeEnum::kGHGEmissions
  { { ConstantValueTag(0x2), "kComfort", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CostTypeEnum::kComfort
  { { ConstantValueTag(0x3), "kTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::CostTypeEnum::kTemperature
};

const Entry<ItemInfo> _DeviceEnergyManagement_ESAStateEnum[] = {
  { { ConstantValueTag(0x0), "kOffline", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESAStateEnum::kOffline
  { { ConstantValueTag(0x1), "kOnline", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESAStateEnum::kOnline
  { { ConstantValueTag(0x2), "kFault", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESAStateEnum::kFault
  { { ConstantValueTag(0x3), "kUserOptOut", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESAStateEnum::kUserOptOut
  { { ConstantValueTag(0x4), "kPowerAdjustActive", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESAStateEnum::kPowerAdjustActive
  { { ConstantValueTag(0x5), "kPaused", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESAStateEnum::kPaused
};

const Entry<ItemInfo> _DeviceEnergyManagement_ESATypeEnum[] = {
  { { ConstantValueTag(0x0), "kEVSE", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kEVSE
  { { ConstantValueTag(0x1), "kSpaceHeating", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kSpaceHeating
  { { ConstantValueTag(0x2), "kWaterHeating", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kWaterHeating
  { { ConstantValueTag(0x3), "kSpaceCooling", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kSpaceCooling
  { { ConstantValueTag(0x4), "kSpaceHeatingCooling", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kSpaceHeatingCooling
  { { ConstantValueTag(0x5), "kBatteryStorage", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kBatteryStorage
  { { ConstantValueTag(0x6), "kSolarPV", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kSolarPV
  { { ConstantValueTag(0x7), "kFridgeFreezer", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kFridgeFreezer
  { { ConstantValueTag(0x8), "kWashingMachine", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kWashingMachine
  { { ConstantValueTag(0x9), "kDishwasher", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kDishwasher
  { { ConstantValueTag(0xA), "kCooking", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kCooking
  { { ConstantValueTag(0xB), "kHomeWaterPump", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kHomeWaterPump
  { { ConstantValueTag(0xC), "kIrrigationWaterPump", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kIrrigationWaterPump
  { { ConstantValueTag(0xD), "kPoolPump", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kPoolPump
  { { ConstantValueTag(0xFF), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::ESATypeEnum::kOther
};

const Entry<ItemInfo> _DeviceEnergyManagement_Feature[] = {
  { { ConstantValueTag(0x1), "kPowerAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::Feature::kPowerAdjustment
  { { ConstantValueTag(0x2), "kPowerForecastReporting", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::Feature::kPowerForecastReporting
  { { ConstantValueTag(0x4), "kStateForecastReporting", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::Feature::kStateForecastReporting
  { { ConstantValueTag(0x8), "kForecastAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // DeviceEnergyManagement::Feature::kForecastAdjustment
};

const Entry<ItemInfo> _EnergyEvse[] = {
  { { AttributeTag(0), "state", ItemType::kEnum }, 605 }, // EnergyEvse::StateEnum
  { { AttributeTag(1), "supplyState", ItemType::kEnum }, 606 }, // EnergyEvse::SupplyStateEnum
  { { AttributeTag(2), "faultState", ItemType::kEnum }, 604 }, // EnergyEvse::FaultStateEnum
  { { AttributeTag(3), "chargingEnabledUntil", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::epoch_s
  { { AttributeTag(4), "dischargingEnabledUntil", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::epoch_s
  { { AttributeTag(5), "circuitCapacity", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
  { { AttributeTag(6), "minimumChargeCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
  { { AttributeTag(7), "maximumChargeCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
  { { AttributeTag(8), "maximumDischargeCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
  { { AttributeTag(9), "userMaximumChargeCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
  { { AttributeTag(10), "randomizationDelayWindow", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::elapsed_s
  { { AttributeTag(33), "numberOfWeeklyTargets", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int8u
  { { AttributeTag(34), "numberOfDailyTargets", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int8u
  { { AttributeTag(35), "nextChargeStartTime", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::epoch_s
  { { AttributeTag(36), "nextChargeTargetTime", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::epoch_s
  { { AttributeTag(37), "nextChargeRequiredEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::energy_mwh
  { { AttributeTag(38), "nextChargeTargetSoC", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::percent
  { { AttributeTag(39), "approximateEVEfficiency", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int16u
  { { AttributeTag(48), "stateOfCharge", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::percent
  { { AttributeTag(49), "batteryCapacity", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::energy_mwh
  { { AttributeTag(50), "vehicleID", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::char_string
  { { AttributeTag(64), "sessionID", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int32u
  { { AttributeTag(65), "sessionDuration", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::elapsed_s
  { { AttributeTag(66), "sessionEnergyCharged", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::energy_mwh
  { { AttributeTag(67), "sessionEnergyDischarged", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::energy_mwh
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // EnergyEvse::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // EnergyEvse::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // EnergyEvse::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // EnergyEvse::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 607 }, // EnergyEvse::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int16u
  { { EventTag(0), "EVConnected", ItemType::kDefault }, 596 }, // EnergyEvse::EVConnected
  { { EventTag(1), "EVNotDetected", ItemType::kDefault }, 597 }, // EnergyEvse::EVNotDetected
  { { EventTag(2), "EnergyTransferStarted", ItemType::kDefault }, 598 }, // EnergyEvse::EnergyTransferStarted
  { { EventTag(3), "EnergyTransferStopped", ItemType::kDefault }, 599 }, // EnergyEvse::EnergyTransferStopped
  { { EventTag(4), "Fault", ItemType::kDefault }, 600 }, // EnergyEvse::Fault
  { { EventTag(5), "RFID", ItemType::kDefault }, 601 }, // EnergyEvse::RFID
  { { CommandTag(1), "Disable", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::Disable::()
  { { CommandTag(2), "EnableCharging", ItemType::kDefault }, 592 }, // EnergyEvse::EnableCharging::EnableChargingRequest
  { { CommandTag(3), "EnableDischarging", ItemType::kDefault }, 593 }, // EnergyEvse::EnableDischarging::EnableDischargingRequest
  { { CommandTag(4), "StartDiagnostics", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::StartDiagnostics::()
  { { CommandTag(5), "SetTargets", ItemType::kDefault }, 594 }, // EnergyEvse::SetTargets::SetTargetsRequest
  { { CommandTag(6), "GetTargets", ItemType::kDefault }, 595 }, // EnergyEvse::GetTargets::GetTargetsRequest
  { { CommandTag(7), "ClearTargets", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::ClearTargets::()
  { { CommandTag(0), "GetTargetsResponse", ItemType::kDefault }, 591 }, // EnergyEvse::GetTargetsResponse
};

const Entry<ItemInfo> _EnergyEvse_ChargingTargetStruct[] = {
  { { ContextTag(0), "targetTimeMinutesPastMidnight", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int16u
  { { ContextTag(1), "targetSoC", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::percent
  { { ContextTag(2), "addedEnergy", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::energy_mwh
};

const Entry<ItemInfo> _EnergyEvse_GetTargetsResponse[] = {
  { { ContextTag(0), "dayOfWeekforSequence", ItemType::kBitmap }, 608 }, // EnergyEvse::TargetDayOfWeekBitmap
  { { ContextTag(1), "chargingTargets", ItemType::kList }, 602 }, // EnergyEvse::ChargingTargetStruct[]
};

const Entry<ItemInfo> _EnergyEvse_EnableChargingRequest[] = {
  { { ContextTag(0), "chargingEnabledUntil", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::epoch_s
  { { ContextTag(1), "minimumChargeCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
  { { ContextTag(2), "maximumChargeCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
};

const Entry<ItemInfo> _EnergyEvse_EnableDischargingRequest[] = {
  { { ContextTag(0), "dischargingEnabledUntil", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::epoch_s
  { { ContextTag(1), "maximumDischargeCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
};

const Entry<ItemInfo> _EnergyEvse_SetTargetsRequest[] = {
  { { ContextTag(0), "dayOfWeekforSequence", ItemType::kBitmap }, 608 }, // EnergyEvse::TargetDayOfWeekBitmap
  { { ContextTag(1), "chargingTargets", ItemType::kList }, 602 }, // EnergyEvse::ChargingTargetStruct[]
};

const Entry<ItemInfo> _EnergyEvse_GetTargetsRequest[] = {
  { { ContextTag(0), "daysToReturn", ItemType::kBitmap }, 608 }, // EnergyEvse::TargetDayOfWeekBitmap
};

const Entry<ItemInfo> _EnergyEvse_EVConnected[] = {
  { { ContextTag(0), "sessionID", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int32u
};

const Entry<ItemInfo> _EnergyEvse_EVNotDetected[] = {
  { { ContextTag(0), "sessionID", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int32u
  { { ContextTag(1), "state", ItemType::kEnum }, 605 }, // EnergyEvse::StateEnum
  { { ContextTag(2), "sessionDuration", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::elapsed_s
  { { ContextTag(3), "sessionEnergyCharged", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::energy_mwh
  { { ContextTag(4), "sessionEnergyDischarged", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::energy_mwh
};

const Entry<ItemInfo> _EnergyEvse_EnergyTransferStarted[] = {
  { { ContextTag(0), "sessionID", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int32u
  { { ContextTag(1), "state", ItemType::kEnum }, 605 }, // EnergyEvse::StateEnum
  { { ContextTag(2), "maximumCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::amperage_ma
};

const Entry<ItemInfo> _EnergyEvse_EnergyTransferStopped[] = {
  { { ContextTag(0), "sessionID", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int32u
  { { ContextTag(1), "state", ItemType::kEnum }, 605 }, // EnergyEvse::StateEnum
  { { ContextTag(2), "reason", ItemType::kEnum }, 603 }, // EnergyEvse::EnergyTransferStoppedReasonEnum
  { { ContextTag(4), "energyTransferred", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::energy_mwh
};

const Entry<ItemInfo> _EnergyEvse_Fault[] = {
  { { ContextTag(0), "sessionID", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::int32u
  { { ContextTag(1), "state", ItemType::kEnum }, 605 }, // EnergyEvse::StateEnum
  { { ContextTag(2), "faultStatePreviousState", ItemType::kEnum }, 604 }, // EnergyEvse::FaultStateEnum
  { { ContextTag(4), "faultStateCurrentState", ItemType::kEnum }, 604 }, // EnergyEvse::FaultStateEnum
};

const Entry<ItemInfo> _EnergyEvse_RFID[] = {
  { { ContextTag(0), "uid", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::octet_string
};

const Entry<ItemInfo> _EnergyEvse_ChargingTargetStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 590 }, // EnergyEvse_ChargingTargetStruct[]
};

const Entry<ItemInfo> _EnergyEvse_EnergyTransferStoppedReasonEnum[] = {
  { { ConstantValueTag(0x0), "kEVStopped", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::EnergyTransferStoppedReasonEnum::kEVStopped
  { { ConstantValueTag(0x1), "kEVSEStopped", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::EnergyTransferStoppedReasonEnum::kEVSEStopped
  { { ConstantValueTag(0x2), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::EnergyTransferStoppedReasonEnum::kOther
};

const Entry<ItemInfo> _EnergyEvse_FaultStateEnum[] = {
  { { ConstantValueTag(0x0), "kNoError", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kNoError
  { { ConstantValueTag(0x1), "kMeterFailure", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kMeterFailure
  { { ConstantValueTag(0x2), "kOverVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kOverVoltage
  { { ConstantValueTag(0x3), "kUnderVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kUnderVoltage
  { { ConstantValueTag(0x4), "kOverCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kOverCurrent
  { { ConstantValueTag(0x5), "kContactWetFailure", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kContactWetFailure
  { { ConstantValueTag(0x6), "kContactDryFailure", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kContactDryFailure
  { { ConstantValueTag(0x7), "kGroundFault", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kGroundFault
  { { ConstantValueTag(0x8), "kPowerLoss", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kPowerLoss
  { { ConstantValueTag(0x9), "kPowerQuality", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kPowerQuality
  { { ConstantValueTag(0xA), "kPilotShortCircuit", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kPilotShortCircuit
  { { ConstantValueTag(0xB), "kEmergencyStop", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kEmergencyStop
  { { ConstantValueTag(0xC), "kEVDisconnected", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kEVDisconnected
  { { ConstantValueTag(0xD), "kWrongPowerSupply", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kWrongPowerSupply
  { { ConstantValueTag(0xE), "kLiveNeutralSwap", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kLiveNeutralSwap
  { { ConstantValueTag(0xF), "kOverTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kOverTemperature
  { { ConstantValueTag(0xFF), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::FaultStateEnum::kOther
};

const Entry<ItemInfo> _EnergyEvse_StateEnum[] = {
  { { ConstantValueTag(0x0), "kNotPluggedIn", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::StateEnum::kNotPluggedIn
  { { ConstantValueTag(0x1), "kPluggedInNoDemand", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::StateEnum::kPluggedInNoDemand
  { { ConstantValueTag(0x2), "kPluggedInDemand", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::StateEnum::kPluggedInDemand
  { { ConstantValueTag(0x3), "kPluggedInCharging", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::StateEnum::kPluggedInCharging
  { { ConstantValueTag(0x4), "kPluggedInDischarging", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::StateEnum::kPluggedInDischarging
  { { ConstantValueTag(0x5), "kSessionEnding", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::StateEnum::kSessionEnding
  { { ConstantValueTag(0x6), "kFault", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::StateEnum::kFault
};

const Entry<ItemInfo> _EnergyEvse_SupplyStateEnum[] = {
  { { ConstantValueTag(0x0), "kDisabled", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::SupplyStateEnum::kDisabled
  { { ConstantValueTag(0x1), "kChargingEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::SupplyStateEnum::kChargingEnabled
  { { ConstantValueTag(0x2), "kDischargingEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::SupplyStateEnum::kDischargingEnabled
  { { ConstantValueTag(0x3), "kDisabledError", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::SupplyStateEnum::kDisabledError
  { { ConstantValueTag(0x4), "kDisabledDiagnostics", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::SupplyStateEnum::kDisabledDiagnostics
};

const Entry<ItemInfo> _EnergyEvse_Feature[] = {
  { { ConstantValueTag(0x1), "kChargingPreferences", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::Feature::kChargingPreferences
  { { ConstantValueTag(0x2), "kSoCReporting", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::Feature::kSoCReporting
  { { ConstantValueTag(0x4), "kPlugAndCharge", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::Feature::kPlugAndCharge
  { { ConstantValueTag(0x8), "kRFID", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::Feature::kRFID
  { { ConstantValueTag(0x10), "kV2X", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::Feature::kV2X
};

const Entry<ItemInfo> _EnergyEvse_TargetDayOfWeekBitmap[] = {
  { { ConstantValueTag(0x1), "kSunday", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::TargetDayOfWeekBitmap::kSunday
  { { ConstantValueTag(0x2), "kMonday", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::TargetDayOfWeekBitmap::kMonday
  { { ConstantValueTag(0x4), "kTuesday", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::TargetDayOfWeekBitmap::kTuesday
  { { ConstantValueTag(0x8), "kWednesday", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::TargetDayOfWeekBitmap::kWednesday
  { { ConstantValueTag(0x10), "kThursday", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::TargetDayOfWeekBitmap::kThursday
  { { ConstantValueTag(0x20), "kFriday", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::TargetDayOfWeekBitmap::kFriday
  { { ConstantValueTag(0x40), "kSaturday", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyEvse::TargetDayOfWeekBitmap::kSaturday
};

const Entry<ItemInfo> _EnergyPreference[] = {
  { { AttributeTag(0), "energyBalances", ItemType::kList }, 611 }, // EnergyPreference::BalanceStruct[]
  { { AttributeTag(1), "currentEnergyBalance", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::int8u
  { { AttributeTag(2), "energyPriorities", ItemType::kList }, 612 }, // EnergyPreference::EnergyPriorityEnum[]
  { { AttributeTag(3), "lowPowerModeSensitivities", ItemType::kList }, 611 }, // EnergyPreference::BalanceStruct[]
  { { AttributeTag(4), "currentLowPowerModeSensitivity", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // EnergyPreference::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // EnergyPreference::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // EnergyPreference::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // EnergyPreference::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 614 }, // EnergyPreference::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::int16u
};

const Entry<ItemInfo> _EnergyPreference_BalanceStruct[] = {
  { { ContextTag(0), "step", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::percent
  { { ContextTag(1), "label", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::char_string
};

const Entry<ItemInfo> _EnergyPreference_BalanceStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 610 }, // EnergyPreference_BalanceStruct[]
};

const Entry<ItemInfo> _EnergyPreference_EnergyPriorityEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 613 }, // EnergyPreference_EnergyPriorityEnum[]
};

const Entry<ItemInfo> _EnergyPreference_EnergyPriorityEnum[] = {
  { { ConstantValueTag(0x0), "kComfort", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::EnergyPriorityEnum::kComfort
  { { ConstantValueTag(0x1), "kSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::EnergyPriorityEnum::kSpeed
  { { ConstantValueTag(0x2), "kEfficiency", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::EnergyPriorityEnum::kEfficiency
  { { ConstantValueTag(0x3), "kWaterConsumption", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::EnergyPriorityEnum::kWaterConsumption
};

const Entry<ItemInfo> _EnergyPreference_Feature[] = {
  { { ConstantValueTag(0x1), "kEnergyBalance", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::Feature::kEnergyBalance
  { { ConstantValueTag(0x2), "kLowPowerModeSensitivity", ItemType::kDefault }, kInvalidNodeIndex }, // EnergyPreference::Feature::kLowPowerModeSensitivity
};

const Entry<ItemInfo> _DoorLock[] = {
  { { AttributeTag(0), "lockState", ItemType::kEnum }, 653 }, // DoorLock::DlLockState
  { { AttributeTag(1), "lockType", ItemType::kEnum }, 654 }, // DoorLock::DlLockType
  { { AttributeTag(2), "actuatorEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::boolean
  { { AttributeTag(3), "doorState", ItemType::kEnum }, 661 }, // DoorLock::DoorStateEnum
  { { AttributeTag(4), "doorOpenEvents", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int32u
  { { AttributeTag(5), "doorClosedEvents", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int32u
  { { AttributeTag(6), "openPeriod", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { AttributeTag(17), "numberOfTotalUsersSupported", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { AttributeTag(18), "numberOfPINUsersSupported", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { AttributeTag(19), "numberOfRFIDUsersSupported", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { AttributeTag(20), "numberOfWeekDaySchedulesSupportedPerUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(21), "numberOfYearDaySchedulesSupportedPerUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(22), "numberOfHolidaySchedulesSupported", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(23), "maxPINCodeLength", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(24), "minPINCodeLength", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(25), "maxRFIDCodeLength", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(26), "minRFIDCodeLength", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(27), "credentialRulesSupport", ItemType::kBitmap }, 670 }, // DoorLock::DlCredentialRuleMask
  { { AttributeTag(28), "numberOfCredentialsSupportedPerUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(33), "language", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::char_string
  { { AttributeTag(34), "LEDSettings", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(35), "autoRelockTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int32u
  { { AttributeTag(36), "soundVolume", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(37), "operatingMode", ItemType::kEnum }, 664 }, // DoorLock::OperatingModeEnum
  { { AttributeTag(38), "supportedOperatingModes", ItemType::kBitmap }, 681 }, // DoorLock::DlSupportedOperatingModes
  { { AttributeTag(39), "defaultConfigurationRegister", ItemType::kBitmap }, 672 }, // DoorLock::DlDefaultConfigurationRegister
  { { AttributeTag(40), "enableLocalProgramming", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::boolean
  { { AttributeTag(41), "enableOneTouchLocking", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::boolean
  { { AttributeTag(42), "enableInsideStatusLED", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::boolean
  { { AttributeTag(43), "enablePrivacyModeButton", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::boolean
  { { AttributeTag(44), "localProgrammingFeatures", ItemType::kBitmap }, 675 }, // DoorLock::DlLocalProgrammingFeatures
  { { AttributeTag(48), "wrongCodeEntryLimit", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(49), "userCodeTemporaryDisableTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(50), "sendPINOverTheAir", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::boolean
  { { AttributeTag(51), "requirePINforRemoteOperation", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::boolean
  { { AttributeTag(53), "expiringUserTimeout", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { AttributeTag(128), "aliroReaderVerificationKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
  { { AttributeTag(129), "aliroReaderGroupIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
  { { AttributeTag(130), "aliroReaderGroupSubIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
  { { AttributeTag(131), "aliroExpeditedTransactionSupportedProtocolVersions", ItemType::kList }, 1 }, // DoorLock::octet_string[]
  { { AttributeTag(132), "aliroGroupResolvingKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
  { { AttributeTag(133), "aliroSupportedBLEUWBProtocolVersions", ItemType::kList }, 1 }, // DoorLock::octet_string[]
  { { AttributeTag(134), "aliroBLEAdvertisingVersion", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { AttributeTag(135), "numberOfAliroCredentialIssuerKeysSupported", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { AttributeTag(136), "numberOfAliroEndpointKeysSupported", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // DoorLock::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // DoorLock::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // DoorLock::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // DoorLock::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 683 }, // DoorLock::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { EventTag(0), "DoorLockAlarm", ItemType::kDefault }, 643 }, // DoorLock::DoorLockAlarm
  { { EventTag(1), "DoorStateChange", ItemType::kDefault }, 644 }, // DoorLock::DoorStateChange
  { { EventTag(2), "LockOperation", ItemType::kDefault }, 645 }, // DoorLock::LockOperation
  { { EventTag(3), "LockOperationError", ItemType::kDefault }, 646 }, // DoorLock::LockOperationError
  { { EventTag(4), "LockUserChange", ItemType::kDefault }, 647 }, // DoorLock::LockUserChange
  { { CommandTag(0), "LockDoor", ItemType::kDefault }, 617 }, // DoorLock::LockDoor::LockDoorRequest
  { { CommandTag(1), "UnlockDoor", ItemType::kDefault }, 618 }, // DoorLock::UnlockDoor::UnlockDoorRequest
  { { CommandTag(3), "UnlockWithTimeout", ItemType::kDefault }, 619 }, // DoorLock::UnlockWithTimeout::UnlockWithTimeoutRequest
  { { CommandTag(11), "SetWeekDaySchedule", ItemType::kDefault }, 620 }, // DoorLock::SetWeekDaySchedule::SetWeekDayScheduleRequest
  { { CommandTag(12), "GetWeekDaySchedule", ItemType::kDefault }, 621 }, // DoorLock::GetWeekDaySchedule::GetWeekDayScheduleRequest
  { { CommandTag(13), "ClearWeekDaySchedule", ItemType::kDefault }, 623 }, // DoorLock::ClearWeekDaySchedule::ClearWeekDayScheduleRequest
  { { CommandTag(14), "SetYearDaySchedule", ItemType::kDefault }, 624 }, // DoorLock::SetYearDaySchedule::SetYearDayScheduleRequest
  { { CommandTag(15), "GetYearDaySchedule", ItemType::kDefault }, 625 }, // DoorLock::GetYearDaySchedule::GetYearDayScheduleRequest
  { { CommandTag(16), "ClearYearDaySchedule", ItemType::kDefault }, 627 }, // DoorLock::ClearYearDaySchedule::ClearYearDayScheduleRequest
  { { CommandTag(17), "SetHolidaySchedule", ItemType::kDefault }, 628 }, // DoorLock::SetHolidaySchedule::SetHolidayScheduleRequest
  { { CommandTag(18), "GetHolidaySchedule", ItemType::kDefault }, 629 }, // DoorLock::GetHolidaySchedule::GetHolidayScheduleRequest
  { { CommandTag(19), "ClearHolidaySchedule", ItemType::kDefault }, 631 }, // DoorLock::ClearHolidaySchedule::ClearHolidayScheduleRequest
  { { CommandTag(26), "SetUser", ItemType::kDefault }, 632 }, // DoorLock::SetUser::SetUserRequest
  { { CommandTag(27), "GetUser", ItemType::kDefault }, 633 }, // DoorLock::GetUser::GetUserRequest
  { { CommandTag(29), "ClearUser", ItemType::kDefault }, 635 }, // DoorLock::ClearUser::ClearUserRequest
  { { CommandTag(34), "SetCredential", ItemType::kDefault }, 636 }, // DoorLock::SetCredential::SetCredentialRequest
  { { CommandTag(36), "GetCredentialStatus", ItemType::kDefault }, 638 }, // DoorLock::GetCredentialStatus::GetCredentialStatusRequest
  { { CommandTag(38), "ClearCredential", ItemType::kDefault }, 640 }, // DoorLock::ClearCredential::ClearCredentialRequest
  { { CommandTag(39), "UnboltDoor", ItemType::kDefault }, 641 }, // DoorLock::UnboltDoor::UnboltDoorRequest
  { { CommandTag(40), "SetAliroReaderConfig", ItemType::kDefault }, 642 }, // DoorLock::SetAliroReaderConfig::SetAliroReaderConfigRequest
  { { CommandTag(41), "ClearAliroReaderConfig", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::ClearAliroReaderConfig::()
  { { CommandTag(12), "GetWeekDayScheduleResponse", ItemType::kDefault }, 622 }, // DoorLock::GetWeekDayScheduleResponse
  { { CommandTag(15), "GetYearDayScheduleResponse", ItemType::kDefault }, 626 }, // DoorLock::GetYearDayScheduleResponse
  { { CommandTag(18), "GetHolidayScheduleResponse", ItemType::kDefault }, 630 }, // DoorLock::GetHolidayScheduleResponse
  { { CommandTag(28), "GetUserResponse", ItemType::kDefault }, 634 }, // DoorLock::GetUserResponse
  { { CommandTag(35), "SetCredentialResponse", ItemType::kDefault }, 637 }, // DoorLock::SetCredentialResponse
  { { CommandTag(37), "GetCredentialStatusResponse", ItemType::kDefault }, 639 }, // DoorLock::GetCredentialStatusResponse
};

const Entry<ItemInfo> _DoorLock_CredentialStruct[] = {
  { { ContextTag(0), "credentialType", ItemType::kEnum }, 651 }, // DoorLock::CredentialTypeEnum
  { { ContextTag(1), "credentialIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_LockDoorRequest[] = {
  { { ContextTag(0), "PINCode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
};

const Entry<ItemInfo> _DoorLock_UnlockDoorRequest[] = {
  { { ContextTag(0), "PINCode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
};

const Entry<ItemInfo> _DoorLock_UnlockWithTimeoutRequest[] = {
  { { ContextTag(0), "timeout", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(1), "PINCode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
};

const Entry<ItemInfo> _DoorLock_SetWeekDayScheduleRequest[] = {
  { { ContextTag(0), "weekDayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(2), "daysMask", ItemType::kBitmap }, 669 }, // DoorLock::DaysMaskMap
  { { ContextTag(3), "startHour", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(4), "startMinute", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(5), "endHour", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(6), "endMinute", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
};

const Entry<ItemInfo> _DoorLock_GetWeekDayScheduleRequest[] = {
  { { ContextTag(0), "weekDayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_GetWeekDayScheduleResponse[] = {
  { { ContextTag(0), "weekDayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(2), "status", ItemType::kEnum }, 655 }, // DoorLock::DlStatus
  { { ContextTag(3), "daysMask", ItemType::kBitmap }, 669 }, // DoorLock::DaysMaskMap
  { { ContextTag(4), "startHour", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(5), "startMinute", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(6), "endHour", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(7), "endMinute", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
};

const Entry<ItemInfo> _DoorLock_ClearWeekDayScheduleRequest[] = {
  { { ContextTag(0), "weekDayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_SetYearDayScheduleRequest[] = {
  { { ContextTag(0), "yearDayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(2), "localStartTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::epoch_s
  { { ContextTag(3), "localEndTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::epoch_s
};

const Entry<ItemInfo> _DoorLock_GetYearDayScheduleRequest[] = {
  { { ContextTag(0), "yearDayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_GetYearDayScheduleResponse[] = {
  { { ContextTag(0), "yearDayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(2), "status", ItemType::kEnum }, 655 }, // DoorLock::DlStatus
  { { ContextTag(3), "localStartTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::epoch_s
  { { ContextTag(4), "localEndTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::epoch_s
};

const Entry<ItemInfo> _DoorLock_ClearYearDayScheduleRequest[] = {
  { { ContextTag(0), "yearDayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_SetHolidayScheduleRequest[] = {
  { { ContextTag(0), "holidayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "localStartTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::epoch_s
  { { ContextTag(2), "localEndTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::epoch_s
  { { ContextTag(3), "operatingMode", ItemType::kEnum }, 664 }, // DoorLock::OperatingModeEnum
};

const Entry<ItemInfo> _DoorLock_GetHolidayScheduleRequest[] = {
  { { ContextTag(0), "holidayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
};

const Entry<ItemInfo> _DoorLock_GetHolidayScheduleResponse[] = {
  { { ContextTag(0), "holidayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
  { { ContextTag(1), "status", ItemType::kEnum }, 655 }, // DoorLock::DlStatus
  { { ContextTag(2), "localStartTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::epoch_s
  { { ContextTag(3), "localEndTime", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::epoch_s
  { { ContextTag(4), "operatingMode", ItemType::kEnum }, 664 }, // DoorLock::OperatingModeEnum
};

const Entry<ItemInfo> _DoorLock_ClearHolidayScheduleRequest[] = {
  { { ContextTag(0), "holidayIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int8u
};

const Entry<ItemInfo> _DoorLock_SetUserRequest[] = {
  { { ContextTag(0), "operationType", ItemType::kEnum }, 652 }, // DoorLock::DataOperationTypeEnum
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(2), "userName", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::char_string
  { { ContextTag(3), "userUniqueID", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int32u
  { { ContextTag(4), "userStatus", ItemType::kEnum }, 667 }, // DoorLock::UserStatusEnum
  { { ContextTag(5), "userType", ItemType::kEnum }, 668 }, // DoorLock::UserTypeEnum
  { { ContextTag(6), "credentialRule", ItemType::kEnum }, 650 }, // DoorLock::CredentialRuleEnum
};

const Entry<ItemInfo> _DoorLock_GetUserRequest[] = {
  { { ContextTag(0), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_GetUserResponse[] = {
  { { ContextTag(0), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(1), "userName", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::char_string
  { { ContextTag(2), "userUniqueID", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int32u
  { { ContextTag(3), "userStatus", ItemType::kEnum }, 667 }, // DoorLock::UserStatusEnum
  { { ContextTag(4), "userType", ItemType::kEnum }, 668 }, // DoorLock::UserTypeEnum
  { { ContextTag(5), "credentialRule", ItemType::kEnum }, 650 }, // DoorLock::CredentialRuleEnum
  { { ContextTag(6), "credentials", ItemType::kList }, 648 }, // DoorLock::CredentialStruct[]
  { { ContextTag(7), "creatorFabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::fabric_idx
  { { ContextTag(8), "lastModifiedFabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::fabric_idx
  { { ContextTag(9), "nextUserIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_ClearUserRequest[] = {
  { { ContextTag(0), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_SetCredentialRequest[] = {
  { { ContextTag(0), "operationType", ItemType::kEnum }, 652 }, // DoorLock::DataOperationTypeEnum
  { { ContextTag(1), "credential", ItemType::kDefault }, 616 }, // DoorLock::CredentialStruct
  { { ContextTag(2), "credentialData", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::long_octet_string
  { { ContextTag(3), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(4), "userStatus", ItemType::kEnum }, 667 }, // DoorLock::UserStatusEnum
  { { ContextTag(5), "userType", ItemType::kEnum }, 668 }, // DoorLock::UserTypeEnum
};

const Entry<ItemInfo> _DoorLock_SetCredentialResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 655 }, // DoorLock::DlStatus
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(2), "nextCredentialIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_GetCredentialStatusRequest[] = {
  { { ContextTag(0), "credential", ItemType::kDefault }, 616 }, // DoorLock::CredentialStruct
};

const Entry<ItemInfo> _DoorLock_GetCredentialStatusResponse[] = {
  { { ContextTag(0), "credentialExists", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::boolean
  { { ContextTag(1), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(2), "creatorFabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::fabric_idx
  { { ContextTag(3), "lastModifiedFabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::fabric_idx
  { { ContextTag(4), "nextCredentialIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_ClearCredentialRequest[] = {
  { { ContextTag(0), "credential", ItemType::kDefault }, 616 }, // DoorLock::CredentialStruct
};

const Entry<ItemInfo> _DoorLock_UnboltDoorRequest[] = {
  { { ContextTag(0), "PINCode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
};

const Entry<ItemInfo> _DoorLock_SetAliroReaderConfigRequest[] = {
  { { ContextTag(0), "signingKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
  { { ContextTag(1), "verificationKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
  { { ContextTag(2), "groupIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
  { { ContextTag(3), "groupResolvingKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::octet_string
};

const Entry<ItemInfo> _DoorLock_DoorLockAlarm[] = {
  { { ContextTag(0), "alarmCode", ItemType::kEnum }, 649 }, // DoorLock::AlarmCodeEnum
};

const Entry<ItemInfo> _DoorLock_DoorStateChange[] = {
  { { ContextTag(0), "doorState", ItemType::kEnum }, 661 }, // DoorLock::DoorStateEnum
};

const Entry<ItemInfo> _DoorLock_LockOperation[] = {
  { { ContextTag(0), "lockOperationType", ItemType::kEnum }, 663 }, // DoorLock::LockOperationTypeEnum
  { { ContextTag(1), "operationSource", ItemType::kEnum }, 666 }, // DoorLock::OperationSourceEnum
  { { ContextTag(2), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(3), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::fabric_idx
  { { ContextTag(4), "sourceNode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::node_id
  { { ContextTag(5), "credentials", ItemType::kList }, 648 }, // DoorLock::CredentialStruct[]
};

const Entry<ItemInfo> _DoorLock_LockOperationError[] = {
  { { ContextTag(0), "lockOperationType", ItemType::kEnum }, 663 }, // DoorLock::LockOperationTypeEnum
  { { ContextTag(1), "operationSource", ItemType::kEnum }, 666 }, // DoorLock::OperationSourceEnum
  { { ContextTag(2), "operationError", ItemType::kEnum }, 665 }, // DoorLock::OperationErrorEnum
  { { ContextTag(3), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(4), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::fabric_idx
  { { ContextTag(5), "sourceNode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::node_id
  { { ContextTag(6), "credentials", ItemType::kList }, 648 }, // DoorLock::CredentialStruct[]
};

const Entry<ItemInfo> _DoorLock_LockUserChange[] = {
  { { ContextTag(0), "lockDataType", ItemType::kEnum }, 662 }, // DoorLock::LockDataTypeEnum
  { { ContextTag(1), "dataOperationType", ItemType::kEnum }, 652 }, // DoorLock::DataOperationTypeEnum
  { { ContextTag(2), "operationSource", ItemType::kEnum }, 666 }, // DoorLock::OperationSourceEnum
  { { ContextTag(3), "userIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
  { { ContextTag(4), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::fabric_idx
  { { ContextTag(5), "sourceNode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::node_id
  { { ContextTag(6), "dataIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::int16u
};

const Entry<ItemInfo> _DoorLock_CredentialStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 616 }, // DoorLock_CredentialStruct[]
};

const Entry<ItemInfo> _DoorLock_AlarmCodeEnum[] = {
  { { ConstantValueTag(0x0), "kLockJammed", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::AlarmCodeEnum::kLockJammed
  { { ConstantValueTag(0x1), "kLockFactoryReset", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::AlarmCodeEnum::kLockFactoryReset
  { { ConstantValueTag(0x3), "kLockRadioPowerCycled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::AlarmCodeEnum::kLockRadioPowerCycled
  { { ConstantValueTag(0x4), "kWrongCodeEntryLimit", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::AlarmCodeEnum::kWrongCodeEntryLimit
  { { ConstantValueTag(0x5), "kFrontEsceutcheonRemoved", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::AlarmCodeEnum::kFrontEsceutcheonRemoved
  { { ConstantValueTag(0x6), "kDoorForcedOpen", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::AlarmCodeEnum::kDoorForcedOpen
  { { ConstantValueTag(0x7), "kDoorAjar", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::AlarmCodeEnum::kDoorAjar
  { { ConstantValueTag(0x8), "kForcedUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::AlarmCodeEnum::kForcedUser
};

const Entry<ItemInfo> _DoorLock_CredentialRuleEnum[] = {
  { { ConstantValueTag(0x0), "kSingle", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialRuleEnum::kSingle
  { { ConstantValueTag(0x1), "kDual", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialRuleEnum::kDual
  { { ConstantValueTag(0x2), "kTri", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialRuleEnum::kTri
};

const Entry<ItemInfo> _DoorLock_CredentialTypeEnum[] = {
  { { ConstantValueTag(0x0), "kProgrammingPIN", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kProgrammingPIN
  { { ConstantValueTag(0x1), "kPIN", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kPIN
  { { ConstantValueTag(0x2), "kRFID", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kRFID
  { { ConstantValueTag(0x3), "kFingerprint", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kFingerprint
  { { ConstantValueTag(0x4), "kFingerVein", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kFingerVein
  { { ConstantValueTag(0x5), "kFace", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kFace
  { { ConstantValueTag(0x6), "kAliroCredentialIssuerKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kAliroCredentialIssuerKey
  { { ConstantValueTag(0x7), "kAliroEvictableEndpointKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kAliroEvictableEndpointKey
  { { ConstantValueTag(0x8), "kAliroNonEvictableEndpointKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::CredentialTypeEnum::kAliroNonEvictableEndpointKey
};

const Entry<ItemInfo> _DoorLock_DataOperationTypeEnum[] = {
  { { ConstantValueTag(0x0), "kAdd", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DataOperationTypeEnum::kAdd
  { { ConstantValueTag(0x1), "kClear", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DataOperationTypeEnum::kClear
  { { ConstantValueTag(0x2), "kModify", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DataOperationTypeEnum::kModify
};

const Entry<ItemInfo> _DoorLock_DlLockState[] = {
  { { ConstantValueTag(0x0), "kNotFullyLocked", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockState::kNotFullyLocked
  { { ConstantValueTag(0x1), "kLocked", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockState::kLocked
  { { ConstantValueTag(0x2), "kUnlocked", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockState::kUnlocked
  { { ConstantValueTag(0x3), "kUnlatched", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockState::kUnlatched
};

const Entry<ItemInfo> _DoorLock_DlLockType[] = {
  { { ConstantValueTag(0x0), "kDeadBolt", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kDeadBolt
  { { ConstantValueTag(0x1), "kMagnetic", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kMagnetic
  { { ConstantValueTag(0x2), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kOther
  { { ConstantValueTag(0x3), "kMortise", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kMortise
  { { ConstantValueTag(0x4), "kRim", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kRim
  { { ConstantValueTag(0x5), "kLatchBolt", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kLatchBolt
  { { ConstantValueTag(0x6), "kCylindricalLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kCylindricalLock
  { { ConstantValueTag(0x7), "kTubularLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kTubularLock
  { { ConstantValueTag(0x8), "kInterconnectedLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kInterconnectedLock
  { { ConstantValueTag(0x9), "kDeadLatch", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kDeadLatch
  { { ConstantValueTag(0xA), "kDoorFurniture", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kDoorFurniture
  { { ConstantValueTag(0xB), "kEurocylinder", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLockType::kEurocylinder
};

const Entry<ItemInfo> _DoorLock_DlStatus[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlStatus::kSuccess
  { { ConstantValueTag(0x1), "kFailure", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlStatus::kFailure
  { { ConstantValueTag(0x2), "kDuplicate", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlStatus::kDuplicate
  { { ConstantValueTag(0x3), "kOccupied", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlStatus::kOccupied
  { { ConstantValueTag(0x85), "kInvalidField", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlStatus::kInvalidField
  { { ConstantValueTag(0x89), "kResourceExhausted", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlStatus::kResourceExhausted
  { { ConstantValueTag(0x8B), "kNotFound", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlStatus::kNotFound
};

const Entry<ItemInfo> _DoorLock_DoorLockOperationEventCode[] = {
  { { ConstantValueTag(0x0), "kUnknownOrMfgSpecific", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kUnknownOrMfgSpecific
  { { ConstantValueTag(0x1), "kLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kLock
  { { ConstantValueTag(0x2), "kUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kUnlock
  { { ConstantValueTag(0x3), "kLockInvalidPinOrId", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kLockInvalidPinOrId
  { { ConstantValueTag(0x4), "kLockInvalidSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kLockInvalidSchedule
  { { ConstantValueTag(0x5), "kUnlockInvalidPinOrId", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kUnlockInvalidPinOrId
  { { ConstantValueTag(0x6), "kUnlockInvalidSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kUnlockInvalidSchedule
  { { ConstantValueTag(0x7), "kOneTouchLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kOneTouchLock
  { { ConstantValueTag(0x8), "kKeyLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kKeyLock
  { { ConstantValueTag(0x9), "kKeyUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kKeyUnlock
  { { ConstantValueTag(0xA), "kAutoLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kAutoLock
  { { ConstantValueTag(0xB), "kScheduleLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kScheduleLock
  { { ConstantValueTag(0xC), "kScheduleUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kScheduleUnlock
  { { ConstantValueTag(0xD), "kManualLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kManualLock
  { { ConstantValueTag(0xE), "kManualUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockOperationEventCode::kManualUnlock
};

const Entry<ItemInfo> _DoorLock_DoorLockProgrammingEventCode[] = {
  { { ConstantValueTag(0x0), "kUnknownOrMfgSpecific", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockProgrammingEventCode::kUnknownOrMfgSpecific
  { { ConstantValueTag(0x1), "kMasterCodeChanged", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockProgrammingEventCode::kMasterCodeChanged
  { { ConstantValueTag(0x2), "kPinAdded", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockProgrammingEventCode::kPinAdded
  { { ConstantValueTag(0x3), "kPinDeleted", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockProgrammingEventCode::kPinDeleted
  { { ConstantValueTag(0x4), "kPinChanged", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockProgrammingEventCode::kPinChanged
  { { ConstantValueTag(0x5), "kIdAdded", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockProgrammingEventCode::kIdAdded
  { { ConstantValueTag(0x6), "kIdDeleted", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockProgrammingEventCode::kIdDeleted
};

const Entry<ItemInfo> _DoorLock_DoorLockSetPinOrIdStatus[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockSetPinOrIdStatus::kSuccess
  { { ConstantValueTag(0x1), "kGeneralFailure", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockSetPinOrIdStatus::kGeneralFailure
  { { ConstantValueTag(0x2), "kMemoryFull", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockSetPinOrIdStatus::kMemoryFull
  { { ConstantValueTag(0x3), "kDuplicateCodeError", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockSetPinOrIdStatus::kDuplicateCodeError
};

const Entry<ItemInfo> _DoorLock_DoorLockUserStatus[] = {
  { { ConstantValueTag(0x0), "kAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserStatus::kAvailable
  { { ConstantValueTag(0x1), "kOccupiedEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserStatus::kOccupiedEnabled
  { { ConstantValueTag(0x3), "kOccupiedDisabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserStatus::kOccupiedDisabled
  { { ConstantValueTag(0xFF), "kNotSupported", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserStatus::kNotSupported
};

const Entry<ItemInfo> _DoorLock_DoorLockUserType[] = {
  { { ConstantValueTag(0x0), "kUnrestricted", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserType::kUnrestricted
  { { ConstantValueTag(0x1), "kYearDayScheduleUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserType::kYearDayScheduleUser
  { { ConstantValueTag(0x2), "kWeekDayScheduleUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserType::kWeekDayScheduleUser
  { { ConstantValueTag(0x3), "kMasterUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserType::kMasterUser
  { { ConstantValueTag(0x4), "kNonAccessUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserType::kNonAccessUser
  { { ConstantValueTag(0xFF), "kNotSupported", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockUserType::kNotSupported
};

const Entry<ItemInfo> _DoorLock_DoorStateEnum[] = {
  { { ConstantValueTag(0x0), "kDoorOpen", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorStateEnum::kDoorOpen
  { { ConstantValueTag(0x1), "kDoorClosed", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorStateEnum::kDoorClosed
  { { ConstantValueTag(0x2), "kDoorJammed", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorStateEnum::kDoorJammed
  { { ConstantValueTag(0x3), "kDoorForcedOpen", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorStateEnum::kDoorForcedOpen
  { { ConstantValueTag(0x4), "kDoorUnspecifiedError", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorStateEnum::kDoorUnspecifiedError
  { { ConstantValueTag(0x5), "kDoorAjar", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorStateEnum::kDoorAjar
};

const Entry<ItemInfo> _DoorLock_LockDataTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kUnspecified
  { { ConstantValueTag(0x1), "kProgrammingCode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kProgrammingCode
  { { ConstantValueTag(0x2), "kUserIndex", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kUserIndex
  { { ConstantValueTag(0x3), "kWeekDaySchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kWeekDaySchedule
  { { ConstantValueTag(0x4), "kYearDaySchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kYearDaySchedule
  { { ConstantValueTag(0x5), "kHolidaySchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kHolidaySchedule
  { { ConstantValueTag(0x6), "kPIN", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kPIN
  { { ConstantValueTag(0x7), "kRFID", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kRFID
  { { ConstantValueTag(0x8), "kFingerprint", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kFingerprint
  { { ConstantValueTag(0x9), "kFingerVein", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kFingerVein
  { { ConstantValueTag(0xA), "kFace", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kFace
  { { ConstantValueTag(0xB), "kAliroCredentialIssuerKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kAliroCredentialIssuerKey
  { { ConstantValueTag(0xC), "kAliroEvictableEndpointKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kAliroEvictableEndpointKey
  { { ConstantValueTag(0xD), "kAliroNonEvictableEndpointKey", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockDataTypeEnum::kAliroNonEvictableEndpointKey
};

const Entry<ItemInfo> _DoorLock_LockOperationTypeEnum[] = {
  { { ConstantValueTag(0x0), "kLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockOperationTypeEnum::kLock
  { { ConstantValueTag(0x1), "kUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockOperationTypeEnum::kUnlock
  { { ConstantValueTag(0x2), "kNonAccessUserEvent", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockOperationTypeEnum::kNonAccessUserEvent
  { { ConstantValueTag(0x3), "kForcedUserEvent", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockOperationTypeEnum::kForcedUserEvent
  { { ConstantValueTag(0x4), "kUnlatch", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::LockOperationTypeEnum::kUnlatch
};

const Entry<ItemInfo> _DoorLock_OperatingModeEnum[] = {
  { { ConstantValueTag(0x0), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperatingModeEnum::kNormal
  { { ConstantValueTag(0x1), "kVacation", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperatingModeEnum::kVacation
  { { ConstantValueTag(0x2), "kPrivacy", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperatingModeEnum::kPrivacy
  { { ConstantValueTag(0x3), "kNoRemoteLockUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperatingModeEnum::kNoRemoteLockUnlock
  { { ConstantValueTag(0x4), "kPassage", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperatingModeEnum::kPassage
};

const Entry<ItemInfo> _DoorLock_OperationErrorEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationErrorEnum::kUnspecified
  { { ConstantValueTag(0x1), "kInvalidCredential", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationErrorEnum::kInvalidCredential
  { { ConstantValueTag(0x2), "kDisabledUserDenied", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationErrorEnum::kDisabledUserDenied
  { { ConstantValueTag(0x3), "kRestricted", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationErrorEnum::kRestricted
  { { ConstantValueTag(0x4), "kInsufficientBattery", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationErrorEnum::kInsufficientBattery
};

const Entry<ItemInfo> _DoorLock_OperationSourceEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kUnspecified
  { { ConstantValueTag(0x1), "kManual", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kManual
  { { ConstantValueTag(0x2), "kProprietaryRemote", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kProprietaryRemote
  { { ConstantValueTag(0x3), "kKeypad", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kKeypad
  { { ConstantValueTag(0x4), "kAuto", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kAuto
  { { ConstantValueTag(0x5), "kButton", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kButton
  { { ConstantValueTag(0x6), "kSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kSchedule
  { { ConstantValueTag(0x7), "kRemote", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kRemote
  { { ConstantValueTag(0x8), "kRFID", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kRFID
  { { ConstantValueTag(0x9), "kBiometric", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kBiometric
  { { ConstantValueTag(0xA), "kAliro", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::OperationSourceEnum::kAliro
};

const Entry<ItemInfo> _DoorLock_UserStatusEnum[] = {
  { { ConstantValueTag(0x0), "kAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserStatusEnum::kAvailable
  { { ConstantValueTag(0x1), "kOccupiedEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserStatusEnum::kOccupiedEnabled
  { { ConstantValueTag(0x3), "kOccupiedDisabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserStatusEnum::kOccupiedDisabled
};

const Entry<ItemInfo> _DoorLock_UserTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUnrestrictedUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kUnrestrictedUser
  { { ConstantValueTag(0x1), "kYearDayScheduleUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kYearDayScheduleUser
  { { ConstantValueTag(0x2), "kWeekDayScheduleUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kWeekDayScheduleUser
  { { ConstantValueTag(0x3), "kProgrammingUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kProgrammingUser
  { { ConstantValueTag(0x4), "kNonAccessUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kNonAccessUser
  { { ConstantValueTag(0x5), "kForcedUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kForcedUser
  { { ConstantValueTag(0x6), "kDisposableUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kDisposableUser
  { { ConstantValueTag(0x7), "kExpiringUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kExpiringUser
  { { ConstantValueTag(0x8), "kScheduleRestrictedUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kScheduleRestrictedUser
  { { ConstantValueTag(0x9), "kRemoteOnlyUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::UserTypeEnum::kRemoteOnlyUser
};

const Entry<ItemInfo> _DoorLock_DaysMaskMap[] = {
  { { ConstantValueTag(0x1), "kSunday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DaysMaskMap::kSunday
  { { ConstantValueTag(0x2), "kMonday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DaysMaskMap::kMonday
  { { ConstantValueTag(0x4), "kTuesday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DaysMaskMap::kTuesday
  { { ConstantValueTag(0x8), "kWednesday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DaysMaskMap::kWednesday
  { { ConstantValueTag(0x10), "kThursday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DaysMaskMap::kThursday
  { { ConstantValueTag(0x20), "kFriday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DaysMaskMap::kFriday
  { { ConstantValueTag(0x40), "kSaturday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DaysMaskMap::kSaturday
};

const Entry<ItemInfo> _DoorLock_DlCredentialRuleMask[] = {
  { { ConstantValueTag(0x1), "kSingle", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlCredentialRuleMask::kSingle
  { { ConstantValueTag(0x2), "kDual", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlCredentialRuleMask::kDual
  { { ConstantValueTag(0x4), "kTri", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlCredentialRuleMask::kTri
};

const Entry<ItemInfo> _DoorLock_DlCredentialRulesSupport[] = {
  { { ConstantValueTag(0x1), "kSingle", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlCredentialRulesSupport::kSingle
  { { ConstantValueTag(0x2), "kDual", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlCredentialRulesSupport::kDual
  { { ConstantValueTag(0x4), "kTri", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlCredentialRulesSupport::kTri
};

const Entry<ItemInfo> _DoorLock_DlDefaultConfigurationRegister[] = {
  { { ConstantValueTag(0x1), "kEnableLocalProgrammingEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlDefaultConfigurationRegister::kEnableLocalProgrammingEnabled
  { { ConstantValueTag(0x2), "kKeypadInterfaceDefaultAccessEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlDefaultConfigurationRegister::kKeypadInterfaceDefaultAccessEnabled
  { { ConstantValueTag(0x4), "kRemoteInterfaceDefaultAccessIsEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlDefaultConfigurationRegister::kRemoteInterfaceDefaultAccessIsEnabled
  { { ConstantValueTag(0x20), "kSoundEnabled", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlDefaultConfigurationRegister::kSoundEnabled
  { { ConstantValueTag(0x40), "kAutoRelockTimeSet", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlDefaultConfigurationRegister::kAutoRelockTimeSet
  { { ConstantValueTag(0x80), "kLEDSettingsSet", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlDefaultConfigurationRegister::kLEDSettingsSet
};

const Entry<ItemInfo> _DoorLock_DlKeypadOperationEventMask[] = {
  { { ConstantValueTag(0x1), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadOperationEventMask::kUnknown
  { { ConstantValueTag(0x2), "kLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadOperationEventMask::kLock
  { { ConstantValueTag(0x4), "kUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadOperationEventMask::kUnlock
  { { ConstantValueTag(0x8), "kLockInvalidPIN", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadOperationEventMask::kLockInvalidPIN
  { { ConstantValueTag(0x10), "kLockInvalidSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadOperationEventMask::kLockInvalidSchedule
  { { ConstantValueTag(0x20), "kUnlockInvalidCode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadOperationEventMask::kUnlockInvalidCode
  { { ConstantValueTag(0x40), "kUnlockInvalidSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadOperationEventMask::kUnlockInvalidSchedule
  { { ConstantValueTag(0x80), "kNonAccessUserOpEvent", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadOperationEventMask::kNonAccessUserOpEvent
};

const Entry<ItemInfo> _DoorLock_DlKeypadProgrammingEventMask[] = {
  { { ConstantValueTag(0x1), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadProgrammingEventMask::kUnknown
  { { ConstantValueTag(0x2), "kProgrammingPINChanged", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadProgrammingEventMask::kProgrammingPINChanged
  { { ConstantValueTag(0x4), "kPINAdded", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadProgrammingEventMask::kPINAdded
  { { ConstantValueTag(0x8), "kPINCleared", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadProgrammingEventMask::kPINCleared
  { { ConstantValueTag(0x10), "kPINChanged", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlKeypadProgrammingEventMask::kPINChanged
};

const Entry<ItemInfo> _DoorLock_DlLocalProgrammingFeatures[] = {
  { { ConstantValueTag(0x1), "kAddUsersCredentialsSchedulesLocally", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLocalProgrammingFeatures::kAddUsersCredentialsSchedulesLocally
  { { ConstantValueTag(0x2), "kModifyUsersCredentialsSchedulesLocally", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLocalProgrammingFeatures::kModifyUsersCredentialsSchedulesLocally
  { { ConstantValueTag(0x4), "kClearUsersCredentialsSchedulesLocally", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLocalProgrammingFeatures::kClearUsersCredentialsSchedulesLocally
  { { ConstantValueTag(0x8), "kAdjustLockSettingsLocally", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlLocalProgrammingFeatures::kAdjustLockSettingsLocally
};

const Entry<ItemInfo> _DoorLock_DlManualOperationEventMask[] = {
  { { ConstantValueTag(0x1), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kUnknown
  { { ConstantValueTag(0x2), "kThumbturnLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kThumbturnLock
  { { ConstantValueTag(0x4), "kThumbturnUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kThumbturnUnlock
  { { ConstantValueTag(0x8), "kOneTouchLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kOneTouchLock
  { { ConstantValueTag(0x10), "kKeyLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kKeyLock
  { { ConstantValueTag(0x20), "kKeyUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kKeyUnlock
  { { ConstantValueTag(0x40), "kAutoLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kAutoLock
  { { ConstantValueTag(0x80), "kScheduleLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kScheduleLock
  { { ConstantValueTag(0x100), "kScheduleUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kScheduleUnlock
  { { ConstantValueTag(0x200), "kManualLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kManualLock
  { { ConstantValueTag(0x400), "kManualUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlManualOperationEventMask::kManualUnlock
};

const Entry<ItemInfo> _DoorLock_DlRFIDOperationEventMask[] = {
  { { ConstantValueTag(0x1), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDOperationEventMask::kUnknown
  { { ConstantValueTag(0x2), "kLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDOperationEventMask::kLock
  { { ConstantValueTag(0x4), "kUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDOperationEventMask::kUnlock
  { { ConstantValueTag(0x8), "kLockInvalidRFID", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDOperationEventMask::kLockInvalidRFID
  { { ConstantValueTag(0x10), "kLockInvalidSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDOperationEventMask::kLockInvalidSchedule
  { { ConstantValueTag(0x20), "kUnlockInvalidRFID", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDOperationEventMask::kUnlockInvalidRFID
  { { ConstantValueTag(0x40), "kUnlockInvalidSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDOperationEventMask::kUnlockInvalidSchedule
};

const Entry<ItemInfo> _DoorLock_DlRFIDProgrammingEventMask[] = {
  { { ConstantValueTag(0x1), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDProgrammingEventMask::kUnknown
  { { ConstantValueTag(0x20), "kRFIDCodeAdded", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDProgrammingEventMask::kRFIDCodeAdded
  { { ConstantValueTag(0x40), "kRFIDCodeCleared", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRFIDProgrammingEventMask::kRFIDCodeCleared
};

const Entry<ItemInfo> _DoorLock_DlRemoteOperationEventMask[] = {
  { { ConstantValueTag(0x1), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteOperationEventMask::kUnknown
  { { ConstantValueTag(0x2), "kLock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteOperationEventMask::kLock
  { { ConstantValueTag(0x4), "kUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteOperationEventMask::kUnlock
  { { ConstantValueTag(0x8), "kLockInvalidCode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteOperationEventMask::kLockInvalidCode
  { { ConstantValueTag(0x10), "kLockInvalidSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteOperationEventMask::kLockInvalidSchedule
  { { ConstantValueTag(0x20), "kUnlockInvalidCode", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteOperationEventMask::kUnlockInvalidCode
  { { ConstantValueTag(0x40), "kUnlockInvalidSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteOperationEventMask::kUnlockInvalidSchedule
};

const Entry<ItemInfo> _DoorLock_DlRemoteProgrammingEventMask[] = {
  { { ConstantValueTag(0x1), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteProgrammingEventMask::kUnknown
  { { ConstantValueTag(0x2), "kProgrammingPINChanged", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteProgrammingEventMask::kProgrammingPINChanged
  { { ConstantValueTag(0x4), "kPINAdded", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteProgrammingEventMask::kPINAdded
  { { ConstantValueTag(0x8), "kPINCleared", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteProgrammingEventMask::kPINCleared
  { { ConstantValueTag(0x10), "kPINChanged", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteProgrammingEventMask::kPINChanged
  { { ConstantValueTag(0x20), "kRFIDCodeAdded", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteProgrammingEventMask::kRFIDCodeAdded
  { { ConstantValueTag(0x40), "kRFIDCodeCleared", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlRemoteProgrammingEventMask::kRFIDCodeCleared
};

const Entry<ItemInfo> _DoorLock_DlSupportedOperatingModes[] = {
  { { ConstantValueTag(0x1), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlSupportedOperatingModes::kNormal
  { { ConstantValueTag(0x2), "kVacation", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlSupportedOperatingModes::kVacation
  { { ConstantValueTag(0x4), "kPrivacy", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlSupportedOperatingModes::kPrivacy
  { { ConstantValueTag(0x8), "kNoRemoteLockUnlock", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlSupportedOperatingModes::kNoRemoteLockUnlock
  { { ConstantValueTag(0x10), "kPassage", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DlSupportedOperatingModes::kPassage
};

const Entry<ItemInfo> _DoorLock_DoorLockDayOfWeek[] = {
  { { ConstantValueTag(0x1), "kSunday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockDayOfWeek::kSunday
  { { ConstantValueTag(0x2), "kMonday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockDayOfWeek::kMonday
  { { ConstantValueTag(0x4), "kTuesday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockDayOfWeek::kTuesday
  { { ConstantValueTag(0x8), "kWednesday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockDayOfWeek::kWednesday
  { { ConstantValueTag(0x10), "kThursday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockDayOfWeek::kThursday
  { { ConstantValueTag(0x20), "kFriday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockDayOfWeek::kFriday
  { { ConstantValueTag(0x40), "kSaturday", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::DoorLockDayOfWeek::kSaturday
};

const Entry<ItemInfo> _DoorLock_Feature[] = {
  { { ConstantValueTag(0x1), "kPINCredential", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kPINCredential
  { { ConstantValueTag(0x2), "kRFIDCredential", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kRFIDCredential
  { { ConstantValueTag(0x4), "kFingerCredentials", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kFingerCredentials
  { { ConstantValueTag(0x8), "kLogging", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kLogging
  { { ConstantValueTag(0x10), "kWeekDayAccessSchedules", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kWeekDayAccessSchedules
  { { ConstantValueTag(0x20), "kDoorPositionSensor", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kDoorPositionSensor
  { { ConstantValueTag(0x40), "kFaceCredentials", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kFaceCredentials
  { { ConstantValueTag(0x80), "kCredentialsOverTheAirAccess", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kCredentialsOverTheAirAccess
  { { ConstantValueTag(0x100), "kUser", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kUser
  { { ConstantValueTag(0x200), "kNotification", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kNotification
  { { ConstantValueTag(0x400), "kYearDayAccessSchedules", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kYearDayAccessSchedules
  { { ConstantValueTag(0x800), "kHolidaySchedules", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kHolidaySchedules
  { { ConstantValueTag(0x1000), "kUnbolt", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kUnbolt
  { { ConstantValueTag(0x2000), "kAliroProvisioning", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kAliroProvisioning
  { { ConstantValueTag(0x4000), "kAliroBLEUWB", ItemType::kDefault }, kInvalidNodeIndex }, // DoorLock::Feature::kAliroBLEUWB
};

const Entry<ItemInfo> _WindowCovering[] = {
  { { AttributeTag(0), "type", ItemType::kEnum }, 690 }, // WindowCovering::Type
  { { AttributeTag(1), "physicalClosedLimitLift", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(2), "physicalClosedLimitTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(3), "currentPositionLift", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(4), "currentPositionTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(5), "numberOfActuationsLift", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(6), "numberOfActuationsTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(7), "configStatus", ItemType::kBitmap }, 691 }, // WindowCovering::ConfigStatus
  { { AttributeTag(8), "currentPositionLiftPercentage", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::percent
  { { AttributeTag(9), "currentPositionTiltPercentage", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::percent
  { { AttributeTag(10), "operationalStatus", ItemType::kBitmap }, 694 }, // WindowCovering::OperationalStatus
  { { AttributeTag(11), "targetPositionLiftPercent100ths", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::percent100ths
  { { AttributeTag(12), "targetPositionTiltPercent100ths", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::percent100ths
  { { AttributeTag(13), "endProductType", ItemType::kEnum }, 689 }, // WindowCovering::EndProductType
  { { AttributeTag(14), "currentPositionLiftPercent100ths", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::percent100ths
  { { AttributeTag(15), "currentPositionTiltPercent100ths", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::percent100ths
  { { AttributeTag(16), "installedOpenLimitLift", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(17), "installedClosedLimitLift", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(18), "installedOpenLimitTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(19), "installedClosedLimitTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { AttributeTag(23), "mode", ItemType::kBitmap }, 693 }, // WindowCovering::Mode
  { { AttributeTag(26), "safetyStatus", ItemType::kBitmap }, 695 }, // WindowCovering::SafetyStatus
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // WindowCovering::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // WindowCovering::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // WindowCovering::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // WindowCovering::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 692 }, // WindowCovering::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
  { { CommandTag(0), "UpOrOpen", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::UpOrOpen::()
  { { CommandTag(1), "DownOrClose", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::DownOrClose::()
  { { CommandTag(2), "StopMotion", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::StopMotion::()
  { { CommandTag(4), "GoToLiftValue", ItemType::kDefault }, 685 }, // WindowCovering::GoToLiftValue::GoToLiftValueRequest
  { { CommandTag(5), "GoToLiftPercentage", ItemType::kDefault }, 686 }, // WindowCovering::GoToLiftPercentage::GoToLiftPercentageRequest
  { { CommandTag(7), "GoToTiltValue", ItemType::kDefault }, 687 }, // WindowCovering::GoToTiltValue::GoToTiltValueRequest
  { { CommandTag(8), "GoToTiltPercentage", ItemType::kDefault }, 688 }, // WindowCovering::GoToTiltPercentage::GoToTiltPercentageRequest
};

const Entry<ItemInfo> _WindowCovering_GoToLiftValueRequest[] = {
  { { ContextTag(0), "liftValue", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
};

const Entry<ItemInfo> _WindowCovering_GoToLiftPercentageRequest[] = {
  { { ContextTag(0), "liftPercent100thsValue", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::percent100ths
};

const Entry<ItemInfo> _WindowCovering_GoToTiltValueRequest[] = {
  { { ContextTag(0), "tiltValue", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::int16u
};

const Entry<ItemInfo> _WindowCovering_GoToTiltPercentageRequest[] = {
  { { ContextTag(0), "tiltPercent100thsValue", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::percent100ths
};

const Entry<ItemInfo> _WindowCovering_EndProductType[] = {
  { { ConstantValueTag(0x0), "kRollerShade", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kRollerShade
  { { ConstantValueTag(0x1), "kRomanShade", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kRomanShade
  { { ConstantValueTag(0x2), "kBalloonShade", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kBalloonShade
  { { ConstantValueTag(0x3), "kWovenWood", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kWovenWood
  { { ConstantValueTag(0x4), "kPleatedShade", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kPleatedShade
  { { ConstantValueTag(0x5), "kCellularShade", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kCellularShade
  { { ConstantValueTag(0x6), "kLayeredShade", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kLayeredShade
  { { ConstantValueTag(0x7), "kLayeredShade2D", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kLayeredShade2D
  { { ConstantValueTag(0x8), "kSheerShade", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kSheerShade
  { { ConstantValueTag(0x9), "kTiltOnlyInteriorBlind", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kTiltOnlyInteriorBlind
  { { ConstantValueTag(0xA), "kInteriorBlind", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kInteriorBlind
  { { ConstantValueTag(0xB), "kVerticalBlindStripCurtain", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kVerticalBlindStripCurtain
  { { ConstantValueTag(0xC), "kInteriorVenetianBlind", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kInteriorVenetianBlind
  { { ConstantValueTag(0xD), "kExteriorVenetianBlind", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kExteriorVenetianBlind
  { { ConstantValueTag(0xE), "kLateralLeftCurtain", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kLateralLeftCurtain
  { { ConstantValueTag(0xF), "kLateralRightCurtain", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kLateralRightCurtain
  { { ConstantValueTag(0x10), "kCentralCurtain", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kCentralCurtain
  { { ConstantValueTag(0x11), "kRollerShutter", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kRollerShutter
  { { ConstantValueTag(0x12), "kExteriorVerticalScreen", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kExteriorVerticalScreen
  { { ConstantValueTag(0x13), "kAwningTerracePatio", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kAwningTerracePatio
  { { ConstantValueTag(0x14), "kAwningVerticalScreen", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kAwningVerticalScreen
  { { ConstantValueTag(0x15), "kTiltOnlyPergola", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kTiltOnlyPergola
  { { ConstantValueTag(0x16), "kSwingingShutter", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kSwingingShutter
  { { ConstantValueTag(0x17), "kSlidingShutter", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kSlidingShutter
  { { ConstantValueTag(0xFF), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::EndProductType::kUnknown
};

const Entry<ItemInfo> _WindowCovering_Type[] = {
  { { ConstantValueTag(0x0), "kRollerShade", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kRollerShade
  { { ConstantValueTag(0x1), "kRollerShade2Motor", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kRollerShade2Motor
  { { ConstantValueTag(0x2), "kRollerShadeExterior", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kRollerShadeExterior
  { { ConstantValueTag(0x3), "kRollerShadeExterior2Motor", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kRollerShadeExterior2Motor
  { { ConstantValueTag(0x4), "kDrapery", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kDrapery
  { { ConstantValueTag(0x5), "kAwning", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kAwning
  { { ConstantValueTag(0x6), "kShutter", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kShutter
  { { ConstantValueTag(0x7), "kTiltBlindTiltOnly", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kTiltBlindTiltOnly
  { { ConstantValueTag(0x8), "kTiltBlindLiftAndTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kTiltBlindLiftAndTilt
  { { ConstantValueTag(0x9), "kProjectorScreen", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kProjectorScreen
  { { ConstantValueTag(0xFF), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Type::kUnknown
};

const Entry<ItemInfo> _WindowCovering_ConfigStatus[] = {
  { { ConstantValueTag(0x1), "kOperational", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::ConfigStatus::kOperational
  { { ConstantValueTag(0x2), "kOnlineReserved", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::ConfigStatus::kOnlineReserved
  { { ConstantValueTag(0x4), "kLiftMovementReversed", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::ConfigStatus::kLiftMovementReversed
  { { ConstantValueTag(0x8), "kLiftPositionAware", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::ConfigStatus::kLiftPositionAware
  { { ConstantValueTag(0x10), "kTiltPositionAware", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::ConfigStatus::kTiltPositionAware
  { { ConstantValueTag(0x20), "kLiftEncoderControlled", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::ConfigStatus::kLiftEncoderControlled
  { { ConstantValueTag(0x40), "kTiltEncoderControlled", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::ConfigStatus::kTiltEncoderControlled
};

const Entry<ItemInfo> _WindowCovering_Feature[] = {
  { { ConstantValueTag(0x1), "kLift", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Feature::kLift
  { { ConstantValueTag(0x2), "kTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Feature::kTilt
  { { ConstantValueTag(0x4), "kPositionAwareLift", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Feature::kPositionAwareLift
  { { ConstantValueTag(0x8), "kAbsolutePosition", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Feature::kAbsolutePosition
  { { ConstantValueTag(0x10), "kPositionAwareTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Feature::kPositionAwareTilt
};

const Entry<ItemInfo> _WindowCovering_Mode[] = {
  { { ConstantValueTag(0x1), "kMotorDirectionReversed", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Mode::kMotorDirectionReversed
  { { ConstantValueTag(0x2), "kCalibrationMode", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Mode::kCalibrationMode
  { { ConstantValueTag(0x4), "kMaintenanceMode", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Mode::kMaintenanceMode
  { { ConstantValueTag(0x8), "kLedFeedback", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::Mode::kLedFeedback
};

const Entry<ItemInfo> _WindowCovering_OperationalStatus[] = {
  { { ConstantValueTag(0x3), "kGlobal", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::OperationalStatus::kGlobal
  { { ConstantValueTag(0xC), "kLift", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::OperationalStatus::kLift
  { { ConstantValueTag(0x30), "kTilt", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::OperationalStatus::kTilt
};

const Entry<ItemInfo> _WindowCovering_SafetyStatus[] = {
  { { ConstantValueTag(0x1), "kRemoteLockout", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kRemoteLockout
  { { ConstantValueTag(0x2), "kTamperDetection", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kTamperDetection
  { { ConstantValueTag(0x4), "kFailedCommunication", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kFailedCommunication
  { { ConstantValueTag(0x8), "kPositionFailure", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kPositionFailure
  { { ConstantValueTag(0x10), "kThermalProtection", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kThermalProtection
  { { ConstantValueTag(0x20), "kObstacleDetected", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kObstacleDetected
  { { ConstantValueTag(0x40), "kPower", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kPower
  { { ConstantValueTag(0x80), "kStopInput", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kStopInput
  { { ConstantValueTag(0x100), "kMotorJammed", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kMotorJammed
  { { ConstantValueTag(0x200), "kHardwareFailure", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kHardwareFailure
  { { ConstantValueTag(0x400), "kManualOperation", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kManualOperation
  { { ConstantValueTag(0x800), "kProtection", ItemType::kDefault }, kInvalidNodeIndex }, // WindowCovering::SafetyStatus::kProtection
};

const Entry<ItemInfo> _BarrierControl[] = {
  { { AttributeTag(1), "barrierMovingState", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::enum8
  { { AttributeTag(2), "barrierSafetyStatus", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::bitmap16
  { { AttributeTag(3), "barrierCapabilities", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::bitmap8
  { { AttributeTag(4), "barrierOpenEvents", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int16u
  { { AttributeTag(5), "barrierCloseEvents", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int16u
  { { AttributeTag(6), "barrierCommandOpenEvents", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int16u
  { { AttributeTag(7), "barrierCommandCloseEvents", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int16u
  { { AttributeTag(8), "barrierOpenPeriod", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int16u
  { { AttributeTag(9), "barrierClosePeriod", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int16u
  { { AttributeTag(10), "barrierPosition", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // BarrierControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // BarrierControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // BarrierControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // BarrierControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int16u
  { { CommandTag(0), "BarrierControlGoToPercent", ItemType::kDefault }, 697 }, // BarrierControl::BarrierControlGoToPercent::BarrierControlGoToPercentRequest
  { { CommandTag(1), "BarrierControlStop", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::BarrierControlStop::()
};

const Entry<ItemInfo> _BarrierControl_BarrierControlGoToPercentRequest[] = {
  { { ContextTag(0), "percentOpen", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::int8u
};

const Entry<ItemInfo> _BarrierControl_BarrierControlCapabilities[] = {
  { { ConstantValueTag(0x1), "kPartialBarrier", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::BarrierControlCapabilities::kPartialBarrier
};

const Entry<ItemInfo> _BarrierControl_BarrierControlSafetyStatus[] = {
  { { ConstantValueTag(0x1), "kRemoteLockout", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::BarrierControlSafetyStatus::kRemoteLockout
  { { ConstantValueTag(0x2), "kTemperDetected", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::BarrierControlSafetyStatus::kTemperDetected
  { { ConstantValueTag(0x4), "kFailedCommunication", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::BarrierControlSafetyStatus::kFailedCommunication
  { { ConstantValueTag(0x8), "kPositionFailure", ItemType::kDefault }, kInvalidNodeIndex }, // BarrierControl::BarrierControlSafetyStatus::kPositionFailure
};

const Entry<ItemInfo> _PumpConfigurationAndControl[] = {
  { { AttributeTag(0), "maxPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16s
  { { AttributeTag(1), "maxSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16u
  { { AttributeTag(2), "maxFlow", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16u
  { { AttributeTag(3), "minConstPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16s
  { { AttributeTag(4), "maxConstPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16s
  { { AttributeTag(5), "minCompPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16s
  { { AttributeTag(6), "maxCompPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16s
  { { AttributeTag(7), "minConstSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16u
  { { AttributeTag(8), "maxConstSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16u
  { { AttributeTag(9), "minConstFlow", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16u
  { { AttributeTag(10), "maxConstFlow", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16u
  { { AttributeTag(11), "minConstTemp", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16s
  { { AttributeTag(12), "maxConstTemp", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16s
  { { AttributeTag(16), "pumpStatus", ItemType::kBitmap }, 704 }, // PumpConfigurationAndControl::PumpStatusBitmap
  { { AttributeTag(17), "effectiveOperationMode", ItemType::kEnum }, 702 }, // PumpConfigurationAndControl::OperationModeEnum
  { { AttributeTag(18), "effectiveControlMode", ItemType::kEnum }, 701 }, // PumpConfigurationAndControl::ControlModeEnum
  { { AttributeTag(19), "capacity", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16s
  { { AttributeTag(20), "speed", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16u
  { { AttributeTag(21), "lifetimeRunningHours", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int24u
  { { AttributeTag(22), "power", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int24u
  { { AttributeTag(23), "lifetimeEnergyConsumed", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int32u
  { { AttributeTag(32), "operationMode", ItemType::kEnum }, 702 }, // PumpConfigurationAndControl::OperationModeEnum
  { { AttributeTag(33), "controlMode", ItemType::kEnum }, 701 }, // PumpConfigurationAndControl::ControlModeEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // PumpConfigurationAndControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // PumpConfigurationAndControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // PumpConfigurationAndControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // PumpConfigurationAndControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 703 }, // PumpConfigurationAndControl::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::int16u
};

const Entry<ItemInfo> _PumpConfigurationAndControl_ControlModeEnum[] = {
  { { ConstantValueTag(0x0), "kConstantSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::ControlModeEnum::kConstantSpeed
  { { ConstantValueTag(0x1), "kConstantPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::ControlModeEnum::kConstantPressure
  { { ConstantValueTag(0x2), "kProportionalPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::ControlModeEnum::kProportionalPressure
  { { ConstantValueTag(0x3), "kConstantFlow", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::ControlModeEnum::kConstantFlow
  { { ConstantValueTag(0x5), "kConstantTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::ControlModeEnum::kConstantTemperature
  { { ConstantValueTag(0x7), "kAutomatic", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::ControlModeEnum::kAutomatic
};

const Entry<ItemInfo> _PumpConfigurationAndControl_OperationModeEnum[] = {
  { { ConstantValueTag(0x0), "kNormal", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::OperationModeEnum::kNormal
  { { ConstantValueTag(0x1), "kMinimum", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::OperationModeEnum::kMinimum
  { { ConstantValueTag(0x2), "kMaximum", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::OperationModeEnum::kMaximum
  { { ConstantValueTag(0x3), "kLocal", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::OperationModeEnum::kLocal
};

const Entry<ItemInfo> _PumpConfigurationAndControl_Feature[] = {
  { { ConstantValueTag(0x1), "kConstantPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::Feature::kConstantPressure
  { { ConstantValueTag(0x2), "kCompensatedPressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::Feature::kCompensatedPressure
  { { ConstantValueTag(0x4), "kConstantFlow", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::Feature::kConstantFlow
  { { ConstantValueTag(0x8), "kConstantSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::Feature::kConstantSpeed
  { { ConstantValueTag(0x10), "kConstantTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::Feature::kConstantTemperature
  { { ConstantValueTag(0x20), "kAutomatic", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::Feature::kAutomatic
  { { ConstantValueTag(0x40), "kLocalOperation", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::Feature::kLocalOperation
};

const Entry<ItemInfo> _PumpConfigurationAndControl_PumpStatusBitmap[] = {
  { { ConstantValueTag(0x1), "kDeviceFault", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kDeviceFault
  { { ConstantValueTag(0x2), "kSupplyFault", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kSupplyFault
  { { ConstantValueTag(0x4), "kSpeedLow", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kSpeedLow
  { { ConstantValueTag(0x8), "kSpeedHigh", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kSpeedHigh
  { { ConstantValueTag(0x10), "kLocalOverride", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kLocalOverride
  { { ConstantValueTag(0x20), "kRunning", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kRunning
  { { ConstantValueTag(0x40), "kRemotePressure", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kRemotePressure
  { { ConstantValueTag(0x80), "kRemoteFlow", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kRemoteFlow
  { { ConstantValueTag(0x100), "kRemoteTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // PumpConfigurationAndControl::PumpStatusBitmap::kRemoteTemperature
};

const Entry<ItemInfo> _Thermostat[] = {
  { { AttributeTag(0), "localTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
  { { AttributeTag(1), "outdoorTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
  { { AttributeTag(2), "occupancy", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::bitmap8
  { { AttributeTag(3), "absMinHeatSetpointLimit", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
  { { AttributeTag(4), "absMaxHeatSetpointLimit", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
  { { AttributeTag(5), "absMinCoolSetpointLimit", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
  { { AttributeTag(6), "absMaxCoolSetpointLimit", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
  { { AttributeTag(7), "PICoolingDemand", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(8), "PIHeatingDemand", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(9), "HVACSystemTypeConfiguration", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::bitmap8
  { { AttributeTag(16), "localTemperatureCalibration", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8s
  { { AttributeTag(17), "occupiedCoolingSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(18), "occupiedHeatingSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(19), "unoccupiedCoolingSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(20), "unoccupiedHeatingSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(21), "minHeatSetpointLimit", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(22), "maxHeatSetpointLimit", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(23), "minCoolSetpointLimit", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(24), "maxCoolSetpointLimit", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(25), "minSetpointDeadBand", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8s
  { { AttributeTag(26), "remoteSensing", ItemType::kBitmap }, 729 }, // Thermostat::RemoteSensingBitmap
  { { AttributeTag(27), "controlSequenceOfOperation", ItemType::kEnum }, 717 }, // Thermostat::ControlSequenceOfOperationEnum
  { { AttributeTag(28), "systemMode", ItemType::kEnum }, 721 }, // Thermostat::SystemModeEnum
  { { AttributeTag(30), "thermostatRunningMode", ItemType::kEnum }, 723 }, // Thermostat::ThermostatRunningModeEnum
  { { AttributeTag(32), "startOfWeek", ItemType::kEnum }, 720 }, // Thermostat::StartOfWeekEnum
  { { AttributeTag(33), "numberOfWeeklyTransitions", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(34), "numberOfDailyTransitions", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(35), "temperatureSetpointHold", ItemType::kEnum }, 722 }, // Thermostat::TemperatureSetpointHoldEnum
  { { AttributeTag(36), "temperatureSetpointHoldDuration", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16u
  { { AttributeTag(37), "thermostatProgrammingOperationMode", ItemType::kBitmap }, 727 }, // Thermostat::ProgrammingOperationModeBitmap
  { { AttributeTag(41), "thermostatRunningState", ItemType::kBitmap }, 728 }, // Thermostat::RelayStateBitmap
  { { AttributeTag(48), "setpointChangeSource", ItemType::kEnum }, 718 }, // Thermostat::SetpointChangeSourceEnum
  { { AttributeTag(49), "setpointChangeAmount", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16s
  { { AttributeTag(50), "setpointChangeSourceTimestamp", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::epoch_s
  { { AttributeTag(52), "occupiedSetback", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(53), "occupiedSetbackMin", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(54), "occupiedSetbackMax", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(55), "unoccupiedSetback", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(56), "unoccupiedSetbackMin", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(57), "unoccupiedSetbackMax", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(58), "emergencyHeatDelta", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { AttributeTag(64), "ACType", ItemType::kEnum }, 716 }, // Thermostat::ACTypeEnum
  { { AttributeTag(65), "ACCapacity", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16u
  { { AttributeTag(66), "ACRefrigerantType", ItemType::kEnum }, 715 }, // Thermostat::ACRefrigerantTypeEnum
  { { AttributeTag(67), "ACCompressorType", ItemType::kEnum }, 713 }, // Thermostat::ACCompressorTypeEnum
  { { AttributeTag(68), "ACErrorCode", ItemType::kBitmap }, 724 }, // Thermostat::ACErrorCodeBitmap
  { { AttributeTag(69), "ACLouverPosition", ItemType::kEnum }, 714 }, // Thermostat::ACLouverPositionEnum
  { { AttributeTag(70), "ACCoilTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
  { { AttributeTag(71), "ACCapacityformat", ItemType::kEnum }, 712 }, // Thermostat::ACCapacityFormatEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Thermostat::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Thermostat::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Thermostat::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Thermostat::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 725 }, // Thermostat::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16u
  { { CommandTag(0), "SetpointRaiseLower", ItemType::kDefault }, 707 }, // Thermostat::SetpointRaiseLower::SetpointRaiseLowerRequest
  { { CommandTag(1), "SetWeeklySchedule", ItemType::kDefault }, 709 }, // Thermostat::SetWeeklySchedule::SetWeeklyScheduleRequest
  { { CommandTag(2), "GetWeeklySchedule", ItemType::kDefault }, 710 }, // Thermostat::GetWeeklySchedule::GetWeeklyScheduleRequest
  { { CommandTag(3), "ClearWeeklySchedule", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ClearWeeklySchedule::()
  { { CommandTag(0), "GetWeeklyScheduleResponse", ItemType::kDefault }, 708 }, // Thermostat::GetWeeklyScheduleResponse
};

const Entry<ItemInfo> _Thermostat_WeeklyScheduleTransitionStruct[] = {
  { { ContextTag(0), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int16u
  { { ContextTag(1), "heatSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
  { { ContextTag(2), "coolSetpoint", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::temperature
};

const Entry<ItemInfo> _Thermostat_SetpointRaiseLowerRequest[] = {
  { { ContextTag(0), "mode", ItemType::kEnum }, 719 }, // Thermostat::SetpointRaiseLowerModeEnum
  { { ContextTag(1), "amount", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8s
};

const Entry<ItemInfo> _Thermostat_GetWeeklyScheduleResponse[] = {
  { { ContextTag(0), "numberOfTransitionsForSequence", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { ContextTag(1), "dayOfWeekForSequence", ItemType::kBitmap }, 730 }, // Thermostat::ScheduleDayOfWeekBitmap
  { { ContextTag(2), "modeForSequence", ItemType::kBitmap }, 731 }, // Thermostat::ScheduleModeBitmap
  { { ContextTag(3), "transitions", ItemType::kList }, 711 }, // Thermostat::WeeklyScheduleTransitionStruct[]
};

const Entry<ItemInfo> _Thermostat_SetWeeklyScheduleRequest[] = {
  { { ContextTag(0), "numberOfTransitionsForSequence", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::int8u
  { { ContextTag(1), "dayOfWeekForSequence", ItemType::kBitmap }, 730 }, // Thermostat::ScheduleDayOfWeekBitmap
  { { ContextTag(2), "modeForSequence", ItemType::kBitmap }, 731 }, // Thermostat::ScheduleModeBitmap
  { { ContextTag(3), "transitions", ItemType::kList }, 711 }, // Thermostat::WeeklyScheduleTransitionStruct[]
};

const Entry<ItemInfo> _Thermostat_GetWeeklyScheduleRequest[] = {
  { { ContextTag(0), "daysToReturn", ItemType::kBitmap }, 730 }, // Thermostat::ScheduleDayOfWeekBitmap
  { { ContextTag(1), "modeToReturn", ItemType::kBitmap }, 731 }, // Thermostat::ScheduleModeBitmap
};

const Entry<ItemInfo> _Thermostat_WeeklyScheduleTransitionStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 706 }, // Thermostat_WeeklyScheduleTransitionStruct[]
};

const Entry<ItemInfo> _Thermostat_ACCapacityFormatEnum[] = {
  { { ConstantValueTag(0x0), "kBTUh", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACCapacityFormatEnum::kBTUh
};

const Entry<ItemInfo> _Thermostat_ACCompressorTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACCompressorTypeEnum::kUnknown
  { { ConstantValueTag(0x1), "kT1", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACCompressorTypeEnum::kT1
  { { ConstantValueTag(0x2), "kT2", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACCompressorTypeEnum::kT2
  { { ConstantValueTag(0x3), "kT3", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACCompressorTypeEnum::kT3
};

const Entry<ItemInfo> _Thermostat_ACLouverPositionEnum[] = {
  { { ConstantValueTag(0x1), "kClosed", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACLouverPositionEnum::kClosed
  { { ConstantValueTag(0x2), "kOpen", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACLouverPositionEnum::kOpen
  { { ConstantValueTag(0x3), "kQuarter", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACLouverPositionEnum::kQuarter
  { { ConstantValueTag(0x4), "kHalf", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACLouverPositionEnum::kHalf
  { { ConstantValueTag(0x5), "kThreeQuarters", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACLouverPositionEnum::kThreeQuarters
};

const Entry<ItemInfo> _Thermostat_ACRefrigerantTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACRefrigerantTypeEnum::kUnknown
  { { ConstantValueTag(0x1), "kR22", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACRefrigerantTypeEnum::kR22
  { { ConstantValueTag(0x2), "kR410a", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACRefrigerantTypeEnum::kR410a
  { { ConstantValueTag(0x3), "kR407c", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACRefrigerantTypeEnum::kR407c
};

const Entry<ItemInfo> _Thermostat_ACTypeEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACTypeEnum::kUnknown
  { { ConstantValueTag(0x1), "kCoolingFixed", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACTypeEnum::kCoolingFixed
  { { ConstantValueTag(0x2), "kHeatPumpFixed", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACTypeEnum::kHeatPumpFixed
  { { ConstantValueTag(0x3), "kCoolingInverter", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACTypeEnum::kCoolingInverter
  { { ConstantValueTag(0x4), "kHeatPumpInverter", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACTypeEnum::kHeatPumpInverter
};

const Entry<ItemInfo> _Thermostat_ControlSequenceOfOperationEnum[] = {
  { { ConstantValueTag(0x0), "kCoolingOnly", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ControlSequenceOfOperationEnum::kCoolingOnly
  { { ConstantValueTag(0x1), "kCoolingWithReheat", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ControlSequenceOfOperationEnum::kCoolingWithReheat
  { { ConstantValueTag(0x2), "kHeatingOnly", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ControlSequenceOfOperationEnum::kHeatingOnly
  { { ConstantValueTag(0x3), "kHeatingWithReheat", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ControlSequenceOfOperationEnum::kHeatingWithReheat
  { { ConstantValueTag(0x4), "kCoolingAndHeating", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ControlSequenceOfOperationEnum::kCoolingAndHeating
  { { ConstantValueTag(0x5), "kCoolingAndHeatingWithReheat", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ControlSequenceOfOperationEnum::kCoolingAndHeatingWithReheat
};

const Entry<ItemInfo> _Thermostat_SetpointChangeSourceEnum[] = {
  { { ConstantValueTag(0x0), "kManual", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SetpointChangeSourceEnum::kManual
  { { ConstantValueTag(0x1), "kSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SetpointChangeSourceEnum::kSchedule
  { { ConstantValueTag(0x2), "kExternal", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SetpointChangeSourceEnum::kExternal
};

const Entry<ItemInfo> _Thermostat_SetpointRaiseLowerModeEnum[] = {
  { { ConstantValueTag(0x0), "kHeat", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SetpointRaiseLowerModeEnum::kHeat
  { { ConstantValueTag(0x1), "kCool", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SetpointRaiseLowerModeEnum::kCool
  { { ConstantValueTag(0x2), "kBoth", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SetpointRaiseLowerModeEnum::kBoth
};

const Entry<ItemInfo> _Thermostat_StartOfWeekEnum[] = {
  { { ConstantValueTag(0x0), "kSunday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::StartOfWeekEnum::kSunday
  { { ConstantValueTag(0x1), "kMonday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::StartOfWeekEnum::kMonday
  { { ConstantValueTag(0x2), "kTuesday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::StartOfWeekEnum::kTuesday
  { { ConstantValueTag(0x3), "kWednesday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::StartOfWeekEnum::kWednesday
  { { ConstantValueTag(0x4), "kThursday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::StartOfWeekEnum::kThursday
  { { ConstantValueTag(0x5), "kFriday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::StartOfWeekEnum::kFriday
  { { ConstantValueTag(0x6), "kSaturday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::StartOfWeekEnum::kSaturday
};

const Entry<ItemInfo> _Thermostat_SystemModeEnum[] = {
  { { ConstantValueTag(0x0), "kOff", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kOff
  { { ConstantValueTag(0x1), "kAuto", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kAuto
  { { ConstantValueTag(0x3), "kCool", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kCool
  { { ConstantValueTag(0x4), "kHeat", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kHeat
  { { ConstantValueTag(0x5), "kEmergencyHeat", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kEmergencyHeat
  { { ConstantValueTag(0x6), "kPrecooling", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kPrecooling
  { { ConstantValueTag(0x7), "kFanOnly", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kFanOnly
  { { ConstantValueTag(0x8), "kDry", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kDry
  { { ConstantValueTag(0x9), "kSleep", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::SystemModeEnum::kSleep
};

const Entry<ItemInfo> _Thermostat_TemperatureSetpointHoldEnum[] = {
  { { ConstantValueTag(0x0), "kSetpointHoldOff", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::TemperatureSetpointHoldEnum::kSetpointHoldOff
  { { ConstantValueTag(0x1), "kSetpointHoldOn", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::TemperatureSetpointHoldEnum::kSetpointHoldOn
};

const Entry<ItemInfo> _Thermostat_ThermostatRunningModeEnum[] = {
  { { ConstantValueTag(0x0), "kOff", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ThermostatRunningModeEnum::kOff
  { { ConstantValueTag(0x3), "kCool", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ThermostatRunningModeEnum::kCool
  { { ConstantValueTag(0x4), "kHeat", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ThermostatRunningModeEnum::kHeat
};

const Entry<ItemInfo> _Thermostat_ACErrorCodeBitmap[] = {
  { { ConstantValueTag(0x1), "kCompressorFail", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACErrorCodeBitmap::kCompressorFail
  { { ConstantValueTag(0x2), "kRoomSensorFail", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACErrorCodeBitmap::kRoomSensorFail
  { { ConstantValueTag(0x4), "kOutdoorSensorFail", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACErrorCodeBitmap::kOutdoorSensorFail
  { { ConstantValueTag(0x8), "kCoilSensorFail", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACErrorCodeBitmap::kCoilSensorFail
  { { ConstantValueTag(0x10), "kFanFail", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ACErrorCodeBitmap::kFanFail
};

const Entry<ItemInfo> _Thermostat_Feature[] = {
  { { ConstantValueTag(0x1), "kHeating", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::Feature::kHeating
  { { ConstantValueTag(0x2), "kCooling", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::Feature::kCooling
  { { ConstantValueTag(0x4), "kOccupancy", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::Feature::kOccupancy
  { { ConstantValueTag(0x8), "kScheduleConfiguration", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::Feature::kScheduleConfiguration
  { { ConstantValueTag(0x10), "kSetback", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::Feature::kSetback
  { { ConstantValueTag(0x20), "kAutoMode", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::Feature::kAutoMode
  { { ConstantValueTag(0x40), "kLocalTemperatureNotExposed", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::Feature::kLocalTemperatureNotExposed
};

const Entry<ItemInfo> _Thermostat_HVACSystemTypeBitmap[] = {
  { { ConstantValueTag(0x3), "kCoolingStage", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::HVACSystemTypeBitmap::kCoolingStage
  { { ConstantValueTag(0xC), "kHeatingStage", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::HVACSystemTypeBitmap::kHeatingStage
  { { ConstantValueTag(0x10), "kHeatingIsHeatPump", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::HVACSystemTypeBitmap::kHeatingIsHeatPump
  { { ConstantValueTag(0x20), "kHeatingUsesFuel", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::HVACSystemTypeBitmap::kHeatingUsesFuel
};

const Entry<ItemInfo> _Thermostat_ProgrammingOperationModeBitmap[] = {
  { { ConstantValueTag(0x1), "kScheduleActive", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ProgrammingOperationModeBitmap::kScheduleActive
  { { ConstantValueTag(0x2), "kAutoRecovery", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ProgrammingOperationModeBitmap::kAutoRecovery
  { { ConstantValueTag(0x4), "kEconomy", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ProgrammingOperationModeBitmap::kEconomy
};

const Entry<ItemInfo> _Thermostat_RelayStateBitmap[] = {
  { { ConstantValueTag(0x1), "kHeat", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RelayStateBitmap::kHeat
  { { ConstantValueTag(0x2), "kCool", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RelayStateBitmap::kCool
  { { ConstantValueTag(0x4), "kFan", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RelayStateBitmap::kFan
  { { ConstantValueTag(0x8), "kHeatStage2", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RelayStateBitmap::kHeatStage2
  { { ConstantValueTag(0x10), "kCoolStage2", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RelayStateBitmap::kCoolStage2
  { { ConstantValueTag(0x20), "kFanStage2", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RelayStateBitmap::kFanStage2
  { { ConstantValueTag(0x40), "kFanStage3", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RelayStateBitmap::kFanStage3
};

const Entry<ItemInfo> _Thermostat_RemoteSensingBitmap[] = {
  { { ConstantValueTag(0x1), "kLocalTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RemoteSensingBitmap::kLocalTemperature
  { { ConstantValueTag(0x2), "kOutdoorTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RemoteSensingBitmap::kOutdoorTemperature
  { { ConstantValueTag(0x4), "kOccupancy", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::RemoteSensingBitmap::kOccupancy
};

const Entry<ItemInfo> _Thermostat_ScheduleDayOfWeekBitmap[] = {
  { { ConstantValueTag(0x1), "kSunday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleDayOfWeekBitmap::kSunday
  { { ConstantValueTag(0x2), "kMonday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleDayOfWeekBitmap::kMonday
  { { ConstantValueTag(0x4), "kTuesday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleDayOfWeekBitmap::kTuesday
  { { ConstantValueTag(0x8), "kWednesday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleDayOfWeekBitmap::kWednesday
  { { ConstantValueTag(0x10), "kThursday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleDayOfWeekBitmap::kThursday
  { { ConstantValueTag(0x20), "kFriday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleDayOfWeekBitmap::kFriday
  { { ConstantValueTag(0x40), "kSaturday", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleDayOfWeekBitmap::kSaturday
  { { ConstantValueTag(0x80), "kAway", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleDayOfWeekBitmap::kAway
};

const Entry<ItemInfo> _Thermostat_ScheduleModeBitmap[] = {
  { { ConstantValueTag(0x1), "kHeatSetpointPresent", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleModeBitmap::kHeatSetpointPresent
  { { ConstantValueTag(0x2), "kCoolSetpointPresent", ItemType::kDefault }, kInvalidNodeIndex }, // Thermostat::ScheduleModeBitmap::kCoolSetpointPresent
};

const Entry<ItemInfo> _FanControl[] = {
  { { AttributeTag(0), "fanMode", ItemType::kEnum }, 735 }, // FanControl::FanModeEnum
  { { AttributeTag(1), "fanModeSequence", ItemType::kEnum }, 736 }, // FanControl::FanModeSequenceEnum
  { { AttributeTag(2), "percentSetting", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::percent
  { { AttributeTag(3), "percentCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::percent
  { { AttributeTag(4), "speedMax", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::int8u
  { { AttributeTag(5), "speedSetting", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::int8u
  { { AttributeTag(6), "speedCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::int8u
  { { AttributeTag(7), "rockSupport", ItemType::kBitmap }, 739 }, // FanControl::RockBitmap
  { { AttributeTag(8), "rockSetting", ItemType::kBitmap }, 739 }, // FanControl::RockBitmap
  { { AttributeTag(9), "windSupport", ItemType::kBitmap }, 740 }, // FanControl::WindBitmap
  { { AttributeTag(10), "windSetting", ItemType::kBitmap }, 740 }, // FanControl::WindBitmap
  { { AttributeTag(11), "airflowDirection", ItemType::kEnum }, 734 }, // FanControl::AirflowDirectionEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // FanControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // FanControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // FanControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // FanControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 738 }, // FanControl::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::int16u
  { { CommandTag(0), "Step", ItemType::kDefault }, 733 }, // FanControl::Step::StepRequest
};

const Entry<ItemInfo> _FanControl_StepRequest[] = {
  { { ContextTag(0), "direction", ItemType::kEnum }, 737 }, // FanControl::StepDirectionEnum
  { { ContextTag(1), "wrap", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::boolean
  { { ContextTag(2), "lowestOff", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::boolean
};

const Entry<ItemInfo> _FanControl_AirflowDirectionEnum[] = {
  { { ConstantValueTag(0x0), "kForward", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::AirflowDirectionEnum::kForward
  { { ConstantValueTag(0x1), "kReverse", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::AirflowDirectionEnum::kReverse
};

const Entry<ItemInfo> _FanControl_FanModeEnum[] = {
  { { ConstantValueTag(0x0), "kOff", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeEnum::kOff
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeEnum::kHigh
  { { ConstantValueTag(0x4), "kOn", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeEnum::kOn
  { { ConstantValueTag(0x5), "kAuto", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeEnum::kAuto
  { { ConstantValueTag(0x6), "kSmart", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeEnum::kSmart
};

const Entry<ItemInfo> _FanControl_FanModeSequenceEnum[] = {
  { { ConstantValueTag(0x0), "kOffLowMedHigh", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeSequenceEnum::kOffLowMedHigh
  { { ConstantValueTag(0x1), "kOffLowHigh", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeSequenceEnum::kOffLowHigh
  { { ConstantValueTag(0x2), "kOffLowMedHighAuto", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeSequenceEnum::kOffLowMedHighAuto
  { { ConstantValueTag(0x3), "kOffLowHighAuto", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeSequenceEnum::kOffLowHighAuto
  { { ConstantValueTag(0x4), "kOffHighAuto", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeSequenceEnum::kOffHighAuto
  { { ConstantValueTag(0x5), "kOffHigh", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::FanModeSequenceEnum::kOffHigh
};

const Entry<ItemInfo> _FanControl_StepDirectionEnum[] = {
  { { ConstantValueTag(0x0), "kIncrease", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::StepDirectionEnum::kIncrease
  { { ConstantValueTag(0x1), "kDecrease", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::StepDirectionEnum::kDecrease
};

const Entry<ItemInfo> _FanControl_Feature[] = {
  { { ConstantValueTag(0x1), "kMultiSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::Feature::kMultiSpeed
  { { ConstantValueTag(0x2), "kAuto", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::Feature::kAuto
  { { ConstantValueTag(0x4), "kRocking", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::Feature::kRocking
  { { ConstantValueTag(0x8), "kWind", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::Feature::kWind
  { { ConstantValueTag(0x10), "kStep", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::Feature::kStep
  { { ConstantValueTag(0x20), "kAirflowDirection", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::Feature::kAirflowDirection
};

const Entry<ItemInfo> _FanControl_RockBitmap[] = {
  { { ConstantValueTag(0x1), "kRockLeftRight", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::RockBitmap::kRockLeftRight
  { { ConstantValueTag(0x2), "kRockUpDown", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::RockBitmap::kRockUpDown
  { { ConstantValueTag(0x4), "kRockRound", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::RockBitmap::kRockRound
};

const Entry<ItemInfo> _FanControl_WindBitmap[] = {
  { { ConstantValueTag(0x1), "kSleepWind", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::WindBitmap::kSleepWind
  { { ConstantValueTag(0x2), "kNaturalWind", ItemType::kDefault }, kInvalidNodeIndex }, // FanControl::WindBitmap::kNaturalWind
};

const Entry<ItemInfo> _ThermostatUserInterfaceConfiguration[] = {
  { { AttributeTag(0), "temperatureDisplayMode", ItemType::kEnum }, 744 }, // ThermostatUserInterfaceConfiguration::TemperatureDisplayModeEnum
  { { AttributeTag(1), "keypadLockout", ItemType::kEnum }, 742 }, // ThermostatUserInterfaceConfiguration::KeypadLockoutEnum
  { { AttributeTag(2), "scheduleProgrammingVisibility", ItemType::kEnum }, 743 }, // ThermostatUserInterfaceConfiguration::ScheduleProgrammingVisibilityEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ThermostatUserInterfaceConfiguration::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ThermostatUserInterfaceConfiguration::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ThermostatUserInterfaceConfiguration::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ThermostatUserInterfaceConfiguration::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::int16u
};

const Entry<ItemInfo> _ThermostatUserInterfaceConfiguration_KeypadLockoutEnum[] = {
  { { ConstantValueTag(0x0), "kNoLockout", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::KeypadLockoutEnum::kNoLockout
  { { ConstantValueTag(0x1), "kLockout1", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::KeypadLockoutEnum::kLockout1
  { { ConstantValueTag(0x2), "kLockout2", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::KeypadLockoutEnum::kLockout2
  { { ConstantValueTag(0x3), "kLockout3", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::KeypadLockoutEnum::kLockout3
  { { ConstantValueTag(0x4), "kLockout4", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::KeypadLockoutEnum::kLockout4
  { { ConstantValueTag(0x5), "kLockout5", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::KeypadLockoutEnum::kLockout5
};

const Entry<ItemInfo> _ThermostatUserInterfaceConfiguration_ScheduleProgrammingVisibilityEnum[] = {
  { { ConstantValueTag(0x0), "kScheduleProgrammingPermitted", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::ScheduleProgrammingVisibilityEnum::kScheduleProgrammingPermitted
  { { ConstantValueTag(0x1), "kScheduleProgrammingDenied", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::ScheduleProgrammingVisibilityEnum::kScheduleProgrammingDenied
};

const Entry<ItemInfo> _ThermostatUserInterfaceConfiguration_TemperatureDisplayModeEnum[] = {
  { { ConstantValueTag(0x0), "kCelsius", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::TemperatureDisplayModeEnum::kCelsius
  { { ConstantValueTag(0x1), "kFahrenheit", ItemType::kDefault }, kInvalidNodeIndex }, // ThermostatUserInterfaceConfiguration::TemperatureDisplayModeEnum::kFahrenheit
};

const Entry<ItemInfo> _ColorControl[] = {
  { { AttributeTag(0), "currentHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(1), "currentSaturation", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(2), "remainingTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(3), "currentX", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(4), "currentY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(5), "driftCompensation", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::enum8
  { { AttributeTag(6), "compensationText", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::char_string
  { { AttributeTag(7), "colorTemperatureMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(8), "colorMode", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::enum8
  { { AttributeTag(15), "options", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { AttributeTag(16), "numberOfPrimaries", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(17), "primary1X", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(18), "primary1Y", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(19), "primary1Intensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(21), "primary2X", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(22), "primary2Y", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(23), "primary2Intensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(25), "primary3X", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(26), "primary3Y", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(27), "primary3Intensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(32), "primary4X", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(33), "primary4Y", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(34), "primary4Intensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(36), "primary5X", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(37), "primary5Y", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(38), "primary5Intensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(40), "primary6X", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(41), "primary6Y", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(42), "primary6Intensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(48), "whitePointX", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(49), "whitePointY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(50), "colorPointRX", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(51), "colorPointRY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(52), "colorPointRIntensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(54), "colorPointGX", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(55), "colorPointGY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(56), "colorPointGIntensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(58), "colorPointBX", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(59), "colorPointBY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(60), "colorPointBIntensity", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(16384), "enhancedCurrentHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(16385), "enhancedColorMode", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::enum8
  { { AttributeTag(16386), "colorLoopActive", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(16387), "colorLoopDirection", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { AttributeTag(16388), "colorLoopTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(16389), "colorLoopStartEnhancedHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(16390), "colorLoopStoredEnhancedHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(16394), "colorCapabilities", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap16
  { { AttributeTag(16395), "colorTempPhysicalMinMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(16396), "colorTempPhysicalMaxMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(16397), "coupleColorTempToLevelMinMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(16400), "startUpColorTemperatureMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ColorControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ColorControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ColorControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ColorControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 775 }, // ColorControl::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { CommandTag(0), "MoveToHue", ItemType::kDefault }, 746 }, // ColorControl::MoveToHue::MoveToHueRequest
  { { CommandTag(1), "MoveHue", ItemType::kDefault }, 747 }, // ColorControl::MoveHue::MoveHueRequest
  { { CommandTag(2), "StepHue", ItemType::kDefault }, 748 }, // ColorControl::StepHue::StepHueRequest
  { { CommandTag(3), "MoveToSaturation", ItemType::kDefault }, 749 }, // ColorControl::MoveToSaturation::MoveToSaturationRequest
  { { CommandTag(4), "MoveSaturation", ItemType::kDefault }, 750 }, // ColorControl::MoveSaturation::MoveSaturationRequest
  { { CommandTag(5), "StepSaturation", ItemType::kDefault }, 751 }, // ColorControl::StepSaturation::StepSaturationRequest
  { { CommandTag(6), "MoveToHueAndSaturation", ItemType::kDefault }, 752 }, // ColorControl::MoveToHueAndSaturation::MoveToHueAndSaturationRequest
  { { CommandTag(7), "MoveToColor", ItemType::kDefault }, 753 }, // ColorControl::MoveToColor::MoveToColorRequest
  { { CommandTag(8), "MoveColor", ItemType::kDefault }, 754 }, // ColorControl::MoveColor::MoveColorRequest
  { { CommandTag(9), "StepColor", ItemType::kDefault }, 755 }, // ColorControl::StepColor::StepColorRequest
  { { CommandTag(10), "MoveToColorTemperature", ItemType::kDefault }, 756 }, // ColorControl::MoveToColorTemperature::MoveToColorTemperatureRequest
  { { CommandTag(64), "EnhancedMoveToHue", ItemType::kDefault }, 757 }, // ColorControl::EnhancedMoveToHue::EnhancedMoveToHueRequest
  { { CommandTag(65), "EnhancedMoveHue", ItemType::kDefault }, 758 }, // ColorControl::EnhancedMoveHue::EnhancedMoveHueRequest
  { { CommandTag(66), "EnhancedStepHue", ItemType::kDefault }, 759 }, // ColorControl::EnhancedStepHue::EnhancedStepHueRequest
  { { CommandTag(67), "EnhancedMoveToHueAndSaturation", ItemType::kDefault }, 760 }, // ColorControl::EnhancedMoveToHueAndSaturation::EnhancedMoveToHueAndSaturationRequest
  { { CommandTag(68), "ColorLoopSet", ItemType::kDefault }, 761 }, // ColorControl::ColorLoopSet::ColorLoopSetRequest
  { { CommandTag(71), "StopMoveStep", ItemType::kDefault }, 762 }, // ColorControl::StopMoveStep::StopMoveStepRequest
  { { CommandTag(75), "MoveColorTemperature", ItemType::kDefault }, 763 }, // ColorControl::MoveColorTemperature::MoveColorTemperatureRequest
  { { CommandTag(76), "StepColorTemperature", ItemType::kDefault }, 764 }, // ColorControl::StepColorTemperature::StepColorTemperatureRequest
};

const Entry<ItemInfo> _ColorControl_MoveToHueRequest[] = {
  { { ContextTag(0), "hue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(1), "direction", ItemType::kEnum }, 768 }, // ColorControl::HueDirection
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_MoveHueRequest[] = {
  { { ContextTag(0), "moveMode", ItemType::kEnum }, 769 }, // ColorControl::HueMoveMode
  { { ContextTag(1), "rate", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(2), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(3), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_StepHueRequest[] = {
  { { ContextTag(0), "stepMode", ItemType::kEnum }, 770 }, // ColorControl::HueStepMode
  { { ContextTag(1), "stepSize", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_MoveToSaturationRequest[] = {
  { { ContextTag(0), "saturation", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(1), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(2), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(3), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_MoveSaturationRequest[] = {
  { { ContextTag(0), "moveMode", ItemType::kEnum }, 771 }, // ColorControl::SaturationMoveMode
  { { ContextTag(1), "rate", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(2), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(3), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_StepSaturationRequest[] = {
  { { ContextTag(0), "stepMode", ItemType::kEnum }, 772 }, // ColorControl::SaturationStepMode
  { { ContextTag(1), "stepSize", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_MoveToHueAndSaturationRequest[] = {
  { { ContextTag(0), "hue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(1), "saturation", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_MoveToColorRequest[] = {
  { { ContextTag(0), "colorX", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(1), "colorY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_MoveColorRequest[] = {
  { { ContextTag(0), "rateX", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16s
  { { ContextTag(1), "rateY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16s
  { { ContextTag(2), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(3), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_StepColorRequest[] = {
  { { ContextTag(0), "stepX", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16s
  { { ContextTag(1), "stepY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16s
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_MoveToColorTemperatureRequest[] = {
  { { ContextTag(0), "colorTemperatureMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(1), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(2), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(3), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_EnhancedMoveToHueRequest[] = {
  { { ContextTag(0), "enhancedHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(1), "direction", ItemType::kEnum }, 768 }, // ColorControl::HueDirection
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_EnhancedMoveHueRequest[] = {
  { { ContextTag(0), "moveMode", ItemType::kEnum }, 769 }, // ColorControl::HueMoveMode
  { { ContextTag(1), "rate", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(2), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(3), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_EnhancedStepHueRequest[] = {
  { { ContextTag(0), "stepMode", ItemType::kEnum }, 770 }, // ColorControl::HueStepMode
  { { ContextTag(1), "stepSize", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_EnhancedMoveToHueAndSaturationRequest[] = {
  { { ContextTag(0), "enhancedHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(1), "saturation", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int8u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(4), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_ColorLoopSetRequest[] = {
  { { ContextTag(0), "updateFlags", ItemType::kBitmap }, 774 }, // ColorControl::ColorLoopUpdateFlags
  { { ContextTag(1), "action", ItemType::kEnum }, 765 }, // ColorControl::ColorLoopAction
  { { ContextTag(2), "direction", ItemType::kEnum }, 766 }, // ColorControl::ColorLoopDirection
  { { ContextTag(3), "time", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(4), "startHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(5), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(6), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_StopMoveStepRequest[] = {
  { { ContextTag(0), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(1), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_MoveColorTemperatureRequest[] = {
  { { ContextTag(0), "moveMode", ItemType::kEnum }, 769 }, // ColorControl::HueMoveMode
  { { ContextTag(1), "rate", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(2), "colorTemperatureMinimumMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "colorTemperatureMaximumMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(4), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(5), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_StepColorTemperatureRequest[] = {
  { { ContextTag(0), "stepMode", ItemType::kEnum }, 770 }, // ColorControl::HueStepMode
  { { ContextTag(1), "stepSize", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(2), "transitionTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(3), "colorTemperatureMinimumMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(4), "colorTemperatureMaximumMireds", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::int16u
  { { ContextTag(5), "optionsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
  { { ContextTag(6), "optionsOverride", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::bitmap8
};

const Entry<ItemInfo> _ColorControl_ColorLoopAction[] = {
  { { ConstantValueTag(0x0), "kDeactivate", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopAction::kDeactivate
  { { ConstantValueTag(0x1), "kActivateFromColorLoopStartEnhancedHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopAction::kActivateFromColorLoopStartEnhancedHue
  { { ConstantValueTag(0x2), "kActivateFromEnhancedCurrentHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopAction::kActivateFromEnhancedCurrentHue
};

const Entry<ItemInfo> _ColorControl_ColorLoopDirection[] = {
  { { ConstantValueTag(0x0), "kDecrementHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopDirection::kDecrementHue
  { { ConstantValueTag(0x1), "kIncrementHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopDirection::kIncrementHue
};

const Entry<ItemInfo> _ColorControl_ColorMode[] = {
  { { ConstantValueTag(0x0), "kCurrentHueAndCurrentSaturation", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorMode::kCurrentHueAndCurrentSaturation
  { { ConstantValueTag(0x1), "kCurrentXAndCurrentY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorMode::kCurrentXAndCurrentY
  { { ConstantValueTag(0x2), "kColorTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorMode::kColorTemperature
};

const Entry<ItemInfo> _ColorControl_HueDirection[] = {
  { { ConstantValueTag(0x0), "kShortestDistance", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueDirection::kShortestDistance
  { { ConstantValueTag(0x1), "kLongestDistance", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueDirection::kLongestDistance
  { { ConstantValueTag(0x2), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueDirection::kUp
  { { ConstantValueTag(0x3), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueDirection::kDown
};

const Entry<ItemInfo> _ColorControl_HueMoveMode[] = {
  { { ConstantValueTag(0x0), "kStop", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueMoveMode::kStop
  { { ConstantValueTag(0x1), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueMoveMode::kUp
  { { ConstantValueTag(0x3), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueMoveMode::kDown
};

const Entry<ItemInfo> _ColorControl_HueStepMode[] = {
  { { ConstantValueTag(0x1), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueStepMode::kUp
  { { ConstantValueTag(0x3), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::HueStepMode::kDown
};

const Entry<ItemInfo> _ColorControl_SaturationMoveMode[] = {
  { { ConstantValueTag(0x0), "kStop", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::SaturationMoveMode::kStop
  { { ConstantValueTag(0x1), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::SaturationMoveMode::kUp
  { { ConstantValueTag(0x3), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::SaturationMoveMode::kDown
};

const Entry<ItemInfo> _ColorControl_SaturationStepMode[] = {
  { { ConstantValueTag(0x1), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::SaturationStepMode::kUp
  { { ConstantValueTag(0x3), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::SaturationStepMode::kDown
};

const Entry<ItemInfo> _ColorControl_ColorCapabilities[] = {
  { { ConstantValueTag(0x1), "kHueSaturationSupported", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorCapabilities::kHueSaturationSupported
  { { ConstantValueTag(0x2), "kEnhancedHueSupported", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorCapabilities::kEnhancedHueSupported
  { { ConstantValueTag(0x4), "kColorLoopSupported", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorCapabilities::kColorLoopSupported
  { { ConstantValueTag(0x8), "kXYAttributesSupported", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorCapabilities::kXYAttributesSupported
  { { ConstantValueTag(0x10), "kColorTemperatureSupported", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorCapabilities::kColorTemperatureSupported
};

const Entry<ItemInfo> _ColorControl_ColorLoopUpdateFlags[] = {
  { { ConstantValueTag(0x1), "kUpdateAction", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopUpdateFlags::kUpdateAction
  { { ConstantValueTag(0x2), "kUpdateDirection", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopUpdateFlags::kUpdateDirection
  { { ConstantValueTag(0x4), "kUpdateTime", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopUpdateFlags::kUpdateTime
  { { ConstantValueTag(0x8), "kUpdateStartHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::ColorLoopUpdateFlags::kUpdateStartHue
};

const Entry<ItemInfo> _ColorControl_Feature[] = {
  { { ConstantValueTag(0x1), "kHueAndSaturation", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::Feature::kHueAndSaturation
  { { ConstantValueTag(0x2), "kEnhancedHue", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::Feature::kEnhancedHue
  { { ConstantValueTag(0x4), "kColorLoop", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::Feature::kColorLoop
  { { ConstantValueTag(0x8), "kXY", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::Feature::kXY
  { { ConstantValueTag(0x10), "kColorTemperature", ItemType::kDefault }, kInvalidNodeIndex }, // ColorControl::Feature::kColorTemperature
};

const Entry<ItemInfo> _BallastConfiguration[] = {
  { { AttributeTag(0), "physicalMinLevel", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int8u
  { { AttributeTag(1), "physicalMaxLevel", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int8u
  { { AttributeTag(2), "ballastStatus", ItemType::kBitmap }, 777 }, // BallastConfiguration::BallastStatusBitmap
  { { AttributeTag(16), "minLevel", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int8u
  { { AttributeTag(17), "maxLevel", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int8u
  { { AttributeTag(20), "intrinsicBallastFactor", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int8u
  { { AttributeTag(21), "ballastFactorAdjustment", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int8u
  { { AttributeTag(32), "lampQuantity", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int8u
  { { AttributeTag(48), "lampType", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::char_string
  { { AttributeTag(49), "lampManufacturer", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::char_string
  { { AttributeTag(50), "lampRatedHours", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int24u
  { { AttributeTag(51), "lampBurnHours", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int24u
  { { AttributeTag(52), "lampAlarmMode", ItemType::kBitmap }, 778 }, // BallastConfiguration::LampAlarmModeBitmap
  { { AttributeTag(53), "lampBurnHoursTripPoint", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int24u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // BallastConfiguration::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // BallastConfiguration::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // BallastConfiguration::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // BallastConfiguration::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::int16u
};

const Entry<ItemInfo> _BallastConfiguration_BallastStatusBitmap[] = {
  { { ConstantValueTag(0x1), "kBallastNonOperational", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::BallastStatusBitmap::kBallastNonOperational
  { { ConstantValueTag(0x2), "kLampFailure", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::BallastStatusBitmap::kLampFailure
};

const Entry<ItemInfo> _BallastConfiguration_LampAlarmModeBitmap[] = {
  { { ConstantValueTag(0x1), "kLampBurnHours", ItemType::kDefault }, kInvalidNodeIndex }, // BallastConfiguration::LampAlarmModeBitmap::kLampBurnHours
};

const Entry<ItemInfo> _IlluminanceMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // IlluminanceMeasurement::int16u
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // IlluminanceMeasurement::int16u
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // IlluminanceMeasurement::int16u
  { { AttributeTag(3), "tolerance", ItemType::kDefault }, kInvalidNodeIndex }, // IlluminanceMeasurement::int16u
  { { AttributeTag(4), "lightSensorType", ItemType::kEnum }, 780 }, // IlluminanceMeasurement::LightSensorTypeEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // IlluminanceMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // IlluminanceMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // IlluminanceMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // IlluminanceMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // IlluminanceMeasurement::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // IlluminanceMeasurement::int16u
};

const Entry<ItemInfo> _IlluminanceMeasurement_LightSensorTypeEnum[] = {
  { { ConstantValueTag(0x0), "kPhotodiode", ItemType::kDefault }, kInvalidNodeIndex }, // IlluminanceMeasurement::LightSensorTypeEnum::kPhotodiode
  { { ConstantValueTag(0x1), "kCMOS", ItemType::kDefault }, kInvalidNodeIndex }, // IlluminanceMeasurement::LightSensorTypeEnum::kCMOS
};

const Entry<ItemInfo> _TemperatureMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureMeasurement::temperature
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureMeasurement::temperature
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureMeasurement::temperature
  { { AttributeTag(3), "tolerance", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureMeasurement::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // TemperatureMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // TemperatureMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // TemperatureMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // TemperatureMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureMeasurement::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // TemperatureMeasurement::int16u
};

const Entry<ItemInfo> _PressureMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16s
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16s
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16s
  { { AttributeTag(3), "tolerance", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16u
  { { AttributeTag(16), "scaledValue", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16s
  { { AttributeTag(17), "minScaledValue", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16s
  { { AttributeTag(18), "maxScaledValue", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16s
  { { AttributeTag(19), "scaledTolerance", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16u
  { { AttributeTag(20), "scale", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int8s
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // PressureMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // PressureMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // PressureMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // PressureMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 783 }, // PressureMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::int16u
};

const Entry<ItemInfo> _PressureMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kExtended", ItemType::kDefault }, kInvalidNodeIndex }, // PressureMeasurement::Feature::kExtended
};

const Entry<ItemInfo> _FlowMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // FlowMeasurement::int16u
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // FlowMeasurement::int16u
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // FlowMeasurement::int16u
  { { AttributeTag(3), "tolerance", ItemType::kDefault }, kInvalidNodeIndex }, // FlowMeasurement::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // FlowMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // FlowMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // FlowMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // FlowMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // FlowMeasurement::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // FlowMeasurement::int16u
};

const Entry<ItemInfo> _RelativeHumidityMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // RelativeHumidityMeasurement::int16u
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // RelativeHumidityMeasurement::int16u
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // RelativeHumidityMeasurement::int16u
  { { AttributeTag(3), "tolerance", ItemType::kDefault }, kInvalidNodeIndex }, // RelativeHumidityMeasurement::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // RelativeHumidityMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // RelativeHumidityMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // RelativeHumidityMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // RelativeHumidityMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // RelativeHumidityMeasurement::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // RelativeHumidityMeasurement::int16u
};

const Entry<ItemInfo> _OccupancySensing[] = {
  { { AttributeTag(0), "occupancy", ItemType::kBitmap }, 788 }, // OccupancySensing::OccupancyBitmap
  { { AttributeTag(1), "occupancySensorType", ItemType::kEnum }, 787 }, // OccupancySensing::OccupancySensorTypeEnum
  { { AttributeTag(2), "occupancySensorTypeBitmap", ItemType::kBitmap }, 789 }, // OccupancySensing::OccupancySensorTypeBitmap
  { { AttributeTag(16), "PIROccupiedToUnoccupiedDelay", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int16u
  { { AttributeTag(17), "PIRUnoccupiedToOccupiedDelay", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int16u
  { { AttributeTag(18), "PIRUnoccupiedToOccupiedThreshold", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int8u
  { { AttributeTag(32), "ultrasonicOccupiedToUnoccupiedDelay", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int16u
  { { AttributeTag(33), "ultrasonicUnoccupiedToOccupiedDelay", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int16u
  { { AttributeTag(34), "ultrasonicUnoccupiedToOccupiedThreshold", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int8u
  { { AttributeTag(48), "physicalContactOccupiedToUnoccupiedDelay", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int16u
  { { AttributeTag(49), "physicalContactUnoccupiedToOccupiedDelay", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int16u
  { { AttributeTag(50), "physicalContactUnoccupiedToOccupiedThreshold", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OccupancySensing::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OccupancySensing::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OccupancySensing::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OccupancySensing::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::int16u
};

const Entry<ItemInfo> _OccupancySensing_OccupancySensorTypeEnum[] = {
  { { ConstantValueTag(0x0), "kPIR", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::OccupancySensorTypeEnum::kPIR
  { { ConstantValueTag(0x1), "kUltrasonic", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::OccupancySensorTypeEnum::kUltrasonic
  { { ConstantValueTag(0x2), "kPIRAndUltrasonic", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::OccupancySensorTypeEnum::kPIRAndUltrasonic
  { { ConstantValueTag(0x3), "kPhysicalContact", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::OccupancySensorTypeEnum::kPhysicalContact
};

const Entry<ItemInfo> _OccupancySensing_OccupancyBitmap[] = {
  { { ConstantValueTag(0x1), "kOccupied", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::OccupancyBitmap::kOccupied
};

const Entry<ItemInfo> _OccupancySensing_OccupancySensorTypeBitmap[] = {
  { { ConstantValueTag(0x1), "kPIR", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::OccupancySensorTypeBitmap::kPIR
  { { ConstantValueTag(0x2), "kUltrasonic", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::OccupancySensorTypeBitmap::kUltrasonic
  { { ConstantValueTag(0x4), "kPhysicalContact", ItemType::kDefault }, kInvalidNodeIndex }, // OccupancySensing::OccupancySensorTypeBitmap::kPhysicalContact
};

const Entry<ItemInfo> _CarbonMonoxideConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 793 }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 792 }, // CarbonMonoxideConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 791 }, // CarbonMonoxideConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // CarbonMonoxideConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // CarbonMonoxideConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // CarbonMonoxideConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // CarbonMonoxideConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 794 }, // CarbonMonoxideConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _CarbonMonoxideConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _CarbonMonoxideConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _CarbonMonoxideConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _CarbonMonoxideConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonMonoxideConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _CarbonDioxideConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 798 }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 797 }, // CarbonDioxideConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 796 }, // CarbonDioxideConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // CarbonDioxideConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // CarbonDioxideConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // CarbonDioxideConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // CarbonDioxideConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 799 }, // CarbonDioxideConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _CarbonDioxideConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _CarbonDioxideConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _CarbonDioxideConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _CarbonDioxideConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // CarbonDioxideConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _NitrogenDioxideConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 803 }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 802 }, // NitrogenDioxideConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 801 }, // NitrogenDioxideConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // NitrogenDioxideConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // NitrogenDioxideConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // NitrogenDioxideConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // NitrogenDioxideConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 804 }, // NitrogenDioxideConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _NitrogenDioxideConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _NitrogenDioxideConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _NitrogenDioxideConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _NitrogenDioxideConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // NitrogenDioxideConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _OzoneConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 808 }, // OzoneConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 807 }, // OzoneConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 806 }, // OzoneConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // OzoneConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // OzoneConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // OzoneConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // OzoneConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 809 }, // OzoneConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _OzoneConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _OzoneConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _OzoneConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _OzoneConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // OzoneConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _Pm25ConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 813 }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 812 }, // Pm25ConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 811 }, // Pm25ConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Pm25ConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Pm25ConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Pm25ConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Pm25ConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 814 }, // Pm25ConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _Pm25ConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _Pm25ConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _Pm25ConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _Pm25ConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm25ConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _FormaldehydeConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 818 }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 817 }, // FormaldehydeConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 816 }, // FormaldehydeConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // FormaldehydeConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // FormaldehydeConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // FormaldehydeConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // FormaldehydeConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 819 }, // FormaldehydeConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _FormaldehydeConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _FormaldehydeConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _FormaldehydeConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _FormaldehydeConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // FormaldehydeConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _Pm1ConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 823 }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 822 }, // Pm1ConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 821 }, // Pm1ConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Pm1ConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Pm1ConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Pm1ConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Pm1ConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 824 }, // Pm1ConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _Pm1ConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _Pm1ConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _Pm1ConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _Pm1ConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm1ConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _Pm10ConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 828 }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 827 }, // Pm10ConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 826 }, // Pm10ConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Pm10ConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Pm10ConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Pm10ConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Pm10ConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 829 }, // Pm10ConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _Pm10ConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _Pm10ConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _Pm10ConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _Pm10ConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // Pm10ConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _TotalVolatileOrganicCompoundsConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 833 }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 832 }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 831 }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 834 }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _TotalVolatileOrganicCompoundsConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _TotalVolatileOrganicCompoundsConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _TotalVolatileOrganicCompoundsConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _TotalVolatileOrganicCompoundsConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // TotalVolatileOrganicCompoundsConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _RadonConcentrationMeasurement[] = {
  { { AttributeTag(0), "measuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::single
  { { AttributeTag(1), "minMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::single
  { { AttributeTag(2), "maxMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::single
  { { AttributeTag(3), "peakMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::single
  { { AttributeTag(4), "peakMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::elapsed_s
  { { AttributeTag(5), "averageMeasuredValue", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::single
  { { AttributeTag(6), "averageMeasuredValueWindow", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::elapsed_s
  { { AttributeTag(7), "uncertainty", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::single
  { { AttributeTag(8), "measurementUnit", ItemType::kEnum }, 838 }, // RadonConcentrationMeasurement::MeasurementUnitEnum
  { { AttributeTag(9), "measurementMedium", ItemType::kEnum }, 837 }, // RadonConcentrationMeasurement::MeasurementMediumEnum
  { { AttributeTag(10), "levelValue", ItemType::kEnum }, 836 }, // RadonConcentrationMeasurement::LevelValueEnum
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // RadonConcentrationMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // RadonConcentrationMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // RadonConcentrationMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // RadonConcentrationMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 839 }, // RadonConcentrationMeasurement::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::int16u
};

const Entry<ItemInfo> _RadonConcentrationMeasurement_LevelValueEnum[] = {
  { { ConstantValueTag(0x0), "kUnknown", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::LevelValueEnum::kUnknown
  { { ConstantValueTag(0x1), "kLow", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::LevelValueEnum::kLow
  { { ConstantValueTag(0x2), "kMedium", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::LevelValueEnum::kMedium
  { { ConstantValueTag(0x3), "kHigh", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::LevelValueEnum::kHigh
  { { ConstantValueTag(0x4), "kCritical", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::LevelValueEnum::kCritical
};

const Entry<ItemInfo> _RadonConcentrationMeasurement_MeasurementMediumEnum[] = {
  { { ConstantValueTag(0x0), "kAir", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementMediumEnum::kAir
  { { ConstantValueTag(0x1), "kWater", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementMediumEnum::kWater
  { { ConstantValueTag(0x2), "kSoil", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementMediumEnum::kSoil
};

const Entry<ItemInfo> _RadonConcentrationMeasurement_MeasurementUnitEnum[] = {
  { { ConstantValueTag(0x0), "kPPM", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementUnitEnum::kPPM
  { { ConstantValueTag(0x1), "kPPB", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementUnitEnum::kPPB
  { { ConstantValueTag(0x2), "kPPT", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementUnitEnum::kPPT
  { { ConstantValueTag(0x3), "kMGM3", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementUnitEnum::kMGM3
  { { ConstantValueTag(0x4), "kUGM3", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementUnitEnum::kUGM3
  { { ConstantValueTag(0x5), "kNGM3", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementUnitEnum::kNGM3
  { { ConstantValueTag(0x6), "kPM3", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementUnitEnum::kPM3
  { { ConstantValueTag(0x7), "kBQM3", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::MeasurementUnitEnum::kBQM3
};

const Entry<ItemInfo> _RadonConcentrationMeasurement_Feature[] = {
  { { ConstantValueTag(0x1), "kNumericMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::Feature::kNumericMeasurement
  { { ConstantValueTag(0x2), "kLevelIndication", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::Feature::kLevelIndication
  { { ConstantValueTag(0x4), "kMediumLevel", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::Feature::kMediumLevel
  { { ConstantValueTag(0x8), "kCriticalLevel", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::Feature::kCriticalLevel
  { { ConstantValueTag(0x10), "kPeakMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::Feature::kPeakMeasurement
  { { ConstantValueTag(0x20), "kAverageMeasurement", ItemType::kDefault }, kInvalidNodeIndex }, // RadonConcentrationMeasurement::Feature::kAverageMeasurement
};

const Entry<ItemInfo> _WakeOnLan[] = {
  { { AttributeTag(0), "MACAddress", ItemType::kDefault }, kInvalidNodeIndex }, // WakeOnLan::char_string
  { { AttributeTag(1), "linkLocalAddress", ItemType::kDefault }, kInvalidNodeIndex }, // WakeOnLan::octet_string
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // WakeOnLan::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // WakeOnLan::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // WakeOnLan::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // WakeOnLan::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // WakeOnLan::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // WakeOnLan::int16u
};

const Entry<ItemInfo> _Channel[] = {
  { { AttributeTag(0), "channelList", ItemType::kList }, 861 }, // Channel::ChannelInfoStruct[]
  { { AttributeTag(1), "lineup", ItemType::kDefault }, 850 }, // Channel::LineupInfoStruct
  { { AttributeTag(2), "currentChannel", ItemType::kDefault }, 845 }, // Channel::ChannelInfoStruct
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // Channel::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // Channel::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // Channel::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // Channel::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 867 }, // Channel::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::int16u
  { { CommandTag(0), "ChangeChannel", ItemType::kDefault }, 851 }, // Channel::ChangeChannel::ChangeChannelRequest
  { { CommandTag(2), "ChangeChannelByNumber", ItemType::kDefault }, 853 }, // Channel::ChangeChannelByNumber::ChangeChannelByNumberRequest
  { { CommandTag(3), "SkipChannel", ItemType::kDefault }, 854 }, // Channel::SkipChannel::SkipChannelRequest
  { { CommandTag(4), "GetProgramGuide", ItemType::kDefault }, 855 }, // Channel::GetProgramGuide::GetProgramGuideRequest
  { { CommandTag(6), "RecordProgram", ItemType::kDefault }, 857 }, // Channel::RecordProgram::RecordProgramRequest
  { { CommandTag(7), "CancelRecordProgram", ItemType::kDefault }, 858 }, // Channel::CancelRecordProgram::CancelRecordProgramRequest
  { { CommandTag(1), "ChangeChannelResponse", ItemType::kDefault }, 852 }, // Channel::ChangeChannelResponse
  { { CommandTag(5), "ProgramGuideResponse", ItemType::kDefault }, 856 }, // Channel::ProgramGuideResponse
};

const Entry<ItemInfo> _Channel_ProgramCastStruct[] = {
  { { ContextTag(0), "name", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(1), "role", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
};

const Entry<ItemInfo> _Channel_ProgramCategoryStruct[] = {
  { { ContextTag(0), "category", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(1), "subCategory", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
};

const Entry<ItemInfo> _Channel_SeriesInfoStruct[] = {
  { { ContextTag(0), "season", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(1), "episode", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
};

const Entry<ItemInfo> _Channel_ChannelInfoStruct[] = {
  { { ContextTag(0), "majorNumber", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::int16u
  { { ContextTag(1), "minorNumber", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::int16u
  { { ContextTag(2), "name", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(3), "callSign", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(4), "affiliateCallSign", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(5), "identifier", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(6), "type", ItemType::kEnum }, 864 }, // Channel::ChannelTypeEnum
};

const Entry<ItemInfo> _Channel_ProgramStruct[] = {
  { { ContextTag(0), "identifier", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(1), "channel", ItemType::kDefault }, 845 }, // Channel::ChannelInfoStruct
  { { ContextTag(2), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::epoch_s
  { { ContextTag(3), "endTime", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::epoch_s
  { { ContextTag(4), "title", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(5), "subtitle", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(6), "description", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(7), "audioLanguages", ItemType::kList }, 1 }, // Channel::char_string[]
  { { ContextTag(8), "ratings", ItemType::kList }, 1 }, // Channel::char_string[]
  { { ContextTag(9), "thumbnailUrl", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(10), "posterArtUrl", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(11), "dvbiUrl", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(12), "releaseDate", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(13), "parentalGuidanceText", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(14), "recordingFlag", ItemType::kBitmap }, 868 }, // Channel::RecordingFlagBitmap
  { { ContextTag(15), "seriesInfo", ItemType::kDefault }, 844 }, // Channel::SeriesInfoStruct
  { { ContextTag(16), "categoryList", ItemType::kList }, 860 }, // Channel::ProgramCategoryStruct[]
  { { ContextTag(17), "castList", ItemType::kList }, 862 }, // Channel::ProgramCastStruct[]
  { { ContextTag(18), "externalIDList", ItemType::kList }, 862 }, // Channel::ProgramCastStruct[]
};

const Entry<ItemInfo> _Channel_PageTokenStruct[] = {
  { { ContextTag(0), "limit", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::int16u
  { { ContextTag(1), "after", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(2), "before", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
};

const Entry<ItemInfo> _Channel_ChannelPagingStruct[] = {
  { { ContextTag(0), "previousToken", ItemType::kDefault }, 847 }, // Channel::PageTokenStruct
  { { ContextTag(1), "nextToken", ItemType::kDefault }, 847 }, // Channel::PageTokenStruct
};

const Entry<ItemInfo> _Channel_AdditionalInfoStruct[] = {
  { { ContextTag(0), "name", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
};

const Entry<ItemInfo> _Channel_LineupInfoStruct[] = {
  { { ContextTag(0), "operatorName", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(1), "lineupName", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(2), "postalCode", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(3), "lineupInfoType", ItemType::kEnum }, 865 }, // Channel::LineupInfoTypeEnum
};

const Entry<ItemInfo> _Channel_ChangeChannelRequest[] = {
  { { ContextTag(0), "match", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
};

const Entry<ItemInfo> _Channel_ChangeChannelResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 866 }, // Channel::StatusEnum
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
};

const Entry<ItemInfo> _Channel_ChangeChannelByNumberRequest[] = {
  { { ContextTag(0), "majorNumber", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::int16u
  { { ContextTag(1), "minorNumber", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::int16u
};

const Entry<ItemInfo> _Channel_SkipChannelRequest[] = {
  { { ContextTag(0), "count", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::int16s
};

const Entry<ItemInfo> _Channel_GetProgramGuideRequest[] = {
  { { ContextTag(0), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::epoch_s
  { { ContextTag(1), "endTime", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::epoch_s
  { { ContextTag(2), "channelList", ItemType::kList }, 861 }, // Channel::ChannelInfoStruct[]
  { { ContextTag(3), "pageToken", ItemType::kDefault }, 847 }, // Channel::PageTokenStruct
  { { ContextTag(4), "recordingFlag", ItemType::kBitmap }, 868 }, // Channel::RecordingFlagBitmap
  { { ContextTag(5), "externalIDList", ItemType::kList }, 863 }, // Channel::AdditionalInfoStruct[]
  { { ContextTag(6), "data", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::octet_string
};

const Entry<ItemInfo> _Channel_ProgramGuideResponse[] = {
  { { ContextTag(0), "paging", ItemType::kDefault }, 848 }, // Channel::ChannelPagingStruct
  { { ContextTag(1), "programList", ItemType::kList }, 859 }, // Channel::ProgramStruct[]
};

const Entry<ItemInfo> _Channel_RecordProgramRequest[] = {
  { { ContextTag(0), "programIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(1), "shouldRecordSeries", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::boolean
  { { ContextTag(2), "externalIDList", ItemType::kList }, 863 }, // Channel::AdditionalInfoStruct[]
  { { ContextTag(3), "data", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::octet_string
};

const Entry<ItemInfo> _Channel_CancelRecordProgramRequest[] = {
  { { ContextTag(0), "programIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::char_string
  { { ContextTag(1), "shouldRecordSeries", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::boolean
  { { ContextTag(2), "externalIDList", ItemType::kList }, 863 }, // Channel::AdditionalInfoStruct[]
  { { ContextTag(3), "data", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::octet_string
};

const Entry<ItemInfo> _Channel_ProgramStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 846 }, // Channel_ProgramStruct[]
};

const Entry<ItemInfo> _Channel_ProgramCategoryStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 843 }, // Channel_ProgramCategoryStruct[]
};

const Entry<ItemInfo> _Channel_ChannelInfoStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 845 }, // Channel_ChannelInfoStruct[]
};

const Entry<ItemInfo> _Channel_ProgramCastStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 842 }, // Channel_ProgramCastStruct[]
};

const Entry<ItemInfo> _Channel_AdditionalInfoStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 849 }, // Channel_AdditionalInfoStruct[]
};

const Entry<ItemInfo> _Channel_ChannelTypeEnum[] = {
  { { ConstantValueTag(0x0), "kSatellite", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::ChannelTypeEnum::kSatellite
  { { ConstantValueTag(0x1), "kCable", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::ChannelTypeEnum::kCable
  { { ConstantValueTag(0x2), "kTerrestrial", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::ChannelTypeEnum::kTerrestrial
};

const Entry<ItemInfo> _Channel_LineupInfoTypeEnum[] = {
  { { ConstantValueTag(0x0), "kMSO", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::LineupInfoTypeEnum::kMSO
};

const Entry<ItemInfo> _Channel_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::StatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kMultipleMatches", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::StatusEnum::kMultipleMatches
  { { ConstantValueTag(0x2), "kNoMatches", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::StatusEnum::kNoMatches
};

const Entry<ItemInfo> _Channel_Feature[] = {
  { { ConstantValueTag(0x1), "kChannelList", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::Feature::kChannelList
  { { ConstantValueTag(0x2), "kLineupInfo", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::Feature::kLineupInfo
  { { ConstantValueTag(0x3), "kElectronicGuide", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::Feature::kElectronicGuide
  { { ConstantValueTag(0x4), "kRecordProgram", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::Feature::kRecordProgram
};

const Entry<ItemInfo> _Channel_RecordingFlagBitmap[] = {
  { { ConstantValueTag(0x1), "kScheduled", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::RecordingFlagBitmap::kScheduled
  { { ConstantValueTag(0x2), "kRecordSeries", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::RecordingFlagBitmap::kRecordSeries
  { { ConstantValueTag(0x3), "kRecorded", ItemType::kDefault }, kInvalidNodeIndex }, // Channel::RecordingFlagBitmap::kRecorded
};

const Entry<ItemInfo> _TargetNavigator[] = {
  { { AttributeTag(0), "targetList", ItemType::kList }, 874 }, // TargetNavigator::TargetInfoStruct[]
  { { AttributeTag(1), "currentTarget", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // TargetNavigator::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // TargetNavigator::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // TargetNavigator::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // TargetNavigator::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::int16u
  { { EventTag(0), "TargetUpdated", ItemType::kDefault }, 873 }, // TargetNavigator::TargetUpdated
  { { CommandTag(0), "NavigateTarget", ItemType::kDefault }, 871 }, // TargetNavigator::NavigateTarget::NavigateTargetRequest
  { { CommandTag(1), "NavigateTargetResponse", ItemType::kDefault }, 872 }, // TargetNavigator::NavigateTargetResponse
};

const Entry<ItemInfo> _TargetNavigator_TargetInfoStruct[] = {
  { { ContextTag(0), "identifier", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::int8u
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::char_string
};

const Entry<ItemInfo> _TargetNavigator_NavigateTargetRequest[] = {
  { { ContextTag(0), "target", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::int8u
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::char_string
};

const Entry<ItemInfo> _TargetNavigator_NavigateTargetResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 875 }, // TargetNavigator::StatusEnum
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::char_string
};

const Entry<ItemInfo> _TargetNavigator_TargetUpdated[] = {
  { { ContextTag(0), "targetList", ItemType::kList }, 874 }, // TargetNavigator::TargetInfoStruct[]
  { { ContextTag(1), "currentTarget", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::int8u
  { { ContextTag(2), "data", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::octet_string
};

const Entry<ItemInfo> _TargetNavigator_TargetInfoStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 870 }, // TargetNavigator_TargetInfoStruct[]
};

const Entry<ItemInfo> _TargetNavigator_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::StatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kTargetNotFound", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::StatusEnum::kTargetNotFound
  { { ConstantValueTag(0x2), "kNotAllowed", ItemType::kDefault }, kInvalidNodeIndex }, // TargetNavigator::StatusEnum::kNotAllowed
};

const Entry<ItemInfo> _MediaPlayback[] = {
  { { AttributeTag(0), "currentState", ItemType::kEnum }, 891 }, // MediaPlayback::PlaybackStateEnum
  { { AttributeTag(1), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::epoch_us
  { { AttributeTag(2), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::int64u
  { { AttributeTag(3), "sampledPosition", ItemType::kDefault }, 879 }, // MediaPlayback::PlaybackPositionStruct
  { { AttributeTag(4), "playbackSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::single
  { { AttributeTag(5), "seekRangeEnd", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::int64u
  { { AttributeTag(6), "seekRangeStart", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::int64u
  { { AttributeTag(7), "activeAudioTrack", ItemType::kDefault }, 878 }, // MediaPlayback::TrackStruct
  { { AttributeTag(8), "availableAudioTracks", ItemType::kList }, 889 }, // MediaPlayback::TrackStruct[]
  { { AttributeTag(9), "activeTextTrack", ItemType::kDefault }, 878 }, // MediaPlayback::TrackStruct
  { { AttributeTag(10), "availableTextTracks", ItemType::kList }, 889 }, // MediaPlayback::TrackStruct[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // MediaPlayback::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // MediaPlayback::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // MediaPlayback::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // MediaPlayback::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 893 }, // MediaPlayback::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::int16u
  { { EventTag(0), "StateChanged", ItemType::kDefault }, 888 }, // MediaPlayback::StateChanged
  { { CommandTag(0), "Play", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Play::()
  { { CommandTag(1), "Pause", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Pause::()
  { { CommandTag(2), "Stop", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Stop::()
  { { CommandTag(3), "StartOver", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::StartOver::()
  { { CommandTag(4), "Previous", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Previous::()
  { { CommandTag(5), "Next", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Next::()
  { { CommandTag(6), "Rewind", ItemType::kDefault }, 880 }, // MediaPlayback::Rewind::RewindRequest
  { { CommandTag(7), "FastForward", ItemType::kDefault }, 881 }, // MediaPlayback::FastForward::FastForwardRequest
  { { CommandTag(8), "SkipForward", ItemType::kDefault }, 882 }, // MediaPlayback::SkipForward::SkipForwardRequest
  { { CommandTag(9), "SkipBackward", ItemType::kDefault }, 883 }, // MediaPlayback::SkipBackward::SkipBackwardRequest
  { { CommandTag(11), "Seek", ItemType::kDefault }, 885 }, // MediaPlayback::Seek::SeekRequest
  { { CommandTag(12), "ActivateAudioTrack", ItemType::kDefault }, 886 }, // MediaPlayback::ActivateAudioTrack::ActivateAudioTrackRequest
  { { CommandTag(13), "ActivateTextTrack", ItemType::kDefault }, 887 }, // MediaPlayback::ActivateTextTrack::ActivateTextTrackRequest
  { { CommandTag(14), "DeactivateTextTrack", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::DeactivateTextTrack::()
  { { CommandTag(10), "PlaybackResponse", ItemType::kDefault }, 884 }, // MediaPlayback::PlaybackResponse
};

const Entry<ItemInfo> _MediaPlayback_TrackAttributesStruct[] = {
  { { ContextTag(0), "languageCode", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::char_string
  { { ContextTag(1), "displayName", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::char_string
};

const Entry<ItemInfo> _MediaPlayback_TrackStruct[] = {
  { { ContextTag(0), "id", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::char_string
  { { ContextTag(1), "trackAttributes", ItemType::kDefault }, 877 }, // MediaPlayback::TrackAttributesStruct
};

const Entry<ItemInfo> _MediaPlayback_PlaybackPositionStruct[] = {
  { { ContextTag(0), "updatedAt", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::epoch_us
  { { ContextTag(1), "position", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::int64u
};

const Entry<ItemInfo> _MediaPlayback_RewindRequest[] = {
  { { ContextTag(0), "audioAdvanceUnmuted", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::boolean
};

const Entry<ItemInfo> _MediaPlayback_FastForwardRequest[] = {
  { { ContextTag(0), "audioAdvanceUnmuted", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::boolean
};

const Entry<ItemInfo> _MediaPlayback_SkipForwardRequest[] = {
  { { ContextTag(0), "deltaPositionMilliseconds", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::int64u
};

const Entry<ItemInfo> _MediaPlayback_SkipBackwardRequest[] = {
  { { ContextTag(0), "deltaPositionMilliseconds", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::int64u
};

const Entry<ItemInfo> _MediaPlayback_PlaybackResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 892 }, // MediaPlayback::StatusEnum
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::char_string
};

const Entry<ItemInfo> _MediaPlayback_SeekRequest[] = {
  { { ContextTag(0), "position", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::int64u
};

const Entry<ItemInfo> _MediaPlayback_ActivateAudioTrackRequest[] = {
  { { ContextTag(0), "trackID", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CHAR_STRING
  { { ContextTag(1), "audioOutputIndex", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::INT8U
};

const Entry<ItemInfo> _MediaPlayback_ActivateTextTrackRequest[] = {
  { { ContextTag(0), "trackID", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CHAR_STRING
};

const Entry<ItemInfo> _MediaPlayback_StateChanged[] = {
  { { ContextTag(0), "currentState", ItemType::kEnum }, 891 }, // MediaPlayback::PlaybackStateEnum
  { { ContextTag(1), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::EPOCH_US
  { { ContextTag(2), "duration", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::INT64U
  { { ContextTag(3), "sampledPosition", ItemType::kDefault }, 879 }, // MediaPlayback::PlaybackPositionStruct
  { { ContextTag(4), "playbackSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::single
  { { ContextTag(5), "seekRangeEnd", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::INT64U
  { { ContextTag(6), "seekRangeStart", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::INT64U
  { { ContextTag(7), "data", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::OCTET_STRING
  { { ContextTag(8), "audioAdvanceUnmuted", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::boolean
};

const Entry<ItemInfo> _MediaPlayback_TrackStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 878 }, // MediaPlayback_TrackStruct[]
};

const Entry<ItemInfo> _MediaPlayback_CharacteristicEnum[] = {
  { { ConstantValueTag(0x0), "kForcedSubtitles", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kForcedSubtitles
  { { ConstantValueTag(0x1), "kDescribesVideo", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kDescribesVideo
  { { ConstantValueTag(0x2), "kEasyToRead", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kEasyToRead
  { { ConstantValueTag(0x3), "kFrameBased", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kFrameBased
  { { ConstantValueTag(0x4), "kMainProgram", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kMainProgram
  { { ConstantValueTag(0x5), "kOriginalContent", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kOriginalContent
  { { ConstantValueTag(0x6), "kVoiceOverTranslation", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kVoiceOverTranslation
  { { ConstantValueTag(0x7), "kCaption", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kCaption
  { { ConstantValueTag(0x8), "kSubtitle", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kSubtitle
  { { ConstantValueTag(0x9), "kAlternate", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kAlternate
  { { ConstantValueTag(0xA), "kSupplementary", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kSupplementary
  { { ConstantValueTag(0xB), "kCommentary", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kCommentary
  { { ConstantValueTag(0xC), "kDubbedTranslation", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kDubbedTranslation
  { { ConstantValueTag(0xD), "kDescription", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kDescription
  { { ConstantValueTag(0xE), "kMetadata", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kMetadata
  { { ConstantValueTag(0xF), "kEnhancedAudioIntelligibility", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kEnhancedAudioIntelligibility
  { { ConstantValueTag(0x10), "kEmergency", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kEmergency
  { { ConstantValueTag(0x11), "kKaraoke", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::CharacteristicEnum::kKaraoke
};

const Entry<ItemInfo> _MediaPlayback_PlaybackStateEnum[] = {
  { { ConstantValueTag(0x0), "kPlaying", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::PlaybackStateEnum::kPlaying
  { { ConstantValueTag(0x1), "kPaused", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::PlaybackStateEnum::kPaused
  { { ConstantValueTag(0x2), "kNotPlaying", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::PlaybackStateEnum::kNotPlaying
  { { ConstantValueTag(0x3), "kBuffering", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::PlaybackStateEnum::kBuffering
};

const Entry<ItemInfo> _MediaPlayback_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::StatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kInvalidStateForCommand", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::StatusEnum::kInvalidStateForCommand
  { { ConstantValueTag(0x2), "kNotAllowed", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::StatusEnum::kNotAllowed
  { { ConstantValueTag(0x3), "kNotActive", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::StatusEnum::kNotActive
  { { ConstantValueTag(0x4), "kSpeedOutOfRange", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::StatusEnum::kSpeedOutOfRange
  { { ConstantValueTag(0x5), "kSeekOutOfRange", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::StatusEnum::kSeekOutOfRange
};

const Entry<ItemInfo> _MediaPlayback_Feature[] = {
  { { ConstantValueTag(0x1), "kAdvancedSeek", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Feature::kAdvancedSeek
  { { ConstantValueTag(0x2), "kVariableSpeed", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Feature::kVariableSpeed
  { { ConstantValueTag(0x3), "kTextTracks", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Feature::kTextTracks
  { { ConstantValueTag(0x4), "kAudioTracks", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Feature::kAudioTracks
  { { ConstantValueTag(0x5), "kAudioAdvance", ItemType::kDefault }, kInvalidNodeIndex }, // MediaPlayback::Feature::kAudioAdvance
};

const Entry<ItemInfo> _MediaInput[] = {
  { { AttributeTag(0), "inputList", ItemType::kList }, 898 }, // MediaInput::InputInfoStruct[]
  { { AttributeTag(1), "currentInput", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // MediaInput::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // MediaInput::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // MediaInput::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // MediaInput::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 900 }, // MediaInput::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::int16u
  { { CommandTag(0), "SelectInput", ItemType::kDefault }, 896 }, // MediaInput::SelectInput::SelectInputRequest
  { { CommandTag(1), "ShowInputStatus", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::ShowInputStatus::()
  { { CommandTag(2), "HideInputStatus", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::HideInputStatus::()
  { { CommandTag(3), "RenameInput", ItemType::kDefault }, 897 }, // MediaInput::RenameInput::RenameInputRequest
};

const Entry<ItemInfo> _MediaInput_InputInfoStruct[] = {
  { { ContextTag(0), "index", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::int8u
  { { ContextTag(1), "inputType", ItemType::kEnum }, 899 }, // MediaInput::InputTypeEnum
  { { ContextTag(2), "name", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::char_string
  { { ContextTag(3), "description", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::char_string
};

const Entry<ItemInfo> _MediaInput_SelectInputRequest[] = {
  { { ContextTag(0), "index", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::int8u
};

const Entry<ItemInfo> _MediaInput_RenameInputRequest[] = {
  { { ContextTag(0), "index", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::int8u
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::char_string
};

const Entry<ItemInfo> _MediaInput_InputInfoStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 895 }, // MediaInput_InputInfoStruct[]
};

const Entry<ItemInfo> _MediaInput_InputTypeEnum[] = {
  { { ConstantValueTag(0x0), "kInternal", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kInternal
  { { ConstantValueTag(0x1), "kAux", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kAux
  { { ConstantValueTag(0x2), "kCoax", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kCoax
  { { ConstantValueTag(0x3), "kComposite", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kComposite
  { { ConstantValueTag(0x4), "kHDMI", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kHDMI
  { { ConstantValueTag(0x5), "kInput", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kInput
  { { ConstantValueTag(0x6), "kLine", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kLine
  { { ConstantValueTag(0x7), "kOptical", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kOptical
  { { ConstantValueTag(0x8), "kVideo", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kVideo
  { { ConstantValueTag(0x9), "kSCART", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kSCART
  { { ConstantValueTag(0xA), "kUSB", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kUSB
  { { ConstantValueTag(0xB), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::InputTypeEnum::kOther
};

const Entry<ItemInfo> _MediaInput_Feature[] = {
  { { ConstantValueTag(0x1), "kNameUpdates", ItemType::kDefault }, kInvalidNodeIndex }, // MediaInput::Feature::kNameUpdates
};

const Entry<ItemInfo> _LowPower[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // LowPower::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // LowPower::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // LowPower::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // LowPower::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // LowPower::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // LowPower::int16u
  { { CommandTag(0), "Sleep", ItemType::kDefault }, kInvalidNodeIndex }, // LowPower::Sleep::()
};

const Entry<ItemInfo> _KeypadInput[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // KeypadInput::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // KeypadInput::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // KeypadInput::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // KeypadInput::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 907 }, // KeypadInput::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::int16u
  { { CommandTag(0), "SendKey", ItemType::kDefault }, 903 }, // KeypadInput::SendKey::SendKeyRequest
  { { CommandTag(1), "SendKeyResponse", ItemType::kDefault }, 904 }, // KeypadInput::SendKeyResponse
};

const Entry<ItemInfo> _KeypadInput_SendKeyRequest[] = {
  { { ContextTag(0), "keyCode", ItemType::kEnum }, 905 }, // KeypadInput::CECKeyCodeEnum
};

const Entry<ItemInfo> _KeypadInput_SendKeyResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 906 }, // KeypadInput::StatusEnum
};

const Entry<ItemInfo> _KeypadInput_CECKeyCodeEnum[] = {
  { { ConstantValueTag(0x0), "kSelect", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSelect
  { { ConstantValueTag(0x1), "kUp", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kUp
  { { ConstantValueTag(0x2), "kDown", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kDown
  { { ConstantValueTag(0x3), "kLeft", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kLeft
  { { ConstantValueTag(0x4), "kRight", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kRight
  { { ConstantValueTag(0x5), "kRightUp", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kRightUp
  { { ConstantValueTag(0x6), "kRightDown", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kRightDown
  { { ConstantValueTag(0x7), "kLeftUp", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kLeftUp
  { { ConstantValueTag(0x8), "kLeftDown", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kLeftDown
  { { ConstantValueTag(0x9), "kRootMenu", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kRootMenu
  { { ConstantValueTag(0xA), "kSetupMenu", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSetupMenu
  { { ConstantValueTag(0xB), "kContentsMenu", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kContentsMenu
  { { ConstantValueTag(0xC), "kFavoriteMenu", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kFavoriteMenu
  { { ConstantValueTag(0xD), "kExit", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kExit
  { { ConstantValueTag(0x10), "kMediaTopMenu", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kMediaTopMenu
  { { ConstantValueTag(0x11), "kMediaContextSensitiveMenu", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kMediaContextSensitiveMenu
  { { ConstantValueTag(0x1D), "kNumberEntryMode", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumberEntryMode
  { { ConstantValueTag(0x1E), "kNumber11", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumber11
  { { ConstantValueTag(0x1F), "kNumber12", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumber12
  { { ConstantValueTag(0x20), "kNumber0OrNumber10", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumber0OrNumber10
  { { ConstantValueTag(0x21), "kNumbers1", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers1
  { { ConstantValueTag(0x22), "kNumbers2", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers2
  { { ConstantValueTag(0x23), "kNumbers3", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers3
  { { ConstantValueTag(0x24), "kNumbers4", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers4
  { { ConstantValueTag(0x25), "kNumbers5", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers5
  { { ConstantValueTag(0x26), "kNumbers6", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers6
  { { ConstantValueTag(0x27), "kNumbers7", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers7
  { { ConstantValueTag(0x28), "kNumbers8", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers8
  { { ConstantValueTag(0x29), "kNumbers9", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNumbers9
  { { ConstantValueTag(0x2A), "kDot", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kDot
  { { ConstantValueTag(0x2B), "kEnter", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kEnter
  { { ConstantValueTag(0x2C), "kClear", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kClear
  { { ConstantValueTag(0x2F), "kNextFavorite", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kNextFavorite
  { { ConstantValueTag(0x30), "kChannelUp", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kChannelUp
  { { ConstantValueTag(0x31), "kChannelDown", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kChannelDown
  { { ConstantValueTag(0x32), "kPreviousChannel", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPreviousChannel
  { { ConstantValueTag(0x33), "kSoundSelect", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSoundSelect
  { { ConstantValueTag(0x34), "kInputSelect", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kInputSelect
  { { ConstantValueTag(0x35), "kDisplayInformation", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kDisplayInformation
  { { ConstantValueTag(0x36), "kHelp", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kHelp
  { { ConstantValueTag(0x37), "kPageUp", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPageUp
  { { ConstantValueTag(0x38), "kPageDown", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPageDown
  { { ConstantValueTag(0x40), "kPower", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPower
  { { ConstantValueTag(0x41), "kVolumeUp", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kVolumeUp
  { { ConstantValueTag(0x42), "kVolumeDown", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kVolumeDown
  { { ConstantValueTag(0x43), "kMute", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kMute
  { { ConstantValueTag(0x44), "kPlay", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPlay
  { { ConstantValueTag(0x45), "kStop", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kStop
  { { ConstantValueTag(0x46), "kPause", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPause
  { { ConstantValueTag(0x47), "kRecord", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kRecord
  { { ConstantValueTag(0x48), "kRewind", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kRewind
  { { ConstantValueTag(0x49), "kFastForward", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kFastForward
  { { ConstantValueTag(0x4A), "kEject", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kEject
  { { ConstantValueTag(0x4B), "kForward", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kForward
  { { ConstantValueTag(0x4C), "kBackward", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kBackward
  { { ConstantValueTag(0x4D), "kStopRecord", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kStopRecord
  { { ConstantValueTag(0x4E), "kPauseRecord", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPauseRecord
  { { ConstantValueTag(0x4F), "kReserved", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kReserved
  { { ConstantValueTag(0x50), "kAngle", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kAngle
  { { ConstantValueTag(0x51), "kSubPicture", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSubPicture
  { { ConstantValueTag(0x52), "kVideoOnDemand", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kVideoOnDemand
  { { ConstantValueTag(0x53), "kElectronicProgramGuide", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kElectronicProgramGuide
  { { ConstantValueTag(0x54), "kTimerProgramming", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kTimerProgramming
  { { ConstantValueTag(0x55), "kInitialConfiguration", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kInitialConfiguration
  { { ConstantValueTag(0x56), "kSelectBroadcastType", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSelectBroadcastType
  { { ConstantValueTag(0x57), "kSelectSoundPresentation", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSelectSoundPresentation
  { { ConstantValueTag(0x60), "kPlayFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPlayFunction
  { { ConstantValueTag(0x61), "kPausePlayFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPausePlayFunction
  { { ConstantValueTag(0x62), "kRecordFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kRecordFunction
  { { ConstantValueTag(0x63), "kPauseRecordFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPauseRecordFunction
  { { ConstantValueTag(0x64), "kStopFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kStopFunction
  { { ConstantValueTag(0x65), "kMuteFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kMuteFunction
  { { ConstantValueTag(0x66), "kRestoreVolumeFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kRestoreVolumeFunction
  { { ConstantValueTag(0x67), "kTuneFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kTuneFunction
  { { ConstantValueTag(0x68), "kSelectMediaFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSelectMediaFunction
  { { ConstantValueTag(0x69), "kSelectAvInputFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSelectAvInputFunction
  { { ConstantValueTag(0x6A), "kSelectAudioInputFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kSelectAudioInputFunction
  { { ConstantValueTag(0x6B), "kPowerToggleFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPowerToggleFunction
  { { ConstantValueTag(0x6C), "kPowerOffFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPowerOffFunction
  { { ConstantValueTag(0x6D), "kPowerOnFunction", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kPowerOnFunction
  { { ConstantValueTag(0x71), "kF1Blue", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kF1Blue
  { { ConstantValueTag(0x72), "kF2Red", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kF2Red
  { { ConstantValueTag(0x73), "kF3Green", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kF3Green
  { { ConstantValueTag(0x74), "kF4Yellow", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kF4Yellow
  { { ConstantValueTag(0x75), "kF5", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kF5
  { { ConstantValueTag(0x76), "kData", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::CECKeyCodeEnum::kData
};

const Entry<ItemInfo> _KeypadInput_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::StatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kUnsupportedKey", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::StatusEnum::kUnsupportedKey
  { { ConstantValueTag(0x2), "kInvalidKeyInCurrentState", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::StatusEnum::kInvalidKeyInCurrentState
};

const Entry<ItemInfo> _KeypadInput_Feature[] = {
  { { ConstantValueTag(0x1), "kNavigationKeyCodes", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::Feature::kNavigationKeyCodes
  { { ConstantValueTag(0x2), "kLocationKeys", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::Feature::kLocationKeys
  { { ConstantValueTag(0x4), "kNumberKeys", ItemType::kDefault }, kInvalidNodeIndex }, // KeypadInput::Feature::kNumberKeys
};

const Entry<ItemInfo> _ContentLauncher[] = {
  { { AttributeTag(0), "acceptHeader", ItemType::kList }, 1 }, // ContentLauncher::char_string[]
  { { AttributeTag(1), "supportedStreamingProtocols", ItemType::kBitmap }, 929 }, // ContentLauncher::SupportedProtocolsBitmap
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ContentLauncher::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ContentLauncher::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ContentLauncher::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ContentLauncher::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 928 }, // ContentLauncher::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::int16u
  { { CommandTag(0), "LaunchContent", ItemType::kDefault }, 917 }, // ContentLauncher::LaunchContent::LaunchContentRequest
  { { CommandTag(1), "LaunchURL", ItemType::kDefault }, 918 }, // ContentLauncher::LaunchURL::LaunchURLRequest
  { { CommandTag(2), "LauncherResponse", ItemType::kDefault }, 919 }, // ContentLauncher::LauncherResponse
};

const Entry<ItemInfo> _ContentLauncher_DimensionStruct[] = {
  { { ContextTag(0), "width", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::double
  { { ContextTag(1), "height", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::double
  { { ContextTag(2), "metric", ItemType::kEnum }, 925 }, // ContentLauncher::MetricTypeEnum
};

const Entry<ItemInfo> _ContentLauncher_TrackPreferenceStruct[] = {
  { { ContextTag(0), "languageCode", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(1), "characteristics", ItemType::kList }, 921 }, // ContentLauncher::CharacteristicEnum[]
  { { ContextTag(2), "audioOutputIndex", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::int8u
};

const Entry<ItemInfo> _ContentLauncher_PlaybackPreferencesStruct[] = {
  { { ContextTag(0), "playbackPosition", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::int64u
  { { ContextTag(1), "textTrack", ItemType::kDefault }, 910 }, // ContentLauncher::TrackPreferenceStruct
  { { ContextTag(2), "audioTracks", ItemType::kList }, 920 }, // ContentLauncher::TrackPreferenceStruct[]
};

const Entry<ItemInfo> _ContentLauncher_AdditionalInfoStruct[] = {
  { { ContextTag(0), "name", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
};

const Entry<ItemInfo> _ContentLauncher_ParameterStruct[] = {
  { { ContextTag(0), "type", ItemType::kEnum }, 926 }, // ContentLauncher::ParameterEnum
  { { ContextTag(1), "value", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(2), "externalIDList", ItemType::kList }, 923 }, // ContentLauncher::AdditionalInfoStruct[]
};

const Entry<ItemInfo> _ContentLauncher_ContentSearchStruct[] = {
  { { ContextTag(0), "parameterList", ItemType::kList }, 922 }, // ContentLauncher::ParameterStruct[]
};

const Entry<ItemInfo> _ContentLauncher_StyleInformationStruct[] = {
  { { ContextTag(0), "imageURL", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(1), "color", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(2), "size", ItemType::kDefault }, 909 }, // ContentLauncher::DimensionStruct
};

const Entry<ItemInfo> _ContentLauncher_BrandingInformationStruct[] = {
  { { ContextTag(0), "providerName", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(1), "background", ItemType::kDefault }, 915 }, // ContentLauncher::StyleInformationStruct
  { { ContextTag(2), "logo", ItemType::kDefault }, 915 }, // ContentLauncher::StyleInformationStruct
  { { ContextTag(3), "progressBar", ItemType::kDefault }, 915 }, // ContentLauncher::StyleInformationStruct
  { { ContextTag(4), "splash", ItemType::kDefault }, 915 }, // ContentLauncher::StyleInformationStruct
  { { ContextTag(5), "waterMark", ItemType::kDefault }, 915 }, // ContentLauncher::StyleInformationStruct
};

const Entry<ItemInfo> _ContentLauncher_LaunchContentRequest[] = {
  { { ContextTag(0), "search", ItemType::kDefault }, 914 }, // ContentLauncher::ContentSearchStruct
  { { ContextTag(1), "autoPlay", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::boolean
  { { ContextTag(2), "data", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(3), "playbackPreferences", ItemType::kDefault }, 911 }, // ContentLauncher::PlaybackPreferencesStruct
  { { ContextTag(4), "useCurrentContext", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::boolean
};

const Entry<ItemInfo> _ContentLauncher_LaunchURLRequest[] = {
  { { ContextTag(0), "contentURL", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(1), "displayString", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
  { { ContextTag(2), "brandingInformation", ItemType::kDefault }, 916 }, // ContentLauncher::BrandingInformationStruct
};

const Entry<ItemInfo> _ContentLauncher_LauncherResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 927 }, // ContentLauncher::StatusEnum
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::char_string
};

const Entry<ItemInfo> _ContentLauncher_TrackPreferenceStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 910 }, // ContentLauncher_TrackPreferenceStruct[]
};

const Entry<ItemInfo> _ContentLauncher_CharacteristicEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 924 }, // ContentLauncher_CharacteristicEnum[]
};

const Entry<ItemInfo> _ContentLauncher_ParameterStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 913 }, // ContentLauncher_ParameterStruct[]
};

const Entry<ItemInfo> _ContentLauncher_AdditionalInfoStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 912 }, // ContentLauncher_AdditionalInfoStruct[]
};

const Entry<ItemInfo> _ContentLauncher_CharacteristicEnum[] = {
  { { ConstantValueTag(0x0), "kForcedSubtitles", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kForcedSubtitles
  { { ConstantValueTag(0x1), "kDescribesVideo", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kDescribesVideo
  { { ConstantValueTag(0x2), "kEasyToRead", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kEasyToRead
  { { ConstantValueTag(0x3), "kFrameBased", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kFrameBased
  { { ConstantValueTag(0x4), "kMainProgram", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kMainProgram
  { { ConstantValueTag(0x5), "kOriginalContent", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kOriginalContent
  { { ConstantValueTag(0x6), "kVoiceOverTranslation", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kVoiceOverTranslation
  { { ConstantValueTag(0x7), "kCaption", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kCaption
  { { ConstantValueTag(0x8), "kSubtitle", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kSubtitle
  { { ConstantValueTag(0x9), "kAlternate", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kAlternate
  { { ConstantValueTag(0xA), "kSupplementary", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kSupplementary
  { { ConstantValueTag(0xB), "kCommentary", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kCommentary
  { { ConstantValueTag(0xC), "kDubbedTranslation", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kDubbedTranslation
  { { ConstantValueTag(0xD), "kDescription", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kDescription
  { { ConstantValueTag(0xE), "kMetadata", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kMetadata
  { { ConstantValueTag(0xF), "kEnhancedAudioIntelligibility", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kEnhancedAudioIntelligibility
  { { ConstantValueTag(0x10), "kEmergency", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kEmergency
  { { ConstantValueTag(0x11), "kKaraoke", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::CharacteristicEnum::kKaraoke
};

const Entry<ItemInfo> _ContentLauncher_MetricTypeEnum[] = {
  { { ConstantValueTag(0x0), "kPixels", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::MetricTypeEnum::kPixels
  { { ConstantValueTag(0x1), "kPercentage", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::MetricTypeEnum::kPercentage
};

const Entry<ItemInfo> _ContentLauncher_ParameterEnum[] = {
  { { ConstantValueTag(0x0), "kActor", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kActor
  { { ConstantValueTag(0x1), "kChannel", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kChannel
  { { ConstantValueTag(0x2), "kCharacter", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kCharacter
  { { ConstantValueTag(0x3), "kDirector", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kDirector
  { { ConstantValueTag(0x4), "kEvent", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kEvent
  { { ConstantValueTag(0x5), "kFranchise", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kFranchise
  { { ConstantValueTag(0x6), "kGenre", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kGenre
  { { ConstantValueTag(0x7), "kLeague", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kLeague
  { { ConstantValueTag(0x8), "kPopularity", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kPopularity
  { { ConstantValueTag(0x9), "kProvider", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kProvider
  { { ConstantValueTag(0xA), "kSport", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kSport
  { { ConstantValueTag(0xB), "kSportsTeam", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kSportsTeam
  { { ConstantValueTag(0xC), "kType", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kType
  { { ConstantValueTag(0xD), "kVideo", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kVideo
  { { ConstantValueTag(0xE), "kSeason", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kSeason
  { { ConstantValueTag(0xF), "kEpisode", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kEpisode
  { { ConstantValueTag(0x10), "kAny", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::ParameterEnum::kAny
};

const Entry<ItemInfo> _ContentLauncher_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::StatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kURLNotAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::StatusEnum::kURLNotAvailable
  { { ConstantValueTag(0x2), "kAuthFailed", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::StatusEnum::kAuthFailed
  { { ConstantValueTag(0x3), "kTextTrackNotAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::StatusEnum::kTextTrackNotAvailable
  { { ConstantValueTag(0x4), "kAudioTrackNotAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::StatusEnum::kAudioTrackNotAvailable
};

const Entry<ItemInfo> _ContentLauncher_Feature[] = {
  { { ConstantValueTag(0x1), "kContentSearch", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::Feature::kContentSearch
  { { ConstantValueTag(0x2), "kURLPlayback", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::Feature::kURLPlayback
  { { ConstantValueTag(0x3), "kAdvancedSeek", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::Feature::kAdvancedSeek
  { { ConstantValueTag(0x4), "kTextTracks", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::Feature::kTextTracks
  { { ConstantValueTag(0x5), "kAudioTracks", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::Feature::kAudioTracks
};

const Entry<ItemInfo> _ContentLauncher_SupportedProtocolsBitmap[] = {
  { { ConstantValueTag(0x1), "kDASH", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::SupportedProtocolsBitmap::kDASH
  { { ConstantValueTag(0x2), "kHLS", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::SupportedProtocolsBitmap::kHLS
  { { ConstantValueTag(0x2), "kWebRTC", ItemType::kDefault }, kInvalidNodeIndex }, // ContentLauncher::SupportedProtocolsBitmap::kWebRTC
};

const Entry<ItemInfo> _AudioOutput[] = {
  { { AttributeTag(0), "outputList", ItemType::kList }, 934 }, // AudioOutput::OutputInfoStruct[]
  { { AttributeTag(1), "currentOutput", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // AudioOutput::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // AudioOutput::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // AudioOutput::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // AudioOutput::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 936 }, // AudioOutput::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::int16u
  { { CommandTag(0), "SelectOutput", ItemType::kDefault }, 932 }, // AudioOutput::SelectOutput::SelectOutputRequest
  { { CommandTag(1), "RenameOutput", ItemType::kDefault }, 933 }, // AudioOutput::RenameOutput::RenameOutputRequest
};

const Entry<ItemInfo> _AudioOutput_OutputInfoStruct[] = {
  { { ContextTag(0), "index", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::int8u
  { { ContextTag(1), "outputType", ItemType::kEnum }, 935 }, // AudioOutput::OutputTypeEnum
  { { ContextTag(2), "name", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::char_string
};

const Entry<ItemInfo> _AudioOutput_SelectOutputRequest[] = {
  { { ContextTag(0), "index", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::int8u
};

const Entry<ItemInfo> _AudioOutput_RenameOutputRequest[] = {
  { { ContextTag(0), "index", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::int8u
  { { ContextTag(1), "name", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::char_string
};

const Entry<ItemInfo> _AudioOutput_OutputInfoStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 931 }, // AudioOutput_OutputInfoStruct[]
};

const Entry<ItemInfo> _AudioOutput_OutputTypeEnum[] = {
  { { ConstantValueTag(0x0), "kHDMI", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::OutputTypeEnum::kHDMI
  { { ConstantValueTag(0x1), "kBT", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::OutputTypeEnum::kBT
  { { ConstantValueTag(0x2), "kOptical", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::OutputTypeEnum::kOptical
  { { ConstantValueTag(0x3), "kHeadphone", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::OutputTypeEnum::kHeadphone
  { { ConstantValueTag(0x4), "kInternal", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::OutputTypeEnum::kInternal
  { { ConstantValueTag(0x5), "kOther", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::OutputTypeEnum::kOther
};

const Entry<ItemInfo> _AudioOutput_Feature[] = {
  { { ConstantValueTag(0x1), "kNameUpdates", ItemType::kDefault }, kInvalidNodeIndex }, // AudioOutput::Feature::kNameUpdates
};

const Entry<ItemInfo> _ApplicationLauncher[] = {
  { { AttributeTag(0), "catalogList", ItemType::kList }, 1 }, // ApplicationLauncher::int16u[]
  { { AttributeTag(1), "currentApp", ItemType::kDefault }, 939 }, // ApplicationLauncher::ApplicationEPStruct
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ApplicationLauncher::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ApplicationLauncher::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ApplicationLauncher::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ApplicationLauncher::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 945 }, // ApplicationLauncher::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::int16u
  { { CommandTag(0), "LaunchApp", ItemType::kDefault }, 940 }, // ApplicationLauncher::LaunchApp::LaunchAppRequest
  { { CommandTag(1), "StopApp", ItemType::kDefault }, 941 }, // ApplicationLauncher::StopApp::StopAppRequest
  { { CommandTag(2), "HideApp", ItemType::kDefault }, 942 }, // ApplicationLauncher::HideApp::HideAppRequest
  { { CommandTag(3), "LauncherResponse", ItemType::kDefault }, 943 }, // ApplicationLauncher::LauncherResponse
};

const Entry<ItemInfo> _ApplicationLauncher_ApplicationStruct[] = {
  { { ContextTag(0), "catalogVendorID", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::int16u
  { { ContextTag(1), "applicationID", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::char_string
};

const Entry<ItemInfo> _ApplicationLauncher_ApplicationEPStruct[] = {
  { { ContextTag(0), "application", ItemType::kDefault }, 938 }, // ApplicationLauncher::ApplicationStruct
  { { ContextTag(1), "endpoint", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::endpoint_no
};

const Entry<ItemInfo> _ApplicationLauncher_LaunchAppRequest[] = {
  { { ContextTag(0), "application", ItemType::kDefault }, 938 }, // ApplicationLauncher::ApplicationStruct
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::octet_string
};

const Entry<ItemInfo> _ApplicationLauncher_StopAppRequest[] = {
  { { ContextTag(0), "application", ItemType::kDefault }, 938 }, // ApplicationLauncher::ApplicationStruct
};

const Entry<ItemInfo> _ApplicationLauncher_HideAppRequest[] = {
  { { ContextTag(0), "application", ItemType::kDefault }, 938 }, // ApplicationLauncher::ApplicationStruct
};

const Entry<ItemInfo> _ApplicationLauncher_LauncherResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 944 }, // ApplicationLauncher::StatusEnum
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::octet_string
};

const Entry<ItemInfo> _ApplicationLauncher_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::StatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kAppNotAvailable", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::StatusEnum::kAppNotAvailable
  { { ConstantValueTag(0x2), "kSystemBusy", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::StatusEnum::kSystemBusy
};

const Entry<ItemInfo> _ApplicationLauncher_Feature[] = {
  { { ConstantValueTag(0x1), "kApplicationPlatform", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationLauncher::Feature::kApplicationPlatform
};

const Entry<ItemInfo> _ApplicationBasic[] = {
  { { AttributeTag(0), "vendorName", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::char_string
  { { AttributeTag(1), "vendorID", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::vendor_id
  { { AttributeTag(2), "applicationName", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::long_char_string
  { { AttributeTag(3), "productID", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::int16u
  { { AttributeTag(4), "application", ItemType::kDefault }, 947 }, // ApplicationBasic::ApplicationStruct
  { { AttributeTag(5), "status", ItemType::kEnum }, 948 }, // ApplicationBasic::ApplicationStatusEnum
  { { AttributeTag(6), "applicationVersion", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::char_string
  { { AttributeTag(7), "allowedVendorList", ItemType::kList }, 1 }, // ApplicationBasic::vendor_id[]
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ApplicationBasic::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ApplicationBasic::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ApplicationBasic::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ApplicationBasic::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::int16u
};

const Entry<ItemInfo> _ApplicationBasic_ApplicationStruct[] = {
  { { ContextTag(0), "catalogVendorID", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::int16u
  { { ContextTag(1), "applicationID", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::char_string
};

const Entry<ItemInfo> _ApplicationBasic_ApplicationStatusEnum[] = {
  { { ConstantValueTag(0x0), "kStopped", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::ApplicationStatusEnum::kStopped
  { { ConstantValueTag(0x1), "kActiveVisibleFocus", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::ApplicationStatusEnum::kActiveVisibleFocus
  { { ConstantValueTag(0x2), "kActiveHidden", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::ApplicationStatusEnum::kActiveHidden
  { { ConstantValueTag(0x3), "kActiveVisibleNotFocus", ItemType::kDefault }, kInvalidNodeIndex }, // ApplicationBasic::ApplicationStatusEnum::kActiveVisibleNotFocus
};

const Entry<ItemInfo> _AccountLogin[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // AccountLogin::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // AccountLogin::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // AccountLogin::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // AccountLogin::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::int16u
  { { EventTag(0), "LoggedOut", ItemType::kDefault }, 954 }, // AccountLogin::LoggedOut
  { { CommandTag(0), "GetSetupPIN", ItemType::kDefault }, 950 }, // AccountLogin::GetSetupPIN::GetSetupPINRequest
  { { CommandTag(2), "Login", ItemType::kDefault }, 952 }, // AccountLogin::Login::LoginRequest
  { { CommandTag(3), "Logout", ItemType::kDefault }, 953 }, // AccountLogin::Logout::LogoutRequest
  { { CommandTag(1), "GetSetupPINResponse", ItemType::kDefault }, 951 }, // AccountLogin::GetSetupPINResponse
};

const Entry<ItemInfo> _AccountLogin_GetSetupPINRequest[] = {
  { { ContextTag(0), "tempAccountIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::char_string
};

const Entry<ItemInfo> _AccountLogin_GetSetupPINResponse[] = {
  { { ContextTag(0), "setupPIN", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::char_string
};

const Entry<ItemInfo> _AccountLogin_LoginRequest[] = {
  { { ContextTag(0), "tempAccountIdentifier", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::char_string
  { { ContextTag(1), "setupPIN", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::char_string
  { { ContextTag(2), "node", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::node_id
};

const Entry<ItemInfo> _AccountLogin_LogoutRequest[] = {
  { { ContextTag(0), "node", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::node_id
};

const Entry<ItemInfo> _AccountLogin_LoggedOut[] = {
  { { ContextTag(0), "node", ItemType::kDefault }, kInvalidNodeIndex }, // AccountLogin::node_id
};

const Entry<ItemInfo> _ContentControl[] = {
  { { AttributeTag(0), "enabled", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::boolean
  { { AttributeTag(1), "onDemandRatings", ItemType::kList }, 963 }, // ContentControl::RatingNameStruct[]
  { { AttributeTag(2), "onDemandRatingThreshold", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
  { { AttributeTag(3), "scheduledContentRatings", ItemType::kList }, 963 }, // ContentControl::RatingNameStruct[]
  { { AttributeTag(4), "scheduledContentRatingThreshold", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
  { { AttributeTag(5), "screenDailyTime", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::elapsed_s
  { { AttributeTag(6), "remainingScreenTime", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::elapsed_s
  { { AttributeTag(7), "blockUnrated", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::boolean
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ContentControl::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ContentControl::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ContentControl::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ContentControl::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kBitmap }, 964 }, // ContentControl::Feature
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::int16u
  { { CommandTag(0), "UpdatePIN", ItemType::kDefault }, 957 }, // ContentControl::UpdatePIN::UpdatePINRequest
  { { CommandTag(1), "ResetPIN", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::ResetPIN::()
  { { CommandTag(3), "Enable", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::Enable::()
  { { CommandTag(4), "Disable", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::Disable::()
  { { CommandTag(5), "AddBonusTime", ItemType::kDefault }, 959 }, // ContentControl::AddBonusTime::AddBonusTimeRequest
  { { CommandTag(6), "SetScreenDailyTime", ItemType::kDefault }, 960 }, // ContentControl::SetScreenDailyTime::SetScreenDailyTimeRequest
  { { CommandTag(7), "BlockUnratedContent", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::BlockUnratedContent::()
  { { CommandTag(8), "UnblockUnratedContent", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::UnblockUnratedContent::()
  { { CommandTag(9), "SetOnDemandRatingThreshold", ItemType::kDefault }, 961 }, // ContentControl::SetOnDemandRatingThreshold::SetOnDemandRatingThresholdRequest
  { { CommandTag(10), "SetScheduledContentRatingThreshold", ItemType::kDefault }, 962 }, // ContentControl::SetScheduledContentRatingThreshold::SetScheduledContentRatingThresholdRequest
  { { CommandTag(2), "ResetPINResponse", ItemType::kDefault }, 958 }, // ContentControl::ResetPINResponse
};

const Entry<ItemInfo> _ContentControl_RatingNameStruct[] = {
  { { ContextTag(0), "ratingName", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
  { { ContextTag(1), "ratingNameDesc", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
};

const Entry<ItemInfo> _ContentControl_UpdatePINRequest[] = {
  { { ContextTag(0), "oldPIN", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
  { { ContextTag(1), "newPIN", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
};

const Entry<ItemInfo> _ContentControl_ResetPINResponse[] = {
  { { ContextTag(0), "PINCode", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
};

const Entry<ItemInfo> _ContentControl_AddBonusTimeRequest[] = {
  { { ContextTag(0), "PINCode", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
  { { ContextTag(1), "bonusTime", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::elapsed_s
};

const Entry<ItemInfo> _ContentControl_SetScreenDailyTimeRequest[] = {
  { { ContextTag(0), "screenTime", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::elapsed_s
};

const Entry<ItemInfo> _ContentControl_SetOnDemandRatingThresholdRequest[] = {
  { { ContextTag(0), "rating", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
};

const Entry<ItemInfo> _ContentControl_SetScheduledContentRatingThresholdRequest[] = {
  { { ContextTag(0), "rating", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::char_string
};

const Entry<ItemInfo> _ContentControl_RatingNameStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 956 }, // ContentControl_RatingNameStruct[]
};

const Entry<ItemInfo> _ContentControl_Feature[] = {
  { { ConstantValueTag(0x1), "kScreenTime", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::Feature::kScreenTime
  { { ConstantValueTag(0x2), "kPINManagement", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::Feature::kPINManagement
  { { ConstantValueTag(0x3), "kBlockUnrated", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::Feature::kBlockUnrated
  { { ConstantValueTag(0x4), "kOnDemandContentRating", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::Feature::kOnDemandContentRating
  { { ConstantValueTag(0x5), "kScheduledContentRating", ItemType::kDefault }, kInvalidNodeIndex }, // ContentControl::Feature::kScheduledContentRating
};

const Entry<ItemInfo> _ContentAppObserver[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ContentAppObserver::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ContentAppObserver::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ContentAppObserver::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ContentAppObserver::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // ContentAppObserver::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ContentAppObserver::int16u
  { { CommandTag(0), "ContentAppMessage", ItemType::kDefault }, 966 }, // ContentAppObserver::ContentAppMessage::ContentAppMessageRequest
  { { CommandTag(1), "ContentAppMessageResponse", ItemType::kDefault }, 967 }, // ContentAppObserver::ContentAppMessageResponse
};

const Entry<ItemInfo> _ContentAppObserver_ContentAppMessageRequest[] = {
  { { ContextTag(0), "data", ItemType::kDefault }, kInvalidNodeIndex }, // ContentAppObserver::char_string
  { { ContextTag(1), "encodingHint", ItemType::kDefault }, kInvalidNodeIndex }, // ContentAppObserver::char_string
};

const Entry<ItemInfo> _ContentAppObserver_ContentAppMessageResponse[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 968 }, // ContentAppObserver::StatusEnum
  { { ContextTag(1), "data", ItemType::kDefault }, kInvalidNodeIndex }, // ContentAppObserver::char_string
  { { ContextTag(2), "encodingHint", ItemType::kDefault }, kInvalidNodeIndex }, // ContentAppObserver::char_string
};

const Entry<ItemInfo> _ContentAppObserver_StatusEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // ContentAppObserver::StatusEnum::kSuccess
  { { ConstantValueTag(0x1), "kUnexpectedData", ItemType::kDefault }, kInvalidNodeIndex }, // ContentAppObserver::StatusEnum::kUnexpectedData
};

const Entry<ItemInfo> _ElectricalMeasurement[] = {
  { { AttributeTag(0), "measurementType", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::bitmap32
  { { AttributeTag(256), "dcVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(257), "dcVoltageMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(258), "dcVoltageMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(259), "dcCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(260), "dcCurrentMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(261), "dcCurrentMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(262), "dcPower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(263), "dcPowerMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(264), "dcPowerMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(512), "dcVoltageMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(513), "dcVoltageDivisor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(514), "dcCurrentMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(515), "dcCurrentDivisor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(516), "dcPowerMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(517), "dcPowerDivisor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(768), "acFrequency", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(769), "acFrequencyMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(770), "acFrequencyMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(771), "neutralCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(772), "totalActivePower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int32s
  { { AttributeTag(773), "totalReactivePower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int32s
  { { AttributeTag(774), "totalApparentPower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int32u
  { { AttributeTag(775), "measured1stHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(776), "measured3rdHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(777), "measured5thHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(778), "measured7thHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(779), "measured9thHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(780), "measured11thHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(781), "measuredPhase1stHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(782), "measuredPhase3rdHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(783), "measuredPhase5thHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(784), "measuredPhase7thHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(785), "measuredPhase9thHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(786), "measuredPhase11thHarmonicCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1024), "acFrequencyMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1025), "acFrequencyDivisor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1026), "powerMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int32u
  { { AttributeTag(1027), "powerDivisor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int32u
  { { AttributeTag(1028), "harmonicCurrentMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int8s
  { { AttributeTag(1029), "phaseHarmonicCurrentMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int8s
  { { AttributeTag(1280), "instantaneousVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1281), "instantaneousLineCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1282), "instantaneousActiveCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1283), "instantaneousReactiveCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1284), "instantaneousPower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1285), "rmsVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1286), "rmsVoltageMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1287), "rmsVoltageMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1288), "rmsCurrent", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1289), "rmsCurrentMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1290), "rmsCurrentMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1291), "activePower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1292), "activePowerMin", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1293), "activePowerMax", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1294), "reactivePower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1295), "apparentPower", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1296), "powerFactor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int8s
  { { AttributeTag(1297), "averageRmsVoltageMeasurementPeriod", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1299), "averageRmsUnderVoltageCounter", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1300), "rmsExtremeOverVoltagePeriod", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1301), "rmsExtremeUnderVoltagePeriod", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1302), "rmsVoltageSagPeriod", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1303), "rmsVoltageSwellPeriod", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1536), "acVoltageMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1537), "acVoltageDivisor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1538), "acCurrentMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1539), "acCurrentDivisor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1540), "acPowerMultiplier", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1541), "acPowerDivisor", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(1792), "overloadAlarmsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::bitmap8
  { { AttributeTag(1793), "voltageOverload", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(1794), "currentOverload", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2048), "acOverloadAlarmsMask", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::bitmap16
  { { AttributeTag(2049), "acVoltageOverload", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2050), "acCurrentOverload", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2051), "acActivePowerOverload", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2052), "acReactivePowerOverload", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2053), "averageRmsOverVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2054), "averageRmsUnderVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2055), "rmsExtremeOverVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2056), "rmsExtremeUnderVoltage", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2057), "rmsVoltageSag", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2058), "rmsVoltageSwell", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2305), "lineCurrentPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2306), "activeCurrentPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2307), "reactiveCurrentPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2309), "rmsVoltagePhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2310), "rmsVoltageMinPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2311), "rmsVoltageMaxPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2312), "rmsCurrentPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2313), "rmsCurrentMinPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2314), "rmsCurrentMaxPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2315), "activePowerPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2316), "activePowerMinPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2317), "activePowerMaxPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2318), "reactivePowerPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2319), "apparentPowerPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2320), "powerFactorPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int8s
  { { AttributeTag(2321), "averageRmsVoltageMeasurementPeriodPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2322), "averageRmsOverVoltageCounterPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2323), "averageRmsUnderVoltageCounterPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2324), "rmsExtremeOverVoltagePeriodPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2325), "rmsExtremeUnderVoltagePeriodPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2326), "rmsVoltageSagPeriodPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2327), "rmsVoltageSwellPeriodPhaseB", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2561), "lineCurrentPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2562), "activeCurrentPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2563), "reactiveCurrentPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2565), "rmsVoltagePhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2566), "rmsVoltageMinPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2567), "rmsVoltageMaxPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2568), "rmsCurrentPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2569), "rmsCurrentMinPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2570), "rmsCurrentMaxPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2571), "activePowerPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2572), "activePowerMinPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2573), "activePowerMaxPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2574), "reactivePowerPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16s
  { { AttributeTag(2575), "apparentPowerPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2576), "powerFactorPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int8s
  { { AttributeTag(2577), "averageRmsVoltageMeasurementPeriodPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2578), "averageRmsOverVoltageCounterPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2579), "averageRmsUnderVoltageCounterPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2580), "rmsExtremeOverVoltagePeriodPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2581), "rmsExtremeUnderVoltagePeriodPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2582), "rmsVoltageSagPeriodPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(2583), "rmsVoltageSwellPeriodPhaseC", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // ElectricalMeasurement::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // ElectricalMeasurement::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // ElectricalMeasurement::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // ElectricalMeasurement::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { CommandTag(0), "GetProfileInfoCommand", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::GetProfileInfoCommand::()
  { { CommandTag(1), "GetMeasurementProfileCommand", ItemType::kDefault }, 972 }, // ElectricalMeasurement::GetMeasurementProfileCommand::GetMeasurementProfileCommandRequest
  { { CommandTag(0), "GetProfileInfoResponseCommand", ItemType::kDefault }, 970 }, // ElectricalMeasurement::GetProfileInfoResponseCommand
  { { CommandTag(1), "GetMeasurementProfileResponseCommand", ItemType::kDefault }, 971 }, // ElectricalMeasurement::GetMeasurementProfileResponseCommand
};

const Entry<ItemInfo> _ElectricalMeasurement_GetProfileInfoResponseCommand[] = {
  { { ContextTag(0), "profileCount", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int8u
  { { ContextTag(1), "profileIntervalPeriod", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::enum8
  { { ContextTag(2), "maxNumberOfIntervals", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int8u
  { { ContextTag(3), "listOfAttributes", ItemType::kList }, 1 }, // ElectricalMeasurement::int16u[]
};

const Entry<ItemInfo> _ElectricalMeasurement_GetMeasurementProfileResponseCommand[] = {
  { { ContextTag(0), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int32u
  { { ContextTag(1), "status", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::enum8
  { { ContextTag(2), "profileIntervalPeriod", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::enum8
  { { ContextTag(3), "numberOfIntervalsDelivered", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int8u
  { { ContextTag(4), "attributeId", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { ContextTag(5), "intervals", ItemType::kList }, 1 }, // ElectricalMeasurement::int8u[]
};

const Entry<ItemInfo> _ElectricalMeasurement_GetMeasurementProfileCommandRequest[] = {
  { { ContextTag(0), "attributeId", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int16u
  { { ContextTag(1), "startTime", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::int32u
  { { ContextTag(2), "numberOfIntervals", ItemType::kDefault }, kInvalidNodeIndex }, // ElectricalMeasurement::enum8
};

const Entry<ItemInfo> _UnitTesting[] = {
  { { AttributeTag(0), "boolean", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { AttributeTag(1), "bitmap8", ItemType::kBitmap }, 1025 }, // UnitTesting::Bitmap8MaskMap
  { { AttributeTag(2), "bitmap16", ItemType::kBitmap }, 1022 }, // UnitTesting::Bitmap16MaskMap
  { { AttributeTag(3), "bitmap32", ItemType::kBitmap }, 1023 }, // UnitTesting::Bitmap32MaskMap
  { { AttributeTag(4), "bitmap64", ItemType::kBitmap }, 1024 }, // UnitTesting::Bitmap64MaskMap
  { { AttributeTag(5), "int8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { AttributeTag(6), "int16u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { AttributeTag(7), "int24u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int24u
  { { AttributeTag(8), "int32u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int32u
  { { AttributeTag(9), "int40u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int40u
  { { AttributeTag(10), "int48u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int48u
  { { AttributeTag(11), "int56u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int56u
  { { AttributeTag(12), "int64u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int64u
  { { AttributeTag(13), "int8s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8s
  { { AttributeTag(14), "int16s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16s
  { { AttributeTag(15), "int24s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int24s
  { { AttributeTag(16), "int32s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int32s
  { { AttributeTag(17), "int40s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int40s
  { { AttributeTag(18), "int48s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int48s
  { { AttributeTag(19), "int56s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int56s
  { { AttributeTag(20), "int64s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int64s
  { { AttributeTag(21), "enum8", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::enum8
  { { AttributeTag(22), "enum16", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::enum16
  { { AttributeTag(23), "floatSingle", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::single
  { { AttributeTag(24), "floatDouble", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::double
  { { AttributeTag(25), "octetString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::octet_string
  { { AttributeTag(26), "listInt8u", ItemType::kList }, 1 }, // UnitTesting::int8u[]
  { { AttributeTag(27), "listOctetString", ItemType::kList }, 1 }, // UnitTesting::octet_string[]
  { { AttributeTag(28), "listStructOctetString", ItemType::kList }, 1016 }, // UnitTesting::TestListStructOctet[]
  { { AttributeTag(29), "longOctetString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::long_octet_string
  { { AttributeTag(30), "charString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { AttributeTag(31), "longCharString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::long_char_string
  { { AttributeTag(32), "epochUs", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::epoch_us
  { { AttributeTag(33), "epochS", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::epoch_s
  { { AttributeTag(34), "vendorId", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::vendor_id
  { { AttributeTag(35), "listNullablesAndOptionalsStruct", ItemType::kList }, 1015 }, // UnitTesting::NullablesAndOptionalsStruct[]
  { { AttributeTag(36), "enumAttr", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
  { { AttributeTag(37), "structAttr", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { AttributeTag(38), "rangeRestrictedInt8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { AttributeTag(39), "rangeRestrictedInt8s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8s
  { { AttributeTag(40), "rangeRestrictedInt16u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { AttributeTag(41), "rangeRestrictedInt16s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16s
  { { AttributeTag(42), "listLongOctetString", ItemType::kList }, 1 }, // UnitTesting::long_octet_string[]
  { { AttributeTag(43), "listFabricScoped", ItemType::kList }, 1017 }, // UnitTesting::TestFabricScoped[]
  { { AttributeTag(48), "timedWriteBoolean", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { AttributeTag(49), "generalErrorBoolean", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { AttributeTag(50), "clusterErrorBoolean", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { AttributeTag(255), "unsupported", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { AttributeTag(16384), "nullableBoolean", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { AttributeTag(16385), "nullableBitmap8", ItemType::kBitmap }, 1025 }, // UnitTesting::Bitmap8MaskMap
  { { AttributeTag(16386), "nullableBitmap16", ItemType::kBitmap }, 1022 }, // UnitTesting::Bitmap16MaskMap
  { { AttributeTag(16387), "nullableBitmap32", ItemType::kBitmap }, 1023 }, // UnitTesting::Bitmap32MaskMap
  { { AttributeTag(16388), "nullableBitmap64", ItemType::kBitmap }, 1024 }, // UnitTesting::Bitmap64MaskMap
  { { AttributeTag(16389), "nullableInt8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { AttributeTag(16390), "nullableInt16u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { AttributeTag(16391), "nullableInt24u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int24u
  { { AttributeTag(16392), "nullableInt32u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int32u
  { { AttributeTag(16393), "nullableInt40u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int40u
  { { AttributeTag(16394), "nullableInt48u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int48u
  { { AttributeTag(16395), "nullableInt56u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int56u
  { { AttributeTag(16396), "nullableInt64u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int64u
  { { AttributeTag(16397), "nullableInt8s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8s
  { { AttributeTag(16398), "nullableInt16s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16s
  { { AttributeTag(16399), "nullableInt24s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int24s
  { { AttributeTag(16400), "nullableInt32s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int32s
  { { AttributeTag(16401), "nullableInt40s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int40s
  { { AttributeTag(16402), "nullableInt48s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int48s
  { { AttributeTag(16403), "nullableInt56s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int56s
  { { AttributeTag(16404), "nullableInt64s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int64s
  { { AttributeTag(16405), "nullableEnum8", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::enum8
  { { AttributeTag(16406), "nullableEnum16", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::enum16
  { { AttributeTag(16407), "nullableFloatSingle", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::single
  { { AttributeTag(16408), "nullableFloatDouble", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::double
  { { AttributeTag(16409), "nullableOctetString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::octet_string
  { { AttributeTag(16414), "nullableCharString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { AttributeTag(16420), "nullableEnumAttr", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
  { { AttributeTag(16421), "nullableStruct", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { AttributeTag(16422), "nullableRangeRestrictedInt8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { AttributeTag(16423), "nullableRangeRestrictedInt8s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8s
  { { AttributeTag(16424), "nullableRangeRestrictedInt16u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { AttributeTag(16425), "nullableRangeRestrictedInt16s", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16s
  { { AttributeTag(16426), "writeOnlyInt8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // UnitTesting::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // UnitTesting::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // UnitTesting::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // UnitTesting::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { EventTag(1), "TestEvent", ItemType::kDefault }, 1013 }, // UnitTesting::TestEvent
  { { EventTag(2), "TestFabricScopedEvent", ItemType::kDefault }, 1014 }, // UnitTesting::TestFabricScopedEvent
  { { CommandTag(0), "Test", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Test::()
  { { CommandTag(1), "TestNotHandled", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::TestNotHandled::()
  { { CommandTag(2), "TestSpecific", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::TestSpecific::()
  { { CommandTag(3), "TestUnknownCommand", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::TestUnknownCommand::()
  { { CommandTag(4), "TestAddArguments", ItemType::kDefault }, 985 }, // UnitTesting::TestAddArguments::TestAddArgumentsRequest
  { { CommandTag(5), "TestSimpleArgumentRequest", ItemType::kDefault }, 987 }, // UnitTesting::TestSimpleArgumentRequest::TestSimpleArgumentRequestRequest
  { { CommandTag(6), "TestStructArrayArgumentRequest", ItemType::kDefault }, 989 }, // UnitTesting::TestStructArrayArgumentRequest::TestStructArrayArgumentRequestRequest
  { { CommandTag(7), "TestStructArgumentRequest", ItemType::kDefault }, 991 }, // UnitTesting::TestStructArgumentRequest::TestStructArgumentRequestRequest
  { { CommandTag(8), "TestNestedStructArgumentRequest", ItemType::kDefault }, 993 }, // UnitTesting::TestNestedStructArgumentRequest::TestNestedStructArgumentRequestRequest
  { { CommandTag(9), "TestListStructArgumentRequest", ItemType::kDefault }, 995 }, // UnitTesting::TestListStructArgumentRequest::TestListStructArgumentRequestRequest
  { { CommandTag(10), "TestListInt8UArgumentRequest", ItemType::kDefault }, 997 }, // UnitTesting::TestListInt8UArgumentRequest::TestListInt8UArgumentRequestRequest
  { { CommandTag(11), "TestNestedStructListArgumentRequest", ItemType::kDefault }, 999 }, // UnitTesting::TestNestedStructListArgumentRequest::TestNestedStructListArgumentRequestRequest
  { { CommandTag(12), "TestListNestedStructListArgumentRequest", ItemType::kDefault }, 1001 }, // UnitTesting::TestListNestedStructListArgumentRequest::TestListNestedStructListArgumentRequestRequest
  { { CommandTag(13), "TestListInt8UReverseRequest", ItemType::kDefault }, 1003 }, // UnitTesting::TestListInt8UReverseRequest::TestListInt8UReverseRequestRequest
  { { CommandTag(14), "TestEnumsRequest", ItemType::kDefault }, 1004 }, // UnitTesting::TestEnumsRequest::TestEnumsRequestRequest
  { { CommandTag(15), "TestNullableOptionalRequest", ItemType::kDefault }, 1005 }, // UnitTesting::TestNullableOptionalRequest::TestNullableOptionalRequestRequest
  { { CommandTag(16), "TestComplexNullableOptionalRequest", ItemType::kDefault }, 1006 }, // UnitTesting::TestComplexNullableOptionalRequest::TestComplexNullableOptionalRequestRequest
  { { CommandTag(17), "SimpleStructEchoRequest", ItemType::kDefault }, 1007 }, // UnitTesting::SimpleStructEchoRequest::SimpleStructEchoRequestRequest
  { { CommandTag(18), "TimedInvokeRequest", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::TimedInvokeRequest::()
  { { CommandTag(19), "TestSimpleOptionalArgumentRequest", ItemType::kDefault }, 1008 }, // UnitTesting::TestSimpleOptionalArgumentRequest::TestSimpleOptionalArgumentRequestRequest
  { { CommandTag(20), "TestEmitTestEventRequest", ItemType::kDefault }, 1009 }, // UnitTesting::TestEmitTestEventRequest::TestEmitTestEventRequestRequest
  { { CommandTag(21), "TestEmitTestFabricScopedEventRequest", ItemType::kDefault }, 1010 }, // UnitTesting::TestEmitTestFabricScopedEventRequest::TestEmitTestFabricScopedEventRequestRequest
  { { CommandTag(22), "TestBatchHelperRequest", ItemType::kDefault }, 1011 }, // UnitTesting::TestBatchHelperRequest::TestBatchHelperRequestRequest
  { { CommandTag(23), "TestSecondBatchHelperRequest", ItemType::kDefault }, 1012 }, // UnitTesting::TestSecondBatchHelperRequest::TestSecondBatchHelperRequestRequest
  { { CommandTag(0), "TestSpecificResponse", ItemType::kDefault }, 981 }, // UnitTesting::TestSpecificResponse
  { { CommandTag(1), "TestAddArgumentsResponse", ItemType::kDefault }, 982 }, // UnitTesting::TestAddArgumentsResponse
  { { CommandTag(2), "TestSimpleArgumentResponse", ItemType::kDefault }, 983 }, // UnitTesting::TestSimpleArgumentResponse
  { { CommandTag(3), "TestStructArrayArgumentResponse", ItemType::kDefault }, 984 }, // UnitTesting::TestStructArrayArgumentResponse
  { { CommandTag(4), "TestListInt8UReverseResponse", ItemType::kDefault }, 986 }, // UnitTesting::TestListInt8UReverseResponse
  { { CommandTag(5), "TestEnumsResponse", ItemType::kDefault }, 988 }, // UnitTesting::TestEnumsResponse
  { { CommandTag(6), "TestNullableOptionalResponse", ItemType::kDefault }, 990 }, // UnitTesting::TestNullableOptionalResponse
  { { CommandTag(7), "TestComplexNullableOptionalResponse", ItemType::kDefault }, 992 }, // UnitTesting::TestComplexNullableOptionalResponse
  { { CommandTag(8), "BooleanResponse", ItemType::kDefault }, 994 }, // UnitTesting::BooleanResponse
  { { CommandTag(9), "SimpleStructResponse", ItemType::kDefault }, 996 }, // UnitTesting::SimpleStructResponse
  { { CommandTag(10), "TestEmitTestEventResponse", ItemType::kDefault }, 998 }, // UnitTesting::TestEmitTestEventResponse
  { { CommandTag(11), "TestEmitTestFabricScopedEventResponse", ItemType::kDefault }, 1000 }, // UnitTesting::TestEmitTestFabricScopedEventResponse
  { { CommandTag(12), "TestBatchHelperResponse", ItemType::kDefault }, 1002 }, // UnitTesting::TestBatchHelperResponse
};

const Entry<ItemInfo> _UnitTesting_SimpleStruct[] = {
  { { ContextTag(0), "a", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(1), "b", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(2), "c", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
  { { ContextTag(3), "d", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::octet_string
  { { ContextTag(4), "e", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(5), "f", ItemType::kBitmap }, 1026 }, // UnitTesting::SimpleBitmap
  { { ContextTag(6), "g", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::single
  { { ContextTag(7), "h", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::double
};

const Entry<ItemInfo> _UnitTesting_TestFabricScoped[] = {
  { { ContextTag(1), "fabricSensitiveInt8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(2), "optionalFabricSensitiveInt8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(3), "nullableFabricSensitiveInt8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(4), "nullableOptionalFabricSensitiveInt8u", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(5), "fabricSensitiveCharString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(6), "fabricSensitiveStruct", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(7), "fabricSensitiveInt8uList", ItemType::kList }, 1 }, // UnitTesting::int8u[]
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::fabric_idx
};

const Entry<ItemInfo> _UnitTesting_NullablesAndOptionalsStruct[] = {
  { { ContextTag(0), "nullableInt", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(1), "optionalInt", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(2), "nullableOptionalInt", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(3), "nullableString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(4), "optionalString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(5), "nullableOptionalString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(6), "nullableStruct", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(7), "optionalStruct", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(8), "nullableOptionalStruct", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(9), "nullableList", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
  { { ContextTag(10), "optionalList", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
  { { ContextTag(11), "nullableOptionalList", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
};

const Entry<ItemInfo> _UnitTesting_NestedStruct[] = {
  { { ContextTag(0), "a", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(1), "b", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(2), "c", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
};

const Entry<ItemInfo> _UnitTesting_NestedStructList[] = {
  { { ContextTag(0), "a", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(1), "b", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(2), "c", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(3), "d", ItemType::kList }, 1018 }, // UnitTesting::SimpleStruct[]
  { { ContextTag(4), "e", ItemType::kList }, 1 }, // UnitTesting::int32u[]
  { { ContextTag(5), "f", ItemType::kList }, 1 }, // UnitTesting::octet_string[]
  { { ContextTag(6), "g", ItemType::kList }, 1 }, // UnitTesting::int8u[]
};

const Entry<ItemInfo> _UnitTesting_DoubleNestedStructList[] = {
  { { ContextTag(0), "a", ItemType::kList }, 1020 }, // UnitTesting::NestedStructList[]
};

const Entry<ItemInfo> _UnitTesting_TestListStructOctet[] = {
  { { ContextTag(0), "member1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int64u
  { { ContextTag(1), "member2", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::octet_string
};

const Entry<ItemInfo> _UnitTesting_TestSpecificResponse[] = {
  { { ContextTag(0), "returnValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
};

const Entry<ItemInfo> _UnitTesting_TestAddArgumentsResponse[] = {
  { { ContextTag(0), "returnValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
};

const Entry<ItemInfo> _UnitTesting_TestSimpleArgumentResponse[] = {
  { { ContextTag(0), "returnValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
};

const Entry<ItemInfo> _UnitTesting_TestStructArrayArgumentResponse[] = {
  { { ContextTag(0), "arg1", ItemType::kList }, 1020 }, // UnitTesting::NestedStructList[]
  { { ContextTag(1), "arg2", ItemType::kList }, 1018 }, // UnitTesting::SimpleStruct[]
  { { ContextTag(2), "arg3", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
  { { ContextTag(3), "arg4", ItemType::kList }, 1 }, // UnitTesting::boolean[]
  { { ContextTag(4), "arg5", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
  { { ContextTag(5), "arg6", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
};

const Entry<ItemInfo> _UnitTesting_TestAddArgumentsRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(1), "arg2", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
};

const Entry<ItemInfo> _UnitTesting_TestListInt8UReverseResponse[] = {
  { { ContextTag(0), "arg1", ItemType::kList }, 1 }, // UnitTesting::int8u[]
};

const Entry<ItemInfo> _UnitTesting_TestSimpleArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
};

const Entry<ItemInfo> _UnitTesting_TestEnumsResponse[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::vendor_id
  { { ContextTag(1), "arg2", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
};

const Entry<ItemInfo> _UnitTesting_TestStructArrayArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kList }, 1020 }, // UnitTesting::NestedStructList[]
  { { ContextTag(1), "arg2", ItemType::kList }, 1018 }, // UnitTesting::SimpleStruct[]
  { { ContextTag(2), "arg3", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
  { { ContextTag(3), "arg4", ItemType::kList }, 1 }, // UnitTesting::boolean[]
  { { ContextTag(4), "arg5", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
  { { ContextTag(5), "arg6", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
};

const Entry<ItemInfo> _UnitTesting_TestNullableOptionalResponse[] = {
  { { ContextTag(0), "wasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(1), "wasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(2), "value", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(3), "originalValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
};

const Entry<ItemInfo> _UnitTesting_TestStructArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
};

const Entry<ItemInfo> _UnitTesting_TestComplexNullableOptionalResponse[] = {
  { { ContextTag(0), "nullableIntWasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(1), "nullableIntValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(2), "optionalIntWasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(3), "optionalIntValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(4), "nullableOptionalIntWasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(5), "nullableOptionalIntWasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(6), "nullableOptionalIntValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(7), "nullableStringWasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(8), "nullableStringValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(9), "optionalStringWasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(10), "optionalStringValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(11), "nullableOptionalStringWasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(12), "nullableOptionalStringWasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(13), "nullableOptionalStringValue", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(14), "nullableStructWasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(15), "nullableStructValue", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(16), "optionalStructWasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(17), "optionalStructValue", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(18), "nullableOptionalStructWasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(19), "nullableOptionalStructWasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(20), "nullableOptionalStructValue", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(21), "nullableListWasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(22), "nullableListValue", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
  { { ContextTag(23), "optionalListWasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(24), "optionalListValue", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
  { { ContextTag(25), "nullableOptionalListWasPresent", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(26), "nullableOptionalListWasNull", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(27), "nullableOptionalListValue", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
};

const Entry<ItemInfo> _UnitTesting_TestNestedStructArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, 977 }, // UnitTesting::NestedStruct
};

const Entry<ItemInfo> _UnitTesting_BooleanResponse[] = {
  { { ContextTag(0), "value", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
};

const Entry<ItemInfo> _UnitTesting_TestListStructArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kList }, 1018 }, // UnitTesting::SimpleStruct[]
};

const Entry<ItemInfo> _UnitTesting_SimpleStructResponse[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
};

const Entry<ItemInfo> _UnitTesting_TestListInt8UArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kList }, 1 }, // UnitTesting::int8u[]
};

const Entry<ItemInfo> _UnitTesting_TestEmitTestEventResponse[] = {
  { { ContextTag(0), "value", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int64u
};

const Entry<ItemInfo> _UnitTesting_TestNestedStructListArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, 978 }, // UnitTesting::NestedStructList
};

const Entry<ItemInfo> _UnitTesting_TestEmitTestFabricScopedEventResponse[] = {
  { { ContextTag(0), "value", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int64u
};

const Entry<ItemInfo> _UnitTesting_TestListNestedStructListArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kList }, 1020 }, // UnitTesting::NestedStructList[]
};

const Entry<ItemInfo> _UnitTesting_TestBatchHelperResponse[] = {
  { { ContextTag(0), "buffer", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::octet_string
};

const Entry<ItemInfo> _UnitTesting_TestListInt8UReverseRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kList }, 1 }, // UnitTesting::int8u[]
};

const Entry<ItemInfo> _UnitTesting_TestEnumsRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::vendor_id
  { { ContextTag(1), "arg2", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
};

const Entry<ItemInfo> _UnitTesting_TestNullableOptionalRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
};

const Entry<ItemInfo> _UnitTesting_TestComplexNullableOptionalRequestRequest[] = {
  { { ContextTag(0), "nullableInt", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(1), "optionalInt", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(2), "nullableOptionalInt", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(3), "nullableString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(4), "optionalString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(5), "nullableOptionalString", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::char_string
  { { ContextTag(6), "nullableStruct", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(7), "optionalStruct", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(8), "nullableOptionalStruct", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(9), "nullableList", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
  { { ContextTag(10), "optionalList", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
  { { ContextTag(11), "nullableOptionalList", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
};

const Entry<ItemInfo> _UnitTesting_SimpleStructEchoRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
};

const Entry<ItemInfo> _UnitTesting_TestSimpleOptionalArgumentRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
};

const Entry<ItemInfo> _UnitTesting_TestEmitTestEventRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(1), "arg2", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
  { { ContextTag(2), "arg3", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
};

const Entry<ItemInfo> _UnitTesting_TestEmitTestFabricScopedEventRequestRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
};

const Entry<ItemInfo> _UnitTesting_TestBatchHelperRequestRequest[] = {
  { { ContextTag(0), "sleepBeforeResponseTimeMs", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(1), "sizeOfResponseBuffer", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(2), "fillCharacter", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
};

const Entry<ItemInfo> _UnitTesting_TestSecondBatchHelperRequestRequest[] = {
  { { ContextTag(0), "sleepBeforeResponseTimeMs", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(1), "sizeOfResponseBuffer", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int16u
  { { ContextTag(2), "fillCharacter", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
};

const Entry<ItemInfo> _UnitTesting_TestEvent[] = {
  { { ContextTag(1), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::int8u
  { { ContextTag(2), "arg2", ItemType::kEnum }, 1021 }, // UnitTesting::SimpleEnum
  { { ContextTag(3), "arg3", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::boolean
  { { ContextTag(4), "arg4", ItemType::kDefault }, 974 }, // UnitTesting::SimpleStruct
  { { ContextTag(5), "arg5", ItemType::kList }, 1018 }, // UnitTesting::SimpleStruct[]
  { { ContextTag(6), "arg6", ItemType::kList }, 1019 }, // UnitTesting::SimpleEnum[]
};

const Entry<ItemInfo> _UnitTesting_TestFabricScopedEvent[] = {
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::fabric_idx
};

const Entry<ItemInfo> _UnitTesting_NullablesAndOptionalsStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 976 }, // UnitTesting_NullablesAndOptionalsStruct[]
};

const Entry<ItemInfo> _UnitTesting_TestListStructOctet_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 980 }, // UnitTesting_TestListStructOctet[]
};

const Entry<ItemInfo> _UnitTesting_TestFabricScoped_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 975 }, // UnitTesting_TestFabricScoped[]
};

const Entry<ItemInfo> _UnitTesting_SimpleStruct_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 974 }, // UnitTesting_SimpleStruct[]
};

const Entry<ItemInfo> _UnitTesting_SimpleEnum_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 1021 }, // UnitTesting_SimpleEnum[]
};

const Entry<ItemInfo> _UnitTesting_NestedStructList_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 978 }, // UnitTesting_NestedStructList[]
};

const Entry<ItemInfo> _UnitTesting_SimpleEnum[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::SimpleEnum::kUnspecified
  { { ConstantValueTag(0x1), "kValueA", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::SimpleEnum::kValueA
  { { ConstantValueTag(0x2), "kValueB", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::SimpleEnum::kValueB
  { { ConstantValueTag(0x3), "kValueC", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::SimpleEnum::kValueC
};

const Entry<ItemInfo> _UnitTesting_Bitmap16MaskMap[] = {
  { { ConstantValueTag(0x1), "kMaskVal1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap16MaskMap::kMaskVal1
  { { ConstantValueTag(0x2), "kMaskVal2", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap16MaskMap::kMaskVal2
  { { ConstantValueTag(0x4), "kMaskVal3", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap16MaskMap::kMaskVal3
  { { ConstantValueTag(0x4000), "kMaskVal4", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap16MaskMap::kMaskVal4
};

const Entry<ItemInfo> _UnitTesting_Bitmap32MaskMap[] = {
  { { ConstantValueTag(0x1), "kMaskVal1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap32MaskMap::kMaskVal1
  { { ConstantValueTag(0x2), "kMaskVal2", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap32MaskMap::kMaskVal2
  { { ConstantValueTag(0x4), "kMaskVal3", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap32MaskMap::kMaskVal3
  { { ConstantValueTag(0x40000000), "kMaskVal4", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap32MaskMap::kMaskVal4
};

const Entry<ItemInfo> _UnitTesting_Bitmap64MaskMap[] = {
  { { ConstantValueTag(0x1), "kMaskVal1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap64MaskMap::kMaskVal1
  { { ConstantValueTag(0x2), "kMaskVal2", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap64MaskMap::kMaskVal2
  { { ConstantValueTag(0x4), "kMaskVal3", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap64MaskMap::kMaskVal3
  { { ConstantValueTag(0x4000000000000000), "kMaskVal4", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap64MaskMap::kMaskVal4
};

const Entry<ItemInfo> _UnitTesting_Bitmap8MaskMap[] = {
  { { ConstantValueTag(0x1), "kMaskVal1", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap8MaskMap::kMaskVal1
  { { ConstantValueTag(0x2), "kMaskVal2", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap8MaskMap::kMaskVal2
  { { ConstantValueTag(0x4), "kMaskVal3", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap8MaskMap::kMaskVal3
  { { ConstantValueTag(0x40), "kMaskVal4", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::Bitmap8MaskMap::kMaskVal4
};

const Entry<ItemInfo> _UnitTesting_SimpleBitmap[] = {
  { { ConstantValueTag(0x1), "kValueA", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::SimpleBitmap::kValueA
  { { ConstantValueTag(0x2), "kValueB", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::SimpleBitmap::kValueB
  { { ConstantValueTag(0x4), "kValueC", ItemType::kDefault }, kInvalidNodeIndex }, // UnitTesting::SimpleBitmap::kValueC
};

const Entry<ItemInfo> _FaultInjection[] = {
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // FaultInjection::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // FaultInjection::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // FaultInjection::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // FaultInjection::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::int16u
  { { CommandTag(0), "FailAtFault", ItemType::kDefault }, 1028 }, // FaultInjection::FailAtFault::FailAtFaultRequest
  { { CommandTag(1), "FailRandomlyAtFault", ItemType::kDefault }, 1029 }, // FaultInjection::FailRandomlyAtFault::FailRandomlyAtFaultRequest
};

const Entry<ItemInfo> _FaultInjection_FailAtFaultRequest[] = {
  { { ContextTag(0), "type", ItemType::kEnum }, 1030 }, // FaultInjection::FaultType
  { { ContextTag(1), "id", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::int32u
  { { ContextTag(2), "numCallsToSkip", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::int32u
  { { ContextTag(3), "numCallsToFail", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::int32u
  { { ContextTag(4), "takeMutex", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::boolean
};

const Entry<ItemInfo> _FaultInjection_FailRandomlyAtFaultRequest[] = {
  { { ContextTag(0), "type", ItemType::kEnum }, 1030 }, // FaultInjection::FaultType
  { { ContextTag(1), "id", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::int32u
  { { ContextTag(2), "percentage", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::int8u
};

const Entry<ItemInfo> _FaultInjection_FaultType[] = {
  { { ConstantValueTag(0x0), "kUnspecified", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::FaultType::kUnspecified
  { { ConstantValueTag(0x1), "kSystemFault", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::FaultType::kSystemFault
  { { ConstantValueTag(0x2), "kInetFault", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::FaultType::kInetFault
  { { ConstantValueTag(0x3), "kChipFault", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::FaultType::kChipFault
  { { ConstantValueTag(0x4), "kCertFault", ItemType::kDefault }, kInvalidNodeIndex }, // FaultInjection::FaultType::kCertFault
};

const Entry<ItemInfo> _SampleMei[] = {
  { { AttributeTag(0), "flipFlop", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::boolean
  { { AttributeTag(65528), "generatedCommandList", ItemType::kList }, 1 }, // SampleMei::command_id[]
  { { AttributeTag(65529), "acceptedCommandList", ItemType::kList }, 1 }, // SampleMei::command_id[]
  { { AttributeTag(65530), "eventList", ItemType::kList }, 1 }, // SampleMei::event_id[]
  { { AttributeTag(65531), "attributeList", ItemType::kList }, 1 }, // SampleMei::attrib_id[]
  { { AttributeTag(65532), "featureMap", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::bitmap32
  { { AttributeTag(65533), "clusterRevision", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::int16u
  { { EventTag(0), "PingCountEvent", ItemType::kDefault }, 1034 }, // SampleMei::PingCountEvent
  { { CommandTag(0), "Ping", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::Ping::()
  { { CommandTag(2), "AddArguments", ItemType::kDefault }, 1033 }, // SampleMei::AddArguments::AddArgumentsRequest
  { { CommandTag(1), "AddArgumentsResponse", ItemType::kDefault }, 1032 }, // SampleMei::AddArgumentsResponse
};

const Entry<ItemInfo> _SampleMei_AddArgumentsResponse[] = {
  { { ContextTag(0), "returnValue", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::int8u
};

const Entry<ItemInfo> _SampleMei_AddArgumentsRequest[] = {
  { { ContextTag(0), "arg1", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::int8u
  { { ContextTag(1), "arg2", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::int8u
};

const Entry<ItemInfo> _SampleMei_PingCountEvent[] = {
  { { ContextTag(1), "count", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::int32u
  { { ContextTag(254), "fabricIndex", ItemType::kDefault }, kInvalidNodeIndex }, // SampleMei::fabric_idx
};

const Entry<ItemInfo> _all_clusters[] = {
  { { ClusterTag(0x03), "Identify", ItemType::kDefault }, 2 },
  { { ClusterTag(0x04), "Groups", ItemType::kDefault }, 8 },
  { { ClusterTag(0x05), "Scenes", ItemType::kDefault }, 20 },
  { { ClusterTag(0x06), "OnOff", ItemType::kDefault }, 49 },
  { { ClusterTag(0x07), "OnOffSwitchConfiguration", ItemType::kDefault }, 58 },
  { { ClusterTag(0x08), "LevelControl", ItemType::kDefault }, 59 },
  { { ClusterTag(0x0F), "BinaryInputBasic", ItemType::kDefault }, 73 },
  { { ClusterTag(0x1C), "PulseWidthModulation", ItemType::kDefault }, 74 },
  { { ClusterTag(0x1D), "Descriptor", ItemType::kDefault }, 75 },
  { { ClusterTag(0x1E), "Binding", ItemType::kDefault }, 81 },
  { { ClusterTag(0x1F), "AccessControl", ItemType::kDefault }, 84 },
  { { ClusterTag(0x25), "Actions", ItemType::kDefault }, 96 },
  { { ClusterTag(0x28), "BasicInformation", ItemType::kDefault }, 120 },
  { { ClusterTag(0x29), "OtaSoftwareUpdateProvider", ItemType::kDefault }, 128 },
  { { ClusterTag(0x2A), "OtaSoftwareUpdateRequestor", ItemType::kDefault }, 138 },
  { { ClusterTag(0x2B), "LocalizationConfiguration", ItemType::kDefault }, 148 },
  { { ClusterTag(0x2C), "TimeFormatLocalization", ItemType::kDefault }, 149 },
  { { ClusterTag(0x2D), "UnitLocalization", ItemType::kDefault }, 154 },
  { { ClusterTag(0x2E), "PowerSourceConfiguration", ItemType::kDefault }, 157 },
  { { ClusterTag(0x2F), "PowerSource", ItemType::kDefault }, 158 },
  { { ClusterTag(0x30), "GeneralCommissioning", ItemType::kDefault }, 179 },
  { { ClusterTag(0x31), "NetworkCommissioning", ItemType::kDefault }, 188 },
  { { ClusterTag(0x32), "DiagnosticLogs", ItemType::kDefault }, 212 },
  { { ClusterTag(0x33), "GeneralDiagnostics", ItemType::kDefault }, 218 },
  { { ClusterTag(0x34), "SoftwareDiagnostics", ItemType::kDefault }, 235 },
  { { ClusterTag(0x35), "ThreadNetworkDiagnostics", ItemType::kDefault }, 240 },
  { { ClusterTag(0x36), "WiFiNetworkDiagnostics", ItemType::kDefault }, 254 },
  { { ClusterTag(0x37), "EthernetNetworkDiagnostics", ItemType::kDefault }, 263 },
  { { ClusterTag(0x38), "TimeSynchronization", ItemType::kDefault }, 266 },
  { { ClusterTag(0x39), "BridgedDeviceBasicInformation", ItemType::kDefault }, 286 },
  { { ClusterTag(0x3B), "Switch", ItemType::kDefault }, 292 },
  { { ClusterTag(0x3C), "AdministratorCommissioning", ItemType::kDefault }, 301 },
  { { ClusterTag(0x3E), "OperationalCredentials", ItemType::kDefault }, 307 },
  { { ClusterTag(0x3F), "GroupKeyManagement", ItemType::kDefault }, 326 },
  { { ClusterTag(0x40), "FixedLabel", ItemType::kDefault }, 339 },
  { { ClusterTag(0x41), "UserLabel", ItemType::kDefault }, 342 },
  { { ClusterTag(0x42), "ProxyConfiguration", ItemType::kDefault }, 345 },
  { { ClusterTag(0x43), "ProxyDiscovery", ItemType::kDefault }, 346 },
  { { ClusterTag(0x44), "ProxyValid", ItemType::kDefault }, 347 },
  { { ClusterTag(0x45), "BooleanState", ItemType::kDefault }, 348 },
  { { ClusterTag(0x46), "IcdManagement", ItemType::kDefault }, 350 },
  { { ClusterTag(0x47), "Timer", ItemType::kDefault }, 360 },
  { { ClusterTag(0x48), "OvenCavityOperationalState", ItemType::kDefault }, 366 },
  { { ClusterTag(0x49), "OvenMode", ItemType::kDefault }, 375 },
  { { ClusterTag(0x4A), "LaundryDryerControls", ItemType::kDefault }, 384 },
  { { ClusterTag(0x50), "ModeSelect", ItemType::kDefault }, 387 },
  { { ClusterTag(0x51), "LaundryWasherMode", ItemType::kDefault }, 394 },
  { { ClusterTag(0x52), "RefrigeratorAndTemperatureControlledCabinetMode", ItemType::kDefault }, 403 },
  { { ClusterTag(0x53), "LaundryWasherControls", ItemType::kDefault }, 412 },
  { { ClusterTag(0x54), "RvcRunMode", ItemType::kDefault }, 416 },
  { { ClusterTag(0x55), "RvcCleanMode", ItemType::kDefault }, 426 },
  { { ClusterTag(0x56), "TemperatureControl", ItemType::kDefault }, 436 },
  { { ClusterTag(0x57), "RefrigeratorAlarm", ItemType::kDefault }, 439 },
  { { ClusterTag(0x59), "DishwasherMode", ItemType::kDefault }, 442 },
  { { ClusterTag(0x5B), "AirQuality", ItemType::kDefault }, 451 },
  { { ClusterTag(0x5C), "SmokeCoAlarm", ItemType::kDefault }, 454 },
  { { ClusterTag(0x5D), "DishwasherAlarm", ItemType::kDefault }, 467 },
  { { ClusterTag(0x5E), "MicrowaveOvenMode", ItemType::kDefault }, 473 },
  { { ClusterTag(0x5F), "MicrowaveOvenControl", ItemType::kDefault }, 480 },
  { { ClusterTag(0x60), "OperationalState", ItemType::kDefault }, 483 },
  { { ClusterTag(0x61), "RvcOperationalState", ItemType::kDefault }, 492 },
  { { ClusterTag(0x71), "HepaFilterMonitoring", ItemType::kDefault }, 501 },
  { { ClusterTag(0x72), "ActivatedCarbonFilterMonitoring", ItemType::kDefault }, 508 },
  { { ClusterTag(0x80), "BooleanStateConfiguration", ItemType::kDefault }, 515 },
  { { ClusterTag(0x81), "ValveConfigurationAndControl", ItemType::kDefault }, 523 },
  { { ClusterTag(0x91), "ElectricalEnergyMeasurement", ItemType::kDefault }, 531 },
  { { ClusterTag(0x96), "DemandResponseLoadControl", ItemType::kDefault }, 540 },
  { { ClusterTag(0x98), "DeviceEnergyManagement", ItemType::kDefault }, 566 },
  { { ClusterTag(0x99), "EnergyEvse", ItemType::kDefault }, 589 },
  { { ClusterTag(0x9B), "EnergyPreference", ItemType::kDefault }, 609 },
  { { ClusterTag(0x101), "DoorLock", ItemType::kDefault }, 615 },
  { { ClusterTag(0x102), "WindowCovering", ItemType::kDefault }, 684 },
  { { ClusterTag(0x103), "BarrierControl", ItemType::kDefault }, 696 },
  { { ClusterTag(0x200), "PumpConfigurationAndControl", ItemType::kDefault }, 700 },
  { { ClusterTag(0x201), "Thermostat", ItemType::kDefault }, 705 },
  { { ClusterTag(0x202), "FanControl", ItemType::kDefault }, 732 },
  { { ClusterTag(0x204), "ThermostatUserInterfaceConfiguration", ItemType::kDefault }, 741 },
  { { ClusterTag(0x300), "ColorControl", ItemType::kDefault }, 745 },
  { { ClusterTag(0x301), "BallastConfiguration", ItemType::kDefault }, 776 },
  { { ClusterTag(0x400), "IlluminanceMeasurement", ItemType::kDefault }, 779 },
  { { ClusterTag(0x402), "TemperatureMeasurement", ItemType::kDefault }, 781 },
  { { ClusterTag(0x403), "PressureMeasurement", ItemType::kDefault }, 782 },
  { { ClusterTag(0x404), "FlowMeasurement", ItemType::kDefault }, 784 },
  { { ClusterTag(0x405), "RelativeHumidityMeasurement", ItemType::kDefault }, 785 },
  { { ClusterTag(0x406), "OccupancySensing", ItemType::kDefault }, 786 },
  { { ClusterTag(0x40C), "CarbonMonoxideConcentrationMeasurement", ItemType::kDefault }, 790 },
  { { ClusterTag(0x40D), "CarbonDioxideConcentrationMeasurement", ItemType::kDefault }, 795 },
  { { ClusterTag(0x413), "NitrogenDioxideConcentrationMeasurement", ItemType::kDefault }, 800 },
  { { ClusterTag(0x415), "OzoneConcentrationMeasurement", ItemType::kDefault }, 805 },
  { { ClusterTag(0x42A), "Pm25ConcentrationMeasurement", ItemType::kDefault }, 810 },
  { { ClusterTag(0x42B), "FormaldehydeConcentrationMeasurement", ItemType::kDefault }, 815 },
  { { ClusterTag(0x42C), "Pm1ConcentrationMeasurement", ItemType::kDefault }, 820 },
  { { ClusterTag(0x42D), "Pm10ConcentrationMeasurement", ItemType::kDefault }, 825 },
  { { ClusterTag(0x42E), "TotalVolatileOrganicCompoundsConcentrationMeasurement", ItemType::kDefault }, 830 },
  { { ClusterTag(0x42F), "RadonConcentrationMeasurement", ItemType::kDefault }, 835 },
  { { ClusterTag(0x503), "WakeOnLan", ItemType::kDefault }, 840 },
  { { ClusterTag(0x504), "Channel", ItemType::kDefault }, 841 },
  { { ClusterTag(0x505), "TargetNavigator", ItemType::kDefault }, 869 },
  { { ClusterTag(0x506), "MediaPlayback", ItemType::kDefault }, 876 },
  { { ClusterTag(0x507), "MediaInput", ItemType::kDefault }, 894 },
  { { ClusterTag(0x508), "LowPower", ItemType::kDefault }, 901 },
  { { ClusterTag(0x509), "KeypadInput", ItemType::kDefault }, 902 },
  { { ClusterTag(0x50A), "ContentLauncher", ItemType::kDefault }, 908 },
  { { ClusterTag(0x50B), "AudioOutput", ItemType::kDefault }, 930 },
  { { ClusterTag(0x50C), "ApplicationLauncher", ItemType::kDefault }, 937 },
  { { ClusterTag(0x50D), "ApplicationBasic", ItemType::kDefault }, 946 },
  { { ClusterTag(0x50E), "AccountLogin", ItemType::kDefault }, 949 },
  { { ClusterTag(0x50F), "ContentControl", ItemType::kDefault }, 955 },
  { { ClusterTag(0x510), "ContentAppObserver", ItemType::kDefault }, 965 },
  { { ClusterTag(0xB04), "ElectricalMeasurement", ItemType::kDefault }, 969 },
  { { ClusterTag(0xFFF1FC05), "UnitTesting", ItemType::kDefault }, 973 },
  { { ClusterTag(0xFFF1FC06), "FaultInjection", ItemType::kDefault }, 1027 },
  { { ClusterTag(0xFFF1FC20), "SampleMei", ItemType::kDefault }, 1031 },

};

// For any non-structure list like u64[] or similar.
const Entry<ItemInfo> _primitive_type_list[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, kInvalidNodeIndex },
};

} // namespace

#define _ENTRY(n) { sizeof(n) / sizeof(n[0]), n}

const std::array<const Node<ItemInfo>, 1033 + 2> clusters_meta = { {
  _ENTRY(_all_clusters), // 0
  _ENTRY(_primitive_type_list), // 1
  _ENTRY(_Identify), // 2
  _ENTRY(_Identify_IdentifyRequest), // 3
  _ENTRY(_Identify_TriggerEffectRequest), // 4
  _ENTRY(_Identify_EffectIdentifierEnum), // 5
  _ENTRY(_Identify_EffectVariantEnum), // 6
  _ENTRY(_Identify_IdentifyTypeEnum), // 7
  _ENTRY(_Groups), // 8
  _ENTRY(_Groups_AddGroupRequest), // 9
  _ENTRY(_Groups_AddGroupResponse), // 10
  _ENTRY(_Groups_ViewGroupRequest), // 11
  _ENTRY(_Groups_ViewGroupResponse), // 12
  _ENTRY(_Groups_GetGroupMembershipRequest), // 13
  _ENTRY(_Groups_GetGroupMembershipResponse), // 14
  _ENTRY(_Groups_RemoveGroupRequest), // 15
  _ENTRY(_Groups_RemoveGroupResponse), // 16
  _ENTRY(_Groups_AddGroupIfIdentifyingRequest), // 17
  _ENTRY(_Groups_Feature), // 18
  _ENTRY(_Groups_NameSupportBitmap), // 19
  _ENTRY(_Scenes), // 20
  _ENTRY(_Scenes_AttributeValuePair), // 21
  _ENTRY(_Scenes_ExtensionFieldSet), // 22
  _ENTRY(_Scenes_SceneInfoStruct), // 23
  _ENTRY(_Scenes_AddSceneRequest), // 24
  _ENTRY(_Scenes_AddSceneResponse), // 25
  _ENTRY(_Scenes_ViewSceneRequest), // 26
  _ENTRY(_Scenes_ViewSceneResponse), // 27
  _ENTRY(_Scenes_RemoveSceneRequest), // 28
  _ENTRY(_Scenes_RemoveSceneResponse), // 29
  _ENTRY(_Scenes_RemoveAllScenesRequest), // 30
  _ENTRY(_Scenes_RemoveAllScenesResponse), // 31
  _ENTRY(_Scenes_StoreSceneRequest), // 32
  _ENTRY(_Scenes_StoreSceneResponse), // 33
  _ENTRY(_Scenes_RecallSceneRequest), // 34
  _ENTRY(_Scenes_GetSceneMembershipRequest), // 35
  _ENTRY(_Scenes_GetSceneMembershipResponse), // 36
  _ENTRY(_Scenes_EnhancedAddSceneRequest), // 37
  _ENTRY(_Scenes_EnhancedAddSceneResponse), // 38
  _ENTRY(_Scenes_EnhancedViewSceneRequest), // 39
  _ENTRY(_Scenes_EnhancedViewSceneResponse), // 40
  _ENTRY(_Scenes_CopySceneRequest), // 41
  _ENTRY(_Scenes_CopySceneResponse), // 42
  _ENTRY(_Scenes_SceneInfoStruct_list_), // 43
  _ENTRY(_Scenes_AttributeValuePair_list_), // 44
  _ENTRY(_Scenes_ExtensionFieldSet_list_), // 45
  _ENTRY(_Scenes_CopyModeBitmap), // 46
  _ENTRY(_Scenes_Feature), // 47
  _ENTRY(_Scenes_NameSupportBitmap), // 48
  _ENTRY(_OnOff), // 49
  _ENTRY(_OnOff_OffWithEffectRequest), // 50
  _ENTRY(_OnOff_OnWithTimedOffRequest), // 51
  _ENTRY(_OnOff_DelayedAllOffEffectVariantEnum), // 52
  _ENTRY(_OnOff_DyingLightEffectVariantEnum), // 53
  _ENTRY(_OnOff_EffectIdentifierEnum), // 54
  _ENTRY(_OnOff_StartUpOnOffEnum), // 55
  _ENTRY(_OnOff_Feature), // 56
  _ENTRY(_OnOff_OnOffControlBitmap), // 57
  _ENTRY(_OnOffSwitchConfiguration), // 58
  _ENTRY(_LevelControl), // 59
  _ENTRY(_LevelControl_MoveToLevelRequest), // 60
  _ENTRY(_LevelControl_MoveRequest), // 61
  _ENTRY(_LevelControl_StepRequest), // 62
  _ENTRY(_LevelControl_StopRequest), // 63
  _ENTRY(_LevelControl_MoveToLevelWithOnOffRequest), // 64
  _ENTRY(_LevelControl_MoveWithOnOffRequest), // 65
  _ENTRY(_LevelControl_StepWithOnOffRequest), // 66
  _ENTRY(_LevelControl_StopWithOnOffRequest), // 67
  _ENTRY(_LevelControl_MoveToClosestFrequencyRequest), // 68
  _ENTRY(_LevelControl_MoveModeEnum), // 69
  _ENTRY(_LevelControl_StepModeEnum), // 70
  _ENTRY(_LevelControl_Feature), // 71
  _ENTRY(_LevelControl_OptionsBitmap), // 72
  _ENTRY(_BinaryInputBasic), // 73
  _ENTRY(_PulseWidthModulation), // 74
  _ENTRY(_Descriptor), // 75
  _ENTRY(_Descriptor_DeviceTypeStruct), // 76
  _ENTRY(_Descriptor_SemanticTagStruct), // 77
  _ENTRY(_Descriptor_SemanticTagStruct_list_), // 78
  _ENTRY(_Descriptor_DeviceTypeStruct_list_), // 79
  _ENTRY(_Descriptor_Feature), // 80
  _ENTRY(_Binding), // 81
  _ENTRY(_Binding_TargetStruct), // 82
  _ENTRY(_Binding_TargetStruct_list_), // 83
  _ENTRY(_AccessControl), // 84
  _ENTRY(_AccessControl_AccessControlTargetStruct), // 85
  _ENTRY(_AccessControl_AccessControlEntryStruct), // 86
  _ENTRY(_AccessControl_AccessControlExtensionStruct), // 87
  _ENTRY(_AccessControl_AccessControlEntryChanged), // 88
  _ENTRY(_AccessControl_AccessControlExtensionChanged), // 89
  _ENTRY(_AccessControl_AccessControlTargetStruct_list_), // 90
  _ENTRY(_AccessControl_AccessControlExtensionStruct_list_), // 91
  _ENTRY(_AccessControl_AccessControlEntryStruct_list_), // 92
  _ENTRY(_AccessControl_AccessControlEntryAuthModeEnum), // 93
  _ENTRY(_AccessControl_AccessControlEntryPrivilegeEnum), // 94
  _ENTRY(_AccessControl_ChangeTypeEnum), // 95
  _ENTRY(_Actions), // 96
  _ENTRY(_Actions_ActionStruct), // 97
  _ENTRY(_Actions_EndpointListStruct), // 98
  _ENTRY(_Actions_InstantActionRequest), // 99
  _ENTRY(_Actions_InstantActionWithTransitionRequest), // 100
  _ENTRY(_Actions_StartActionRequest), // 101
  _ENTRY(_Actions_StartActionWithDurationRequest), // 102
  _ENTRY(_Actions_StopActionRequest), // 103
  _ENTRY(_Actions_PauseActionRequest), // 104
  _ENTRY(_Actions_PauseActionWithDurationRequest), // 105
  _ENTRY(_Actions_ResumeActionRequest), // 106
  _ENTRY(_Actions_EnableActionRequest), // 107
  _ENTRY(_Actions_EnableActionWithDurationRequest), // 108
  _ENTRY(_Actions_DisableActionRequest), // 109
  _ENTRY(_Actions_DisableActionWithDurationRequest), // 110
  _ENTRY(_Actions_StateChanged), // 111
  _ENTRY(_Actions_ActionFailed), // 112
  _ENTRY(_Actions_ActionStruct_list_), // 113
  _ENTRY(_Actions_EndpointListStruct_list_), // 114
  _ENTRY(_Actions_ActionErrorEnum), // 115
  _ENTRY(_Actions_ActionStateEnum), // 116
  _ENTRY(_Actions_ActionTypeEnum), // 117
  _ENTRY(_Actions_EndpointListTypeEnum), // 118
  _ENTRY(_Actions_CommandBits), // 119
  _ENTRY(_BasicInformation), // 120
  _ENTRY(_BasicInformation_CapabilityMinimaStruct), // 121
  _ENTRY(_BasicInformation_ProductAppearanceStruct), // 122
  _ENTRY(_BasicInformation_StartUp), // 123
  _ENTRY(_BasicInformation_Leave), // 124
  _ENTRY(_BasicInformation_ReachableChanged), // 125
  _ENTRY(_BasicInformation_ColorEnum), // 126
  _ENTRY(_BasicInformation_ProductFinishEnum), // 127
  _ENTRY(_OtaSoftwareUpdateProvider), // 128
  _ENTRY(_OtaSoftwareUpdateProvider_QueryImageRequest), // 129
  _ENTRY(_OtaSoftwareUpdateProvider_QueryImageResponse), // 130
  _ENTRY(_OtaSoftwareUpdateProvider_ApplyUpdateRequestRequest), // 131
  _ENTRY(_OtaSoftwareUpdateProvider_ApplyUpdateResponse), // 132
  _ENTRY(_OtaSoftwareUpdateProvider_NotifyUpdateAppliedRequest), // 133
  _ENTRY(_OtaSoftwareUpdateProvider_DownloadProtocolEnum_list_), // 134
  _ENTRY(_OtaSoftwareUpdateProvider_ApplyUpdateActionEnum), // 135
  _ENTRY(_OtaSoftwareUpdateProvider_DownloadProtocolEnum), // 136
  _ENTRY(_OtaSoftwareUpdateProvider_StatusEnum), // 137
  _ENTRY(_OtaSoftwareUpdateRequestor), // 138
  _ENTRY(_OtaSoftwareUpdateRequestor_ProviderLocation), // 139
  _ENTRY(_OtaSoftwareUpdateRequestor_AnnounceOTAProviderRequest), // 140
  _ENTRY(_OtaSoftwareUpdateRequestor_StateTransition), // 141
  _ENTRY(_OtaSoftwareUpdateRequestor_VersionApplied), // 142
  _ENTRY(_OtaSoftwareUpdateRequestor_DownloadError), // 143
  _ENTRY(_OtaSoftwareUpdateRequestor_ProviderLocation_list_), // 144
  _ENTRY(_OtaSoftwareUpdateRequestor_AnnouncementReasonEnum), // 145
  _ENTRY(_OtaSoftwareUpdateRequestor_ChangeReasonEnum), // 146
  _ENTRY(_OtaSoftwareUpdateRequestor_UpdateStateEnum), // 147
  _ENTRY(_LocalizationConfiguration), // 148
  _ENTRY(_TimeFormatLocalization), // 149
  _ENTRY(_TimeFormatLocalization_CalendarTypeEnum_list_), // 150
  _ENTRY(_TimeFormatLocalization_CalendarTypeEnum), // 151
  _ENTRY(_TimeFormatLocalization_HourFormatEnum), // 152
  _ENTRY(_TimeFormatLocalization_Feature), // 153
  _ENTRY(_UnitLocalization), // 154
  _ENTRY(_UnitLocalization_TempUnitEnum), // 155
  _ENTRY(_UnitLocalization_Feature), // 156
  _ENTRY(_PowerSourceConfiguration), // 157
  _ENTRY(_PowerSource), // 158
  _ENTRY(_PowerSource_BatChargeFaultChangeType), // 159
  _ENTRY(_PowerSource_BatFaultChangeType), // 160
  _ENTRY(_PowerSource_WiredFaultChangeType), // 161
  _ENTRY(_PowerSource_WiredFaultChange), // 162
  _ENTRY(_PowerSource_BatFaultChange), // 163
  _ENTRY(_PowerSource_BatChargeFaultChange), // 164
  _ENTRY(_PowerSource_BatChargeFaultEnum_list_), // 165
  _ENTRY(_PowerSource_WiredFaultEnum_list_), // 166
  _ENTRY(_PowerSource_BatFaultEnum_list_), // 167
  _ENTRY(_PowerSource_BatApprovedChemistryEnum), // 168
  _ENTRY(_PowerSource_BatChargeFaultEnum), // 169
  _ENTRY(_PowerSource_BatChargeLevelEnum), // 170
  _ENTRY(_PowerSource_BatChargeStateEnum), // 171
  _ENTRY(_PowerSource_BatCommonDesignationEnum), // 172
  _ENTRY(_PowerSource_BatFaultEnum), // 173
  _ENTRY(_PowerSource_BatReplaceabilityEnum), // 174
  _ENTRY(_PowerSource_PowerSourceStatusEnum), // 175
  _ENTRY(_PowerSource_WiredCurrentTypeEnum), // 176
  _ENTRY(_PowerSource_WiredFaultEnum), // 177
  _ENTRY(_PowerSource_Feature), // 178
  _ENTRY(_GeneralCommissioning), // 179
  _ENTRY(_GeneralCommissioning_BasicCommissioningInfo), // 180
  _ENTRY(_GeneralCommissioning_ArmFailSafeRequest), // 181
  _ENTRY(_GeneralCommissioning_ArmFailSafeResponse), // 182
  _ENTRY(_GeneralCommissioning_SetRegulatoryConfigRequest), // 183
  _ENTRY(_GeneralCommissioning_SetRegulatoryConfigResponse), // 184
  _ENTRY(_GeneralCommissioning_CommissioningCompleteResponse), // 185
  _ENTRY(_GeneralCommissioning_CommissioningErrorEnum), // 186
  _ENTRY(_GeneralCommissioning_RegulatoryLocationTypeEnum), // 187
  _ENTRY(_NetworkCommissioning), // 188
  _ENTRY(_NetworkCommissioning_NetworkInfoStruct), // 189
  _ENTRY(_NetworkCommissioning_ThreadInterfaceScanResultStruct), // 190
  _ENTRY(_NetworkCommissioning_WiFiInterfaceScanResultStruct), // 191
  _ENTRY(_NetworkCommissioning_ScanNetworksRequest), // 192
  _ENTRY(_NetworkCommissioning_ScanNetworksResponse), // 193
  _ENTRY(_NetworkCommissioning_AddOrUpdateWiFiNetworkRequest), // 194
  _ENTRY(_NetworkCommissioning_AddOrUpdateThreadNetworkRequest), // 195
  _ENTRY(_NetworkCommissioning_RemoveNetworkRequest), // 196
  _ENTRY(_NetworkCommissioning_NetworkConfigResponse), // 197
  _ENTRY(_NetworkCommissioning_ConnectNetworkRequest), // 198
  _ENTRY(_NetworkCommissioning_ConnectNetworkResponse), // 199
  _ENTRY(_NetworkCommissioning_ReorderNetworkRequest), // 200
  _ENTRY(_NetworkCommissioning_QueryIdentityRequest), // 201
  _ENTRY(_NetworkCommissioning_QueryIdentityResponse), // 202
  _ENTRY(_NetworkCommissioning_WiFiInterfaceScanResultStruct_list_), // 203
  _ENTRY(_NetworkCommissioning_WiFiBandEnum_list_), // 204
  _ENTRY(_NetworkCommissioning_NetworkInfoStruct_list_), // 205
  _ENTRY(_NetworkCommissioning_ThreadInterfaceScanResultStruct_list_), // 206
  _ENTRY(_NetworkCommissioning_NetworkCommissioningStatusEnum), // 207
  _ENTRY(_NetworkCommissioning_WiFiBandEnum), // 208
  _ENTRY(_NetworkCommissioning_Feature), // 209
  _ENTRY(_NetworkCommissioning_ThreadCapabilitiesBitmap), // 210
  _ENTRY(_NetworkCommissioning_WiFiSecurityBitmap), // 211
  _ENTRY(_DiagnosticLogs), // 212
  _ENTRY(_DiagnosticLogs_RetrieveLogsRequestRequest), // 213
  _ENTRY(_DiagnosticLogs_RetrieveLogsResponse), // 214
  _ENTRY(_DiagnosticLogs_IntentEnum), // 215
  _ENTRY(_DiagnosticLogs_StatusEnum), // 216
  _ENTRY(_DiagnosticLogs_TransferProtocolEnum), // 217
  _ENTRY(_GeneralDiagnostics), // 218
  _ENTRY(_GeneralDiagnostics_NetworkInterface), // 219
  _ENTRY(_GeneralDiagnostics_TestEventTriggerRequest), // 220
  _ENTRY(_GeneralDiagnostics_TimeSnapshotResponse), // 221
  _ENTRY(_GeneralDiagnostics_HardwareFaultChange), // 222
  _ENTRY(_GeneralDiagnostics_RadioFaultChange), // 223
  _ENTRY(_GeneralDiagnostics_NetworkFaultChange), // 224
  _ENTRY(_GeneralDiagnostics_BootReason), // 225
  _ENTRY(_GeneralDiagnostics_NetworkInterface_list_), // 226
  _ENTRY(_GeneralDiagnostics_NetworkFaultEnum_list_), // 227
  _ENTRY(_GeneralDiagnostics_HardwareFaultEnum_list_), // 228
  _ENTRY(_GeneralDiagnostics_RadioFaultEnum_list_), // 229
  _ENTRY(_GeneralDiagnostics_BootReasonEnum), // 230
  _ENTRY(_GeneralDiagnostics_HardwareFaultEnum), // 231
  _ENTRY(_GeneralDiagnostics_InterfaceTypeEnum), // 232
  _ENTRY(_GeneralDiagnostics_NetworkFaultEnum), // 233
  _ENTRY(_GeneralDiagnostics_RadioFaultEnum), // 234
  _ENTRY(_SoftwareDiagnostics), // 235
  _ENTRY(_SoftwareDiagnostics_ThreadMetricsStruct), // 236
  _ENTRY(_SoftwareDiagnostics_SoftwareFault), // 237
  _ENTRY(_SoftwareDiagnostics_ThreadMetricsStruct_list_), // 238
  _ENTRY(_SoftwareDiagnostics_Feature), // 239
  _ENTRY(_ThreadNetworkDiagnostics), // 240
  _ENTRY(_ThreadNetworkDiagnostics_NeighborTableStruct), // 241
  _ENTRY(_ThreadNetworkDiagnostics_OperationalDatasetComponents), // 242
  _ENTRY(_ThreadNetworkDiagnostics_RouteTableStruct), // 243
  _ENTRY(_ThreadNetworkDiagnostics_SecurityPolicy), // 244
  _ENTRY(_ThreadNetworkDiagnostics_ConnectionStatus), // 245
  _ENTRY(_ThreadNetworkDiagnostics_NetworkFaultChange), // 246
  _ENTRY(_ThreadNetworkDiagnostics_RouteTableStruct_list_), // 247
  _ENTRY(_ThreadNetworkDiagnostics_NetworkFaultEnum_list_), // 248
  _ENTRY(_ThreadNetworkDiagnostics_NeighborTableStruct_list_), // 249
  _ENTRY(_ThreadNetworkDiagnostics_ConnectionStatusEnum), // 250
  _ENTRY(_ThreadNetworkDiagnostics_NetworkFaultEnum), // 251
  _ENTRY(_ThreadNetworkDiagnostics_RoutingRoleEnum), // 252
  _ENTRY(_ThreadNetworkDiagnostics_Feature), // 253
  _ENTRY(_WiFiNetworkDiagnostics), // 254
  _ENTRY(_WiFiNetworkDiagnostics_Disconnection), // 255
  _ENTRY(_WiFiNetworkDiagnostics_AssociationFailure), // 256
  _ENTRY(_WiFiNetworkDiagnostics_ConnectionStatus), // 257
  _ENTRY(_WiFiNetworkDiagnostics_AssociationFailureCauseEnum), // 258
  _ENTRY(_WiFiNetworkDiagnostics_ConnectionStatusEnum), // 259
  _ENTRY(_WiFiNetworkDiagnostics_SecurityTypeEnum), // 260
  _ENTRY(_WiFiNetworkDiagnostics_WiFiVersionEnum), // 261
  _ENTRY(_WiFiNetworkDiagnostics_Feature), // 262
  _ENTRY(_EthernetNetworkDiagnostics), // 263
  _ENTRY(_EthernetNetworkDiagnostics_PHYRateEnum), // 264
  _ENTRY(_EthernetNetworkDiagnostics_Feature), // 265
  _ENTRY(_TimeSynchronization), // 266
  _ENTRY(_TimeSynchronization_DSTOffsetStruct), // 267
  _ENTRY(_TimeSynchronization_FabricScopedTrustedTimeSourceStruct), // 268
  _ENTRY(_TimeSynchronization_TimeZoneStruct), // 269
  _ENTRY(_TimeSynchronization_TrustedTimeSourceStruct), // 270
  _ENTRY(_TimeSynchronization_SetUTCTimeRequest), // 271
  _ENTRY(_TimeSynchronization_SetTrustedTimeSourceRequest), // 272
  _ENTRY(_TimeSynchronization_SetTimeZoneRequest), // 273
  _ENTRY(_TimeSynchronization_SetTimeZoneResponse), // 274
  _ENTRY(_TimeSynchronization_SetDSTOffsetRequest), // 275
  _ENTRY(_TimeSynchronization_SetDefaultNTPRequest), // 276
  _ENTRY(_TimeSynchronization_DSTStatus), // 277
  _ENTRY(_TimeSynchronization_TimeZoneStatus), // 278
  _ENTRY(_TimeSynchronization_DSTOffsetStruct_list_), // 279
  _ENTRY(_TimeSynchronization_TimeZoneStruct_list_), // 280
  _ENTRY(_TimeSynchronization_GranularityEnum), // 281
  _ENTRY(_TimeSynchronization_StatusCode), // 282
  _ENTRY(_TimeSynchronization_TimeSourceEnum), // 283
  _ENTRY(_TimeSynchronization_TimeZoneDatabaseEnum), // 284
  _ENTRY(_TimeSynchronization_Feature), // 285
  _ENTRY(_BridgedDeviceBasicInformation), // 286
  _ENTRY(_BridgedDeviceBasicInformation_ProductAppearanceStruct), // 287
  _ENTRY(_BridgedDeviceBasicInformation_StartUp), // 288
  _ENTRY(_BridgedDeviceBasicInformation_ReachableChanged), // 289
  _ENTRY(_BridgedDeviceBasicInformation_ColorEnum), // 290
  _ENTRY(_BridgedDeviceBasicInformation_ProductFinishEnum), // 291
  _ENTRY(_Switch), // 292
  _ENTRY(_Switch_SwitchLatched), // 293
  _ENTRY(_Switch_InitialPress), // 294
  _ENTRY(_Switch_LongPress), // 295
  _ENTRY(_Switch_ShortRelease), // 296
  _ENTRY(_Switch_LongRelease), // 297
  _ENTRY(_Switch_MultiPressOngoing), // 298
  _ENTRY(_Switch_MultiPressComplete), // 299
  _ENTRY(_Switch_Feature), // 300
  _ENTRY(_AdministratorCommissioning), // 301
  _ENTRY(_AdministratorCommissioning_OpenCommissioningWindowRequest), // 302
  _ENTRY(_AdministratorCommissioning_OpenBasicCommissioningWindowRequest), // 303
  _ENTRY(_AdministratorCommissioning_CommissioningWindowStatusEnum), // 304
  _ENTRY(_AdministratorCommissioning_StatusCode), // 305
  _ENTRY(_AdministratorCommissioning_Feature), // 306
  _ENTRY(_OperationalCredentials), // 307
  _ENTRY(_OperationalCredentials_FabricDescriptorStruct), // 308
  _ENTRY(_OperationalCredentials_NOCStruct), // 309
  _ENTRY(_OperationalCredentials_AttestationRequestRequest), // 310
  _ENTRY(_OperationalCredentials_AttestationResponse), // 311
  _ENTRY(_OperationalCredentials_CertificateChainRequestRequest), // 312
  _ENTRY(_OperationalCredentials_CertificateChainResponse), // 313
  _ENTRY(_OperationalCredentials_CSRRequestRequest), // 314
  _ENTRY(_OperationalCredentials_CSRResponse), // 315
  _ENTRY(_OperationalCredentials_AddNOCRequest), // 316
  _ENTRY(_OperationalCredentials_UpdateNOCRequest), // 317
  _ENTRY(_OperationalCredentials_NOCResponse), // 318
  _ENTRY(_OperationalCredentials_UpdateFabricLabelRequest), // 319
  _ENTRY(_OperationalCredentials_RemoveFabricRequest), // 320
  _ENTRY(_OperationalCredentials_AddTrustedRootCertificateRequest), // 321
  _ENTRY(_OperationalCredentials_FabricDescriptorStruct_list_), // 322
  _ENTRY(_OperationalCredentials_NOCStruct_list_), // 323
  _ENTRY(_OperationalCredentials_CertificateChainTypeEnum), // 324
  _ENTRY(_OperationalCredentials_NodeOperationalCertStatusEnum), // 325
  _ENTRY(_GroupKeyManagement), // 326
  _ENTRY(_GroupKeyManagement_GroupInfoMapStruct), // 327
  _ENTRY(_GroupKeyManagement_GroupKeyMapStruct), // 328
  _ENTRY(_GroupKeyManagement_GroupKeySetStruct), // 329
  _ENTRY(_GroupKeyManagement_KeySetWriteRequest), // 330
  _ENTRY(_GroupKeyManagement_KeySetReadRequest), // 331
  _ENTRY(_GroupKeyManagement_KeySetReadResponse), // 332
  _ENTRY(_GroupKeyManagement_KeySetRemoveRequest), // 333
  _ENTRY(_GroupKeyManagement_KeySetReadAllIndicesResponse), // 334
  _ENTRY(_GroupKeyManagement_GroupInfoMapStruct_list_), // 335
  _ENTRY(_GroupKeyManagement_GroupKeyMapStruct_list_), // 336
  _ENTRY(_GroupKeyManagement_GroupKeySecurityPolicyEnum), // 337
  _ENTRY(_GroupKeyManagement_Feature), // 338
  _ENTRY(_FixedLabel), // 339
  _ENTRY(_FixedLabel_LabelStruct), // 340
  _ENTRY(_FixedLabel_LabelStruct_list_), // 341
  _ENTRY(_UserLabel), // 342
  _ENTRY(_UserLabel_LabelStruct), // 343
  _ENTRY(_UserLabel_LabelStruct_list_), // 344
  _ENTRY(_ProxyConfiguration), // 345
  _ENTRY(_ProxyDiscovery), // 346
  _ENTRY(_ProxyValid), // 347
  _ENTRY(_BooleanState), // 348
  _ENTRY(_BooleanState_StateChange), // 349
  _ENTRY(_IcdManagement), // 350
  _ENTRY(_IcdManagement_MonitoringRegistrationStruct), // 351
  _ENTRY(_IcdManagement_RegisterClientRequest), // 352
  _ENTRY(_IcdManagement_RegisterClientResponse), // 353
  _ENTRY(_IcdManagement_UnregisterClientRequest), // 354
  _ENTRY(_IcdManagement_StayActiveResponse), // 355
  _ENTRY(_IcdManagement_MonitoringRegistrationStruct_list_), // 356
  _ENTRY(_IcdManagement_OperatingModeEnum), // 357
  _ENTRY(_IcdManagement_Feature), // 358
  _ENTRY(_IcdManagement_UserActiveModeTriggerBitmap), // 359
  _ENTRY(_Timer), // 360
  _ENTRY(_Timer_SetTimerRequest), // 361
  _ENTRY(_Timer_AddTimeRequest), // 362
  _ENTRY(_Timer_ReduceTimeRequest), // 363
  _ENTRY(_Timer_TimerStatusEnum), // 364
  _ENTRY(_Timer_Feature), // 365
  _ENTRY(_OvenCavityOperationalState), // 366
  _ENTRY(_OvenCavityOperationalState_ErrorStateStruct), // 367
  _ENTRY(_OvenCavityOperationalState_OperationalStateStruct), // 368
  _ENTRY(_OvenCavityOperationalState_OperationalCommandResponse), // 369
  _ENTRY(_OvenCavityOperationalState_OperationalError), // 370
  _ENTRY(_OvenCavityOperationalState_OperationCompletion), // 371
  _ENTRY(_OvenCavityOperationalState_OperationalStateStruct_list_), // 372
  _ENTRY(_OvenCavityOperationalState_ErrorStateEnum), // 373
  _ENTRY(_OvenCavityOperationalState_OperationalStateEnum), // 374
  _ENTRY(_OvenMode), // 375
  _ENTRY(_OvenMode_ModeTagStruct), // 376
  _ENTRY(_OvenMode_ModeOptionStruct), // 377
  _ENTRY(_OvenMode_ChangeToModeRequest), // 378
  _ENTRY(_OvenMode_ChangeToModeResponse), // 379
  _ENTRY(_OvenMode_ModeOptionStruct_list_), // 380
  _ENTRY(_OvenMode_ModeTagStruct_list_), // 381
  _ENTRY(_OvenMode_ModeTag), // 382
  _ENTRY(_OvenMode_Feature), // 383
  _ENTRY(_LaundryDryerControls), // 384
  _ENTRY(_LaundryDryerControls_DrynessLevelEnum_list_), // 385
  _ENTRY(_LaundryDryerControls_DrynessLevelEnum), // 386
  _ENTRY(_ModeSelect), // 387
  _ENTRY(_ModeSelect_SemanticTagStruct), // 388
  _ENTRY(_ModeSelect_ModeOptionStruct), // 389
  _ENTRY(_ModeSelect_ChangeToModeRequest), // 390
  _ENTRY(_ModeSelect_SemanticTagStruct_list_), // 391
  _ENTRY(_ModeSelect_ModeOptionStruct_list_), // 392
  _ENTRY(_ModeSelect_Feature), // 393
  _ENTRY(_LaundryWasherMode), // 394
  _ENTRY(_LaundryWasherMode_ModeTagStruct), // 395
  _ENTRY(_LaundryWasherMode_ModeOptionStruct), // 396
  _ENTRY(_LaundryWasherMode_ChangeToModeRequest), // 397
  _ENTRY(_LaundryWasherMode_ChangeToModeResponse), // 398
  _ENTRY(_LaundryWasherMode_ModeOptionStruct_list_), // 399
  _ENTRY(_LaundryWasherMode_ModeTagStruct_list_), // 400
  _ENTRY(_LaundryWasherMode_ModeTag), // 401
  _ENTRY(_LaundryWasherMode_Feature), // 402
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode), // 403
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode_ModeTagStruct), // 404
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode_ModeOptionStruct), // 405
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode_ChangeToModeRequest), // 406
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode_ChangeToModeResponse), // 407
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode_ModeOptionStruct_list_), // 408
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode_ModeTagStruct_list_), // 409
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode_ModeTag), // 410
  _ENTRY(_RefrigeratorAndTemperatureControlledCabinetMode_Feature), // 411
  _ENTRY(_LaundryWasherControls), // 412
  _ENTRY(_LaundryWasherControls_NumberOfRinsesEnum_list_), // 413
  _ENTRY(_LaundryWasherControls_NumberOfRinsesEnum), // 414
  _ENTRY(_LaundryWasherControls_Feature), // 415
  _ENTRY(_RvcRunMode), // 416
  _ENTRY(_RvcRunMode_ModeTagStruct), // 417
  _ENTRY(_RvcRunMode_ModeOptionStruct), // 418
  _ENTRY(_RvcRunMode_ChangeToModeRequest), // 419
  _ENTRY(_RvcRunMode_ChangeToModeResponse), // 420
  _ENTRY(_RvcRunMode_ModeOptionStruct_list_), // 421
  _ENTRY(_RvcRunMode_ModeTagStruct_list_), // 422
  _ENTRY(_RvcRunMode_ModeTag), // 423
  _ENTRY(_RvcRunMode_StatusCode), // 424
  _ENTRY(_RvcRunMode_Feature), // 425
  _ENTRY(_RvcCleanMode), // 426
  _ENTRY(_RvcCleanMode_ModeTagStruct), // 427
  _ENTRY(_RvcCleanMode_ModeOptionStruct), // 428
  _ENTRY(_RvcCleanMode_ChangeToModeRequest), // 429
  _ENTRY(_RvcCleanMode_ChangeToModeResponse), // 430
  _ENTRY(_RvcCleanMode_ModeOptionStruct_list_), // 431
  _ENTRY(_RvcCleanMode_ModeTagStruct_list_), // 432
  _ENTRY(_RvcCleanMode_ModeTag), // 433
  _ENTRY(_RvcCleanMode_StatusCode), // 434
  _ENTRY(_RvcCleanMode_Feature), // 435
  _ENTRY(_TemperatureControl), // 436
  _ENTRY(_TemperatureControl_SetTemperatureRequest), // 437
  _ENTRY(_TemperatureControl_Feature), // 438
  _ENTRY(_RefrigeratorAlarm), // 439
  _ENTRY(_RefrigeratorAlarm_Notify), // 440
  _ENTRY(_RefrigeratorAlarm_AlarmMap), // 441
  _ENTRY(_DishwasherMode), // 442
  _ENTRY(_DishwasherMode_ModeTagStruct), // 443
  _ENTRY(_DishwasherMode_ModeOptionStruct), // 444
  _ENTRY(_DishwasherMode_ChangeToModeRequest), // 445
  _ENTRY(_DishwasherMode_ChangeToModeResponse), // 446
  _ENTRY(_DishwasherMode_ModeOptionStruct_list_), // 447
  _ENTRY(_DishwasherMode_ModeTagStruct_list_), // 448
  _ENTRY(_DishwasherMode_ModeTag), // 449
  _ENTRY(_DishwasherMode_Feature), // 450
  _ENTRY(_AirQuality), // 451
  _ENTRY(_AirQuality_AirQualityEnum), // 452
  _ENTRY(_AirQuality_Feature), // 453
  _ENTRY(_SmokeCoAlarm), // 454
  _ENTRY(_SmokeCoAlarm_SmokeAlarm), // 455
  _ENTRY(_SmokeCoAlarm_COAlarm), // 456
  _ENTRY(_SmokeCoAlarm_LowBattery), // 457
  _ENTRY(_SmokeCoAlarm_InterconnectSmokeAlarm), // 458
  _ENTRY(_SmokeCoAlarm_InterconnectCOAlarm), // 459
  _ENTRY(_SmokeCoAlarm_AlarmStateEnum), // 460
  _ENTRY(_SmokeCoAlarm_ContaminationStateEnum), // 461
  _ENTRY(_SmokeCoAlarm_EndOfServiceEnum), // 462
  _ENTRY(_SmokeCoAlarm_ExpressedStateEnum), // 463
  _ENTRY(_SmokeCoAlarm_MuteStateEnum), // 464
  _ENTRY(_SmokeCoAlarm_SensitivityEnum), // 465
  _ENTRY(_SmokeCoAlarm_Feature), // 466
  _ENTRY(_DishwasherAlarm), // 467
  _ENTRY(_DishwasherAlarm_ResetRequest), // 468
  _ENTRY(_DishwasherAlarm_ModifyEnabledAlarmsRequest), // 469
  _ENTRY(_DishwasherAlarm_Notify), // 470
  _ENTRY(_DishwasherAlarm_AlarmMap), // 471
  _ENTRY(_DishwasherAlarm_Feature), // 472
  _ENTRY(_MicrowaveOvenMode), // 473
  _ENTRY(_MicrowaveOvenMode_ModeTagStruct), // 474
  _ENTRY(_MicrowaveOvenMode_ModeOptionStruct), // 475
  _ENTRY(_MicrowaveOvenMode_ModeTagStruct_list_), // 476
  _ENTRY(_MicrowaveOvenMode_ModeOptionStruct_list_), // 477
  _ENTRY(_MicrowaveOvenMode_ModeTag), // 478
  _ENTRY(_MicrowaveOvenMode_Feature), // 479
  _ENTRY(_MicrowaveOvenControl), // 480
  _ENTRY(_MicrowaveOvenControl_SetCookingParametersRequest), // 481
  _ENTRY(_MicrowaveOvenControl_AddMoreTimeRequest), // 482
  _ENTRY(_OperationalState), // 483
  _ENTRY(_OperationalState_ErrorStateStruct), // 484
  _ENTRY(_OperationalState_OperationalStateStruct), // 485
  _ENTRY(_OperationalState_OperationalCommandResponse), // 486
  _ENTRY(_OperationalState_OperationalError), // 487
  _ENTRY(_OperationalState_OperationCompletion), // 488
  _ENTRY(_OperationalState_OperationalStateStruct_list_), // 489
  _ENTRY(_OperationalState_ErrorStateEnum), // 490
  _ENTRY(_OperationalState_OperationalStateEnum), // 491
  _ENTRY(_RvcOperationalState), // 492
  _ENTRY(_RvcOperationalState_ErrorStateStruct), // 493
  _ENTRY(_RvcOperationalState_OperationalStateStruct), // 494
  _ENTRY(_RvcOperationalState_OperationalCommandResponse), // 495
  _ENTRY(_RvcOperationalState_OperationalError), // 496
  _ENTRY(_RvcOperationalState_OperationCompletion), // 497
  _ENTRY(_RvcOperationalState_OperationalStateStruct_list_), // 498
  _ENTRY(_RvcOperationalState_ErrorStateEnum), // 499
  _ENTRY(_RvcOperationalState_OperationalStateEnum), // 500
  _ENTRY(_HepaFilterMonitoring), // 501
  _ENTRY(_HepaFilterMonitoring_ReplacementProductStruct), // 502
  _ENTRY(_HepaFilterMonitoring_ReplacementProductStruct_list_), // 503
  _ENTRY(_HepaFilterMonitoring_ChangeIndicationEnum), // 504
  _ENTRY(_HepaFilterMonitoring_DegradationDirectionEnum), // 505
  _ENTRY(_HepaFilterMonitoring_ProductIdentifierTypeEnum), // 506
  _ENTRY(_HepaFilterMonitoring_Feature), // 507
  _ENTRY(_ActivatedCarbonFilterMonitoring), // 508
  _ENTRY(_ActivatedCarbonFilterMonitoring_ReplacementProductStruct), // 509
  _ENTRY(_ActivatedCarbonFilterMonitoring_ReplacementProductStruct_list_), // 510
  _ENTRY(_ActivatedCarbonFilterMonitoring_ChangeIndicationEnum), // 511
  _ENTRY(_ActivatedCarbonFilterMonitoring_DegradationDirectionEnum), // 512
  _ENTRY(_ActivatedCarbonFilterMonitoring_ProductIdentifierTypeEnum), // 513
  _ENTRY(_ActivatedCarbonFilterMonitoring_Feature), // 514
  _ENTRY(_BooleanStateConfiguration), // 515
  _ENTRY(_BooleanStateConfiguration_SuppressAlarmRequest), // 516
  _ENTRY(_BooleanStateConfiguration_EnableDisableAlarmRequest), // 517
  _ENTRY(_BooleanStateConfiguration_AlarmsStateChanged), // 518
  _ENTRY(_BooleanStateConfiguration_SensorFault), // 519
  _ENTRY(_BooleanStateConfiguration_AlarmModeBitmap), // 520
  _ENTRY(_BooleanStateConfiguration_Feature), // 521
  _ENTRY(_BooleanStateConfiguration_SensorFaultBitmap), // 522
  _ENTRY(_ValveConfigurationAndControl), // 523
  _ENTRY(_ValveConfigurationAndControl_OpenRequest), // 524
  _ENTRY(_ValveConfigurationAndControl_ValveStateChanged), // 525
  _ENTRY(_ValveConfigurationAndControl_ValveFault), // 526
  _ENTRY(_ValveConfigurationAndControl_StatusCodeEnum), // 527
  _ENTRY(_ValveConfigurationAndControl_ValveStateEnum), // 528
  _ENTRY(_ValveConfigurationAndControl_Feature), // 529
  _ENTRY(_ValveConfigurationAndControl_ValveFaultBitmap), // 530
  _ENTRY(_ElectricalEnergyMeasurement), // 531
  _ENTRY(_ElectricalEnergyMeasurement_MeasurementAccuracyRangeStruct), // 532
  _ENTRY(_ElectricalEnergyMeasurement_MeasurementAccuracyStruct), // 533
  _ENTRY(_ElectricalEnergyMeasurement_EnergyMeasurementStruct), // 534
  _ENTRY(_ElectricalEnergyMeasurement_CumulativeEnergyMeasured), // 535
  _ENTRY(_ElectricalEnergyMeasurement_PeriodicEnergyMeasured), // 536
  _ENTRY(_ElectricalEnergyMeasurement_MeasurementAccuracyRangeStruct_list_), // 537
  _ENTRY(_ElectricalEnergyMeasurement_MeasurementTypeEnum), // 538
  _ENTRY(_ElectricalEnergyMeasurement_Feature), // 539
  _ENTRY(_DemandResponseLoadControl), // 540
  _ENTRY(_DemandResponseLoadControl_HeatingSourceControlStruct), // 541
  _ENTRY(_DemandResponseLoadControl_PowerSavingsControlStruct), // 542
  _ENTRY(_DemandResponseLoadControl_DutyCycleControlStruct), // 543
  _ENTRY(_DemandResponseLoadControl_AverageLoadControlStruct), // 544
  _ENTRY(_DemandResponseLoadControl_TemperatureControlStruct), // 545
  _ENTRY(_DemandResponseLoadControl_LoadControlEventTransitionStruct), // 546
  _ENTRY(_DemandResponseLoadControl_LoadControlEventStruct), // 547
  _ENTRY(_DemandResponseLoadControl_LoadControlProgramStruct), // 548
  _ENTRY(_DemandResponseLoadControl_RegisterLoadControlProgramRequestRequest), // 549
  _ENTRY(_DemandResponseLoadControl_UnregisterLoadControlProgramRequestRequest), // 550
  _ENTRY(_DemandResponseLoadControl_AddLoadControlEventRequestRequest), // 551
  _ENTRY(_DemandResponseLoadControl_RemoveLoadControlEventRequestRequest), // 552
  _ENTRY(_DemandResponseLoadControl_LoadControlEventStatusChange), // 553
  _ENTRY(_DemandResponseLoadControl_LoadControlEventStruct_list_), // 554
  _ENTRY(_DemandResponseLoadControl_LoadControlProgramStruct_list_), // 555
  _ENTRY(_DemandResponseLoadControl_LoadControlEventTransitionStruct_list_), // 556
  _ENTRY(_DemandResponseLoadControl_CriticalityLevelEnum), // 557
  _ENTRY(_DemandResponseLoadControl_HeatingSourceEnum), // 558
  _ENTRY(_DemandResponseLoadControl_LoadControlEventChangeSourceEnum), // 559
  _ENTRY(_DemandResponseLoadControl_LoadControlEventStatusEnum), // 560
  _ENTRY(_DemandResponseLoadControl_CancelControlBitmap), // 561
  _ENTRY(_DemandResponseLoadControl_DeviceClassBitmap), // 562
  _ENTRY(_DemandResponseLoadControl_EventControlBitmap), // 563
  _ENTRY(_DemandResponseLoadControl_EventTransitionControlBitmap), // 564
  _ENTRY(_DemandResponseLoadControl_Feature), // 565
  _ENTRY(_DeviceEnergyManagement), // 566
  _ENTRY(_DeviceEnergyManagement_CostStruct), // 567
  _ENTRY(_DeviceEnergyManagement_SlotStruct), // 568
  _ENTRY(_DeviceEnergyManagement_ForecastStruct), // 569
  _ENTRY(_DeviceEnergyManagement_ConstraintsStruct), // 570
  _ENTRY(_DeviceEnergyManagement_PowerAdjustStruct), // 571
  _ENTRY(_DeviceEnergyManagement_SlotAdjustmentStruct), // 572
  _ENTRY(_DeviceEnergyManagement_PowerAdjustRequestRequest), // 573
  _ENTRY(_DeviceEnergyManagement_StartTimeAdjustRequestRequest), // 574
  _ENTRY(_DeviceEnergyManagement_PauseRequestRequest), // 575
  _ENTRY(_DeviceEnergyManagement_ModifyForecastRequestRequest), // 576
  _ENTRY(_DeviceEnergyManagement_RequestConstraintBasedForecastRequest), // 577
  _ENTRY(_DeviceEnergyManagement_PowerAdjustEnd), // 578
  _ENTRY(_DeviceEnergyManagement_ConstraintsStruct_list_), // 579
  _ENTRY(_DeviceEnergyManagement_CostStruct_list_), // 580
  _ENTRY(_DeviceEnergyManagement_PowerAdjustStruct_list_), // 581
  _ENTRY(_DeviceEnergyManagement_SlotAdjustmentStruct_list_), // 582
  _ENTRY(_DeviceEnergyManagement_SlotStruct_list_), // 583
  _ENTRY(_DeviceEnergyManagement_CauseEnum), // 584
  _ENTRY(_DeviceEnergyManagement_CostTypeEnum), // 585
  _ENTRY(_DeviceEnergyManagement_ESAStateEnum), // 586
  _ENTRY(_DeviceEnergyManagement_ESATypeEnum), // 587
  _ENTRY(_DeviceEnergyManagement_Feature), // 588
  _ENTRY(_EnergyEvse), // 589
  _ENTRY(_EnergyEvse_ChargingTargetStruct), // 590
  _ENTRY(_EnergyEvse_GetTargetsResponse), // 591
  _ENTRY(_EnergyEvse_EnableChargingRequest), // 592
  _ENTRY(_EnergyEvse_EnableDischargingRequest), // 593
  _ENTRY(_EnergyEvse_SetTargetsRequest), // 594
  _ENTRY(_EnergyEvse_GetTargetsRequest), // 595
  _ENTRY(_EnergyEvse_EVConnected), // 596
  _ENTRY(_EnergyEvse_EVNotDetected), // 597
  _ENTRY(_EnergyEvse_EnergyTransferStarted), // 598
  _ENTRY(_EnergyEvse_EnergyTransferStopped), // 599
  _ENTRY(_EnergyEvse_Fault), // 600
  _ENTRY(_EnergyEvse_RFID), // 601
  _ENTRY(_EnergyEvse_ChargingTargetStruct_list_), // 602
  _ENTRY(_EnergyEvse_EnergyTransferStoppedReasonEnum), // 603
  _ENTRY(_EnergyEvse_FaultStateEnum), // 604
  _ENTRY(_EnergyEvse_StateEnum), // 605
  _ENTRY(_EnergyEvse_SupplyStateEnum), // 606
  _ENTRY(_EnergyEvse_Feature), // 607
  _ENTRY(_EnergyEvse_TargetDayOfWeekBitmap), // 608
  _ENTRY(_EnergyPreference), // 609
  _ENTRY(_EnergyPreference_BalanceStruct), // 610
  _ENTRY(_EnergyPreference_BalanceStruct_list_), // 611
  _ENTRY(_EnergyPreference_EnergyPriorityEnum_list_), // 612
  _ENTRY(_EnergyPreference_EnergyPriorityEnum), // 613
  _ENTRY(_EnergyPreference_Feature), // 614
  _ENTRY(_DoorLock), // 615
  _ENTRY(_DoorLock_CredentialStruct), // 616
  _ENTRY(_DoorLock_LockDoorRequest), // 617
  _ENTRY(_DoorLock_UnlockDoorRequest), // 618
  _ENTRY(_DoorLock_UnlockWithTimeoutRequest), // 619
  _ENTRY(_DoorLock_SetWeekDayScheduleRequest), // 620
  _ENTRY(_DoorLock_GetWeekDayScheduleRequest), // 621
  _ENTRY(_DoorLock_GetWeekDayScheduleResponse), // 622
  _ENTRY(_DoorLock_ClearWeekDayScheduleRequest), // 623
  _ENTRY(_DoorLock_SetYearDayScheduleRequest), // 624
  _ENTRY(_DoorLock_GetYearDayScheduleRequest), // 625
  _ENTRY(_DoorLock_GetYearDayScheduleResponse), // 626
  _ENTRY(_DoorLock_ClearYearDayScheduleRequest), // 627
  _ENTRY(_DoorLock_SetHolidayScheduleRequest), // 628
  _ENTRY(_DoorLock_GetHolidayScheduleRequest), // 629
  _ENTRY(_DoorLock_GetHolidayScheduleResponse), // 630
  _ENTRY(_DoorLock_ClearHolidayScheduleRequest), // 631
  _ENTRY(_DoorLock_SetUserRequest), // 632
  _ENTRY(_DoorLock_GetUserRequest), // 633
  _ENTRY(_DoorLock_GetUserResponse), // 634
  _ENTRY(_DoorLock_ClearUserRequest), // 635
  _ENTRY(_DoorLock_SetCredentialRequest), // 636
  _ENTRY(_DoorLock_SetCredentialResponse), // 637
  _ENTRY(_DoorLock_GetCredentialStatusRequest), // 638
  _ENTRY(_DoorLock_GetCredentialStatusResponse), // 639
  _ENTRY(_DoorLock_ClearCredentialRequest), // 640
  _ENTRY(_DoorLock_UnboltDoorRequest), // 641
  _ENTRY(_DoorLock_SetAliroReaderConfigRequest), // 642
  _ENTRY(_DoorLock_DoorLockAlarm), // 643
  _ENTRY(_DoorLock_DoorStateChange), // 644
  _ENTRY(_DoorLock_LockOperation), // 645
  _ENTRY(_DoorLock_LockOperationError), // 646
  _ENTRY(_DoorLock_LockUserChange), // 647
  _ENTRY(_DoorLock_CredentialStruct_list_), // 648
  _ENTRY(_DoorLock_AlarmCodeEnum), // 649
  _ENTRY(_DoorLock_CredentialRuleEnum), // 650
  _ENTRY(_DoorLock_CredentialTypeEnum), // 651
  _ENTRY(_DoorLock_DataOperationTypeEnum), // 652
  _ENTRY(_DoorLock_DlLockState), // 653
  _ENTRY(_DoorLock_DlLockType), // 654
  _ENTRY(_DoorLock_DlStatus), // 655
  _ENTRY(_DoorLock_DoorLockOperationEventCode), // 656
  _ENTRY(_DoorLock_DoorLockProgrammingEventCode), // 657
  _ENTRY(_DoorLock_DoorLockSetPinOrIdStatus), // 658
  _ENTRY(_DoorLock_DoorLockUserStatus), // 659
  _ENTRY(_DoorLock_DoorLockUserType), // 660
  _ENTRY(_DoorLock_DoorStateEnum), // 661
  _ENTRY(_DoorLock_LockDataTypeEnum), // 662
  _ENTRY(_DoorLock_LockOperationTypeEnum), // 663
  _ENTRY(_DoorLock_OperatingModeEnum), // 664
  _ENTRY(_DoorLock_OperationErrorEnum), // 665
  _ENTRY(_DoorLock_OperationSourceEnum), // 666
  _ENTRY(_DoorLock_UserStatusEnum), // 667
  _ENTRY(_DoorLock_UserTypeEnum), // 668
  _ENTRY(_DoorLock_DaysMaskMap), // 669
  _ENTRY(_DoorLock_DlCredentialRuleMask), // 670
  _ENTRY(_DoorLock_DlCredentialRulesSupport), // 671
  _ENTRY(_DoorLock_DlDefaultConfigurationRegister), // 672
  _ENTRY(_DoorLock_DlKeypadOperationEventMask), // 673
  _ENTRY(_DoorLock_DlKeypadProgrammingEventMask), // 674
  _ENTRY(_DoorLock_DlLocalProgrammingFeatures), // 675
  _ENTRY(_DoorLock_DlManualOperationEventMask), // 676
  _ENTRY(_DoorLock_DlRFIDOperationEventMask), // 677
  _ENTRY(_DoorLock_DlRFIDProgrammingEventMask), // 678
  _ENTRY(_DoorLock_DlRemoteOperationEventMask), // 679
  _ENTRY(_DoorLock_DlRemoteProgrammingEventMask), // 680
  _ENTRY(_DoorLock_DlSupportedOperatingModes), // 681
  _ENTRY(_DoorLock_DoorLockDayOfWeek), // 682
  _ENTRY(_DoorLock_Feature), // 683
  _ENTRY(_WindowCovering), // 684
  _ENTRY(_WindowCovering_GoToLiftValueRequest), // 685
  _ENTRY(_WindowCovering_GoToLiftPercentageRequest), // 686
  _ENTRY(_WindowCovering_GoToTiltValueRequest), // 687
  _ENTRY(_WindowCovering_GoToTiltPercentageRequest), // 688
  _ENTRY(_WindowCovering_EndProductType), // 689
  _ENTRY(_WindowCovering_Type), // 690
  _ENTRY(_WindowCovering_ConfigStatus), // 691
  _ENTRY(_WindowCovering_Feature), // 692
  _ENTRY(_WindowCovering_Mode), // 693
  _ENTRY(_WindowCovering_OperationalStatus), // 694
  _ENTRY(_WindowCovering_SafetyStatus), // 695
  _ENTRY(_BarrierControl), // 696
  _ENTRY(_BarrierControl_BarrierControlGoToPercentRequest), // 697
  _ENTRY(_BarrierControl_BarrierControlCapabilities), // 698
  _ENTRY(_BarrierControl_BarrierControlSafetyStatus), // 699
  _ENTRY(_PumpConfigurationAndControl), // 700
  _ENTRY(_PumpConfigurationAndControl_ControlModeEnum), // 701
  _ENTRY(_PumpConfigurationAndControl_OperationModeEnum), // 702
  _ENTRY(_PumpConfigurationAndControl_Feature), // 703
  _ENTRY(_PumpConfigurationAndControl_PumpStatusBitmap), // 704
  _ENTRY(_Thermostat), // 705
  _ENTRY(_Thermostat_WeeklyScheduleTransitionStruct), // 706
  _ENTRY(_Thermostat_SetpointRaiseLowerRequest), // 707
  _ENTRY(_Thermostat_GetWeeklyScheduleResponse), // 708
  _ENTRY(_Thermostat_SetWeeklyScheduleRequest), // 709
  _ENTRY(_Thermostat_GetWeeklyScheduleRequest), // 710
  _ENTRY(_Thermostat_WeeklyScheduleTransitionStruct_list_), // 711
  _ENTRY(_Thermostat_ACCapacityFormatEnum), // 712
  _ENTRY(_Thermostat_ACCompressorTypeEnum), // 713
  _ENTRY(_Thermostat_ACLouverPositionEnum), // 714
  _ENTRY(_Thermostat_ACRefrigerantTypeEnum), // 715
  _ENTRY(_Thermostat_ACTypeEnum), // 716
  _ENTRY(_Thermostat_ControlSequenceOfOperationEnum), // 717
  _ENTRY(_Thermostat_SetpointChangeSourceEnum), // 718
  _ENTRY(_Thermostat_SetpointRaiseLowerModeEnum), // 719
  _ENTRY(_Thermostat_StartOfWeekEnum), // 720
  _ENTRY(_Thermostat_SystemModeEnum), // 721
  _ENTRY(_Thermostat_TemperatureSetpointHoldEnum), // 722
  _ENTRY(_Thermostat_ThermostatRunningModeEnum), // 723
  _ENTRY(_Thermostat_ACErrorCodeBitmap), // 724
  _ENTRY(_Thermostat_Feature), // 725
  _ENTRY(_Thermostat_HVACSystemTypeBitmap), // 726
  _ENTRY(_Thermostat_ProgrammingOperationModeBitmap), // 727
  _ENTRY(_Thermostat_RelayStateBitmap), // 728
  _ENTRY(_Thermostat_RemoteSensingBitmap), // 729
  _ENTRY(_Thermostat_ScheduleDayOfWeekBitmap), // 730
  _ENTRY(_Thermostat_ScheduleModeBitmap), // 731
  _ENTRY(_FanControl), // 732
  _ENTRY(_FanControl_StepRequest), // 733
  _ENTRY(_FanControl_AirflowDirectionEnum), // 734
  _ENTRY(_FanControl_FanModeEnum), // 735
  _ENTRY(_FanControl_FanModeSequenceEnum), // 736
  _ENTRY(_FanControl_StepDirectionEnum), // 737
  _ENTRY(_FanControl_Feature), // 738
  _ENTRY(_FanControl_RockBitmap), // 739
  _ENTRY(_FanControl_WindBitmap), // 740
  _ENTRY(_ThermostatUserInterfaceConfiguration), // 741
  _ENTRY(_ThermostatUserInterfaceConfiguration_KeypadLockoutEnum), // 742
  _ENTRY(_ThermostatUserInterfaceConfiguration_ScheduleProgrammingVisibilityEnum), // 743
  _ENTRY(_ThermostatUserInterfaceConfiguration_TemperatureDisplayModeEnum), // 744
  _ENTRY(_ColorControl), // 745
  _ENTRY(_ColorControl_MoveToHueRequest), // 746
  _ENTRY(_ColorControl_MoveHueRequest), // 747
  _ENTRY(_ColorControl_StepHueRequest), // 748
  _ENTRY(_ColorControl_MoveToSaturationRequest), // 749
  _ENTRY(_ColorControl_MoveSaturationRequest), // 750
  _ENTRY(_ColorControl_StepSaturationRequest), // 751
  _ENTRY(_ColorControl_MoveToHueAndSaturationRequest), // 752
  _ENTRY(_ColorControl_MoveToColorRequest), // 753
  _ENTRY(_ColorControl_MoveColorRequest), // 754
  _ENTRY(_ColorControl_StepColorRequest), // 755
  _ENTRY(_ColorControl_MoveToColorTemperatureRequest), // 756
  _ENTRY(_ColorControl_EnhancedMoveToHueRequest), // 757
  _ENTRY(_ColorControl_EnhancedMoveHueRequest), // 758
  _ENTRY(_ColorControl_EnhancedStepHueRequest), // 759
  _ENTRY(_ColorControl_EnhancedMoveToHueAndSaturationRequest), // 760
  _ENTRY(_ColorControl_ColorLoopSetRequest), // 761
  _ENTRY(_ColorControl_StopMoveStepRequest), // 762
  _ENTRY(_ColorControl_MoveColorTemperatureRequest), // 763
  _ENTRY(_ColorControl_StepColorTemperatureRequest), // 764
  _ENTRY(_ColorControl_ColorLoopAction), // 765
  _ENTRY(_ColorControl_ColorLoopDirection), // 766
  _ENTRY(_ColorControl_ColorMode), // 767
  _ENTRY(_ColorControl_HueDirection), // 768
  _ENTRY(_ColorControl_HueMoveMode), // 769
  _ENTRY(_ColorControl_HueStepMode), // 770
  _ENTRY(_ColorControl_SaturationMoveMode), // 771
  _ENTRY(_ColorControl_SaturationStepMode), // 772
  _ENTRY(_ColorControl_ColorCapabilities), // 773
  _ENTRY(_ColorControl_ColorLoopUpdateFlags), // 774
  _ENTRY(_ColorControl_Feature), // 775
  _ENTRY(_BallastConfiguration), // 776
  _ENTRY(_BallastConfiguration_BallastStatusBitmap), // 777
  _ENTRY(_BallastConfiguration_LampAlarmModeBitmap), // 778
  _ENTRY(_IlluminanceMeasurement), // 779
  _ENTRY(_IlluminanceMeasurement_LightSensorTypeEnum), // 780
  _ENTRY(_TemperatureMeasurement), // 781
  _ENTRY(_PressureMeasurement), // 782
  _ENTRY(_PressureMeasurement_Feature), // 783
  _ENTRY(_FlowMeasurement), // 784
  _ENTRY(_RelativeHumidityMeasurement), // 785
  _ENTRY(_OccupancySensing), // 786
  _ENTRY(_OccupancySensing_OccupancySensorTypeEnum), // 787
  _ENTRY(_OccupancySensing_OccupancyBitmap), // 788
  _ENTRY(_OccupancySensing_OccupancySensorTypeBitmap), // 789
  _ENTRY(_CarbonMonoxideConcentrationMeasurement), // 790
  _ENTRY(_CarbonMonoxideConcentrationMeasurement_LevelValueEnum), // 791
  _ENTRY(_CarbonMonoxideConcentrationMeasurement_MeasurementMediumEnum), // 792
  _ENTRY(_CarbonMonoxideConcentrationMeasurement_MeasurementUnitEnum), // 793
  _ENTRY(_CarbonMonoxideConcentrationMeasurement_Feature), // 794
  _ENTRY(_CarbonDioxideConcentrationMeasurement), // 795
  _ENTRY(_CarbonDioxideConcentrationMeasurement_LevelValueEnum), // 796
  _ENTRY(_CarbonDioxideConcentrationMeasurement_MeasurementMediumEnum), // 797
  _ENTRY(_CarbonDioxideConcentrationMeasurement_MeasurementUnitEnum), // 798
  _ENTRY(_CarbonDioxideConcentrationMeasurement_Feature), // 799
  _ENTRY(_NitrogenDioxideConcentrationMeasurement), // 800
  _ENTRY(_NitrogenDioxideConcentrationMeasurement_LevelValueEnum), // 801
  _ENTRY(_NitrogenDioxideConcentrationMeasurement_MeasurementMediumEnum), // 802
  _ENTRY(_NitrogenDioxideConcentrationMeasurement_MeasurementUnitEnum), // 803
  _ENTRY(_NitrogenDioxideConcentrationMeasurement_Feature), // 804
  _ENTRY(_OzoneConcentrationMeasurement), // 805
  _ENTRY(_OzoneConcentrationMeasurement_LevelValueEnum), // 806
  _ENTRY(_OzoneConcentrationMeasurement_MeasurementMediumEnum), // 807
  _ENTRY(_OzoneConcentrationMeasurement_MeasurementUnitEnum), // 808
  _ENTRY(_OzoneConcentrationMeasurement_Feature), // 809
  _ENTRY(_Pm25ConcentrationMeasurement), // 810
  _ENTRY(_Pm25ConcentrationMeasurement_LevelValueEnum), // 811
  _ENTRY(_Pm25ConcentrationMeasurement_MeasurementMediumEnum), // 812
  _ENTRY(_Pm25ConcentrationMeasurement_MeasurementUnitEnum), // 813
  _ENTRY(_Pm25ConcentrationMeasurement_Feature), // 814
  _ENTRY(_FormaldehydeConcentrationMeasurement), // 815
  _ENTRY(_FormaldehydeConcentrationMeasurement_LevelValueEnum), // 816
  _ENTRY(_FormaldehydeConcentrationMeasurement_MeasurementMediumEnum), // 817
  _ENTRY(_FormaldehydeConcentrationMeasurement_MeasurementUnitEnum), // 818
  _ENTRY(_FormaldehydeConcentrationMeasurement_Feature), // 819
  _ENTRY(_Pm1ConcentrationMeasurement), // 820
  _ENTRY(_Pm1ConcentrationMeasurement_LevelValueEnum), // 821
  _ENTRY(_Pm1ConcentrationMeasurement_MeasurementMediumEnum), // 822
  _ENTRY(_Pm1ConcentrationMeasurement_MeasurementUnitEnum), // 823
  _ENTRY(_Pm1ConcentrationMeasurement_Feature), // 824
  _ENTRY(_Pm10ConcentrationMeasurement), // 825
  _ENTRY(_Pm10ConcentrationMeasurement_LevelValueEnum), // 826
  _ENTRY(_Pm10ConcentrationMeasurement_MeasurementMediumEnum), // 827
  _ENTRY(_Pm10ConcentrationMeasurement_MeasurementUnitEnum), // 828
  _ENTRY(_Pm10ConcentrationMeasurement_Feature), // 829
  _ENTRY(_TotalVolatileOrganicCompoundsConcentrationMeasurement), // 830
  _ENTRY(_TotalVolatileOrganicCompoundsConcentrationMeasurement_LevelValueEnum), // 831
  _ENTRY(_TotalVolatileOrganicCompoundsConcentrationMeasurement_MeasurementMediumEnum), // 832
  _ENTRY(_TotalVolatileOrganicCompoundsConcentrationMeasurement_MeasurementUnitEnum), // 833
  _ENTRY(_TotalVolatileOrganicCompoundsConcentrationMeasurement_Feature), // 834
  _ENTRY(_RadonConcentrationMeasurement), // 835
  _ENTRY(_RadonConcentrationMeasurement_LevelValueEnum), // 836
  _ENTRY(_RadonConcentrationMeasurement_MeasurementMediumEnum), // 837
  _ENTRY(_RadonConcentrationMeasurement_MeasurementUnitEnum), // 838
  _ENTRY(_RadonConcentrationMeasurement_Feature), // 839
  _ENTRY(_WakeOnLan), // 840
  _ENTRY(_Channel), // 841
  _ENTRY(_Channel_ProgramCastStruct), // 842
  _ENTRY(_Channel_ProgramCategoryStruct), // 843
  _ENTRY(_Channel_SeriesInfoStruct), // 844
  _ENTRY(_Channel_ChannelInfoStruct), // 845
  _ENTRY(_Channel_ProgramStruct), // 846
  _ENTRY(_Channel_PageTokenStruct), // 847
  _ENTRY(_Channel_ChannelPagingStruct), // 848
  _ENTRY(_Channel_AdditionalInfoStruct), // 849
  _ENTRY(_Channel_LineupInfoStruct), // 850
  _ENTRY(_Channel_ChangeChannelRequest), // 851
  _ENTRY(_Channel_ChangeChannelResponse), // 852
  _ENTRY(_Channel_ChangeChannelByNumberRequest), // 853
  _ENTRY(_Channel_SkipChannelRequest), // 854
  _ENTRY(_Channel_GetProgramGuideRequest), // 855
  _ENTRY(_Channel_ProgramGuideResponse), // 856
  _ENTRY(_Channel_RecordProgramRequest), // 857
  _ENTRY(_Channel_CancelRecordProgramRequest), // 858
  _ENTRY(_Channel_ProgramStruct_list_), // 859
  _ENTRY(_Channel_ProgramCategoryStruct_list_), // 860
  _ENTRY(_Channel_ChannelInfoStruct_list_), // 861
  _ENTRY(_Channel_ProgramCastStruct_list_), // 862
  _ENTRY(_Channel_AdditionalInfoStruct_list_), // 863
  _ENTRY(_Channel_ChannelTypeEnum), // 864
  _ENTRY(_Channel_LineupInfoTypeEnum), // 865
  _ENTRY(_Channel_StatusEnum), // 866
  _ENTRY(_Channel_Feature), // 867
  _ENTRY(_Channel_RecordingFlagBitmap), // 868
  _ENTRY(_TargetNavigator), // 869
  _ENTRY(_TargetNavigator_TargetInfoStruct), // 870
  _ENTRY(_TargetNavigator_NavigateTargetRequest), // 871
  _ENTRY(_TargetNavigator_NavigateTargetResponse), // 872
  _ENTRY(_TargetNavigator_TargetUpdated), // 873
  _ENTRY(_TargetNavigator_TargetInfoStruct_list_), // 874
  _ENTRY(_TargetNavigator_StatusEnum), // 875
  _ENTRY(_MediaPlayback), // 876
  _ENTRY(_MediaPlayback_TrackAttributesStruct), // 877
  _ENTRY(_MediaPlayback_TrackStruct), // 878
  _ENTRY(_MediaPlayback_PlaybackPositionStruct), // 879
  _ENTRY(_MediaPlayback_RewindRequest), // 880
  _ENTRY(_MediaPlayback_FastForwardRequest), // 881
  _ENTRY(_MediaPlayback_SkipForwardRequest), // 882
  _ENTRY(_MediaPlayback_SkipBackwardRequest), // 883
  _ENTRY(_MediaPlayback_PlaybackResponse), // 884
  _ENTRY(_MediaPlayback_SeekRequest), // 885
  _ENTRY(_MediaPlayback_ActivateAudioTrackRequest), // 886
  _ENTRY(_MediaPlayback_ActivateTextTrackRequest), // 887
  _ENTRY(_MediaPlayback_StateChanged), // 888
  _ENTRY(_MediaPlayback_TrackStruct_list_), // 889
  _ENTRY(_MediaPlayback_CharacteristicEnum), // 890
  _ENTRY(_MediaPlayback_PlaybackStateEnum), // 891
  _ENTRY(_MediaPlayback_StatusEnum), // 892
  _ENTRY(_MediaPlayback_Feature), // 893
  _ENTRY(_MediaInput), // 894
  _ENTRY(_MediaInput_InputInfoStruct), // 895
  _ENTRY(_MediaInput_SelectInputRequest), // 896
  _ENTRY(_MediaInput_RenameInputRequest), // 897
  _ENTRY(_MediaInput_InputInfoStruct_list_), // 898
  _ENTRY(_MediaInput_InputTypeEnum), // 899
  _ENTRY(_MediaInput_Feature), // 900
  _ENTRY(_LowPower), // 901
  _ENTRY(_KeypadInput), // 902
  _ENTRY(_KeypadInput_SendKeyRequest), // 903
  _ENTRY(_KeypadInput_SendKeyResponse), // 904
  _ENTRY(_KeypadInput_CECKeyCodeEnum), // 905
  _ENTRY(_KeypadInput_StatusEnum), // 906
  _ENTRY(_KeypadInput_Feature), // 907
  _ENTRY(_ContentLauncher), // 908
  _ENTRY(_ContentLauncher_DimensionStruct), // 909
  _ENTRY(_ContentLauncher_TrackPreferenceStruct), // 910
  _ENTRY(_ContentLauncher_PlaybackPreferencesStruct), // 911
  _ENTRY(_ContentLauncher_AdditionalInfoStruct), // 912
  _ENTRY(_ContentLauncher_ParameterStruct), // 913
  _ENTRY(_ContentLauncher_ContentSearchStruct), // 914
  _ENTRY(_ContentLauncher_StyleInformationStruct), // 915
  _ENTRY(_ContentLauncher_BrandingInformationStruct), // 916
  _ENTRY(_ContentLauncher_LaunchContentRequest), // 917
  _ENTRY(_ContentLauncher_LaunchURLRequest), // 918
  _ENTRY(_ContentLauncher_LauncherResponse), // 919
  _ENTRY(_ContentLauncher_TrackPreferenceStruct_list_), // 920
  _ENTRY(_ContentLauncher_CharacteristicEnum_list_), // 921
  _ENTRY(_ContentLauncher_ParameterStruct_list_), // 922
  _ENTRY(_ContentLauncher_AdditionalInfoStruct_list_), // 923
  _ENTRY(_ContentLauncher_CharacteristicEnum), // 924
  _ENTRY(_ContentLauncher_MetricTypeEnum), // 925
  _ENTRY(_ContentLauncher_ParameterEnum), // 926
  _ENTRY(_ContentLauncher_StatusEnum), // 927
  _ENTRY(_ContentLauncher_Feature), // 928
  _ENTRY(_ContentLauncher_SupportedProtocolsBitmap), // 929
  _ENTRY(_AudioOutput), // 930
  _ENTRY(_AudioOutput_OutputInfoStruct), // 931
  _ENTRY(_AudioOutput_SelectOutputRequest), // 932
  _ENTRY(_AudioOutput_RenameOutputRequest), // 933
  _ENTRY(_AudioOutput_OutputInfoStruct_list_), // 934
  _ENTRY(_AudioOutput_OutputTypeEnum), // 935
  _ENTRY(_AudioOutput_Feature), // 936
  _ENTRY(_ApplicationLauncher), // 937
  _ENTRY(_ApplicationLauncher_ApplicationStruct), // 938
  _ENTRY(_ApplicationLauncher_ApplicationEPStruct), // 939
  _ENTRY(_ApplicationLauncher_LaunchAppRequest), // 940
  _ENTRY(_ApplicationLauncher_StopAppRequest), // 941
  _ENTRY(_ApplicationLauncher_HideAppRequest), // 942
  _ENTRY(_ApplicationLauncher_LauncherResponse), // 943
  _ENTRY(_ApplicationLauncher_StatusEnum), // 944
  _ENTRY(_ApplicationLauncher_Feature), // 945
  _ENTRY(_ApplicationBasic), // 946
  _ENTRY(_ApplicationBasic_ApplicationStruct), // 947
  _ENTRY(_ApplicationBasic_ApplicationStatusEnum), // 948
  _ENTRY(_AccountLogin), // 949
  _ENTRY(_AccountLogin_GetSetupPINRequest), // 950
  _ENTRY(_AccountLogin_GetSetupPINResponse), // 951
  _ENTRY(_AccountLogin_LoginRequest), // 952
  _ENTRY(_AccountLogin_LogoutRequest), // 953
  _ENTRY(_AccountLogin_LoggedOut), // 954
  _ENTRY(_ContentControl), // 955
  _ENTRY(_ContentControl_RatingNameStruct), // 956
  _ENTRY(_ContentControl_UpdatePINRequest), // 957
  _ENTRY(_ContentControl_ResetPINResponse), // 958
  _ENTRY(_ContentControl_AddBonusTimeRequest), // 959
  _ENTRY(_ContentControl_SetScreenDailyTimeRequest), // 960
  _ENTRY(_ContentControl_SetOnDemandRatingThresholdRequest), // 961
  _ENTRY(_ContentControl_SetScheduledContentRatingThresholdRequest), // 962
  _ENTRY(_ContentControl_RatingNameStruct_list_), // 963
  _ENTRY(_ContentControl_Feature), // 964
  _ENTRY(_ContentAppObserver), // 965
  _ENTRY(_ContentAppObserver_ContentAppMessageRequest), // 966
  _ENTRY(_ContentAppObserver_ContentAppMessageResponse), // 967
  _ENTRY(_ContentAppObserver_StatusEnum), // 968
  _ENTRY(_ElectricalMeasurement), // 969
  _ENTRY(_ElectricalMeasurement_GetProfileInfoResponseCommand), // 970
  _ENTRY(_ElectricalMeasurement_GetMeasurementProfileResponseCommand), // 971
  _ENTRY(_ElectricalMeasurement_GetMeasurementProfileCommandRequest), // 972
  _ENTRY(_UnitTesting), // 973
  _ENTRY(_UnitTesting_SimpleStruct), // 974
  _ENTRY(_UnitTesting_TestFabricScoped), // 975
  _ENTRY(_UnitTesting_NullablesAndOptionalsStruct), // 976
  _ENTRY(_UnitTesting_NestedStruct), // 977
  _ENTRY(_UnitTesting_NestedStructList), // 978
  _ENTRY(_UnitTesting_DoubleNestedStructList), // 979
  _ENTRY(_UnitTesting_TestListStructOctet), // 980
  _ENTRY(_UnitTesting_TestSpecificResponse), // 981
  _ENTRY(_UnitTesting_TestAddArgumentsResponse), // 982
  _ENTRY(_UnitTesting_TestSimpleArgumentResponse), // 983
  _ENTRY(_UnitTesting_TestStructArrayArgumentResponse), // 984
  _ENTRY(_UnitTesting_TestAddArgumentsRequest), // 985
  _ENTRY(_UnitTesting_TestListInt8UReverseResponse), // 986
  _ENTRY(_UnitTesting_TestSimpleArgumentRequestRequest), // 987
  _ENTRY(_UnitTesting_TestEnumsResponse), // 988
  _ENTRY(_UnitTesting_TestStructArrayArgumentRequestRequest), // 989
  _ENTRY(_UnitTesting_TestNullableOptionalResponse), // 990
  _ENTRY(_UnitTesting_TestStructArgumentRequestRequest), // 991
  _ENTRY(_UnitTesting_TestComplexNullableOptionalResponse), // 992
  _ENTRY(_UnitTesting_TestNestedStructArgumentRequestRequest), // 993
  _ENTRY(_UnitTesting_BooleanResponse), // 994
  _ENTRY(_UnitTesting_TestListStructArgumentRequestRequest), // 995
  _ENTRY(_UnitTesting_SimpleStructResponse), // 996
  _ENTRY(_UnitTesting_TestListInt8UArgumentRequestRequest), // 997
  _ENTRY(_UnitTesting_TestEmitTestEventResponse), // 998
  _ENTRY(_UnitTesting_TestNestedStructListArgumentRequestRequest), // 999
  _ENTRY(_UnitTesting_TestEmitTestFabricScopedEventResponse), // 1000
  _ENTRY(_UnitTesting_TestListNestedStructListArgumentRequestRequest), // 1001
  _ENTRY(_UnitTesting_TestBatchHelperResponse), // 1002
  _ENTRY(_UnitTesting_TestListInt8UReverseRequestRequest), // 1003
  _ENTRY(_UnitTesting_TestEnumsRequestRequest), // 1004
  _ENTRY(_UnitTesting_TestNullableOptionalRequestRequest), // 1005
  _ENTRY(_UnitTesting_TestComplexNullableOptionalRequestRequest), // 1006
  _ENTRY(_UnitTesting_SimpleStructEchoRequestRequest), // 1007
  _ENTRY(_UnitTesting_TestSimpleOptionalArgumentRequestRequest), // 1008
  _ENTRY(_UnitTesting_TestEmitTestEventRequestRequest), // 1009
  _ENTRY(_UnitTesting_TestEmitTestFabricScopedEventRequestRequest), // 1010
  _ENTRY(_UnitTesting_TestBatchHelperRequestRequest), // 1011
  _ENTRY(_UnitTesting_TestSecondBatchHelperRequestRequest), // 1012
  _ENTRY(_UnitTesting_TestEvent), // 1013
  _ENTRY(_UnitTesting_TestFabricScopedEvent), // 1014
  _ENTRY(_UnitTesting_NullablesAndOptionalsStruct_list_), // 1015
  _ENTRY(_UnitTesting_TestListStructOctet_list_), // 1016
  _ENTRY(_UnitTesting_TestFabricScoped_list_), // 1017
  _ENTRY(_UnitTesting_SimpleStruct_list_), // 1018
  _ENTRY(_UnitTesting_SimpleEnum_list_), // 1019
  _ENTRY(_UnitTesting_NestedStructList_list_), // 1020
  _ENTRY(_UnitTesting_SimpleEnum), // 1021
  _ENTRY(_UnitTesting_Bitmap16MaskMap), // 1022
  _ENTRY(_UnitTesting_Bitmap32MaskMap), // 1023
  _ENTRY(_UnitTesting_Bitmap64MaskMap), // 1024
  _ENTRY(_UnitTesting_Bitmap8MaskMap), // 1025
  _ENTRY(_UnitTesting_SimpleBitmap), // 1026
  _ENTRY(_FaultInjection), // 1027
  _ENTRY(_FaultInjection_FailAtFaultRequest), // 1028
  _ENTRY(_FaultInjection_FailRandomlyAtFaultRequest), // 1029
  _ENTRY(_FaultInjection_FaultType), // 1030
  _ENTRY(_SampleMei), // 1031
  _ENTRY(_SampleMei_AddArgumentsResponse), // 1032
  _ENTRY(_SampleMei_AddArgumentsRequest), // 1033
  _ENTRY(_SampleMei_PingCountEvent), // 1034
} };

} // namespace TLVMeta
} // namespace chip
