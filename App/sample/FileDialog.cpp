#include <SmrFramework.h>
#include <SmrAppFramework.h>

using namespace SmrFramework;

#include "FileDialog.h"

Int32 FileDialog::_fileDialogSorter(Object* a, Object* b) {
  return a->ToString().ToUpper().CompareTo(b->ToString().ToUpper());
  }

FileDialog::FileDialog(Control *parent,int x, int y, int w, int h) :
            Dialog(parent, x, y, w, h) {
  fileList = new ListBox(this, 10, 10, this->Width()-20, this->Height()-60);
  fileList->Visible(true);
  fileList->Comparer(FileDialog::_fileDialogSorter);
  this->Add(fileList);

  okButton = new Button(this, 10, this->Height()-40, 80, 30);
  okButton->BackgroundColor(Color::White());
  okButton->ForegroundColor(Color::Black());
  okButton->Text("Open");
  okButton->OnRelease(new EventHandler(FileDialog::_OkButtonReleased, this));
  okButton->Visible(true);
  this->Add(okButton);

  cancelButton = new Button(this, this->Width()-90, this->Height()-40, 80, 30);
  cancelButton->BackgroundColor(Color::White());
  cancelButton->ForegroundColor(Color::Black());
  cancelButton->Text("Cancel");
  cancelButton->OnRelease(new EventHandler(FileDialog::_CancelButtonReleased, this));
  cancelButton->Visible(true);
  this->Add(cancelButton);

  this->BorderWidth(3);

  pattern = NULL;
  }

FileDialog::~FileDialog() {
  if (pattern != NULL) delete(pattern);
  }

DialogResult FileDialog::ShowDialog() {
  refreshFiles();
  return Dialog::ShowDialog();
  }

RegEx* FileDialog::Pattern() {
  return pattern;
  }

RegEx* FileDialog::Pattern(RegEx* r) {
  pattern = r;
  return pattern;
  }

void FileDialog::refreshFiles() {
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

String* FileDialog::FileName() {
  if (fileList->SelectedIndex() < 0) return NULL;
  return new String((String*)(fileList->SelectedItem()));
return NULL;
  }

void FileDialog::_OkButtonReleased(void* obj, void* sender, EventArgs args) {
  ((FileDialog*)obj)->OkButtonReleased(sender, (MouseEventArgs&)args);
  }

void FileDialog::_CancelButtonReleased(void* obj, void* sender, EventArgs args) {
  ((FileDialog*)obj)->CancelButtonReleased(sender, (MouseEventArgs&)args);
  }

void FileDialog::OkButtonReleased(void* sender, MouseEventArgs args) {
  printf("OK pressed\n");
  this->Ok();
  }

void FileDialog::CancelButtonReleased(void* sender, MouseEventArgs args) {
  printf("Cancel pressed\n");
  this->Cancel();
  }


