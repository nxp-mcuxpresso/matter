#include <app-common/zap-generated/callback.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <lib/support/Span.h>
#include <protocols/interaction_model/Constants.h>

using namespace chip;

// Cluster Init Functions
void emberAfClusterInitCallback(EndpointId endpoint, ClusterId clusterId)
{
    switch (clusterId)
    {
    case app::Clusters::AccessControl::Id:
        emberAfAccessControlClusterInitCallback(endpoint);
        break;
    case app::Clusters::AdministratorCommissioning::Id:
        emberAfAdministratorCommissioningClusterInitCallback(endpoint);
        break;
    case app::Clusters::BasicInformation::Id:
        emberAfBasicInformationClusterInitCallback(endpoint);
        break;
    case app::Clusters::Descriptor::Id:
        emberAfDescriptorClusterInitCallback(endpoint);
        break;
    case app::Clusters::DeviceEnergyManagement::Id:
        emberAfDeviceEnergyManagementClusterInitCallback(endpoint);
        break;
    case app::Clusters::DeviceEnergyManagementMode::Id:
        emberAfDeviceEnergyManagementModeClusterInitCallback(endpoint);
        break;
    case app::Clusters::ElectricalEnergyMeasurement::Id:
        emberAfElectricalEnergyMeasurementClusterInitCallback(endpoint);
        break;
    case app::Clusters::ElectricalPowerMeasurement::Id:
        emberAfElectricalPowerMeasurementClusterInitCallback(endpoint);
        break;
    case app::Clusters::EnergyEvse::Id:
        emberAfEnergyEvseClusterInitCallback(endpoint);
        break;
    case app::Clusters::EnergyEvseMode::Id:
        emberAfEnergyEvseModeClusterInitCallback(endpoint);
        break;
    case app::Clusters::GeneralCommissioning::Id:
        emberAfGeneralCommissioningClusterInitCallback(endpoint);
        break;
    case app::Clusters::GeneralDiagnostics::Id:
        emberAfGeneralDiagnosticsClusterInitCallback(endpoint);
        break;
    case app::Clusters::GroupKeyManagement::Id:
        emberAfGroupKeyManagementClusterInitCallback(endpoint);
        break;
    case app::Clusters::Identify::Id:
        emberAfIdentifyClusterInitCallback(endpoint);
        break;
    case app::Clusters::LocalizationConfiguration::Id:
        emberAfLocalizationConfigurationClusterInitCallback(endpoint);
        break;
    case app::Clusters::NetworkCommissioning::Id:
        emberAfNetworkCommissioningClusterInitCallback(endpoint);
        break;
    case app::Clusters::OperationalCredentials::Id:
        emberAfOperationalCredentialsClusterInitCallback(endpoint);
        break;
    case app::Clusters::PowerSource::Id:
        emberAfPowerSourceClusterInitCallback(endpoint);
        break;
    case app::Clusters::PowerTopology::Id:
        emberAfPowerTopologyClusterInitCallback(endpoint);
        break;
    case app::Clusters::TimeFormatLocalization::Id:
        emberAfTimeFormatLocalizationClusterInitCallback(endpoint);
        break;
    case app::Clusters::UnitLocalization::Id:
        emberAfUnitLocalizationClusterInitCallback(endpoint);
        break;
    default:
        // Unrecognized cluster ID
        break;
    }
}
