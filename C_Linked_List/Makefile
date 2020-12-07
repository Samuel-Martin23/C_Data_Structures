
object_files=main.o linked_list.o
name_of_executable=executable

$(name_of_executable): $(object_files)
	cc $^ -o $@

main.o: main.c
	cc -c main.c -o main.o

linked_list.o: linked_list.c linked_list.h
	cc -c linked_list.c -o linked_list.o

clear:
	rm *.o $(name_of_executable)