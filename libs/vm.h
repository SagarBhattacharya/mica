#ifndef MICA_VM_H
#define MICA_VM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "parser.h"

typedef struct machine{
  int top;
  Value* stack;
  Parser* parser;
  void (*execute)(struct machine* self);
  void (*dispose)(struct machine* self);
} Machine;

Machine* NewMachine(const char* source);

#endif