#pragma once
void MatterAccessControlPluginServerInitCallback();
void MatterAdministratorCommissioningPluginServerInitCallback();
void MatterBasicInformationPluginServerInitCallback();
void MatterDescriptorPluginServerInitCallback();
void MatterDeviceEnergyManagementPluginServerInitCallback();
void MatterDeviceEnergyManagementModePluginServerInitCallback();
void MatterElectricalEnergyMeasurementPluginServerInitCallback();
void MatterElectricalPowerMeasurementPluginServerInitCallback();
void MatterEnergyEvsePluginServerInitCallback();
void MatterEnergyEvseModePluginServerInitCallback();
void MatterGeneralCommissioningPluginServerInitCallback();
void MatterGeneralDiagnosticsPluginServerInitCallback();
void MatterGroupKeyManagementPluginServerInitCallback();
void MatterIdentifyPluginServerInitCallback();
void MatterLocalizationConfigurationPluginServerInitCallback();
void MatterNetworkCommissioningPluginServerInitCallback();
void MatterOperationalCredentialsPluginServerInitCallback();
void MatterPowerSourcePluginServerInitCallback();
void MatterPowerTopologyPluginServerInitCallback();
void MatterTimeFormatLocalizationPluginServerInitCallback();
void MatterUnitLocalizationPluginServerInitCallback();

#define MATTER_PLUGINS_INIT \
    MatterAccessControlPluginServerInitCallback(); \
    MatterAdministratorCommissioningPluginServerInitCallback(); \
    MatterBasicInformationPluginServerInitCallback(); \
    MatterDescriptorPluginServerInitCallback(); \
    MatterDeviceEnergyManagementPluginServerInitCallback(); \
    MatterDeviceEnergyManagementModePluginServerInitCallback(); \
    MatterElectricalEnergyMeasurementPluginServerInitCallback(); \
    MatterElectricalPowerMeasurementPluginServerInitCallback(); \
    MatterEnergyEvsePluginServerInitCallback(); \
    MatterEnergyEvseModePluginServerInitCallback(); \
    MatterGeneralCommissioningPluginServerInitCallback(); \
    MatterGeneralDiagnosticsPluginServerInitCallback(); \
    MatterGroupKeyManagementPluginServerInitCallback(); \
    MatterIdentifyPluginServerInitCallback(); \
    MatterLocalizationConfigurationPluginServerInitCallback(); \
    MatterNetworkCommissioningPluginServerInitCallback(); \
    MatterOperationalCredentialsPluginServerInitCallback(); \
    MatterPowerSourcePluginServerInitCallback(); \
    MatterPowerTopologyPluginServerInitCallback(); \
    MatterTimeFormatLocalizationPluginServerInitCallback(); \
    MatterUnitLocalizationPluginServerInitCallback();

