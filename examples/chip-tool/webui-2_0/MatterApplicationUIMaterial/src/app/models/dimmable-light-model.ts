// Class that extends the endpoint model and adds a slider with the lighting level

import { LightingModel } from "./lighting-model";

export class DimmableLightModel extends LightingModel {
  // The slider value
  sliderValue!: number;
  
  // Constructor
  constructor(endpointModel: LightingModel, sliderValue: number) {
    super(endpointModel, endpointModel.color, endpointModel.brightness);
    this.sliderValue = sliderValue;
  }
  
  public getSliderValue(): number {
    return this.sliderValue;
  }
  
  public setSliderValue(sliderValue: number): void {
    this.sliderValue = sliderValue;
  }
}