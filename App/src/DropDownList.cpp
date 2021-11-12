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

  DropDownList::DropDownList(Control* parent, int x, int y, int w, int h) :
         Panel(parent, x, y, w, h) {
    this->opened = false;
    this->BorderWidth(1);
    this->objectType = (char*)"DropDownList";
    this->items = new List<Object*>();
    this->selectedIndex = -1;
    this->top = 0;
    this->baseHeight = this->height;
    this->baseY = y;
    this->scroller = new VScrollBar(this, width-20, 0, 20, height);
    this->scroller->OnValueChanged(new EventHandler(DropDownList::_ScrollChangedEvent, this));
    scroller->Visible(false);
    this->valueChangedHandler = NULL;
    Panel::Add(this->scroller);
    }

  DropDownList::~DropDownList() {
    Clear();
    if (valueChangedHandler != NULL) delete(valueChangedHandler);
    }

  void DropDownList::scrollChangedEvent(void* sender, ValueEventArgs args) {
    top = args.Value();
    Redraw();
    }

  void DropDownList::_ScrollChangedEvent(void* obj, void* sender, EventArgs args) {
    ((DropDownList*)obj)->scrollChangedEvent(sender, (ValueEventArgs&)args);
    }

  void DropDownList::Add(Object* o) {
    items->Add(o);
    Redraw();
    }

  void DropDownList::RemoveItemAt(UInt32 i) {
    Object* obj;
    obj = items->At(i);
    if (obj != NULL) delete(obj);
    items->RemoveAt(i);
    }

  void DropDownList::Clear() {
    UInt32 i;
    for (i=0; i<items->Count(); i++)
      delete(items->At(i));
    items->Clear();
    top = 0;
    selectedIndex = -1;
    }

  UInt32 DropDownList::Count() {
    return items->Count();
    }

  Object* DropDownList::ItemAt(UInt32 i) {
    if (i < 0 || i>= items->Count()) throw RangeException(this, "Value out of range");
    return items->At(i);
    }

  UInt32 DropDownList::MaxVisibleItems() {
    return maxVisibleItems;
    }

  UInt32 DropDownList::MaxVisibleItems(UInt32 i) {
    if (i < 1) throw RangeException(this, "Value out of range");
    maxVisibleItems = i;
    Redraw();
    return maxVisibleItems;
    }

  void DropDownList::OnValueChanged(EventHandler* handler) {
    if (valueChangedHandler != NULL) delete(valueChangedHandler);
    valueChangedHandler = handler;
    }

  int DropDownList::SelectedIndex() {
    return selectedIndex;
    }

  int DropDownList::SelectedIndex(Int32 i) {
    if (i < -1) throw RangeException(this, "Value out of range");
    if ((UInt32)i >= items->Count()) throw RangeException(this, "Value out of range");
    selectedIndex = i;
    Redraw();
    return selectedIndex;
    }

  Object* DropDownList::SelectedItem() {
    if (selectedIndex < 0) return NULL;
    return items->At(selectedIndex);
    }

  void DropDownList::MouseDownEvent(int x, int y, int button) {
    Redraw();
    XFlush(display);
    Control::MouseDownEvent(x, y, button);
    }
    
  void DropDownList::MouseUpEvent(int x, int y, int button) {
    Graphics* gc;
    UInt32    count;
    UInt32    h;
    UInt32    cellHeight;
    opened = !opened;
    gc = GetGraphics();
    count = items->Count();
    if (count > maxVisibleItems) count = maxVisibleItems;
    cellHeight = gc->TextHeight() + 5;
    delete(gc);
    if (opened) {
      h = count * cellHeight;
      this->Height(h);
      scroller->Height(h);
      if (this->y+h > parent->Height())
        Location(this->x, parent->Height() - h);
      if (count < items->Count()) {
        scroller->MinValue(0);
        scroller->MaxValue(items->Count());
        scroller->PuckRange(count);
        scroller->Visible(true);
        }
      XRaiseWindow(display, window);
      }
    else {
      this->Height(baseHeight);
      this->Y(baseY);
      selectedIndex = top + (y / cellHeight);
      if ((UInt32)selectedIndex >= top + items->Count())
        selectedIndex = top + items->Count() - 1;
      if (valueChangedHandler != NULL)
        valueChangedHandler->Call(this, ValueEventArgs(selectedIndex));
      scroller->Visible(false);
      }
    Redraw();
    }

  void DropDownList::Redraw() {
    UInt32    i;
    UInt32    cell;
    List<Point*>* arrow;
    Graphics* gc;
    UInt32    count;
    if (!visible) return;
    gc = GetGraphics();
    XClearWindow(display, window);
    if (opened) {
      gc->ForegroundColor(Color::DarkGray());
      gc->DrawLine(0,0,width-1,0);
      gc->DrawLine(0,0,0,height-1);
      gc->DrawLine(0,height-1,width-1,height-1);
      gc->DrawLine(width-1,0,width-1,height-1);
      gc->ForegroundColor(Color::LightGray());
      gc->DrawLine(1,1,width-2,1);
      gc->DrawLine(1,1,1,height-2);
      gc->DrawLine(1,height-2,width-2,height-2);
      gc->DrawLine(width-2,1,width-2,height-2);
      gc->ForegroundColor(foregroundColor);
      count = items->Count();
      if (count > maxVisibleItems) count = maxVisibleItems;
      for (i=0; i<count; i++) {
        cell = i * (gc->TextHeight() + 5);
        gc->DrawText(5, cell + gc->TextHeight(),
          items->At(top+i)->ToString().AsCharArray());
        }
      }
    else {
      if (selectedIndex >= 0) {
        gc->ForegroundColor(foregroundColor);
        gc->DrawText(5,(height/2) + (gc->TextHeight() / 2),
          items->At(selectedIndex)->ToString().AsCharArray());
        }
      gc->ForegroundColor(Color::DarkGray());
      gc->DrawLine(0,0,width,0);
      gc->DrawLine(0,0,0,height);
      gc->DrawLine(0,height-1,width,height-1);
      gc->DrawLine(width-1,0,width-1,height-1);
      gc->ForegroundColor(Color::LightGray());
      gc->DrawLine(1,1,width-2,1);
      gc->DrawLine(1,1,1,height-2);
      gc->DrawLine(1,height-2,width-2,height-2);
      gc->DrawLine(width-2,1,width-2,height-2);
      gc->DrawLine(width - 22, 1, width - 22, height-2);
      gc->DrawLine(width - 18, 1, width - 18, height-2);
      gc->ForegroundColor(Color::DarkGray());
      gc->DrawLine(width - 21, 0, width - 21, height-1);
      gc->DrawLine(width - 19, 0, width - 19, height-1);
      gc->ForegroundColor(Color::Black());
      gc->DrawLine(width - 20, 0, width - 20, height);
      arrow = new List<Point*>();
      arrow->Add(new Point(width - 18, 2));
      arrow->Add(new Point(width - 2, 2));
      arrow->Add(new Point(width - 10, height - 2));
      gc->FillPolygon(arrow, 0);
      delete(arrow->At(0));
      delete(arrow->At(1));
      delete(arrow->At(2));
      delete(arrow);
      }
    delete(gc);


/*
    int           i;
    int           last;
    GC            gc;
    XGCValues     values;
    XFontStruct*  font;
    unsigned long mask;
    XSetWindowBackground(display, window, backgroundColor);
    XClearWindow(display, window);
    if (this->font.Length() == 0) font = XLoadQueryFont(display, "fixed");
      else font = XLoadQueryFont(display, this->font.AsCharArray());
    if (font == NULL) font = XLoadQueryFont(display, "fixed");
    values.line_width = 1;
    values.foreground = foregroundColor;
    values.background = backgroundColor;
    values.font = font->fid;
    mask = GCLineWidth | GCForeground | GCBackground | GCFont;
    gc = XCreateGC(display, window, mask, &values);
    lineHeight = font->ascent + font->descent;
    lineCount = height / lineHeight;
    if ((int)(items->Count()) <= lineCount) {
      scroller->Visible(false);
      }
    else {
      scroller->Visible(true);
      }
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
*/
    }

  } 

