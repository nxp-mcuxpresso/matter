import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DevicesapplicationComponent } from './devicesapplication.component';

describe('DevicesapplicationComponent', () => {
  let component: DevicesapplicationComponent;
  let fixture: ComponentFixture<DevicesapplicationComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [DevicesapplicationComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(DevicesapplicationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
