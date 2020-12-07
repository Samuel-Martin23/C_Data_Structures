
object_files=main.o vector.o
name_of_executable=executable

$(name_of_executable): $(object_files)
	cc $^ -o $@

main.o: main.c
	cc -c main.c -o main.o

vector.o: vector.c vector.h
	cc -c vector.c -o vector.o

clean:
	rm *.o $(name_of_executable)