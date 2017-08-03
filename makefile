CC = gcc
FLAGS = -lm -lreadline -I.
HEADERS = strucon.h sya.h stack.h multi.h funcs.h
CFILES = strucon.c sya.c stack.c multi.c funcs.c calc.c

calc2: $(HEADERS) $(CFILES)
	$(CC) -o calc2 $(CFILES) $(FLAGS)

usr: $(HEADERS) $(CFILES)
	$(CC) -o /usr/local/bin/calc $(CFILES) $(FLAGS)

