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
#include <termios.h>
#include <unistd.h>
#include "string.h"
#include "SmrFramework.h"

namespace SmrFramework {

  Terminal::Terminal() {
    objectType = (char*)"Terminal";
    tcgetattr(0, &original);
    bold = false;
    inverse = false;
    underline = false;
    foreground = Black;
    background = White;
    width = 80;
    buffered = true;
    cursorShown = true;
    echo = true;
    }
  
  Terminal::~Terminal() {
    tcsetattr(0, TCSANOW, &original);
    }
  
  void Terminal::setAttributes() {
    printf("\e[0m");
    if (bold) printf("\e[1m");
    if (underline) printf("\e[4m");
    if (inverse) printf("\e[7m");
    printf("\e[%dm",30+foreground);
    printf("\e[%dm",40+background);
    fflush(stdout);
    }

  void Terminal::Background(UInt32 i) {
    if (i<0 || i > 7) return;
    background = i;
    setAttributes();
    }

  void Terminal::Bold(bool b) {
    bold = b;
    setAttributes();
    }

  void Terminal::Box(UInt32 x, UInt32 y, UInt32 w, UInt32 h) {
    UInt32 i,j;
    Position(x,y); Write("+");
    for (i=0; i<w-2; i++)
      Write("-");
    Write("+");
    for (j=0; j<h-2; j++) {
      Position(x,y+j+1); Write("|");
      for (i=0; i<w-2; i++)
        Write(" ");
      Write("|");
      }
    Position(x,y+h-1); Write("+");
    for (i=0; i<w-2; i++)
      Write("-");
    Write("+");
    Flush();
    }
 
  void Terminal::Buffered(bool b) {
    struct termios terminal;
    tcgetattr(0, &terminal);
    if (b) {
      terminal.c_lflag |= ICANON;
      }
    else {
      terminal.c_lflag &= ~ICANON;
      }
    tcsetattr(0, TCSANOW, &terminal);
    buffered = b;
    }

  void Terminal::Center(const char* msg) {
    UInt32 i;
    UInt32 len;
    len = (width / 2) - (strlen(msg) / 2);
    for (i=0; i<len; i++) printf(" ");
    printf("%s",msg);
    }

  void Terminal::Center(String* msg) {
    Center(msg->AsCharArray());
    }

  void Terminal::ClearScreen() {
    printf("\E[H\E[2J");
    fflush(stdout);
    }

  void Terminal::Echo(bool b) {
    struct termios terminal;
    tcgetattr(0, &terminal);
    if (b) {
      terminal.c_lflag |= ECHO;
      }
    else {
      terminal.c_lflag &= ~ECHO;
      }
    tcsetattr(0, TCSANOW, &terminal);
    echo = b;
    }

  void Terminal::Flush() {
    fflush(stdout);
    }

  void Terminal::Foreground(UInt32 i) {
    if (i<0 || i > 7) return;
    foreground = i;
    setAttributes();
    }

