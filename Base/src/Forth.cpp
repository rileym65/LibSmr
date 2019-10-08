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
#include <stdio.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

#define CMD_DUP        0x80000001
#define CMD_DROP       0x80000002
#define CMD_CLEAR      0x80000003
#define CMD_SWAP       0x80000004
#define CMD_DEPTH      0x80000005
#define CMD_TRUE       0x80000006
#define CMD_FALSE      0x80000007
#define CMD_NEG        0x80000008
#define CMD_PLUS       0x80000009
#define CMD_MINUS      0x8000000a
#define CMD_MUL        0x8000000b
#define CMD_DIV        0x8000000c
#define CMD_EQ         0x8000000d
#define CMD_NEQ        0x8000000e
#define CMD_GT         0x8000000f
#define CMD_LT         0x80000010
#define CMD_GTE        0x80000011
#define CMD_LTE        0x80000012
#define CMD_IF         0x80000013
#define CMD_THEN       0x80000014
#define CMD_ELSE       0x80000015
#define CMD_DO         0x80000016
#define CMD_LOOP       0x80000017
#define CMD_I          0x80000018
#define CMD_PLOOP      0x80000019
#define CMD_BEGIN      0x8000001a
#define CMD_UNTIL      0x8000001b
#define CMD_WHILE      0x8000001c
#define CMD_REPEAT     0x8000001d
#define CMD_WRITE      0x8000001e
#define CMD_READ       0x8000001f
#define CMD_EMIT       0x80000020
#define CMD_ROT        0x80000021
#define CMD_MROT       0x80000022
#define CMD_OVER       0x80000023
#define CMD_AUG        0x80000024
#define CMD_DIM        0x80000025
#define CMD_DOT        0x80000026
#define CMD_ABS        0x80000027
#define CMD_LBR        0x80000028
#define CMD_RBR        0x80000029
#define CMD_PLMAX      0x8000002a
#define CMD_MIMIN      0x8000002b
#define CMD_PLMIN      0x8000002c
#define CMD_MIMAX      0x8000002d
#define CMD_PICK       0x8000002e
#define CMD_NIP        0x8000002f
#define CMD_TUCK       0x80000030
#define CMD_AND        0x80000031
#define CMD_OR         0x80000032
#define CMD_XOR        0x80000033
#define CMD_NOT        0x80000034
#define CMD_EQZERO     0x80000035
#define CMD_GTZERO     0x80000036
#define CMD_LTZERO     0x80000037
#define CMD_ONEPLUS    0x80000038
#define CMD_ONEMINUS   0x80000039
#define CMD_BL         0x8000003a
#define CMD_CLEARTO    0x8000003b
#define CMD_END        0x8000003c
#define CMD_HOST       0x8000003d
#define CMD_INVERT     0x8000003e
#define CMD_NDROP      0x8000003f
#define CMD_SGN        0x80000040
#define CMD_RGT        0x80000041
#define CMD_GTR        0x80000042
#define CMD_PLUSSTORE  0x80000043
#define CMD_MINUSSTORE 0x80000044
#define CMD_MINUSZERO  0x80000045
#define CMD_QDUP       0x80000046
#define CMD_SDOT       0x80000047
#define CMD_SDROP      0x80000048
#define CMD_LEFT       0x80000049
#define CMD_RIGHT      0x8000004a
#define CMD_MID        0x8000004b
#define CMD_SDUP       0x8000004c
#define CMD_LEN        0x8000004d

typedef struct {
  const char*  command;
  UInt32 token;
  } _FORTH_CMDS;

