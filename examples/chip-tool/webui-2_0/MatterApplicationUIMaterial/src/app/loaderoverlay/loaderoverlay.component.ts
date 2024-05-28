import { CommonModule, NgIf } from '@angular/common';
import { Component, OnInit, Input, Directive } from '@angular/core';
import { LoaderService } from '../services/loader.service';

@Component({
  selector: 'app-loaderoverlay',
  standalone: true,
  imports: [CommonModule, NgIf],
  templateUrl: './loaderoverlay.component.html',
  styleUrl: './loaderoverlay.component.css'
})

export class LoaderoverlayComponent implements OnInit {
  // public message: string = 'Loading...'
  constructor(private loaderService: LoaderService) {
  }
  ngOnInit(): void {

  }
}
