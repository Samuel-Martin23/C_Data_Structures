
object_files=main.o hash_table.o


executable: $(object_files)
	cc $^ -o $@

main.o: main.c
	cc -c main.c -o main.o

hash_table.o: hash_table.c hash_table.h
	cc -c hash_table.c -o hash_table.o

clean:
	rm *.o