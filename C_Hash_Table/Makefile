
CC = clang
flags = -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break

object_files = main.o hash_table.o
template_dir = ../C_Template/
name_of_executable = program

$(name_of_executable): $(object_files)
	$(CC) $^ -Wl,-rpath,$(template_dir) -L$(template_dir) -ltemplate -o $@

main.o: main.c
	$(CC) $(flags) -c main.c -o main.o

hash_table.o: hash_table.c hash_table.h
	$(CC) $(flags) -c hash_table.c -o hash_table.o

clean:
	rm *.o $(name_of_executable)