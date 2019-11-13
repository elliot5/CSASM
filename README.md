
# CSASM

CSASM (C Simple ASM) is a made-up interpreted instruction set (similar to the Little Man Computer instruction set).

## Syntax

The following opcodes are available for use using the following format.

`[opcode] [operand]` e.g `mov 0x2` or `out`

Each newline determines a new instruction to be executed.

| opcode | name | operand  | description                        |
|--------|------|----------|------------------------------------|
| 0x0    | add  | address  | adds address to accumulator        |
| 0x1    | out  | null     | outputs accumulator                |
| 0x2    | mov  | address  | moves accumulator value to address |
| 0x3    | set  | long     | sets address to value              |
| 0x4    | ldr  | address  | moves address value to accumulator |
| 0x5    | lbl  | lbl name | defines a label with given name    |
| 0x6    | jmp  | lbl name | jumps to the given label name      |

## Installing / Running

1. Make the program from C source code provided.
2. The resulting executable can be ran with one parameter passed for the directory.
3. e.g `./csasm /example/dir/file.txt`

## License

CSASM uses the MIT License.

## Authors

(2019), Elliot
