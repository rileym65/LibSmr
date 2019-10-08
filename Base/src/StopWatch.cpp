/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


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
    startTime = DateTime::Now();
    running = true;
    }

  void StopWatch::Stop() {
    stopTime = DateTime::Now();
    running = false;
    }

  TimeSpan StopWatch::Time() {
    if (running) return DateTime::Now().Subtract(&startTime);
    return stopTime.Subtract(&startTime);
    }

  }

