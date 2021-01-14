
CC = clang
flags = -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break

object_files = main.o vector.o
template_dir = ../C_Template/
name_of_executable = program

$(name_of_executable): $(object_files)
	$(CC) $^ -Wall -Wl,-rpath,$(template_dir) -L$(template_dir) -ltemplate -o $@

main.o: main.c
	$(CC) $(flags) -c main.c -o main.o

vector.o: vector.c vector.h
	$(CC) $(flags) -c vector.c -o vector.o

clean:
	rm *.o $(name_of_executable)