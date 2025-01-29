#include "parser.h"

static InstructionType tokToInsMap[] = {
  INS_NOP, INS_PUSH, INS_POP, INS_ADD, INS_SUB, INS_MUL, INS_DIV, INS_MOD,
  INS_DUP, INS_SWAP, INS_DUP2, INS_SWAP2, INS_CMPE, INS_CMPNE, INS_CMPG, INS_CMPL, INS_CMPLE,
  INS_CMPGE, INS_JMP, INS_JZ, INS_JNZ, INS_PRINT, INS_INT, INS_FLOAT, INS_CHAR,
  INS_HALT, INS_LABEL, INS_LABEL_REF,
};

// Display names for instructions
char displayInstructions[][25] = {
  "NOP", "PUSH", "POP", "ADD", "SUB", "MUL", "DIV", "MOD", "DUP", "SWAP",
  "DUP2", "SWAP2", "CMPE", "CMPNE", "CMPG", "CMPL", "CMPLE", "CMPGE", "JMP",
  "JZ", "JNZ", "PRINT", "INT", "FLOAT", "CHAR", "HALT", "LABEL", "LABEL_REF",
};

struct label { 
  char* name;
  int index;
};

Label LabelNew(char* name, int index){
  Label label;
  label.name = name;
  label.index = index;
  return label;
}

Labels LabelsNew(){
  Labels arr;
  arr.labels = (Label*) malloc(sizeof(Label) * 8);
  arr.length = 0;
  arr.capacity = 8;
  return arr;
}

void LabelsAdd(Labels* arr, Label label) {
  if (arr->length == arr->capacity) {
    arr->capacity *= 2;
    arr->labels = (Label*) realloc(arr->labels, sizeof(Label) * arr->capacity);
  }
  arr->labels[arr->length++] = label;
}

void InstructionPrint(Instruction self) {
  if (self.type == INS_PUSH || self.type == INS_JMP || self.type == INS_JZ || self.type == INS_JNZ){
    if(self.value.type == VALUE_INT){
      printf("%s %lld\n", displayInstructions[self.type], self.value.asInt);
    }else if(self.value.type == VALUE_FLOAT){
      printf("%s %.2f\n", displayInstructions[self.type], self.value.asFloat);
    }else if(self.value.type == VALUE_CHAR){
      printf("%s %c\n", displayInstructions[self.type], self.value.asChar);
    }
  }else{
    printf("%s\n", displayInstructions[self.type]);
  }
}

Instruction InstructionNew(InstructionType type, Value value) {
  Instruction ins;
  ins.type = type;
  ins.value = value;
  ins.print = InstructionPrint;
  return ins;
}

void ProgramDispose(Program self){
  free(self.instructions);
}

void ProgramPrint(Program self){
  for(int i = 0; i < self.length; i++){
    self.instructions->print(self.instructions[i]);
  };
}

Program ProgramNew(){
  Program program;
  program.instructions = (Instruction*) malloc(sizeof(Instruction) * 8);
  program.length = 0;
  program.capacity = 8;
  program.print = ProgramPrint;
  program.dispose = ProgramDispose;
  return program;
}

void ProgramAddInstruction(Program* self, Instruction ins){
  if (self->length == self->capacity) {
    self->capacity *= 2;
    self->instructions = (Instruction*) realloc(self->instructions, sizeof(Instruction) * self->capacity);
  }
  self->instructions[self->length++] = ins;
}

void DisposeParser(Parser* parser) {
  parser->lexer->dispose(parser->lexer);
  parser->program.dispose(parser->program);
  free(parser->labels.labels);
  free(parser);
}

void ParserResolveLabels(Parser* self){
  TokenList list = self->lexer->tokens;
  int k = 0;
  for(int i = 0; i < list.length; i++){
    TokenType type = list.list[i].type;
    if(type == TOK_LABEL){
      LabelsAdd(&self->labels, LabelNew(list.list[i].lexeme, k));
    }else if(type == TOK_PUSH || type == TOK_JMP || type == TOK_JZ || type == TOK_JNZ){
      i++;
    }
    k++;
  }
}

void ParserParse(Parser* self) {
  self->lexer->tokenize(self->lexer);
  ParserResolveLabels(self);

  TokenList list = self->lexer->tokens;
  for(int i = 0; i < list.length; i++){
    TokenType type = list.list[i].type;
    if (type == TOK_PUSH || type == TOK_JMP || type == TOK_JZ || type == TOK_JNZ) {
      Token arg = list.list[++i];
      if (arg.type != TOK_INT && arg.type != TOK_FLOAT && arg.type != TOK_CHAR && arg.type != TOK_LABEL_REF) {
        printf("Error: expected value after PUSH\n");
        exit(1);
      }
      if (arg.type == TOK_INT) {
        ProgramAddInstruction(&self->program, InstructionNew(tokToInsMap[type], IntValue(atoi(arg.lexeme))));
      }else if (arg.type == TOK_FLOAT) {
        ProgramAddInstruction(&self->program, InstructionNew(tokToInsMap[type], FloatValue(atof(arg.lexeme))));
      }else if (arg.type == TOK_CHAR) {
        ProgramAddInstruction(&self->program, InstructionNew(tokToInsMap[type], CharValue(arg.lexeme[0])));
      }else if (arg.type == TOK_LABEL_REF) {
        int index = -1;
        for (int j = 0; j < self->labels.length; j++) {
          if(strcmp(self->labels.labels[j].name, arg.lexeme) == 0) {
            index = self->labels.labels[j].index;
            break;
          }
        }
        if (index == -1) {
          printf("Error: label not found\n");
          exit(1);
        }
        ProgramAddInstruction(&self->program, InstructionNew(tokToInsMap[type], IntValue(index)));
      }
    } else {
      ProgramAddInstruction(&self->program, InstructionNew(tokToInsMap[list.list[i].type], IntValue(0)));
    }
  }
}

// Create a new parser
Parser* ParserNew(const char* source) {
  Parser* parser = (Parser*) malloc(sizeof(Parser));
  parser->lexer = LexerNew(source);
  parser->program = ProgramNew();
  parser->labels = LabelsNew();
  parser->dispose = DisposeParser;
  parser->parse = ParserParse;
  return parser;
}
