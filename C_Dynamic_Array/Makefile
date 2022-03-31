CC := clang
FLAGS := -std=c17 -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break -Wno-sign-conversion
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
HEADERS := $(wildcard *.h)

EXECUTABLE_NAME := program

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

$(EXECUTABLE_NAME): $(OBJECTS)
	$(CC) $^ -o $@

clean:
	rm *.o
	rm $(EXECUTABLE_NAME)
