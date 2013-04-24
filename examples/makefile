CC = g++
CFLAGS = -O3 -g -lpthread -I ..

ALL = hybrid codcompare matrank shorten w64cod u8coding speedM2\
	  speedXOR speed04M2 speed08M2 speed16M2 speed32M2 speed64M2\
	  speedMCP speedCOD u8shift gfrcfunc findfrc sfindfrc\
	  nkfindfrc vfindfrc \
	  frcfunc vfrcfunc tvfrcfunc tfrcfunc lfrcfunc 
#frcfunc original
#vfrcfunc add RP_property
#tvfrcfunc add output
#tfrcfunc add tags for matrix
#lfrcfunc add CL_property

all: $(ALL)

hybrid: hybrid.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

codcompare: codcompare.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

matrank: matrank.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

shorten: shorten.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

w64cod: w64cod.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

u8coding:u8coding.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

speedM2:speedM2.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

speed04M2:speed04M2.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

speed08M2:speed08M2.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

speed16M2:speed16M2.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

speed32M2:speed32M2.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

speed64M2:speed64M2.o
	$(CC) $(CFLAGS)  -o $@ $< ../*.o ../*.a

speedXOR:speedXOR.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

speedMCP:speedMCP.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

speedCOD:speedCOD.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

u8shift:u8shift.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

frcfunc:frcfunc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

gfrcfunc:gfrcfunc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

findfrc:findfrc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

sfindfrc:sfindfrc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

nkfindfrc:nkfindfrc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

vfindfrc:vfindfrc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

vfrcfunc:vfrcfunc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

tvfrcfunc:tvfrcfunc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

tfrcfunc:tfrcfunc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a

lfrcfunc:lfrcfunc.o
	$(CC) $(CFLAGS) -o $@ $< ../*.o ../*.a


.cc.o:
	$(CC) $(CFLAGS) -c $*.cc

clean:
	rm -f *.o $(ALL)
