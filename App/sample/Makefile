PROJECT = test
CC = g++
DEFS =
INCDIR = -I/usr/include/freetype2
INCS =
LIBDIR =
LIBS = -lX11 -lSmrFramework -lSmrAppFramework -lXft
DEFS = -pthread -std=c++11 -Wall
HEADERS = header.h FileDialog.h
OBJS = \
	MainApplication.o \
	MainWindow.o \
	MainWindow_init.o \
	main.o

$(PROJECT): $(OBJS)
	$(CC) $(DEFS) $(LIBDIR) $(OBJS) $(LIBS) -o $(PROJECT)

.c.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

.cpp.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

clean:
	-rm $(PROJECT)
	-rm *.o

MainApplication.o:  $(HEADERS) MainApplication.cpp
MainWindow.o:       $(HEADERS) MainWindow.cpp
MainWindow_init.o:  $(HEADERS) MainWindow_init.cpp
main.o:             $(HEADERS) main.cpp

