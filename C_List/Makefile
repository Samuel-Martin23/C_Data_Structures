
CC = clang
flags = -std=c17 -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break

object_files = main.o list.o
template_dir = ../C_Template/
name_of_executable = program

$(name_of_executable): $(object_files)
	$(CC) $^ -Wl,-rpath,$(template_dir) -L$(template_dir) -ltemplate -o $@

main.o: main.c
	$(CC) $(flags) -c main.c -o main.o

list.o: list.c list.h
	$(CC) $(flags) -c list.c -o list.o

clean:
	rm *.o $(name_of_executable)