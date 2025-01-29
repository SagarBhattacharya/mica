#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "value.h"

typedef enum inst_type {
  INS_NOP, INS_PUSH, INS_POP, INS_ADD, INS_SUB, INS_MUL, INS_DIV,
  INS_MOD, INS_DUP, INS_SWAP, INS_DUP2, INS_SWAP2, INS_CMPE, INS_CMPNE, INS_CMPG, INS_CMPL,
  INS_CMPLE, INS_CMPGE, INS_JMP, INS_JZ, INS_JNZ, INS_PRINT, INS_INT,
  INS_FLOAT, INS_CHAR, INS_HALT, INS_LABEL, INS_LABEL_REF,
} InstructionType;

typedef struct instruction {
  InstructionType type;
  Value value;
  void (*print) (struct instruction self);
} Instruction;

typedef struct instruction_list {
  Instruction* instructions;
  int length;
  int capacity;
  void (*print) (struct instruction_list self);
  void (*dispose)(struct instruction_list self);
} Program;

typedef struct label Label;

typedef struct label_list {
  Label* labels;
  int length;
  int capacity;
} Labels;

typedef struct parser {
  Lexer* lexer;
  Program program;
  Labels labels;
  void (*parse)(struct parser* self);
  void (*dispose)(struct parser* self);
} Parser;

Parser* ParserNew(const char* source);

#endif