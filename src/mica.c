#include <stdio.h>
#include <stdlib.h>
#include "../libs/vm.h"

int main(){
  Machine* machine = NewMachine("src/test.mica");
  machine->execute(machine);
  machine->dispose(machine);
  return 0;
}