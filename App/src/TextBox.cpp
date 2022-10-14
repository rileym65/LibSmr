#include <stdio.h>
#include <string.h>
#include "SmrAppFramework.h"

using namespace SmrFramework;

TextBox::TextBox(Control* parent, int x, int y, int w, int h, const char* text) :
       Label(parent, x, y, w, h, text) {
  XSetWindowBorderWidth(display, window, 1);
  this->borderWidth = 1;
  this->objectType = (char*)"TextBox";
  this->align = LEFT;
  this->maxLen = 255;
  this->validation = 0;
  this->alphaCase = 0;
  this->readOnly = false;
  if (strlen(text)<=255) Control::Text(text);
    else Control::Text(String(text).Substring(0,255));
  BackgroundColor(Color::RGB(0xffff, 0xffff, 0xffff));
  textChangedHandler = NULL;
  pos = 0;
  cursor_x = 0;
  hasFocus = false;
  }

TextBox::TextBox(Control* parent, int x, int y, int w, int h) :
       Label(parent, x, y, w, h, "") {
  XSetWindowBorderWidth(display, window, 1);
  this->borderWidth = 1;
  this->objectType = (char*)"TextBox";
  this->maxLen = 255;
  this->align = LEFT;
  this->validation = 0;
  this->alphaCase = 0;
  this->readOnly = false;
  Control::Text("");
  BackgroundColor(Color::RGB(0xffff, 0xffff, 0xffff));
  textChangedHandler = NULL;
  pos = 0;
  cursor_x = 0;
  hasFocus = false;
  }

TextBox::~TextBox() {
  if (textChangedHandler != NULL) delete(textChangedHandler);
  }

String TextBox::Text() {
  return text;
  }

String TextBox::Text(const char* text) {
  if (text == NULL) {
    this->text = Text(String());
    }
  if (strlen(text)<=255) this->text = String(text);
    else this->text = String(text).Substring(0,255);
  pos = this->text.Length();
  _updateCursorPosition();
  Redraw();
  return this->text;
  }

String TextBox::Text(String text) {
  if (text.Length() < maxLen) this->text = String(text);
    else this->text = text.Substring(0, 255);
  pos = this->text.Length();
  _updateCursorPosition();
  Redraw();
  return this->text;
  }

  void TextBox::Redraw() {
    GC            gc;
    unsigned long mask;
    XGCValues     values;
#ifdef USEXFT
    XftFont      *fontObj;
#else
    XFontStruct*  font;
#endif
    int           xoffset;
    int           yoffset;
    if (!visible) return;
    XClearWindow(display, window);
    if (enabled) {
#ifdef USEXFT
      XGlyphInfo ginfo;
      fontObj = font->FontObject();
      ascent = font->Ascent();
      descent = font->Descent();
      XftTextExtents8(display, fontObj, (const FcChar8*)text.AsCharArray(), text.Length(), &ginfo);
      yoffset = height / 2 - ascent / 2;
      if (align == CENTER)
        xoffset = width / 2 - ginfo.width / 2;
      if (align == LEFT)
        xoffset = 5;
      if (align == RIGHT)
        xoffset = (width - ginfo.width) - 5;
      text_x = ginfo.x+xoffset + textOffsetX;
      XftDrawString8(xftdrawable, &xftcolor, fontObj, ginfo.x+xoffset+textOffsetX,ginfo.y+yoffset+textOffsetY,
                    (const FcChar8*)text.AsCharArray(),text.Length());
      if (this == application->Focus() || (application->Focus() == NULL && hasFocus)) {
        values.line_width = 1;
        values.foreground = foregroundColor;
        values.background = backgroundColor;
        mask = GCLineWidth | GCForeground | GCBackground;
        gc = XCreateGC(display, window, mask, &values);
        XDrawLine(display, window, gc, text_x+cursor_x,height-2-yoffset+textOffsetY-ascent,
                                       text_x+cursor_x,height-2-yoffset+textOffsetY+descent+2);
        XDrawLine(display, window, gc, text_x+cursor_x-2,height-2-yoffset+textOffsetY-ascent-1,
                                       text_x+cursor_x-0,height-2-yoffset+textOffsetY-ascent-1);
        XDrawLine(display, window, gc, text_x+cursor_x+1,height-2-yoffset+textOffsetY-ascent-1,
                                       text_x+cursor_x+3,height-2-yoffset+textOffsetY-ascent-1);
        XDrawLine(display, window, gc, text_x+cursor_x-2,height-2-yoffset+textOffsetY+descent+2,
                                       text_x+cursor_x-0,height-2-yoffset+textOffsetY+descent+2);
        XDrawLine(display, window, gc, text_x+cursor_x+1,height-2-yoffset+textOffsetY+descent+2,
                                       text_x+cursor_x+3,height-2-yoffset+textOffsetY+descent+2);
        XFreeGC(display, gc);
        }

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
      if (align == CENTER)
        xoffset = (width - XTextWidth(font, text.AsCharArray(), text.Length())) / 2;
      if (align == LEFT)
        xoffset = 5;
      if (align == RIGHT)
        xoffset = (width - XTextWidth(font, text.AsCharArray(), text.Length())) - 5;
      text_x = xoffset + textOffsetX;
      XDrawString(display,window,gc, text_x,height-yoffset+textOffsetY,text.AsCharArray(),text.Length());
      if (this == application->Focus() || (application->Focus() == NULL && hasFocus)) {
        XDrawLine(display, window, gc, text_x+cursor_x,height-yoffset+textOffsetY-ascent,
                                       text_x+cursor_x,height-yoffset+textOffsetY+descent+2);
        XDrawLine(display, window, gc, text_x+cursor_x-2,height-yoffset+textOffsetY-ascent-1,
                                       text_x+cursor_x-0,height-yoffset+textOffsetY-ascent-1);
        XDrawLine(display, window, gc, text_x+cursor_x+1,height-yoffset+textOffsetY-ascent-1,
                                       text_x+cursor_x+3,height-yoffset+textOffsetY-ascent-1);
        XDrawLine(display, window, gc, text_x+cursor_x-2,height-yoffset+textOffsetY+descent+2,
                                       text_x+cursor_x-0,height-yoffset+textOffsetY+descent+2);
        XDrawLine(display, window, gc, text_x+cursor_x+1,height-yoffset+textOffsetY+descent+2,
                                       text_x+cursor_x+3,height-yoffset+textOffsetY+descent+2);
        }
      XFreeGC(display, gc);
      XFreeFont(display, font);
#endif
      }
    }

