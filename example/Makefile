CPP   = g++-4.7
FLAGS	= -std=c++11 -g -Wall -Werror
LIBS	= 



all: dirs itest

dirs:
	mkdir -p objs bin

itest: objs/main.o
	$(CPP) $(FLAGS) $(LIBS) -o itest objs/main.o

objs/main.o: main.cc ../src/Filter.h ../src/FIter.h
	$(CPP) -c $(FLAGS) $(LIBS) -o objs/main.o main.cc

clean:
	rm -f itest objs/*
