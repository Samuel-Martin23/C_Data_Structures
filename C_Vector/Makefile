
CC = clang
name_of_executable = program

object_files = main.o vector.o
template_dir = ../Template/

$(name_of_executable): $(object_files)
	$(CC) $^ -Wall -Wl,-rpath,$(template_dir) -L$(template_dir) -ltemplate -o $@

main.o: main.c
	$(CC) -c main.c -o main.o

vector.o: vector.c vector.h
	$(CC) -c vector.c -o vector.o

clean:
	rm *.o $(name_of_executable)