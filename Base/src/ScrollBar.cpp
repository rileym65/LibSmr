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
#include "SmrFramework.h"

namespace SmrFramework {

  ScrollBar::ScrollBar(Control* parent, int x, int y, int w, int h) :
         Control(parent, x, y, w, h) {
    this->minValue = 0;
    this->maxValue = 100;
    this->puckRange = 10;
    this->puckPosition = 0;
    this->value = 0;
    this->puckGrabbed = 0;
    this->valueChangedHandler = NULL;
    this->objectType = (char*)"ScrollBar";
    XSetWindowBorderWidth(display, window, 0);
    this->borderWidth = 0;
    if (parent != NULL) BackgroundColor(parent->BackgroundColor());
    XFlush(display);
    }

  ScrollBar::~ScrollBar() {
    if (valueChangedHandler != NULL) delete(valueChangedHandler);
    }

  double ScrollBar::MinValue() {
    return minValue;
    }

  double ScrollBar::MinValue(double v) {
    minValue = v;
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    return minValue;
    }

  double ScrollBar::MaxValue() {
    return maxValue;
    }

  double ScrollBar::MaxValue(double v) {
    maxValue = v;
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    return maxValue;
    }

  double ScrollBar::PuckRange() {
    return puckRange;
    }

  double ScrollBar::PuckRange(double v) {
    puckRange = v;
    computePuck();
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    return puckRange;
    }

  double ScrollBar::Value() {
    return value;
    }

  double ScrollBar::Value(double v) {
    if (v > maxValue) v = maxValue;
    updateValue(v);
    puckPosition = value / pixelValue;
    Redraw();
    return value;
    }

  void ScrollBar::OnValueChanged(EventHandler* handler) {
    if (valueChangedHandler != NULL) delete(valueChangedHandler);
    valueChangedHandler = handler;
    }

  void ScrollBar::movePuck(int amount) {
    puckPosition += amount;
    if (puckPosition < 0) puckPosition = 0;
    if (puckPosition + puckLength >= high-low+1)
      puckPosition = high-low-puckLength+1;
    updateValue((double)puckPosition * pixelValue);
    Redraw();
    }

  void ScrollBar::adjustPuck(int pointer) {
    if (pointer < low) {
      movePuck(-1);
      }
    else if (pointer > high) {
      movePuck(1);
      }
    else if (pointer < puckPosition + low) {
      movePuck(-puckLength);
      }
    else if (pointer > puckPosition + low + puckLength) {
      movePuck(puckLength);
      }
    else {
      puckGrabbed = -1;
      mouseOffset = pointer - (puckPosition + low);
      }
    }

  void ScrollBar::updateValue(double v) {
    value = v + minValue;
    if (value < minValue) value = minValue;
    if (value > maxValue) value = maxValue;
    if (this->valueChangedHandler != NULL) {
      valueChangedHandler->Call(this, ValueEventArgs(value));
      }
    }

  void ScrollBar::computePuck() {
    double graphicRange;
    double valueRange;
    double ratio;
    graphicRange = high - low;
    valueRange = maxValue - minValue;
    ratio = puckRange / valueRange;
    puckLength = graphicRange * ratio;
    if (puckLength < 10) puckLength = 10;
    graphicRange -= puckLength;
    pixelValue = valueRange / graphicRange;
    }

  void ScrollBar::computeGraphics(int longSide, int shortSide) {
    low = shortSide + 1;
    high = longSide - shortSide - 2;
    }


  } 

