#include <stdio.h>
#include <stdlib.h>
#include "../libs/instruction_set.h"

int main(){
  Machine* machine = NewMachine("src/test.mica");
  machine->execute(machine);
  machine->dispose(machine);
  return 0;
}