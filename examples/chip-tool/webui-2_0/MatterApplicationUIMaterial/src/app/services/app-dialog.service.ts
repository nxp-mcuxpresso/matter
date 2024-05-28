import { Dialog, DialogConfig, DialogRef } from '@angular/cdk/dialog';
import {Injectable} from '@angular/core';

import {
  MatDialog,
  MatDialogRef,
  MatDialogActions,
  MatDialogClose,
  MatDialogTitle,
  MatDialogContent,
} from '@angular/material/dialog';

import {MatButtonModule} from '@angular/material/button';
import { MatIcon } from '@angular/material/icon';
import { CommonModule, NgIf } from '@angular/common';
import { Component, Input, NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import {MatRipple, MatRippleModule} from '@angular/material/core';

import { PopupdialogmaterialComponent } from '../popupdialogmaterial/popupdialogmaterial.component';

@Injectable (
  {
    providedIn:'root'
  }
)

export class AppDialogService {
  dialogConfig : DialogConfig = {
    panelClass: 'dialog-container',
  };

  dialogState: unknown;
  dialogRef? : DialogRef<unknown, PopupdialogmaterialComponent> | null;

  constructor(private dialog: Dialog) {}

  openDialog(title: string, dialogImageSrc: string, content: string, buttons: {buttonName: string, action: Function, color: string, icon?: string}[])
  : DialogRef<unknown,PopupdialogmaterialComponent> {
    this.dialogRef = this.dialog.open(PopupdialogmaterialComponent,
      {
        panelClass: 'dialog-container',
        data: {dialogImageSrc: dialogImageSrc, dialogTitle: title, dialogContent: content, dialogButtons: buttons}
      }
    );

    this.dialogRef.closed.subscribe((result : unknown) => {
      console.log(`Dialog result: ${result}`);
    });

    this.dialogRef.componentInstance!.dialogImageSrc = dialogImageSrc;
    this.dialogRef.componentInstance!.dialogTitle = title;
    this.dialogRef.componentInstance!.dialogContent = content;
    this.dialogRef.componentInstance!.dialogButtons = buttons;

    return this.dialogRef;
  }

  closeDialog() : void {
    this.dialogRef?.close();
  }
  reset () : void {
    this.dialogState = null;
  }

  showErrorDialog(error: string) : void {
    this.openDialog('Error', '../../../assets/warning.png', error, [{buttonName: 'OK', action: () => this.closeDialog(), color: 'primary'}]);
  }

  showInfoDialog(info: string) : void {
    this.openDialog('Information', '../../../assets/information.png', info, [{buttonName: 'OK', action: () => this.closeDialog(), color: 'primary'}]);
  }

  showWarningDialog(warning: string) : void {
    this.openDialog('Warning', '../../../assets/warning.png', warning, [{buttonName: 'OK', action: () => this.closeDialog(), color: 'primary'}]);
  }

  showConfirmDialog(confirm: string, action: Function) : void {
    this.openDialog('Confirm', '../../../assets/information.png', confirm, [{buttonName: 'Yes', action: action, color: 'primary'}, {buttonName: 'No', action: () => this.closeDialog(), color: 'warn'}]);
  }

  showCustomDialog(title: string, dialogImageSrc: string, content: string, buttons: {buttonName: string, action: Function, color: string, icon?: string}[]) : void {
    this.openDialog(title, dialogImageSrc, content, buttons);
  }

  showNotImplementedDialog() : void {
    this.openDialog('Work in progress, not yet implemented', '../../../assets/warning.png', 'This function is not yet implemented.', []);
  }


}
