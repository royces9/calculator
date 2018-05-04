CC = gcc
FLAGS = -lm -lreadline -I. -flto
HEADERS = $(wildcard *.h)
CFILES = $(wildcard *.c)
OFILES = $(wildcard *.o)

all: matrix.o stack.o multi.o sya.o calc.o file.o operator.o fileStruct.o calc2

calc2: $(CFILES) $(HEADERS) $(OFILES)
	$(CC) $(OFILES) -o calc2 $(FLAGS)

matrix.o: matrix.c matrix.h
	$(CC) -c matrix.c

stack.o: stack.c stack.h matrix.h
	$(CC) -c stack.c

multi.o: multi.c multi.h stack.h sya.h
	$(CC) -c multi.c

sya.o: sya.c sya.h stack.h operator.h
	$(CC) -c sya.c

calc.o: calc.c stack.h sya.h
	$(CC) -c calc.c

file.o: file.c file.h stack.h sya.h fileStruct.h
	$(CC) -c file.c

operator.o: operator.c operator.h operatorUtility.h
	$(CC) -c operator.c

fileStruct.o: fileStruct.c fileStruct.h
	$(CC) -c fileStruct.c

debug: $(OFILES)
	$(CC) -g $(OFILES) -o calc2 $(FLAGS)

usr: $(OFILES)
	$(CC) $(OFILES) -o /usr/local/bin/calc $(FLAGS)

del: $(OFILES)
	del $(OFILES)

