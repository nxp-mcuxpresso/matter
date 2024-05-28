import { Injectable } from '@angular/core';
import { Socket } from 'ngx-socket-io';
import {MatSnackBar} from '@angular/material/snack-bar';
import { API_WEBSOCKET_URL } from '../../api_addresses';
import { min } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class WebSocketService {

  private socket!: WebSocket;

  constructor(private _snackBar: MatSnackBar) { }

  connect(): void {
    this.socket = new WebSocket(
      API_WEBSOCKET_URL
    );

    this.socket.onopen = () => {
      console.log('WebSocket connection established.');
      this._snackBar.open('WebSocket connection established.', 'Close', {
        duration: 5000,
      });
    };

    this.socket.onmessage = (event) => {
      console.log('Received message:', event.data);
      this._snackBar.open(event.data, 'Close', {
        duration: 5000,
      });
    };

    this.socket.onclose = (event) => {
      console.log('WebSocket connection closed:', event);
      this._snackBar.open('WebSocket connection closed.', 'Close', {
        duration: 5000,
      });
    };

    this.socket.onerror = (error) => {
      console.error('WebSocket error:', error);
      this._snackBar.open('WebSocket error.', 'Close', {
        duration: 5000,
      });
    };
  }

  sendMessage(message: string): void {
    this.socket.send(message);
  }

  closeConnection(): void {
    this.socket.close();
  }

  subscribeToDeviceEndpoint(device_id: string, device_endpoint_id: string, min_interval: number, max_interval: number, cluster_name: string, node_alias: string) {
    console.log('Data: ' + device_id + ' ' + device_endpoint_id + ' ' + min_interval + ' ' + max_interval + ' ' + cluster_name);
    if (cluster_name === 'cluster on/off') {
      console.log('Subscribing to on-off cluster. Sending message to server: onoff subscribe on-off' + ' ' + min_interval + ' ' + max_interval + ' ' + device_id + ' ' + device_endpoint_id);
      if (this.socket.readyState === 1) {
        // this.sendMessage('onoff subscribe on-off' + ' ' + min_interval + ' ' + max_interval + ' ' + device_id + ' ' + device_endpoint_id);
        console.log('Sending message as JSON: ', JSON.stringify({
          'command': 'onoff subscribe on-off',
          'minInterval': min_interval,
          'maxInterval': max_interval,
          'nodeId': device_id,
          'endPointId': device_endpoint_id,
          'nodeAlias': node_alias
        }));

        this.sendMessage(JSON.stringify({
          'command': 'onoff subscribe on-off',
          'minInterval': min_interval,
          'maxInterval': max_interval,
          'nodeId': device_id,
          'endPointId': device_endpoint_id,
          'nodeAlias': node_alias
        }));
      } else {
        console.log('Socket not connected');
      }
    }
  }

  public getWebSocket(): WebSocket {
    return this.socket;
  }


}
