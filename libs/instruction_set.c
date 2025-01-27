#include "../includes/instruction_set.h"

int top = -1;
int stack[1024] = {0};

static void push(int value) {
  if (top < 1023) {
    stack[++top] = value;
  } else {
    fprintf(stderr, "Stack overflow\n");
    exit(EXIT_FAILURE);
  }
}

static int pop() {
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
  top = -1;
}