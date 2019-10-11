#include <stdio.h>
#include "header.h"

void MainWindow::InitializeComponent() {
  GroupBox*     group;
  Label*        label;
  buttonDown = 0;
  image1 = new Image("1.png");
  imageE = new Image("Enter.png");
  imageD = new Image("disabled.png");
  this->BackgroundColor(0x8000, 0x8000, 0x8000);
  this->OnResize(new EventHandler(MainWindow::_OnResizedEvent, this));
  ctrl = new Control(this, 10, 10, 50, 50);
  ctrl->BorderWidth(2);
  ctrl->BackgroundColor(0, 0, 0xffff);
  ctrl->Visible(true);
  ctrl->Location(100, 100);
  ctrl->OnMouseDown(new EventHandler(MainWindow::_OnMouseDownEvent, this));
  ctrl->OnMouseMove(new EventHandler(MainWindow::_OnMouseMoveEvent, this));
  ctrl->OnMouseUp(new EventHandler(MainWindow::_OnMouseUpEvent, this));
  ctrl->OnKeyDown(new EventHandler(MainWindow::_OnKeyDownEvent, this));
  ctrl->OnKeyUp(new EventHandler(MainWindow::_OnKeyUpEvent, this));
  ctrl->OnPaint(new EventHandler(MainWindow::_OnPaintEvent, this));
  ctrl->OnMouseEnter(new EventHandler(MainWindow::_OnMouseEnterEvent, this));
  ctrl->OnMouseLeave(new EventHandler(MainWindow::_OnMouseLeaveEvent, this));
  this->Add(ctrl);
  this->OnMouseDown(new EventHandler(MainWindow::_OnMainDownEvent, this));
  this->OnKeyDown(new EventHandler(MainWindow::_OnKeyDownEvent, this));
  this->OnPaint(new EventHandler(MainWindow::_OnPaintEvent, this));
  quitButton = new Button(this, 20, 20, 75, 25);
  quitButton->Visible(true);
  quitButton->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  quitButton->Text("Exit");
  this->Add(quitButton);
  button1 = new Button(this, 200, 50, 75, 25);
  button1->Visible(true);
  button1->BackgroundColor(0x0000, 0xffff, 0x0000);
  button1->SetPressedColor(0x0000, 0xc000, 0x0000);
button1->BorderWidth(0);
  button1->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  button1->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  button1->Text("Button 1");
  this->Add(button1);
  button2 = new Button(this, 200, 80, 75, 25);
  button2->Visible(true);
  button2->BackgroundColor(0xffff, 0x0000, 0x0000);
  button2->SetPressedColor(0xc000, 0x0000, 0x0000);
  button2->ForegroundColor(0x0000, 0x0000, 0xffff);
  button2->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  button2->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
//  button2->Font((char*)"-*-times-*-r-*-*-14-*-*-*-*-*-*-*");
  button2->Font("Roboto-12");
  button2->Text("Button 2");
  this->Add(button2);
  button3 = new Toggle(this, 300, 50, 75, 25);
  button3->Visible(true);
  button3->BackgroundColor(0x0000, 0xffff, 0x0000);
  button3->SetPressedColor(0x0000, 0xc000, 0x0000);
  button3->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  button3->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  button3->Text("Toggle");
  this->Add(button3);
  pictureButton = new Button(this, 400, 450, 100, 100);
  pictureButton->BackgroundColor(0x0000, 0xffff, 0x0000);
  pictureButton->BackgroundImage(image1);
  pictureButton->PressedImage(imageE);
  pictureButton->DisabledImage(imageD);
  pictureButton->Visible(true);
  pictureButton->SetPressedColor(0x0000, 0xc000, 0x0000);
  pictureButton->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  pictureButton->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  pictureButton->Text("");
  this->Add(pictureButton);

  picture1 = new PictureBox(this, 500, 450, 100, 100);
  picture1->BackgroundColor(0x0000, 0x0000, 0x0000);
  picture1->Picture(imageE);
  picture1->Visible(true);
  this->Add(picture1);

  checkbox1 = new CheckBox(this, 200, 410, 100, 25);
  checkbox1->Visible(true);
  checkbox1->ForegroundColor(0x0000, 0x0000, 0x0000);
  checkbox1->OnCheckChanged(new EventHandler(MainWindow::_CheckChangedEvent, this));
  checkbox1->Text("Checkbox 1");
  this->Add(checkbox1);

  testLabel = new Label(this, 300, 10, 75, 25, "Test Label");
  testLabel->BackgroundColor(0xffff, 0x0000, 0x0000);
  testLabel->TextOffsetX(-5);
  testLabel->TextOffsetY(-5);
  this->Add(testLabel);

  radio1 = new RadioButton(this, 200, 440, 100, 25);
  radio1->Visible(true);
  radio1->Text("Radio 1");
  radio1->OnCheckChanged(new EventHandler(MainWindow::_RadioChangedEvent, this));
  this->Add(radio1);
  radio2 = new RadioButton(this, 200, 470, 100, 25);
  radio2->Visible(true);
  radio2->Text("Radio 2");
  radio2->OnCheckChanged(new EventHandler(MainWindow::_RadioChangedEvent, this));
  this->Add(radio2);
  radio3 = new RadioButton(this, 200, 500, 100, 25);
  radio3->Visible(true);
  radio3->Text("Radio 3");
  radio3->OnCheckChanged(new EventHandler(MainWindow::_RadioChangedEvent, this));
  this->Add(radio3);
  radio4 = new RadioButton(this, 200, 530, 100, 25);
  radio4->Visible(true);
  radio4->Text("Radio 4");
  radio4->OnCheckChanged(new EventHandler(MainWindow::_RadioChangedEvent, this));
  this->Add(radio4);

  group = new GroupBox(this, 20,200,125,120);
  group->Visible(true);
  group->Text("Group 1");
  this->Add(group);
  radioA = new RadioButton(group, 20, 20, 75, 25);
  radioA->Visible(true);
  radioA->Text("Radio A");
  radioA->OnCheckChanged(new EventHandler(MainWindow::_RadioChangedEvent, this));
  group->Add(radioA);
  radioB = new RadioButton(group, 20, 50, 75, 25);
  radioB->Visible(true);
  radioB->Text("Radio B");
  radioB->OnCheckChanged(new EventHandler(MainWindow::_RadioChangedEvent, this));
  group->Add(radioB);
  radioC = new RadioButton(group, 20, 80, 75, 25);
  radioC->Visible(true);
  radioC->Text("Radio C");
  radioC->OnCheckChanged(new EventHandler(MainWindow::_RadioChangedEvent, this));
  group->Add(radioC);

  vscroll = new VScrollBar(this, this->width-20, 0, 20, height);
  vscroll->BackgroundColor(0xffff, 0xffff, 0xffff);
  vscroll->Visible(true);
  vscroll->MinValue(25);
  vscroll->OnValueChanged(new EventHandler(MainWindow::_ScrollChangedEvent, this));
  this->Add(vscroll);

  hscroll = new HScrollBar(this, 0, this->height-20, width-20, 20);
  hscroll->BackgroundColor(0xffff, 0xffff, 0xffff);
  hscroll->Visible(true);
  hscroll->MinValue(25);
  hscroll->MaxValue(200);
  hscroll->PuckRange(20);
  hscroll->OnValueChanged(new EventHandler(MainWindow::_ScrollChangedEvent, this));
  this->Add(hscroll);

  label = new Label(this, 20, 375, 74, 25, "Spinner");
  label->Font("Gentium Book Basic-14");
  label->ForegroundColor(0x0000,0x0000,0x7fff);
  this->Add(label);
//  this->Add(new Label(this, 20, 375, 75, 25, "Spinner"));
  spinner = new Spinner(this, 20, 400, 75, 25);
  spinner->Visible(true);
  spinner->MinValue(0);
  spinner->MaxValue(100);
  spinner->Step(5);
  spinner->OnValueChanged(new EventHandler(MainWindow::_SpinnerChangedEvent, this));
  this->Add(spinner);

  list = new ListBox(this, 180, 200, 200, 150);
  list->BackgroundColor(0xffff, 0xffff, 0xffff);
  list->Visible(true);
  list->OnValueChanged(new EventHandler(MainWindow::_ListChangedEvent, this));
  this->Add(list);
  list->Add(new String((char*)"Item 1"));
  list->Add(new String((char*)"Item 2"));
  list->Add(new String((char*)"Item 3"));
  list->Add(new String((char*)"Item 4"));

  addListItemButton = new Button(this, 200, 375, 125, 25);
  addListItemButton->Visible(true);
  addListItemButton->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  addListItemButton->Text("Add List Item");
  this->Add(addListItemButton);
 
  imageI1 = new Image("iw1.jpg");
  imageI2 = new Image("iw2.jpg");
  imageI3 = new Image("iw3.jpg");
  picture2 = new PictureBox(this, 400,10,350,400);
  picture2->Picture(imageI2);
  picture2->Mode(PictureBox::MODE_FIT);
  picture2->Visible(true);
  this->Add(picture2);

  pic1Button = new Button(this, 700, 415, 75, 25);
  pic1Button->Visible(true);
  pic1Button->BackgroundColor(0xffff, 0x0000, 0x0000);
  pic1Button->SetPressedColor(0xc000, 0x0000, 0x0000);
  pic1Button->ForegroundColor(0x0000, 0x0000, 0xffff);
  pic1Button->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  pic1Button->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  pic1Button->Text("Picture 1");
  this->Add(pic1Button);

  pic2Button = new Button(this, 700, 450, 75, 25);
  pic2Button->Visible(true);
  pic2Button->BackgroundColor(0xffff, 0x0000, 0x0000);
  pic2Button->SetPressedColor(0xc000, 0x0000, 0x0000);
  pic2Button->ForegroundColor(0x0000, 0x0000, 0xffff);
  pic2Button->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  pic2Button->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  pic2Button->Text("Picture 2");
  this->Add(pic2Button);

  pic3Button = new Button(this, 700, 485, 75, 25);
  pic3Button->Visible(true);
  pic3Button->BackgroundColor(0xffff, 0x0000, 0x0000);
  pic3Button->SetPressedColor(0xc000, 0x0000, 0x0000);
  pic3Button->ForegroundColor(0x0000, 0x0000, 0xffff);
  pic3Button->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  pic3Button->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  pic3Button->Text("Picture 3");
  this->Add(pic3Button);

  standardButton = new Button(this, 400, 415, 75, 25);
  standardButton->Visible(true);
  standardButton->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  standardButton->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  standardButton->Text("Standard");
  this->Add(standardButton);

  fitButton = new Button(this, 500, 415, 75, 25);
  fitButton->Visible(true);
  fitButton->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  fitButton->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  fitButton->Text("Fit");
  this->Add(fitButton);

  stretchButton = new Button(this, 600, 415, 75, 25);
  stretchButton->Visible(true);
  stretchButton->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  stretchButton->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  stretchButton->Text("Stretch");
  this->Add(stretchButton);

  centerButton = new Toggle(this, 600, 445, 75, 25);
  centerButton->Visible(true);
  centerButton->OnPress(new EventHandler(MainWindow::_ButtonPressEvent, this));
  centerButton->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  centerButton->Text("Center");
  this->Add(centerButton);

printf("%d\n",Height());
  dropDown = new DropDownList(this, 10, 440, 125, 25);
  dropDown->ForegroundColor(Color::Blue());
  dropDown->MaxVisibleItems(10);
  dropDown->Add(new String("Item 1"));
  dropDown->Add(new String("Item 2"));
  dropDown->Add(new String("Item 3"));
  dropDown->Add(new String("Item 4"));
  dropDown->Add(new String("Item 5"));
  dropDown->Add(new String("Item 6"));
  dropDown->Add(new String("Item 7"));
  dropDown->Add(new String("Item 8"));
  dropDown->Add(new String("Item 9"));
  dropDown->Add(new String("Item 10"));
  dropDown->Add(new String("Item 11"));
  dropDown->Add(new String("Item 12"));
  dropDown->Add(new String("Item 13"));
  dropDown->Add(new String("Item 14"));
  dropDown->Add(new String("Item 15"));
  dropDown->SelectedIndex(1);
  dropDown->Visible(true);
  dropDown->OnValueChanged(new EventHandler(MainWindow::_DropChangedEvent, this));
  this->Add(dropDown);

  textBox = new TextBox(this, 10, 470, 100, 25, "Text box");
  textBox->Visible(true);
  this->Add(textBox);

  timer = new Timer();
  timer->OnTick(new EventHandler(MainWindow::_TimerTick, this));
  timer->Interval(350);
  timer->StopOnTick(true);

  dialogButton = new Button(this, 10, 500, 100, 25);
  dialogButton->Visible(true);
  dialogButton->OnRelease(new EventHandler(MainWindow::_ButtonReleaseEvent, this));
  dialogButton->Text("Dialog");
  this->Add(dialogButton);
  }

