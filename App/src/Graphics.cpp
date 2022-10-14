/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include <string.h>
#include "SmrAppFramework.h"

namespace SmrFramework {

//  Graphics::Graphics(Display *display, Window window) {
  Graphics::Graphics(Control *ctrl) {
    XGCValues values;
    unsigned long mask;
    this->control = ctrl;
    this->display = ctrl->_Display();
    this->window = ctrl->_Window();
    this->foregroundColor = Color::Black();
    font = XLoadQueryFont(this->display, "fixed");
    values.foreground = Color::Black();
    values.background = Color::White();
    values.line_width = 1;
    values.line_style = LineSolid;
    values.cap_style = CapNotLast;
    values.join_style = JoinMiter;
    values.font = font->fid;
    mask = GCLineWidth | GCForeground | GCBackground | GCCapStyle | GCJoinStyle |
           GCLineStyle | GCFont;
    this->gc = XCreateGC(display, window, mask, &values);
    lineWidth = 1;
    lineStyle = LineSolid;
    capStyle = CapNotLast;
    joinStyle = JoinMiter;
    this->foregroundColor = Color::Black();
    this->backgroundColor = Color::White();
#ifdef USEXFT
    xftfont = control->Font()->FontObject();
    xftdrawable = control->_XftDrawable();
    xftcolor = control->_XftColor();
    ginfo = control->Font()->GlyphInfo();
#endif
    }

  Graphics::~Graphics() {
    XFreeGC(display, gc);
    XFreeFont(display, font);
    XFlush(this->display);
    }

  UInt32 Graphics::BackgroundColor() {
    return backgroundColor;
    }

  UInt32 Graphics::BackgroundColor(int r, int g, int b) {
    backgroundColor = Color::RGB(r, g, b);
    XSetBackground(this->display, this->gc, backgroundColor);
    return backgroundColor;
    }

  UInt32 Graphics::BackgroundColor(UInt32 pixel) {
    backgroundColor = pixel;
    XSetBackground(this->display, this->gc, backgroundColor);
    return backgroundColor;
    }

  UInt32 Graphics::BackgroundColor(Color* color) {
    backgroundColor = color->Pixel();
    XSetBackground(this->display, this->gc, backgroundColor);
    return backgroundColor;
    }

  UInt32 Graphics::CapStyle() {
    return capStyle;
    }

  UInt32 Graphics::CapStyle(UInt32 w) {
    capStyle = w;
    XSetLineAttributes(this->display, this->gc, this->lineWidth, this->lineStyle,
                       this->capStyle, this->joinStyle);
    return capStyle;
    }

  UInt32 Graphics::ForegroundColor() {
    return foregroundColor;
    }

  UInt32 Graphics::ForegroundColor(int r, int g, int b) {
    foregroundColor = Color::RGB(r, g, b);
    XSetForeground(this->display, this->gc, foregroundColor);
    return foregroundColor;
    }

  UInt32 Graphics::ForegroundColor(UInt32 pixel) {
    foregroundColor = pixel;
    XSetForeground(this->display, this->gc, foregroundColor);
    return foregroundColor;
    }

  UInt32 Graphics::ForegroundColor(Color* color) {
    foregroundColor = color->Pixel();
    XSetForeground(this->display, this->gc, foregroundColor);
    return foregroundColor;
    }

  UInt32 Graphics::JoinStyle() {
    return joinStyle;
    }

  UInt32 Graphics::JoinStyle(UInt32 w) {
    joinStyle = w;
    XSetLineAttributes(this->display, this->gc, this->lineWidth, this->lineStyle,
                       this->capStyle, this->joinStyle);
    return joinStyle;
    }

  UInt32 Graphics::LineStyle() {
    return lineStyle;
    }

  UInt32 Graphics::LineStyle(UInt32 w) {
    lineStyle = w;
    XSetLineAttributes(this->display, this->gc, this->lineWidth, this->lineStyle,
                       this->capStyle, this->joinStyle);
    return lineStyle;
    }

  UInt32 Graphics::LineWidth(UInt32 w) {
    lineWidth = w;
    XSetLineAttributes(this->display, this->gc, this->lineWidth, this->lineStyle,
                       this->capStyle, this->joinStyle);
    return lineWidth;
    }

  void Graphics::Clear() {
    XClearWindow(this->display, this->window);
    }

