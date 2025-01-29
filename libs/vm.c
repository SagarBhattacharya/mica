#include "vm.h"
#define STACK_MAX 1024

static void push(Machine* self, Value value) {
  if (self->top < STACK_MAX - 1) {
    self->stack[++self->top] = value;
  } else {
    fprintf(stderr, "Stack overflow\n");
    exit(EXIT_FAILURE);
  }
}

static Value pop(Machine* self) {
  if (self->top < 0) {
    fprintf(stderr, "Stack underflow\n");
    exit(EXIT_FAILURE);
  }
  return self->stack[self->top--];
}

void DisposeMachine(Machine* self) {
  free(self->stack);
  self->parser->dispose(self->parser);
  free(self);
}

#define BinaryOperation(op, a, b)                                 \
  do{                                                             \
    if(b.type == VALUE_INT && a.type == VALUE_INT){               \
      push(self, IntValue(b.asInt op a.asInt));                   \
    }else if(b.type == VALUE_INT && a.type == VALUE_FLOAT){       \
      push(self, FloatValue((float) b.asInt op a.asFloat));       \
    }else if(b.type == VALUE_FLOAT && a.type == VALUE_INT){       \
      push(self, FloatValue(b.asFloat op (float) a.asInt));       \
    }else if(b.type == VALUE_FLOAT && a.type == VALUE_FLOAT){     \
      push(self, FloatValue(b.asFloat op a.asFloat));             \
    }                                                             \
  } while(0)                                                      \

void ExecuteCode(Machine* self) {
  self->parser->parse(self->parser);
  Value a, b, c, d;
  for (int pc = 0; pc < self->parser->program.length; pc++) {
    Instruction ins = self->parser->program.instructions[pc];
    switch (ins.type) {
      case INS_INT:
      case INS_FLOAT:
      case INS_LABEL:
      case INS_LABEL_REF:
      case INS_NOP:
        continue;
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
      case INS_DUP2:
        a = pop(self);
        b = pop(self);
        push(self, b);
        push(self, a);
        push(self, b);
        push(self, a);
        break;
      case INS_SWAP2:
        a = pop(self);
        b = pop(self);
        c = pop(self);
        d = pop(self);
        push(self, b);
        push(self, a);
        push(self, d);
        push(self, c);
        break;
      case INS_ADD:
        a = pop(self);
        b = pop(self);
        BinaryOperation(+, a, b);
        break;
      case INS_SUB:
        a = pop(self);
        b = pop(self);
        BinaryOperation(-, a, b);
        break;
      case INS_MUL:
        a = pop(self);
        b = pop(self);
        BinaryOperation(*, a, b);
        break;
      case INS_DIV:
        a = pop(self);
        b = pop(self);
        if(a.asInt == 0){
          fprintf(stderr, "Division by zero error\n");
          exit(EXIT_FAILURE);
        }
        BinaryOperation(/, a, b);
        break;
      case INS_MOD:
        a = pop(self);
        b = pop(self);
        if(a.asInt == 0){
          fprintf(stderr, "Division by zero error\n");
          exit(EXIT_FAILURE);
        }
        if(b.type == VALUE_INT && a.type == VALUE_INT){
          push(self, IntValue(b.asInt % a.asInt));
        }else{
          fprintf(stderr, "Cannot perform mod on floating point values\n");
          exit(EXIT_FAILURE);
        }
        break;
      case INS_CMPE:
        a = pop(self);
        b = pop(self);
        BinaryOperation(==, a, b);
        break;
      case INS_CMPNE:
        a = pop(self);
        b = pop(self);
        BinaryOperation(!=, a, b);
        break;
      case INS_CMPG:
        a = pop(self);
        b = pop(self);
        BinaryOperation(>, a, b);
        break;
      case INS_CMPL:
        a = pop(self);
        b = pop(self);
        BinaryOperation(<, a, b);
        break;
      case INS_CMPLE:
        a = pop(self);
        b = pop(self);
        BinaryOperation(<=, a, b);
        break;
      case INS_CMPGE:
        a = pop(self);
        b = pop(self);
        BinaryOperation(>=, a, b);
        break;
      case INS_PRINT:
        a = pop(self);
        if(a.type == VALUE_FLOAT){
          printf("%.2f\n", a.asFloat);
        }else if(a.type == VALUE_INT){
          printf("%lld\n", a.asInt);
        }else if(a.type == VALUE_CHAR){
          printf("%c\n", a.asChar);
        }
        break;
      case INS_JZ:
        a = pop(self);
        if(a.asInt == 0){
          pc = ins.value.asInt - 1;
        }
        break;
      case INS_JNZ:
        a = pop(self);
        if(a.asInt == 1){
          pc = ins.value.asInt - 1;
        }
        break;
      case INS_JMP:
        pc = ins.value.asInt - 1;
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

#undef BinaryOperation

Machine* NewMachine(const char* source) {
  Machine* machine = (Machine*) malloc(sizeof(Machine));
  machine->top = -1;
  machine->stack = (Value*) calloc(STACK_MAX, sizeof(Value));
  machine->parser = ParserNew(source);
  machine->execute = ExecuteCode;
  machine->dispose = DisposeMachine;
  return machine;
}