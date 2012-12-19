CC = g++
CFLAGS = -g -Wall -lpthread -I ..

ALL = srctest reed_soltest

all: $(ALL)

srctest: srctest.o
		$(CC) $(CFLAGS) -o $@ $< ../src.o ../../*.o

reed_soltest: reed_soltest.o
		$(CC) $(CFLAGS) -o $@ $< ../reed_sol.o ../../*.o

.cc.o:
		$(CC) $(CFLAGS) -c $*.cc

clean:
		rm -f *.o $(ALL)
