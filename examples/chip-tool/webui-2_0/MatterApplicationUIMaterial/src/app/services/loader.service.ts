import { Injectable } from '@angular/core';
import { Overlay } from '@angular/cdk/overlay';
import { ComponentPortal } from '@angular/cdk/portal';
import { LoaderoverlayComponent } from '../loaderoverlay/loaderoverlay.component';

@Injectable({
  providedIn: 'root',
})
export class LoaderService {
  overlayRef = this.overlay.create({
    positionStrategy: this.overlay
      .position()
      .global()
      .centerHorizontally()
      .centerVertically(),
    hasBackdrop: true,
  });

  constructor(private overlay: Overlay) {}

  showLoader() {
    this.overlayRef.attach(new ComponentPortal(LoaderoverlayComponent));
  }

  hideLoader() {
    this.overlayRef.detach();
  }
}
