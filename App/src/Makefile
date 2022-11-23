PLATFORM = linux
#PLATFORM = cygwin
#PLATFORM = wsl
#PLATFORM = pi

ifeq ($(PLATFORM),cygwin)
PROJECT = SmrAppFramework
else
PROJECT = libSmrAppFramework.so.1
endif

CC = g++
LIBPATH = ./
LIBS =
INCPATH = -I. -I/usr/include/freetype2
INCS =
DEFS = -Wall -std=c++11 -fpic
HEADERS = SmrAppFramework.h
OBJS = \
	Application.o \
	Button.o \
	CheckBox.o \
	Color.o \
	Component.o \
	Control.o \
	CustomEventArgs.o \
	Dialog.o \
	DialogResult.o \
	DropDownList.o \
	EventArgs.o \
	EventHandler.o \
	Font.o \
	Form.o \
	Graphics.o \
	GroupBox.o \
	HScrollBar.o \
	KeyEventArgs.o \
	Label.o \
	ListBox.o \
	MouseCursor.o \
	MouseEventArgs.o \
	OpenFileDialog.o \
	PaintEventArgs.o \
	Panel.o \
	PictureBox.o \
	RadioButton.o \
	ResizeEventArgs.o \
	ScrollBar.o \
	Spinner.o \
	TextBox.o \
	Timer.o \
	Toggle.o \
	ValueEventArgs.o \
	VScrollBar.o

ifeq ($(PLATFORM),wsl)
DEFS += -DNO_FTX
endif

ifeq ($(PLATFORM),linux)
DEFS += -DUSE_BACKING_STORE
endif

ifeq ($(PLATFORM),cygwin)
LIBCMD = $(CC) -shared -o cyg$(PROJECT).dll \
	-Wl,--out-implib=lib$(PROJECT).dll.a \
	-Wl,--export-all-symbols \
	-Wl,--enable-auto-import \
	-Wl,--whole-archive $(OBJS) \
	-Wl,--no-whole-archive -lX11
else
LIBCMD = $(CC) -shared -Xlinker -h -Xlinker $(PROJECT) $(OBJS) -o $(PROJECT)
endif

$(PROJECT): $(OBJS)
	-rm $(PROJECT)
	$(LIBCMD)

.cpp.o:
	$(CC) $(DEFS) $(INCPATH) $(INCS) -c $<

clean:
ifeq ($(PLATFORM),cygwin)
	-rm *.dll
	-rm *.o
	-rm *.a
else
	-rm $(PROJECT)
	-rm *.o
endif

install:
ifeq ($(PLATFORM),cygwin)
	cp cygSmrAppFramework.dll /usr/lib
	cp cygSmrAppFramework.dll /usr/bin
	cp libSmrAppFramework.dll.a /usr/lib
	cp Smr*.h /usr/include
	chmod a+r /usr/lib/cygSmrAppFramework.dll
	chmod a+r /usr/bin/cygSmrAppFramework.dll
	chmod a+r /usr/lib/libSmrAppFramework.dll.a
	chmod a+r /usr/include/Smr*.h
else
	cp $(PROJECT) /usr/lib
	cp Smr*.h /usr/include
	chmod a+r /usr/lib/$(PROJECT)
	chmod a+r /usr/include/SmrAppFramework.h
	-cd /usr/lib; ln -s libSmrAppFramework.so.1 libSmrAppFramework.so
endif

Application.o:         $(HEADERS) Application.cpp
Button.o:              $(HEADERS) Button.cpp
CheckBox.o:            $(HEADERS) CheckBox.cpp
Color.o:               $(HEADERS) Color.cpp
Component.o:           $(HEADERS) Component.cpp
Control.o:             $(HEADERS) Control.cpp
CustomEventArgs.o:     $(HEADERS) CustomEventArgs.cpp
Dialog.o:              $(HEADERS) Dialog.cpp
DialogResult.o:        $(HEADERS) DialogResult.cpp
DropDownList.o:        $(HEADERS) DropDownList.cpp
EventArgs.o:           $(HEADERS) EventArgs.cpp
Eventhandler.o:        $(HEADERS) EventHandler.cpp
Font.o:                $(HEADERS) Font.cpp
Form.o:                $(HEADERS) Form.cpp
Graphics.o:            $(HEADERS) Graphics.cpp
GroupBox.o:            $(HEADERS) GroupBox.cpp
HScrollBar.o:          $(HEADERS) HScrollBar.cpp
KeyEventArgs.o:        $(HEADERS) KeyEventArgs.cpp
Label.o:               $(HEADERS) Label.cpp
ListBox.o:             $(HEADERS) ListBox.cpp
MouseCursor.o:         $(HEADERS) MouseCursor.cpp
MouseEventArgs.o:      $(HEADERS) MouseEventArgs.cpp
OpenFileDialog.o:      $(HEADERS) OpenFileDialog.cpp
PaintEventArgs.o:      $(HEADERS) PaintEventArgs.cpp
Panel.o:               $(HEADERS) Panel.cpp
PictureBox.o:          $(HEADERS) PictureBox.cpp
RadioButton.o:         $(HEADERS) RadioButton.cpp
ResizeEventArgs.o:     $(HEADERS) ResizeEventArgs.cpp
ScrollBar.o:           $(HEADERS) ScrollBar.cpp
Spinner.o:             $(HEADERS) Spinner.cpp
TextBox.o:             $(HEADERS) TextBox.cpp
Timer.o:               $(HEADERS) Timer.cpp
Toggle.o:              $(HEADERS) Toggle.cpp
ValueEventArgs.o:      $(HEADERS) ValueEventArgs.cpp
VScrollBar.o:          $(HEADERS) VScrollBar.cpp

