
object_files=main.o linked_list.o

executable: $(object_files)
	cc $^ -o $@

main.o: main.c
	cc -c main.c -o main.o

linked_list.o: linked_list.c linked_list.h
	cc -c linked_list.c -o linked_list.o

clear:
	rm *.o