#pragma once
void MatterAccessControlPluginServerInitCallback();
void MatterAdministratorCommissioningPluginServerInitCallback();
void MatterBasicInformationPluginServerInitCallback();
void MatterDescriptorPluginServerInitCallback();
void MatterGeneralCommissioningPluginServerInitCallback();
void MatterGeneralDiagnosticsPluginServerInitCallback();
void MatterGroupKeyManagementPluginServerInitCallback();
void MatterLaundryWasherControlsPluginServerInitCallback();
void MatterLaundryWasherModePluginServerInitCallback();
void MatterNetworkCommissioningPluginServerInitCallback();
void MatterOperationalCredentialsPluginServerInitCallback();
void MatterOperationalStatePluginServerInitCallback();
void MatterUnitLocalizationPluginServerInitCallback();
void MatterWiFiNetworkDiagnosticsPluginServerInitCallback();

#define MATTER_PLUGINS_INIT \
    MatterAccessControlPluginServerInitCallback(); \
    MatterAdministratorCommissioningPluginServerInitCallback(); \
    MatterBasicInformationPluginServerInitCallback(); \
    MatterDescriptorPluginServerInitCallback(); \
    MatterGeneralCommissioningPluginServerInitCallback(); \
    MatterGeneralDiagnosticsPluginServerInitCallback(); \
    MatterGroupKeyManagementPluginServerInitCallback(); \
    MatterLaundryWasherControlsPluginServerInitCallback(); \
    MatterLaundryWasherModePluginServerInitCallback(); \
    MatterNetworkCommissioningPluginServerInitCallback(); \
    MatterOperationalCredentialsPluginServerInitCallback(); \
    MatterOperationalStatePluginServerInitCallback(); \
    MatterUnitLocalizationPluginServerInitCallback(); \
    MatterWiFiNetworkDiagnosticsPluginServerInitCallback();

