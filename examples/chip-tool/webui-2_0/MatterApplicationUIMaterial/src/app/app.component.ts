import { WebSocketService } from './services/web-socket.service';
import { FootercomponentComponent } from './footercomponent/footercomponent.component';
import { Component, OnDestroy, OnInit } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import {MatChipsModule} from '@angular/material/chips';
import { ThemePalette } from '@angular/material/core';
import { NgbModal, NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { CommonModule, NgStyle } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { ApplicationheaderComponent } from './applicationheader/applicationheader.component';
import { MainapplicationbodyComponent } from './mainapplicationbody/mainapplicationbody.component';
import { MatSidenavContainer } from '@angular/material/sidenav';
import { ApplicationsidebarComponent } from './applicationsidebar/applicationsidebar.component';
import { MatSidenav } from '@angular/material/sidenav';
import {MatSidenavModule} from '@angular/material/sidenav';

// For mat-toolbar
import {MatToolbarModule} from '@angular/material/toolbar';
import { ViewChild } from '@angular/core';

import { FloatingactionareaComponent } from './mainapplicationbody/floatingactionarea/floatingactionarea.component';
import { LoaderoverlayComponent } from './loaderoverlay/loaderoverlay.component';


export interface ChipColor {
  name: string;
  color: ThemePalette;
}

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, MatChipsModule,CommonModule, NgbModule,FormsModule,
    ApplicationheaderComponent, FloatingactionareaComponent, NgStyle, MatToolbarModule, FootercomponentComponent,
    MatSidenavModule, MainapplicationbodyComponent, MatSidenavContainer, ApplicationsidebarComponent, ApplicationsidebarComponent,
    LoaderoverlayComponent
  ],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})

export class AppComponent implements OnInit, OnDestroy  {
  private events: string[] = [];
  public title: string;
  public isLoading: boolean = false;

  private isDrawerOpen: boolean = false;

  @ViewChild('drawer') drawer!: MatSidenav;

  constructor(private webSocketService: WebSocketService) {
    this.title = "Matter Web Application";
  }

  ngOnInit(): void {
    this.initializeSocketConnection();
    // this.webSocketService.messageReceived.subscribe((message: string) => {
    //   console.log('Message received in AppComponent: ' + message);
    // }
   }

   ngOnDestroy() {
    this.disconnectSocket();
   }

   initializeSocketConnection() {
    this.webSocketService.connect();
   }

   disconnectSocket() {
    this.webSocketService.closeConnection();
   }


  onSidebarColapsedEventCatch(value: boolean) {
    console.log('Sidebar collapsed event caught in the AppComponent; value: ' + value);
    this.isDrawerOpen = value;
    this.drawer.toggle();
  }



  public getTitle(): string {
    return this.title;
  }

  public getIsDrawerOpen(): boolean {
    return this.isDrawerOpen;
  }

  public setIsDrawerOpen(value: boolean) {
    this.isDrawerOpen = value;
  }

  public getIsLoading(): boolean {
    return this.isLoading;
  }

  public setIsLoading(value: boolean) {
    this.isLoading = value;
  }
}
