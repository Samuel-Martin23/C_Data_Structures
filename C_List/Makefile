
object_files=main.o list.o

executable: $(object_files)
	cc $^ -o $@

main.o: main.c
	cc -c main.c -o main.o

list.o: list.c list.h
	cc -c list.c -o list.o

clear:
	rm *.o