#ifndef _SMR_APP_FRAMEWORK_H
#define _SMR_APP_FRAMEWORK_H

#define USEXFT

#include <sys/types.h>
#include <sys/socket.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <time.h>
#include <exception>
#include <termios.h>
#include <SmrFramework.h>

#ifdef USEXFT
#include <X11/Xft/Xft.h>
#endif

namespace SmrFramework {

class Control;

#define KEY_LEFT        XK_Left
#define KEY_RIGHT       XK_Right
#define KEY_UP          XK_Up
#define KEY_DOWN        XK_Down
#define KEY_HOME        XK_Home
#define KEY_END         XK_End
#define KEY_PGUP        XK_Page_Up
#define KEY_PGDN        XK_Page_Down
#define KEY_ESCAPE      XK_Escape
#define KEY_BACKSPACE   XK_BackSpace
#define KEY_TAB         XK_Tab
#define KEY_LINE_FEED   XK_LineFeed
#define KEY_ENTER       XK_Return
#define KEY_CLEAR       XK_Clear
#define KEY_PAUSE       XK_Pause
#define KEY_SCROLL_LOCK XK_Scroll_Lock
#define KEY_DELETE      XK_Delete
#define KEY_INSERT      XK_Insert
#define KEY_KP_0        XK_KP_0
#define KEY_KP_1        XK_KP_1
#define KEY_KP_2        XK_KP_2
#define KEY_KP_3        XK_KP_3
#define KEY_KP_4        XK_KP_4
#define KEY_KP_5        XK_KP_5
#define KEY_KP_6        XK_KP_6
#define KEY_KP_7        XK_KP_7
#define KEY_KP_8        XK_KP_8
#define KEY_KP_9        XK_KP_9
#define KEY_KP_UP       XK_KP_Up
#define KEY_KP_DOWN     XK_KP_Down
#define KEY_KP_LEFT     XK_KP_Left
#define KEY_KP_RIGHT    XK_KP_Right
#define KEY_KP_INSERT   XK_KP_Insert
#define KEY_KP_DELETE   XK_KP_Delete
#define KEY_KP_ENTER    XK_KP_Enter
#define KEY_KP_HOME     XK_KP_Home
#define KEY_KP_END      XK_KP_End
#define KEY_KP_PGUP     XK_KP_Page_Up
#define KEY_KP_PGDN     XK_KP_Page_Down
#define KEY_KP_ADD      XK_KP_Add
#define KET_KP_SUBTRACT XK_KP_Subtract
#define KET_KP_MULTIPLY XK_KP_Multiply
#define KET_KP_DIVIDE   XK_KP_Divide
#define KET_KP_DECIMAL  XK_KP_Decimal
#define KEY_SHIFT_LEFT  XK_Shift_L
#define KEY_SHIFT_RIGHT XK_Shift_R
#define KEY_ALT_LEFT    XK_Alt_L
#define KEY_ALT_RIGHT   XK_Alt_R
#define KEY_CTRL_LEFT   XK_Control_L
#define KEY_CTRL_RIGHT  XK_Control_R
#define KEY_CAPS_LOCK   XK_Caps_Lock
#define KEY_SHIFT_LOCK  XK_Shift_Lock
#define KEY_NUM_LOCK    XK_Num_Lock
#define KEY_F1          XK_F1
#define KEY_F2          XK_F2
#define KEY_F3          XK_F3
#define KEY_F4          XK_F4
#define KEY_F5          XK_F5
#define KEY_F6          XK_F6
#define KEY_F7          XK_F7
#define KEY_F8          XK_F8
#define KEY_F9          XK_F9
#define KEY_F10         XK_F10
#define KEY_F11         XK_F11
#define KEY_F12         XK_F12


  class EventArgs : public Object {
    protected:
      Int32  i1;
      Int32  i2;
      Int32  i3;
      Int32  i4;
      Int32  i5;
      Double d1;
      Byte data[32];
    public:
      EventArgs();
      virtual ~EventArgs();
    };

  class ResizeEventArgs : public EventArgs {
    public:
      ResizeEventArgs(int w, int h);
      virtual ~ResizeEventArgs();
      Int32 Width();
      Int32 Height();
    };

  class MouseEventArgs : public EventArgs {
    public:
      MouseEventArgs(int x, int y, int button, int state);
      virtual ~MouseEventArgs();
      Int32 X();
      Int32 Y();
      Int32 Button();
      Int32 State();
    };

  class KeyEventArgs : public EventArgs {
    public:
      KeyEventArgs(int key);
      virtual ~KeyEventArgs();
      Int32   Key();
    };

