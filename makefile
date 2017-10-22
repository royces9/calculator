CC = gcc
FLAGS = -lm -lreadline -I. -flto
HEADERS = sya.h stack.h multi.h onearg.h file.h twoarg.h operator.h
CFILES = sya.c stack.c multi.c onearg.c calc.c file.c twoarg.c oeprator.c
OFILES = sya.o stack.o multi.o onearg.o calc.o file.o twoarg.o operator.o

calc2: $(OFILES)
	$(CC) $(OFILES) -o calc2 $(FLAGS)

debug: $(OFILES)
	$(CC) -g $(OFILES) -o calc2 $(FLAGS)

stack.o: stack.c stack.h
	$(CC) -c stack.c

multi.o: multi.c multi.h stack.h onearg.h sya.h
	$(CC) -c multi.c

onearg.o: onearg.c onearg.h multi.h stack.h file.h operator.h
	$(CC) -c onearg.c

sya.o: sya.c sya.h stack.h onearg.h twoarg.h operator.h
	$(CC) -c sya.c

calc.o: calc.c stack.h sya.h
	$(CC) -c calc.c

file.o: file.c file.h stack.h sya.h
	$(CC) -c file.c

twoarg.o: twoarg.c twoarg.h operator.h
	$(CC) -c twoarg.c

operator.o: operator.c operator.h
	$(CC) -c operator.c

usr: $(OFILES)
	$(CC) $(OFILES) -o /usr/local/bin/calc $(FLAGS)

del: $(OFILES)
	del $(OFILES)
