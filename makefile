FLAGS = -lm -lreadline -I. -flto
MOREFLAGS = 
CC = gcc -Wall $(MOREFLAGS)
CCC = gcc

HFILES = $(wildcard *.h)
CFILES = $(wildcard *.c)
OFILES = $(wildcard *.o)

CALC2 = calc2
USR = /usr/local/bin/calc
TARGET =

all: MOREFLAGS += -static -g
all: main.o userFunctions.o matrix.o stack.o functions.o operatorUtility.o operator.o multi.o sya.o file.o fileStruct.o
all: TARGET = $(CALC2)
all: calc

usr: userFunctions.o matrix.o stack.o functions.o operatorUtility.o operator.o multi.o sya.o file.o fileStruct.o main.o
usr: TARGET = $(USR)
usr: calc


userFunctions.o: userFunctions.c userFunctions.h matrix.c matrix.h multi.c multi.h
	$(CC) -c userFunctions.c

matrix.o: matrix.c matrix.h operator.c operator.h
	$(CC) -c matrix.c

stack.o: stack.c stack.h
	$(CC) -c stack.c

multi.o: multi.c multi.h stack.c stack.h sya.c sya.h operator.c operator.h
	$(CC) -c multi.c

sya.o: sya.c sya.h stack.c stack.h operator.c operator.h
	$(CC) -c sya.c

file.o: file.c file.h stack.c stack.h sya.c sya.h fileStruct.c fileStruct.h
	$(CC) -c file.c

operator.o: operator.c operator.h operatorUtility.c operatorUtility.h stack.c stack.h functions.c functions.h multi.c multi.h file.c file.h sya.c sya.h
	$(CC) -c operator.c

operatorUtility.o: operatorUtility.c operatorUtility.h
	$(CC) -c operatorUtility.c

functions.o: functions.c functions.h operator.c operator.h
	$(CC) -c functions.c

fileStruct.o: fileStruct.c fileStruct.h
	$(CC) -c fileStruct.c

main.o: main.c stack.c stack.h sya.c sya.h
	$(CC) -c main.c

calc: $(OFILES)
	$(CCC) *.o -o $(TARGET) $(FLAGS)


del:
	del $(OFILES)

