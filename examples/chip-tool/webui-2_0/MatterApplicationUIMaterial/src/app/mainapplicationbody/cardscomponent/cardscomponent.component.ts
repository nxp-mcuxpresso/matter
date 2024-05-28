import { Component, Directive, ElementRef, Input, ViewChild, OnInit, AfterViewInit, OnDestroy, Inject } from '@angular/core';
import { ApplicationcardComponent } from '../../applicationcard/applicationcard.component';
import { NgFor, NgIf, NgStyle } from '@angular/common';
import { CardModel } from '../../models/card-model';
import { NgxMasonryComponent, NgxMasonryModule } from 'ngx-masonry';

@Component({
  selector: 'app-cardscomponent',
  standalone: true,
  imports: [ApplicationcardComponent, NgFor, NgxMasonryModule, NgStyle, NgIf],
  templateUrl: './cardscomponent.component.html',
  styleUrl: './cardscomponent.component.css'
})
export class CardscomponentComponent implements AfterViewInit, OnDestroy {
  ngAfterViewInit(): void {
    throw new Error('Method not implemented.');
  }
  private MassonryLayoutGap = 20;

  // The cards that will be displayed in the cards component
  @Input() cards!: CardModel[];
  @Input() cardsHaveSubscriptionOption!: boolean;
  @ViewChild('ngxmasonry_container') masonrycontainer!: ElementRef;
  @ViewChild('masonry_item_container') masonryitemcontainer!: NgxMasonryComponent;
  @ViewChild('card_component_container') cardcomponentcontainer!: ElementRef;

  reloadMasonryLayout() {
    if (this.masonryitemcontainer !== undefined) {
      this.masonryitemcontainer.reloadItems();
      this.masonryitemcontainer.layout();
    }
  }

  ngOnDestroy(): void {
    console.log('Destroying Cards Component');
  }

  public getMasonryContainerWidth(): number {
    return this.masonrycontainer.nativeElement.offsetWidth;
  }


  public getCardComponentContainerWidth(): number {
    return this.cardcomponentcontainer.nativeElement.offsetWidth;
  }

  public getMasonryLayoutGap(): number {
    return this.MassonryLayoutGap;
  }

  public getNumberOfCards() {
    return this.cards.length;
  }

}
