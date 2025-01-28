# MICA

This is a simple virtual machine project written in C. This is strictly for educational purposes and might resemble to the sources i am learning from and is not meant for any commercial use. This project does not guarantee
a completion. Although I do wanna complete it.

## Features

- Simple Instruction Set
- Custom assembly

## Opcodes
| Opcode   | Description                       |
|----------|-----------------------------------|
| nop      | No operation                      |
| push val | Push a value onto the stack       |
| pop      | Pop the top value from the stack  |
| dup      | Duplicate the top value           |
| swap     | Swap the top two values           |
| add      | Pop top two, push sum             |
| sub      | Pop top two, push difference      |
| mul      | Pop top two, push product         |
| div      | Pop top two, push quotient        |
| mod      | Pop top two, push remainder       |
| cmpe     | Compare equal, push 1 if true     |
| cmpne    | Compare not equal, push 1 if true |
| cmpg     | Compare greater, push 1 if true   |
| cmpl     | Compare lesser, push 1 if true    |
| cmpge    | Compare ≥, push 1 if true         |
| cmple    | Compare ≤, push 1 if true         |
| jmp val  | Jump to instruction at index val  |
| jz val   | Jump if top is 0                  |
| jnz val  | Jump if top is nonzero            |
| halt     | Halt execution                    |

## Example program

Here’s a short "test.mica" program that prints numbers from 0 to 9:

```pl
push 0
dup
print
push 1
add
dup
push 10
cmpe
jz 1
halt
```

## Planned

- More datatypes (currently works on integers only)
- support of label in mica assmebly
- including syscalls


