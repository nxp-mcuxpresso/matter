#include <tlv/meta/protocols_meta.h>

namespace chip {
namespace TLVMeta {
namespace {

using namespace chip::FlatTree;
using namespace chip::TLV;

const Entry<ItemInfo> _SecureChannelProtocol[] = {
  { { AttributeTag(0), "msg_counter_sync_request", ItemType::kProtocolBinaryData }, kInvalidNodeIndex }, // SecureChannelProtocol::protocol_binary_data
  { { AttributeTag(1), "msg_counter_sync_response", ItemType::kProtocolBinaryData }, kInvalidNodeIndex }, // SecureChannelProtocol::protocol_binary_data
  { { AttributeTag(16), "mrp_ack", ItemType::kProtocolBinaryData }, kInvalidNodeIndex }, // SecureChannelProtocol::protocol_binary_data
  { { AttributeTag(32), "pbkdf_param_request", ItemType::kDefault }, 4 }, // SecureChannelProtocol::PBKDFParamRequest
  { { AttributeTag(33), "pbkdf_param_response", ItemType::kDefault }, 6 }, // SecureChannelProtocol::PBKDFParamResponse
  { { AttributeTag(34), "pase_pake1", ItemType::kDefault }, 7 }, // SecureChannelProtocol::PasePake1
  { { AttributeTag(35), "pase_pake2", ItemType::kDefault }, 8 }, // SecureChannelProtocol::PasePake2
  { { AttributeTag(36), "pase_pake3", ItemType::kDefault }, 9 }, // SecureChannelProtocol::PasePake3
  { { AttributeTag(48), "case_sigma1", ItemType::kDefault }, 10 }, // SecureChannelProtocol::CaseSigma1
  { { AttributeTag(49), "case_sigma2", ItemType::kDefault }, 11 }, // SecureChannelProtocol::CaseSigma2
  { { AttributeTag(50), "case_sigma3", ItemType::kDefault }, 12 }, // SecureChannelProtocol::CaseSigma3
  { { AttributeTag(51), "case_sigma2_resume", ItemType::kDefault }, 13 }, // SecureChannelProtocol::CaseSigma2Resume
  { { AttributeTag(64), "status_report", ItemType::kProtocolBinaryData }, kInvalidNodeIndex }, // SecureChannelProtocol::protocol_binary_data
  { { AttributeTag(80), "icd_checkin", ItemType::kProtocolBinaryData }, kInvalidNodeIndex }, // SecureChannelProtocol::protocol_binary_data
};

const Entry<ItemInfo> _SecureChannelProtocol_SessionParameterStruct[] = {
  { { ContextTag(1), "session_idle_interval", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int32u
  { { ContextTag(2), "session_active_interval", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int32u
  { { ContextTag(3), "session_active_threshold", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(4), "data_model_revision", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(5), "interaction_model_revision", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(6), "specification_version", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int32u
  { { ContextTag(7), "max_paths_per_invoke", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
};

const Entry<ItemInfo> _SecureChannelProtocol_PBKDFParamRequest[] = {
  { { ContextTag(1), "initiator_random", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(2), "initiator_session_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(3), "passcode_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(4), "has_pbkdf_parameters", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::boolean
  { { ContextTag(5), "initiator_session_params", ItemType::kDefault }, 3 }, // SecureChannelProtocol::SessionParameterStruct
};

const Entry<ItemInfo> _SecureChannelProtocol_CryptoPBKDFParameterSet[] = {
  { { ContextTag(1), "iterations", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int32u
  { { ContextTag(2), "salt", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
};

const Entry<ItemInfo> _SecureChannelProtocol_PBKDFParamResponse[] = {
  { { ContextTag(1), "initiator_random", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(2), "responder_random", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(3), "responder_session_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(4), "pbkdf_parameters", ItemType::kDefault }, 5 }, // SecureChannelProtocol::CryptoPBKDFParameterSet
  { { ContextTag(5), "responder_icd_params", ItemType::kDefault }, 3 }, // SecureChannelProtocol::SessionParameterStruct
};

const Entry<ItemInfo> _SecureChannelProtocol_PasePake1[] = {
  { { ContextTag(1), "pA", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
};

const Entry<ItemInfo> _SecureChannelProtocol_PasePake2[] = {
  { { ContextTag(1), "pB", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(2), "cB", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
};

const Entry<ItemInfo> _SecureChannelProtocol_PasePake3[] = {
  { { ContextTag(1), "cA", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
};

const Entry<ItemInfo> _SecureChannelProtocol_CaseSigma1[] = {
  { { ContextTag(1), "initiator_random", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(2), "initiator_session_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(3), "destination_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(4), "initiator_eph_pub_key", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(5), "initiator_session_params", ItemType::kDefault }, 3 }, // SecureChannelProtocol::SessionParameterStruct
  { { ContextTag(6), "resumption_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(7), "initiator_resume_mic", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
};

const Entry<ItemInfo> _SecureChannelProtocol_CaseSigma2[] = {
  { { ContextTag(1), "responder_random", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(2), "responder_sessoion_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(3), "responder_eph_pub_key", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(4), "encrypted2", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(5), "responder_session_params", ItemType::kDefault }, 3 }, // SecureChannelProtocol::SessionParameterStruct
};

const Entry<ItemInfo> _SecureChannelProtocol_CaseSigma3[] = {
  { { ContextTag(1), "encrypted", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
};

const Entry<ItemInfo> _SecureChannelProtocol_CaseSigma2Resume[] = {
  { { ContextTag(1), "resumption_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(2), "sigma2_resume_mic", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::octet_string
  { { ContextTag(3), "responder_sessoion_id", ItemType::kDefault }, kInvalidNodeIndex }, // SecureChannelProtocol::int16u
  { { ContextTag(4), "responder_session_params", ItemType::kDefault }, 3 }, // SecureChannelProtocol::SessionParameterStruct
};

const Entry<ItemInfo> _IMProtocol[] = {
  { { AttributeTag(1), "status_response", ItemType::kDefault }, 20 }, // IMProtocol::StatusResponseMessage
  { { AttributeTag(2), "read_request", ItemType::kDefault }, 21 }, // IMProtocol::ReadRequestMessage
  { { AttributeTag(3), "subscribe_request", ItemType::kDefault }, 22 }, // IMProtocol::SubscribeRequestMessage
  { { AttributeTag(4), "subscribe_response", ItemType::kDefault }, 23 }, // IMProtocol::SubscribeResponseMessage
  { { AttributeTag(5), "report_data", ItemType::kDefault }, 32 }, // IMProtocol::ReportDataMessage
  { { AttributeTag(6), "write_request", ItemType::kDefault }, 34 }, // IMProtocol::WriteRequestMessage
  { { AttributeTag(7), "write_response", ItemType::kDefault }, 36 }, // IMProtocol::WriteResponseMessage
  { { AttributeTag(8), "invoke_request", ItemType::kDefault }, 40 }, // IMProtocol::InvokeRequestMessage
  { { AttributeTag(9), "invoke_response", ItemType::kDefault }, 43 }, // IMProtocol::InvokeResponseMessage
  { { AttributeTag(10), "timed_request", ItemType::kDefault }, 44 }, // IMProtocol::TimedRequestMessage
};

const Entry<ItemInfo> _IMProtocol_AttributePathIB[] = {
  { { ContextTag(0), "enable_tag_compression", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(1), "node_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(2), "endpoint_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(3), "cluster_id", ItemType::kProtocolClusterId }, kInvalidNodeIndex }, // IMProtocol::protocol_cluster_id
  { { ContextTag(4), "attribute_id", ItemType::kProtocolAttributeId }, kInvalidNodeIndex }, // IMProtocol::protocol_attribute_id
  { { ContextTag(5), "list_index", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
};

const Entry<ItemInfo> _IMProtocol_EventPathIB[] = {
  { { ContextTag(0), "node_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(1), "endpoint_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(2), "cluster_id", ItemType::kProtocolClusterId }, kInvalidNodeIndex }, // IMProtocol::protocol_cluster_id
  { { ContextTag(3), "event_id", ItemType::kProtocolEventId }, kInvalidNodeIndex }, // IMProtocol::protocol_event_id
  { { ContextTag(4), "is_urgent", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
};

const Entry<ItemInfo> _IMProtocol_EventFilterIB[] = {
  { { ContextTag(0), "node_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(1), "event_min", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
};

const Entry<ItemInfo> _IMProtocol_ClusterPathIB[] = {
  { { ContextTag(0), "node_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(1), "endpoint_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(2), "cluster_id", ItemType::kProtocolClusterId }, kInvalidNodeIndex }, // IMProtocol::protocol_cluster_id
};

const Entry<ItemInfo> _IMProtocol_DataVersionFilterIB[] = {
  { { ContextTag(0), "path", ItemType::kDefault }, 18 }, // IMProtocol::ClusterPathIB
  { { ContextTag(1), "data_version", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int32u
};

const Entry<ItemInfo> _IMProtocol_StatusResponseMessage[] = {
  { { ContextTag(0), "status", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_ReadRequestMessage[] = {
  { { ContextTag(0), "attribute_requests", ItemType::kList }, 46 }, // IMProtocol::AttributePathIB[]
  { { ContextTag(1), "event_requests", ItemType::kList }, 51 }, // IMProtocol::EventPathIB[]
  { { ContextTag(2), "event_filters", ItemType::kList }, 54 }, // IMProtocol::EventFilterIB[]
  { { ContextTag(3), "fabric_filtered", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(4), "data_version_filters", ItemType::kList }, 52 }, // IMProtocol::DataVersionFilterIB[]
  { { ContextTag(5), "include_attribution_data", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_SubscribeRequestMessage[] = {
  { { ContextTag(0), "keep_subscriptions", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(1), "min_minterval_floor", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(2), "max_minterval_ceiling", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(3), "attribute_requests", ItemType::kList }, 46 }, // IMProtocol::AttributePathIB[]
  { { ContextTag(4), "event_requests", ItemType::kList }, 51 }, // IMProtocol::EventPathIB[]
  { { ContextTag(5), "event_filters", ItemType::kList }, 54 }, // IMProtocol::EventFilterIB[]
  { { ContextTag(7), "fabric_filtered", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(8), "data_version_filters", ItemType::kList }, 52 }, // IMProtocol::DataVersionFilterIB[]
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_SubscribeResponseMessage[] = {
  { { ContextTag(0), "subscription_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int32u
  { { ContextTag(2), "max_interval", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_StatusIB[] = {
  { { ContextTag(0), "status", ItemType::kEnum }, 55 }, // IMProtocol::StatusCodeEnum
  { { ContextTag(1), "cluster_status", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_AttributeStatus[] = {
  { { ContextTag(0), "path", ItemType::kDefault }, 15 }, // IMProtocol::AttributePathIB
  { { ContextTag(1), "status", ItemType::kDefault }, 24 }, // IMProtocol::StatusIB
};

const Entry<ItemInfo> _IMProtocol_AttributionData[] = {
  { { ContextTag(1), "context_information", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
  { { ContextTag(2), "source_context", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int32u
  { { ContextTag(3), "node_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(4), "group_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(5), "epoch_timestamp", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(6), "system_timestamp", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(254), "fabric_index", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_AttributeData[] = {
  { { ContextTag(0), "data_version", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int32u
  { { ContextTag(1), "path", ItemType::kDefault }, 15 }, // IMProtocol::AttributePathIB
  { { ContextTag(2), "data", ItemType::kProtocolPayloadAttribute }, kInvalidNodeIndex }, // IMProtocol::cluster_attribute_payload
};

const Entry<ItemInfo> _IMProtocol_AttributeReportIB[] = {
  { { ContextTag(0), "attribute_status", ItemType::kDefault }, 25 }, // IMProtocol::AttributeStatus
  { { ContextTag(1), "attribute_data", ItemType::kDefault }, 27 }, // IMProtocol::AttributeData
};

const Entry<ItemInfo> _IMProtocol_EventStatusIB[] = {
  { { ContextTag(0), "path", ItemType::kDefault }, 16 }, // IMProtocol::EventPathIB
  { { ContextTag(1), "status", ItemType::kDefault }, 24 }, // IMProtocol::StatusIB
};

const Entry<ItemInfo> _IMProtocol_EventDataIB[] = {
  { { ContextTag(0), "path", ItemType::kDefault }, 16 }, // IMProtocol::EventPathIB
  { { ContextTag(1), "event_number", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(2), "priority", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
  { { ContextTag(3), "epoch_timestamp", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(4), "system_timestamp", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(5), "delta_epoch_timestamp", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(6), "delta_system_timestamp", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int64u
  { { ContextTag(7), "data", ItemType::kProtocolPayloadEvent }, kInvalidNodeIndex }, // IMProtocol::cluster_event_payload
  { { ContextTag(8), "attribution_data", ItemType::kDefault }, 26 }, // IMProtocol::AttributionData
};

const Entry<ItemInfo> _IMProtocol_EventReportIB[] = {
  { { ContextTag(0), "event_status", ItemType::kDefault }, 29 }, // IMProtocol::EventStatusIB
  { { ContextTag(1), "event_data", ItemType::kDefault }, 30 }, // IMProtocol::EventDataIB
};

const Entry<ItemInfo> _IMProtocol_ReportDataMessage[] = {
  { { ContextTag(0), "subscription_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int32u
  { { ContextTag(1), "attribute_reports", ItemType::kList }, 49 }, // IMProtocol::AttributeReportIB[]
  { { ContextTag(2), "event_reports", ItemType::kList }, 50 }, // IMProtocol::EventReportIB[]
  { { ContextTag(3), "more_cunked_messages", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(4), "suppress_response", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_AttributeDataIB[] = {
  { { ContextTag(0), "data_version", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int32u
  { { ContextTag(1), "path", ItemType::kDefault }, 15 }, // IMProtocol::AttributePathIB
  { { ContextTag(2), "data", ItemType::kProtocolPayloadAttribute }, kInvalidNodeIndex }, // IMProtocol::cluster_attribute_payload
  { { ContextTag(3), "attribution_data", ItemType::kDefault }, 26 }, // IMProtocol::AttributionData
};

const Entry<ItemInfo> _IMProtocol_WriteRequestMessage[] = {
  { { ContextTag(0), "suppres_response", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(1), "timed_request", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(2), "write_requests", ItemType::kList }, 45 }, // IMProtocol::AttributeDataIB[]
  { { ContextTag(3), "more_chunked_messages", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_AttributeStatusIB[] = {
  { { ContextTag(0), "path", ItemType::kDefault }, 15 }, // IMProtocol::AttributePathIB
  { { ContextTag(1), "status", ItemType::kDefault }, 24 }, // IMProtocol::StatusIB
};

const Entry<ItemInfo> _IMProtocol_WriteResponseMessage[] = {
  { { ContextTag(0), "write_responses", ItemType::kList }, 47 }, // IMProtocol::AttributeStatusIB[]
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_CommandPathIB[] = {
  { { ContextTag(0), "endpoint_id", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(1), "cluster_id", ItemType::kProtocolClusterId }, kInvalidNodeIndex }, // IMProtocol::protocol_cluster_id
  { { ContextTag(2), "command_id", ItemType::kProtocolCommandId }, kInvalidNodeIndex }, // IMProtocol::protocol_command_id
};

const Entry<ItemInfo> _IMProtocol_CommandDataIB[] = {
  { { ContextTag(0), "path", ItemType::kDefault }, 37 }, // IMProtocol::CommandPathIB
  { { ContextTag(1), "data", ItemType::kProtocolPayloadCommand }, kInvalidNodeIndex }, // IMProtocol::cluster_command_payload
  { { ContextTag(2), "command_ref", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(3), "supplied_attribution_data", ItemType::kDefault }, 39 }, // IMProtocol::SuppliedAttribution
  { { ContextTag(4), "supplied_attribution_data_repeat", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
};

const Entry<ItemInfo> _IMProtocol_SuppliedAttribution[] = {
  { { ContextTag(0), "context_information", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::SuppliedAttributeContextInformation
  { { ContextTag(1), "source_context", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int32u
};

const Entry<ItemInfo> _IMProtocol_InvokeRequestMessage[] = {
  { { ContextTag(0), "suppress_response", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(1), "timed_request", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(2), "invoke_requests", ItemType::kList }, 48 }, // IMProtocol::CommandDataIB[]
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_CommandStatusIB[] = {
  { { ContextTag(0), "path", ItemType::kDefault }, 37 }, // IMProtocol::CommandPathIB
  { { ContextTag(1), "status", ItemType::kDefault }, 24 }, // IMProtocol::StatusIB
};

const Entry<ItemInfo> _IMProtocol_InvokeResponseIB[] = {
  { { ContextTag(0), "command", ItemType::kDefault }, 38 }, // IMProtocol::CommandDataIB
  { { ContextTag(1), "status", ItemType::kDefault }, 41 }, // IMProtocol::CommandStatusIB
};

const Entry<ItemInfo> _IMProtocol_InvokeResponseMessage[] = {
  { { ContextTag(0), "suppress_response", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(1), "invoke_responses", ItemType::kList }, 53 }, // IMProtocol::InvokeResponseIB[]
  { { ContextTag(2), "more_chunked_messages", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::boolean
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_TimedRequestMessage[] = {
  { { ContextTag(0), "timeout", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int16u
  { { ContextTag(255), "interaction_model_revison", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::int8u
};

const Entry<ItemInfo> _IMProtocol_AttributeDataIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 33 }, // IMProtocol_AttributeDataIB[]
};

const Entry<ItemInfo> _IMProtocol_AttributePathIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 15 }, // IMProtocol_AttributePathIB[]
};

const Entry<ItemInfo> _IMProtocol_AttributeStatusIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 35 }, // IMProtocol_AttributeStatusIB[]
};

const Entry<ItemInfo> _IMProtocol_CommandDataIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 38 }, // IMProtocol_CommandDataIB[]
};

const Entry<ItemInfo> _IMProtocol_AttributeReportIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 28 }, // IMProtocol_AttributeReportIB[]
};

const Entry<ItemInfo> _IMProtocol_EventReportIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 31 }, // IMProtocol_EventReportIB[]
};

const Entry<ItemInfo> _IMProtocol_EventPathIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 16 }, // IMProtocol_EventPathIB[]
};

const Entry<ItemInfo> _IMProtocol_DataVersionFilterIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 19 }, // IMProtocol_DataVersionFilterIB[]
};

const Entry<ItemInfo> _IMProtocol_InvokeResponseIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 42 }, // IMProtocol_InvokeResponseIB[]
};

const Entry<ItemInfo> _IMProtocol_EventFilterIB_list_[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, 17 }, // IMProtocol_EventFilterIB[]
};

const Entry<ItemInfo> _IMProtocol_StatusCodeEnum[] = {
  { { ConstantValueTag(0x0), "kSuccess", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kSuccess
  { { ConstantValueTag(0x1), "kFailure", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kFailure
  { { ConstantValueTag(0x7D), "kInvalidSubscription", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kInvalidSubscription
  { { ConstantValueTag(0x7E), "kUnsupportedAccess", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnsupportedAccess
  { { ConstantValueTag(0x7F), "kUnsupportedEndpoint", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnsupportedEndpoint
  { { ConstantValueTag(0x80), "kInvalidAction", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kInvalidAction
  { { ConstantValueTag(0x81), "kUnsupportedCommand", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnsupportedCommand
  { { ConstantValueTag(0x85), "kInvalidCommand", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kInvalidCommand
  { { ConstantValueTag(0x86), "kUnsupportedAttribute", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnsupportedAttribute
  { { ConstantValueTag(0x87), "kConstraintError", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kConstraintError
  { { ConstantValueTag(0x88), "kUnsupportedWrite", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnsupportedWrite
  { { ConstantValueTag(0x89), "kResourceExhausted", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kResourceExhausted
  { { ConstantValueTag(0x8B), "kNotFound", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kNotFound
  { { ConstantValueTag(0x8C), "kUnreportableAttribute", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnreportableAttribute
  { { ConstantValueTag(0x8D), "kInvalidDataType", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kInvalidDataType
  { { ConstantValueTag(0x8F), "kUnsupportedRead", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnsupportedRead
  { { ConstantValueTag(0x92), "kDataVersionMismatch", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kDataVersionMismatch
  { { ConstantValueTag(0x94), "kTimeout", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kTimeout
  { { ConstantValueTag(0x9C), "kBusy", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kBusy
  { { ConstantValueTag(0xC3), "kUnsupportedCluster", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnsupportedCluster
  { { ConstantValueTag(0xC5), "kNoUpstreamSubscription", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kNoUpstreamSubscription
  { { ConstantValueTag(0xC6), "kNeedsTimedInteraction", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kNeedsTimedInteraction
  { { ConstantValueTag(0xC7), "kUnsupportedEvent", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kUnsupportedEvent
  { { ConstantValueTag(0xC8), "kPathsExhausted", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kPathsExhausted
  { { ConstantValueTag(0xC9), "kTimedRequestMismatch", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kTimedRequestMismatch
  { { ConstantValueTag(0xCA), "kFailsafeRequired", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kFailsafeRequired
  { { ConstantValueTag(0xCB), "kInvalidInState", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kInvalidInState
  { { ConstantValueTag(0xCC), "kNoCommandResponse", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kNoCommandResponse
  { { ConstantValueTag(0xF0), "kWriteIgnored", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::StatusCodeEnum::kWriteIgnored
};

const Entry<ItemInfo> _IMProtocol_SuppliedAttributionContextInformation[] = {
  { { ConstantValueTag(0x0), "kDefaultClientAction", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::SuppliedAttributionContextInformation::kDefaultClientAction
  { { ConstantValueTag(0x1), "kUserInteraction", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::SuppliedAttributionContextInformation::kUserInteraction
  { { ConstantValueTag(0x2), "kClientAutomationRule", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::SuppliedAttributionContextInformation::kClientAutomationRule
  { { ConstantValueTag(0x3), "kClientSchedule", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::SuppliedAttributionContextInformation::kClientSchedule
  { { ConstantValueTag(0x4), "kClientTimer", ItemType::kDefault }, kInvalidNodeIndex }, // IMProtocol::SuppliedAttributionContextInformation::kClientTimer
};

const Entry<ItemInfo> _BdxProtocol[] = {
  { { AttributeTag(1), "send_init", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(2), "send_accept", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(4), "receive_init", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(5), "receive_accept", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(16), "block_query", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(17), "block", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(18), "block_eof", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(19), "block_ack", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(20), "block_ack_eof", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
  { { AttributeTag(21), "block_query_with_skip", ItemType::kDefault }, kInvalidNodeIndex }, // BdxProtocol::octet_string
};

const Entry<ItemInfo> _UserDirectedCommissioningProtocol[] = {
  { { AttributeTag(0), "identification_declaration", ItemType::kDefault }, 59 }, // UserDirectedCommissioningProtocol::IdentificationDeclarationStruct
};

const Entry<ItemInfo> _UserDirectedCommissioningProtocol_IdentificationDeclarationStruct[] = {
  { { ContextTag(1), "instance_name", ItemType::kDefault }, kInvalidNodeIndex }, // UserDirectedCommissioningProtocol::octet_string
};

const Entry<ItemInfo> _all_clusters[] = {
  { { ClusterTag(0xFFFF0000), "SecureChannelProtocol", ItemType::kDefault }, 2 },
  { { ClusterTag(0xFFFF0001), "IMProtocol", ItemType::kDefault }, 14 },
  { { ClusterTag(0xFFFF0002), "BdxProtocol", ItemType::kDefault }, 57 },
  { { ClusterTag(0xFFFF0003), "UserDirectedCommissioningProtocol", ItemType::kDefault }, 58 },

};

// For any non-structure list like u64[] or similar.
const Entry<ItemInfo> _primitive_type_list[] = {
  { { AnonymousTag(), "Anonymous<>", ItemType::kDefault }, kInvalidNodeIndex },
};

} // namespace

#define _ENTRY(n) { sizeof(n) / sizeof(n[0]), n}

const std::array<const Node<ItemInfo>, 58 + 2> protocols_meta = { {
  _ENTRY(_all_clusters), // 0
  _ENTRY(_primitive_type_list), // 1
  _ENTRY(_SecureChannelProtocol), // 2
  _ENTRY(_SecureChannelProtocol_SessionParameterStruct), // 3
  _ENTRY(_SecureChannelProtocol_PBKDFParamRequest), // 4
  _ENTRY(_SecureChannelProtocol_CryptoPBKDFParameterSet), // 5
  _ENTRY(_SecureChannelProtocol_PBKDFParamResponse), // 6
  _ENTRY(_SecureChannelProtocol_PasePake1), // 7
  _ENTRY(_SecureChannelProtocol_PasePake2), // 8
  _ENTRY(_SecureChannelProtocol_PasePake3), // 9
  _ENTRY(_SecureChannelProtocol_CaseSigma1), // 10
  _ENTRY(_SecureChannelProtocol_CaseSigma2), // 11
  _ENTRY(_SecureChannelProtocol_CaseSigma3), // 12
  _ENTRY(_SecureChannelProtocol_CaseSigma2Resume), // 13
  _ENTRY(_IMProtocol), // 14
  _ENTRY(_IMProtocol_AttributePathIB), // 15
  _ENTRY(_IMProtocol_EventPathIB), // 16
  _ENTRY(_IMProtocol_EventFilterIB), // 17
  _ENTRY(_IMProtocol_ClusterPathIB), // 18
  _ENTRY(_IMProtocol_DataVersionFilterIB), // 19
  _ENTRY(_IMProtocol_StatusResponseMessage), // 20
  _ENTRY(_IMProtocol_ReadRequestMessage), // 21
  _ENTRY(_IMProtocol_SubscribeRequestMessage), // 22
  _ENTRY(_IMProtocol_SubscribeResponseMessage), // 23
  _ENTRY(_IMProtocol_StatusIB), // 24
  _ENTRY(_IMProtocol_AttributeStatus), // 25
  _ENTRY(_IMProtocol_AttributionData), // 26
  _ENTRY(_IMProtocol_AttributeData), // 27
  _ENTRY(_IMProtocol_AttributeReportIB), // 28
  _ENTRY(_IMProtocol_EventStatusIB), // 29
  _ENTRY(_IMProtocol_EventDataIB), // 30
  _ENTRY(_IMProtocol_EventReportIB), // 31
  _ENTRY(_IMProtocol_ReportDataMessage), // 32
  _ENTRY(_IMProtocol_AttributeDataIB), // 33
  _ENTRY(_IMProtocol_WriteRequestMessage), // 34
  _ENTRY(_IMProtocol_AttributeStatusIB), // 35
  _ENTRY(_IMProtocol_WriteResponseMessage), // 36
  _ENTRY(_IMProtocol_CommandPathIB), // 37
  _ENTRY(_IMProtocol_CommandDataIB), // 38
  _ENTRY(_IMProtocol_SuppliedAttribution), // 39
  _ENTRY(_IMProtocol_InvokeRequestMessage), // 40
  _ENTRY(_IMProtocol_CommandStatusIB), // 41
  _ENTRY(_IMProtocol_InvokeResponseIB), // 42
  _ENTRY(_IMProtocol_InvokeResponseMessage), // 43
  _ENTRY(_IMProtocol_TimedRequestMessage), // 44
  _ENTRY(_IMProtocol_AttributeDataIB_list_), // 45
  _ENTRY(_IMProtocol_AttributePathIB_list_), // 46
  _ENTRY(_IMProtocol_AttributeStatusIB_list_), // 47
  _ENTRY(_IMProtocol_CommandDataIB_list_), // 48
  _ENTRY(_IMProtocol_AttributeReportIB_list_), // 49
  _ENTRY(_IMProtocol_EventReportIB_list_), // 50
  _ENTRY(_IMProtocol_EventPathIB_list_), // 51
  _ENTRY(_IMProtocol_DataVersionFilterIB_list_), // 52
  _ENTRY(_IMProtocol_InvokeResponseIB_list_), // 53
  _ENTRY(_IMProtocol_EventFilterIB_list_), // 54
  _ENTRY(_IMProtocol_StatusCodeEnum), // 55
  _ENTRY(_IMProtocol_SuppliedAttributionContextInformation), // 56
  _ENTRY(_BdxProtocol), // 57
  _ENTRY(_UserDirectedCommissioningProtocol), // 58
  _ENTRY(_UserDirectedCommissioningProtocol_IdentificationDeclarationStruct), // 59
} };

} // namespace TLVMeta
} // namespace chip
