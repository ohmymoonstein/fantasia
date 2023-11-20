
## Memory mapping

Address space of 18-bits.

Start | End   | Description  | Size
------|-------|--------------|-----
00000 | 03FFF | Graphic VRAM | 16 KB
04000 | 043FF | Text VRAM    | 1 KB
04400 | 04FFF | Unused       | 3 KB
05000 | 0CFFF | ROM          | 32 KB
0D000 | 2CFFF | RAM          | 128 KB
2D000 | 2D3FF | Input IO     | 1 KB
2D400 | 3FFFF | Unused       | 75 KB

### Text VRAM

Text mode VRAM information is accessible at 0x04000 as an array of 32640 entries of 16-bit integers (1020 bytes). Each entry in the array represents a character in the 30x17 text mode screen. The last 32-bits in this region is unused. The following table shows the layout for elements of this array.

Description       | Size
------------------|------
ASCII code        | 8bit
Background color  | 4bit
Text color        | 4bit

TODO: keep interlaced or change to planar?

240x136 4bpp

### Mouse IO

Mouse input information is accessible at 0x2D000 as a 32-bit value.

The X and Y position values represent where the mouse cursor is pointing in screen. Valid values for X range from 0 to 239 and for Y range from 0 to 135. The X and Y values will be 255 if the cursor is outside the screen.

The H and V scroll values are used for the horizontal and vertical mouse wheel deltas. These represent the amount of steps they had moved in the last frame. These are two's complement numbers, with their sign indicating which direction they're scrolling.

Description  | Size
-------------|-------|
X position   | 8bit
Y position   | 8bit
Left         | 1bit
Middle       | 1bit
Right        | 1bit
H scroll     | 6bit
V scroll     | 6bit
(unused)     | 1bit

### Keyboard IO

Keyboard input information is accessible at 0x2D004 as a 128-bit bitmap.

Keys are mapped to the bitmap following the table below. When a bit position is 1 it means the respective key is pressed.

Bit | Key
----|-----
0   | Any key
1 to 26 | A to Z
27 to 36  | 0 to 1
37 | MINUS
38 | EQUALS
39 | LEFTBRACKET
40 | RIGHTBRACKET
41 | BACKSLASH
42 | SEMICOLON
43 | APOSTROPHE
44 | GRAVE
45 | COMMA
46 | PERIOD
47 | SLASH
48 | SPACE
49 | TAB
50 | RETURN
51 | BACKSPACE
52 | DELETE
53 | INSERT
83 | NUMPAD4
54 | PAGEUP
55 | PAGEDOWN
56 | HOME
57 | END
58 | UP
59 | DOWN
60 | LEFT
61 | RIGHT
62 | CAPSLOCK
63 | CTRL
64 | SHIFT
65 | ALT
66 | ESC
67 to 78 | F1 to F12
79 to 88 | NUMPAD0 to NUMPAD9
89 | NUMPADPLUS
90 | NUMPADMINUS
91 | NUMPADMULTIPLY
92 | NUMPADDIVIDE
93 | NUMPADENTER
94 | NUMPADPERIOD

## Intructions set

Scheme  | Size    | Opcode | Immediate
--------|---------|--------|-----------
small   | 8-bits  | 6-bits | 2-bits unsigned int
medium  | 16-bits | 6-bits | 10-bits unsigned int

