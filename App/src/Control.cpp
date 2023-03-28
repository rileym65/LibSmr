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

  Control::Control(Control* parent, int x,int y, int w, int h) : Component() {
    Window               pwindow;
    XSetWindowAttributes attr;
    unsigned long        mask;
    this->objectType = (char*)"Control";
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
    this->window = -1;
    this->visible = false;
    this->enabled = true;
    this->borderWidth = 1;
    this->inputMask = 0;
    this->backgroundColor = Color::RGB(0xffff, 0xffff, 0xffff);
    this->foregroundColor = Color::RGB(0x0000, 0x0000, 0x0000);
    this->borderColor = Color::RGB(0x0000, 0x0000, 0x0000);
    this->mouseDownHandler = NULL;
    this->mouseUpHandler = NULL;
    this->mouseMoveHandler = NULL;
    this->keyDownHandler = NULL;
    this->keyUpHandler = NULL;
    this->paintHandler = NULL;
    this->customHandler = NULL;
    this->mouseEnterHandler = NULL;
    this->mouseLeaveHandler = NULL;
    this->resizedHandler = NULL;
    this->backgroundImage = NULL;
    this->backgroundPixmap = 0;
    this->disabledImage = NULL;
    this->disabledPixmap = 0;
    this->cursor = 0xffffffff;
    display = application->GetDisplay();
    pwindow = (parent != NULL) ? parent->GetWindow()
                               : DefaultRootWindow(display);
    attr.background_pixel = WhitePixel(display, DefaultScreen(display));
    attr.border_pixel = BlackPixel(display, DefaultScreen(display));
#ifdef USE_BACKING_STORE
    attr.backing_store = WhenMapped;
    mask = CWBackPixel | CWBorderPixel | CWBackingStore;
#else
    mask = CWBackPixel | CWBorderPixel;
#endif
    this->window = XCreateWindow(display, pwindow, x, y, w, h, 1,
                   CopyFromParent, CopyFromParent, CopyFromParent,
                   mask, &attr);
    inputMask = ExposureMask | ButtonPressMask | ButtonReleaseMask |
                ButtonMotionMask | KeyPressMask | KeyReleaseMask |
                StructureNotifyMask | FocusChangeMask | EnterWindowMask |
                LeaveWindowMask | PointerMotionMask;
    XSelectInput(display, window, inputMask);
    XGetWindowAttributes(display, window, &winattr);
    screen = DefaultScreen(display);
    if (this->parent != NULL && this->parent->Font() != NULL)
      font = this->parent->Font();
    else
      font = application->Font();

#ifdef USEXFT
    this->xftcolor.pixel = this->foregroundColor;
    this->xftcolor.color.red = 0x0000;
    this->xftcolor.color.green = 0x0000;
    this->xftcolor.color.blue = 0x0000;
    this->xftcolor.color.alpha = 0xffff;
    xftdrawable = XftDrawCreate(display, window, winattr.visual, winattr.colormap);
    if (xftdrawable == NULL) throw InvalidOpException(this, "Could not get drawable");

#endif
    }
  
  Control::~Control() {
    if (mouseDownHandler != NULL) delete(mouseDownHandler);
    if (mouseUpHandler != NULL) delete(mouseUpHandler);
    if (mouseMoveHandler != NULL) delete(mouseMoveHandler);
    if (keyDownHandler != NULL) delete(keyDownHandler);
    if (keyUpHandler != NULL) delete(keyUpHandler);
    if (paintHandler != NULL) delete(paintHandler);
//    if (backgroundImage != NULL) delete(backgroundImage);
//    if (disabledImage != NULL) delete(disabledImage);
    if (backgroundPixmap != 0) XFreePixmap(display, backgroundPixmap);
    if (disabledPixmap != 0) XFreePixmap(display, disabledPixmap);
    if (cursor != 0xffffffff) XFreeCursor(display, cursor);
#ifdef USEXFT
    if (xftdrawable != NULL) XftDrawDestroy(xftdrawable);
#endif
    XDestroyWindow(display, window);
    }

  UInt32 Control::BackgroundColor() {
    return this->backgroundColor;
    }
  
  UInt32 Control::BackgroundColor(u_int32_t pixel) {
    XSetWindowBackground(display, window, pixel);
    this->backgroundColor = pixel;
    XClearWindow(display, window);
    Redraw();
    XFlush(display);
    return this->backgroundColor;
    }

  UInt32 Control::BackgroundColor(int red, int green, int blue) {
    u_int32_t pixel;
    pixel = Color::RGB(red, green, blue);
    XSetWindowBackground(display, window, pixel);
    this->backgroundColor = pixel;
    XClearWindow(display, window);
    Redraw();
    XFlush(display);
    return this->backgroundColor;
    }

  Image* Control::BackgroundImage() {
    return backgroundImage;
    }

  Image* Control::BackgroundImage(Image* i) {
    Graphics* g;
    unsigned long mask;
    XSetWindowAttributes attr;
    backgroundImage = i;
    if (backgroundPixmap != 0)
      XFreePixmap(display, backgroundPixmap);
    backgroundPixmap = XCreatePixmap(display, window, width, height,
      application->ScreenDepth());
    g = GetGraphics();
    g->DrawImage(backgroundPixmap, 0, 0, backgroundImage);
    delete(g);
    mask = CWBackPixmap;
    attr.background_pixmap = backgroundPixmap;
    XChangeWindowAttributes(display, window, mask, &attr);
    XClearWindow(display, window);
    XFlush(display);
    return backgroundImage;
    }

  Image* Control::DisabledImage() {
    return disabledImage;
    }

  Image* Control::DisabledImage(Image* i) {
    Graphics* g;
    unsigned long mask;
    XSetWindowAttributes attr;
    disabledImage = i;
    if (disabledPixmap != 0)
      XFreePixmap(display, disabledPixmap);
    disabledPixmap = XCreatePixmap(display, window, width, height,
      application->ScreenDepth());
    g = GetGraphics();
    g->DrawImage(disabledPixmap, 0, 0, disabledImage);
    delete(g);
    if (!enabled) {
      mask = CWBackPixmap;
      attr.background_pixmap = disabledPixmap;
      XChangeWindowAttributes(display, window, mask, &attr);
      XClearWindow(display, window);
      XFlush(display);
      }
    return disabledImage;
    }

  void Control::UseParentBackground() {
    unsigned long mask;
    XSetWindowAttributes attr;
    mask = CWBackPixmap;
    attr.background_pixmap = ParentRelative;
    XChangeWindowAttributes(display, window, mask, &attr);
    XClearWindow(display, window);
    XFlush(display);
    }

  UInt32 Control::BorderColor() {
    return this->borderColor;
    }
  
  UInt32 Control::BorderColor(UInt32 pixel) {
    XSetWindowBorder(display, window, pixel);
    this->borderColor = pixel;
    XClearWindow(display, window);
    Redraw();
    XFlush(display);
    return this->borderColor;
    }

  UInt32 Control::BorderColor(UInt32 red, UInt32 green, UInt32 blue) {
    u_int32_t pixel;
    pixel = Color::RGB(red, green, blue);
    XSetWindowBorder(display, window, pixel);
    this->borderColor = pixel;
    XClearWindow(display, window);
    Redraw();
    XFlush(display);
    return this->borderColor;
    }

  UInt32 Control::BorderWidth() {
    return borderWidth;
    }
  
  UInt32 Control::BorderWidth(UInt32 w) {
    XSetWindowBorderWidth(display, window, w);
    this->borderWidth = w;
    XFlush(display);
    return borderWidth;
    }

  void     Control::BringToFront() {
    XRaiseWindow(display, window);
    XFlush(display);
    }

  Boolean  Control::Enabled() {
    return enabled;
    }

  Boolean  Control::Enabled(Boolean b) {
    XSetWindowAttributes attr;
    unsigned long mask;
    if (b != enabled) {
      enabled = b;
      if (enabled) {
        if (backgroundImage != NULL) {
          attr.background_pixmap = backgroundPixmap;
          mask = CWBackPixmap;
          XChangeWindowAttributes(display, window, mask, &attr);
          XClearWindow(display, window);
          }
        }
      else {
        if (disabledImage != NULL) {
          attr.background_pixmap = disabledPixmap;
          mask = CWBackPixmap;
          XChangeWindowAttributes(display, window, mask, &attr);
          XClearWindow(display, window);
          }
        }
      Redraw();
      }
    return enabled;
    }

  UInt32   Control::ForegroundColor() {
    return foregroundColor;
    }

  UInt32   Control::ForegroundColor(int red, int green, int blue) {
    u_int32_t pixel;
    pixel = Color::RGB(red, green, blue);
    this->foregroundColor = pixel;
#ifdef USEXFT
    xftcolor.pixel = pixel;
    xftcolor.color.red = red;
    xftcolor.color.blue = blue;
    xftcolor.color.green = green;
    xftcolor.color.alpha = 0xffff;
#endif
    Redraw();
    return foregroundColor;
    }
  
  UInt32   Control::ForegroundColor(UInt32 pixel) {
    this->foregroundColor = pixel;
    Redraw();
    return foregroundColor;
    }

  Graphics* Control::GetGraphics() {
    return new Graphics(this);
//    return new Graphics(this->display, this->window);
    }

  UInt32 Control::Height() {
    return height;
    }

  UInt32 Control::Height(UInt32 h) {
    this->height = h;
    XResizeWindow(display, window, width, height);
    XFlush(display);
    return this->height;
    }

  void Control::SendCustomEvent(void* data, Byte len) {
    int i;
    XEvent event;
    int status;
    if (len > 32) throw InvalidOpException(this, "Length exceeds 32");
    event.xclient.type = ClientMessage;
    event.xclient.window = window;
    event.xclient.serial = 0;
    event.xclient.send_event = true;
    event.xclient.format = 32;
    for (i=0; i<len; i++) event.xclient.data.b[i] = ((Byte*)data)[i];
    status = XSendEvent(display, window, true, 0, &event);
    if (status == 0) throw InvalidOpException(this, "Failed to send");
    }


  String   Control::Tag() {
    return tag;
    }

  String   Control::Tag(String s) {
    tag = s;
    return tag;
    }

  String   Control::Text() {
    return text;
    }

  String   Control::Text(const char* text) {
    this->text = String(text);
    Redraw();
    return this->text;
    }

  String Control::Text(String text) {
    this->text = text;
    Redraw();
    return this->text;
    }

  String Control::Text(String* text) {
    return Text(text->AsCharArray());
    }

  UInt32 Control::Width() {
    return width;
    }

  UInt32 Control::Width(UInt32 w) {
    this->width = w;
    XResizeWindow(display, window, width, height);
    XFlush(display);
    return this->width;
    }

  Boolean Control::Visible() {
    return visible;
    }

  Boolean Control::Visible(Boolean state) {
    this->visible = state;
    if (state) XMapWindow(display, window);
      else XUnmapWindow(display, window);
    this->Location(this->x, this->y);
    XFlush(display);
    return visible;
    }

  Window Control::GetWindow() {
    return window;
     }
  
  void Control::OnMouseDown(EventHandler* handler) {
    if (this->mouseDownHandler != NULL) delete(this->mouseDownHandler);
    this->mouseDownHandler = handler;
    }
  
  void Control::OnMouseEnter(EventHandler* handler) {
    if (this->mouseEnterHandler != NULL) delete(this->mouseEnterHandler);
    this->mouseEnterHandler = handler;
    }
  
  void Control::OnMouseLeave(EventHandler* handler) {
    if (this->mouseLeaveHandler != NULL) delete(this->mouseLeaveHandler);
    this->mouseLeaveHandler = handler;
    }

  void Control::OnMouseMove(EventHandler* handler) {
    if (this->mouseMoveHandler != NULL) delete(this->mouseMoveHandler);
    this->mouseMoveHandler = handler;
    }
  
  void Control::OnMouseUp(EventHandler* handler) {
    if (this->mouseUpHandler != NULL) delete(this->mouseUpHandler);
    this->mouseUpHandler = handler;
    }
  
  void Control::OnKeyDown(EventHandler* handler) {
    if (this->keyDownHandler != NULL) delete(this->keyDownHandler);
    this->keyDownHandler = handler;
    }
  
  void Control::OnKeyUp(EventHandler* handler) {
    if (this->keyUpHandler != NULL) delete(this->keyUpHandler);
    this->keyUpHandler = handler;
    }
  
  void Control::OnPaint(EventHandler* handler) {
    if (this->paintHandler != NULL) delete(this->paintHandler);
    this->paintHandler = handler;
    }
  
  void Control::OnCustom(EventHandler* handler) {
    if (this->customHandler != NULL) delete(this->customHandler);
    this->customHandler = handler;
    }
  
  void Control::OnResize(EventHandler* handler) {
    if (this->resizedHandler != NULL) delete(this->resizedHandler);
    this->resizedHandler = handler;
    }

  int Control::ProcessEvent(XEvent event) {
    KeySym         key;
    XComposeStatus status;
    char           tmp[256];
    if (event.xany.window == window) {
      if (event.type == MappingNotify)
        XRefreshKeyboardMapping((XMappingEvent*) &event);
      if (enabled) {
        if (event.type == ButtonPress) 
          MouseDownEvent(event.xbutton.x, event.xbutton.y, event.xbutton.button);
        if (event.type == ButtonRelease) 
          MouseUpEvent(event.xbutton.x, event.xbutton.y, event.xbutton.button);
        if (event.type == MotionNotify) 
          MouseMoveEvent(event.xmotion.x, event.xmotion.y, event.xmotion.state);
        if (event.type == EnterNotify) 
          EnterEvent(event.xcrossing.x, event.xcrossing.y);
        if (event.type == LeaveNotify) 
          LeaveEvent(event.xcrossing.x, event.xcrossing.y);
        if (event.type == KeyPress) {
          XLookupString((XKeyEvent*)&event, tmp, 10, &key, &status);
          KeyDownEvent(key, status);
          }
        if (event.type == KeyRelease) {
          XLookupString((XKeyEvent*)&event, tmp, 10, &key, &status);
          KeyUpEvent(key, status);
          }
        if (event.type ==  FocusIn) {
          FocusInEvent();
          }
        if (event.type ==  FocusOut) {
          FocusOutEvent();
          }
        }
      if (event.type == Expose) {
        ExposeEvent(event.xexpose.x, event.xexpose.y,
                    event.xexpose.width, event.xexpose.height,
                    event.xexpose.count);
        }
      if (event.type == ConfigureNotify)
        ConfigureEvent(event.xconfigure.x,
                       event.xconfigure.y,
                       event.xconfigure.width,
                       event.xconfigure.height);
      if (event.type == ClientMessage) {
        if (customHandler != NULL) {
          customHandler->Call(this, CustomEventArgs((Byte*)event.xclient.data.b,20));
          }
        }
      return -1;
      }
    return 0;
    }

  void   Control::Redraw() {
    }

  TextFont*  Control::Font() {
    return font;
    }

  TextFont*  Control::Font(TextFont* f) {
    font = f;
    Redraw();
    return font;
    }

  void   Control::Location(int x, int y) {
    this->x = x;
    this->y = y;
    XMoveWindow(display, window, x, y);
    XFlush(display);
    }

  void     Control::SendToBack() {
    XLowerWindow(display, window);
    XFlush(display);
    }

  void   Control::Size(int w, int h) {
    this->width = w;
    this->height = h;
    XResizeWindow(display, window, width, height);
    XFlush(display);
    }

  void Control::MouseDownEvent(int x, int y, int button) {
    if (this->mouseDownHandler != NULL) {
      mouseDownHandler->Call(this, MouseEventArgs(x, y, button, 0));
      }
    }

  void Control::MouseUpEvent(int x, int y, int button) {
    if (this->mouseUpHandler != NULL) {
      mouseUpHandler->Call(this, MouseEventArgs(x, y, button, 0));
      }
    }

  void Control::MouseMoveEvent(int x, int y, int state) {
    if (this->mouseMoveHandler != NULL) {
      mouseMoveHandler->Call(this, MouseEventArgs(x, y, 0, state));
      }
    }

  void Control::KeyDownEvent(KeySym key, XComposeStatus status) {
    if (this->keyDownHandler != NULL) {
      keyDownHandler->Call(this, KeyEventArgs((int)key));
      }
    }

  void Control::KeyUpEvent(KeySym key, XComposeStatus status) {
    if (this->keyUpHandler != NULL) {
      keyUpHandler->Call(this, KeyEventArgs((int)key));
      }
    }

  void Control::EnterEvent(int x, int y) {
    if (this->mouseEnterHandler != NULL) {
      mouseEnterHandler->Call(this, MouseEventArgs(x, y, 0, 0));
      }
    }

  void Control::LeaveEvent(int x, int y) {
    if (this->mouseLeaveHandler != NULL) {
      mouseLeaveHandler->Call(this, MouseEventArgs(x, y, 0, 0));
      }
    }
 
  void Control::ExposeEvent(int x,int y, int w, int h, int count) {
    Redraw();
    if (this->paintHandler != NULL) {
      paintHandler->Call(this, PaintEventArgs(x, y, w, h, count));
      }
    }

  void Control::ConfigureEvent(int x,int y, int w, int h) {
    Boolean resized;
    resized = ((UInt32)w != width || (UInt32)h != height) ? true : false;
    width = w;
    height = h;
    if (resized && this->resizedHandler != NULL) {
      resizedHandler->Call(this, ResizeEventArgs(w, h));
      }
    }

  void Control::FocusInEvent() {
    }

  void Control::FocusOutEvent() {
    }

  UInt32 Control::X() {
    return x;
    }

  UInt32 Control::X(UInt32 i) {
    x = i;
    Location(x, y);
    return x;
    }

  UInt32 Control::Y() {
    return y;
    }

  UInt32 Control::Y(UInt32 i) {
    y = i;
    Location(x, y);
    return y;
    }

  Display* Control::_Display() {
    return display;
    }

  Window Control::_Window() {
    return window;
    }

  void Control::LostFocus() {
    }

#ifdef USEXFT

  XftDraw* Control::_XftDrawable() {
    return xftdrawable;
    }

  XftColor Control::_XftColor() {
    return xftcolor;
    }

#endif

  }

