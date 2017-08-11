CC = gcc
FLAGS = -lm -lreadline -I. -flto
HEADERS = sya.h stack.h multi.h funcs.h
CFILES = sya.c stack.c multi.c funcs.c calc.c

calc2: $(HEADERS) $(CFILES)
	$(CC) -o calc2 $(CFILES) $(FLAGS)

usr: $(HEADERS) $(CFILES)
	$(CC) -o /usr/local/bin/calc $(CFILES) $(FLAGS)

