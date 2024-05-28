import { LoaderService } from './../../services/loader.service';
import { PostRequestsService } from './../../services/post-requests.service';
import { AppDialogWithInputFieldsService } from './../../services/app-dialog-input.service';
import { Component, EventEmitter, Output } from '@angular/core';
import { MatChip, MatChipListbox } from '@angular/material/chips';
import { MatIcon } from '@angular/material/icon';
import {MatChipsModule} from '@angular/material/chips';
import { NgFor, NgStyle } from '@angular/common';
import { PopupdialogmaterialComponent } from '../../popupdialogmaterial/popupdialogmaterial.component';
import { MatDialog, MatDialogActions, MatDialogClose } from '@angular/material/dialog';
import { Dialog } from '@angular/cdk/dialog';
import { FloatingActionAreaComponentActions } from './floating-action-area-component-actions';
import {GetRequestsService} from '../../services/get-requests.service';

import { AppDialogService } from '../../services/app-dialog.service';

@Component({
  selector: 'app-floatingactionarea',
  standalone: true,
  imports: [MatIcon, MatChip, MatChipListbox, MatChipsModule, NgFor, NgStyle, PopupdialogmaterialComponent, MatDialogActions, MatDialogClose],
  templateUrl: './floatingactionarea.component.html',
  styleUrl: './floatingactionarea.component.css'
})
export class FloatingactionareaComponent {

  constructor (private appDialogService : AppDialogService, private appDialogWithInputFieldsService : AppDialogWithInputFieldsService,
    private getRequestsService: GetRequestsService, private postRequestsService: PostRequestsService, private loaderService: LoaderService
  ) {}

  @Output()
  public buttonPressedEvent : EventEmitter<{buttonName: string, action: Function}>
    = new EventEmitter<{buttonName: string, action: Function}>();

  emitButtonPressed(button: {buttonName: string, action: Function}) {
    console.log("Emitting button pressed event from the floating action area component");
    this.buttonPressedEvent.emit(button); 
  }

  public getListOfButtonsAndActions() {
    const floatingActionAreaActions = new FloatingActionAreaComponentActions(this.appDialogService, this.appDialogWithInputFieldsService, 
      this.postRequestsService, this.getRequestsService, this.loaderService
    );
    return floatingActionAreaActions.getButtonPressedListWithEmitFunction(
      (paramFunc : {buttonName: string, action: Function}) => this.emitButtonPressed(paramFunc));
  }

}
