#ifndef VALUE_H
#define VALUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum value_type {
  VALUE_INT, VALUE_FLOAT,VALUE_CHAR,
} ValueType;

typedef struct value {
  ValueType type;
  union val {
    int64_t asInt;
    double asFloat;
    char asChar;
  };
} Value;

#define IntValue(x) (Value) {.asInt = (x), .type = VALUE_INT}
#define FloatValue(x) (Value) {.asFloat = (x), .type = VALUE_FLOAT}
#define CharValue(x) (Value) {.asChar = (x), .type = VALUE_CHAR}

#endif