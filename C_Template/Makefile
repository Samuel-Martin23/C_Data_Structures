
CC = clang
file_to_compile = template
name_of_lib = template

static:
	$(CC) -c $(file_to_compile).c -o $(file_to_compile).o
	ar -r lib$(name_of_lib).a $(file_to_compile).o
	rm $(file_to_compile).o

shared:
	$(CC) -dynamiclib $(file_to_compile).c -o lib$(name_of_lib).dylib -install_name @rpath/lib$(name_of_lib).dylib

clean:
	rm *.dylib *.a

# Building all the files
# files = main vector linked_list hash_table
# object_files := $(foreach file, $(files), $(file).o)

# compile_all:
# 	@$(foreach file, $(files), $(CC) -c $(file).c -o $(file).o;)
# 	$(CC) $(object_files) -o program

# clean:
# 	rm *.o $(name_of_executable)



# Linking with no prefix
# $(CC) $^ -Wl,-rpath,. libc_template.dylib -o $@


# Linking static and shared
# ar -r $(lib_file).a $(lib_file).o
# $(CC) -dynamiclib $(lib_file).c -o $(lib_file).dylib


# Linking from a different folder
# $(CC) main.o -Wall -Wl,-rpath,../LC/ -L../LC/ -ladd -o program