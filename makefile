LDFLAGS = -lm -lreadline -flto
FLAGS = -MMD -Wall -Wpedantic
DEBUG_FLAGS = -g
CC = gcc

CFILES = $(wildcard *.c)
OFILES = $(addprefix $(OBJD)/, $(CFILES:.c=.o))

RELEASE = calc
TARGET = calc_d

OBJD = obj

all: FLAGS += $(DEBUG_FLAGS)
all: $(TARGET)

release: TARGET = $(RELEASE)
release: FLAGS += -O2
release: $(TARGET)


$(TARGET): $(OFILES)
	$(CC) $^ -o $(TARGET) $(LDFLAGS)

$(OBJD)/%.o: %.c | $(OBJD)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJD):
	mkdir $(OBJD)

.PHONY: clean
clean:
	del $(OBJD)/*.o $(OBJD)/*.d $(USR) calc calc_d

-include $(CFILES:.c=.d)