  class ValueEventArgs : public EventArgs {
    public:
      ValueEventArgs(double value);
      virtual ~ValueEventArgs();
      Double  Value();
    };

  class PaintEventArgs : public EventArgs {
    public:
      PaintEventArgs(int x,int y,int width,int height,int count);
      virtual ~PaintEventArgs();
      Int32 X();
      Int32 Y();
      Int32 Width();
      Int32 Height();
      Int32 Count();
    };
  
  class CustomEventArgs : public EventArgs {
    protected:
    public:
      CustomEventArgs(Byte* data, Byte len);
      ~CustomEventArgs();
      Byte* Data();
    };

  class Component : public Object {
    protected:
    public:
      Component();
      virtual ~Component();
      virtual int IsContainer();
      virtual int IsRadioButton();
    };
  
  class EventHandler : Object {
    protected:
      void (*callback_obj)(void*,void*,EventArgs);
      void *object;
    public:
      EventHandler(void (*cb)(void*,void*,EventArgs),void* obj);
      virtual ~EventHandler();
      virtual void Call(void* sender,EventArgs args);
      };

  class Color {
    protected:
      int red;
      int green;
      int blue;
      u_int32_t pixel;
    public:
      Color(int r, int g, int b);
      virtual ~Color();
      static u_int32_t RGB(int r, int g, int b);
      virtual u_int32_t Pixel();
      static u_int32_t AliceBlue();
      static u_int32_t AntiqueWhite();
      static u_int32_t Aqua();
      static u_int32_t Aquamarine();
      static u_int32_t Azure();
      static u_int32_t Beige();
      static u_int32_t Bisque();
      static u_int32_t Black();
      static u_int32_t BlanchedAlmond();
      static u_int32_t Blue();
      static u_int32_t BlueViolet();
      static u_int32_t Brown();
      static u_int32_t Burlywood();
      static u_int32_t CadetBlue();
      static u_int32_t Chartreuse();
      static u_int32_t Chocolate();
      static u_int32_t Coral();
      static u_int32_t CornflowerBlue();
      static u_int32_t Cornsilk();
      static u_int32_t Crimson();
      static u_int32_t Cyan();
      static u_int32_t DarkBlue();
      static u_int32_t DarkCyan();
      static u_int32_t DarkGoldenrod();
      static u_int32_t DarkGray();
      static u_int32_t DarkGreen();
      static u_int32_t DarkKhaki();
      static u_int32_t DarkMagenta();
      static u_int32_t DarkOliveGreen();
      static u_int32_t DarkOrange();
      static u_int32_t DarkOrchid();
      static u_int32_t DarkRed();
      static u_int32_t DarkSalmon();
      static u_int32_t DarkSeaGreen();
      static u_int32_t DarkSlateBlue();
      static u_int32_t DarkSlateGray();
      static u_int32_t DarkTurquoise();
      static u_int32_t DarkViolet();
      static u_int32_t DeepPink();
      static u_int32_t DeepSkyBlue();
      static u_int32_t DimGray();
      static u_int32_t DodgerBlue();
      static u_int32_t Firebrick();
      static u_int32_t FloralWhite();
      static u_int32_t ForestGreen();
      static u_int32_t Fuchsia();
      static u_int32_t Gainsboro();
      static u_int32_t GhostWhite();
      static u_int32_t Gold();
      static u_int32_t Goldenrod();
      static u_int32_t Gray();
      static u_int32_t WebGray();
      static u_int32_t Green();
      static u_int32_t WebGreen();
      static u_int32_t GreenYellow();
      static u_int32_t Honeydew();
      static u_int32_t HotPink();
      static u_int32_t IndianRed();
      static u_int32_t Indigo();
      static u_int32_t Ivory();
      static u_int32_t Khaki();
      static u_int32_t Lavender();
      static u_int32_t LavenderBlush();
      static u_int32_t LawnGreen();
      static u_int32_t LemonChiffon();
      static u_int32_t LightBlue();
      static u_int32_t LightCoral();
      static u_int32_t LightCyan();
      static u_int32_t LightGoldenrod();
      static u_int32_t LightGray();
      static u_int32_t LightGreen();
      static u_int32_t LightPink();
      static u_int32_t LightSalmon();
      static u_int32_t LightSeaGreen();
      static u_int32_t LightSkyBlue();
      static u_int32_t LightSlateGray();
      static u_int32_t LightSteelBlue();
      static u_int32_t LightYellow();
      static u_int32_t Lime();
      static u_int32_t LimeGreen();
      static u_int32_t Linen();
      static u_int32_t Magenta();
      static u_int32_t Maroon();
      static u_int32_t WebMaroon();
      static u_int32_t MediumAquamarine();
      static u_int32_t MediumBlue();
      static u_int32_t MediumOrchid();
      static u_int32_t MediumPurple();
      static u_int32_t MediumSeaGreen();
      static u_int32_t MediumSlateBlue();
      static u_int32_t MediumSpringGreen();
      static u_int32_t MediumTurquoise();
      static u_int32_t MediumVioletRed();
      static u_int32_t MidnightBlue();
      static u_int32_t MintCream();
      static u_int32_t MistyRose();
      static u_int32_t Moccasin();
      static u_int32_t NavajoWhite();
      static u_int32_t NavyBlue();
      static u_int32_t OldLace();
      static u_int32_t Olive();
      static u_int32_t OliveDrab();
      static u_int32_t Orange();
      static u_int32_t OrangeRed();
      static u_int32_t Orchid();
      static u_int32_t PaleGoldenrod();
      static u_int32_t PaleGreen();
      static u_int32_t PaleTurquoise();
      static u_int32_t PaleVioletRed();
      static u_int32_t PapayaWhip();
      static u_int32_t PeachPuff();
      static u_int32_t Peru();
      static u_int32_t Pink();
      static u_int32_t Plum();
      static u_int32_t PowderBlue();
      static u_int32_t Purple();
      static u_int32_t WebPurple();
      static u_int32_t RebeccaPurple();
      static u_int32_t Red();
      static u_int32_t RosyBrown();
      static u_int32_t RoyalBlue();
      static u_int32_t SaddleBrown();
      static u_int32_t Salmon();
      static u_int32_t SandyBrown();
      static u_int32_t SeaGreen();
      static u_int32_t Seashell();
      static u_int32_t Sienna();
      static u_int32_t Silver();
      static u_int32_t SkyBlue();
      static u_int32_t SlateBlue();
      static u_int32_t SlateGray();
      static u_int32_t Snow();
      static u_int32_t SpringGreen();
      static u_int32_t SteelBlue();
      static u_int32_t Tan();
      static u_int32_t Teal();
      static u_int32_t Thistle();
      static u_int32_t Tomato();
      static u_int32_t Turquoise();
      static u_int32_t Violet();
      static u_int32_t Wheat();
      static u_int32_t White();
      static u_int32_t WhiteSmoke();
      static u_int32_t Yellow();
      static u_int32_t YellowGreen();
    };

