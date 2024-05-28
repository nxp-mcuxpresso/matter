// This class represents the endpoint model

import { SimpleChanges, model } from "@angular/core";
import { ApplicationendpointComponent } from "../applicationendpoint/applicationendpoint.component";

export class EndpointModel {
  
  endpointId!: string;
  endpointName!: string;
  endpointImageURL!: string;
  endpointSubscriptionId!: string | undefined;
  endpointStatus!: boolean; // Activated or Deactivated
  
  constructor(endpointId: string, endpointName: string, endpointImageURL: string, endpointStatus: boolean, endpointSubscriptionId: string | undefined) {
    this.endpointName = endpointName;
    this.endpointImageURL = endpointImageURL;
    this.endpointSubscriptionId = endpointSubscriptionId;
    this.endpointStatus = endpointStatus;
  }
  

  public getEndpointName(): string {
    return this.endpointName;
  }

  public getEndpointImageURL(): string {
    return this.endpointImageURL;
  }

  public getEndpointSubscriptionId(): string | undefined {
    return this.endpointSubscriptionId;
  }

  public getEndpointStatus(): boolean {
    return this.endpointStatus;
  }

  public getEndpointStatusColor(): string {
    return this.endpointStatus === true ? 'green' : 'red';
  }

  public getEndpointStatusIcon(): string {
    return this.endpointStatus === true ? 'check' : 'close';
  }


}