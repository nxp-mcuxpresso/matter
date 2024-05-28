import { Component } from '@angular/core';
import { MatCardModule } from '@angular/material/card';
import { MatIcon, MatIconModule } from '@angular/material/icon';
import { ScrollgallerycomponentComponent } from '../../mainapplicationbody/scrollgallerycomponent/scrollgallerycomponent.component';

@Component({
  selector: 'app-audioapplication',
  standalone: true,
  // Import mat-card module
  imports: [MatCardModule, MatIconModule, 
    ScrollgallerycomponentComponent
  ],
  templateUrl: './audioapplication.component.html',
  styleUrl: './audioapplication.component.css'
})
export class AudioapplicationComponent {

}
