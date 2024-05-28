import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AudioapplicationComponent } from './audioapplication.component';

describe('AudioapplicationComponent', () => {
  let component: AudioapplicationComponent;
  let fixture: ComponentFixture<AudioapplicationComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [AudioapplicationComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(AudioapplicationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
