import { Component, EventEmitter, Input, OnDestroy, Output } from '@angular/core';
import { NgOptimizedImage, NgStyle, NgClass } from '@angular/common'
import {MatMenuModule} from '@angular/material/menu';
import {MatIconModule} from '@angular/material/icon';
import {MatToolbarModule} from '@angular/material/toolbar';
import {MatChipsModule} from '@angular/material/chips';
import { ButtonpanelComponent } from './buttonpanel/buttonpanel.component';
import {OnInit } from '@angular/core';
import { NgbCollapse } from '@ng-bootstrap/ng-bootstrap';
import {MatButtonModule} from '@angular/material/button';
import { MatSidenav } from '@angular/material/sidenav';

import { AppDialogWithInputFieldsService } from '../services/app-dialog-input.service';

@Component({
  selector: 'app-applicationheader',
  standalone: true,
  imports: [NgOptimizedImage, MatMenuModule, MatIconModule, MatToolbarModule,
    MatChipsModule, ButtonpanelComponent, NgClass, NgStyle, NgbCollapse, MatButtonModule],
  templateUrl: './applicationheader.component.html',
  styleUrl: './applicationheader.component.css'
})

export class ApplicationheaderComponent implements OnInit, OnDestroy{

  constructor (
    private appDialogWithInputFieldsService: AppDialogWithInputFieldsService) {}

  @Input() sidebarStatus: boolean = false;
  @Output()
  public SidebarCollapsed : EventEmitter<boolean> = new EventEmitter<boolean>();
  @Output()
  public buttonPressedEvent : EventEmitter<{buttonName: string, action: Function}>
    = new EventEmitter<{buttonName: string, action: Function}>();

  ngOnDestroy(): void {
    throw new Error('Method not implemented.');
  }

  ngOnInit(): void {}

  toggleSidebar() {
    this.sidebarStatus = !this.sidebarStatus;
    this.SidebarCollapsed.emit(this.sidebarStatus);
  }

  public getSidebarStatus(): boolean {
    return this.sidebarStatus;
  }


  onButtonPressedEvent(button: {buttonName: string, action: Function}) {
    this.buttonPressedEvent.emit(button);
  }

}
