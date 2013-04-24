CC = g++
CFLAGS = -g -Wall -fPIC -I$(HOME)/include 

ALL = src.o reed_sol.o frc.o
all: $(ALL)

clean:
		rm -f core *.o $(ALL) a.out *.so

.SUFFIXES: .cc .o
.cc.o:
		$(CC) $(CFLAGS) -c $*.cc

src.o: src.h
reed_sol.o: reed_sol.h
frc.o: frc.h