  class Graphics : public Object {
    protected:
      Display     *display;
      Window       window;
      Control     *control;
      GC           gc;
      XFontStruct* font;
      UInt32       lineWidth;
      UInt32       lineStyle;
      UInt32       capStyle;
      UInt32       joinStyle;
      UInt32       backgroundColor;
      UInt32       foregroundColor;
#ifdef USEXFT
      XftFont   *xftfont;
      XftDraw   *xftdrawable;
      XftColor   xftcolor;
      XGlyphInfo ginfo;
#endif
    public:
      static const int LineStyleSolid = LineSolid;
      static const int LineStyleOnOffDash = LineOnOffDash;
      static const int LineStyleDoubleDash = LineDoubleDash;
      static const int CapStyleNotLast = CapNotLast;
      static const int CapStyleButt = CapButt;
      static const int CapStyleRound = CapRound;
      static const int CapStyleProjecting = CapProjecting;
      static const int JoinStyleMiter = JoinMiter;
      static const int JoinStyleRound = JoinRound;
      static const int JoinStyleBevel = JoinBevel;
      static const int FillComplex = Complex;
      static const int FillConvex = Convex;
      static const int FillNonConvex = Nonconvex;
      Graphics(Control* ctrl);
//      Graphics(Display *display, Window window);
      virtual ~Graphics();
      virtual UInt32    BackgroundColor();
      virtual UInt32    BackgroundColor(int r, int g, int b);
      virtual UInt32    BackgroundColor(UInt32 pixel);
      virtual UInt32    BackgroundColor(Color* color);
      virtual UInt32    CapStyle();
      virtual UInt32    CapStyle(UInt32 w);
      virtual UInt32    ForegroundColor();
      virtual UInt32    ForegroundColor(int r, int g, int b);
      virtual UInt32    ForegroundColor(UInt32 pixel);
      virtual UInt32    ForegroundColor(Color* color);
      virtual UInt32    JoinStyle();
      virtual UInt32    JoinStyle(UInt32 w);
      virtual UInt32    LineStyle();
      virtual UInt32    LineStyle(UInt32 w);
      virtual UInt32    LineWidth();
      virtual UInt32    LineWidth(UInt32 w);
      virtual void      Clear();
      virtual void      DrawClearText(int x, int y, const char* text);
      virtual void      DrawClearText(int x, int y, String* text);
      virtual void      DrawEllipse(int x, int y, int width, int height);
      virtual void      DrawImage(int x, int y, Image* image);
      virtual void      DrawImage(Drawable dest, int x, int y, Image* image);
      virtual void      DrawLine(int x1, int y1, int x2, int y2);
      virtual void      DrawLine(Point start, Point end);
      virtual void      DrawLine(Line* line);
      virtual void      DrawLines(List<Point*>* points);
      virtual void      DrawPoint(int x, int y);
      virtual void      DrawPoint(Point point);
      virtual void      DrawRectangle(int x, int y, int width, int height);
      virtual void      DrawRectangle(Point start, Point end);
      virtual void      DrawRectangle(Line line);
      virtual void      DrawRectangle(Rectangle rect);
      virtual void      DrawText(int x, int y, const char* text);
      virtual void      DrawText(int x, int y, String* text);
      virtual void      FillEllipse(int x, int y, int width, int height);
      virtual void      FillPolygon(List<Point*>* points, int hint);
      virtual void      FillRectangle(int x, int y, int width, int height);
      virtual void      FillRectangle(Point start, Point end);
      virtual void      FillRectangle(Line line);
      virtual void      FillRectangle(Rectangle rect);
      virtual void      Flush();
      virtual int       TextBaseline();
      virtual int       TextHeight();
      virtual int       TextWidth(const char* text);
      virtual int       TextWidth(String* text);
    };

