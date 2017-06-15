CC = gcc
FLAGS = -lm -lreadline -I.
#HEADERS = stack.h strucon.h funcs.h multi.h sya.h

calc2: strucon.c stack.c funcs.c calc.c multi.c sya.c
	gcc -o calc2 strucon.c sya.c multi.c stack.c funcs.c calc.c $(FLAGS)

usr: stack.c funcs.c calc.c
	gcc -o /usr/local/bin/calc sya.c multi.c stack.c funcs.c calc.c -I. -lm -lreadline
