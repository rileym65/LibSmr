/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <time.h>
#include <sys/time.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  StopWatch::StopWatch() {
    objectType = (char*)"StopWatch";
    running = false;
    }
  
  StopWatch::~StopWatch() {
    }
  
  void StopWatch::Restart() {
    startTime = DateTime::Now();
    running = true;
    }

  Boolean StopWatch::Running() {
    return running;
    }

  void StopWatch::Start() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    startTime = DateTime::Now();
    startMilli = tv.tv_sec * 1000;
    startMilli += tv.tv_usec / 1000;
    running = true;
    }

  void StopWatch::Stop() {
    struct timeval tv;
    stopTime = DateTime::Now();
    gettimeofday(&tv, NULL);
    stopMilli = tv.tv_sec * 1000;
    stopMilli += tv.tv_usec / 1000;
    running = false;
    }

  TimeSpan StopWatch::Time() {
    if (running) return DateTime::Now().Subtract(&startTime);
    return stopTime.Subtract(&startTime);
    }

  UInt64 StopWatch::Seconds() {
    struct timeval tv;
    if (running) {
      stopTime = DateTime::Now();
      gettimeofday(&tv, NULL);
      stopMilli = tv.tv_sec * 1000;
      stopMilli += tv.tv_usec / 1000;
      }
    return (stopMilli - startMilli) / 1000;
    }

  UInt64 StopWatch::Milliseconds() {
    struct timeval tv;
    if (running) {
      stopTime = DateTime::Now();
      gettimeofday(&tv, NULL);
      stopMilli = tv.tv_sec * 1000;
      stopMilli += tv.tv_usec / 1000;
      }
    return stopMilli - startMilli;
    }

  }