  UInt32 Terminal::Inkey() {
    int  esc_mode;
    char esc_seq[80];
    int           c;
    char          key;
    c = 0;
    esc_mode = 0;
    while (c == 0) {
      read(0,&key,1);
      c = key;
      if (esc_mode > 0) {
        esc_seq[esc_mode-1] = c;
        esc_seq[esc_mode] = 0;
        esc_mode++;
        if (c>='A' && c<='Z' && c != 'O') esc_mode = 0;
        if (c>='a' && c<='~') esc_mode = 0;
        if (esc_mode > 10) esc_mode = 0;
        c = 0;
        if (esc_mode == 0) {
          if (strcmp(esc_seq,"[2~") == 0)    c = Key_Insert;
          if (strcmp(esc_seq,"[2;5~") == 0)  c = Key_Ctrl_Insert;
          if (strcmp(esc_seq,"[2;3~") == 0)  c = Key_Alt_Insert;
          if (strcmp(esc_seq,"[2;2~") == 0)  c = Key_Shift_Insert;
          if (strcmp(esc_seq,"[3~") == 0)    c = Key_Delete;
          if (strcmp(esc_seq,"[3;5~") == 0)  c = Key_Ctrl_Delete;
          if (strcmp(esc_seq,"[3;3~") == 0)  c = Key_Alt_Delete;
          if (strcmp(esc_seq,"[3;2~") == 0)  c = Key_Shift_Delete;
          if (strcmp(esc_seq,"[H") == 0)     c = Key_Home;
          if (strcmp(esc_seq,"[1;5H") == 0)  c = Key_Ctrl_Home;
          if (strcmp(esc_seq,"[1;3H") == 0)  c = Key_Alt_Home;
          if (strcmp(esc_seq,"[1;2H") == 0)  c = Key_Shift_Home;
          if (strcmp(esc_seq,"[F") == 0)     c = Key_End;
          if (strcmp(esc_seq,"[1;5F") == 0)  c = Key_Ctrl_End;
          if (strcmp(esc_seq,"[1;3F") == 0)  c = Key_Alt_End;
          if (strcmp(esc_seq,"[1;2F") == 0)  c = Key_Shift_End;
          if (strcmp(esc_seq,"[5~") == 0)    c = Key_PageUp;
          if (strcmp(esc_seq,"[5;5~") == 0)  c = Key_Ctrl_PageUp;
          if (strcmp(esc_seq,"[5;3~") == 0)  c = Key_Alt_PageUp;
          if (strcmp(esc_seq,"[5;2~") == 0)  c = Key_Shift_PageUp;
          if (strcmp(esc_seq,"[6~") == 0)    c = Key_PageDn;
          if (strcmp(esc_seq,"[6;5~") == 0)  c = Key_Ctrl_PageDn;
          if (strcmp(esc_seq,"[6;3~") == 0)  c = Key_Alt_PageDn;
          if (strcmp(esc_seq,"[6;2~") == 0)  c = Key_Shift_PageDn;
          if (strcmp(esc_seq,"[A") == 0)     c = Key_UpArrow;
          if (strcmp(esc_seq,"[1;5A") == 0)  c = Key_Ctrl_UpArrow;
          if (strcmp(esc_seq,"[1;3A") == 0)  c = Key_Alt_UpArrow;
          if (strcmp(esc_seq,"[1;2A") == 0)  c = Key_Shift_UpArrow;
          if (strcmp(esc_seq,"[B") == 0)     c = Key_DownArrow;
          if (strcmp(esc_seq,"[1;5B") == 0)  c = Key_Ctrl_DownArrow;
          if (strcmp(esc_seq,"[1;3B") == 0)  c = Key_Alt_DownArrow;
          if (strcmp(esc_seq,"[1;2B") == 0)  c = Key_Shift_DownArrow;
          if (strcmp(esc_seq,"[D") == 0)     c = Key_LeftArrow;
          if (strcmp(esc_seq,"[1;5D") == 0)  c = Key_Ctrl_LeftArrow;
          if (strcmp(esc_seq,"[1;3D") == 0)  c = Key_Alt_LeftArrow;
          if (strcmp(esc_seq,"[1;2D") == 0)  c = Key_Shift_LeftArrow;
          if (strcmp(esc_seq,"[C") == 0)     c = Key_RightArrow;
          if (strcmp(esc_seq,"[1;5C") == 0)  c = Key_Ctrl_RightArrow;
          if (strcmp(esc_seq,"[1;3C") == 0)  c = Key_Alt_RightArrow;
          if (strcmp(esc_seq,"[1;2C") == 0)  c = Key_Shift_RightArrow;
          if (strcmp(esc_seq,"[E") == 0)     c = Key_Keypad5;
          if (strcmp(esc_seq,"OP") == 0)     c = Key_F1;         /* F1 */
          if (strcmp(esc_seq,"OQ") == 0)     c = Key_F2;         /* F2 */
          if (strcmp(esc_seq,"OR") == 0)     c = Key_F3;         /* F3 */
          if (strcmp(esc_seq,"OS") == 0)     c = Key_F4;         /* F4 */
          if (strcmp(esc_seq,"[15~") == 0)   c = Key_F5;         /* F5 */
          if (strcmp(esc_seq,"[17~") == 0)   c = Key_F6;         /* F6 */
          if (strcmp(esc_seq,"[18~") == 0)   c = Key_F7;         /* F7 */
          if (strcmp(esc_seq,"[19~") == 0)   c = Key_F8;         /* F8 */
          if (strcmp(esc_seq,"[20~") == 0)   c = Key_F9;         /* F9 */
          if (strcmp(esc_seq,"[21~") == 0)   c = Key_F10;        /* F10 */
          if (strcmp(esc_seq,"[23~") == 0)   c = Key_F11;        /* F11 */
          if (strcmp(esc_seq,"[24~") == 0)   c = Key_F12;        /* F12 */
          if (strcmp(esc_seq,"[1;2P") == 0)  c = Key_Shift_F1;   /* Shift F1 */
          if (strcmp(esc_seq,"[1;2Q") == 0)  c = Key_Shift_F2;   /* Shift F2 */
          if (strcmp(esc_seq,"[1;2R") == 0)  c = Key_Shift_F3;   /* Shift F3 */
          if (strcmp(esc_seq,"[1;2S") == 0)  c = Key_Shift_F4;   /* Shift F4 */
          if (strcmp(esc_seq,"[15;2~") == 0) c = Key_Shift_F5;   /* Shift F5 */
          if (strcmp(esc_seq,"[17;2~") == 0) c = Key_Shift_F6;   /* Shift F6 */
          if (strcmp(esc_seq,"[18;2~") == 0) c = Key_Shift_F7;   /* Shift F7 */
          if (strcmp(esc_seq,"[19;2~") == 0) c = Key_Shift_F8;   /* Shift F8 */
          if (strcmp(esc_seq,"[20;2~") == 0) c = Key_Shift_F9;   /* Shift F9 */
          if (strcmp(esc_seq,"[21;2~") == 0) c = Key_Shift_F10;  /* Shift F10 */
          if (strcmp(esc_seq,"[23;2~") == 0) c = Key_Shift_F11;  /* Shift F11 */
          if (strcmp(esc_seq,"[24;2~") == 0) c = Key_Shift_F12;  /* Shift F12 */
          if (strcmp(esc_seq,"[1;5P") == 0)  c = Key_Ctrl_F1;    /* Ctrl F1 */
          if (strcmp(esc_seq,"[1;5Q") == 0)  c = Key_Ctrl_F2;    /* Ctrl F2 */
          if (strcmp(esc_seq,"[1;5R") == 0)  c = Key_Ctrl_F3;    /* Ctrl F3 */
          if (strcmp(esc_seq,"[1;5S") == 0)  c = Key_Ctrl_F4;    /* Ctrl F4 */
          if (strcmp(esc_seq,"[15;5~") == 0) c = Key_Ctrl_F5;    /* Ctrl F5 */
          if (strcmp(esc_seq,"[17;5~") == 0) c = Key_Ctrl_F6;    /* Ctrl F6 */
          if (strcmp(esc_seq,"[18;5~") == 0) c = Key_Ctrl_F7;    /* Ctrl F7 */
          if (strcmp(esc_seq,"[19;5~") == 0) c = Key_Ctrl_F8;    /* Ctrl F8 */
          if (strcmp(esc_seq,"[20;5~") == 0) c = Key_Ctrl_F9;    /* Ctrl F9 */
          if (strcmp(esc_seq,"[21;5~") == 0) c = Key_Ctrl_F10;   /* Ctrl F10 */
          if (strcmp(esc_seq,"[23;5~") == 0) c = Key_Ctrl_F11;   /* Ctrl F11 */
          if (strcmp(esc_seq,"[24;5~") == 0) c = Key_Ctrl_F12;   /* Ctrl F12 */
          if (strcmp(esc_seq,"[1;3P") == 0)  c = Key_Alt_F1;     /* Alt F1 */
          if (strcmp(esc_seq,"[1;3Q") == 0)  c = Key_Alt_F2;     /* Alt F2 */
          if (strcmp(esc_seq,"[1;3R") == 0)  c = Key_Alt_F3;     /* Alt F3 */
          if (strcmp(esc_seq,"[1;3S") == 0)  c = Key_Alt_F4;     /* Alt F4 */
          if (strcmp(esc_seq,"[15;3~") == 0) c = Key_Alt_F5;     /* Alt F5 */
          if (strcmp(esc_seq,"[17;3~") == 0) c = Key_Alt_F6;     /* Alt F6 */
          if (strcmp(esc_seq,"[18;3~") == 0) c = Key_Alt_F7;     /* Alt F7 */
          if (strcmp(esc_seq,"[19;3~") == 0) c = Key_Alt_F8;     /* Alt F8 */
          if (strcmp(esc_seq,"[20;3~") == 0) c = Key_Alt_F9;     /* Alt F9 */
          if (strcmp(esc_seq,"[21;3~") == 0) c = Key_Alt_F10;    /* Alt F10 */
          if (strcmp(esc_seq,"[23;3~") == 0) c = Key_Alt_F11;    /* Alt F11 */
          if (strcmp(esc_seq,"[24;3~") == 0) c = Key_Alt_F12;    /* Alt F12 */
          if (strcmp(esc_seq,"[Z") == 0) c = Key_Shift_Tab;      /* Shift Tab */
if (c == 0) printf("unknown: %s\n",esc_seq);
          }
        }
      else if (c == 27) {
        usleep(5000);
        if (KeyPressed()) {
          esc_mode = 1;
          esc_seq[0] = 0;
          c = 0;
          }
        }
      }
    if (c == 10) c = 13;
    return c;
    }

