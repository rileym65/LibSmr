#include <string.h>
#include <math.h>
#include <stdio.h>
#include "SmrFramework.h"

using namespace SmrFramework;

#define OP_END     0x01
#define OP_OP      0x10
#define OP_CP      0x11
#define OP_LAND    0x21
#define OP_LOR     0x22
#define OP_AND     0x31
#define OP_OR      0x32
#define OP_XOR     0x33
#define OP_EQ      0x40
#define OP_NE      0x41
#define OP_LT      0x42
#define OP_GT      0x43
#define OP_LTE     0x44
#define OP_GTE     0x45
#define OP_SHR     0x50
#define OP_SHL     0x51
#define OP_SUB     0x61
#define OP_ADD     0x60
#define OP_MUL     0x70
#define OP_DIV     0x71
#define OP_MOD     0x72
#define OP_POW     0x80
#define OP_ABS     0xa0
#define OP_CHS     0xa1
#define OP_SGN     0xa2
#define OP_SQR     0xa3
#define OP_SQRT    0xa4
#define OP_COS     0xa5
#define OP_SIN     0xa6
#define OP_TAN     0xa7
#define OP_ACOS    0xa8
#define OP_ASIN    0xa9
#define OP_ATAN    0xaa
#define OP_LN      0xab
#define OP_LOG     0xac
#define OP_EXP     0xad
#define OP_INT     0xae
#define OP_FRAC    0xaf
#define OP_PI      0xd0
#define OP_E       0xd1
#define OP_HI      0xe0
#define OP_LO      0xe1

Calculator::Calculator() {
  objectType = (char*)"Calculator";
  trigMode = 'D';
  numVariables = 0;
  }

Calculator::~Calculator() {
  UInt32 i;
  if (numVariables > 0) {
    for (i=0; i<numVariables; i++)
      free(variableNames[i]);
    free(variableNames);
    free(variableValues);
    }
  }

Boolean Calculator::_isTerm(char c) {
  if (c >= 'a' && c<= 'z') return false;
  if (c >= 'A' && c<= 'Z') return false;
  if (c >= '0' && c<= '9') return false;
  return true;
  }

Int32 Calculator::_findVariable(const char* name) {
  UInt32 i;
  for (i=0; i<numVariables; i++)
    if (strcmp(name, variableNames[i]) == 0) {
      return i;
      }
  return -1;
  }

