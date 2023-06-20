#include <string.h>
#include <math.h>
#include <stdio.h>
#include "SmrFramework.h"

using namespace SmrFramework;

RpnCalculator::RpnCalculator() {
  objectType = (char*)"RpnCalculator";
  sp = 0;
  trigMode = 'R';
  numVariables = 0;
  }

RpnCalculator::~RpnCalculator() {
  UInt32 i;
  if (numVariables > 0) {
    for (i=0; i<numVariables; i++)
      free(variableNames[i]);
    free(variableNames);
    free(variableValues);
    }
  }

Boolean RpnCalculator::_isTerm(char c) {
  if (c >= 'a' && c<= 'z') return false;
  if (c >= 'A' && c<= 'Z') return false;
  if (c >= '0' && c<= '9') return false;
  return true;
  }

Int32 RpnCalculator::_findVariable(const char* name) {
  UInt32 i;
  for (i=0; i<numVariables; i++)
    if (strcmp(name, variableNames[i]) == 0) {
      return i;
      }
  return -1;
  }

Double RpnCalculator::_pop() {
  if (sp == 0)
    throw InvalidOpException("Stack empty");
  return stack[--sp];
  }

void RpnCalculator::_push(Double d) {
  if (sp == 1023)
    throw InvalidOpException("Stack overflow");
  stack[sp++] = d;
  }

