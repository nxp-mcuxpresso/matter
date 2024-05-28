import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SubscriptionsapplicationComponent } from './subscriptionsapplication.component';

describe('SubscriptionsapplicationComponent', () => {
  let component: SubscriptionsapplicationComponent;
  let fixture: ComponentFixture<SubscriptionsapplicationComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [SubscriptionsapplicationComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(SubscriptionsapplicationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