Double Calculator::Calculate(const char* line) {
  Double numbers[128];
  Byte ops[128];
  Byte nsp;
  Byte osp;
  char name[64];
  int  flag;
  int p;
  Double v;
  Boolean neg;
  Boolean dot;
  Double  decimals;
  Byte op;
  osp = 0;
  nsp = 0;
  while (*line == ' ' || *line == '\t') line++;
  while (*line != 0 && *line != ',' && *line != ']') {
    flag = -1;
    while (flag) {
      flag = 0;
      if (*line == '(') {
        ops[osp++] = OP_OP;
        line++;
        flag = -1;
        }
      else if (strncasecmp(line,"HI(",3) == 0) {
        ops[osp++] = OP_HI;
        ops[osp++] = OP_OP;
        line += 3;
        flag = -1;
        }
      else if (strncasecmp(line,"LO(",3) == 0) {
        ops[osp++] = OP_LO;
        ops[osp++] = OP_OP;
        line += 3;
        flag = -1;
        }
      else if (strncasecmp(line,"abs(",4) == 0) {
        ops[osp++] = OP_ABS;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"chs(",4) == 0) {
        ops[osp++] = OP_CHS;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"sgn(",4) == 0) {
        ops[osp++] = OP_SGN;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"sqr(",4) == 0) {
        ops[osp++] = OP_SQR;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"sqrt(",5) == 0) {
        ops[osp++] = OP_SQRT;
        ops[osp++] = OP_OP;
        line += 5;
        flag = -1;
        }
      else if (strncasecmp(line,"cos(",4) == 0) {
        ops[osp++] = OP_COS;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"sin(",4) == 0) {
        ops[osp++] = OP_SIN;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"tan(",4) == 0) {
        ops[osp++] = OP_TAN;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"acos(",5) == 0) {
        ops[osp++] = OP_ACOS;
        ops[osp++] = OP_OP;
        line += 5;
        flag = -1;
        }
      else if (strncasecmp(line,"asin(",5) == 0) {
        ops[osp++] = OP_ASIN;
        ops[osp++] = OP_OP;
        line += 5;
        flag = -1;
        }
      else if (strncasecmp(line,"atan(",5) == 0) {
        ops[osp++] = OP_ATAN;
        ops[osp++] = OP_OP;
        line += 5;
        flag = -1;
        }
      else if (strncasecmp(line,"ln(",3) == 0) {
        ops[osp++] = OP_LN;
        ops[osp++] = OP_OP;
        line += 3;
        flag = -1;
        }
      else if (strncasecmp(line,"log(",4) == 0) {
        ops[osp++] = OP_LOG;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"exp(",4) == 0) {
        ops[osp++] = OP_EXP;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"int(",4) == 0) {
        ops[osp++] = OP_INT;
        ops[osp++] = OP_OP;
        line += 4;
        flag = -1;
        }
      else if (strncasecmp(line,"frac(",5) == 0) {
        ops[osp++] = OP_FRAC;
        ops[osp++] = OP_OP;
        line += 5;
        flag = -1;
        }
      while (*line == ' ' || *line == '\t') line++;
      }
    if (*line == '\'') {
      line++;
      numbers[nsp++] = *line++;
      if (*line != '\'') {
        throw InvalidOpException("Character constant more than 1 character");
        }
      line++;
      }
    else if (strncmp(line,"pi",2) == 0 && _isTerm(line[2])) {
      numbers[nsp++] =  M_PI;
      line += 2;
      }
    else if (strncmp(line,"e",1) == 0 && _isTerm(line[1])) {
      numbers[nsp++] =  M_E;
      line += 1;
      }
    else if ((*line >= 'a' && *line <= 'z') ||
        (*line >= 'A' && *line <= 'Z')) {
      p = 0;
      while ((*line >= 'a' && *line <= 'z') ||
             (*line >= 'A' && *line <= 'Z') ||
             (*line >= '0' && *line <= '9') ||
             *line == '_') name[p++] = *line++;
      name[p] = 0;
      numbers[nsp++] =  GetVar(name);
      }
    else if (*line == '$') {
      v = 0;
      line++;
      while ((*line >= '0' && *line <= '9') ||
             (*line >= 'a' && *line <= 'f') ||
             (*line >= 'A' && *line <= 'F')) {
        if (*line >= '0' && *line <= '9') v = (v * 16) + (*line - '0');
        if (*line >= 'a' && *line <= 'f') v = (v * 16) + (*line - 87);
        if (*line >= 'A' && *line <= 'F') v = (v * 16) + (*line - 55);
        line++;
        }
      numbers[nsp++] = v;
      }

    else if ((*line >= '0' && *line <= '9') ||
             (*line == '-' && line[1] >= '0' && line[1] <= '9')) {
      dot = false;
      neg = false;
      if (*line == '-') {
        line++;
        neg = true;
        }
      decimals = 1;
      v = 0;
      while ((*line >= '0' && *line <= '9') || *line == '.') {
        if (*line == '.') {
          if (dot) throw InvalidOpException("Invalid number");
          dot = true;
          }
        else {
          v = v * 10 + (*line - '0');
          if (dot) decimals *= 10;
          }
        line++;
        }
      if (decimals > 1) v /= decimals;
      if (neg) v = -v;
      numbers[nsp++] = v;
      }

    else {
      }
    while (*line == ' ' || *line == '\t') line++;
    flag = -1;
    while (flag) {
      flag = 0;
      if (*line == '+') { op = OP_ADD; line++; }
      else if (*line == '-') { op = OP_SUB; line++; }
      else if (*line == '*' && *(line+1) == '*') { op = OP_POW; line+=2; }
      else if (*line == '*') { op = OP_MUL; line++; }
      else if (*line == '/') { op = OP_DIV; line++; }
      else if (*line == '%') { op = OP_MOD; line++; }
      else if (*line == '&' && *(line+1) == '&') { op = OP_LAND; line+=2; }
      else if (*line == '&') { op = OP_AND; line++; }
      else if (*line == '|' && *(line+1) == '|') { op = OP_LOR; line+=2; }
      else if (*line == '|') { op = OP_OR; line++; }
      else if (*line == '^') { op = OP_XOR; line++; }
      else if (*line == '=') { op = OP_EQ; line++; }
      else if (*line == '<' && *(line+1) == '>') { op = OP_NE; line+=2; }
      else if (*line == '<' && *(line+1) == '=') { op = OP_LTE; line+=2; }
      else if (*line == '<' && *(line+1) == '<') { op = OP_SHL; line+=2; }
      else if (*line == '<') { op = OP_LT; line++; }
      else if (*line == '>' && *(line+1) == '=') { op = OP_GTE; line+=2; }
      else if (*line == '>' && *(line+1) == '>') { op = OP_SHR; line+=2; }
      else if (*line == '>') { op = OP_GT; line++; }
      else if (*line == ')') { op = OP_CP; line++; }
      else if (*line == ',') op = OP_END;
      else if (*line == ']') op = OP_END;
      else if (*line == 0) op = OP_END;
      else op = 0;
      if (op == 0) {
        throw InvalidOpException("Expression error");
        }
      while (osp > 0 && (op & 0xf0) <= (ops[osp-1] & 0xf0)) {
        if (nsp > 0 && ops[osp-1] == OP_ABS) {
          if (numbers[nsp-1] < 0) numbers[nsp-1] = -numbers[nsp-1];
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_CHS) {
          numbers[nsp-1] = -numbers[nsp-1];
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_SGN) {
          if (numbers[nsp-1] < 0) numbers[nsp-1] = -1;
          if (numbers[nsp-1] > 0) numbers[nsp-1] = 1;
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_SQR) {
          numbers[nsp-1] = numbers[nsp-1] * numbers[nsp-1];
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_SQRT) {
          numbers[nsp-1] = sqrt(numbers[nsp-1]);
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_COS) {
          if (trigMode == 'R') numbers[nsp-1] = cos(numbers[nsp-1]);
          if (trigMode == 'D') numbers[nsp-1] = cos(numbers[nsp-1]*M_PI/180.0);
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_SIN) {
          if (trigMode == 'R') numbers[nsp-1] = sin(numbers[nsp-1]);
          if (trigMode == 'D') numbers[nsp-1] = sin(numbers[nsp-1]*M_PI/180.0);
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_TAN) {
          if (trigMode == 'R') numbers[nsp-1] = tan(numbers[nsp-1]);
          if (trigMode == 'D') numbers[nsp-1] = tan(numbers[nsp-1]*M_PI/180.0);
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_ACOS) {
          if (trigMode == 'R') numbers[nsp-1] = acos(numbers[nsp-1]);
          if (trigMode == 'D') numbers[nsp-1] = acos(numbers[nsp-1])*180.0/M_PI;
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_ASIN) {
          if (trigMode == 'R') numbers[nsp-1] = asin(numbers[nsp-1]);
          if (trigMode == 'D') numbers[nsp-1] = asin(numbers[nsp-1])*180.0/M_PI;
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_ATAN) {
          if (trigMode == 'R') numbers[nsp-1] = atan(numbers[nsp-1]);
          if (trigMode == 'D') numbers[nsp-1] = atan(numbers[nsp-1])*180.0/M_PI;
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_LN) {
          numbers[nsp-1] = log(numbers[nsp-1]);
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_LOG) {
          numbers[nsp-1] = log10(numbers[nsp-1]);
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_EXP) {
          numbers[nsp-1] = exp(numbers[nsp-1]);
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_INT) {
          numbers[nsp-1] = (int)numbers[nsp-1];
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_FRAC) {
          v = numbers[nsp-1];
          if (v < 0) {
            v = -v;
            v -= (int)v;
            v = -v;
            }
          else {
            v -= (int)v;
            }
          numbers[nsp-1] = v;
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_HI) {
          numbers[nsp - 1] = (int)(numbers[nsp -1] / 256.0);
          osp--;
          }
        else if (nsp > 0 && ops[osp-1] == OP_LO) {
          numbers[nsp - 1] = (int)numbers[nsp -1] & 0xff;
          osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_ADD) {
          numbers[nsp - 2] += numbers[nsp -1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_SUB) {
          numbers[nsp - 2] -= numbers[nsp -1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_MUL) {
          numbers[nsp - 2] *= numbers[nsp -1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_MOD) {
          numbers[nsp-2] = (int)numbers[nsp-2] % (int)numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_DIV) {
          numbers[nsp - 2] /= numbers[nsp -1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_POW) {
          numbers[nsp - 2] = pow(numbers[nsp-2], numbers[nsp-1]);
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_AND) {
          numbers[nsp-2] = (int)numbers[nsp-2] & (int)numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_OR) {
          numbers[nsp-2] = (int)numbers[nsp-2] | (int)numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_LAND) {
          numbers[nsp-2] = (int)numbers[nsp-2] && (int)numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_LOR) {
          numbers[nsp-2] = (int)numbers[nsp-2] || (int)numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_XOR) {
          numbers[nsp-2] = (int)numbers[nsp-2] ^ (int)numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_SHL) {
          numbers[nsp-2] = (int)numbers[nsp-2] << (int)numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_SHR) {
          numbers[nsp-2] = (int)numbers[nsp-2] >> (int)numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_EQ) {
          numbers[nsp-2] = numbers[nsp-2] == numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_NE) {
          numbers[nsp-2] = numbers[nsp-2] != numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_LT) {
          numbers[nsp-2] = numbers[nsp-2] < numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_LTE) {
          numbers[nsp-2] = numbers[nsp-2] <= numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_GT) {
          numbers[nsp-2] = numbers[nsp-2] > numbers[nsp-1];
          nsp--; osp--;
          }
        else if (nsp > 1 && ops[osp-1] == OP_GTE) {
          numbers[nsp-2] = numbers[nsp-2] >= numbers[nsp-1];
          nsp--; osp--;
          }
        else if (op == OP_CP && ops[osp-1] == OP_OP) {
          osp--;
          op = 0xff;
          flag = -1;
          }
        }
      if (op != 0xff) ops[osp++] = op;
      while (*line == ' ' || *line == '\t') line++;
      }
    }
  if (nsp != 1) {
    throw InvalidOpException("Expression did not reduce to single term");
    }
  return numbers[0];
  }

Double Calculator::GetVar(const char* name) {
  Int32 v;
  v = _findVariable(name);
  if (v < 0) throw NotFoundException("Variable not found");
  return variableValues[v];
  }

void Calculator::SetVar(const char* name, Double value) {
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

