#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

typedef struct machine{
  int top;
  int* stack;
  Parser* parser;
  void (*execute)(struct machine* self);
  void (*dispose)(struct machine* self);
} Machine;

Machine* NewMachine(const char* source);

#endif