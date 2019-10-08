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
#include <string.h>
#include "SmrAppFramework.h"

namespace SmrFramework {

  VScrollBar::VScrollBar(Control* parent, int x, int y, int w, int h) :
         ScrollBar(parent, x, y, w, h) {
    this->objectType = (char*)"VScrollBar";
    computeGraphics(height, width);
    computePuck();
    }

  VScrollBar::~VScrollBar() {
    }

  UInt32 VScrollBar::Height(UInt32 h) {
    Control::Height(h);
    computeGraphics(height, width);
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    return height;
    }

  void VScrollBar::Size(int h, int w) {
    Control::Height(h);
    Control::Width(w);
    computeGraphics(height, width);
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    }

  UInt32 VScrollBar::Width(UInt32 w) {
    Control::Width(w);
    computeGraphics(height, width);
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    return width;
    }

  void VScrollBar::MouseDownEvent(int x, int y, int button) {
    Control::MouseDownEvent(x, y, button);
    adjustPuck(y);
    }

  void VScrollBar::MouseUpEvent(int x, int y, int button) {
    Control::MouseUpEvent(x, y, button);
    puckGrabbed = 0;
    }

  void VScrollBar::MouseMoveEvent(int x, int y, int button) {
    Control::MouseMoveEvent(x, y, button);
    if (puckGrabbed) {
      puckPosition = y - low - mouseOffset;
      adjustPuck(0);
      }
    }

  void VScrollBar::Redraw() {
    GC            gc;
    XGCValues     values;
    unsigned long mask;
    XPoint        arrow[3];
    if (!visible) return;
    XSetWindowBackground(display, window, backgroundColor);
    XClearWindow(display, window);
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = backgroundColor;
    mask = GCLineWidth | GCForeground | GCBackground;
    gc = XCreateGC(display, window, mask, &values);
    XDrawRectangle(display, window, gc, 0, 0, width-1, height-1);
    XDrawRectangle(display, window, gc, 0, 0, width-1, width-1);
    XDrawRectangle(display, window, gc, 0, height - width, width-1, width-1);
    arrow[0].x = width / 2; arrow[0].y = 2;
    arrow[1].x = width - 3; arrow[1].y = width - 3;
    arrow[2].x = 2;         arrow[2].y = width - 3;
    XFillPolygon(display, window, gc, arrow, 3, Convex, CoordModeOrigin);
    arrow[0].x = width / 2; arrow[0].y = height - 3;
    arrow[1].x = 2;         arrow[1].y = height - width + 3;
    arrow[2].x = width - 3; arrow[2].y = height - width + 3;
    XFillPolygon(display, window, gc, arrow, 3, Convex, CoordModeOrigin);
    XFillRectangle(display,window,gc,2,low+puckPosition,width-4,puckLength);
    XFreeGC(display, gc);
    XFlush(display);
    }

  } 

