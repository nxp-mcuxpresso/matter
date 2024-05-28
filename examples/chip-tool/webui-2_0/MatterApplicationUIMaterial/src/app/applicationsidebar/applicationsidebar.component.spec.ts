import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ApplicationsidebarComponent } from './applicationsidebar.component';

describe('ApplicationsidebarComponent', () => {
  let component: ApplicationsidebarComponent;
  let fixture: ComponentFixture<ApplicationsidebarComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ApplicationsidebarComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ApplicationsidebarComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
