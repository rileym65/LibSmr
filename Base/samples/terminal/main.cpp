#include <unistd.h>
#include <stdio.h>
#include <SmrFramework.h>

using namespace SmrFramework;

int main(int argc, char** argv) {
  UInt32 key;
  Terminal* term;
  printf("Starting\n");
  term = new Terminal();
  term->Foreground(Terminal::Black);
  term->Background(Terminal::Blue);
  term->ClearScreen();
  term->ShowCursor(false);
  term->Position(10, 2);
  printf("Test\n");
  term->Bold(true);
  printf("Bold\n");
  term->Bold(false);
  printf("Normal\n");
  term->Underline(true);
  printf("Underlined\n");
  term->Underline(false);
  printf("Not underlined\n");
  term->Foreground(Terminal::Red);
  printf("Red\n");
  term->Inverse(true);
  printf("Inverted\n");
  term->Inverse(false);
  printf("Not inverted\n");
  term->Buffered(false);
  term->Echo(false);
term->Position(10, 20);
printf("\e[?6n\n");
printf("XX\n");
  while (1) {
    key = term->Inkey();
    printf("key = %x\n",key);
    }


  term->ShowCursor(true);
  }