  void Graphics::DrawClearText(int x, int y, const char* text) {
#ifdef USEXFT
    XftTextExtents8(display, xftfont, (const FcChar8*)text, strlen(text), &ginfo);
    XFillRectangle(this->display, this->window, this->gc, x-ginfo.x, y-ginfo.y, ginfo.width, ginfo.height);
    XftDrawString8(xftdrawable, &xftcolor, xftfont, x, y, (const FcChar8*)text, strlen(text));
#else
    XDrawImageString(this->display, this->window, this->gc, x, y,
                text, strlen(text));
#endif
    }

  void Graphics::DrawClearText(int x, int y, String* text) {
    DrawClearText(x, y, text->AsCharArray());
    }
    
  void Graphics::DrawEllipse(int x, int y, int width, int height) {
    XDrawArc(this->display, this->window, this->gc, x, y, width, height, 0, 23040);
    }

  void Graphics::DrawImage(int x, int y, Image* image) {
    UInt32        xx,yy;
    int           pos;
    int           depth;
    unsigned long color;
    XImage       *xi;
    char*         data;
    Byte*         imageData;
    int           bytespixel;;
    int           bytesrow;
    XWindowAttributes winattr;
    XGetWindowAttributes(display, window, &winattr);
    depth = winattr.depth;
    bytespixel = (depth + 7) / 8;
    bytespixel = 4;
    bytesrow = (image->Width() * bytespixel + bytespixel) / 4 * 4;
    data = (char*)malloc(image->Height() * bytesrow);
    xi = XCreateImage(display, winattr.visual, depth, ZPixmap, 0, data,
                       image->Width(), image->Height(), 32, bytesrow);
    XInitImage(xi);
    pos = 0;
    imageData = image->ImageData();
    for (yy=0; yy<image->Height(); yy++) {
      for (xx=0; xx<image->Width(); xx++) {
        color = Color::RGB(imageData[pos] << 8,
                           imageData[pos+1] << 8,
                           imageData[pos+2] << 8);
        pos += 3;
        XPutPixel(xi,xx,yy,color);
        }
      }
    XPutImage(display,window,gc,xi,0,0,x,y,image->Width(),image->Height());
    XDestroyImage(xi);
    XFlush(display);
    }

  void Graphics::DrawImage(Drawable dest, int x, int y, Image* image) {
    UInt32        xx,yy;
    int           pos;
    int           depth;
    unsigned long color;
    XImage       *xi;
    char*         data;
    Byte*         imageData;
    int           bytespixel;;
    int           bytesrow;
    XWindowAttributes winattr;
    XGetWindowAttributes(display, window, &winattr);
    depth = winattr.depth;
    bytespixel = (depth + 7) / 8;
    bytespixel = 4;
    bytesrow = (image->Width() * bytespixel + bytespixel) / 4 * 4;
    data = (char*)malloc(image->Height() * bytesrow);
    xi = XCreateImage(display, winattr.visual, depth, ZPixmap, 0, data,
                       image->Width(), image->Height(), 32, bytesrow);
    XInitImage(xi);
    pos = 0;
    imageData = image->ImageData();
    for (yy=0; yy<image->Height(); yy++) {
      for (xx=0; xx<image->Width(); xx++) {
        color = Color::RGB(imageData[pos] << 8,
                           imageData[pos+1] << 8,
                           imageData[pos+2] << 8);
        pos += 3;
        XPutPixel(xi,xx,yy,color);
        }
      }
    XPutImage(display,dest,gc,xi,0,0,x,y,image->Width(),image->Height());
    XDestroyImage(xi);
    XFlush(display);
    }

  void Graphics::DrawLine(int x1, int y1, int x2, int y2) {
    XDrawLine(this->display, this->window, this->gc, x1, y1, x2, y2);
    }

  void Graphics::DrawLine(Point start, Point end) {
    XDrawLine(this->display, this->window, this->gc, start.X(), start.Y(),
              end.X(), end.Y());
    }

  void Graphics::DrawLine(Line* line) {
    Point start;
    Point end;
    start = line->Start();
    end = line->End();
    XDrawLine(this->display, this->window, this->gc, start.X(), start.Y(),
              end.X(), end.Y());
    }

