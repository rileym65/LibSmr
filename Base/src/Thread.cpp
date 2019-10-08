/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <pthread.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Thread::Thread() {
    terminate = false;
    objectType = (char*)"Thread";
    }

  void* Thread::arg() {
    return _arg;
    }

  void Thread::arg(void* a) {
    _arg = a;
    }
  
  Int32 Thread::run(void* a) {
    setup();
    execute(a);
    return 0;
    }

  void* Thread::entryPoint(void* pthis) {
    Thread *pt = (Thread*)pthis;
    pt->run( pt->arg() );
    return 0;
    }

  void Thread::setup() {
    }

  void Thread::execute(void* arg) {
    }

  Int32 Thread::Start(void* a) {
    Int32 code;
    arg(a);
    code = pthread_create(&threadId, NULL, Thread::entryPoint, this);
    return code;
    }

  void Thread::Terminate() {
    terminate = true;
    }

  pthread_t Thread::ThreadId() {
    return threadId;
    }

  }

