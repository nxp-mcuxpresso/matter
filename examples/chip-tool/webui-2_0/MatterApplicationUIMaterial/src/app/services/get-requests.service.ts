import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable, catchError, throwError } from 'rxjs';
import { API_BASE_URL } from '../../api_addresses';

@Injectable({
  providedIn: 'root'
})

export class GetRequestsService {

  constructor(private http: HttpClient) { }
  serverUrl: string = API_BASE_URL;

  getInformationAboutTheBackend() : Observable<string> {
    return this.http.get<string>(
      `${this.serverUrl}/info`).pipe(
      catchError((error: any) => {
        console.error('Error getting information: ', error);
        return throwError(error);
      }
    ));
  }

  getDevicesIDsAndAliases() : Observable<string> {
    return this.http.get<string>(
      `${this.serverUrl}/get_status`).pipe(
      catchError((error: any) => {
        console.error('Error getting devices status: ', error);
        return throwError(error);
      }
    ));
  }

  getDataset(): Observable<string> {
    return this.http.get<string>(
      `${this.serverUrl}/get_dataset`).pipe(
      catchError((error: any) => {
        console.error('Error getting dataset: ', error);
        return throwError(error);
      }
    ));
  }
}