  void Graphics::DrawLines(List<Point*>* points) {
    UInt32 i;
    XPoint *xpoints;
    if (points->Count() < 1) return;
    xpoints = (XPoint*)malloc(sizeof(XPoint) * points->Count());
    for (i=0; i<points->Count(); i++) {
      xpoints[i].x = (points->At(i))->X();
      xpoints[i].y = (points->At(i))->Y();
      }
    XDrawLines(this->display, this->window, this->gc, xpoints, points->Count(),
               CoordModeOrigin);
    free(xpoints);
    }

  void Graphics::DrawText(int x, int y, const char* text) {
#ifdef USEXFT
    XftDrawString8(xftdrawable, &xftcolor, xftfont, x, y, (const FcChar8*)text, strlen(text));
#else
    XDrawString(this->display, this->window, this->gc, x, y,
                text, strlen(text));
#endif
    }

  void Graphics::DrawText(int x, int y, String* text) {
    DrawText(x, y, text->AsCharArray());
    }

  void Graphics::DrawPoint(int x, int y) {
    XDrawPoint(this->display, this->window, this->gc, x, y);
    }

  void Graphics::DrawPoint(Point point) {
    XDrawPoint(this->display, this->window, this->gc, point.X(), point.Y());
    }

  void Graphics::DrawRectangle(int x, int y, int width, int height) {
    XDrawRectangle(this->display, this->window, this->gc, x, y, width, height);
    }

  void Graphics::DrawRectangle(Point start, Point end) {
    int x, y, width, height;
    x = start.X();
    y = start.Y();
    width = end.X() - start.X();
    height = end.Y() - start.Y();
    XDrawRectangle(this->display, this->window, this->gc, x, y, width, height);
    }

  void Graphics::DrawRectangle(Line line) {
    DrawRectangle(line.Start(), line.End());
    }

  void Graphics::DrawRectangle(Rectangle rect) {
    int x, y, width, height;
    x = rect.X();
    y = rect.Y();
    width = rect.Width();
    height = rect.Height();
    XDrawRectangle(this->display, this->window, this->gc, x, y, width, height);
    }

  void Graphics::FillEllipse(int x, int y, int width, int height) {
    XFillArc(this->display, this->window, this->gc, x, y, width, height, 0, 23040);
    }

  void Graphics::FillPolygon(List<Point*>* points, int hint) {
    UInt32 i;
    XPoint *xpoints;
    if (points->Count() < 1) return;
    xpoints = (XPoint*)malloc(sizeof(XPoint) * points->Count());
    for (i=0; i<points->Count(); i++) {
      xpoints[i].x = (points->At(i))->X();
      xpoints[i].y = (points->At(i))->Y();
      }
    XFillPolygon(this->display, this->window, this->gc, xpoints, points->Count(),
               hint, CoordModeOrigin);
    free(xpoints);
    }

  void Graphics::FillRectangle(int x, int y, int width, int height) {
    XFillRectangle(this->display, this->window, this->gc, x, y, width, height);
    }

  void Graphics::FillRectangle(Point start, Point end) {
    int x, y, width, height;
    x = start.X();
    y = start.Y();
    width = end.X() - start.X();
    height = end.Y() - start.Y();
    XFillRectangle(this->display, this->window, this->gc, x, y, width, height);
    }

  void Graphics::FillRectangle(Line line) {
    FillRectangle(line.Start(), line.End());
    }

  void Graphics::FillRectangle(Rectangle rect) {
    int x, y, width, height;
    x = rect.X();
    y = rect.Y();
    width = rect.Width();
    height = rect.Height();
    XFillRectangle(this->display, this->window, this->gc, x, y, width, height);
    }

  void Graphics::Flush() {
    XFlush(this->display);
    }

  UInt32 Graphics::LineWidth() {
    return lineWidth;
    }

  int Graphics::TextBaseline() {
    return font->ascent;
    }

  int Graphics::TextHeight() {
    return font->ascent + font->descent;
    }

  int Graphics::TextWidth(const char* text) {
#ifdef USEXFT
    XftTextExtents8(display, xftfont, (const FcChar8*)text, strlen(text), &ginfo);
    return ginfo.width;
#else
    return XTextWidth(font, text, strlen(text));
#endif
    }

  int Graphics::TextWidth(String* text) {
    return TextWidth(text->AsCharArray());
    }

  }

