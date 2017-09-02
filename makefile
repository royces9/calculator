CC = gcc
FLAGS = -lm -lreadline -I. -flto
HEADERS = sya.h stack.h multi.h funcs.h
CFILES = sya.c stack.c multi.c funcs.c calc.c
<<<<<<< HEAD
=======
OFILES = sya.o stack.o multi.o funcs.o calc.o
>>>>>>> master

calc2: $(OFILES)
	$(CC) $(OFILES) -o calc2 $(FLAGS)

debug: $(OFILES)
	$(CC) -g $(OFILES) -o calc2 $(FLAGS)

stack.o: stack.c stack.h
	$(CC) -c stack.c

multi.o: multi.c multi.h stack.h funcs.h sya.h
	$(CC) -c multi.c

funcs.o: funcs.c funcs.h multi.h stack.h
	$(CC) -c funcs.c

sya.o: sya.c sya.h stack.h funcs.h
	$(CC) -c sya.c

calc.o: calc.c stack.h sya.h
	$(CC) -c calc.c

usr: $(OFILES)
	$(CC) $(OFILES) -o /usr/local/bin/calc $(FLAGS)

del: $(OFILES)
	del $(OFILES)
