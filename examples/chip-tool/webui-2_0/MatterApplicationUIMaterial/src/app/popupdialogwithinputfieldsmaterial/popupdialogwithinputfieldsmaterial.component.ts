import { Component, Input } from '@angular/core';

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
import { BrowserModule } from '@angular/platform-browser';
import {Dialog, DialogModule, DialogRef} from '@angular/cdk/dialog';
import {MatRipple, MatRippleModule} from '@angular/material/core';
import { MatChip, MatChipsModule } from '@angular/material/chips';
import { FormsModule, NgModel } from '@angular/forms';
import { MatFormField, MatLabel } from '@angular/material/form-field';

import { MatInputModule } from '@angular/material/input';
import { MatFormFieldModule } from '@angular/material/form-field';
import {ScrollingModule} from '@angular/cdk/scrolling';
import {ThemePalette} from '@angular/material/core';

@Component({
  selector: 'app-popupdialogwithinputfieldsmaterial',
  standalone: true,
  imports: [MatButtonModule, MatDialogActions, FormsModule,
    MatLabel, MatFormField, MatRippleModule, MatDialogClose, MatDialogTitle,
    MatDialogContent, MatIcon, NgIf, CommonModule, DialogModule, MatChip, MatChipsModule, MatInputModule, MatFormFieldModule, ScrollingModule],
  templateUrl: './popupdialogwithinputfieldsmaterial.component.html',
  styleUrl: './popupdialogwithinputfieldsmaterial.component.css'
})


export class PopupdialogwithinputfieldsmaterialComponent {
  constructor(public dialogRef: DialogRef<PopupdialogwithinputfieldsmaterialComponent>) {}

  dialogType: string = 'info';

  @Input() dialogImageSrc!: string;
  @Input() dialogTitle!: string;
  @Input() dialogContent!: {inputFields: {inputFieldType: string, inputFieldName: string, inputFieldContent: string, inputFieldDefaultValue: string}[]};
  @Input() dialogButtons!: {buttonName: string, action: Function, color: string, icon?: string}[];
  @Input() dialogAvailableSelectionItems? : {name: string, color: string}[];
  public selectedSelectionItem?: string;

  onNoClick(): void {
    this.dialogRef.close();
  }

  onYesClick(): void {
    this.dialogRef.close();
  }

  onActionClick(action: Function): void {
    action();
  }

  public onSelectOption(option: string): void {
    this.selectedSelectionItem = option;
    console.log('Selected option: ', option);
  }

  public getSelectedItemFromAvailableSelectionItems() : string | undefined{
    return this.selectedSelectionItem;
  }

  public getDialogTitle(): string {
    return this.dialogTitle;
  }

  public getDialogContent(): {inputFields: {inputFieldType: string, inputFieldName: string, inputFieldContent: string, inputFieldDefaultValue: string}[]} {
    return this.dialogContent;
  }

  public getDialogButtons(): {buttonName: string, action: Function, color: string, icon?: string }[] {
    return this.dialogButtons;
  }

  public getInputFields(): {inputFieldType: string, inputFieldName: string, inputFieldContent: string, inputFieldDefaultValue: string}[] {
    return this.dialogContent.inputFields;
  }

  public getDialogImageSrc(): string {
    return this.dialogImageSrc;
  }

  public getDialogType(): string {
    return this.dialogType;
  }

  public getDialogAvailableSelectionItems(): {name: string, color: string}[] {
    return this.dialogAvailableSelectionItems!;
  }

}
