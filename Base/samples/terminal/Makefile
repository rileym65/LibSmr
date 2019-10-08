PROJECT = test
CC = g++
DEFS =
INCDIR =
INCS =
LIBDIR =
LIBS = -lSmrFramework
DEFS = -pthread -std=c++11
HEADERS = header.h
OBJS = \
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

main.o:             $(HEADERS) main.cpp
