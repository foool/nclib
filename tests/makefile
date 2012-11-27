CC = g++
CFLAGS = -g -lpthread -I ..

ALL = matrank test00 test01 test02 test03 test04 test05 test06 test07 test08 test09 test10 test11 test12 test13 test14 test15 test16 test17

all: $(ALL)

test%: test%.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o

matrank: matrank.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o

.cc.o:
		$(CC) $(CFLAGS) -c $*.cc

clean:
		rm -f *.o $(ALL)
