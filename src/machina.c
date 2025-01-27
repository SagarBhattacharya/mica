#include <stdio.h>
#include <stdlib.h>
#include "../includes/instruction_set.h"

int main(){
  Instruction program[] = {
    PUSH(20),
    PUSH(30),
    SWAP,
    DIV,
    PRINT,
  };

  int program_length = sizeof(program) / sizeof(program[0]);
  Machine* mac = NewMachine(program, program_length);
  mac->execute(mac);
  mac->dispose(mac);

  return 0;
}
