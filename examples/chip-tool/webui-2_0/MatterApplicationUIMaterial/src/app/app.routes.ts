import { Routes } from '@angular/router';
import { MainapplicationbodyComponent } from './mainapplicationbody/mainapplicationbody.component';
import { DevicesapplicationComponent } from './sidebarroutes/devicesapplication/devicesapplication.component';
import { SettingsapplicationComponent } from './sidebarroutes/settingsapplication/settingsapplication.component';
import { HelpapplicationComponent } from './sidebarroutes/helpapplication/helpapplication.component';
import { AudioapplicationComponent } from './sidebarroutes/audioapplication/audioapplication.component';
import { SubscriptionsapplicationComponent } from './sidebarroutes/subscriptionsapplication/subscriptionsapplication.component';

export const routes: Routes = [
  {path: '', redirectTo: 'dashboard', pathMatch: 'full'},
  {path: 'dashboard', component: MainapplicationbodyComponent},
  {path: 'devices', component: DevicesapplicationComponent},
  {path: 'settings', component: SettingsapplicationComponent},
  {path: 'help', component: HelpapplicationComponent},
  {path: 'audio', component: AudioapplicationComponent},
  {path: 'subscriptions', component: SubscriptionsapplicationComponent},
];
