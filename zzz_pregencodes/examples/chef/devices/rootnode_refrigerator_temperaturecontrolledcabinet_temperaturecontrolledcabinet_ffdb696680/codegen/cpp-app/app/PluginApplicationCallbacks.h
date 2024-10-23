#pragma once
void MatterAccessControlPluginServerInitCallback();
void MatterAdministratorCommissioningPluginServerInitCallback();
void MatterBasicInformationPluginServerInitCallback();
void MatterDescriptorPluginServerInitCallback();
void MatterGeneralCommissioningPluginServerInitCallback();
void MatterGeneralDiagnosticsPluginServerInitCallback();
void MatterGroupKeyManagementPluginServerInitCallback();
void MatterNetworkCommissioningPluginServerInitCallback();
void MatterOperationalCredentialsPluginServerInitCallback();
void MatterRefrigeratorAlarmPluginServerInitCallback();
void MatterRefrigeratorAndTemperatureControlledCabinetModePluginServerInitCallback();
void MatterTemperatureControlPluginServerInitCallback();
void MatterTemperatureMeasurementPluginServerInitCallback();
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
    MatterNetworkCommissioningPluginServerInitCallback(); \
    MatterOperationalCredentialsPluginServerInitCallback(); \
    MatterRefrigeratorAlarmPluginServerInitCallback(); \
    MatterRefrigeratorAndTemperatureControlledCabinetModePluginServerInitCallback(); \
    MatterTemperatureControlPluginServerInitCallback(); \
    MatterTemperatureMeasurementPluginServerInitCallback(); \
    MatterUnitLocalizationPluginServerInitCallback(); \
    MatterWiFiNetworkDiagnosticsPluginServerInitCallback();

