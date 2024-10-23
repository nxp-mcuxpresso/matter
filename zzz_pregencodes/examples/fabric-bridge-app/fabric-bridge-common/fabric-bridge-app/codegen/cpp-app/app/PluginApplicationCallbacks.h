#pragma once
void MatterAccessControlPluginServerInitCallback();
void MatterAdministratorCommissioningPluginServerInitCallback();
void MatterBasicInformationPluginServerInitCallback();
void MatterCommissionerControlPluginServerInitCallback();
void MatterDescriptorPluginServerInitCallback();
void MatterDiagnosticLogsPluginServerInitCallback();
void MatterEthernetNetworkDiagnosticsPluginServerInitCallback();
void MatterGeneralCommissioningPluginServerInitCallback();
void MatterGeneralDiagnosticsPluginServerInitCallback();
void MatterGroupKeyManagementPluginServerInitCallback();
void MatterIdentifyPluginServerInitCallback();
void MatterNetworkCommissioningPluginServerInitCallback();
void MatterOperationalCredentialsPluginServerInitCallback();
void MatterSoftwareDiagnosticsPluginServerInitCallback();
void MatterThreadNetworkDiagnosticsPluginServerInitCallback();
void MatterWiFiNetworkDiagnosticsPluginServerInitCallback();

#define MATTER_PLUGINS_INIT \
    MatterAccessControlPluginServerInitCallback(); \
    MatterAdministratorCommissioningPluginServerInitCallback(); \
    MatterBasicInformationPluginServerInitCallback(); \
    MatterCommissionerControlPluginServerInitCallback(); \
    MatterDescriptorPluginServerInitCallback(); \
    MatterDiagnosticLogsPluginServerInitCallback(); \
    MatterEthernetNetworkDiagnosticsPluginServerInitCallback(); \
    MatterGeneralCommissioningPluginServerInitCallback(); \
    MatterGeneralDiagnosticsPluginServerInitCallback(); \
    MatterGroupKeyManagementPluginServerInitCallback(); \
    MatterIdentifyPluginServerInitCallback(); \
    MatterNetworkCommissioningPluginServerInitCallback(); \
    MatterOperationalCredentialsPluginServerInitCallback(); \
    MatterSoftwareDiagnosticsPluginServerInitCallback(); \
    MatterThreadNetworkDiagnosticsPluginServerInitCallback(); \
    MatterWiFiNetworkDiagnosticsPluginServerInitCallback();

