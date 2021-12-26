CC=g++
OPTS=-g -std=c++11

all: main.o predictor.o
	$(CC) $(OPTS) -lm -o predictor main.o predictor.o

main.o: main.c predictor.h
	$(CC) $(OPTS) -c main.c

predictor.o: predictor.h predictor.cpp
	$(CC) $(OPTS) -c predictor.cpp

clean:
	rm -f *.o predictor;
