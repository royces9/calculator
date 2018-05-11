CC = gcc
FLAGS = -lm -lreadline -I. -flto
HEADERS = $(wildcard *.h)
CFILES = $(wildcard *.c)
OFILES = $(wildcard *.o)

all: matrix.o stack.o functions.o operatorUtility.o operator.o multi.o sya.o file.o fileStruct.o calc.o calc2

calc2: $(CFILES) $(HEADERS) $(OFILES)
	$(CC) $(OFILES) -o calc2 $(FLAGS)

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

calc.o: calc.c stack.c stack.h sya.c sya.h
	$(CC) -c calc.c

debug: $(OFILES)
	$(CC) -g $(OFILES) -o calc2 $(FLAGS)

usr: $(OFILES)
	$(CC) $(OFILES) -o /usr/local/bin/calc $(FLAGS)

del: $(OFILES)
	del $(OFILES)

