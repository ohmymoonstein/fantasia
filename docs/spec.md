
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

### Text VRAM IO

Text mode VRAM information is accessible at 0x04000 as an array of 32640 entries of 16-bit integers (1020 bytes). Each entry in the array represents a character in the 30x17 text mode screen. The last 32-bits in this region is unused. The following table shows the layout for elements of this array.

Description       | Size
------------------|------
ASCII code        | 8bit
Background color  | 4bit
Text color        | 4bit

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
add         | small  | Sign-agnostic addition (`arg1` + `arg2`). | N/A | `[i32 i32] -> [i32]`
sub         | small  | Sign-agnostic subtraction (`arg1` - `arg2`). | N/A | `[i32 i32] -> [i32]`
mul         | small  | Sign-agnostic multiplication (`arg1` * `arg2`), modulo 2^32. | N/A | `[i32 i32] -> [i32]`
sdiv        | small  | Signed division (`arg1` / `arg2`), result is truncated towards zero. | N/A | `[i32 i32] -> [i32]`
udiv        | small  | Unsigned division (`arg1` / `arg2`), result is floored. | N/A | `[i32 i32] -> [i32]`
srem        | small  | Signed remainder (`arg1` mod `arg2`), result has the sign of the dividend. | N/A | `[i32 i32] -> [i32]`
urem        | small  | Unsigned remainder (`arg1` mod `arg2`). | N/A | `[i32 i32] -> [i32]`
and         | small  | Sign-agnostic bitwise "and" (`arg1` and `arg2`) | N/A | `[i32 i32] -> [i32]`
or          | small  | Sign-agnostic bitwise "or" (`arg1` or `arg2`) | N/A | `[i32 i32] -> [i32]`
xor         | small  | Sign-agnostic bitwise "xor" (`arg1` xor `arg2`) | N/A | `[i32 i32] -> [i32]`
shl         | small  | Sign-agnostic shift left `arg1` by `arg2` bits, modulo 2^32 | N/A | `[i32 i32] -> [i32]`
shr         | small  | Sign-agnostic shift right `arg1` by `arg2` bits, modulo 2^32 | N/A | `[i32 i32] -> [i32]`
rotl        | small  | Sign-agnostic rotate `arg1` by `arg2` bits. | N/A | `[i32 i32] -> [i32]`
rotr        | small  | Sign-agnostic rotate `arg1` by `arg2` bits. | N/A | `[i32 i32] -> [i32]`
eq          | small  | Sign-agnostic comparison which returns non-zero if `arg1` is equal to `arg2`. | N/A | `[i32 i32] -> [i32]`
ne          | small  | Sign-agnostic comparison which returns non-zero if `arg1` is not equal to `arg2`. | N/A |`[i32 i32] -> [i32]`
slt         | small  | Signed comparison which returns non-zero if `arg1` is less than `arg2`. | N/A | `[i32 i32] -> [i32]`
sgt         | small  | Signed comparison which returns non-zero if `arg1` is grater than `arg2`. | N/A |    `[i32 i32] -> [i32]`
sle         | small  | Signed comparison which returns non-zero if `arg1` is less than or equal to `arg2`. | N/A |    `[i32 i32] -> [i32]`
sge         | small  | Signed comparison which returns non-zero if `arg1` is grater than or equal to `arg2`. | N/A |    `[i32 i32] -> [i32]`
ult         | small  | Unsigned comparison which returns non-zero if `arg1` is less than `arg2`. | N/A |`[i32 i32] -> [i32]`
ugt         | small  | Unsigned comparison which returns non-zero if `arg1` is grater than `arg2`. | N/A | `[i32 i32] -> [i32]`
ule         | small  | Unsigned comparison which returns non-zero if `arg1` is less than or equal to `arg2`. | N/A |    `[i32 i32] -> [i32]`
uge         | small  | Unsigned comparison which returns non-zero if `arg1` is grater than or equal to `arg2`. | N/A |    `[i32 i32] -> [i32]`
clz         | small  | Count of leading zero bits in `arg1`. All zero bits are considered leading if the value is zero. | N/A | `[i32] -> [i32]`
ctz         | small  | Count of trailing zero bits in `arg1`. All zero bits are considered trailing if the value is zero. | N/A | `[i32] -> [i32]`
cbit        | small  | Count number of one bits in `arg1`. | N/A | `[i32] -> [i32]`
load        | small  | Load a 32-bits value from memory at address `arg1`. | N/A | `[i32] -> [i32]`
store       | small  | Store a 32-bits value `arg2` at memory at address `arg1`. | N/A | `[i32 i32] -> []`
ldc         | medium | Load a 32-bits value from the constant pool. | index | `[] -> [i32]`
ldv         | medium | Load a 10-bits value from immediate. | value | `[] -> [i32]`
ldo         | small  | Load a one value. | N/A | `[] -> [i32]`
ldz         | small  | Load a zero value. | N/A | `[] -> [i32]`
lget        | medium | Get the value of a local variable or parameter. | index | `[] -> [i32]`
lset        | medium | Set the value of a local variable or parameter. | index | `[i32] -> []`
gget        | medium | Get the value of a global variable. | index | `[] -> [i32]`
gset        | medium | Set the value of a global variable. | index | `[i32] -> []`
call        | medium | Call a function. | index | `[*] -> [*]`
return      | medium  | Clean up the stack and returns `count` values (zero or more) to the caller. The values returned are those that are at the top of the stack when the instruction is executed. | count | `[*] -> [*]`
jmp         | medium  | Jump to label unconditionally. | label_id | `[] -> []`
jt          | medium  | Jump to label if `arg1` is non-zero (i.e. true) | label_id | `[i32] -> []`
jf          | medium  | Jump to label if `arg1` is zero (i.e. false) | label_id | `[i32] -> []`
drop        | small  | Discard the first `arg1` values in the stack. | N/A | `[i32 *] -> []`
dup         | small  | Push `arg1` copies of `arg2` value in the stack. | N/A | `[i32 i32] -> [*]`
nop         | small   | No operation | N/A | `[] -> []`
trap        | small   | | N/A | `[] -> []`

### Calling convention

Function arguments are pushed on the stack in the right-to-left order (i.e. top of the task contains the first argument). The callee is responsible for cleaning the stack before pushing the return values, behaviour automatically done by `return`.