  class MouseCursor : public Object {
    protected:
      Byte cursorType;
      Byte cursorNumber;
      Cursor cursor;
    public:
      static const Byte Predefined = 1;
      static const Byte Image = 2;
      MouseCursor();
      MouseCursor(UInt32 i);
      virtual ~MouseCursor();
      UInt32   CursorNumber();
      UInt32   CursorNumber(UInt32 i);
      Byte     CursorType();
      Byte     CursorType(Byte b);
    };

  class Control : public Component {
    protected:
      Display  *display;
      Window    window;
      int       screen;
      XWindowAttributes winattr;
      Control  *parent;
      UInt32    x;
      UInt32    y;
      UInt32    width;
      UInt32    height;
      Boolean   visible;
      Boolean   enabled;
      int       borderWidth;
      UInt32    backgroundColor;
      UInt32    foregroundColor;
      UInt32    borderColor;
      long      inputMask;
      String    text;
      String    font;
      Image    *backgroundImage;
      Pixmap    backgroundPixmap;
      Image    *disabledImage;
      Pixmap    disabledPixmap;
      String    tag;
      Cursor    cursor;
#ifdef USEXFT
      XftFont   *xftfont;
      XftDraw   *xftdrawable;
      XftColor   xftcolor;
      XGlyphInfo ginfo;
#endif
      EventHandler* mouseDownHandler;
      EventHandler* mouseUpHandler;
      EventHandler* mouseMoveHandler;
      EventHandler* keyDownHandler;
      EventHandler* keyUpHandler;
      EventHandler* paintHandler;
      EventHandler* customHandler;
      EventHandler* mouseEnterHandler;
      EventHandler* mouseLeaveHandler;
      EventHandler* resizedHandler;
    public:
      Control(Control* parent, int x, int y, int w, int h);
      virtual ~Control();
      virtual UInt32    BackgroundColor();
      virtual UInt32    BackgroundColor(UInt32 pixel);
      virtual UInt32    BackgroundColor(int red, int green, int blue);
      virtual Image*    BackgroundImage();
      virtual Image*    BackgroundImage(Image* i);
      virtual Image*    DisabledImage();
      virtual Image*    DisabledImage(Image* i);
      virtual void      UseParentBackground();
      virtual UInt32    BorderColor();
      virtual UInt32    BorderColor(UInt32 pixel);
      virtual UInt32    BorderColor(UInt32 red, UInt32 green, UInt32 blue);
      virtual UInt32    BorderWidth();
      virtual UInt32    BorderWidth(UInt32 w);
      virtual void      BringToFront();
      virtual Boolean   Enabled();
      virtual Boolean   Enabled(Boolean b);
      virtual String    Font();
      virtual String    Font(const char* font);
      virtual String    Font(String font);
      virtual String    Font(String* font);
      virtual UInt32    ForegroundColor();
      virtual UInt32    ForegroundColor(int red, int green, int blue);
      virtual UInt32    ForegroundColor(UInt32 pixel);
      virtual UInt32    Height();
      virtual UInt32    Height(UInt32 h);
      virtual void      Location(int x, int y);
      virtual void      LostFocus();
      virtual void      SendCustomEvent(void* data, Byte len);
      virtual void      Size(int w, int h);
      virtual String    Tag();
      virtual String    Tag(String s);
      virtual String    Text();
      virtual String    Text(const char* text);
      virtual String    Text(String text);
      virtual String    Text(String* text);
      virtual Boolean   Visible();
      virtual Boolean   Visible(Boolean state);
      virtual UInt32    Width();
      virtual UInt32    Width(UInt32 w);
      virtual Window    GetWindow();
      virtual Graphics* GetGraphics();
      virtual void      OnMouseDown(EventHandler* handler);
      virtual void      OnMouseUp(EventHandler* handler);
      virtual void      OnMouseMove(EventHandler* handler);
      virtual void      OnKeyDown(EventHandler* handler);
      virtual void      OnKeyUp(EventHandler* handler);
      virtual void      OnPaint(EventHandler* handler);
      virtual void      OnCustom(EventHandler* handler);
      virtual void      OnMouseEnter(EventHandler* handler);
      virtual void      OnMouseLeave(EventHandler* handler);
      virtual void      OnResize(EventHandler* handler);
      virtual int       ProcessEvent(XEvent event);
      virtual void      MouseDownEvent(int x, int y, int button);
      virtual void      MouseUpEvent(int x, int y, int button);
      virtual void      MouseMoveEvent(int x, int y, int state);
      virtual void      KeyDownEvent(KeySym key, XComposeStatus status);
      virtual void      KeyUpEvent(KeySym key, XComposeStatus status);
      virtual void      EnterEvent(int x, int y);
      virtual void      LeaveEvent(int x, int y);
      virtual void      ExposeEvent(int x, int y, int w, int h, int count);
      virtual void      ConfigureEvent(int x, int y, int w, int h);
      virtual void      FocusInEvent();
      virtual void      FocusOutEvent();
      virtual void      Redraw();
      virtual void      SendToBack();
      virtual UInt32    X();
      virtual UInt32    X(UInt32 i);
      virtual UInt32    Y();
      virtual UInt32    Y(UInt32 i);
      virtual Display*  _Display();
      virtual Window    _Window();
#ifdef USEXFT
      virtual XftFont*   _XftFont();
      virtual XftDraw*   _XftDrawable();
      virtual XftColor   _XftColor();
      virtual XGlyphInfo _Ginfo();
#endif
      };
  
