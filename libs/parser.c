#include "parser.h"

static InstructionType tokToInsMap[] = {
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
};

static Instruction NewInstruction(InstructionType type, int value){
  Instruction ins;
  ins.type = type;
  ins.value = value;
  return ins;
}

void disposeProgram(Program* self){
  free(self->instructions);
}

static Program NewProgram(){
  Program program;
  program.instructions = (Instruction*) malloc(sizeof(Instruction) * 8);
  program.length = 0;
  program.capacity = 8;
  program.dispose = disposeProgram;
  return program;
}

static void addInstruction(Parser* self, Instruction ins){
  if(self->program.length == self->program.capacity){
    self->program.capacity *= 2;
    self->program.instructions = (Instruction*) realloc(self->program.instructions, sizeof(Instruction) * self->program.capacity);
  }
  self->program.instructions[self->program.length++] = ins;
}

void parserDispose(Parser* parser){
  parser->lexer->dispose(parser->lexer);
  parser->program.dispose(&parser->program);
  free(parser);
}

void parserParse(Parser* parser){
  parser->lexer->tokenize(parser->lexer);
  Token* current = parser->lexer->tokens;
  while(current != NULL){
    InstructionType type = tokToInsMap[current->type];
    if(type == INS_PUSH){
      current = current->next;
      if(current == NULL || current->type != TOK_NUMERIC){
        printf("Error: expected value after PUSH\n");
        exit(1);
      }
      addInstruction(parser, NewInstruction(type, current->value));
    } else {
      addInstruction(parser, NewInstruction(type, 0));
    }
    current = current->next;
  }
}

Parser* NewParser(const char* source){
  Parser* parser = (Parser*)malloc(sizeof(Parser));
  parser->lexer = NewLexer(source);
  parser->program = NewProgram();
  parser->dispose = parserDispose;
  parser->parse = parserParse;
  return parser;
}