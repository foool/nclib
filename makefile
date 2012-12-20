CC = g++
CFLAGS = -g -Wall -Wextra -fPIC -I$(HOME)/include 

ALL = config.o galois.o gmatrix.o utils.o libnc.so
all: $(ALL)

clean:
	rm -f core *.o $(ALL) a.out *.so

.SUFFIXES: .cc .o
.cc.o:
	$(CC) $(CFLAGS) -c $*.cc
config.o: config.h
galois.o: galois.h
nc.o: nc.h galois.h
libnc.so:
	$(CC) -fPIC t03.cc -o libnc.so -shared config.o galois.o gmatrix.o utils.o
