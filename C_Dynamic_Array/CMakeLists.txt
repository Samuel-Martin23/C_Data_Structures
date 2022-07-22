cmake_minimum_required(VERSION 3.20)
project(C_Dynamic_Array)

if (${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    set(FLAGS "-std=c17 -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break")
    set(CMAKE_C_FLAGS ${FLAGS})
endif()

add_executable(${PROJECT_NAME} main.c dynamic_array.c dynamic_array_int.c)
target_link_libraries(${PROJECT_NAME} m)
