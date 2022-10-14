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

  ListBox::ListBox(Control* parent, int x, int y, int w, int h) :
         Panel(parent, x, y, w, h) {
    this->BorderWidth(1);
    this->objectType = (char*)"ListBox";
    this->items = new List<Object*>();
    this->selected = NULL;
    this->top = 0;
    this->selectMultiple = 0;
    this->pos = 0;
    this->scroller = new VScrollBar(this, width-20, 0, 20, height);
    this->valueChangedHandler = NULL;
    this->comparer = NULL;
    scroller->Visible(true);
    scroller->OnValueChanged(new EventHandler(ListBox::_ScrollChangedEvent, this));
    Panel::Add(this->scroller);
    }

  ListBox::~ListBox() {
    UInt32 i;
    for (i=0; i<items->Count(); i++)
      if (items->At(i) != NULL) delete(items->At(i));
    delete(items);
    if (valueChangedHandler != NULL) delete(valueChangedHandler);
    }

  void ListBox::scrollChangedEvent(void* sender, ValueEventArgs args) {
    top = args.Value();
    Redraw();
    }

  void ListBox::_ScrollChangedEvent(void* obj, void* sender, EventArgs args) {
    ((ListBox*)obj)->scrollChangedEvent(sender, (ValueEventArgs&)args);
    }

  void ListBox::Add(Object* o) {
    items->Add(o);
    if (items->Count() == 1)
      selected = (char*)malloc(items->Count());
    else
      selected = (char*)realloc(selected, items->Count());
    selected[items->Count() - 1] = 0;
    if ((int)(items->Count()) <= lineCount) {
      scroller->Visible(false);
      }
    else {
      scroller->Visible(true);
      scroller->MinValue(0);
      scroller->MaxValue(items->Count());
      scroller->PuckRange(lineCount);
      }
    Redraw();
    }

  void ListBox::Clear() {
    if (selected != NULL) free(selected);
    items->Clear();
    selected = NULL;
    top = 0;
    pos = 0;
    }

  void ListBox::Comparer(Int32 (*func)(Object*,Object*)) {
    comparer = func;
    }

  UInt32 ListBox::Count() {
    return items->Count();
    }

  int ListBox::IsItemSelected(int i) {
    if (i < 0 || i >= (int)(items->Count())) return 0;
    return (selected[i]) ? -1 : 0;
    }

  int ListBox::FirstSelectedIndex() {
    pos = 0;
    return NextSelectedIndex();
    }

  void ListBox::OnValueChanged(EventHandler* handler) {
    if (valueChangedHandler != NULL) delete(valueChangedHandler);
    valueChangedHandler = handler;
    }

  int ListBox::SelectedIndex() {
    pos = 0;
    return NextSelectedIndex();
    }

  void ListBox::SetSelectMultiple(int i) {
    int old;
    selectMultiple = (i) ? -1 : 0;
    if (selectMultiple == 0) {
      for (i=0; i<(int)(items->Count()); i++){
        old = selected[i];
        selected[i] = 0;
        if (old && valueChangedHandler != NULL) {
          valueChangedHandler->Call(this, ValueEventArgs(i));
          }
        }
      Redraw();
      }
    }

  int ListBox::NextSelectedIndex() {
    int ret;
    while (pos < (int)(items->Count())) {
      if (selected[pos]) {
        ret = pos++;
        return ret;
        }
      pos++;
      }
    return -1;
    }

  Object* ListBox::ItemAt(UInt32 i) {
    if (i > items->Count()) return NULL;
    return items->At(i);
    }

  Object* ListBox::SelectedItem() {
    if (SelectedIndex() < 0) return NULL;
    return items->At(SelectedIndex());
    }

  void ListBox::Sort() {
    Object* tmp;
    UInt32  i;
    Boolean flag;
    if (comparer == NULL) return;
    if (items->Count() < 2) return;
    flag = true;
    while (flag) {
      flag = false;
      for (i=0; i<items->Count()-1; i++) {
        if (comparer(items->At(i), items->At(i+1)) > 0) {
          tmp = items->At(i);
          items->AtPut(i, items->At(i+1));
          items->AtPut(i+1, tmp);
          flag = true;
          }
        }
      }
    }

  void ListBox::MouseDownEvent(int x, int y, int button) {
    Redraw();
    XFlush(display);
    Control::MouseDownEvent(x, y, button);
    }
    
  void ListBox::MouseUpEvent(int x, int y, int button) {
    int old;
    int i;
    int line;
    Control::MouseUpEvent(x, y, button);
    line = (y / lineHeight);
    if (line >= lineCount) line = -1;
    if (top + line >= (int)(items->Count())) line = -1;
    if (line >= 0) {
      selected[top+line] ^= 0xff;
      if (selected[top+line] && !selectMultiple) {
        for (i=0; i<(int)(items->Count()); i++)
          if (i != top+line) {
            old = selected[i];
            selected[i] = 0;
            if (old && valueChangedHandler != NULL) {
              valueChangedHandler->Call(this, ValueEventArgs(i));
              }
            }
        }
      Redraw();
      XFlush(display);
      if (this->valueChangedHandler != NULL) {
        valueChangedHandler->Call(this, ValueEventArgs(top+line));
        }
      }
    }

  void ListBox::Redraw() {
    int           i;
    int           last;
    GC            gc;
    XGCValues     values;
    XFontStruct*  font;
    unsigned long mask;
    if (!visible) return;
    XSetWindowBackground(display, window, backgroundColor);
    XClearWindow(display, window);
    font = XLoadQueryFont(display, "fixed");
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = backgroundColor;
    values.font = font->fid;
    mask = GCLineWidth | GCForeground | GCBackground | GCFont;
    gc = XCreateGC(display, window, mask, &values);
    lineHeight = font->ascent + font->descent;
    lineCount = height / lineHeight;
    last = top + (height / lineHeight);
    if (last >= (int)(items->Count())) last = items->Count() - 1;
    if (last-top >= lineCount) last = top + lineCount - 1;
    for (i=top; i<=last; i++) {
      if (selected != NULL) {
        XSetForeground(display, gc, (selected[i]?foregroundColor:backgroundColor));
        XFillRectangle(display,window,gc,0,(i-top)*lineHeight,width,lineHeight);
        if (selected[i]) {
          XSetForeground(display, gc, backgroundColor);
          XSetBackground(display, gc, foregroundColor);
          } else {
          XSetForeground(display, gc, foregroundColor);
          XSetBackground(display, gc, backgroundColor);
          }
        }
      XDrawImageString(display,window,gc, 5, (i-top)*lineHeight + font->ascent,
         items->At(i)->AsCharArray(), strlen(items->At(i)->AsCharArray()));
      }

    XFreeGC(display, gc);
    XFreeFont(display, font);
    }

  } 

