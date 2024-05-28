import {
  Component,
  ElementRef,
  EventEmitter,
  Input,
  Output,
  SimpleChanges,
  ViewChild,
} from '@angular/core';
import {
  MatDrawer,
  MatSidenav,
  MatSidenavContainer,
  MatSidenavModule,
} from '@angular/material/sidenav';
import { MatPseudoCheckboxModule } from '@angular/material/core';
import { MatCheckboxModule } from '@angular/material/checkbox';
import { FormsModule } from '@angular/forms';
import { NgClass, NgFor, NgStyle } from '@angular/common';
import { MatSidenavContent } from '@angular/material/sidenav';
import { MdbCheckboxModule } from 'mdb-angular-ui-kit/checkbox';

import { MatIconModule } from '@angular/material/icon';
import { MatChip, MatChipsModule } from '@angular/material/chips';
import { Router } from '@angular/router';

@Component({
  selector: 'app-applicationsidebar',
  standalone: true,
  imports: [
    MatSidenavModule,
    MatCheckboxModule,
    FormsModule,
    NgFor,
    MatSidenavContent,
    NgClass,
    MatSidenav,
    MatIconModule,
    NgStyle,
    MatChip,
    MatChipsModule
  ],
  templateUrl: './applicationsidebar.component.html',
  styleUrl: './applicationsidebar.component.css',
})
export class ApplicationsidebarComponent {
  constructor(private router: Router) {}

  @Input() public isDrawerOpen!: boolean;
  @Output() public isCollapsedChange = new EventEmitter<boolean>();
  @ViewChild('drawer') drawer!: MatDrawer;
  @ViewChild('listgroup_item') listgroupitem!: ElementRef;

  public activeItemFromList!: number;

  // public toggleSidebar(): void {
  //   this.isDrawerOpen = !this.isDrawerOpen;
  //   this.isCollapsedChange.emit(this.isDrawerOpen);
  //   console.log('isCollapsed in Sidebar Component: ', this.isDrawerOpen);
  // }

  public onClick(index : number) : void {
    console.log('Clicked on item: ', index);
    this.activeItemFromList = index;
    this.router.navigate([this.sidebarNavigationItems[index].linkRoute]);
  }


  public sidebarNavigationItems = [
    {
      name: 'Dashboard',
      icon: 'dashboard',
      linkRoute: 'dashboard',
    },
    {
      name: 'Devices',
      icon: 'devices',
      linkRoute: 'devices',
    },
    {
      name: 'Subscriptions',
      icon: 'subscriptions',
      linkRoute: 'subscriptions',
    },
    {
      name: 'Audio',
      icon: 'audiotrack',
      linkRoute: 'audio'
    },
    {
      name: 'Help',
      icon: 'help',
      linkRoute: 'help'
    }
  ];

}
