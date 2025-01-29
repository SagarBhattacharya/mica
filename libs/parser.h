#ifndef PARSER_H
#define PARSER_H

#include "micalexer.h"

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
  INS_NUMERIC,
  INS_HALT,
  INS_LABEL,
  INS_LABEL_REF,
} InstructionType;

typedef struct instruction{
  InstructionType type;
  int value;
  void (*print) (struct instruction);
} Instruction;

typedef struct insArr {
  Instruction* instructions;
  int length;
  int capacity;
  void (*dispose)(struct insArr* self);
} Program;

typedef struct label { 
  char* name;
  int index;
} Label;

typedef struct labels {
  Label* labels;
  int length;
  int capacity;
} Labels;

typedef struct parser {
  Lexer* lexer;
  Program program;
  Labels labels;
  void (*dispose)(struct parser* self);
  void (*parse)(struct parser* self);
} Parser;

Parser* NewParser(const char* source);

#endif