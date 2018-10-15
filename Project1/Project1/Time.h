/*
============================================================================
----------------------------------------------------------------------------
Name        : Project 1
Author      : Adam Williams
Version     : 1.0
Copyright   : 2018
Description : Program implements Bell-LaPadula security rules using a
              reference monitor to grant access to interactions between
              various subjects and objects              
----------------------------------------------------------------------------
============================================================================
*/

#pragma once
#include<string>
#include<ctime>
using namespace std;

class Time {
  public:
  Time() {
    time(&rawtime);
    timer = localtime(&rawtime);      
  }

  string getTimeAndDate() {
    char timeStr[50];
    sprintf(timeStr,"%02d/%02d/%02d %02d:%02d:%02d",
        timer->tm_mon+1,timer->tm_mday,timer->tm_year%100,
        timer->tm_hour,timer->tm_min,timer->tm_sec);
    return string{timeStr};
  }

  string getFullTime() {
    return asctime(timer);
  }

  string getTime() {
    char timeStr[50];
    sprintf(timeStr,"%02d:%02d:%02d",
            timer->tm_hour%12,timer->tm_min,timer->tm_sec);
    return string{timeStr};
  }

  time_t rawtime;
  struct tm* timer;    
};

