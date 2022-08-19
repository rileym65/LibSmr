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
#include <unistd.h>
#include "SmrAppFramework.h"

namespace SmrFramework {

Application* application;

Application::Application() {
  char *displayname;
  this->objectType = (char*)"Application";
  displayname = getenv("DISPLAY");
  focus = NULL;
  if (displayname != NULL) display = XOpenDisplay(displayname);
    else display = XOpenDisplay(":0.0");
  XGetWindowAttributes(display, RootWindow(display, DefaultScreen(display)), &screenAttributes);
  getColorMapping();
  forms = NULL;
  numForms = 0;
  application = this;
  timers = NULL;
  numTimers = 0;
  loopDelay = 1000;
  dialogs = NULL;
  numDialogs = 0;
  }

Application::~Application() {
  UInt32 i;
  if (forms != NULL) {
    for (i=0; i< numForms; i++) {
      delete(forms[i]);
      }
    free(forms);
    }
  if (dialogs != NULL) {
    for (i=0; i< numDialogs; i++) {
      delete(dialogs[i]);
      }
    free(dialogs);
    }
  if (timers != NULL) {
    for (i=0; i<numTimers; i++)
      delete(timers[i]);
    free(timers);
    }
//    XCloseDisplay(display);
  }

void Application::getColorMapping() {
  Visual* visual;
  visual = DefaultVisual(display, DefaultScreen(display));
  red_mask = visual->red_mask;
  green_mask = visual->green_mask;
  blue_mask = visual->blue_mask;
  red_shift = 0;
  while (red_mask >= 0x10000) {
    red_mask >>= 1;
    red_shift--;
    }
  while (red_mask < 0x8000) {
    red_mask <<= 1;
    red_shift++;
    }
  green_shift = 0;
  while (green_mask >= 0x10000) {
    green_mask >>= 1;
    green_shift--;
    }
  while (green_mask < 0x8000) {
    green_mask <<= 1;
    green_shift++;
    }
  blue_shift = 0;
  while (blue_mask >= 0x10000) {
    blue_mask >>= 1;
    blue_shift--;
    }
  while (blue_mask < 0x8000) {
    blue_mask <<= 1;
    blue_shift++;
    }
  }

void Application::AddDialog(Dialog* d) {
  numDialogs++;
  if (numDialogs == 1)
    dialogs = (Dialog**)malloc(sizeof(Dialog*));
  else
    dialogs = (Dialog**)realloc(dialogs,sizeof(Dialog*) * numDialogs);
  dialogs[numDialogs - 1] = d;
  }

void Application::AddForm(Form* form) {
  numForms++;
  if (numForms == 1)
    forms = (Form**)malloc(sizeof(Form*));
  else
    forms = (Form**)realloc(forms,sizeof(Form*) * numForms);
  forms[numForms - 1] = form;
  }

void Application::AddTimer(Timer* t) {
  numTimers++;
  if (numTimers == 1)
    timers = (Timer**)malloc(sizeof(Timer*));
  else
    timers = (Timer**)realloc(timers,sizeof(Timer*) * numTimers);
  timers[numTimers-1] = t;
  }

void Application::RemoveTimer(Timer* t) {
  UInt32 i;
  Int32 p;
  p = -1;
  for (i=0; i<numTimers; i++)
    if (timers[i] == t) p = i;
  if (p < 0) return;
  for (i=p; i<numTimers-1; i++)
    timers[i] = timers[i+1];
  numTimers--;
  if (numTimers == 0) {
    free(timers);
    timers = NULL;
    }
  else {
    timers = (Timer**)realloc(timers,sizeof(Timer*) * numTimers);
    }
  }

void Application::RemoveDialog(Dialog* d) {
  UInt32 i;
  Int32 p;
  p = -1;
  for (i=0; i<numDialogs; i++)
    if (dialogs[i] == d) p = i;
  if (p < 0) return;
  for (i=p; i<numDialogs-1; i++)
    dialogs[i] = dialogs[i+1];
  numDialogs--;
  if (numDialogs == 0) {
    free(dialogs);
    dialogs = NULL;
    }
  else {
    dialogs = (Dialog**)realloc(dialogs,sizeof(Dialog*) * numDialogs);
    }
  }

void Application::RemoveForm(Form* f) {
  UInt32 i;
  Int32 p;
  p = -1;
  for (i=0; i<numForms; i++)
    if (forms[i] == f) p = i;
  if (p < 0) return;
  for (i=p; i<numForms-1; i++)
    forms[i] = forms[i+1];
  numForms--;
  if (numForms == 0) {
    free(forms);
    forms = NULL;
    }
  else {
    forms = (Form**)realloc(forms,sizeof(Form*) * numForms);
    }
  }

void Application::Init() {
  }

Control* Application::Focus() {
  return focus;
  }

Control* Application::Focus(Control* c) {
  Control* current;
  current = focus;
  focus = c;
  if (current != NULL && current != c) current->LostFocus();
  return focus;
  }

UInt32 Application::LoopDelay() {
  return loopDelay;
  }

UInt32 Application::LoopDelay(UInt32 i) {
  loopDelay = i;
  return loopDelay;
  }

void Application::DoEvents() {
  UInt32 i;
  XEvent event;
    if (numTimers > 0) {
      for (i=0; i<numTimers; i++) timers[i]->Tick();
      }
  while (XPending(display) > 0) {
    if (XNextEvent(display, &event) >= 0) {
      if (focus != NULL && event.type == KeyPress) {
        event.xany.window = focus->GetWindow();
        focus->ProcessEvent(event);
        }
      else if (focus != NULL && event.type == KeyRelease) {
        event.xany.window = focus->GetWindow();
        focus->ProcessEvent(event);
        }
      else if (numDialogs > 0 && event.type != Expose) {
        dialogs[numDialogs-1]->ProcessEvent(event);
        }
      else if (event.type == Expose) {
        for (i=0; i<numDialogs; i++)
          if (dialogs[i] != NULL) {
            if (dialogs[i]->ProcessEvent(event)) i = numDialogs;
            }
        for (i=0; i<numForms; i++)
          if (forms[i] != NULL) {
            if (forms[i]->ProcessEvent(event)) i = numForms;
            }
        }
      else {
        for (i=0; i<numForms; i++)
          if (forms[i] != NULL) {
            if (forms[i]->ProcessEvent(event)) i = numForms;
            }
        }
      }
    }
  }

void Application::Run() {
  terminate = 0;
  while (terminate == 0) {
    DoEvents();
    usleep(loopDelay);
    }
  }

void Application::Stop() {
  terminate = -1;
  }

Display *Application::GetDisplay() {
  return display;
  }

int Application::ScreenWidth() {
  return screenAttributes.width;
  }

int Application::ScreenHeight() {
  return screenAttributes.height;
  }

int Application::ScreenDepth() {
  return screenAttributes.depth;
  }

}

