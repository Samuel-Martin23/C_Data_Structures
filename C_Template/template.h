
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define int_cast_void(value)            INT, &(int){value}
#define double_cast_void(value)         DOUBLE, &(double){value}
#define float_cast_void(value)          FLOAT, &(float){value}
#define char_cast_void(value)           CHAR, &(char){value}
#define str_cast_void(value)            STR, (char*){value}
#define bool_cast_void(value)           BOOL, &(bool){value}

#define int_array_cast_void(...)        INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define double_array_cast_void(...)     DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define float_array_cast_void(...)      FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define char_array_cast_void(...)       CHAR, (char[]){__VA_ARGS__}, sizeof((char[]){__VA_ARGS__})/sizeof((char[]){__VA_ARGS__}[0])
#define str_array_cast_void(...)        STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])
#define bool_array_cast_void(...)       BOOL, (bool[]){__VA_ARGS__}, sizeof((bool[]){__VA_ARGS__})/sizeof((bool[]){__VA_ARGS__}[0])

typedef enum template {INT, DOUBLE, FLOAT, CHAR, STR, BOOL} template_t;

int void_cast_int(void *value);
double void_cast_double(void *value);
float void_cast_float(void *value);
char void_cast_char(void *value);
const char *void_cast_str(void *value);
bool void_cast_bool(void *value);

void print_t(template_t T, void *value, const char *beginning, const char *end);

bool check_float_equal(float value1, float value2);
bool check_double_equal(double value1, double value2);
bool check_less_equal_value(template_t T, void *value1, void *value2);
bool check_greater_value(template_t T, void *value1, void *value2);
bool check_equal_value(template_t T, void *value1, void *value2);

int get_bytes(template_t T, void *value);

#endif /* TEMPLATE_H */
