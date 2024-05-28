import { ComponentFixture, TestBed } from '@angular/core/testing';

import { FootercomponentComponent } from './footercomponent.component';

describe('FootercomponentComponent', () => {
  let component: FootercomponentComponent;
  let fixture: ComponentFixture<FootercomponentComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [FootercomponentComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(FootercomponentComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
