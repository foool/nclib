CC = g++
CFLAGS = -g -lpthread -I ..

ALL = hybrid

all: $(ALL)

hybrid: hybrid.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o

.cc.o:
	$(CC) $(CFLAGS) -c $*.cc

clean:
	rm -f *.o $(ALL)
