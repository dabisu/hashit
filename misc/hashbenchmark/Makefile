# Makefile for the hash benchmark. The programs are compiled statically
# to have them run as fast as possible.

CC        = gcc 
CXX       = g++
LD        = gcc 
OPTFLAGS  = -O2 -DNDEBUG
CFLAGS    = $(OPTFLAGS) -Wall
CXXFLAGS  = $(OPTFLAGS) -Wall
LDFLAGS   = -static

TARGETS   = bench_stl \
            bench_glibc \
            bench_hashit \
	    bench_ght \
	    mean

all:	$(TARGETS)

# Rules
.SUFFIXES: .c
.c.o: %.o:%.c
	$(CC) -c $(CFLAGS) $<

.cc.o: %.o:%.cc
	$(CXX) -c $(CXXFLAGS) $<

mean: mean.o
	$(LD) -o mean mean.o

bench_stl: bench_stl.o bench.o bench_main.o
	$(LD) $(LDFLAGS) -o bench_stl bench_stl.o bench.o bench_main.o -lstdc++

bench_ght: bench_ght.o bench.o bench_main.o
	$(LD) $(LDFLAGS) -o bench_ght bench_ght.o bench.o bench_main.o -lghthash

bench_glibc: bench_glibc.o bench.o bench_main.o
	$(LD) $(LDFLAGS) -o bench_glibc bench_glibc.o bench.o bench_main.o -lc

bench_hashit: bench_hashit.o bench.o bench_main.o
	$(LD) $(LDFLAGS) -o bench_hashit bench_hashit.o bench.o bench_main.o -lhashit

clean:
	@rm -f *.o  $(TARGETS) lib/$(LIBS) lib/$(SHARED_LIBS)\
	@rm -f *~ core *.gcov *.da *.bb *.bbg *.log *.aux gmon.out logfile