void TextBox::_updateCursorPosition() {
  int i;
  int c;
  int x;
  x = 0;
  i = 0;
  c = pos;
  Byte* w;
  w = font->Widths();
  while (c != 0) {
    x += w[(int)(text.CharAt(i++))];
    c--;
    }
  cursor_x = x;
  }

void TextBox::EnterEvent(int x, int y) {
  hasFocus = true;
  Redraw();
  }

void TextBox::LeaveEvent(int x, int y) {
  hasFocus = false;
  Redraw();
  }


void TextBox::MouseDownEvent(int x, int y, int button) {
  int p;
  int w;
  unsigned int c;
  Byte* widths;
  Control::MouseDownEvent(x, y, button);
  widths = font->Widths();
  x -= text_x;
  if (x < 0) x = 0;
  c = 0;
  p = 0;
  w = 0;
  if (text.Length() > 0) {
    w = widths[(int)(text.CharAt(c))] / 2;
    if (w < 1) w = 1;
    }
  while (c < text.Length() && x < (int)width-5 && x > p+w) {
    c++;
    if (c < text.Length()) {
      w = widths[(int)(text.CharAt(c))] / 2;
      p += widths[(int)(text.CharAt(c-1))];
      }
    }
  pos = c;
  _updateCursorPosition();
  application->Focus(this);
  Redraw();
  }

void TextBox::KeyUpEvent(KeySym key, XComposeStatus status) {
  String tmp;
  char   buffer[1024];
  if (readOnly) return;
  if (key == XK_KP_0) key = '0';
  if (key == XK_KP_1) key = '1';
  if (key == XK_KP_2) key = '2';
  if (key == XK_KP_3) key = '3';
  if (key == XK_KP_4) key = '4';
  if (key == XK_KP_5) key = '5';
  if (key == XK_KP_6) key = '6';
  if (key == XK_KP_7) key = '7';
  if (key == XK_KP_8) key = '8';
  if (key == XK_KP_9) key = '9';
  if (key == XK_KP_Decimal) key = '.';
  if (key == XK_KP_Divide) key = '/';
  if (key == XK_KP_Multiply) key = '*';
  if (key == XK_KP_Add) key = '+';
  if (key == XK_KP_Subtract) key = '-';
  switch (key) {
    case XK_Return:
    case XK_KP_Enter:
         application->Focus(NULL);
         Redraw();
         break;
    case XK_BackSpace:
         if (pos > 0) {
           if (text.Length() > 0) {
             if (pos >= (Int32)text.Length()) {
               text = text.Substring(0, text.Length() - 1);
               pos--;
               }
             else if (pos == 1) {
               text = text.Substring(1);
               pos--;
               }
             else {
               strncpy(buffer, text.AsCharArray(), pos-1);
               buffer[pos-1] = 0;
               strcat(buffer, text.AsCharArray()+pos);
               text = String(buffer);
               pos--;
               }
             if (this->textChangedHandler != NULL)
               textChangedHandler->Call(this, KeyEventArgs((int)key));
             _updateCursorPosition();
             Redraw();
             }
           }
         break;
    case XK_Escape:
         printf("Escape Pressed\n");
         break;
    case XK_Tab:
         printf("Tab Pressed\n");
         break;
    case XK_Home:
    case XK_KP_Home:
         pos = 0;
         _updateCursorPosition();
         Redraw();
         break;
    case XK_End:
    case XK_KP_End:
         pos = text.Length();
         _updateCursorPosition();
         Redraw();
         break;
    case XK_Insert:
    case XK_KP_Insert:
         printf("Insert Pressed\n");
         break;
    case XK_Delete:
    case XK_KP_Delete:
         if (pos < (Int32)text.Length() && text.Length() > 0) {
           if (pos == 0) {
             text = text.Substring(1);
             }
           else {
             strncpy(buffer, text.AsCharArray(), pos);
             buffer[pos] = 0;
             strcat(buffer, text.AsCharArray() + pos + 1);
             text = String(buffer);
             }
           if (this->textChangedHandler != NULL)
             textChangedHandler->Call(this, KeyEventArgs((int)key));
           _updateCursorPosition();
           Redraw();
           }
         break;
    case XK_Left:
    case XK_KP_Left:
         if (pos > 0) pos--;
         _updateCursorPosition();
         Redraw();
         break;
    case XK_Right:
    case XK_KP_Right:
         if (pos < (Int32)text.Length()) pos++;
         _updateCursorPosition();
         Redraw();
         break;
    default:
         if (key >= ' ' && key <= 255) {
           if (text.Length() < maxLen) {
             if (key >= 'a' && key <'z' && alphaCase == AlphaCaseUpper) key -= 32;
             if (key >= 'A' && key <'Z' && alphaCase == AlphaCaseLower) key += 32;
             if (pos >= (int)text.Length()) {
               tmp = text.Append(key);
               pos++;
               }
             else {
               if (pos == 0) {
                 buffer[0] = key;
                 buffer[1] = 0;
                 strcat(buffer, text.AsCharArray());
                 }
               else {
                 strncpy(buffer, text.AsCharArray(), pos);
                 buffer[pos] = key;
                 buffer[pos+1] = 0;
                 strcat(buffer, text.AsCharArray() + pos);
                 }
               pos++;
               tmp = String(buffer);
               }
             if (valid(&tmp)) {
               text = String(tmp);
               if (this->textChangedHandler != NULL)
                 textChangedHandler->Call(this, KeyEventArgs((int)key));
               _updateCursorPosition();
               Redraw();
               }
             }
           }
         break;
    }
  if (this->keyUpHandler != NULL) {
    keyUpHandler->Call(this, KeyEventArgs((int)key));
    }
  }

