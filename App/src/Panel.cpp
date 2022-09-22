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
#include "SmrAppFramework.h"

namespace SmrFramework {

  Panel::Panel(Control* parent, int x, int y, int w, int h) :
         Control(parent, x, y, w, h) {
    this->objectType = (char*)"Panel";
    numControls = 0;
    controls = NULL;
    }

  Panel::~Panel() {
    int i;
    if (controls != NULL) {
      for (i=0; i<numControls; i++) {
        if (controls[i] != NULL) delete(controls[i]);
        }
      free(controls);
      }
    }

  void Panel::Add(Control* control) {
    numControls++;
    if (numControls == 1)
      controls = (Control**)malloc(sizeof(Control*));
    else
      controls = (Control**)realloc(controls,sizeof(Control*) * numControls);
    controls[numControls-1] = control;
    }

  void Panel::Remove(Control* control) {
    int i;
    int p;
    p = -1;
    for (i=0; i<numControls; i++)
      if (controls[i] == control) p = i;
    if (p < 0) return;
    if (controls[p] != NULL) delete(controls[p]);
    for (i=p; i<numControls-1; i++)
      controls[i] = controls[i+1];
    numControls--;
    if (numControls == 0) {
      free(controls);
      controls = NULL;
      }
    }

  int Panel::ProcessEvent(XEvent event) {
    int i;
    if (event.xany.window == window) {
      return Control::ProcessEvent(event);
      }
    for (i=0; i<numControls; i++)
      if (controls[i] != NULL)
        if (controls[i]->ProcessEvent(event)) return -1;
    return 0;
    }

  int Panel::IsContainer() {
    return -1;
    }

  void Panel::ProcessRadioButtons(RadioButton* trigger) {
    int i;
    RadioButton* rb;
    for (i=0; i<numControls; i++)
      if (controls[i] != NULL && controls[i]->IsRadioButton()) {
        rb = (RadioButton*)controls[i];
        if (rb != trigger) rb->Checked(false);
        }
    }

  } 

