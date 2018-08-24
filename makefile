FLAGS = -Wall -lm -lreadline -I. -flto
DEBUG_FLAGS =
CC = gcc $(DEBUG_FLAGS)
CCC = gcc

CFILES = $(wildcard *.c)

USR = /usr/local/bin/calc
TARGET = calc2

all: DEBUG_FLAGS += -static -g
all: calc

usr: TARGET = $(USR)
usr: calc

calc: $(CFILES:.c=.o)
	$(CCC) $^ -o $(TARGET) $(FLAGS)

%.o: %.c
	$(CC) -MMD -c $<

.PHONY: cleanall
cleanall:
	del *.o *.d

.PHONY: cleano
cleano:
	del *.o

.PHONY: cleand
cleand:
	del *.d

-include $(CFILES:.c=.d)
