CC = g++
CFLAGS = -g -Wall -lpthread -I ..

ALL = frctest reed_soltest test1 test2

all: $(ALL)

srctest: srctest.o
		$(CC) $(CFLAGS) -o $@ $< ../src.o ../../*.o

reed_soltest: reed_soltest.o
		$(CC) $(CFLAGS) -o $@ $< ../reed_sol.o ../../*.o

frctest: frctest.o
	$(CC) $(CFLAGS) -o $@ $< ../frc.o ../../*.o ../../*.a

test1: test1.o
	$(CC) $(CFLAGS) -o $@ $< ../../*.o

test2: test2.o
	$(CC) $(CFLAGS) -o $@ $< ../../*.o


.cc.o:
		$(CC) $(CFLAGS) -c $*.cc

clean:
		rm -f *.o $(ALL)
