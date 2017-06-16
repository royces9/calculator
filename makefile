CC = gcc
FLAGS = -lm -lreadline -I.
#HEADERS = stack.h strucon.h funcs.h multi.h sya.h

calc2: strucon.c stack.c funcs.c calc.c multi.c sya.c
	gcc -o calc2 strucon.c sya.c multi.c stack.c funcs.c calc.c $(FLAGS)

usr: strucon.c stack.c funcs.c calc.c multi.c sya.c
	gcc -o /usr/local/bin/calc strucon.c sya.c multi.c stack.c funcs.c calc.c $(FLAGS)

