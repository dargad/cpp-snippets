CXXFLAGS=-g
LDFLAGS=-lrt

all: client.o server.o
	clang++ $(LDFLAGS) client.o -o client
	clang++ $(LDFLAGS) server.o -o server
