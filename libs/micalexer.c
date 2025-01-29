#include "micalexer.h"

struct keywords {
  char name[25];
  TokenType type;
};

struct keywords K[] = {
  {"nop", TOK_NOP}, {"push", TOK_PUSH}, {"pop", TOK_POP}, {"add", TOK_ADD},
  {"sub", TOK_SUB}, {"mul", TOK_MUL}, {"div", TOK_DIV}, {"mod", TOK_MOD},
  {"dup", TOK_DUP}, {"swap", TOK_SWAP}, {"cmpe", TOK_CMPE}, {"cmpne", TOK_CMPNE},
  {"cmpg", TOK_CMPG}, {"cmpl", TOK_CMPL}, {"cmple", TOK_CMPLE}, {"cmpge", TOK_CMPGE},
  {"jmp", TOK_JMP}, {"jz", TOK_JZ}, {"jnz", TOK_JNZ}, {"print", TOK_PRINT},
  {"halt", TOK_HALT}
};

char displayNames[][25] = {
  "TOK_NOP", "TOK_PUSH", "TOK_POP", "TOK_ADD", "TOK_SUB", "TOK_MUL", "TOK_DIV",
  "TOK_MOD", "TOK_DUP", "TOK_SWAP", "TOK_CMPE", "TOK_CMPNE", "TOK_CMPG", "TOK_CMPL",
  "TOK_CMPLE", "TOK_CMPGE", "TOK_JMP", "TOK_JZ", "TOK_JNZ", "TOK_PRINT",
  "TOK_NUMERIC", "TOK_HALT", "TOK_LABEL", "TOK_LABEL_REF"
};

static char* readSource(const char* filePath) {
  FILE* file = fopen(filePath, "r");
  if (file == NULL) {
    fprintf(stderr, "Error: Could not open file %s\n", filePath);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = malloc(sizeof(char) * (length + 1));
  fread(buffer, 1, length, file);
  buffer[length] = '\0';

  fclose(file);
  return buffer;
}

void printToken(Token* self) {
  printf("Token{type: %s, lexeme: %s, line: %d, value: %d}\n",
         displayNames[self->type], self->lexeme, self->line, self->value);
}

static Token* NewToken(TokenType type, char* lexeme, int line, int value) {
  Token* tok = malloc(sizeof(Token));
  tok->type = type;
  tok->lexeme = lexeme;
  tok->line = line;
  tok->value = value;
  tok->print = printToken;
  tok->next = NULL;
  return tok;
}

static void addToken(Lexer* self, Token* token) {
  if (self->tokens == NULL) {
    self->tokens = token;
  } else {
    Token* current = self->tokens;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = token;
  }
}

void lexerLexToken(Lexer* self) {
  int pos = 0;
  int line = 1;

  while (pos < self->length) {
    char c = self->src[pos];

    if (c == '\n') {
      line++;
      pos++;
    } else if (isdigit(c)) {
      int value = 0;
      while (isdigit(c)) {
        value = value * 10 + (c - '0');
        c = self->src[++pos];
      }
      addToken(self, NewToken(TOK_NUMERIC, NULL, line, value));
    } else if (isalpha(c)) {
      char* lexeme = malloc(sizeof(char) * 25);
      int i = 0;
      while (pos < self->length && isalpha(c)) {
        lexeme[i++] = c;
        c = self->src[++pos];
      }
      lexeme[i] = '\0';

      int found = 0;
      for (int j = 0; j < (int)(sizeof(K) / sizeof(K[0])); j++) {
        if (strcmp(lexeme, K[j].name) == 0) {
          addToken(self, NewToken(K[j].type, lexeme, line, 0));
          found = 1;
          break;
        }
      }

      if (!found) {
        if (c == ':') {
          addToken(self, NewToken(TOK_LABEL, lexeme, line, 0));
          pos++;
        } else {
          addToken(self, NewToken(TOK_LABEL_REF, lexeme, line, 0));
        }
      }
    } else {
      pos++;
    }
  }
}

void lexerDispose(Lexer* self) {
  Token* current = self->tokens;
  while (current != NULL) {
    Token* next = current->next;
    free(current->lexeme);
    free(current);
    current = next;
  }
  free(self->src);
  free(self);
}

Lexer* NewLexer(const char* filePath) {
  Lexer* lex = malloc(sizeof(Lexer));
  lex->src = readSource(filePath);
  lex->length = strlen(lex->src);
  lex->tokens = NULL;
  lex->dispose = lexerDispose;
  lex->tokenize = lexerLexToken;
  return lex;
}
