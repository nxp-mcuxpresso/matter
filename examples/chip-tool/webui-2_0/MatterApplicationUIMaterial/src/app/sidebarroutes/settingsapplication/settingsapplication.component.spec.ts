import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SettingsapplicationComponent } from './settingsapplication.component';

describe('SettingsapplicationComponent', () => {
  let component: SettingsapplicationComponent;
  let fixture: ComponentFixture<SettingsapplicationComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [SettingsapplicationComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(SettingsapplicationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
