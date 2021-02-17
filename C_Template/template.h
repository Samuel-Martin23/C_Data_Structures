
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "../C_Allocation_Metrics/allocation_metrics.h"

typedef enum template {NONE, INT, DOUBLE, FLOAT, CHAR, STR, BOOL} template_t;

int void_cast_int(void *value);
double void_cast_double(void *value);
float void_cast_float(void *value);
char void_cast_char(void *value);
char *void_cast_str(void *value);
bool void_cast_bool(void *value);

void print_t(template_t T, void *value, const char *beginning, const char *end);

bool check_float_equal(float value_1, float value_2);
bool check_double_equal(double value_1, double value_2);

bool check_less_value(template_t T, void *value_1, void *value_2, bool ascii);
bool check_greater_value(template_t T, void *value_1, void *value_2, bool ascii);
bool check_less_equal_value(template_t T, void *value_1, void *value_2, bool ascii);
bool check_greater_equal_value(template_t T, void *value_1, void *value_2, bool ascii);
bool check_equal_value(template_t T, void *value_1, void *value_2, bool ascii);
bool check_not_equal_value(template_t T, void *value_1, void *value_2, bool ascii);

size_t get_bytes(template_t T, void *value);
void *new_T_value(template_t T, void *value);
void free_T_value(template_t T, void *value);
void *new_arg_T_value(template_t T, va_list args);

#endif /* TEMPLATE_H */
