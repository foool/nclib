CC = g++
CFLAGS = -g -lpthread -I ..

ALL = matrank conftest battest test00 test01 test02 test03 test04 test05 test06 test07

all: $(ALL)

test%: test%.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o

battest: battest.o
		$(CC) $(CFLAG) -o $@ $< ../*.o

conftest: conftest.o
		$(CC) $(CFLAG) -o $@ $< ../*.o

matrank: matrank.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o

.cc.o:
		$(CC) $(CFLAGS) -c $*.cc

clean:
		rm -f *.o $(ALL)
