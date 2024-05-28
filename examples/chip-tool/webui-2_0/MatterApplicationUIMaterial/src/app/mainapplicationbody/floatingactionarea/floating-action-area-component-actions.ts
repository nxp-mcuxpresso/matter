import { LoaderService } from './../../services/loader.service';
import { Dialog } from '@angular/cdk/dialog';
import { Component, EventEmitter, Output } from '@angular/core';
import { MatChip, MatChipListbox } from '@angular/material/chips';
import { MatIcon } from '@angular/material/icon';
import { MatChipsModule } from '@angular/material/chips';
import { NgFor, NgStyle } from '@angular/common';
import { PopupdialogmaterialComponent } from '../../popupdialogmaterial/popupdialogmaterial.component';
import { AppDialogService } from '../../services/app-dialog.service';
import { MatDialog } from '@angular/material/dialog';
import { AppDialogWithInputFieldsService } from '../../services/app-dialog-input.service';
import {GetRequestsService} from '../../services/get-requests.service';
import {PostRequestsService} from '../../services/post-requests.service';


export class FloatingActionAreaComponentActions {
  // Dependency injecting the needed services
  constructor (private appDialogService : AppDialogService, private appDialogWithInputFieldsService: AppDialogWithInputFieldsService,
    private postRequestsService: PostRequestsService, private getRequestsService: GetRequestsService, private loaderService: LoaderService
  ) {}

