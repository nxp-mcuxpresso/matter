import { Overlay } from "@angular/cdk/overlay"
import { ComponentPortal } from "@angular/cdk/portal"
import { Directive, ElementRef, Input } from "@angular/core"
import { Observable } from "rxjs/internal/Observable"
import { Subscription } from "rxjs/internal/Subscription"
import { LoaderoverlayComponent } from "./loaderoverlay.component"

@Directive({
  selector: '[appLoader]'
})
export class LoaderDirective {
  
  overlayRef = this.overlay.create({
      positionStrategy: this.overlay.position().global().centerHorizontally().centerVertically(),
      hasBackdrop: true
  })

  currentSubscription: Subscription | undefined

  @Input()
  set appOverlayLoader(value: Observable<any>) {
      this.currentSubscription?.unsubscribe()
      this.showLoader()
      
      this.currentSubscription = value.subscribe(
          {
              next: () => this.hideLoader(),
              complete: () => this.hideLoader(),
              error: () => this.hideLoader()
          }
      )

      this.overlay.create({
        scrollStrategy: this.overlay.scrollStrategies.reposition(),
        positionStrategy: this.overlay.position()
            .flexibleConnectedTo(this.elementRef)
            .withPush(false)
            .withPositions(
                [
                    {
                        originX: "center",
                        originY: "center",
                        overlayX: "center",
                        overlayY: "center"
                    }
                ]
            )
    })
  };

  constructor(
      private elementRef: ElementRef,
      private overlay: Overlay,
  ) {
  }

  private showLoader() {
      this.overlayRef.attach(new ComponentPortal(LoaderoverlayComponent))
  }

  private hideLoader() {
      this.overlayRef.detach()
  }
}