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
  INS_PRINT
} InstructionType;

typedef struct {
  InstructionType type;
  int value;
} Instruction;

#define PUSH(v) {INS_PUSH, v}
#define POP {INS_POP, 0}
#define ADD {INS_ADD, 0}
#define SUB {INS_SUB, 0}
#define MUL {INS_MUL, 0}
#define DIV {INS_DIV, 0}
#define PRINT {INS_PRINT, 0}

void execute(Instruction* program, int program_length);

#endif