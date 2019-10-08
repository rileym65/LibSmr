#ifndef _FILEDIALOG_H
#define _FILEDIALOG_H

class FileDialog : public Dialog {
  protected:
    ListBox *fileList;
    Button  *okButton;
    Button  *cancelButton;
    RegEx   *pattern;
    void     refreshFiles();
    void     OkButtonReleased(void* sender, MouseEventArgs args);
    void     CancelButtonReleased(void* sender, MouseEventArgs args);
    Int32 static _fileDialogSorter(Object* a, Object* b);
    void  static _OkButtonReleased(void* obj, void* sender, EventArgs args);
    void  static _CancelButtonReleased(void* obj, void* sender, EventArgs args);
  public:
    FileDialog(Control* parent, int x, int y, int w, int h);
    virtual ~FileDialog();
    virtual String* FileName();
    virtual RegEx*  Pattern();
    virtual RegEx*  Pattern(RegEx* r);
    virtual DialogResult ShowDialog();
    };

#endif
