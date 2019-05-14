LDFLAGS = -lm -lreadline -flto
FLAGS = -MMD -Wall -Wpedantic
DEBUG_FLAGS =
CC = gcc

CFILES = $(wildcard *.c)
OFILES = $(addprefix $(OBJD)/, $(CFILES:.c=.o))

USR = /usr/local/bin/calc
TARGET = calc2

OBJD = obj

all: DEBUG_FLAGS += -g
all: $(TARGET)

usr: TARGET = $(USR)
usr: FLAGS += -O2
usr: $(TARGET)



$(TARGET): $(OFILES)
	$(CC) $^ -o $(TARGET) $(LDFLAGS)

$(OBJD)/%.o: %.c | $(OBJD)
	$(CC) $(FLAGS) $(DEBUG_FLAGS) -c $< -o $@

$(OBJD):
	mkdir $(OBJD)

.PHONY: clean
clean:
	del $(OBJD)/*.o $(OBJD)/*.d

-include $(CFILES:.c=.d)