  class Label : public Control {
    protected:
      Byte align;
      Int32 textOffsetX;
      Int32 textOffsetY;
    public:
      static const int LEFT = 0;
      static const int CENTER = 1;
      static const int RIGHT = 2;
      Label(Control* parent, int x, int y, int w, int h, const char* text);
      virtual ~Label();
      virtual Byte Align();
      virtual Byte Align(Byte b);
      virtual void Redraw();
      virtual Int32 TextOffsetX();
      virtual Int32 TextOffsetX(Int32 i);
      virtual Int32 TextOffsetY();
      virtual Int32 TextOffsetY(Int32 i);
    };

  class TextBox : public Label {
  protected:
    EventHandler* textChangedHandler;
    Byte          alphaCase;
    Word          maxLen;
    Boolean       readOnly;
    Byte          validation;
    Boolean       valid(String* t);
    Byte widths[256];
    int  cursor_x;
    int  pos;
    int  text_x;
    int  ascent;
    int  descent;
    Boolean hasFocus;
    void _updateCursorPosition();
    void _setupFont();
  public:
    static const Byte ValidationNone = 0;
    static const Byte ValidationInt = 1;
    static const Byte ValidationUInt = 2;
    static const Byte ValidationFloat = 3;
    static const Byte ValidationUFloat = 4;
    static const Byte ValidationHex = 5;
    static const Byte ValidationAlpha = 6;
    static const Byte ValidationAlphaNumeric = 7;
    static const Byte AlphaCaseNone = 0;
    static const Byte AlphaCaseUpper = 1;
    static const Byte AlphaCaseLower = 2;
    TextBox(Control*   parent, int x, int y, int w, int h);
    TextBox(Control*   parent, int x, int y, int w, int h, const char* text);
    ~TextBox();
    virtual void       Redraw();
    virtual Byte       AlphaCase();
    virtual Byte       AlphaCase(Byte b);
    virtual void       EnterEvent(int x, int y);
    virtual void       KeyUpEvent(KeySym key, XComposeStatus status);
    virtual void       LeaveEvent(int x, int y);
    virtual void       LostFocus();
    virtual Word       MaxLen();
    virtual Word       MaxLen(Word w);
    virtual void       MouseDownEvent(int x, int y, int button);
    virtual void       OnTextChanged(EventHandler* handler);
    virtual Boolean    ReadOnly();
    virtual Boolean    ReadOnly(Boolean b);
    virtual String     Text();
    virtual String     Text(const char* text);
    virtual String     Text(String text);
    virtual Byte       Validation();
    virtual Byte       Validation(Byte b);
    };

