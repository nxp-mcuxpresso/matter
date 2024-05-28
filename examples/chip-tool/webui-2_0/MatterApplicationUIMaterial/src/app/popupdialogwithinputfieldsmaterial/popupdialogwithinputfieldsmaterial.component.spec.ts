import { ComponentFixture, TestBed } from '@angular/core/testing';

import { PopupdialogwithinputfieldsmaterialComponent } from './popupdialogwithinputfieldsmaterial.component';

describe('PopupdialogwithinputfieldsmaterialComponent', () => {
  let component: PopupdialogwithinputfieldsmaterialComponent;
  let fixture: ComponentFixture<PopupdialogwithinputfieldsmaterialComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [PopupdialogwithinputfieldsmaterialComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(PopupdialogwithinputfieldsmaterialComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
