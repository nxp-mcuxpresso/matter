import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AboutapplicationComponent } from './aboutapplication.component';

describe('AboutapplicationComponent', () => {
  let component: AboutapplicationComponent;
  let fixture: ComponentFixture<AboutapplicationComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [AboutapplicationComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(AboutapplicationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
