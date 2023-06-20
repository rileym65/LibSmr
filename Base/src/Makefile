PLATFORM = linux
#PLATFORM = cygwin

ifeq ($(PLATFORM),cygwin)
PROJECT = SmrFramework
else
PROJECT = libSmrFramework.so.1
endif

CC = g++
LIBPATH = ./
LIBS =
INCPATH = -I.
INCS =
DEFS = -Wall -std=c++11 -fPIC
HEADERS = SmrFramework.h SmrList.h SmrSet.h SmrStack.h SmrQueue.h SmrArray.h
OBJS = \
	AssociativeArray.o \
	Base64.o \
	BitStream.o \
	Calculator.o \
	Callback.o \
	CastException.o \
	ComplexNumber.o \
	Convert.o \
	DateTime.o \
	Directory.o \
	Environment.o \
	Exception.o \
	File.o \
	FileInfo.o \
	FileStream.o \
	Flate.o \
	FormatException.o \
	Forth.o \
	ForthContext.o \
	ForthProgram.o \
	Fraction.o \
	GridObject.o \
	Hash.o \
	HexGrid.o \
	HrTimer.o \
	HttpListener.o \
	HttpRequest.o \
	HttpServer.o \
	Huffman.o \
	Image.o \
	ImageBMP.o \
	ImageGIF.o \
	ImageJPG.o \
	ImagePNG.o \
	ImageXBM.o \
	ImageXPM.o \
	IniFile.o \
	InvalidOpException.o \
	Json.o \
	Length.o \
	Lexer.o \
	Lfsr.o \
	Line.o \
	Log.o \
	LZW.o \
	Math.o \
	MemoryException.o \
	MemoryStream.o \
	NeuralNetwork.o \
	NotFoundException.o \
	NullException.o \
	Object.o \
	Palette.o \
	Parser.o \
	Point.o \
	Polygon.o \
	Random.o \
	RangeException.o \
	Rectangle.o \
	RegEx.o \
	RpnCalculator.o \
	Socket.o \
	StopWatch.o \
	Stream.o \
	StreamReader.o \
	StreamWriter.o \
	String.o \
	StringStream.o \
	System.o \
	Terminal.o \
	Tests.o \
	Thread.o \
	TimeSpan.o \
	Utils.o \
	Vector.o \

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
	cp cygSmrFramework.dll /usr/lib
	cp cygSmrFramework.dll /usr/bin
	cp libSmrFramework.dll.a /usr/lib
	cp Smr*.h /usr/include
	chmod a+r /usr/lib/cygSmrFramework.dll
	chmod a+r /usr/bin/cygSmrFramework.dll
	chmod a+r /usr/lib/libSmrFramework.dll.a
	chmod a+r /usr/include/Smr*.h
else
	cp $(PROJECT) /usr/lib
	cp Smr*.h /usr/include
	chmod a+r /usr/lib/$(PROJECT)
	chmod a+r /usr/include/SmrFramework.h
	chmod a+r /usr/include/Smr*.h
	-cd /usr/lib; sudo ln -s libSmrFramework.so.1 libSmrFramework.so
endif