Instruction | Scheme | Description | Immediate | Stack bahaviour
------------|--------|-------------|-----------|-----------------
add         | small  | Sign-agnostic addition (`arg1` + `arg2`). | N/A | `[int int] -> [int]`
sub         | small  | Sign-agnostic subtraction (`arg1` - `arg2`). | N/A | `[int int] -> [int]`
mul         | small  | Sign-agnostic multiplication (`arg1` * `arg2`), modulo 2^32. | N/A | `[int int] -> [int]`
sdiv        | small  | Signed division (`arg1` / `arg2`), result is truncated towards zero. | N/A | `[int int] -> [int]`
udiv        | small  | Unsigned division (`arg1` / `arg2`), result is floored. | N/A | `[int int] -> [int]`
srem        | small  | Signed remainder (`arg1` mod `arg2`), result has the sign of the dividend. | N/A | `[int int] -> [int]`
urem        | small  | Unsigned remainder (`arg1` mod `arg2`). | N/A | `[int int] -> [int]`
and         | small  | Sign-agnostic bitwise "and" (`arg1` and `arg2`) | N/A | `[int int] -> [int]`
or          | small  | Sign-agnostic bitwise "or" (`arg1` or `arg2`) | N/A | `[int int] -> [int]`
xor         | small  | Sign-agnostic bitwise "xor" (`arg1` xor `arg2`) | N/A | `[int int] -> [int]`
shl         | small  | Sign-agnostic shift left `arg1` by `arg2` bits, modulo 2^32 | N/A | `[int int] -> [int]`
shr         | small  | Sign-agnostic shift right `arg1` by `arg2` bits, modulo 2^32 | N/A | `[int int] -> [int]`
rotl        | small  | Sign-agnostic rotate `arg1` by `arg2` bits. | N/A | `[int int] -> [int]`
rotr        | small  | Sign-agnostic rotate `arg1` by `arg2` bits. | N/A | `[int int] -> [int]`
eq          | small  | Sign-agnostic comparison which returns non-zero if `arg1` is equal to `arg2`. | N/A | `[int int] -> [int]`
ne          | small  | Sign-agnostic comparison which returns non-zero if `arg1` is not equal to `arg2`. | N/A |`[int int] -> [int]`
slt         | small  | Signed comparison which returns non-zero if `arg1` is less than `arg2`. | N/A | `[int int] -> [int]`
sgt         | small  | Signed comparison which returns non-zero if `arg1` is grater than `arg2`. | N/A |    `[int int] -> [int]`
sle         | small  | Signed comparison which returns non-zero if `arg1` is less than or equal to `arg2`. | N/A |    `[int int] -> [int]`
sge         | small  | Signed comparison which returns non-zero if `arg1` is grater than or equal to `arg2`. | N/A |    `[int int] -> [int]`
ult         | small  | Unsigned comparison which returns non-zero if `arg1` is less than `arg2`. | N/A |`[int int] -> [int]`
ugt         | small  | Unsigned comparison which returns non-zero if `arg1` is grater than `arg2`. | N/A | `[int int] -> [int]`
ule         | small  | Unsigned comparison which returns non-zero if `arg1` is less than or equal to `arg2`. | N/A |    `[int int] -> [int]`
uge         | small  | Unsigned comparison which returns non-zero if `arg1` is grater than or equal to `arg2`. | N/A |    `[int int] -> [int]`
clz         | small  | Count of leading zero bits in `arg1`. All zero bits are considered leading if the value is zero. | N/A | `[int] -> [int]`
ctz         | small  | Count of trailing zero bits in `arg1`. All zero bits are considered trailing if the value is zero. | N/A | `[int] -> [int]`
cbit        | small  | Count number of one bits in `arg1`. | N/A | `[int] -> [int]`
load        | small  | Load a 32-bits value from memory at address `arg1`. | N/A | `[int] -> [int]`
store       | small  | Store a 32-bits value `arg2` at memory at address `arg1`. | N/A | `[int int] -> []`
ldc         | medium | Load a 32-bits value from the constant pool. | index | `[] -> [int]`
ldv         | medium | Load a 10-bits value from immediate. | value | `[] -> [int]`
ldo         | small  | Load a one value. | N/A | `[] -> [int]`
ldz         | small  | Load a zero value. | N/A | `[] -> [int]`
lget        | medium | Get the value of a local variable or parameter. | index | `[] -> [int]`
lset        | medium | Set the value of a local variable or parameter. | index | `[int] -> []`
gget        | medium | Get the value of a global variable. | index | `[] -> [int]`
gset        | medium | Set the value of a global variable. | index | `[int] -> []`
call        | medium | Call a function. | index | `[*] -> [*]`
return      | medium  | Clean up the stack and returns `count` values (zero or more) to the caller. The values returned are those that are at the top of the stack when the instruction is executed. | count | `[*] -> [*]`
jmp         | medium  | Jump to label unconditionally. | label_id | `[] -> []`
jt          | medium  | Jump to label if `arg1` is non-zero (i.e. true) | label_id | `[int] -> []`
jf          | medium  | Jump to label if `arg1` is zero (i.e. false) | label_id | `[int] -> []`
drop        | small  | Discard the first `arg1` values in the stack. | N/A | `[int *] -> []`
dup         | small  | Push `arg1` copies of `arg2` value in the stack. | N/A | `[int int] -> [*]`
nop         | small   | No operation | N/A | `[] -> []`
trap        | small   | Interrupt the program | N/A | `[] -> []`
alloc       | small   | Allocate `arg1` bytes of memory from RAM and returns the address of the allocated region. | N/A | `[int] -> [int]`
free        | small   | Deallocate the memory region pointed by `arg1`. | N/A | `[int] -> []`
cat         | small   | Concatenate two or more strings in the stack. | N/A | `[int *str] -> [str]`
fmt         | small   | Loads the data from the stack and converts them to character string. | N/A | `[str *] -> [str]`
len         | small   | Returns the length of `arg1`. | N/A | `[] -> [int]`
copy        | small   | Copy a memory region to another. | N/A | `[] -> []`
repeat      | small   | Repeat the given byte in a memory region | N/A | `[] -> []`
print       | small   | Print the string `arg1` in the text mode video memory at position (x:`arg2`,y:`arg3`)  | N/A | `[] -> []`

### Calling convention

Function arguments are pushed on the stack in the right-to-left order (i.e. top of the task contains the first argument). The callee is responsible for cleaning the stack before pushing the return values, behaviour automatically done by `return`.