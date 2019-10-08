#include <stdio.h>
#include "header.h"

Boolean flasher;

MainWindow::MainWindow(int x, int y) : Form(NULL, 0, 0, x, y) {
  Size(800, 600);
  InitializeComponent();
  }

MainWindow::~MainWindow() {
  }

void MainWindow::TimerTick(void* sender) {
  if (flasher) {
    ctrl->BackgroundColor(Color::Red());
    flasher = false;
    }
  else {
    ctrl->BackgroundColor(Color::Green());
    flasher = true;
    }
  timer->Start();
  }

void MainWindow::OnResizedEvent(void* sender, ResizeEventArgs args) {
  printf("Window resized new=%d,%d\n",args.Width(),args.Height());
  }

void MainWindow::OnMouseLeaveEvent(void* sender, MouseEventArgs args) {
  printf("Mouse left %d,%d\n",args.X(), args.Y());
  }

void MainWindow::OnMouseEnterEvent(void* sender, MouseEventArgs args) {
  printf("Mouse entered %d,%d\n",args.X(), args.Y());
  }


void MainWindow::OnMouseDownEvent(void* sender, MouseEventArgs args) {
  printf("Mouse Down %d,%d  %d\n",args.X(), args.Y(), args.Button());
  buttonDown = -1;
  }

void MainWindow::OnMouseMoveEvent(void* sender, MouseEventArgs args) {
  if (buttonDown)
    printf("Mouse Move %d,%d  %d\n",args.X(), args.Y(), args.Button());
  }

void MainWindow::OnMouseUpEvent(void* sender, MouseEventArgs args) {
  printf("Mouse Up %d,%d  %d\n",args.X(), args.Y(), args.State());
  buttonDown = 0;
  }

void MainWindow::OnMainDownEvent(void* sender, MouseEventArgs args) {
  printf("Main Mouse Down %d,%d  %d\n",args.X(), args.Y(), args.Button());
  }

void MainWindow::OnKeyDownEvent(void* sender, KeyEventArgs args) {
  if (sender == this) printf("Main Key Down %d\n",args.Key());
  if (sender == ctrl) printf("Sub Key Down %d\n",args.Key());
  }

void MainWindow::OnKeyUpEvent(void* sender, KeyEventArgs args) {
  printf("Key Up %d\n",args.Key());
  }

void MainWindow::OnPaintEvent(void* sender, PaintEventArgs args) {
  if (sender == this) printf("Main ");
  if (sender == ctrl) printf("Sub  ");
  printf("Paint %d,%d  %d,%d  %d\n",args.X(), args.Y(), args.Width(),
          args.Height(), args.Count());
  }


void MainWindow::ButtonPressEvent(void* sender, MouseEventArgs args) {
  if (sender == button1) printf("Button1 ");
  if (sender == button2) printf("Button2 ");
  if (sender == button3) printf("Button3 ");
  if (sender == button1) button1->Text("Release");
  if (sender == button2) button1->Visible(false);
  if (sender == button2) pictureButton->Visible(false);
  if (sender == centerButton) picture2->Center(true);
  printf("Pressed %d,%d  %d\n",args.X(), args.Y(), args.Button());
  if (sender == button3) {
    list->SetSelectMultiple(-1);
    }
  if (sender == button1) {
    Graphics* g;
    g = GetGraphics();
    g->Clear();
    delete(g);
    }
  if (sender == button3) {
    timer->Start();
    pictureButton->Enabled(false);
    button2->Font("Roboto-14");
    }
  }

