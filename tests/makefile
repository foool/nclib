CC = g++
CFLAGS = -O3 -g -lpthread -I ..

ALL = 	matcodtest conftest battest mctest gftest\
		test00 test01 test02 test03 test04 test05 test06 test07 test08\
		test09 test10 test11 test12 test13 test14 test15 test16 test17\
		test18 test19 test20 test21 test22 test23 test24 test25\
		test50 test51 test52

all: $(ALL)

test%: test%.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

codcompare: codcompare.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

battest: battest.o
		$(CC) $(CFLAG) -o $@ $< ../*.o ../*.a

conftest: conftest.o
		$(CC) $(CFLAG) -o $@ $< ../*.o ../*.a

matcodtest: matcodtest.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

mctest: mctest.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

gftest: gftest.o
		$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

.cc.o:
		$(CC) $(CFLAGS) -c $*.cc

clean:
		rm -f *.o $(ALL)
