CC = clang
file_to_compile = allocation_metrics
name_of_lib = allocation_metrics

static:
	$(CC) -c $(file_to_compile).c -o $(file_to_compile).o
	ar -rcs lib$(name_of_lib).a $(file_to_compile).o
	rm $(file_to_compile).o

shared:
	$(CC) -dynamiclib $(file_to_compile).c -o lib$(name_of_lib).dylib -install_name @rpath/lib$(name_of_lib).dylib

clean:
	rm *.dylib *.a