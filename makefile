LDFLAGS = -lm -lreadline -flto
FLAGS = -MMD -Wall -Wpedantic
DEBUG_FLAGS =
CC = gcc

CFILES = $(wildcard *.c)
OFILES = $(addprefix $(OBJD)/, $(CFILES:.c=.o))

USR = calc
TARGET = calc_d

OBJD = obj

all: DEBUG_FLAGS += -g
all: $(TARGET)

release: TARGET = $(USR)
release: FLAGS += -O2
release: $(TARGET)


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
