import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ApplicationheaderComponent } from './applicationheader.component';

describe('ApplicationheaderComponent', () => {
  let component: ApplicationheaderComponent;
  let fixture: ComponentFixture<ApplicationheaderComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ApplicationheaderComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ApplicationheaderComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