void MainWindow::ButtonReleaseEvent(void* sender, MouseEventArgs args) {
  Object* obj;
  int result;
  OpenFileDialog *dialog;
  char temp[64];
  if (sender == button1) printf("Button1 ");
  if (sender == button2) printf("Button2 ");
  if (sender == button3) printf("Button3 ");
  if (sender == button1) button1->Text("Press");
  if (sender == button2) button1->Visible(true);
  if (sender == button2) pictureButton->Visible(true);
  if (sender == pic1Button) picture2->Picture(imageI1);
  if (sender == pic2Button) picture2->Picture(imageI2);
  if (sender == pic3Button) picture2->Picture(imageI3);
  if (sender == centerButton) picture2->Center(false);
  if (sender == standardButton) picture2->Mode(PictureBox::MODE_STANDARD);
  if (sender == fitButton) picture2->Mode(PictureBox::MODE_FIT);
  if (sender == stretchButton) picture2->Mode(PictureBox::MODE_STRETCH);
  printf("Released %d,%d  %d\n",args.X(), args.Y(), args.Button());
  if (sender == quitButton) application->Stop();
  if (sender == addListItemButton) {
    sprintf(temp,"Item %d", list->Count()+1);
    list->Add(new String(temp));
    textBox->Text("New text");
    }
  if (sender == button3) {
    list->SetSelectMultiple(0);
    }
  if (sender == button3) {
    timer->Stop();
    pictureButton->Enabled(true);
    button2->Font("Roboto-12");
    }
  if (sender == button1) {
    Graphics* g;
    List<Point*>* points;
    g = GetGraphics();
    g->LineWidth(5);
    g->CapStyle(Graphics::CapStyleRound);
    g->DrawLine(10,10,500,500);
    g->LineWidth(10);
    g->DrawPoint(10,50);
    g->ForegroundColor(0xff00, 0x0000, 0x0000);
    g->DrawRectangle(500,480,50, 50);
    g->LineWidth(3);
    g->ForegroundColor(0x0000, 0x7fff, 0x0000);
    g->DrawEllipse(500, 430, 50, 50);
    g->ForegroundColor(0x0000, 0x0000, 0xffff);
    points = new List<Point*>();
    points->Add(new Point(550, 380));
    points->Add(new Point(600, 425));
    points->Add(new Point(500, 425));
    points->Add(new Point(550, 380));
    g->DrawLines(points);
    for (UInt32 i=0; i<points->Count(); i++) {
      delete(points->At(i));
      }
    delete(points);
    g->ForegroundColor(0xffff, 0xffff, 0x0000);
    g->FillRectangle(560, 480, 50, 50);
    g->ForegroundColor(0xffff, 0x0000, 0xffff);
    g->FillEllipse(560, 430, 50, 50);
    g->ForegroundColor(0x0000, 0xffff, 0xffff);
    points = new List<Point*>();
    points->Add(new Point(650, 380));
    points->Add(new Point(700, 425));
    points->Add(new Point(600, 425));
    points->Add(new Point(650, 380));
    g->FillPolygon(points, Graphics::FillConvex);
    for (UInt32 i=0; i<points->Count(); i++) {
      delete(points->At(i));
      }
    delete(points);
    g->DrawText(400, 575, (char*)"Test Text");
    g->BackgroundColor(0, 0, 0);
    g->DrawClearText(560, 575, (char*)"Test Text");
printf("text height = %d\n",g->TextHeight());
printf("baseline    = %d\n",g->TextBaseline());
printf("Text width  = %d\n",g->TextWidth("Test Text"));
    delete(g);
    }
  if (sender == dialogButton) {
    dialog = new OpenFileDialog(this, this->Width()/2-160, this->Height()/2-120, 320, 240);
    dialog->Pattern(new RegEx(".*\\.cpp"));
    result = (dialog->ShowDialog()).Result();
    if (result == DialogResult::OK) {
      obj = dialog->FileName();
      if (obj != NULL) {
printf("Item: %s\n",((String*)obj)->AsCharArray());
        }
      else {
printf("Null object\n");
        }
      }
    delete(dialog);
    }
  }

void MainWindow::CheckChangedEvent(void* sender, MouseEventArgs args) {
  CheckBox *cb;
  cb = (CheckBox*)sender;
  if (cb->Checked()) printf("Checkbox checked\n");
  else printf("Checkbox cleared\n");
  }

void MainWindow::RadioChangedEvent(void* sender, MouseEventArgs args) {
  RadioButton *rb;
  rb = (RadioButton*)sender;
  if (rb->Checked()) printf("Radiobutton %s checked\n",rb->Text().AsCharArray());
    else printf("Radiobutton %s cleared\n",rb->Text().AsCharArray());
  }

void MainWindow::ScrollChangedEvent(void* sender, ValueEventArgs args) {
  printf("Scroll value %f\n",args.Value());
  }

void MainWindow::SpinnerChangedEvent(void* sender, ValueEventArgs args) {
  printf("Spinner value %f\n",args.Value());
  }

void MainWindow::ListChangedEvent(void* sender, ValueEventArgs args) {
  ListBox* lb;
  int item;
  lb = (ListBox*)sender;
  item = args.Value();
  printf("List item %d ",item);
  if (lb->IsItemSelected(item)) printf(" selected");
    else printf(" cleared");
  
  printf("\n");
  }

void MainWindow::DropChangedEvent(void* sender, ValueEventArgs args) {
  Object* o;
  printf("drop changed, index = %f\n",args.Value());
  o = dropDown->SelectedItem();
  printf("item = %s\n",((String*)o)->AsCharArray());
  }

