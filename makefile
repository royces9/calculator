calc2: stack.c funcs.c calc.c
	gcc -o calc2 stack.c funcs.c calc.c -I. -lm -lreadline
