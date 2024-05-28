import { Component, ElementRef, HostListener, ViewChild } from '@angular/core';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import {MatCard, MatCardModule} from '@angular/material/card';
import { NgClass, NgFor, NgIf, NgStyle } from '@angular/common';

// Models import
import { MatDialog } from '@angular/material/dialog';
import { ScrollgallerycomponentComponent } from '../../mainapplicationbody/scrollgallerycomponent/scrollgallerycomponent.component';
import { ApplicationcardComponent } from '../../applicationcard/applicationcard.component';
import { CardscomponentComponent } from '../../mainapplicationbody/cardscomponent/cardscomponent.component';
import { FloatingactionareaComponent } from '../../mainapplicationbody/floatingactionarea/floatingactionarea.component';
import { CardModel } from '../../models/card-model';
import { EndpointModel } from '../../models/endpoint-model';
import { DimmableLightModel } from '../../models/dimmable-light-model';
import { LightingModel } from '../../models/lighting-model';
import { GetRequestsService } from '../../services/get-requests.service';
import { AppDialogService } from '../../services/app-dialog.service';

@Component({
  selector: 'app-mainapplicationbody',
  standalone: true,
  imports: [NgbModule, MatCardModule, MatCard, NgClass, NgFor, NgIf, NgStyle,
     ScrollgallerycomponentComponent, ApplicationcardComponent, CardscomponentComponent, FloatingactionareaComponent],
  templateUrl: './devicesapplication.component.html',
  styleUrl: './devicesapplication.component.css'
})

export class DevicesapplicationComponent {

  public open(modal: any): void {
    this.modalService.open(modal);
  }

  @ViewChild('containingappcards_component_div') cards_component_div!: ElementRef;
  @ViewChild('cards_component') cards_component_component!: CardscomponentComponent;
  private events: string[] = [];
  private title: string;

  constructor(private modalService: NgbModal, private dialog: MatDialog, private getRequestsService: GetRequestsService, private appDialogService: AppDialogService) {
    this.title = "Matter Web Application";
  }

  onButtonPressedEventCatch(value: {
    buttonName: string;
    action: Function;
  }) {
    console.log('Button pressed event caught in the ApplicationBodyComponent; value: ' + value.buttonName);
    value.action();
  };

  onResize() {
    this.cards_component_div.nativeElement.style.width = window.innerWidth * 1 / 2 + 'px';

    const cardWidth = this.cards_component_component.getCardComponentContainerWidth();
    const windowWidth = window.innerWidth;
    const numCards = Math.floor(windowWidth / cardWidth) > this.cards_component_component.getNumberOfCards() ? this.cards_component_component.getNumberOfCards() : Math.floor(windowWidth / cardWidth);
    this.cards_component_div.nativeElement.style.width = numCards * (cardWidth + this.cards_component_component.getMasonryLayoutGap())  + 'px';

    this.cards_component_component.reloadMasonryLayout();

  }

  private intervalId: any;

  ngOnInit(): void {
    this.getRequestsService.getDevicesIDsAndAliases().subscribe((data: any) => {
      const parsedData = JSON.parse(JSON.stringify(data));
      if (parsedData['result'] === 'successful') {
        // for (let i = 0; i < parsedData['status'].length; i++) {
        //   console.log(parsedData['status'][i]);
        //   const device = parsedData['status'][i];
        //   const deviceType = device['type'];
        //   const deviceId = device['id'];
        //   const deviceAlias = device['alias'];
        //   const deviceStatus = device['status'];
        //   const deviceEndpoints = device['endpoints'];

        //   let card: CardModel;
        //   if (deviceType === 'switch') {
        //     card = new CardModel('../../assets/thread-logo-transparent.png', deviceId, deviceEndpoints, deviceStatus, deviceAlias);
        //   } else if (deviceType === 'dimmable_light') {
        //     card = new CardModel('../../assets/wifi-logo-transparent.png', deviceId, deviceEndpoints, deviceStatus, deviceAlias);
        //   } else if (deviceType === 'lighting') {
        //     card = new CardModel('../../assets/wifi-logo-transparent.png', deviceId, deviceEndpoints, deviceStatus, deviceAlias);
        //   } else {
        //     card = new CardModel('../../assets/wifi-logo-transparent.png', deviceId, deviceEndpoints, deviceStatus, deviceAlias);
        //   }
        //   this.cards.push(card);
        // }
        for (const [key, value] of Object.entries(parsedData['status'])) {
          let card: CardModel;
          const device = value as string;
          card = new CardModel('../../assets/wireless-transparent.png', parseInt(device), [], true, key);
          this.cards.push(card);
        }
      } else {
        this.appDialogService.showErrorDialog('Error getting devices IDs and aliases');
      }
    });
  }

  ngOnDestroy(): void {}

  public cards: CardModel[] = [];
}
