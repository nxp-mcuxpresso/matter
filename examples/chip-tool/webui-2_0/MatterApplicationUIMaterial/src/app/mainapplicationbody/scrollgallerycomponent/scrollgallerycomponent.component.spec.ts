import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ScrollgallerycomponentComponent } from './scrollgallerycomponent.component';

describe('ScrollgallerycomponentComponent', () => {
  let component: ScrollgallerycomponentComponent;
  let fixture: ComponentFixture<ScrollgallerycomponentComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ScrollgallerycomponentComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ScrollgallerycomponentComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
