import { Component, ElementRef, HostListener, ViewChild } from '@angular/core';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import {MatCard, MatCardModule} from '@angular/material/card';
import { NgClass, NgFor, NgIf, NgStyle } from '@angular/common';
import { ApplicationcardComponent } from '../../applicationcard/applicationcard.component';

// Models import
import { CardModel } from '../../models/card-model';
import { EndpointModel } from '../../models/endpoint-model';
import { LightingModel } from '../../models/lighting-model';
import { DimmableLightModel } from '../../models/dimmable-light-model';
import { CardscomponentComponent } from '../../mainapplicationbody/cardscomponent/cardscomponent.component';
import { FloatingactionareaComponent } from '../../mainapplicationbody/floatingactionarea/floatingactionarea.component';
import { ScrollgallerycomponentComponent } from '../../mainapplicationbody/scrollgallerycomponent/scrollgallerycomponent.component';
import { MatDialog } from '@angular/material/dialog';

@Component({
  selector: 'app-helpapplication',
  standalone: true,
  imports: [NgbModule, MatCardModule, MatCard, NgClass, NgFor, NgIf, NgStyle, ApplicationcardComponent, CardscomponentComponent, FloatingactionareaComponent, ScrollgallerycomponentComponent],
  templateUrl: './helpapplication.component.html',
  styleUrl: './helpapplication.component.css'
})
export class HelpapplicationComponent {
  public open(modal: any): void {
    this.modalService.open(modal);
  }

  private events: string[] = [];
  private title: string;

  constructor(private modalService: NgbModal, public dialog: MatDialog) {
    this.title = "Matter Web Application";
  }

  onButtonPressedEventCatch(value: {
    buttonName: string;
    action: Function;
  }) {
    console.log('Button pressed event caught in the ApplicationBodyComponent; value: ' + value.buttonName);
    value.action();
  };

  // onResize() {
  //   this.cards_component_div.nativeElement.style.width = window.innerWidth * 1 / 2 + 'px';

  //   const cardWidth = this.cards_component_component.getCardComponentContainerWidth();
  //   const windowWidth = window.innerWidth;
  //   const numCards = Math.floor(windowWidth / cardWidth) > this.cards_component_component.getNumberOfCards() ? this.cards_component_component.getNumberOfCards() : Math.floor(windowWidth / cardWidth);
  //   this.cards_component_div.nativeElement.style.width = numCards * (cardWidth + this.cards_component_component.getMasonryLayoutGap())  + 'px';

  //   this.cards_component_component.reloadMasonryLayout();

  // }

  ngOnInit(): void {}

  ngOnDestroy(): void {}

  // Hardcoded cards
  // Card: imageUrl, deviceId, endpointsList, isOnline, deviceName
  // Endpoint: endpointId, endpointName, endpointImageURL, endpointSubscriptionId, endpointStatus (all string values)

  // public cards: CardModel[] = [
  //   new CardModel('../../assets/thread-logo-transparent.png', 1, [
  //     new EndpointModel('1', 'Switch 1', '../../assets/endpoint/onoffLight.png', true,'subId1',),
  //     new EndpointModel('2', 'Switch 2', '../../assets/endpoint/onoffLight.png', false, 'subId2'),
  //     new EndpointModel('3', 'Switch 3', '../../assets/endpoint/onoffLight.png', true, 'subId3'),
  //   ], true, 'Switch Card 1'),
  //   new CardModel('../../assets/wifi-logo-transparent.png', 2, [
  //     new DimmableLightModel(new LightingModel(new EndpointModel('1', 'Dimmable Light 1', '../../assets/endpoint/onoffLight.png', true, 'subId1'), 'orange', 50), 10),
  //     new DimmableLightModel(new LightingModel(new EndpointModel('2', 'Dimmable Light 2', '../../assets/endpoint/onoffLight.png', false, 'subId2'), 'blue', 0), 10),
  //     new DimmableLightModel(new LightingModel(new EndpointModel('3', 'Dimmable Light 3', '../../assets/endpoint/onoffLight.png', true, 'subId3'), 'green', 100), 10),
  //   ], true, 'Dimmable Light Card 1'),
  //   new CardModel('../../assets/wifi-logo-transparent.png', 3, [
  //     new LightingModel(new EndpointModel('1', 'Light 1', '../../assets/endpoint/onoffLight.png', true, 'subId1'), 'orange', 50),
  //     new LightingModel(new EndpointModel('2', 'Light 2', '../../assets/endpoint/onoffLight.png', false, 'subId2'), 'blue', 0),
  //   ], true, 'Lighting Card 1'),
  // ];
}
