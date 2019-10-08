#include <SmrFramework.h>
#include <SmrAppFramework.h>

using namespace SmrFramework;

#include "FileDialog.h"

class MainApplication : public Application {
  protected:
  public:
    MainApplication();
    ~MainApplication();
  };

class MainWindow : public Form {
  protected:
    int           buttonDown;
    Control      *ctrl;
    Button       *button1;
    Button       *button2;
    Toggle       *button3;
    Button       *pictureButton;
    Button       *addListItemButton;
    Button       *quitButton;
    CheckBox     *checkbox1;
    RadioButton  *radio1;
    RadioButton  *radio2;
    RadioButton  *radio3;
    RadioButton  *radio4;
    RadioButton  *radioA;
    RadioButton  *radioB;
    RadioButton  *radioC;
    VScrollBar   *vscroll;
    HScrollBar   *hscroll;
    Spinner      *spinner;
    ListBox      *list;
    Image*        image1;
    Image*        imageE;
    Image*        imageD;
    Image*        imageI1;
    Image*        imageI2;
    Image*        imageI3;
    PictureBox   *picture1;
    PictureBox   *picture2;
    Button       *pic1Button;
    Button       *pic2Button;
    Button       *pic3Button;
    Button       *standardButton;
    Button       *fitButton;
    Button       *stretchButton;
    Button       *dialogButton;
    Toggle       *centerButton;
    DropDownList *dropDown;
    TextBox      *textBox;
    Timer        *timer;
    void OnMouseDownEvent(void* sender, MouseEventArgs args);
    void OnMouseMoveEvent(void* sender, MouseEventArgs args);
    void OnMouseUpEvent(void* sender, MouseEventArgs args);
    void OnMainDownEvent(void* sender, MouseEventArgs args);
    void OnKeyDownEvent(void* sender, KeyEventArgs args);
    void OnKeyUpEvent(void* sender, KeyEventArgs args);
    void OnPaintEvent(void* sender, PaintEventArgs args);
    void ButtonPressEvent(void* sender, MouseEventArgs args);
    void ButtonReleaseEvent(void* sender, MouseEventArgs args);
    void CheckChangedEvent(void* sender, MouseEventArgs args);
    void RadioChangedEvent(void* sender, MouseEventArgs args);
    void ScrollChangedEvent(void* sender, ValueEventArgs args);
    void SpinnerChangedEvent(void* sender, ValueEventArgs args);
    void ListChangedEvent(void* sender, ValueEventArgs args);
    void DropChangedEvent(void* sender, ValueEventArgs args);
    void OnMouseEnterEvent(void* sender, MouseEventArgs args);
    void OnMouseLeaveEvent(void* sender, MouseEventArgs args);
    void OnResizedEvent(void* sender, ResizeEventArgs args);
    void TimerTick(void* sender);
  public:
    MainWindow(int w, int h);
    ~MainWindow();
    void InitializeComponent();
    void static _OnMouseDownEvent(void* obj, void* sender, EventArgs args);
    void static _OnMouseMoveEvent(void* obj, void* sender, EventArgs args);
    void static _OnMouseUpEvent(void* obj, void* sender, EventArgs args);
    void static _OnMainDownEvent(void* obj, void* sender, EventArgs args);
    void static _OnKeyDownEvent(void* obj, void* sender, EventArgs args);
    void static _OnKeyUpEvent(void* obj, void* sender, EventArgs args);
    void static _OnPaintEvent(void* obj, void* sender, EventArgs args);
    void static _ButtonPressEvent(void* obj, void* sender, EventArgs args);
    void static _ButtonReleaseEvent(void* obj, void* sender, EventArgs args);
    void static _CheckChangedEvent(void* obj, void* sender, EventArgs args);
    void static _RadioChangedEvent(void* obj, void* sender, EventArgs args);
    void static _ScrollChangedEvent(void* obj, void* sender, EventArgs args);
    void static _SpinnerChangedEvent(void* obj, void* sender, EventArgs args);
    void static _ListChangedEvent(void* obj, void* sender, EventArgs args);
    void static _DropChangedEvent(void* obj, void* sender, EventArgs args);
    void static _OnMouseEnterEvent(void* obj, void* sender, EventArgs args);
    void static _OnMouseLeaveEvent(void* obj, void* sender, EventArgs args);
    void static _OnResizedEvent(void* obj, void* sender, EventArgs args);
    void static _TimerTick(void* obj, void* sender, EventArgs args);
  };

