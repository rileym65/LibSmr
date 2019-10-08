#include <unistd.h>
#include <SmrFramework.h>
#include <SmrAppFramework.h>

namespace SmrFramework {

  Dialog::Dialog(Control* parent, int x, int y, int w, int h) :
    Form(parent, x, y, w, h) {
    running = false;
    this->objectType = (char*)"Dialog";
    result = DialogResult(DialogResult::CANCEL);
    application->AddDialog(this);
    }
  
  Dialog::~Dialog() {
    application->RemoveDialog(this);
    }
  
  DialogResult Dialog::ShowDialog() {
    running = true;
    this->Visible(true);
    while (running) {
      application->DoEvents();
      usleep(application->LoopDelay());
      }
    return result;
    }
  
  void Dialog::Close() {
    this->Visible(false);
    running = false;
    }
  
  void Dialog::Ok() {
    result = DialogResult(DialogResult::OK);
    Close();
    }
  
  void Dialog::Cancel() {
    result = DialogResult(DialogResult::CANCEL);
    Close();
    }
  
  DialogResult Dialog::Result() {
    return result;
    }

  }
