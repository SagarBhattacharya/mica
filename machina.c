#include <stdio.h>
#include <stdlib.h>

typedef enum {
  INS_PUSH,
  INS_POP,
  INS_ADD,
  INS_SUB,
  INS_MUL,
  INS_DIV,
  INS_PRINT
} InstructionType;

typedef struct {
  InstructionType type;
  int value;
} Instruction;

#define PUSH(v) {INS_PUSH, v}
#define POP {INS_POP, 0}
#define ADD {INS_ADD, 0}
#define SUB {INS_SUB, 0}
#define MUL {INS_MUL, 0}
#define DIV {INS_DIV, 0}
#define PRINT {INS_PRINT, 0}

int top = -1;
int stack[1024] = {0};

void push(int value) {
  if (top < 1023) {
    stack[++top] = value;
  } else {
    fprintf(stderr, "Stack overflow\n");
    exit(EXIT_FAILURE);
  }
}

int pop() {
  if (top < 0) {
    fprintf(stderr, "Stack underflow error\n");
    exit(EXIT_FAILURE);
  }
  return stack[top--];
}

void execute(Instruction* program, int program_length) {
  int a, b;
  for (int pc = 0; pc < program_length; pc++) {
    Instruction ins = program[pc];
    switch (ins.type) {
      case INS_PUSH:
        push(ins.value);
        break;
      case INS_POP:
        pop();
        break;
      case INS_ADD:
        a = pop();
        b = pop();
        push(b + a);
        break;
      case INS_SUB:
        a = pop();
        b = pop();
        push(b - a);
        break;
      case INS_MUL:
        a = pop();
        b = pop();
        push(b * a);
        break;
      case INS_DIV:
        a = pop();
        b = pop();
        push(b / a);
        break;
      case INS_PRINT:
        printf("%d\n", pop());
        break;
      default:
        fprintf(stderr, "Unknown instruction\n");
        exit(EXIT_FAILURE);
    }
  }
}

int main(){
  Instruction program[] = {
    PUSH(10),
    PUSH(5),
    MUL,
    PRINT,
  };

  int program_length = sizeof(program) / sizeof(program[0]);
  execute(program, program_length);

  return 0;
}