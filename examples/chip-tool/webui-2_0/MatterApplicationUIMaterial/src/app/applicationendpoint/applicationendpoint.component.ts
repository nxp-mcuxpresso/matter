import { Component, Input, OnChanges, OnInit, SimpleChange, SimpleChanges } from '@angular/core';
import { Injectable } from '@angular/core';
import { MatCard, MatCardActions, MatCardContent } from '@angular/material/card';
import { MatIconModule } from '@angular/material/icon';
import { MatCardHeader } from '@angular/material/card';
import {MatCardTitle} from '@angular/material/card';
import { NgClass, NgFor, NgIf, NgStyle } from '@angular/common';
import { MatCardSubtitle } from '@angular/material/card';
import { EndpointModel } from '../models/endpoint-model';
import { DimmableLightModel } from '../models/dimmable-light-model';
import { LightingModel } from '../models/lighting-model';
import {MatRippleModule} from '@angular/material/core';
import { MatChip } from '@angular/material/chips';

@Component({
  selector: 'app-applicationendpoint',
  standalone: true,
  imports: [MatCard, NgClass, NgFor, NgIf, NgStyle,
    MatRippleModule, MatCardSubtitle, MatCardTitle, MatCardHeader, MatIconModule, MatCardContent, MatCardActions, MatChip],
  templateUrl: './applicationendpoint.component.html',
  styleUrl: './applicationendpoint.component.css'
})

export class ApplicationendpointComponent {

  @Input() endpointModel!: EndpointModel;

  constructor() {}

  public getEndpoint(): EndpointModel {
    return this.endpointModel;
  }

  public turnOnDevice(): void {
    this.endpointModel.endpointStatus = true;
  }

  public turnOffDevice(): void {
    this.endpointModel.endpointStatus = false;
  }

  public getEndpointStatus(): boolean {
    return this.endpointModel.endpointStatus;
  }

  public openEndpointDetails() {
    throw new Error('Method not implemented.');
  }

}
