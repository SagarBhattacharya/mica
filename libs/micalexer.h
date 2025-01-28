#ifndef MICA_LEXER_H
#define MICA_LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
  TOK_NOP,
  TOK_PUSH,
  TOK_POP,
  TOK_ADD,
  TOK_SUB,
  TOK_MUL,
  TOK_DIV,
  TOK_MOD,
  TOK_DUP,
  TOK_SWAP,
  TOK_CMPE,
  TOK_CMPNE,
  TOK_CMPG,
  TOK_CMPL,
  TOK_CMPLE,
  TOK_CMPGE,
  TOK_JMP,
  TOK_JZ,
  TOK_JNZ,
  TOK_PRINT,
  TOK_NUMERIC,
  TOK_HALT
} TokenType;

typedef struct token{
  TokenType type;
  char* lexeme;
  int line;
  int value;
  struct token* next;
  void (*print)(struct token* self);
} Token;

typedef struct lexer{
  char* src;
  Token* tokens;
  int length;
  void (*dispose)(struct lexer* self);
  void (*tokenize)(struct lexer* self);
} Lexer;

Lexer* NewLexer(const char* filePath);

#endif