AssociativeArray.o:    $(HEADERS) AssociativeArray.cpp
Base64.o:              $(HEADERS) Base64.cpp
BitStream.o:           $(HEADERS) BitStream.cpp
Calculator.o:          $(HEADERS) Calculator.cpp
Callback.o:            $(HEADERS) Callback.cpp
CastException.o:       $(HEADERS) CastException.cpp
ComplexNumber.o:       $(HEADERS) ComplexNumber.cpp
Convert.o:             $(HEADERS) Convert.cpp
DateTime.o:            $(HEADERS) DateTime.cpp
Directory.o:           $(HEADERS) Directory.cpp
Environment.o:         $(HEADERS) Environment.cpp
Exception.o:           $(HEADERS) Exception.cpp
File.o:                $(HEADERS) File.cpp
FileInfo.o:            $(HEADERS) FileInfo.cpp
FileStream.o:          $(HEADERS) FileStream.cpp
Flate.o:               $(HEADERS) Flate.cpp
FormatException.o:     $(HEADERS) FormatException.cpp
Forth.o:               $(HEADERS) Forth.cpp
ForthContext.o:        $(HEADERS) ForthContext.cpp
ForthProgram.o:        $(HEADERS) ForthProgram.cpp
Fraction.o:            $(HEADERS) Fraction.cpp
GridObject.o:          $(HEADERS) GridObject.cpp
Hash.o:                $(HEADERS) Hash.cpp
HexGrid.o:             $(HEADERS) HexGrid.cpp
HrTimer.o:             $(HEADERS) HrTimer.cpp
HttpListener.o:        $(HEADERS) HttpListener.cpp
HttpRequest.o:         $(HEADERS) HttpRequest.cpp
HttpServer.o:          $(HEADERS) HttpServer.cpp
Huffman.o:             $(HEADERS) Huffman.cpp
Image.o:               $(HEADERS) Image.cpp
ImageBMP.o:            $(HEADERS) ImageBMP.cpp
ImageGIF.o:            $(HEADERS) ImageGIF.cpp
ImageJPG.o:            $(HEADERS) ImageJPG.cpp
ImagePNG.o:            $(HEADERS) ImagePNG.cpp
ImageXBM.o:            $(HEADERS) ImageXBM.cpp
ImageXPM.o:            $(HEADERS) ImageXPM.cpp
IniFile.o:             $(HEADERS) IniFile.cpp
InvalidOpException.o:  $(HEADERS) InvalidOpException.cpp
Json.o:                $(HEADERS) Json.cpp
Length.o:              $(HEADERS) Length.cpp
Lexer.o:               $(HEADERS) Lexer.cpp
Lfsr.o:                $(HEADERS) Lfsr.cpp
Line.o:                $(HEADERS) Line.cpp
Log.o:                 $(HEADERS) Log.cpp
LZW.o:                 $(HEADERS) LZW.cpp
Math.o:                $(HEADERS) Math.cpp
MemoryException.o:     $(HEADERS) MemoryException.cpp
MemoryStream.o:        $(HEADERS) MemoryStream.cpp
NeuralNetwork.o:       $(HEADERS) NeuralNetwork.cpp
NotFoundException.o:   $(HEADERS) NotFoundException.cpp
NullException.o:       $(HEADERS) NullException.cpp
Object.o:              $(HEADERS) Object.cpp
Palette.o:             $(HEADERS) Palette.cpp
Parser.o:              $(HEADERS) Parser.cpp
Point.o:               $(HEADERS) Point.cpp
Polygon.o:             $(HEADERS) Polygon.cpp
Random.o:              $(HEADERS) Random.cpp
RangeException.o:      $(HEADERS) RangeException.cpp
Rectangle.o:           $(HEADERS) Rectangle.cpp
RegEx.o:               $(HEADERS) RegEx.cpp
RpnCalculator.o:       $(HEADERS) RpnCalculator.cpp
Socket.o:              $(HEADERS) Socket.cpp
StopWatch.o:           $(HEADERS) StopWatch.cpp
Stream.o:              $(HEADERS) Stream.cpp
StreamReader.o:        $(HEADERS) StreamReader.cpp
StreamWriter.o:        $(HEADERS) StreamWriter.cpp
String.o:              $(HEADERS) String.cpp
StringStream.o:        $(HEADERS) StringStream.cpp
System.o:              $(HEADERS) System.cpp
Terminal.o:            $(HEADERS) Terminal.cpp
Tests.o:               $(HEADERS) Tests.cpp
Thread.o:              $(HEADERS) Thread.cpp
TimeSpan.o:            $(HEADERS) TimeSpan.cpp
Utils.o:               $(HEADERS) Utils.cpp
Vector.o:              $(HEADERS) Vector.cpp

