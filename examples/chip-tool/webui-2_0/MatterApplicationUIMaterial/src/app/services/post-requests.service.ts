// Post requests service

import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, catchError, throwError } from 'rxjs';
import { HttpHeaders } from '@angular/common/http';
import { API_BASE_URL } from '../../api_addresses';

@Injectable({
  providedIn: 'root'
})

export class PostRequestsService {
  constructor(private httpClient: HttpClient) { }
  serverUrl: string = API_BASE_URL;

  sendPairBLEThreadCommand(deviceId: string, pinCode: string,  nodeAlias: string, dataset: string, discriminator: string) : Observable<string> {
    const data = {
      nodeId: deviceId,
      pinCode: pinCode,
      type: "ble-thread",
      nodeAlias: nodeAlias,
      dataset: dataset,
      discriminator: discriminator
    };

    console.log(
      `Sending pairing request to ${this.serverUrl}/pairing with data: ${JSON.stringify(data)}`
    );

    return this.httpClient.post<string>(
      `${this.serverUrl}/pairing`, JSON.stringify(data)).pipe(
      catchError((error: any) => {
        console.error('Error sending pairing request', error);
        return throwError(error);
      }
    ));
  }

  sendPairBLEWifiCommand(deviceId: string, pinCode: string, nodeAlias: string, net_ssid: string, net_pass: string, discriminator: string) : Observable<string> {
    const data = {
      nodeId: deviceId,
      pinCode: pinCode,
      type: "ble-wifi",
      nodeAlias: nodeAlias,
      ssId: net_ssid,
      password: net_pass,
      discriminator: discriminator
    };
    console.log(
      `Sending pairing request to ${this.serverUrl}/pairing with data: ${JSON.stringify(data)}`
    )
    // Send the pairing request to the server
    return this.httpClient.post<string>(
      `${this.serverUrl}/pairing`, JSON.stringify(data)).pipe(
      catchError((error: any) => {
        console.error('Error sending pairing request', error);
        return throwError(error);
      }
    ));
  }

  sendPairOnNetworkCommand(deviceId: string, pinCode: string, nodeAlias:string) : Observable<string>{
    const data = {
      nodeId: deviceId,
      pinCode: pinCode,
      type: "onnetwork",
      nodeAlias: nodeAlias
    };

    console.log(
      `Sending pairing request to ${this.serverUrl}/pairing with data: ${JSON.stringify(data)}`
    )

    return this.httpClient.post<string>(
      `${this.serverUrl}/pairing`, JSON.stringify(data)
    ).pipe(
      catchError((error: any) => {
        console.error('Error sending pairing request', error);
        return throwError(error);
      }
    ));
  }

  sendPairEthernetCommand(deviceId: string, pinCode: string, discriminator: string, ip_address: string, port: string, nodeAlias: string) : Observable<string> {
    const data = {
      nodeId: deviceId,
      pinCode: pinCode,
      type: "ethernet",
      discriminator: discriminator,
      ip_address: ip_address,
      port: port,
      nodeAlias: nodeAlias
    };
    console.log(
      `Sending pairing request to ${this.serverUrl}/pairing with data: ${JSON.stringify(data)}`
    );

    return this.httpClient.post<string>(
      `${this.serverUrl}/pairing`, JSON.stringify(data)).pipe(
      catchError((error: any) => {
        console.error('Error sending pairing request', error);
        return throwError(error);
      }
    ));

  }

  sendOnOffToggleEndpointCommand(device_alias: string, deviceId: string, endPointId: string, type: string) : Observable<string> {
    const data = {
      nodeAlias: device_alias,
      nodeId: deviceId,
      endPointId: endPointId,
      type: type
    };
    console.log(
      `Sending on/off toggle request to ${this.serverUrl}/onoff with data: ${JSON.stringify(data)}`
    );

    return this.httpClient.post<string>(
      `${this.serverUrl}/onoff`, JSON.stringify(data)).pipe(
      catchError((error: any) => {
        console.error('Error sending on/off toggle request', error);
        return throwError(error);
      }
    ));
  }

  sendOnOffReadEndpointCommand(device_alias: string, deviceId: string, endPointId: string) : Observable<string> {
    const data = {
      nodeAlias: device_alias,
      nodeId: deviceId,
      endPointId: endPointId,
    };
    console.log(
      `Sending on/off read request to ${this.serverUrl}/onoff with data: ${JSON.stringify(data)}`
    );

    return this.httpClient.post<string>(
      `${this.serverUrl}/onoff_report`, JSON.stringify(data)).pipe(
      catchError((error: any) => {
        console.error('Error sending on/off read request', error);
        return throwError(error);
      }
    ));
  }

}
