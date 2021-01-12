
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define IntCastVoid(value)          INT, &(int){value}
#define DoubleCastVoid(value)       DOUBLE, &(double){value}
#define FloatCastVoid(value)        FLOAT, &(float){value}
#define CharCastVoid(value)         CHAR, &(char){value}
#define StrCastVoid(value)          STR, (char*){value}

#define IntCastVoidArray(...)       INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define DoubleCastVoidArray(...)    DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define FloatCastVoidArray(...)     FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define CharCastVoidArray(...)      CHAR, (char[]){__VA_ARGS__}, sizeof((char[]){__VA_ARGS__})/sizeof((char[]){__VA_ARGS__}[0])
#define StrCastVoidArray(...)       STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])

typedef enum Type_Template {INT, DOUBLE, FLOAT, CHAR, STR} Template;

int VoidCastInt(void *value);
double VoidCastDouble(void *value);
float VoidCastFloat(void *value);
char VoidCastChar(void *value);
const char *VoidCastStr(void *value);

void PrintT(Template T, void *value, const char *beginning, const char *end);

bool CheckLessEqualValue(Template T, void *value1, void *value2);
bool CheckGreaterValue(Template T, void *value1, void *value2);
bool CheckEqualValue(Template T, void *value1, void *value2);

int SizeOfType(Template T, void *value);

#endif /* TEMPLATE_H */