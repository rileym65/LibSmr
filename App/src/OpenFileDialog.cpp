#include <SmrFramework.h>
#include "SmrAppFramework.h"

using namespace SmrFramework;

Int32 OpenFileDialog::_fileDialogSorter(Object* a, Object* b) {
  return a->ToString().ToUpper().CompareTo(b->ToString().ToUpper());
  }

OpenFileDialog::OpenFileDialog(Control *parent,int x, int y, int w, int h) :
            Dialog(parent, x, y, w, h) {
  this->objectType = (char*)"Dialog";
  fileList = new ListBox(this, 10, 10, this->Width()-20, this->Height()-60);
  fileList->Visible(true);
  fileList->Comparer(OpenFileDialog::_fileDialogSorter);
  this->Add(fileList);

  okButton = new Button(this, 10, this->Height()-40, 80, 30);
  okButton->BackgroundColor(Color::White());
  okButton->ForegroundColor(Color::Black());
  okButton->Text("Open");
  okButton->OnRelease(new EventHandler(OpenFileDialog::_OkButtonReleased, this));
  okButton->Visible(true);
  this->Add(okButton);

  cancelButton = new Button(this, this->Width()-90, this->Height()-40, 80, 30);
  cancelButton->BackgroundColor(Color::White());
  cancelButton->ForegroundColor(Color::Black());
  cancelButton->Text("Cancel");
  cancelButton->OnRelease(new EventHandler(OpenFileDialog::_CancelButtonReleased, this));
  cancelButton->Visible(true);
  this->Add(cancelButton);

  this->BorderWidth(3);

  pattern = NULL;
  }

OpenFileDialog::~OpenFileDialog() {
  if (pattern != NULL) delete(pattern);
  }

DialogResult OpenFileDialog::ShowDialog() {
  refreshFiles();
  return Dialog::ShowDialog();
  }

RegEx* OpenFileDialog::Pattern() {
  return pattern;
  }

RegEx* OpenFileDialog::Pattern(RegEx* r) {
  pattern = r;
  return pattern;
  }

void OpenFileDialog::refreshFiles() {
  UInt32 i;
  List<String*>* files;
  fileList->Clear();
  files = Directory::GetFiles(".");
  if (files == NULL) {
    return;
    }
  for (i=0; i<files->Count(); i++) {
    if (pattern == NULL) fileList->Add(files->At(i));
    else if (pattern->Match(files->At(i)->AsCharArray())) fileList->Add(files->At(i));
    }
  fileList->Sort();
  }

String* OpenFileDialog::FileName() {
  if (fileList->SelectedIndex() < 0) return NULL;
  return new String((String*)(fileList->SelectedItem()));
return NULL;
  }

void OpenFileDialog::_OkButtonReleased(void* obj, void* sender, EventArgs args) {
  ((OpenFileDialog*)obj)->OkButtonReleased(sender, (MouseEventArgs&)args);
  }

void OpenFileDialog::_CancelButtonReleased(void* obj, void* sender, EventArgs args) {
  ((OpenFileDialog*)obj)->CancelButtonReleased(sender, (MouseEventArgs&)args);
  }

void OpenFileDialog::OkButtonReleased(void* sender, MouseEventArgs args) {
  this->Ok();
  }

void OpenFileDialog::CancelButtonReleased(void* sender, MouseEventArgs args) {
  this->Cancel();
  }


