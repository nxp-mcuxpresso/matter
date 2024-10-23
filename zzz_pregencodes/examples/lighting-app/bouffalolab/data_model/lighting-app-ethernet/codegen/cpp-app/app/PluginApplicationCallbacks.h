#pragma once
void MatterAccessControlPluginServerInitCallback();
void MatterAdministratorCommissioningPluginServerInitCallback();
void MatterBasicInformationPluginServerInitCallback();
void MatterColorControlPluginServerInitCallback();
void MatterDescriptorPluginServerInitCallback();
void MatterDiagnosticLogsPluginServerInitCallback();
void MatterEthernetNetworkDiagnosticsPluginServerInitCallback();
void MatterFixedLabelPluginServerInitCallback();
void MatterGeneralCommissioningPluginServerInitCallback();
void MatterGeneralDiagnosticsPluginServerInitCallback();
void MatterGroupKeyManagementPluginServerInitCallback();
void MatterGroupsPluginServerInitCallback();
void MatterIdentifyPluginServerInitCallback();
void MatterLevelControlPluginServerInitCallback();
void MatterNetworkCommissioningPluginServerInitCallback();
void MatterOnOffPluginServerInitCallback();
void MatterOperationalCredentialsPluginServerInitCallback();
void MatterOtaSoftwareUpdateRequestorPluginServerInitCallback();
void MatterSoftwareDiagnosticsPluginServerInitCallback();
void MatterUserLabelPluginServerInitCallback();

#define MATTER_PLUGINS_INIT \
    MatterAccessControlPluginServerInitCallback(); \
    MatterAdministratorCommissioningPluginServerInitCallback(); \
    MatterBasicInformationPluginServerInitCallback(); \
    MatterColorControlPluginServerInitCallback(); \
    MatterDescriptorPluginServerInitCallback(); \
    MatterDiagnosticLogsPluginServerInitCallback(); \
    MatterEthernetNetworkDiagnosticsPluginServerInitCallback(); \
    MatterFixedLabelPluginServerInitCallback(); \
    MatterGeneralCommissioningPluginServerInitCallback(); \
    MatterGeneralDiagnosticsPluginServerInitCallback(); \
    MatterGroupKeyManagementPluginServerInitCallback(); \
    MatterGroupsPluginServerInitCallback(); \
    MatterIdentifyPluginServerInitCallback(); \
    MatterLevelControlPluginServerInitCallback(); \
    MatterNetworkCommissioningPluginServerInitCallback(); \
    MatterOnOffPluginServerInitCallback(); \
    MatterOperationalCredentialsPluginServerInitCallback(); \
    MatterOtaSoftwareUpdateRequestorPluginServerInitCallback(); \
    MatterSoftwareDiagnosticsPluginServerInitCallback(); \
    MatterUserLabelPluginServerInitCallback();

