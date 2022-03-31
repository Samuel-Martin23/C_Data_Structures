CC = clang
flags = -std=c17 -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break

object_files = main.o dictionary.o
template_dir = ../C_Template/
allocation_metrics_dir = ../C_Allocation_Metrics/
name_of_executable = program

template_lib = -Wl,-rpath,$(template_dir) -L$(template_dir) -ltemplate
allocation_metrics_lib = -Wl,-rpath,$(allocation_metrics_dir) -L$(allocation_metrics_dir) -lallocation_metrics

$(name_of_executable): $(object_files)
	$(CC) $^ $(template_lib) $(allocation_metrics_lib) -o $@

main.o: main.c
	$(CC) $(flags) -c main.c -o main.o

dictionary.o: dictionary.c dictionary.h
	$(CC) $(flags) -c dictionary.c -o dictionary.o

clean:
	rm *.o $(name_of_executable)