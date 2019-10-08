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

  HScrollBar::HScrollBar(Control* parent, int x, int y, int w, int h) :
         ScrollBar(parent, x, y, w, h) {
    this->objectType = (char*)"HScrollBar";
    computeGraphics(width, height);
    computePuck();
    }

  HScrollBar::~HScrollBar() {
    }

  UInt32 HScrollBar::Height(UInt32 h) {
    Control::Height(h);
    computeGraphics(width, height);
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    return height;
    }

  void HScrollBar::Size(int h, int w) {
    Control::Height(h);
    Control::Width(w);
    computeGraphics(width, height);
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    }

  UInt32 HScrollBar::Width(UInt32 w) {
    Control::Width(w);
    computeGraphics(width, height);
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    return width;
    }

  void HScrollBar::MouseDownEvent(int x, int y, int button) {
    Control::MouseDownEvent(x, y, button);
    adjustPuck(x);
    }

  void HScrollBar::MouseUpEvent(int x, int y, int button) {
    Control::MouseUpEvent(x, y, button);
    puckGrabbed = 0;
    }

  void HScrollBar::MouseMoveEvent(int x, int y, int button) {
    Control::MouseMoveEvent(x, y, button);
    if (puckGrabbed) {
      puckPosition = x - low - mouseOffset;
      adjustPuck(0);
      }
    }

  void HScrollBar::Redraw() {
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
    XDrawRectangle(display, window, gc, 0, 0, height-1, height-1);
    XDrawRectangle(display, window, gc, width - height, 0, height-1, height-1);
    arrow[0].x = 2;          arrow[0].y = height / 2;
    arrow[1].x = height - 3; arrow[1].y = 2;
    arrow[2].x = height - 3; arrow[2].y = height - 3;
    XFillPolygon(display, window, gc, arrow, 3, Convex, CoordModeOrigin);
    arrow[0].x = width - 3; arrow[0].y = height / 2;
    arrow[1].x = width-height+3; arrow[1].y = height - 2;
    arrow[2].x = width-height+3; arrow[2].y = 2;
    XFillPolygon(display, window, gc, arrow, 3, Convex, CoordModeOrigin);
    XFillRectangle(display,window,gc,low+puckPosition,2,puckLength,height-4);
    XFreeGC(display, gc);
    XFlush(display);
    }

  } 

