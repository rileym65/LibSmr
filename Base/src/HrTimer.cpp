/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <sys/time.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  HrTimer::HrTimer() {
    running = false;
    seconds = 0;
    microSeconds = 0;
    objectType = (char*)"HrTimer";
    }
  
  HrTimer::~HrTimer() {
    }

  UInt64 HrTimer::Microseconds() {
    return (UInt64)(ToDouble() * 1000000);
    }

  UInt64 HrTimer::Milliseconds() {
    return (UInt64)(ToDouble() * 1000);
    }
  
  void HrTimer::Restart() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    seconds = tv.tv_sec;
    microSeconds = tv.tv_usec;
    running = true;
    }

  Boolean HrTimer::Running() {
    return running;
    }

  UInt64 HrTimer::Seconds() {
    return (UInt64)ToDouble();
    }

  void HrTimer::Start() {
    struct timeval tv;
    if (seconds == 0 && microSeconds == 0) {
      gettimeofday(&tv, NULL);
      seconds = tv.tv_sec;
      microSeconds = tv.tv_usec;
      }
    running = true;
    }

  void HrTimer::Stop() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    lapSeconds = tv.tv_sec;
    lapMicroSeconds = tv.tv_usec;
    running = false;
    }

  Double HrTimer::ToDouble() {
    Double a,b;
    struct timeval tv;
    if (running) {
      gettimeofday(&tv, NULL);
      lapSeconds = tv.tv_sec;
      lapMicroSeconds = tv.tv_usec;
      }
    a = seconds + (microSeconds / 1000000.0);
    b = lapSeconds + (lapMicroSeconds / 1000000.0);
    return (b - a);
    }

  }

