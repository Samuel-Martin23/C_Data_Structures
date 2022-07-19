cmake_minimum_required(VERSION 3.20)
project(C_Hash_Table)

if (${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    set(FLAGS "-std=c17 -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break")
    set(CMAKE_C_FLAGS ${FLAGS})
endif()

add_executable(${PROJECT_NAME} main.c hash_table.c ht_str_int.c)