  public getButtonPressedListWithEmitFunction(
    emitButtonPressed: Function
  ): { buttonName: string; action: Function; color: string; icon?: string; }[] {

    // All the buttons and actions for the floating action area
    const listOfButtonsAndActions: { buttonName: string; action: Function; color: string; icon?: string; }[] = [
      {
        buttonName: 'Refresh', action: () => emitButtonPressed({
          buttonName: 'Refresh', action: () => this.appDialogService.openDialog(
            'Refresh', '../../../assets/matter-logo-transparent.png', 'Refresh the list of devices. The application will do a full refresh of all the devices. This may take a while.',
            [
              { buttonName: 'Refresh', action: () => {
                this.appDialogService.showInfoDialog('This function is not yet implemented.'); }, color: 'accent', icon: 'refresh' },
              // { buttonName: 'Cancel', action: () => { this.appDialogService.closeDialog(); }, color: 'warn', icon: 'cancel' },
            ]
          )
        }
        ), color: 'accent', icon: 'refresh'
      },

      {
        buttonName: 'Get Devices', action: () => emitButtonPressed({
          buttonName: 'Get Devices', action: () => this.appDialogService.openDialog(
            'Get Devices', '../../../assets/matter-logo-transparent.png', 'Get the list of devices from the database and their availability status.',
            [
              { buttonName: 'Get Devices', action: () => {
                this.appDialogService.showInfoDialog('This function is not yet implemented.'); }, color: 'accent', icon: 'list', },
              // { buttonName: 'Cancel', action: () => { this.appDialogService.closeDialog(); }, color: 'warn', icon: 'cancel' },
            ]
          )
        }
        ), color: 'accent', icon: 'list'
      },

      {
        buttonName: 'Add Device', action: () => emitButtonPressed({
          buttonName: 'Add', action: () => this.appDialogService.openDialog(
            'Add Device', '../../../assets/matter-logo-transparent.png', 'Add a new device. The devices can be added via BLE-Thread, BLE-WIFI or Ethernet. The commissioning process may take a while.',
            [
              {
                buttonName: 'Add Device', action: () => {
                  this.appDialogService.openDialog(
                    'Add Device', '../../../assets/matter-logo-transparent.png', 'Add a new device. The devices can be added via BLE-Thread, BLE-WIFI or Ethernet. The commissioning process may take a while.',
                    [
                      {
                        buttonName: 'Add BLE-Thread Device', action: () => {
                          this.appDialogWithInputFieldsService.openDialog(
                            'Connect BLE-Thread Device', '../../../assets/matter-logo-transparent.png',
                            {
                              inputFields: [
                                {inputFieldType: 'text', inputFieldName: 'Device ID', inputFieldContent: '1234', inputFieldDefaultValue: 'Default: 1234'},
                                {inputFieldType: 'text', inputFieldName: 'Device Code (20202021)', inputFieldContent: '20202021', inputFieldDefaultValue: 'Default: 20202021'},
                                {inputFieldType: 'text', inputFieldName: 'Device Alias', inputFieldContent: 'DeviceAliasName', inputFieldDefaultValue: 'Default: DeviceAliasName'},
                                {inputFieldType: 'text', inputFieldName: 'Device Dataset', inputFieldContent: 'Dataset', inputFieldDefaultValue: 'Default: 0x123123123123123123123123123123123'},
                                {inputFieldType: 'text', inputFieldName: 'Device Bluetooth Discriminator (3840)', inputFieldContent: '3840', inputFieldDefaultValue: 'Default: 3840'},

                              ]
                            },
                            [
                              { buttonName: 'Send Command', action: () => {
                                  var values_for_the_input = this.appDialogWithInputFieldsService.getInputFieldsValues()!;
                                  this.loaderService.showLoader();
                                  this.postRequestsService.sendPairBLEThreadCommand(
                                    values_for_the_input[0].inputFieldContent, values_for_the_input[1].inputFieldContent, values_for_the_input[2].inputFieldContent,
                                    values_for_the_input[3].inputFieldContent, values_for_the_input[4].inputFieldContent
                                  ).subscribe(
                                    data => {
                                      console.log('Data received: ', data);
                                      this.loaderService.hideLoader();
                                      const parsedResult = JSON.parse(JSON.stringify(data));
                                      if (parsedResult.result === 'successful') {
                                        this.appDialogService.showInfoDialog('BLE-Thread device added successfully. Please refresh to interact with the device.');
                                      } else if (parsedResult.result === 'failed') {
                                        this.appDialogService.showErrorDialog('Error adding BLE-Thread device');
                                      }
                                    },

                                    error => {
                                      console.error('Error received: ', error);
                                      this.loaderService.hideLoader();
                                      this.appDialogService.showErrorDialog('Error adding BLE-Thread device. Network error.');
                                    }
                                  );

                              }, color: 'accent', icon: 'send' },
                              {
                                buttonName: 'Get Dataset', action: () => {
                                  var values_for_the_input = this.appDialogWithInputFieldsService.getInputFieldsValues()!;
                                  this.loaderService.showLoader();
                                  this.getRequestsService.getDataset().subscribe(
                                    data => {
                                      console.log('Data received: ', data);
                                      this.loaderService.hideLoader();
                                      const parsedResult = JSON.parse(JSON.stringify(data));
                                      if (parsedResult.result === 'successful') {
                                        this.appDialogService.showInfoDialog('Dataset received successfully.');
                                        this.appDialogWithInputFieldsService.updateInputFieldContent(3, parsedResult.dataset);
                                      } else if (parsedResult.result === 'failed') {
                                        this.appDialogService.showErrorDialog('Error receiving dataset');
                                      }
                                    },

                                    error => {
                                      console.error('Error received: ', error);
                                      this.loaderService.hideLoader();
                                      this.appDialogWithInputFieldsService.updateInputFieldContent(3, 'Error receiving dataset. Try again or manually insert the dataset.');
                                      this.appDialogService.showErrorDialog('Error receiving dataset. Network error.');
                                    }
                                  );
                                }, color: 'accent', icon: 'get_app'},
                              // { buttonName: 'Cancel', action: () => { this.appDialogWithInputFieldsService.closeDialog(); }, color: 'warn', icon: 'cancel' },
                            ]
                          );
                      }, color: 'accent', icon: 'add' },
                      { buttonName: 'Add BLE-WIFI Device', action: () => {
                          this.appDialogWithInputFieldsService.openDialog(
                            'Connect BLE-WIFI Device', '../../../assets/matter-logo-transparent.png',
                            {
                              inputFields: [
                                {inputFieldType: 'text', inputFieldName: 'Device ID', inputFieldContent: '1234', inputFieldDefaultValue: 'Default: 1234'},
                                {inputFieldType: 'text', inputFieldName: 'Device Code (20202021)', inputFieldContent: '20202021', inputFieldDefaultValue: 'Default: 20202021'},
                                {inputFieldType: 'text', inputFieldName: 'Device Alias', inputFieldContent: 'DeviceAliasName', inputFieldDefaultValue: 'Default: DeviceAliasName'},
                                {inputFieldType: 'text', inputFieldName: 'Network SSID', inputFieldContent: 'NetworkSSID', inputFieldDefaultValue: 'Default: NetworkSSID'},
                                {inputFieldType: 'password', inputFieldName: 'Network Password', inputFieldContent: 'NetPa55!', inputFieldDefaultValue: 'Default: NetPa55!'},
                                {inputFieldType: 'text', inputFieldName: 'Device Bluetooth Discriminator (3840)', inputFieldContent: '3840', inputFieldDefaultValue: 'Default: 3840'},

                              ]
                            },
                            [
                              { buttonName: 'Send Command', action: () => {
                                  var values_for_the_input = this.appDialogWithInputFieldsService.getInputFieldsValues()!;
                                  this.loaderService.showLoader();
                                  this.postRequestsService.sendPairBLEWifiCommand(
                                    values_for_the_input[0].inputFieldContent, values_for_the_input[1].inputFieldContent, values_for_the_input[2].inputFieldContent,
                                    values_for_the_input[3].inputFieldContent, values_for_the_input[4].inputFieldContent, values_for_the_input[5].inputFieldContent
                                  ).subscribe(
                                    data => {
                                      console.log('Data received: ', data);
                                      this.loaderService.hideLoader();
                                      const parsedResult = JSON.parse(JSON.stringify(data));
                                      if (parsedResult.result === 'successful') {
                                        this.appDialogService.showInfoDialog('BLE-WIFI device added successfully. Please refresh to interact with the device.');
                                      } else if (parsedResult.result === 'failed') {
                                        this.appDialogService.showErrorDialog('Error adding BLE-WIFI device');
                                      }
                                    },

                                    error => {
                                      console.error('Error received: ', error);
                                      this.loaderService.hideLoader();
                                      this.appDialogService.showErrorDialog('Error adding BLE-WIFI device. Network error.');
                                    }
                                  );

                              }, color: 'accent', icon: 'send' },
                              // { buttonName: 'Cancel', action: () => {
                              //   this.appDialogWithInputFieldsService.closeDialog();
                              // }, color: 'warn', icon: 'cancel' },
                            ]
                          );
                      }, color: 'warn', icon: 'add' },
                      { buttonName: 'Add OnNetwork Device', action: () => {
                          this.appDialogWithInputFieldsService.openDialog(
                            'Connect OnNetwork Device', '../../../assets/matter-logo-transparent.png',
                            {
                              inputFields: [
                                {inputFieldType: 'text', inputFieldName: 'Device ID', inputFieldContent: '1234', inputFieldDefaultValue: 'Default: 1234'},
                                {inputFieldType: 'text', inputFieldName: 'Device Code (20202021)', inputFieldContent: '20202021', inputFieldDefaultValue: 'Default: 20202021'},
                                {inputFieldType: 'text', inputFieldName: 'Device Alias', inputFieldContent: 'DeviceAliasName', inputFieldDefaultValue: 'Default: DeviceAliasName'},
                              ]
                            },
                            [
                              { buttonName: 'Send Command', action: () => {
                                var values_for_the_input = this.appDialogWithInputFieldsService.getInputFieldsValues()!;
                                this.loaderService.showLoader();
                                this.postRequestsService.sendPairOnNetworkCommand(
                                  values_for_the_input[0].inputFieldContent, values_for_the_input[1].inputFieldContent, values_for_the_input[2].inputFieldContent
                                ).subscribe(
                                  data => {
                                    console.log('Data received: ', data);
                                    const parsedResult = JSON.parse(JSON.stringify(data));
                                    if (parsedResult.result === 'successful') {
                                      this.loaderService.hideLoader();
                                      this.appDialogService.showInfoDialog('OnNetwork device added successfully. Please refresh to interact with the device.');
                                    } else if (parsedResult.result === 'failed') {
                                      this.loaderService.hideLoader();
                                      this.appDialogService.showErrorDialog('Error adding OnNetwork device');
                                    }
                                  },
                                  error => {
                                    console.error('Error received: ', error);
                                    this.loaderService.hideLoader();
                                    this.appDialogService.showErrorDialog('Error adding OnNetwork device. Network error.');
                                  }
                                );
                               }, color: 'accent', icon: 'send' },
                              // { buttonName: 'Cancel', action: () => {
                              //   this.appDialogWithInputFieldsService.closeDialog();
                              //  }, color: 'warn', icon: 'cancel' },
                            ]
                          );
                      }, color: 'warn', icon: 'add' },
                      { buttonName: 'Add Ethernet Device', action: () => {
                          this.appDialogWithInputFieldsService.openDialog(
                            'Connect Ethernet Device', '../../../assets/matter-logo-transparent.png',
                            {
                              inputFields: [
                                {inputFieldType: 'text', inputFieldName: 'Device ID', inputFieldContent: '1234', inputFieldDefaultValue: 'Default: 1234'},
                                { inputFieldType: 'text', inputFieldName: 'Device Code (20202021)', inputFieldContent: '20202021', inputFieldDefaultValue: 'Default: 20202021' },
                                { inputFieldType: 'text', inputFieldName: 'Device Bluetooth Discriminator (3840)', inputFieldContent: '3840', inputFieldDefaultValue: 'Default: 3840'},
                                {inputFieldType: 'text', inputFieldName: 'Device IP Address', inputFieldContent: '127.0.0.1', inputFieldDefaultValue: 'Default: 127.0.0.1'},
                                {inputFieldType: 'number', inputFieldName: 'Device Port Number', inputFieldContent: '5540', inputFieldDefaultValue: 'Default: 5540'},
                              ]
                            },
                            [
                              { buttonName: 'Send Command', action: () => {
                                this.appDialogService.showInfoDialog('This function is not yet implemented.');
                               }, color: 'accent', icon: 'send' },
                              // { buttonName: 'Cancel', action: () => { this.appDialogWithInputFieldsService.closeDialog(); }, color: 'warn', icon: 'cancel' },
                            ]
                          );
                      }, color: 'warn', icon: 'add' },
                      // { buttonName: 'Cancel', action: () => { this.appDialogService.closeDialog(); }, color: 'warn', icon: 'cancel' },
                    ]
                  );
                }, color: 'accent', icon: 'add'
              },
              // { buttonName: 'Cancel', action: () => { this.appDialogService.closeDialog(); }, color: 'warn', icon: 'cancel' },
            ]
          )
        }), color: 'accent', icon: 'add'
      },

      {
        buttonName: 'About', action: () => emitButtonPressed({
          buttonName: 'About', action: () => this.appDialogService.openDialog(
            'About', '../../../assets/matter-logo-transparent.png', 'Matter controller version 1.2 Q3 2023',
            [
              // { buttonName: 'Cancel', action: () => { this.appDialogService.closeDialog(); }, color: 'warn', icon: 'cancel' },
            ]
          )
        }), color: 'accent', icon: 'info'
      },

      {
        buttonName: 'Network Map', action: () => emitButtonPressed({
          buttonName: 'Topology', action: () => this.appDialogService.openDialog(
            'Network Map', '../../../assets/matter-logo-transparent.png', 'View the network topology.',
            [
              { buttonName: 'Network Map', action: () => {
                this.appDialogService.showInfoDialog('This function is not yet implemented.'); }, color: 'accent', icon: 'map' },
              // { buttonName: 'Cancel', action: () => { this.appDialogService.closeDialog(); }, color: 'warn', icon: 'cancel' },
            ]
          )
        }), color: 'accent', icon: 'map'
      },

      {
        buttonName: 'OTA Updates', action: () => emitButtonPressed({
          buttonName: 'OTA', action: () => this.appDialogService.openDialog(
            'OTA Updates', '../../../assets/matter-logo-transparent.png', 'View available updates.',
            [
              { buttonName: 'OTA Updates', action: () => {
                this.appDialogService.showInfoDialog('This function is not yet implemented.'); }, color: 'accent', icon: 'update' },
              // { buttonName: 'Cancel', action: () => { this.appDialogService.closeDialog(); }, color: 'warn', icon: 'cancel' },
            ]
          )
        }), color: 'accent', icon: 'download'
      },
    ];
    return listOfButtonsAndActions;
  }
}
