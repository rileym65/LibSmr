#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

int main(int argc,char** argv) {
  MainWindow*      window;
  application = new MainApplication();
  application->LoopDelay(1000);
  if (application == NULL) {
    printf("Could not create application object\n");
    exit(1);
    }
  window = new MainWindow(320, 240);
  window->Location(200, 200);
  window->BorderWidth(5);
  window->BackgroundColor(0x8000, 0x8000, 0x8000);
  window->Text("test window");

  application->AddForm(window);

  window->Visible(true);

  try {
    application->Run();
    }
  catch (Exception &e) {
printf("Exception caught: %s\n",e.Message());
if (e.Sender() != NULL) printf("  Type: %s\n",e.Sender()->Type());
else printf("Sender is NULL\n");
fflush(stdout);
    }

  delete(application);
  }

