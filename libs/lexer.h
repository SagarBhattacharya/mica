#ifndef MICA_LEXER_H
#define MICA_LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum token_type {
  TOK_NOP, TOK_PUSH, TOK_POP, TOK_ADD, TOK_SUB, TOK_MUL, TOK_DIV, TOK_MOD,
  TOK_DUP, TOK_SWAP,TOK_DUP2, TOK_SWAP2, TOK_CMPE, TOK_CMPNE, TOK_CMPG, TOK_CMPL, TOK_CMPLE, 
  TOK_CMPGE, TOK_JMP, TOK_JZ, TOK_JNZ, TOK_PRINT, TOK_INT, TOK_FLOAT,
  TOK_CHAR, TOK_HALT, TOK_LABEL, TOK_LABEL_REF,
} TokenType;

typedef struct token {
  TokenType type;
  char* lexeme;
  int line;
  void (*print)(struct token self);
} Token;

typedef struct token_list {
  Token* list;
  int length;
  int capacity;
  void (*print) (struct token_list self);
  void (*dispose) (struct token_list self);
} TokenList;

typedef struct lexer {
  char* source;
  TokenList tokens;
  void (*tokenize)(struct lexer* self);
  void (*dispose)(struct lexer* self);
} Lexer;

Lexer* LexerNew(const char* filePath);

#endif