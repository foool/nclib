CC = g++
CFLAGS = -g -fPIC -I$(HOME)/include 

ALL = config.o galois.o gmatrix.o utils.o nccloud.o libnc.so
all: $(ALL)

clean:
	rm -f core *.o $(ALL) a.out *.so

.SUFFIXES: .cc .o
.cc.o:
	$(CC) $(CFLAGS) -c $*.cc
config.o: config.h
galois.o: galois.h
nc.o: nc.h galois.h
nccloud.o: nc.h
libnc.so:
	$(CC) -fPIC t03.cc -o libnc.so -shared config.o galois.o gmatrix.o utils.o nccloud.o