void MainWindow::_OnMouseDownEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnMouseDownEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_OnMouseMoveEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnMouseMoveEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_OnMouseUpEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnMouseUpEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_OnMainDownEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnMainDownEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_OnKeyDownEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnKeyDownEvent(sender, (KeyEventArgs&)args);
  }

void MainWindow::_OnKeyUpEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnKeyUpEvent(sender, (KeyEventArgs&)args);
  }

void MainWindow::_OnPaintEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnPaintEvent(sender, (PaintEventArgs&)args);
  }

void MainWindow::_ButtonPressEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->ButtonPressEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_ButtonReleaseEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->ButtonReleaseEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_CheckChangedEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->CheckChangedEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_RadioChangedEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->RadioChangedEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_ScrollChangedEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->ScrollChangedEvent(sender, (ValueEventArgs&)args);
  }

void MainWindow::_SpinnerChangedEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->SpinnerChangedEvent(sender, (ValueEventArgs&)args);
  }

void MainWindow::_ListChangedEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->ListChangedEvent(sender, (ValueEventArgs&)args);
  }

void MainWindow::_DropChangedEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->DropChangedEvent(sender, (ValueEventArgs&)args);
  }

void MainWindow::_OnMouseEnterEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnMouseEnterEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_OnMouseLeaveEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnMouseLeaveEvent(sender, (MouseEventArgs&)args);
  }

void MainWindow::_OnResizedEvent(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->OnResizedEvent(sender, (ResizeEventArgs&)args);
  }

void MainWindow::_TimerTick(void* obj, void* sender, EventArgs args) {
  ((MainWindow*)obj)->TimerTick(sender);
  }
