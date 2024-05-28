import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ButtonpanelComponent } from './buttonpanel.component';

describe('ButtonpanelComponent', () => {
  let component: ButtonpanelComponent;
  let fixture: ComponentFixture<ButtonpanelComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ButtonpanelComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ButtonpanelComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
