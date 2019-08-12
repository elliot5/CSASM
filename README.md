
## CSASM

CSASM (C Simple ASM) is a made-up interpreted instruction set (similar to the Little Man Computer instruction set).

# Syntax

The following opcodes are available for use using the following format.

`[opcode] [operand]` e.g `mov 0x2'

| opcode | name | operand | description                        |
|--------|------|---------|------------------------------------|
| 0x0    | add  | address | adds address to accumulator        |
| 0x1    | out  | null    | outputs accumulator                |
| 0x2    | mov  | address | moves accumulator value to address |
| 0x3    | set  | long    | sets address to value              |
| 0x4    | ldr  | address | moves address value to accumulator |

# Authors

(2019), Elliot Bewey
