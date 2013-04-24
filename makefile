CC = g++
CFLAGS = -O3 -g -Wall -Wextra -fPIC  -I$(HOME)/include 

ALL = config.o galois.o gmatrix.o gmatrix64.o gmatrixu8.o mcoding.o property.o utils.o nc.o
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
	$(CC) -fPIC -o libnc.so -shared config.o galois.o gmatrix.o gmatrix64.o utils.o
