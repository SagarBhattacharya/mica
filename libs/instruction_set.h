#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
  INS_PUSH,
  INS_POP,
  INS_ADD,
  INS_SUB,
  INS_MUL,
  INS_DIV,
  INS_DUP,
  INS_SWAP,
  INS_PRINT
} InstructionType;

typedef struct {
  InstructionType type;
  int value;
} Instruction;

typedef struct machine{
  int top;
  int* stack;
  Instruction* program;
  int program_length;
  void (*execute)(struct machine* self);
  void (*dispose)(struct machine* self);
} Machine;

#define PUSH(v) {INS_PUSH, v}
#define POP {INS_POP, 0}
#define ADD {INS_ADD, 0}
#define SUB {INS_SUB, 0}
#define MUL {INS_MUL, 0}
#define DIV {INS_DIV, 0}
#define PRINT {INS_PRINT, 0}
#define DUP {INS_DUP, 0}
#define SWAP {INS_SWAP, 0}

Machine* NewMachine(Instruction* program, int program_length);

#endif