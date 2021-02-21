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

  Button::Button(Control* parent, int x, int y, int w, int h) :
         Control(parent, x, y, w, h) {
    pressHandler = NULL;
    releaseHandler = NULL;
    this->pressed = 0;
    this->pressedColor = Color::RGB(0x8000, 0x8000, 0x8000);
    this->objectType = (char*)"Button";
    this->pressedImage = NULL;
    this->pressedPixmap = 0;
    }

  Button::~Button() {
    if (pressHandler != NULL) delete(pressHandler);
    if (releaseHandler != NULL) delete(releaseHandler);
    if (pressedPixmap != 0) XFreePixmap(display, pressedPixmap);
    }

  int Button::IsPressed() {
    return pressed;
    }

  void Button::SetPressedColor(int red, int green, int blue) {
    u_int32_t pixel;
    pixel = Color::RGB(red, green, blue);
    this->pressedColor = pixel;
    }

  void Button::OnPress(EventHandler* handler) {
    if (pressHandler != NULL) delete(pressHandler);
    pressHandler = handler;
    }

  void Button::OnRelease(EventHandler* handler) {
    if (releaseHandler != NULL) delete(releaseHandler);
    releaseHandler = handler;
    }
    
  void Button::MouseDownEvent(int x, int y, int button) {
    XSetWindowAttributes attr;
    unsigned long mask;
    pressed = -1;
    if (pressedImage != NULL) {
      mask = CWBackPixmap;
      attr.background_pixmap = pressedPixmap;
      XChangeWindowAttributes(display, window, mask, &attr);
      XClearWindow(display, window);
      }
    Redraw();
    XFlush(display);
    Control::MouseDownEvent(x, y, button);
    if (this->pressHandler != NULL) {
      pressHandler->Call(this, MouseEventArgs(x, y, button, 0));
      }
    }
    
  void Button::MouseUpEvent(int x, int y, int button) {
    XSetWindowAttributes attr;
    unsigned long mask;
    pressed = 0;
    if (backgroundImage != NULL) {
      mask = CWBackPixmap;
      attr.background_pixmap = backgroundPixmap;
      XChangeWindowAttributes(display, window, mask, &attr);
      XClearWindow(display, window);
      }
    Redraw();
    XFlush(display);
    Control::MouseUpEvent(x, y, button);
    if (this->releaseHandler != NULL) {
      releaseHandler->Call(this, MouseEventArgs(x, y, button, 0));
      }
    }

  Image* Button::PressedImage() {
    return pressedImage;
    }

  Image* Button::PressedImage(Image* i) {
    Graphics* g;
    pressedImage = i;
    if (i != NULL) {
      pressedPixmap = XCreatePixmap(display, window, width, height,
         application->ScreenDepth());
      g = GetGraphics();
      g->DrawImage(pressedPixmap, 0, 0, pressedImage);
      delete(g);
      }
    return pressedImage;
    }

  void Button::Redraw() {
    GC            gc;
    XGCValues     values;
#ifndef USEXFT
    XFontStruct*  font;
#endif
    unsigned long mask;
    int           xoffset;
    int           yoffset;
    Graphics*     gr;
    if (!visible) return;
    if (backgroundImage == NULL) {
      XSetWindowBackground(display, window, (pressed)?pressedColor:backgroundColor);
      XClearWindow(display, window);
      }
#ifndef USEXFT
    if (this->font.Length() == 0) font = XLoadQueryFont(display, "fixed");
      else font = XLoadQueryFont(display, this->font.AsCharArray());
    if (font == NULL) font = XLoadQueryFont(display, "fixed");
#endif

    if (backgroundImage == NULL) {
      gr = GetGraphics();
      gr->ForegroundColor((pressed) ? Color::DarkGray() : Color::LightGray());
      gr->DrawLine(0,0,width,0);
      gr->DrawLine(0,1,width,1);
      gr->DrawLine(0,2,width,2);
      gr->DrawLine(0,0,0,height);
      gr->DrawLine(1,0,1,height);
      gr->DrawLine(2,0,2,height);
      gr->ForegroundColor((pressed) ? Color::LightGray() : Color::DarkGray());
      gr->DrawLine(width-1, 1, width-1, height);
      gr->DrawLine(width-2, 2, width-2, height);
      gr->DrawLine(width-3, 3, width-3, height);
      gr->DrawLine(1, height-1, width, height-1);
      gr->DrawLine(2, height-2, width, height-2);
      gr->DrawLine(3, height-3, width, height-3);
      delete(gr);
      }
    
#ifdef USEXFT
    XftTextExtents8(display, xftfont, (const FcChar8*)text.AsCharArray(), text.Length(), &ginfo);
    yoffset = height / 2 - ginfo.height / 2;
    xoffset = width / 2 - ginfo.width / 2;
    XftDrawString8(xftdrawable, &xftcolor, xftfont, ginfo.x+xoffset,ginfo.y+yoffset,
                  (const FcChar8*)text.AsCharArray(),text.Length());
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = (pressed) ? pressedColor : backgroundColor;
    mask = GCLineWidth | GCForeground | GCBackground;
    gc = XCreateGC(display, window, mask, &values);
#else
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = (pressed) ? pressedColor : backgroundColor;
    values.font = font->fid;
    mask = GCLineWidth | GCForeground | GCBackground | GCFont;
    gc = XCreateGC(display, window, mask, &values);
    yoffset = (height - (font->ascent + font->descent)) / 2 + font->descent;
    xoffset = (width - XTextWidth(font, text.AsCharArray(), text.Length())) / 2;
    XDrawImageString(display,window,gc, xoffset,height-yoffset,text.AsCharArray(),text.Length());
#endif
    XFreeGC(display, gc);
#ifndef USEXFT
    XFreeFont(display, font);
#endif
    }

  Boolean Button::Visible() {
    return Control::Visible();
    }

  Boolean Button::Visible(Boolean state) {
    this->pressed = 0;
    return Control::Visible(state);
    }
 
  Boolean Button::Pressed() {
    return (pressed == 0) ? false : true;
    }

  Boolean Button::Pressed(Boolean b) {
    XSetWindowAttributes attr;
    unsigned long mask;
    if (b) {
      pressed = -1;
      if (pressedImage != NULL) {
        mask = CWBackPixmap;
        attr.background_pixmap = pressedPixmap;
        XChangeWindowAttributes(display, window, mask, &attr);
        XClearWindow(display, window);
        }
      }
    else {
      pressed = 0;
      if (backgroundImage != NULL) {
        mask = CWBackPixmap;
        attr.background_pixmap = backgroundPixmap;
        XChangeWindowAttributes(display, window, mask, &attr);
        XClearWindow(display, window);
        }
      }
    Redraw();
    return (pressed == 0) ? false : true;
    }
  } 

