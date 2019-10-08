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
  }

TextBox::~TextBox() {
  if (textChangedHandler != NULL) delete(textChangedHandler);
  }

void TextBox::MouseDownEvent(int x, int y, int button) {
  Control::MouseDownEvent(x, y, button);
printf("Text box mouse down\n");
  application->Focus(this);
  }

void TextBox::KeyUpEvent(KeySym key, XComposeStatus status) {
  String tmp;
  if (readOnly) return;
  switch (key) {
    case XK_KP_Enter:
         printf("Kp Enter Pressed\n");
         application->Focus(NULL);
         break;
    case XK_Return:
         printf("Enter Pressed\n");
         application->Focus(NULL);
         break;
    case XK_BackSpace:
         if (text.Length() > 0) {
           text = text.Substring(0, text.Length() - 1);
           if (this->textChangedHandler != NULL)
             textChangedHandler->Call(this, KeyEventArgs((int)key));
           Redraw();
           }
         break;
    case XK_Escape:
         printf("Escape Pressed\n");
         break;
    case XK_Tab:
         printf("Tab Pressed\n");
         break;
    case XK_Home:
         printf("Home Pressed\n");
         break;
    case XK_End:
         printf("End Pressed\n");
         break;
    case XK_Page_Up:
         printf("Page up Pressed\n");
         break;
    case XK_Page_Down:
         printf("Page down Pressed\n");
         break;
    case XK_Insert:
         printf("Insert Pressed\n");
         break;
    case XK_Delete:
         printf("Delete Pressed\n");
         break;
    case XK_Up:
         printf("Up arros Pressed\n");
         break;
    case XK_Down:
         printf("Down arros Pressed\n");
         break;
    case XK_Left:
         printf("Left arros Pressed\n");
         break;
    case XK_Right:
         printf("Right arros Pressed\n");
         break;
    default:
         if (key >= ' ' && key <= 255) {
           if (text.Length() < maxLen) {
             if (key >= 'a' && key <'z' && alphaCase == AlphaCaseUpper) key -= 32;
             if (key >= 'A' && key <'Z' && alphaCase == AlphaCaseLower) key += 32;
             tmp = text.Append(key);
             if (valid(&tmp)) {
               text = text.Append(key);
               if (this->textChangedHandler != NULL)
                 textChangedHandler->Call(this, KeyEventArgs((int)key));
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

String TextBox::Text() {
  return text;
  }

String TextBox::Text(const char* text) {
  if (text == NULL) {
    Control::Text(String());
    }
  if (strlen(text)<=255) Control::Text(text);
    else Control::Text(String(text).Substring(0,255));
  return this->text;
  }

String TextBox::Text(String text) {
  if (text.Length() < maxLen) Control::Text(text);
    else Control::Text(text.Substring(0, 255));
  return this->text;
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