_FORTH_CMDS _forth_cmds[] = {
  { ".", CMD_DOT },
  { "!", CMD_WRITE },
  { "@", CMD_READ },
  { "+", CMD_PLUS },
  { "-", CMD_MINUS },
  { "*", CMD_MUL },
  { "/", CMD_DIV },
  { "=", CMD_EQ },
  { "<>", CMD_NEQ },
  { ">", CMD_GT },
  { "<", CMD_LT },
  { ">=", CMD_GTE },
  { "<=", CMD_LTE },
  { "{", CMD_LBR },
  { "}", CMD_RBR },
  { "=0", CMD_EQZERO },
  { ">0", CMD_GTZERO },
  { "<0", CMD_LTZERO },
  { "1+", CMD_ONEPLUS },
  { "1-", CMD_ONEMINUS },
  { "+!", CMD_PLUSSTORE },
  { "-!", CMD_MINUSSTORE },
  { "-?0", CMD_MINUSZERO },
  { "+max", CMD_PLMAX },
  { "+min", CMD_PLMIN },
  { "-max", CMD_MIMAX },
  { "-min", CMD_MIMIN },
  { "abs", CMD_ABS },
  { "and", CMD_AND },
  { "aug", CMD_AUG },
  { "begin", CMD_BEGIN },
  { "bl", CMD_BL },
  { "clear", CMD_CLEAR },
  { "clearto", CMD_CLEARTO },
  { "depth", CMD_DEPTH },
  { "dim", CMD_DIM },
  { "do", CMD_DO },
  { "dup", CMD_DUP },
  { "?dup", CMD_QDUP },
  { "drop", CMD_DROP },
  { "else", CMD_ELSE },
  { "emit", CMD_EMIT },
  { "end", CMD_END },
  { "false", CMD_FALSE },
  { "host", CMD_HOST },
  { "i", CMD_I },
  { "if", CMD_IF },
  { "invert", CMD_INVERT },
  { "left", CMD_LEFT },
  { "len", CMD_LEN },
  { "loop", CMD_LOOP },
  { "+loop", CMD_PLOOP },
  { "mid", CMD_MID },
  { "ndrop", CMD_NDROP },
  { "neg", CMD_NEG },
  { "nip", CMD_NIP },
  { "not", CMD_NOT },
  { "or", CMD_OR },
  { "over", CMD_OVER },
  { "pick", CMD_PICK },
  { "r>", CMD_RGT },
  { ">r", CMD_GTR },
  { "repeat", CMD_REPEAT },
  { "right", CMD_RIGHT },
  { "rot", CMD_ROT },
  { "-rot", CMD_MROT },
  { "s.", CMD_SDOT },
  { "sdrop", CMD_SDROP },
  { "sdup", CMD_SDUP },
  { "sgn", CMD_SGN },
  { "swap", CMD_SWAP },
  { "then", CMD_THEN },
  { "true", CMD_TRUE },
  { "tuck", CMD_TUCK },
  { "until", CMD_UNTIL },
  { "while", CMD_WHILE },
  { "xor", CMD_XOR },
  { "__END__", 0 },
  };

  Forth::Forth(ForthContext* c) {
    context = c;
    rstack = new Stack<UInt32>();
    objectType = (char*)"Forth";
    functions = NULL;
    numFunctions = 0;
    variables = NULL;
    variableLocations = NULL;
    numVariables = 0;
    }

  Forth::~Forth() {
    UInt32 i;
    delete(rstack);
    if (functions != NULL) {
      for (i=0; i<numFunctions; i++) delete(functions[i]);
      free(functions);
      }
    if (variables != NULL) {
      for (i=0; i<numVariables; i++) free(variables[i]);
      free(variables);
      }
    if (variableLocations != NULL) free(variableLocations);
    }

  UInt32 Forth::findToken(const char* token) {
    UInt32 i;
    for (i=0; i<numFunctions; i++)
      if (strcmp(token,functions[i]->GetName()) == 0) return 0x81000000+i;
    for (i=0; i<numVariables; i++)
      if (strcmp(token,variables[i]) == 0) return variableLocations[i];
    i = 0;
    while (_forth_cmds[i].token != 0) {
      if (strcmp(token, _forth_cmds[i].command) == 0) return _forth_cmds[i].token;
      i++;
      }
    return 0xffffffff;
    }

  char* Forth::PopString() {
    int   size;
    int   pos;
    char* ret;
    if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
    size = 0;
    while (context->At(size) != 0) {
      size++;
      if (size >= (int)context->Count()) throw InvalidOpException(this, "Stack empty");
      }
    ret = (char*)malloc(size+1);
    pos = 0;
    while (size >= 0) {
      ret[pos++] = context->Pop();
      size--;
      }
    return ret;
    }

  void Forth::PushString(const char* str) {
    int i;
    context->Push(0);
    for (i=strlen(str)-1; i>=0; i--)
      context->Push(str[i]);
    }

  ForthContext* Forth::GetContext() {
    return context;
    }

  UInt32 Forth::Exec(ForthProgram* program) {
    UInt32 a,b,c;
    Int32  ivalue;
    Int32  ia,ib,ic;
    UInt32 token;
    UInt32 pos;
    Int32  count;
    char   buffer[1024];
    char*  pstr;
    char*  pstr1;
    program->Reset();
    rstack->Clear();
    while ((token = program->Next()) != 0xffffffff) {
      if ((token &0xff000000) == 0x81000000) functions[token & 0xffffff]->Exec();
      else switch (token) {
        case CMD_ABS:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             context->Push((ia >= 0) ? ia : -ia);
             break;
        case CMD_AND:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push(ia & ib);
             break;
        case CMD_AUG:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             if (ia > 0) ia++;
             else if (ia < 0) ia--;
             context->Push(ia);
             break;
        case CMD_BEGIN:
             pos = program->GetPos();
             rstack->Push(pos);
             break;
        case CMD_BL:
             context->Push(32);
             break;
        case CMD_CLEAR:
             context->Clear();
             break;
        case CMD_CLEARTO:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = -ia;
             while (ia != ib) {
               if (context->Count() > 0) ib = context->Pop();
                 else ib = ia;
               }
             break;
        case CMD_DEPTH:
             context->Push(context->Count());
             break;
        case CMD_DIM:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             if (ia > 0) ia--;
             else if (ia < 0) ia++;
             context->Push(ia);
             break;
        case CMD_DIV:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push(ib/ia);
             break;
        case CMD_DO:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ib = context->Pop();
             ia = context->Pop();
             pos = program->GetPos();
             if (ia != ib) {
               rstack->Push(pos);
               rstack->Push(ia);
               rstack->Push(ib);
               }
             else {
               count = 1;
               while (count != 0) {
                 ia = program->At(pos);
                 pos++;
                 if (ia == (int)0xffffffff) throw InvalidOpException(this, "No matching loop");
                 if (ia == (int)CMD_DO) count++;
                 if (ia == (int)CMD_LOOP) count--;
                 }
               program->SetPos(pos);
               }
             break;
        case CMD_DOT:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             sprintf(buffer,"%d",ia);
             context->Output(buffer);
             break;
        case CMD_DROP:
             if (context->Count() == 0) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             break;
        case CMD_DUP:
             if (context->Count() == 0) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             context->Push(a);
             context->Push(a);
             break;
        case CMD_ELSE:
             pos = program->GetPos();
             count = 1;
             while (count != 0) {
               ia = program->At(pos);
               pos++;
               if (ia == (int)0xffffffff) throw InvalidOpException(this, "No matching then");
               if (ia == (int)CMD_IF) count++;
               if (ia == (int)CMD_THEN) count--;
               }
             program->SetPos(pos);
             break;
        case CMD_EMIT:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             buffer[0] = (a & 0xff);
             buffer[1] = 0;
             context->Output(buffer);
             break;
        case CMD_END:
             return 0;
        case CMD_EQ:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push((ia == ib) ? -1 : 0);
             break;
        case CMD_EQZERO:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             context->Push((ia == 0) ? 0xffffffff : 0);
             break;
        case CMD_FALSE:
             context->Push(0x00000000);
             break;
        case CMD_GT:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push((ib > ia) ? -1 : 0);
             break;
        case CMD_GTE:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push((ib >= ia) ? -1 : 0);
             break;
        case CMD_GTR:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             rstack->Push(a);
             break;
        case CMD_GTZERO:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             context->Push((ia > 0) ? 0xffffffff : 0);
             break;
        case CMD_HOST:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             context->Function(a);
             break;
        case CMD_I:
             if (rstack->Count() < 1) throw InvalidOpException(this, "Rstack empty");
             ia = rstack->Peek();
             context->Push(ia);
             break;
        case CMD_IF:
             if (context->Count() == 0) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             if (ia == 0) {
               pos = program->GetPos();
               count = 1;
               while (count != 0) {
                 ia = program->At(pos);
                 pos++;
                 if (ia == (int)0xffffffff) throw InvalidOpException(this, "No matching then");
                 if (ia == (int)CMD_IF) count++;
                 if (ia == (int)CMD_THEN) count--;
                 if (ia == (int)CMD_ELSE) count--;
                 }
               program->SetPos(pos);
               }
             break;
        case CMD_INVERT:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             a = a ^ 0xffffffff;
             context->Push(a);
             break;
        case CMD_LBR:
             context->Push(0x80000000);
             break;
        case CMD_LEFT:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             pstr = PopString();
             if (strlen(pstr) <= a) {
               PushString(pstr);
               free(pstr);
               }
             else {
               pstr1 = (char*)malloc(strlen(pstr) + 1);
               strncpy(pstr1, pstr, a);
               pstr1[a] = 0;
               PushString(pstr1);
               free(pstr);
               free(pstr1);
               }
             break;
        case CMD_LEN:
             a = context->Count();
             if (a < 1) throw InvalidOpException(this, "Rstack empty");
             b = 0;
             ia = -1;
             while (b < a) {
               if (context->At(b) == 0) {
                 ia = b;
                 b = a;
                 }
               b++;
               }
             context->Push(ia);
             break;
        case CMD_LOOP:
             if (rstack->Count() < 3) throw InvalidOpException(this, "Rstack empty");
             ib = rstack->Pop();
             ia = rstack->Pop();
             pos = rstack->Pop();
             if (ib < ia) ib++; else ib--;
             if (ib != ia) {
               rstack->Push(pos);
               rstack->Push(ia);
               rstack->Push(ib);
               program->SetPos(pos);
               }
             break;
        case CMD_PLOOP:
             if (rstack->Count() < 3) throw InvalidOpException(this, "Rstack empty");
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ib = rstack->Pop();
             ia = rstack->Pop();
             pos = rstack->Pop();
             ic = context->Pop();
             if (ib < ia) {
               ib += ic;
               if (ib < ia) {
                 rstack->Push(pos);
                 rstack->Push(ia);
                 rstack->Push(ib);
                 program->SetPos(pos);
                 }
               }
             else if (ib > ia) {
               ib += ic;
               if (ib > ia) {
                 rstack->Push(pos);
                 rstack->Push(ia);
                 rstack->Push(ib);
                 program->SetPos(pos);
                 }
               }
             break;
        case CMD_LT:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push((ib < ia) ? -1 : 0);
             break;
        case CMD_LTE:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push((ib <= ia) ? -1 : 0);
             break;
        case CMD_LTZERO:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             context->Push((ia < 0) ? 0xffffffff : 0);
             break;
        case CMD_MID:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             b = context->Pop();
             pstr = PopString();
             if (strlen(pstr) <= a+b) {
               PushString(pstr);
               free(pstr);
               }
             else {
               pstr1 = (char*)malloc(strlen(pstr) + 1);
               strncpy(pstr1, pstr+b, a);
               pstr1[a] = 0;
               PushString(pstr1);
               free(pstr);
               free(pstr1);
               }
             break;
        case CMD_MIMAX:
             if (context->Count() < 3) throw InvalidOpException(this, "Stack empty");
             ic = context->Pop();
             ib = context->Pop();
             ia = context->Pop();
             ia -= ib;
             if (ia > ic) ia = ic;
             context->Push(ia);
             break;
        case CMD_MIMIN:
             if (context->Count() < 3) throw InvalidOpException(this, "Stack empty");
             ic = context->Pop();
             ib = context->Pop();
             ia = context->Pop();
             ia -= ib;
             if (ia < ic) ia = ic;
             context->Push(ia);
             break;
        case CMD_MINUS:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push(ib-ia);
             break;
        case CMD_MINUSSTORE:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             ia = context->Pop();
             ib = context->Read(a);
             ib -= ia;
             context->Write(a,ib);
             break;
        case CMD_MINUSZERO:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             ib -= ia;
             if (ib < 0) ib = 0;
             context->Push(ib);
             break;
        case CMD_MROT:
             if (context->Count() < 3) throw InvalidOpException(this, "Stack empty");
             c = context->Pop();
             b = context->Pop();
             a = context->Pop();
             context->Push(c);
             context->Push(a);
             context->Push(b);
             break;
        case CMD_MUL:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push(ib*ia);
             break;
        case CMD_NDROP:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             if (ia < 0) throw InvalidOpException(this, "Invalid value");
             if (ia >= (int)context->Count()) throw InvalidOpException(this, "Stack empty");
             for (a=0; (int)a<ia; a++) b = context->Pop();
             break;
        case CMD_NOT:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             context->Push((ia == 0) ? 0xffffffff : 0);
             break;
        case CMD_ONEMINUS:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             context->Push(ia-1);
             break;
        case CMD_ONEPLUS:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             context->Push(ia+1);
             break;
        case CMD_OR:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push(ia | ib);
             break;
        case CMD_OVER:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             b = context->Pop();
             a = context->Pop();
             context->Push(a);
             context->Push(b);
             context->Push(a);
             break;
        case CMD_PICK:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             if (a < 0) throw InvalidOpException(this, "Invalid value");
             if (a >= context->Count()) throw InvalidOpException(this, "Stack empty");
             ia = context->At(a);
             context->Push(ia);
             break;
        case CMD_NEG:
             if (context->Count() == 0) throw InvalidOpException(this, "Stack empty");
             ivalue = context->Pop();
             ivalue = -ivalue;
             context->Push(ivalue);
             break;
        case CMD_NEQ:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push((ia != ib) ? -1 : 0);
             break;
        case CMD_NIP:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             b = context->Pop();
             context->Push(a);
             break;
        case CMD_QDUP:
             if (context->Count() == 0) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             context->Push(a);
             if (a != 0) context->Push(a);
             break;
        case CMD_RBR:
             a = 0;
             while (a != 0x80000000) {
               if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
               a = context->Pop();
               }
             break;
        case CMD_RGT:
             if (rstack->Count() < 1) throw InvalidOpException(this, "Rstack empty");
             a = rstack->Pop();
             context->Push(a);
             break;
        case CMD_READ:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             context->Push(context->Read(a));
             break;
        case CMD_REPEAT:
             if (rstack->Count() < 1) throw InvalidOpException(this, "Rstack empty");
             pos = rstack->Pop();
             program->SetPos(pos);
             break;
        case CMD_RIGHT:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             pstr = PopString();
             if (strlen(pstr) <= a) {
               PushString(pstr);
               free(pstr);
               }
             else {
               pstr1 = (char*)malloc(strlen(pstr) + 1);
               strncpy(pstr1, pstr+(strlen(pstr)-a), a);
               pstr1[a] = 0;
               PushString(pstr1);
               free(pstr);
               free(pstr1);
               }
             break;
        case CMD_ROT:
             if (context->Count() < 3) throw InvalidOpException(this, "Stack empty");
             c = context->Pop();
             b = context->Pop();
             a = context->Pop();
             context->Push(b);
             context->Push(c);
             context->Push(a);
             break;
        case CMD_SDOT:
             pstr = PopString();
             context->Output(pstr);
             free(pstr);
             break;
        case CMD_SDROP:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             a = 0;
             b = 99;
             while (b != 0) {
               if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
               b = context->Pop();
               }
             break;
        case CMD_SDUP:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             pstr = PopString();
             PushString(pstr);
             PushString(pstr);
             free(pstr);
             break;
        case CMD_SWAP:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             b = context->Pop();
             context->Push(a);
             context->Push(b);
             break;
        case CMD_PLMAX:
             if (context->Count() < 3) throw InvalidOpException(this, "Stack empty");
             ic = context->Pop();
             ia = context->Pop();
             ib = context->Pop();
             ia += ib;
             if (ia > ic) ia = ic;
             context->Push(ia);
             break;
        case CMD_PLMIN:
             if (context->Count() < 3) throw InvalidOpException(this, "Stack empty");
             ic = context->Pop();
             ia = context->Pop();
             ib = context->Pop();
             ia += ib;
             if (ia < ic) ia = ic;
             context->Push(ia);
             break;
        case CMD_PLUS:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push(ia+ib);
             break;
        case CMD_PLUSSTORE:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             a = context->Pop();
             ia = context->Pop();
             ib = context->Read(a);
             ia += ib;
             context->Write(a,ia);
             break;
        case CMD_SGN:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             if (ia < 0) context->Push(-1);
             else if (ia > 0) context->Push(1);
             else context->Push(0);
             break;
        case CMD_THEN:
             break;
        case CMD_TRUE:
             context->Push(0xffffffff);
             break;
        case CMD_TUCK:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             c = context->Pop();
             b = context->Pop();
             context->Push(c);
             context->Push(b);
             context->Push(c);
             break;
        case CMD_UNTIL:
             if (rstack->Count() < 1) throw InvalidOpException(this, "Rstack empty");
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             pos = rstack->Pop();
             ia = context->Pop();
             if (ia == 0) {
               rstack->Push(pos);
               program->SetPos(pos);
               }
             break;
        case CMD_WHILE:
             if (context->Count() < 1) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             if (ia != 0) {
               pos = program->GetPos() - 1;
               rstack->Push(pos);
               }
             else {
               count = 1;
               pos = program->GetPos();
               while (count != 0) {
                 ia = program->At(pos);
                 pos++;
                 if (ia == (int)0xffffffff) throw InvalidOpException(this, "No matching then");
                 if (ia == (int)CMD_WHILE) count++;
                 if (ia == (int)CMD_REPEAT) count--;
                 }
               program->SetPos(pos);
               }
             break;
        case CMD_WRITE:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             b = context->Pop();
             a = context->Pop();
             context->Write(b, a);
             break;
        case CMD_XOR:
             if (context->Count() < 2) throw InvalidOpException(this, "Stack empty");
             ia = context->Pop();
             ib = context->Pop();
             context->Push(ia ^ ib);
             break;
        default:
             if (token < 0x80000000) context->Push(token);
             else throw InvalidOpException(this, "Invalid token in command stream");
             break;
        }
      }

    return 0;
    }

  UInt32 Forth::Variable(const char* name, UInt32 pos) {
    UInt32 i;
    for (i=0; i<numVariables; i++)
      if (strcmp(name, variables[i]) == 0) {
        variableLocations[i] = pos;
        return pos;
        }
    numVariables++;
    if (numVariables == 1) {
      variables = (char**)malloc(sizeof(char*));
      variableLocations = (UInt32*)malloc(sizeof(UInt32));
      }
    else {
      variables = (char**)realloc(variables,sizeof(char*)*numVariables);
      variableLocations = (UInt32*)realloc(variableLocations,sizeof(UInt32)*numVariables);
      }
    variables[numVariables-1] = (char*)malloc(strlen(name) + 1);
    strcpy(variables[numVariables-1], name);
    variableLocations[numVariables-1] = pos;
    return pos;
    }

  UInt32 Forth::Variable(const char* name) {
    UInt32 i;
    bool   found;
    UInt32 pos;
    for (i=0; i<numVariables; i++)
      if (strcmp(name, variables[i]) == 0)
        return variableLocations[i];
    found = false;
    pos = 0;
    while (found == false) {
      found = true;
      for (i=0; i<numVariables; i++)
        if (variableLocations[i] == pos) {
          found = false;
          i = numVariables;
          }
      }
    return Variable(name, pos);
    }

  ForthProgram* Forth::Parse(const char* text) {
    Int32  ivalue;
    char   token[256];
    char   temp[1024];
    char*  pToken;
    UInt32 tok;
    bool   def;
    bool   named;
    bool   quote;
    ForthProgram* program;
    program = new ForthProgram(this);
    def = false;
    while (*text > 0 && *text <= 32) text++;
    while (*text != 0) {
      pToken = token;
      quote = false;
      while (*text > 32 || quote) {
        if (*text == '"') quote = !quote;
        *pToken++ = *text++;
        if (*text == 0 && quote)  throw InvalidOpException(this, "String not terminated");
        }
      *pToken = 0;
      while (*text > 0 && *text <= 32) text++;
      if (token[0] != 0) {
        if (Tests::IsInteger(token)) {
          ivalue = Convert::ToInt32(token);
          if (ivalue >= 0) {
            program->Add(ivalue);
            } else {
            program->Add(-ivalue);
            program->Add(CMD_NEG);
            }
          }
        else if (token[0] == '"' && token[strlen(token)-1] == '"') {
          program->Add(0);
          for (ivalue = strlen(token)-2; ivalue > 0; ivalue--) {
            program->Add(token[ivalue]);
            }
          }
        else if (strcmp(token,":") == 0) {
          if (def) {
            delete(program);
            throw InvalidOpException(this, ": inside def is not allowed");
            }
          def = true;
          named = false;
          }
        else if (strcmp(token,";") == 0) {
          if (!def) {
            delete(program);
            throw InvalidOpException(this, "; used outside def");
            }
          if (!named) {
            delete(program);
            throw InvalidOpException(this, "null function not allowed");
            }
          numFunctions++;
          if (numFunctions == 1)
            functions = (ForthProgram**)malloc(sizeof(ForthProgram*));
          else
            functions = (ForthProgram**)realloc(functions,sizeof(ForthProgram*)*numFunctions);
          functions[numFunctions-1] = program;
          return program;
          }
        else if (def && !named) {
          program->SetName(token);
          named = true;
          }
        else {
          tok = findToken(token);
          if (tok == 0xffffffff) {
            delete(program);
            sprintf(temp,"Invalid symbol found: %s",token);
            throw InvalidOpException(this, temp);
            }
          program->Add(tok);
          }
        }
      }
    return program;
    }

  }

