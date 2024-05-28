import { ComponentFixture, TestBed } from '@angular/core/testing';

import { HelpapplicationComponent } from './helpapplication.component';

describe('HelpapplicationComponent', () => {
  let component: HelpapplicationComponent;
  let fixture: ComponentFixture<HelpapplicationComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [HelpapplicationComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(HelpapplicationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
