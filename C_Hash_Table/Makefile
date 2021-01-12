
CC = clang
name_of_executable = program

object_files = main.o hash_table.o
template_dir = ../Template/

$(name_of_executable): $(object_files)
	$(CC) $^ -Wall -Wl,-rpath,$(template_dir) -L$(template_dir) -ltemplate -o $@

main.o: main.c
	$(CC) -c main.c -o main.o

hash_table.o: hash_table.c hash_table.h
	$(CC) -c hash_table.c -o hash_table.o

clean:
	rm *.o $(name_of_executable)