  class Spinner : public Control {
    protected:
      Int32 minValue;
      Int32 maxValue;
      Int32 value;
      Int32 step;
      EventHandler* valueChangedHandler;
    public:
      Spinner(Control* parent, int x, int y, int w, int h);
      virtual ~Spinner();
      virtual Int32 MaxValue();
      virtual Int32 MaxValue(Int32 i);
      virtual Int32 MinValue();
      virtual Int32 MinValue(Int32 i);
      virtual Int32 Step();
      virtual Int32 Step(Int32 i);
      virtual Int32 Value();
      virtual Int32 Value(Int32 i);
      virtual void OnValueChanged(EventHandler* handler);
      virtual void MouseUpEvent(int x, int y, int button);
      virtual void Redraw();
    };

  class PictureBox : public Control {
    protected:
      Image*  picture;
      UInt32  offsetX;
      UInt32  offsetY;
      Byte    mode;
      Boolean center;
    public:
      static const int MODE_STANDARD  = 0x00;
      static const int MODE_FIT       = 0x01;
      static const int MODE_STRETCH   = 0x02;
      PictureBox(Control* parent, int x, int y, int w, int h);
      virtual ~PictureBox();
      virtual Boolean Center();
      virtual Boolean Center(Boolean b);
      virtual Byte    Mode();
      virtual Byte    Mode(Byte m);
      virtual UInt32  OffsetX();
      virtual UInt32  OffsetX(UInt32 x);
      virtual UInt32  OffsetY();
      virtual UInt32  OffsetY(UInt32 y);
      virtual Image*  Picture();
      virtual Image*  Picture(Image* i);
      virtual void    Redraw();
    };

  class Button : public Control {
    protected:
      int           pressed;
      u_int32_t     pressedColor;
      Image        *pressedImage;
      Pixmap        pressedPixmap;
      EventHandler* pressHandler;
      EventHandler* releaseHandler;
    public:
      Button(Control* parent, int x, int y, int w, int h);
      virtual ~Button();
      virtual int  IsPressed();
      virtual Image* PressedImage();
      virtual Image* PressedImage(Image* i);
      virtual void SetPressedColor(int red, int green, int blue);
      virtual void OnPress(EventHandler* handler);
      virtual void OnRelease(EventHandler* handler);
      virtual void MouseDownEvent(int x, int y, int button);
      virtual void MouseUpEvent(int x, int y, int button);
      virtual Boolean Pressed();
      virtual Boolean Pressed(Boolean b);
      virtual void Redraw();
      virtual Boolean   Visible();
      virtual Boolean   Visible(Boolean state);
    };
  
  class Toggle : public Button {
    protected:
    public:
      Toggle(Control* parent, int x, int y, int w, int h);
      virtual ~Toggle();
      virtual void MouseDownEvent(int x, int y, int button);
      virtual void MouseUpEvent(int x, int y, int button);
    };

  class CheckBox : public Control {
    protected:
      Boolean       checked;
      EventHandler* checkChangedHandler;
      virtual void  changeCheck(Boolean checked);
    public:
      CheckBox(Control* parent, int x, int y, int w, int h);
      virtual ~CheckBox();
      virtual Boolean  Checked();
      virtual Boolean  Checked(Boolean checked);
      virtual void OnCheckChanged(EventHandler* handler);
      virtual void MouseUpEvent(int x, int y, int button);
      virtual void Redraw();
    };

  class RadioButton : public CheckBox {
    protected:
    public:
      RadioButton(Control* parent, int x, int y, int w, int h);
      virtual ~RadioButton();
      virtual void MouseUpEvent(int x, int y, int button);
      virtual void Redraw();
      virtual int IsRadioButton();
    };

