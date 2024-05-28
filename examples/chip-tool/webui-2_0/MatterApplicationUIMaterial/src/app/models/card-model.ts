// This class represents the card model

import { ApplicationendpointComponent } from "../applicationendpoint/applicationendpoint.component";
import { EndpointModel } from "./endpoint-model";

export class CardModel {
  // The URL of the device type image
  deviceTypeImageURL!: string;
  // The device id
  deviceId!: number;
  // The endpoints
  endpoints!: EndpointModel[];
  // The device online status
  isDeviceOnline!: boolean;
  // The device name
  deviceName!: string;

  // Constructor
  constructor(deviceTypeImageURL: string, deviceId: number, endpoints: EndpointModel[], isDeviceOnline: boolean, deviceName: string) {
    this.deviceTypeImageURL = deviceTypeImageURL;
    this.deviceId = deviceId;
    this.endpoints = endpoints;
    this.isDeviceOnline = isDeviceOnline;
    this.deviceName = deviceName;
  }

  // Get the device type image URL
  public getDeviceTypeImageURL(): string {
    return this.deviceTypeImageURL;
  }

  // Get the device id
  public getDeviceId(): number {
    return this.deviceId;
  }

  public getEndpoints(): EndpointModel[] {
    return this.endpoints;
  }

  // Get the device online status

  public getIsDeviceOnline(): boolean {
    return this.isDeviceOnline;
  }

  // Get the device name

  public getDeviceName(): string {
    return this.deviceName;
  }

  // Get the device status
  public getDeviceStatus(): string {
    return this.isDeviceOnline ? 'Online' : 'Offline';
  }

  // Get the device status color
  public getDeviceStatusColor(): string {
    return this.isDeviceOnline ? 'green' : 'red';
  }

  // Get the device status icon
  public getDeviceStatusIcon(): string {
    return this.isDeviceOnline ? 'check_circle' : 'cancel';
  }

 
}