// Thermostat model extends the endpoint model and adds the temperature and the mode

import { EndpointModel } from './endpoint-model';

export class ThermostatModel extends EndpointModel {
  // The temperature of the thermostat
  temperature!: number;

  // The mode of the thermostat
  mode!: string;

  // Constructor
  constructor(endpointModel: EndpointModel, temperature: number, mode: string) {
    super(endpointModel.endpointName, endpointModel.endpointImageURL, endpointModel.endpointSubscriptionId, endpointModel.endpointStatus);
    this.temperature = temperature;
    this.mode = mode;
  }

  public getTemperature(): number {
    return this.temperature;
  }

  public getMode(): string {
    return this.mode;
  }

}