  class ScrollBar : public Control {
    protected:
      double minValue;
      double maxValue;
      double puckRange;
      double pixelValue;
      int    puckPosition;
      int    puckLength;
      int    low;
      int    high;
      int    puckGrabbed;
      int    mouseOffset;
      double value;
      EventHandler* valueChangedHandler;
      void   computePuck();
      void   computeGraphics(int longSide, int shortSide);
      void   updateValue(double v);
      void   movePuck(int amount);
      void   adjustPuck(int pointer);
    public:
      ScrollBar(Control* parent,int x,int y,int w,int h);
      virtual ~ScrollBar();
      virtual double MinValue();
      virtual double MinValue(double v);
      virtual double MaxValue(double v);
      virtual double MaxValue();
      virtual double PuckRange();
      virtual double PuckRange(double v);
      virtual double Value();
      virtual double Value(double v);
      virtual void   OnValueChanged(EventHandler* handler);
    };

  class HScrollBar : public ScrollBar {
    protected:
    public:
      HScrollBar(Control* parent,int x,int y,int w,int h);
      virtual ~HScrollBar();
      virtual void MouseDownEvent(int x, int y, int button);
      virtual void MouseUpEvent(int x, int y, int button);
      virtual void MouseMoveEvent(int x, int y, int state);
      virtual void Redraw();
      virtual UInt32 Height(UInt32 h);
      virtual void   Size(int w, int h);
      virtual UInt32 Width(UInt32 w);
    };

  class VScrollBar : public ScrollBar {
    protected:
    public:
      VScrollBar(Control* parent,int x,int y,int w,int h);
      virtual ~VScrollBar();
      virtual void MouseDownEvent(int x, int y, int button);
      virtual void MouseUpEvent(int x, int y, int button);
      virtual void MouseMoveEvent(int x, int y, int state);
      virtual void Redraw();
      virtual UInt32 Height(UInt32 h);
      virtual void Size(int w, int h);
      virtual UInt32 Width(UInt32 w);
    };

  class Panel : public Control {
    protected:
      Control **controls;
      int       numControls;
    public:
      Panel(Control* parent, int x, int y, int w, int h);
      virtual ~Panel();
      virtual void Add(Control* control);
      virtual void Remove(Control* control);
      virtual int  ProcessEvent(XEvent event);
      virtual int  IsContainer();
      virtual void ProcessRadioButtons(RadioButton* trigger);
    };

  class ListBox : public Panel {
    protected:
      int            top;
      List<Object*> *items;
      char*          selected;
      VScrollBar*    scroller;
      int            lineHeight;
      int            lineCount;
      int            selectMultiple;
      int            pos;
      Int32          (*comparer)(Object*,Object*);
      EventHandler*  valueChangedHandler;
      void           scrollChangedEvent(void* sender, ValueEventArgs args);
      void static    _ScrollChangedEvent(void* obj, void* sender, EventArgs args);
    public:
      ListBox(Control* parent,int x,int y,int w,int h);
      virtual ~ListBox();
      virtual void    Add(Object* o);
      virtual void    Clear();
      virtual UInt32  Count();
      virtual int     FirstSelectedIndex();
      virtual int     IsItemSelected(int i);
      virtual Object* ItemAt(UInt32 i);
      virtual int     SelectedIndex();
      virtual Object* SelectedItem();
      virtual void    MouseDownEvent(int x, int y, int button);
      virtual void    MouseUpEvent(int x, int y, int button);
      virtual void    OnValueChanged(EventHandler* handler);
      virtual int     NextSelectedIndex();
      virtual void    Redraw();
      virtual void    SetSelectMultiple(int i);
      virtual void    Sort();
      virtual void    Comparer(Int32 (*func)(Object*,Object*));
    };

  class DropDownList : public Panel {
    protected:
      int            top;
      List<Object*> *items;
      VScrollBar*    scroller;
      int            lineHeight;
      int            lineCount;
      int            selectedIndex;
      UInt32         maxVisibleItems;
      Boolean        opened;
      UInt32         baseHeight;
      UInt32         baseY;
      EventHandler*  valueChangedHandler;
      void           scrollChangedEvent(void* sender, ValueEventArgs args);
      void static    _ScrollChangedEvent(void* obj, void* sender, EventArgs args);
    public:
      DropDownList(Control* parent,int x,int y,int w,int h);
      virtual ~DropDownList();
      virtual void    Add(Object* o);
      virtual void    RemoveItemAt(UInt32 i);
      virtual void    Clear();
      virtual UInt32  Count();
      virtual Object* ItemAt(UInt32 i);
      virtual List<Object*>* Items();
      virtual UInt32  MaxVisibleItems();
      virtual UInt32  MaxVisibleItems(UInt32 i);
      virtual int     SelectedIndex();
      virtual int     SelectedIndex(Int32 i);
      virtual Object* SelectedItem();
      virtual void    MouseDownEvent(int x, int y, int button);
      virtual void    MouseUpEvent(int x, int y, int button);
      virtual void    OnValueChanged(EventHandler* handler);
      virtual void    Redraw();
    };
    
