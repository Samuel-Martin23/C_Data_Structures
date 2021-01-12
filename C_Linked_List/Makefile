
CC = clang
name_of_executable = program

object_files = main.o linked_list.o
template_dir = ../Template/

$(name_of_executable): $(object_files)
	$(CC) $^ -Wall -Wl,-rpath,$(template_dir) -L$(template_dir) -ltemplate -o $@

main.o: main.c
	$(CC) -c main.c -o main.o

linked_list.o: linked_list.c linked_list.h
	$(CC) -c linked_list.c -o linked_list.o

clean:
	rm *.o $(name_of_executable)