#include <stdio.h>
#include <stdlib.h>
#include "../includes/instruction_set.h"

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
