#include "lexer.h"

struct keywords {
  char name[25];
  TokenType type;
};

struct keywords K[] = {
  {"nop", TOK_NOP}, {"push", TOK_PUSH}, {"pop", TOK_POP}, {"add", TOK_ADD},
  {"sub", TOK_SUB}, {"mul", TOK_MUL}, {"div", TOK_DIV}, {"mod", TOK_MOD},
  {"dup", TOK_DUP}, {"swap", TOK_SWAP},{"dup2", TOK_DUP2}, {"swap2", TOK_SWAP2}, {"cmpe", TOK_CMPE}, {"cmpne", TOK_CMPNE},
  {"cmpg", TOK_CMPG}, {"cmpl", TOK_CMPL}, {"cmple", TOK_CMPLE}, {"cmpge", TOK_CMPGE},
  {"jmp", TOK_JMP}, {"jz", TOK_JZ}, {"jnz", TOK_JNZ}, {"print", TOK_PRINT},
  {"halt", TOK_HALT},
};

char displayNames[][20] = {
  "TOK_NOP", "TOK_PUSH", "TOK_POP", "TOK_ADD", "TOK_SUB", "TOK_MUL", "TOK_DIV",
  "TOK_MOD", "TOK_DUP", "TOK_SWAP", "TOK_DUP2", "TOK_SWAP2", "TOK_CMPE", "TOK_CMPNE", "TOK_CMPG", "TOK_CMPL",
  "TOK_CMPLE", "TOK_CMPGE", "TOK_JMP", "TOK_JZ", "TOK_JNZ", "TOK_PRINT",
  "TOK_INT", "TOK_FLOAT", "TOK_CHAR", "TOK_HALT", "TOK_LABEL", "TOK_LABEL_REF",
};

char* ReadSource(const char* filePath) {
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

void TokenPrint(Token self) {
  printf("Token {type: %s, lexeme: %s, line: %d}\n", displayNames[self.type], self.lexeme, self.line);
}

Token TokenNew(TokenType type, char* lexeme, int line) {
  Token token;
  token.type = type;
  token.lexeme = lexeme;
  token.line = line;
  token.print = TokenPrint;
  return token;
}

void TokenListPrint(TokenList self){
  for(int i = 0; i < self.length; i++){
    self.list->print(self.list[i]);
  }
}

void TokenListDispose(TokenList self){
  free(self.list);
}

TokenList TokenListNew(){
  TokenList list;
  list.list = (Token*) calloc(8, sizeof(Token));
  list.capacity = 8;
  list.length = 0;
  list.print = TokenListPrint;
  list.dispose = TokenListDispose;
  return list;
}

void TokenListAdd(TokenList* self, Token token) {
  if(self->length == self->capacity){
    self->capacity *= 2;
    self->list = realloc(self->list, self->capacity * sizeof(Token));
  }
  self->list[self->length++] = token;
}

void LexerTokenize(Lexer* self) {
  int pos = 0, line = 1;
  int size = strlen(self->source);
  char c;

  while(pos < size){
    c = self->source[pos];

    if(c == '\n'){

      line++;
      pos++;

    }else if(isdigit(c)){

      char* lexeme = malloc(sizeof(char) * 25);
      int k = 0, isFloat = 0;
      while(isdigit(c)){
        lexeme[k++] = c;
        c = self->source[++pos];
      }
      if(c == '.'){
        isFloat = 1;
        lexeme[k++] = c;
        c = self->source[++pos];
        while(isdigit(c)){
          lexeme[k++] = c;
          c = self->source[++pos];
        }
      }
      lexeme[k] = '\0';
      if(isFloat){
        TokenListAdd(&self->tokens, TokenNew(TOK_FLOAT, lexeme, line));
      }else{
        TokenListAdd(&self->tokens, TokenNew(TOK_INT, lexeme, line));
      }

    }else if(isalpha(c)){

      char* lexeme = malloc(sizeof(char) * 25);
      int k = 0, found = 0;
      while(isalnum(c)){
        lexeme[k++] = c;
        c = self->source[++pos];
      }
      lexeme[k] = '\0';

      for(int j = 0; j < (int)(sizeof(K) / sizeof(K[0])); j++){
        if(strcmp(lexeme, K[j].name) == 0){
          TokenListAdd(&self->tokens, TokenNew(K[j].type, lexeme, line));
          found = 1;
          break;
        }
      }
      if(!found){
        if(c == ':'){
          TokenListAdd(&self->tokens, TokenNew(TOK_LABEL, lexeme, line));
          pos++;
        }else{
          TokenListAdd(&self->tokens, TokenNew(TOK_LABEL_REF, lexeme, line));
        }
      }
    }else if(c == '\''){
      c = self->source[++pos];
      char* lexeme = malloc(1);
      *lexeme = c;
      TokenListAdd(&self->tokens, TokenNew(TOK_CHAR, lexeme, line));
      c = self->source[++pos];
      if(c == '\''){
        pos++;
      }else{
        fprintf(stderr, "Error: Non-terminated string missing ending quote");
        exit(1);
      }
    }else if(c == '#'){
      while(c != '\n'){
        c = self->source[++pos];
      }
    }else{
      pos++;
    }
  }
}

void DisposeLexer(Lexer* self) {
  self->tokens.dispose(self->tokens);
  free(self->source);
  free(self);
}

Lexer* LexerNew(const char* filePath) {
  Lexer* lex = malloc(sizeof(Lexer));
  lex->source = ReadSource(filePath);
  lex->tokens = TokenListNew();
  lex->tokenize = LexerTokenize;
  lex->dispose = DisposeLexer;
  return lex;
}