  Byte Terminal::Input(char* buffer, UInt32 maxlen) {
    UInt32 key;
    Boolean flag;
    Boolean buf;
    Boolean cur;
    Boolean ec;
    UInt32 len;
    buf = buffered;
    cur = cursorShown;
    ec = echo;
    Buffered(false);
    ShowCursor(true);
    Echo(false);
    strcpy(buffer,"");
    flag = true;
 
    while (flag) {
      key = Inkey();
      if (key == 10) key = 13;
      if (key == 27) flag = false;
      else if (key == 13) flag = false;
      else if (key == 9) flag = false;
      else if (key == Key_Shift_Tab) flag = false;
      else if (key == 127 || key == Key_Delete) {
        len = strlen(buffer);
        if (len > 0) {
          buffer[len-1] = 0;
          printf("%c%c%c",8,32,8); fflush(stdout);
          }
        }
      else if (key >= ' ' && key < 127) {
        len = strlen(buffer);
        if (len < maxlen) {
          buffer[len] = key & 0xff;
          buffer[len+1] = 0;
          printf("%c",key); fflush(stdout);
          }
        }
      }
    Buffered(buf);
    ShowCursor(cur);
    Echo(ec);
    return key;
    }

  Byte Terminal::InputAt(UInt32 x, UInt32 y, char* buffer, UInt32 maxlen) {
    Position(x,y);
    return Input(buffer, maxlen);
    }

