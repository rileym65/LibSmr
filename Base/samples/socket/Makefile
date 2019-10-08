CC = g++
DEFS =
INCDIR =
INCS =
LIBDIR =
LIBS = -lSmrFramework
DEFS = -pthread -std=c++11

build: server client

server: server.o
	$(CC) $(DEFS) $(LIBDIR) server.o $(LIBS) -o server

client: client.o
	$(CC) $(DEFS) $(LIBDIR) client.o $(LIBS) -o client

.c.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

.cpp.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

clean:
	-rm server
	-rm client
	-rm *.o

server.o:	server.cpp
client.o:	client.cpp

