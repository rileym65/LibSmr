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

  Spinner::Spinner(Control* parent, int x, int y, int w, int h) :
         Control(parent, x, y, w, h) {
    valueChangedHandler = NULL;
    this->minValue = 1;
    this->maxValue = 10;
    this->value = 1;
    this->step = 1;
    this->objectType = (char*)"Spinner";
    }

  Spinner::~Spinner() {
    if (valueChangedHandler != NULL) delete(valueChangedHandler);
    }

  Int32 Spinner::MaxValue() {
    return maxValue;
    }

  Int32 Spinner::MaxValue(Int32 i) {
    maxValue = i;
    if (value > maxValue) Value(maxValue);
    return maxValue;
    }

  Int32 Spinner::MinValue() {
    return minValue;
    }

  Int32 Spinner::MinValue(Int32 i) {
    minValue = i;
    if (value < minValue) Value(minValue);
    return minValue;
    }

  Int32 Spinner::Step() {
    return step;
    }

  Int32 Spinner::Step(Int32 i) {
    step = i;
    value = (((value-minValue) / step) * step)+minValue;
    Value(value);
    return step;
    }

  Int32 Spinner::Value() {
    return value;
    }

  Int32 Spinner::Value(Int32 i) {
    value = i;
    if (value < minValue) value = minValue;
    if (value > maxValue) value = maxValue;
    value = (((value-minValue) / step) * step)+minValue;
    if (valueChangedHandler != NULL) {
      valueChangedHandler->Call(this, ValueEventArgs(value));
      }
    Redraw();
    return value;
    }

  void Spinner::OnValueChanged(EventHandler* handler) {
    if (valueChangedHandler != NULL) delete(valueChangedHandler);
    valueChangedHandler = handler;
    }

  void Spinner::MouseUpEvent(int x, int y, int button) {
    int controlLine;
    Control::MouseUpEvent(x, y, button);
    controlLine = height / 2;
    if (controlLine < 20) controlLine = 20;
    controlLine = width - controlLine;
    if (x > controlLine) {
      if ((UInt32)y < height/2) {
        Value(value + step);
        }
      if ((UInt32)y > height/2) {
        Value(value - step);
        }
      }
    }

  void Spinner::Redraw() {
    GC            gc;
    XGCValues     values;
#ifndef USEXFT
    XFontStruct*  font;
#endif
    unsigned long mask;
    int           xoffset;
    int           yoffset;
    char          buffer[20];
    int           controlLine;
    XPoint        arrow[3];
    if (!visible) return;
    XSetWindowBackground(display, window, backgroundColor);
    XClearWindow(display, window);
    sprintf(buffer,"%d",value);
    controlLine = height / 2;
    if (controlLine < 20) controlLine = 20;
    controlLine = width - controlLine;
#ifdef USEXFT
    XftTextExtents8(display, xftfont, (const FcChar8*)buffer, strlen(buffer), &ginfo);
    yoffset = height / 2 - ginfo.height / 2;
    xoffset = (width - height) / 2 - ginfo.width / 2;
    XftDrawString8(xftdrawable, &xftcolor, xftfont, ginfo.x+xoffset,ginfo.y+yoffset,
                  (const FcChar8*)buffer,strlen(buffer));
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = backgroundColor;
    mask = GCLineWidth | GCForeground | GCBackground;
    gc = XCreateGC(display, window, mask, &values);
#else
    if (this->font.Length() == 0) font = XLoadQueryFont(display, "fixed");
      else font = XLoadQueryFont(display, this->font.AsCharArray());
    if (font == NULL) font = XLoadQueryFont(display, "fixed");
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = backgroundColor;
    values.font = font->fid;
    mask = GCLineWidth | GCForeground | GCBackground | GCFont;
    gc = XCreateGC(display, window, mask, &values);
    yoffset = (height - (font->ascent + font->descent)) / 2 + font->descent;
    xoffset = (width-controlLine-XTextWidth(font, buffer, strlen(buffer))) / 2;
    XDrawImageString(display,window,gc, xoffset,height-yoffset,
                     buffer,strlen(buffer));
#endif
    XDrawLine(display, window, gc, controlLine, 0, controlLine , height);
    XDrawLine(display, window, gc, controlLine, height / 2, width, height / 2);
    arrow[0].x = (width-controlLine)/2+controlLine+1; arrow[0].y = 2;
    arrow[1].x = width - 2;      arrow[1].y = height/2 - 2;
    arrow[2].x = controlLine+2;  arrow[2].y = height/2 - 2;
    XFillPolygon(display, window, gc, arrow, 3, Convex, CoordModeOrigin);
    arrow[0].x = (width-controlLine)/2+controlLine+1; arrow[0].y = height - 3;
    arrow[1].x = controlLine+2; arrow[1].y = height/2 + 3;
    arrow[2].x = width-2;       arrow[2].y = height/2 + 3;
    XFillPolygon(display, window, gc, arrow, 3, Convex, CoordModeOrigin);
    XFreeGC(display, gc);
#ifndef USEXFT
    XFreeFont(display, font);
#endif
    }

  } 

