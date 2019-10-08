/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "SmrAppFramework.h"

namespace SmrFramework {

  Timer::Timer() : Object() {
    this->objectType = (char*)"Timer";
    this->running = false;
    this->interval = 100;
    this->tickHandler = NULL;
    this->count = 0;
    this->stopOnTick = false;
    this->millis = 0;
    application->AddTimer(this);
    }

  Timer::~Timer() {
    application->RemoveTimer(this);
    if (tickHandler != NULL) delete(tickHandler);
    }

  UInt32 Timer::Count() {
    return count;
    }

  UInt32 Timer::Interval() {
    return interval;
    }

  UInt32 Timer::Interval(UInt32 ms) {
    interval = ms;
    return interval;
    }

  void Timer::OnTick(EventHandler* h) {
    if (tickHandler != NULL) delete(tickHandler);
    tickHandler = h;
    }

  Boolean Timer::Running() {
    return running;
    }

  void Timer::Start() {
    running = true;
    gettimeofday(&lastTime,NULL);
    millis = 0;
    }

  void Timer::Stop() {
    running = false;
    }

  Boolean Timer::StopOnTick() {
    return stopOnTick;
    }

  Boolean Timer::StopOnTick(Boolean b) {
    stopOnTick = b;
    return stopOnTick;
    }

  void Timer::Tick() {
    struct timeval t;
    struct timeval td;
    UInt32 tm;
    if (!running) return;
      gettimeofday(&t, NULL);
      td = t;
      t.tv_sec -= lastTime.tv_sec;
      tm = t.tv_sec * 1000 + (t.tv_usec / 1000);
      tm -= (lastTime.tv_usec / 1000);
      if (tm != 0) lastTime = td;
      millis += tm;
      while (millis >= interval) {
        millis -= interval;
        count++;
        if (stopOnTick) running = false;
        if (tickHandler != NULL)
          tickHandler->Call(this, EventArgs());
        }
    }

  } 

