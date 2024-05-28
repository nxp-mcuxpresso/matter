// This endpoint extends the endpoint model
import { EndpointModel } from './endpoint-model';


export class LightingModel extends EndpointModel {
  // The color of the light
  color: string | undefined;

  // The brightness of the light
  brightness: number | undefined;

  // The state of the light
  state: boolean | undefined;

  // Constructor only with endpoint model and color and brightness
  constructor(endpointModel: EndpointModel, color: string | undefined, brightness: number | undefined) {
    super(endpointModel.endpointId, endpointModel.endpointName, endpointModel.endpointImageURL, endpointModel.endpointStatus, endpointModel.endpointSubscriptionId);
    this.color = color;
    this.brightness = brightness;
  }

  public getColor(): string | undefined {
    return this.color;
  }

  public getBrightness(): number | undefined {
    return this.brightness;
  }

  public getState(): boolean | undefined {
    return this.state;
  }

}