Byte TextBox::AlphaCase() {
  return alphaCase;
  }

Byte TextBox::AlphaCase(Byte b) {
  if (b <= 2) alphaCase = b;
  return alphaCase;
  }

Word TextBox::MaxLen() {
  return maxLen;
  }

Word TextBox::MaxLen(Word w) {
  maxLen = w;
  return maxLen;
  }

void TextBox::OnTextChanged(EventHandler* handler) {
  if (this->textChangedHandler != NULL) delete(this->textChangedHandler);
  this->textChangedHandler = handler;
  }

Boolean TextBox::ReadOnly() {
  return readOnly;
  }

Boolean TextBox::ReadOnly(Boolean b) {
  readOnly = b;
  return readOnly;
  }

Boolean TextBox::valid(String* t) {
  UInt32 i;
  UInt32 alpha;
  UInt32 number;
  UInt32 dots;
  UInt32 minus;
  UInt32 other;
  UInt32 hex;
  char   ch;
  if (validation == ValidationNone) return true;
  if (t->Length() == 0) return true;
  alpha = 0;
  number = 0;
  dots = 0;
  minus = 0;
  other = 0;
  hex = 0;
  for (i=0; i<t->Length(); i++) {
    ch = t->CharAt(i);
    if (ch >= 'a' && ch <= 'z') alpha++;
    else if (ch >= 'A' && ch <= 'Z') alpha++;
    else if (ch >= '0' && ch <= '9') number++;
    else if (ch == '-') minus++;
    else if (ch == '.') dots++;
    else other++;
    if (ch >= 'a' && ch <= 'f') hex++;
    if (ch >= 'A' && ch <= 'F') hex++;
    }
  switch (validation) {
    case ValidationInt:
         if (alpha > 0) return false;
         if (dots > 0) return false;
         if (minus > 1) return false;
         if (minus == 1 && t->CharAt(0) != '-') return false;
         if (other > 0) return false;
         break;
    case ValidationUInt:
         if (alpha > 0) return false;
         if (dots > 0) return false;
         if (minus > 0) return false;
         if (other > 0) return false;
         break;
    case ValidationFloat:
         if (alpha > 0) return false;
         if (dots > 1) return false;
         if (minus > 1) return false;
         if (minus == 1 && t->CharAt(0) != '-') return false;
         if (other > 0) return false;
         break;
    case ValidationUFloat:
         if (alpha > 0) return false;
         if (dots > 1) return false;
         if (minus > 0) return false;
         if (other > 0) return false;
         break;
    case ValidationHex:
         if (hex != t->Length()) return false;
         break;
    case ValidationAlpha:
         if (number > 0) return false;
         if (dots > 0) return false;
         if (minus > 0) return false;
         if (other > 0) return false;
         break;
    case ValidationAlphaNumeric:
         if (dots > 0) return false;
         if (minus > 0) return false;
         if (other > 0) return false;
         break;
    }
  return true;
  }

Byte TextBox::Validation() {
  return validation;
  }

Byte TextBox::Validation(Byte b) {
  if (b <= 7) validation = b;
  return validation;
  }

void TextBox::LostFocus() {
  Redraw();
  }


