// Switch model class that extends the endpoint model

import { EndpointModel } from './endpoint-model';

export class SwitchModel extends EndpointModel {
  // The state of the switch
  state: boolean | undefined;

  // Constructor only with endpoint model and state
  constructor(endpointModel: EndpointModel, state: boolean | undefined) {
    super(endpointModel.endpointId, endpointModel.endpointName, endpointModel.endpointImageURL, endpointModel.endpointStatus, endpointModel.endpointSubscriptionId);
    this.state = state;
  }

  public getState(): boolean | undefined {
    return this.state;
  }

}