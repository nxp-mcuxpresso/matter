import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ApplicationcardComponent } from './applicationcard.component';

describe('ApplicationcardComponent', () => {
  let component: ApplicationcardComponent;
  let fixture: ComponentFixture<ApplicationcardComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ApplicationcardComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ApplicationcardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
