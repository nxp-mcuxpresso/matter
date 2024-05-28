import { ComponentFixture, TestBed } from '@angular/core/testing';

import { MainapplicationbodyComponent } from './mainapplicationbody.component';

describe('MainapplicationbodyComponent', () => {
  let component: MainapplicationbodyComponent;
  let fixture: ComponentFixture<MainapplicationbodyComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [MainapplicationbodyComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(MainapplicationbodyComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
