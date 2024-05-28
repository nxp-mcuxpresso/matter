import { ComponentFixture, TestBed } from '@angular/core/testing';

import { PopupdialogmaterialComponent } from './popupdialogmaterial.component';

describe('PopupdialogmaterialComponent', () => {
  let component: PopupdialogmaterialComponent;
  let fixture: ComponentFixture<PopupdialogmaterialComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [PopupdialogmaterialComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(PopupdialogmaterialComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
