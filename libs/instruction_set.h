#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
  INS_NOP,
  INS_PUSH,
  INS_POP,
  INS_ADD,
  INS_SUB,
  INS_MUL,
  INS_DIV,
  INS_MOD,
  INS_DUP,
  INS_SWAP,
  INS_CMPE,
  INS_CMPNE,
  INS_CMPG,
  INS_CMPL,
  INS_CMPLE,
  INS_CMPGE,
  INS_JMP,
  INS_JZ,
  INS_JNZ,
  INS_PRINT,
  INS_HALT
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

#define NOP {INS_NOP, 0}
#define PUSH(v) {INS_PUSH, v}
#define POP {INS_POP, 0}
#define ADD {INS_ADD, 0}
#define SUB {INS_SUB, 0}
#define MUL {INS_MUL, 0}
#define DIV {INS_DIV, 0}
#define MOD {INS_MOD, 0}
#define PRINT {INS_PRINT, 0}
#define DUP {INS_DUP, 0}
#define SWAP {INS_SWAP, 0}
#define CMPE {INS_CMPE, 0}
#define CMPNE {INS_CMPNE, 0}
#define CMPG {INS_CMPG, 0}
#define CMPL {INS_CMPL, 0}
#define CMPLE {INS_CMPLE, 0}
#define CMPGE {INS_CMPGE, 0}
#define JMP(x) {INS_JMP, x}
#define JZ(x) {INS_JZ, x}
#define JNZ(x) {INS_JNZ, x}
#define HALT {INS_HALT, 0}

Machine* NewMachine(Instruction* program, int program_length);

#endif