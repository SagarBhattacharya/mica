#include "parser.h"

// Mapping of tokens to instruction types
static InstructionType tokToInsMap[] = {
  INS_NOP, INS_PUSH, INS_POP, INS_ADD, INS_SUB, INS_MUL, INS_DIV, INS_MOD,
  INS_DUP, INS_SWAP, INS_CMPE, INS_CMPNE, INS_CMPG, INS_CMPL, INS_CMPLE,
  INS_CMPGE, INS_JMP, INS_JZ, INS_JNZ, INS_PRINT, INS_NUMERIC, INS_HALT,
  INS_LABEL, INS_LABEL_REF,
};

// Display names for instructions
char displayInstructions[][25] = {
  "NOP", "PUSH", "POP", "ADD", "SUB", "MUL", "DIV", "MOD", "DUP", "SWAP",
  "CMPE", "CMPNE", "CMPG", "CMPL", "CMPLE", "CMPGE", "JMP", "JZ", "JNZ",
  "PRINT", "NUMERIC", "HALT", "LABEL", "LABEL_REF",
};

// Print an instruction
void printInstruction(Instruction ins) {
  if (ins.type == INS_PUSH || ins.type == INS_JMP || ins.type == INS_JZ || ins.type == INS_JNZ)
    printf("%s %d\n", displayInstructions[ins.type], ins.value);
  else
    printf("%s\n", displayInstructions[ins.type]);
}

// Create a new instruction
static Instruction newInstruction(InstructionType type, int value) {
  Instruction ins;
  ins.type = type;
  ins.value = value;
  ins.print = printInstruction;
  return ins;
}

// Dispose of a program
void disposeProgram(Program* self) {
  free(self->instructions);
}

// Create a new program
static Program NewProgram() {
  Program program;
  program.instructions = (Instruction*) malloc(sizeof(Instruction) * 8);
  program.length = 0;
  program.capacity = 8;
  program.dispose = disposeProgram;
  return program;
}

// Add an instruction to the parser
static void addInstruction(Parser* self, Instruction ins) {
  if (self->program.length == self->program.capacity) {
    self->program.capacity *= 2;
    self->program.instructions = (Instruction*) realloc(self->program.instructions, sizeof(Instruction) * self->program.capacity);
  }
  self->program.instructions[self->program.length++] = ins;
}

// Dispose of a parser
void parserDispose(Parser* parser) {
  parser->lexer->dispose(parser->lexer);
  parser->program.dispose(&parser->program);
  free(parser->labels.labels);
  free(parser);
}

// Create a new label
static Label newLabel(char* name, int index) {
  Label label;
  label.name = name;
  label.index = index;
  return label;
}

// Create a new labels array
Labels NewLables() {
  Labels arr;
  arr.labels = (Label*) malloc(sizeof(Label) * 8);
  arr.length = 0;
  arr.capacity = 8;
  return arr;
}

// Add a label to the labels array
static void addLabel(Labels* arr, Label label) {
  if (arr->length == arr->capacity) {
    arr->capacity *= 2;
    arr->labels = (Label*) realloc(arr->labels, sizeof(Label) * arr->capacity);
  }
  arr->labels[arr->length++] = label;
}

// Resolve labels in the parser
void parserResolveLabels(Parser* self) {
  Token* current = self->lexer->tokens;
  int instructionIndex = 0;
  while (current != NULL) {
    if (current->type == TOK_LABEL) {
      addLabel(&self->labels, newLabel(current->lexeme, instructionIndex));
    } else if (current->type == TOK_PUSH || current->type == TOK_JMP || current->type == TOK_JZ || current->type == TOK_JNZ) {
      current = current->next;
    }
    instructionIndex++;
    current = current->next;
  }
}

// Parse the tokens into instructions
void parserParse(Parser* parser) {
  parser->lexer->tokenize(parser->lexer);
  parserResolveLabels(parser);
  Token* current = parser->lexer->tokens;
  while (current != NULL) {
    InstructionType type = tokToInsMap[current->type];
    if (type == INS_PUSH || type == INS_JMP || type == INS_JZ || type == INS_JNZ) {
      current = current->next;
      if (current == NULL || (current->type != TOK_NUMERIC && current->type != TOK_LABEL_REF)) {
        printf("Error: expected value after PUSH\n");
        exit(1);
      }
      if (current->type == TOK_NUMERIC) {
        addInstruction(parser, newInstruction(type, current->value));
      } else if (current->type == TOK_LABEL_REF) {
        int index = -1;
        for (int i = 0; i < parser->labels.length; i++) {
          if (strcmp(parser->labels.labels[i].name, current->lexeme) == 0) {
            index = parser->labels.labels[i].index;
            break;
          }
        }
        if (index == -1) {
          printf("Error: label not found\n");
          exit(1);
        }
        addInstruction(parser, newInstruction(type, index));
      }
    } else {
      addInstruction(parser, newInstruction(type, 0));
    }
    current = current->next;
  }
}

// Create a new parser
Parser* NewParser(const char* source) {
  Parser* parser = (Parser*) malloc(sizeof(Parser));
  parser->lexer = NewLexer(source);
  parser->program = NewProgram();
  parser->labels = NewLables();
  parser->dispose = parserDispose;
  parser->parse = parserParse;
  return parser;
}
