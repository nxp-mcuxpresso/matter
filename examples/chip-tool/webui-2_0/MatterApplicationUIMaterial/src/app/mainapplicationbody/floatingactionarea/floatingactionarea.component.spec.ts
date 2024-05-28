import { ComponentFixture, TestBed } from '@angular/core/testing';

import { FloatingactionareaComponent } from './floating-action-area-component-actions';

describe('FloatingactionareaComponent', () => {
  let component: FloatingactionareaComponent;
  let fixture: ComponentFixture<FloatingactionareaComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [FloatingactionareaComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(FloatingactionareaComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