  void Terminal::Inverse(bool b) {
    inverse = b;
    setAttributes();
    }

  bool Terminal::KeyPressed() {
    int           c;
    fd_set         rfds;
    struct timeval tv;
    tv.tv_sec=0;
    tv.tv_usec=0;
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    c=select(FD_SETSIZE,&rfds,NULL,NULL,&tv);
    return (c) ? true : false;
    }

  void Terminal::Position(UInt32 x, UInt32 y) {
    printf("\e[%d;%dH", y+1, x+1);
    fflush(stdout);
    }

  void Terminal::ShowCursor(bool b) {
    if (b) printf("\E[?25h");
      else printf("\E[?25l");
    fflush(stdout);
    cursorShown = b;
    }

  void Terminal::Underline(bool b) {
    underline = b;
    setAttributes();
    }

  UInt32 Terminal::Width() {
    return width;
    }

  UInt32 Terminal::Width(UInt32 i) {
    width = i;
    return width;
    }

  void Terminal::WrapAround(bool b) {
    if (b) printf("\E[?7h");
      else printf("\E[?7l");
    fflush(stdout);
    }

  void Terminal::Write(const char* msg) {
    printf("%s",msg);
    }

  void Terminal::Write(String* msg) {
    Write(msg->AsCharArray());
    }

  void Terminal::WriteAt(UInt32 x, UInt32 y, const char* msg) {
    Position(x,y);
    Write(msg);
    }

  void Terminal::WriteAt(UInt32 x, UInt32 y, String* msg) {
    WriteAt(x,y,msg->AsCharArray());
    }

  void Terminal::WriteLn(const char* msg) {
    printf("%s\n",msg);
    }

  void Terminal::WriteLn(String* msg) {
    WriteLn(msg->AsCharArray());
    }

  void Terminal::WriteLnAt(UInt32 x, UInt32 y, const char* msg) {
    Position(x,y);
    WriteLn(msg);
    }

  void Terminal::WriteLnAt(UInt32 x, UInt32 y, String* msg) {
    WriteLnAt(x,y,msg->AsCharArray());
    }

  }

