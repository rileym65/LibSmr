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
#include "SmrAppFramework.h"

namespace SmrFramework {

  TextFont::TextFont(const char* name) {
    UInt32   i;
    UInt32   height;
    Display* display;
#ifdef USEXFT
    char buffer[2];
#endif
    this->objectType = (char*)"Font";
    display = application->GetDisplay();
#ifdef USEXFT
    xftfont = XftFontOpenName(display, DefaultScreen(display), name);
    if (xftfont == NULL) throw InvalidOpException(this, "Could not open font");
    for (i=0; i<255; i++) widths[i] = 6;
    height = 0;
    for (i=32; i<127; i++) {
      buffer[0] = i;
      buffer[1] = 0;
      XftTextExtents8(display, xftfont, (const FcChar8*)buffer, 1, &ginfo);
      if (ginfo.height > height) height = ginfo.height;
      widths[i] = ginfo.xOff;
      }
    ascent = height;
    descent = 0;
#else
    xfont = XLoadQueryFont(display, name);
    if (xfont == NULL) throw InvalidOpException(this, "Could not open font");
    ascent = xfont->max_bounds.ascent;
    descent = xfont->max_bounds.descent;
    for (i=0; i<255; i++) widths[i] = xfont->max_bounds.width;
    if (xfont->per_char != NULL) {
      for (i=xfont->min_char_or_byte2; i<=xfont->max_char_or_byte2; i++) {
        if (xfont->per_char[i].width == 0)
          widths[i] = xfont->max_bounds.width;
        else
          widths[i] = xfont->per_char[i].width;
        }
      }
#endif
    }

  TextFont::~TextFont() {
    Display* display;
    display = application->GetDisplay();
#ifdef USEXFT
    if (xftfont != NULL) XftFontClose(display, xftfont);
#else
    XFreeFont(display, xfont);
#endif
    }

#ifdef USEXFT
XftFont* TextFont::FontObject() {
  return xftfont;
  }
#else
XFontStruct* TextFont::FontObject() {
  return xfont;
  }
#endif

int   TextFont::Ascent() {
  return ascent;
  }

int   TextFont::Descent() {
  return descent;
  }

#ifdef USEXFT
XGlyphInfo TextFont::GlyphInfo() {
  return ginfo;
  }
#endif

Byte* TextFont::Widths() {
  return widths;
  }

  }
