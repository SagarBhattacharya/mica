#include "instruction_set.h"
#define STACK_MAX 1024

static void push(Machine* mac, int value) {
  if (mac->top < STACK_MAX - 1) {
    mac->stack[++mac->top] = value;
  } else {
    fprintf(stderr, "Stack overflow\n");
    exit(EXIT_FAILURE);
  }
}

static int pop(Machine* mac) {
  if (mac->top < 0) {
    fprintf(stderr, "Stack underflow\n");
    exit(EXIT_FAILURE);
  }
  return mac->stack[mac->top--];
}

void machineDispose(Machine* self) {
  free(self->stack);
  self->parser->dispose(self->parser);
  free(self);
}

void machineExecute(Machine* self) {
  self->parser->parse(self->parser);
  int a, b;
  for (int pc = 0; pc < self->parser->program.length; pc++) {
    Instruction ins = self->parser->program.instructions[pc];
    switch (ins.type) {
      case INS_NUMERIC:
      case INS_NOP:
        break;
      case INS_PUSH:
        push(self, ins.value);
        break;
      case INS_POP:
        pop(self);
        break;
      case INS_DUP:
        a = pop(self);
        push(self, a);
        push(self, a);
        break;
      case INS_SWAP:
        a = pop(self);
        b = pop(self);
        push(self, a);
        push(self, b);
        break;
      case INS_ADD:
        a = pop(self);
        b = pop(self);
        push(self, b + a);
        break;
      case INS_SUB:
        a = pop(self);
        b = pop(self);
        push(self, b - a);
        break;
      case INS_MUL:
        a = pop(self);
        b = pop(self);
        push(self, b * a);
        break;
      case INS_DIV:
        a = pop(self);
        b = pop(self);
        if(a == 0){
          fprintf(stderr, "Division by zero error\n");
          exit(EXIT_FAILURE);
        }
        push(self, b / a);
        break;
      case INS_MOD:
        a = pop(self);
        b = pop(self);
        if(a == 0){
          fprintf(stderr, "Division by zero error\n");
          exit(EXIT_FAILURE);
        }
        push(self, b % a);
        break;
      case INS_CMPE:
        a = pop(self);
        b = pop(self);
        push(self, b == a);
        break;
      case INS_CMPNE:
        a = pop(self);
        b = pop(self);
        push(self, b != a);
        break;
      case INS_CMPG:
        a = pop(self);
        b = pop(self);
        push(self, b > a);
        break;
      case INS_CMPL:
        a = pop(self);
        b = pop(self);
        push(self, b < a);
        break;
      case INS_CMPLE:
        a = pop(self);
        b = pop(self);
        push(self, b <= a);
        break;
      case INS_CMPGE:
        a = pop(self);
        b = pop(self);
        push(self, b >= a);
        break;
      case INS_PRINT:
        printf("%d\n", pop(self));
        break;
      case INS_JZ:
        a = pop(self);
        if(a == 0){
          pc = ins.value - 1;
        }
        break;
      case INS_JNZ:
        a = pop(self);
        if(a == 1){
          pc = ins.value - 1;
        }
        break;
      case INS_JMP:
        pc = ins.value - 1;
        break;
      case INS_HALT:
        pc = self->parser->program.length;
        break;
      default:
        fprintf(stderr, "Unknown instruction\n");
        exit(EXIT_FAILURE);
    }
  }
}

Machine* NewMachine(const char* source) {
  Machine* machine = (Machine*) malloc(sizeof(Machine));
  machine->top = -1;
  machine->stack = (int*)calloc(STACK_MAX, sizeof(int));
  machine->parser = NewParser(source);
  machine->execute = machineExecute;
  machine->dispose = machineDispose;
  return machine;
}