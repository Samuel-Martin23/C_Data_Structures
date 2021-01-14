
CC = clang
flags = -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break

object_files = main.o linked_list.o
template_dir = ../C_Template/
name_of_executable = program

$(name_of_executable): $(object_files)
	$(CC) $^ -Wall -Wl,-rpath,$(template_dir) -L$(template_dir) -ltemplate -o $@

main.o: main.c
	$(CC) $(flags) -c main.c -o main.o

linked_list.o: linked_list.c linked_list.h
	$(CC) $(flags) -c linked_list.c -o linked_list.o

clean:
	rm *.o $(name_of_executable)