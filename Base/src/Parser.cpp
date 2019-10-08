/*
 *******************************************************************
 *** This software is copyright 1985-2014 by Michael H Riley     ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/


#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Parser::Parser() {
    objectType = (char*)"Parser";
    lex = new Lexer();
    ownsLexer = true;
    init();
    }

  Parser::Parser(Lexer* l) {
    objectType = (char*)"Parser";
    lex = l;
    ownsLexer = false;
    init();
    }
  
  Parser::~Parser() {
    if (ownsLexer) delete(lex);
    }
  
  void Parser::init() {
    nextToken = 0x100;
    tokens = new List<String*>();
    tokenValues = new List<UInt32>();
    }

  UInt32 Parser::AddToken(String name, UInt32 token) {
    if (token == 0) token = nextToken++;
    else {
      if (token < nextToken) throw InvalidOpException(this, "Invalid token value");
      nextToken = token + 1;
      }
    tokens->Add(new String(name));
    tokenValues->Add(token);
    return token;
    }

  UInt32 Parser::AddToken(String name, String pattern, UInt32 token) {
    if (token == 0) token = nextToken++;
    else {
      if (token < nextToken) throw InvalidOpException(this, "Invalid token value");
      nextToken = token + 1;
      }
    lex->AddPattern(pattern.AsCharArray(), token);
    tokens->Add(new String(name));
    tokenValues->Add(token);
    return token;
    }

  Lexer* Parser::Lex() {
    return lex;
    }

  Lexer* Parser::Lex(Lexer* l) {
    if (ownsLexer) delete(lex);
    lex = l;
    ownsLexer = false;
    return lex;
    }

  UInt32 Parser::LookupToken(String t) {
    UInt32 i;
    for (i=0; i<tokens->Count(); i++)
      if (t.Equals(tokens->At(i))) return tokenValues->At(i);
    throw InvalidOpException(this, "Token not found");
    }

  String* Parser::LookupToken(UInt32 t) {
    UInt32 i;
    for (i=0; i<tokens->Count(); i++)
      if (tokenValues->At(i) == t) return tokens->At(i);
    throw InvalidOpException(this, "Token not found");
    }

  }