  class GroupBox : public Panel {
    protected:
    public:
      GroupBox(Control* parent, int x, int y, int w, int h);
      virtual ~GroupBox();
      virtual void Redraw();
      };

  class Form : public Panel {
    protected:
    public:
      Form(Control* parent, int x, int y, int w, int h);
      virtual ~Form();
      virtual String Text();
      virtual String Text(const char* text);
      };
  
  class DialogResult : public Object {
    protected:
      Byte result;
    public:
      static const Byte OK = 1;
      static const Byte CANCEL = 0;
      DialogResult();
      DialogResult(Byte r);
      virtual ~DialogResult();
      virtual Byte Result();
    };
  
  class Dialog : public Form {
    protected:
      Boolean running;
      DialogResult result;
    public:
      Dialog(Control* parent, int x, int y, int w, int h);
      virtual ~Dialog();
      virtual DialogResult ShowDialog();
      virtual void         Close();
      virtual void         Ok();
      virtual void         Cancel();
      virtual DialogResult Result();
    };

class OpenFileDialog : public Dialog {
  protected:
    ListBox *fileList;
    Button  *okButton;
    Button  *cancelButton;
    RegEx   *pattern;
    String   path;
    void     refreshFiles();
    void     OkButtonReleased(void* sender, MouseEventArgs args);
    void     CancelButtonReleased(void* sender, MouseEventArgs args);
    Int32 static _fileDialogSorter(Object* a, Object* b);
    void  static _OkButtonReleased(void* obj, void* sender, EventArgs args);
    void  static _CancelButtonReleased(void* obj, void* sender, EventArgs args);
  public:
    OpenFileDialog(Control* parent, int x, int y, int w, int h);
    virtual ~OpenFileDialog();
    virtual String* FileName();
    virtual String* Path();
    virtual String* Path(String* s);
    virtual RegEx*  Pattern();
    virtual RegEx*  Pattern(RegEx* r);
    virtual DialogResult ShowDialog();
    };


  class Timer : public Object {
    protected:
      struct timeval lastTime;
      UInt32  millis;
      UInt32  interval;
      UInt32  count;
      Boolean running;
      Boolean stopOnTick;
      EventHandler*  tickHandler;
    public:
      Timer();
      ~Timer();
      UInt32  Count();
      UInt32  Interval();
      UInt32  Interval(UInt32 ms);
      void    OnTick(EventHandler* h);
      Boolean Running();
      void    Start();
      void    Stop();
      Boolean StopOnTick();
      Boolean StopOnTick(Boolean b);
      void    Tick();
    };

  class Application : public Object {
    protected:
      int      terminate;
      Display *display;
      void     getColorMapping();
      Form   **forms;
      UInt32   numForms;
      Dialog **dialogs;
      UInt32   numDialogs;
      Control* focus;
      XWindowAttributes screenAttributes;
      Timer  **timers;
      UInt32   numTimers;
      UInt32   loopDelay;
    public:
      int      red_mask;
      int      green_mask;
      int      blue_mask;
      int      red_shift;
      int      green_shift;
      int      blue_shift;
      Application();
      virtual ~Application();
      virtual Display *GetDisplay();
      virtual void     AddDialog(Dialog* d);
      virtual void     AddForm(Form* form);
      virtual void     AddTimer(Timer* t);
      virtual void     DoEvents();
      virtual Control* Focus();
      virtual Control* Focus(Control* c);
      virtual void     Init();
      virtual UInt32   LoopDelay();
      virtual UInt32   LoopDelay(UInt32 i);
      virtual void     RemoveDialog(Dialog* d);
      virtual void     RemoveForm(Form* f);
      virtual void     RemoveTimer(Timer* t);
      virtual void     Run();
      virtual void     Stop();
      virtual int      ScreenWidth();
      virtual int      ScreenHeight();
      virtual int      ScreenDepth();
    };

  extern Application* application;
  }

#endif

