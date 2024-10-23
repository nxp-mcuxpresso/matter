#pragma once
void MatterAccessControlPluginServerInitCallback();
void MatterAdministratorCommissioningPluginServerInitCallback();
void MatterBasicInformationPluginServerInitCallback();
void MatterColorControlPluginServerInitCallback();
void MatterDescriptorPluginServerInitCallback();
void MatterDiagnosticLogsPluginServerInitCallback();
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
void MatterThreadNetworkDiagnosticsPluginServerInitCallback();
void MatterUserLabelPluginServerInitCallback();

#define MATTER_PLUGINS_INIT \
    MatterAccessControlPluginServerInitCallback(); \
    MatterAdministratorCommissioningPluginServerInitCallback(); \
    MatterBasicInformationPluginServerInitCallback(); \
    MatterColorControlPluginServerInitCallback(); \
    MatterDescriptorPluginServerInitCallback(); \
    MatterDiagnosticLogsPluginServerInitCallback(); \
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
    MatterThreadNetworkDiagnosticsPluginServerInitCallback(); \
    MatterUserLabelPluginServerInitCallback();

