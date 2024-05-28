import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ApplicationendpointComponent } from './applicationendpoint.component';

describe('ApplicationendpointComponent', () => {
  let component: ApplicationendpointComponent;
  let fixture: ComponentFixture<ApplicationendpointComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ApplicationendpointComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ApplicationendpointComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