Double RpnCalculator::Calculate(const char* expr) {
  Boolean dot;
  Boolean neg;
  Double  value;
  Double  decimals;
  char    token[128];
  UInt32  pos;
  while (*expr != 0) {

    while (*expr == ' ') {
      expr++;
      }

    if (*expr == '+') {
      if (sp < 2) throw InvalidOpException("Stack empty");
      stack[sp-2] += stack[sp-1];
      sp--;
      expr++;
      }

    else if (*expr == '-' && _isTerm(expr[1])) {
      if (sp < 2) throw InvalidOpException("Stack empty");
      stack[sp-2] -= stack[sp-1];
      sp--;
      expr++;
      }

    else if (*expr == '*') {
      if (sp < 2) throw InvalidOpException("Stack empty");
      stack[sp-2] *= stack[sp-1];
      sp--;
      expr++;
      }

    else if (*expr == '/') {
      if (sp < 2) throw InvalidOpException("Stack empty");
      stack[sp-2] /= stack[sp-1];
      sp--;
      expr++;
      }

    else if (*expr == '.') {
      value = _pop();
      printf("%f",value);
      expr++;
      }

    else if (strncmp(expr, "abs", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (stack[sp-1] < 0) stack[sp-1] = -stack[sp-1];
      expr += 3;
      }

    else if (strncmp(expr, "acos", 4) == 0 && _isTerm(expr[4])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (trigMode == 'R') stack[sp-1] = acos(stack[sp-1]);
      if (trigMode == 'D') stack[sp-1] = acos(stack[sp-1])*180.0/M_PI;
      expr += 4;
      }

    else if (strncmp(expr, "asin", 4) == 0 && _isTerm(expr[4])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (trigMode == 'R') stack[sp-1] = asin(stack[sp-1]);
      if (trigMode == 'D') stack[sp-1] = asin(stack[sp-1])*180.0/M_PI;
      expr += 4;
      }

    else if (strncmp(expr, "atan", 4) == 0 && _isTerm(expr[4])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (trigMode == 'R') stack[sp-1] = atan(stack[sp-1]);
      if (trigMode == 'D') stack[sp-1] = atan(stack[sp-1])*180.0/M_PI;
      expr += 4;
      }

    else if (strncmp(expr, "chs", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      stack[sp-1] = -stack[sp-1];
      expr += 3;
      }

    else if (strncmp(expr, "cos", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (trigMode == 'R') stack[sp-1] = cos(stack[sp-1]);
      if (trigMode == 'D') stack[sp-1] = cos(stack[sp-1]*M_PI/180.0);
      expr += 3;
      }

    else if (strncmp(expr, "cr", 2) == 0 && _isTerm(expr[2])) {
      printf("\n");
      expr += 2;
      }

    else if (strncmp(expr, "deg", 3) == 0 && _isTerm(expr[3])) {
      trigMode = 'D';
      expr += 3;
      }

    else if (strncmp(expr, "dup", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      _push(stack[sp-1]);
      expr += 3;
      }

    else if (strncmp(expr, "drop", 4) == 0 && _isTerm(expr[4])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      _pop();
      expr += 4;
      }

    else if (strncmp(expr, "e", 1) == 0 && _isTerm(expr[1])) {
      _push(M_E);
      expr += 1;
      }

    else if (strncmp(expr, "exp", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      stack[sp-1] = exp(stack[sp-1]);
      expr += 3;
      }

    else if (strncmp(expr, "frac", 4) == 0 && _isTerm(expr[4])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (stack[sp-1] < 0) {
        value = -stack[sp-1];
        value = value - (int)value;
        stack[sp-1] = -value;
        }
      else {
        stack[sp-1] = stack[sp-1] - (int)stack[sp-1];
        }
      expr += 4;
      }

    else if (strncmp(expr, "hex", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      printf("%x",(int)stack[sp-1]);
      expr += 3;
      }

    else if (strncmp(expr, "int", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      stack[sp-1] = (int)stack[sp-1];
      expr += 3;
      }

    else if (strncmp(expr, "ln", 2) == 0 && _isTerm(expr[2])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      stack[sp-1] = log(stack[sp-1]);
      expr += 2;
      }

    else if (strncmp(expr, "log", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      stack[sp-1] = log10(stack[sp-1]);
      expr += 3;
      }

    else if (strncmp(expr, "pi", 2) == 0 && _isTerm(expr[2])) {
      _push(M_PI);
      expr += 2;
      }

    else if (strncmp(expr, "pow", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 2) throw InvalidOpException("Stack empty");
      stack[sp-2] = pow(stack[sp-2], stack[sp-1]);
      sp--;
      expr += 3;
      }

    else if (strncmp(expr, "rad", 3) == 0 && _isTerm(expr[3])) {
      trigMode = 'R';
      expr += 3;
      }

    else if (strncmp(expr, "sgn", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (stack[sp-1] < 0) stack[sp-1] = -1;
      if (stack[sp-1] > 0) stack[sp-1] = 1;
      expr += 3;
      }

    else if (strncmp(expr, "sin", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (trigMode == 'R') stack[sp-1] = sin(stack[sp-1]);
      if (trigMode == 'D') stack[sp-1] = sin(stack[sp-1]*M_PI/180.0);
      expr += 3;
      }

    else if (strncmp(expr, "sp", 2) == 0 && _isTerm(expr[2])) {
      printf(" ");
      expr += 2;
      }

    else if (strncmp(expr, "sqr", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      stack[sp-1] = stack[sp-1] * stack[sp-1];
      expr += 3;
      }

    else if (strncmp(expr, "sqrt", 4) == 0 && _isTerm(expr[4])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      stack[sp-1] = sqrt(stack[sp-1]);
      expr += 4;
      }

    else if (strncmp(expr, "swp", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 2) throw InvalidOpException("Stack empty");
      value = stack[sp-2];
      stack[sp-2] = stack[sp-1];
      stack[sp-1] = value;
      expr += 3;
      }

    else if (strncmp(expr, "tan", 3) == 0 && _isTerm(expr[3])) {
      if (sp < 1) throw InvalidOpException("Stack empty");
      if (trigMode == 'R') stack[sp-1] = tan(stack[sp-1]);
      if (trigMode == 'D') stack[sp-1] = tan(stack[sp-1]*M_PI/180.0);
      expr += 3;
      }

    else if (*expr == '$' &&
             ((expr[1] >= '0' && expr[1] <= '9') ||
              (expr[1] >= 'a' && expr[1] <= 'f') ||
              (expr[1] >= 'A' && expr[1] <= 'F'))) {
      value = 0;
      expr++;
      while ((*expr >= '0' && *expr <= '9') ||
             (*expr >= 'a' && *expr <= 'f') ||
             (*expr >= 'A' && *expr <= 'F')) {
        value *= 16;
        if (*expr >= '0' && *expr <= '9') value += (*expr - '0');
        if (*expr >= 'A' && *expr <= 'F') value += (*expr - 55);
        if (*expr >= 'a' && *expr <= 'f') value += (*expr - 87);
        expr++;
        }
      _push(value);
      }

    else if ((*expr >= '0' && *expr <= '9') ||
             (*expr == '-' && expr[1] >= '0' && expr[1] <= '9')) {
      dot = false;
      neg = false;
      if (*expr == '-') {
        expr++;
        neg = true;
        }
      decimals = 1;
      value = 0;
      while ((*expr >= '0' && *expr <= '9') || *expr == '.') {
        if (*expr == '.') {
          if (dot) throw InvalidOpException("Invalid number");
          dot = true;
          }
        else {
          value = value * 10 + (*expr - '0');
          if (dot) decimals *= 10;
          }
        expr++;
        }
      if (decimals > 1) value /= decimals;
      if (neg) value = -value;
      _push(value);
      }

    else if ((*expr >= 'a' && *expr <= 'z') ||
             (*expr >= 'A' && *expr <= 'Z')) {
      pos = 0;
      while ((*expr >= 'a' && *expr <= 'z') ||
             (*expr >= 'A' && *expr <= 'Z') ||
             (*expr >= '0' && *expr <= '9') ||
             *expr == '_') {
        token[pos++] = *expr++;
        }
      token[pos] = 0;
      _push(GetVar(token));
      }

    else if (*expr == '!' &&
              ((expr[1] >= 'a' && expr[1] <= 'z') ||
              (expr[1] >= 'A' && expr[1] <= 'Z'))) {
      expr++;
      pos = 0;
      while ((*expr >= 'a' && *expr <= 'z') ||
             (*expr >= 'A' && *expr <= 'Z') ||
             (*expr >= '0' && *expr <= '9') ||
             *expr == '_') {
        token[pos++] = *expr++;
        }
      token[pos] = 0;
      SetVar(token, _pop());
      }

    else {
      throw InvalidOpException("Invalid expression");
      }

    }
  if (sp == 0) throw InvalidOpException("No result");
  return stack[--sp];
  }

Double RpnCalculator::GetVar(const char* name) {
  Int32 v;
  v = _findVariable(name);
  if (v < 0) throw NotFoundException("Variable not found");
  return variableValues[v];
  }

void RpnCalculator::SetVar(const char* name, Double value) {
  Int32  v;
  v = _findVariable(name);
  if (v >= 0) {
    variableValues[v] = value;
    return;
    }
  numVariables++;
  if (numVariables == 1) {
    variableNames = (char**)malloc(sizeof(char*));
    variableValues = (Double*)malloc(sizeof(Double));
    }
  else {
    variableNames = (char**)realloc(variableNames,sizeof(char*)*numVariables);
    variableValues = (Double*)realloc(variableValues,sizeof(Double)*numVariables);
    }
  variableValues[numVariables-1] = value;
  variableNames[numVariables-1] = (char*)malloc(strlen(name)+1);
  strcpy(variableNames[numVariables-1], name);
  }

