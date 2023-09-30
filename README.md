# Plastic Assembler

A small work-in-progress assembler/disassembler/emulator to the plastic assembly language.

## About

The Plastic Assembly Language (PLS) is a simple lightweight language that runs on the Plastic Assembler. I created the language to learn how to design a language from scratch that would assemble and run 'code' and for my own personal entertainment. The emulator supports the use of the stack and registers in order to perform basic arithmetic and memory based operations.

While progress may be slow, the future of the Plastic Assembly Language involves ongoing development and expansion of Plastic's instruction set. In the distant future, as Plastic becomes more adept at interfacing with registers, flags, memory, and the ALU in alignment with modern conventions, I plan to develop a CPU emulator to host the Plastic Assembly Language.

## How to build

This project uses only what exists in the repository and the c standard library. The make file utilizes gcc and with all of the above installed and loaded simply requires one command:
```shell
$ make
```

## Writing code

Plastic code is written in files ending in the .pls file extension. Each new line is a new instruction to be performed by the emulator. The instructions are formatted as:
```
opcode operand1 operand2 ... operand#
```
where the opcode specifies the operation to be performed on the two operands. Typically, in the case that operand1 is a register or memory location, the output value of the instruction will be stored in operand1. Otherwise, the output will be stored at the top of the stack.

e.g.
```
add eax 5
```
eax and 5 are operand1 and operand2 respectively.

### Registers

The registers are all 32 bit signed register, and their uses are as follows:

| Register | Use |
|----------|-----|
| eax      | General use. Used in the div operation. |
| ebx      | General use. |
| ecx      | General use. |
| edx      | General use. Used in the div operation. |
| ebp      | (UNIMPLEMENTED) Holds the memory location of the beginning of the current function. |
| esp      | Holds the memory location of the top of the stack. |
| eip      | Points to the current location in code being read by the emulator. |
| eir      | Holds the opcode of the current instruction being read by the emulator. |
| alu      | (INACCESSABLE) Holds the most recently performed arithmetic operation. |
| sv       | (INACCESSABLE) Used only in testing to test the value at the top of the stack. |
| flag     | Updated based on the most recent state of the alu. |

General use registers are acceptable for general use in instructions, however any register which is not marked as inaccessable can be used in an instruction. Use of any non general use registers is unadvised and may cause runtime errors, misplaced stack, base, and instruction pointers, and crashes.

General use registers are automatically set to 0 at the beginning of runtime.

To utilize a register in an instruction, simply type its name as an excepting operand.

e.g.
```
add eax 5
```

### Stack

The stack holds values which have been pushed to it. Pushed values grow to higher memory addresses. The esp register indicates the top of the stack (where the most recently pushed value lies).

### Flags

The flag register is currently mostly unimplemented.

### Memory

An instruction which utilizes memory addresses can access the value at the memory address pointed to by the indicated register. An instruction can also apply an offset (in bytes) to a memory address by adding or subtracting the operand directly following the register operand.

e.g.
```
set [esp] 5
```
```
set [esp]+2 5
```
```
set [esp]-2 5
```

### Modes

Modes are used in some instructions which require information on the type of the data being manipulated.

| Mode | symbol |
|------|--------|
| Integer | int |
| String  | str |

### Hexidecimal

Numbers can be written in hexidecimal instead of decimal. A number is indicated as hexidecimal if it has ```0x``` preceding the value.

e.g.
```
push 0x4b
```
Pushes the hexidecimal number 4b (75 in decimal) onto the stack.

### Strings

Strings are written with a surrounding ```"``` at the beginning and end of the string

e.g.
```
push "Hello, world!"
```
Pushes the characters 'H' 'e' 'l' 'l' 'o' ',' ' ' 'w' 'o' 'r' 'l' 'd' '!' '\0' onto the stop of the stack in order.

### Comments

```//``` indicates the beginning of a comment. Anywhere the parser sees a comment, it will ignore the rest of the line and move onto the next line.

### Instruction set

**No operation**

Written as:
```
nop
```
performs no operation.

**Push**

Written as:
```
push operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 |      | X      | X        |        | X      |      |             |

Pushes a value from operand1 onto the stack.

e.g.
```
push 5
```
Pushes the value 5 onto the top of the stack.
```
push eax
```
Pushes the value in the eax register onto the top of the stack.
```
push "Hello, world!"
```
Pushes the characters 'H' 'e' 'l' 'l' 'o' ',' ' ' 'w' 'o' 'r' 'l' 'd' '!' '\0' onto the stop of the stack in order.

**Pop**

Written as:
```
pop operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 |      |        | X        |        |        |      |             |

Pops a value from the stack into operand1.

e.g.
```
pop eax
```
Pops the top value off of the stack and places it into the eax register.

**Set**

Written as:
```
set operand1+-operand2 operand3
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 |      |        |          | X      |        |      |             |
| operand2 | X    | X      |          |        |        |      |             |
| operand3 |      | X      | X        |        | X      |      |             |

Sets the memory at memory address given by operand1 offset by operand2 (if operand1 is a memory address) to the value in operand3.

e.g.
```
set [esp]+2 5
```
Sets the value at the memory address given by the esp register offset by 2 to 5.

**Get**

Written as:
```
get operand1+-operand2 operand3 operand4
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 |      |        |          | X      |        |      |             |
| operand2 | X    | X      |          |        |        |      |             |
| operand3 |      |        | X        |        |        |      |             |
| operand4 |      |        |          |        |        | X    |             |

Retrieves the value at memory address given by operand1 offset by operand2 (if operand1 is a memory address) and places it into operand3. The mode is determined by operand4.

e.g.
```
get [esp]+2 eax int
```
Retrieves integer value at memory address given by the esp register offset by 2 and places it into the eax register.

**Move**

Written as:
```
mov operand1 operand2+-operand3
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 |      |        | X        |        |        |      |             |
| operand2 |      | X      | X        | X      |        |      |             |
| operand3 | X    | X      |          |        |        |      |             |

Sets operand1 to the value given by operand2 offset by operand3 (if operand2 is a memory address).

e.g.
```
mov eax [esp]-2
```
Sets the eax register to the value at the memory location given by the esp register offset by -2.
```
mov eax 5
```
Sets the eax register to 5.
```
mov eax ebx
```
Sets the eax register to the value in the ebx register.

**Add**

Written as:
```
add operand1 operand2+-operand3
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    |        | X        |        |        |      |             |
| operand2 | X    | X      | X        | X      |        |      |             |
| operand3 | X    | X      |          |        |        |      |             |

Adds the values in operand1 and operand2 offset by operand3 (if operand2 is a memory address). The result is stored in operand1. If only operand1 is used, the the value on top of the stack is popped and added to operand1. If no operands are used, both values are popped from the stack, added, and the result is pushed back onto the stack. The result is stored in operand1.

e.g.
```
add
Input stack:
    3
    5
Output stack:
    8
```
Pops and adds top two stack values and pushes result onto the stack.
```
add eax
```
Pops value from top of the stack and adds to the eax register.
```
add eax ebx
```
Adds the ebx register to the eax register.
```
add eax [esp]-2
```
Adds value at memory location given by the esp register offset by -2 to the eax register.

**Subtract**

Written as:
```
sub operand1 operand2+-operand3
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    |        | X        |        |        |      |             |
| operand2 | X    | X      | X        | X      |        |      |             |
| operand3 | X    | X      |          |        |        |      |             |

Subtracts the value in operand1 by operand2 offset by operand3 (if operand2 is a memory address). The result is stored in operand1.  If only operand1 is used, the the value on top of the stack is popped and subtracted from operand1. If no operands are used, both values are popped from the stack, the first value popped is subtracted from the second, and the result is pushed back onto the stack. The result is stored in operand1.

e.g.
```
sub
Input stack:
    3
    5
Output stack:
    2
```
Pops two values from the stack and subtracts the first popped value from the second popped value. Pushes result onto stack.
```
sub eax
```
Pops value from top of the stack and subtracts from the eax register.
```
sub eax ebx
```
Subtracts the ebx register from the eax register.
```
sub eax [esp]-2
```
Subtracts the value at memory location given by the esp register offset by -2 from the eax register.

**Multiply**

Written as:
```
mul operand1 operand2+-operand3
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    |        | X        |        |        |      |             |
| operand2 | X    | X      | X        | X      |        |      |             |
| operand3 | X    | X      |          |        |        |      |             |

Multiplies the value in operand1 by operand2 offset by operand3 (if operand2 is a memory address). The result is stored in operand1.  If only operand1 is used, the the value on top of the stack is popped and multiplied with operand1. If no operands are used, both values are popped from the stack, multiplied, and the result is pushed back onto the stack. The result is stored in operand1.

e.g.
```
mul
Input stack:
    3
    5
Output stack:
    15
```
Pops two values from the stack and multiplies the two values, and pushes result onto stack.
```
mul eax
```
Multiplies the eax register by a value popped from the stack.
```
mul eax ebx
```
Multiplies the eax register by the ebx register.
```
add eax [esp]-2
```
Multiplies eax register by the value at memory location given by the esp reguster offset by -2.

**Divide**

Written as:
```
div operand1 operand2+-operand3
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Combines the eax and edx registers to use the edx register as the first 32 bits of the numerator, and the eax register as the last 32 bits of the numerator. The new 64 bit number is then divided by operand1. The result is stored in the eax register and the remainer is stored in the edx register. If no operand is used, the value for the denominator will be popped from the stack.

```
div 4
Input registers:
    eax: 13
    edx: 0
Output registers:
    eax: 3
    edx: 1
```
Divides 13 by 4 and sets the eax register to the result and the edx register to the remainder.
```
div 8
Input registers:
    eax: 3
    edx: 2
Output registers:
    eax: 1073741824
    edx: 3
```
Bitshifts the number 2 to the left by 32 bits and ors it with 3 then divides it by 8. The result is placed in the eax register and the remainder is placed in the edx register.

**Increment**

Written as:
```
inc operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 |      |        | X        |        |        |      |             |

Increments the value in operand1 by 1.

**Decrement**

Written as:
```
dec operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 |      |        | X        |        |        |      |             |

Decrements the value in operand1 by 1.

**Jump**

Written as:
```
jmp operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Changes the eip register to operand1. If no operand is used, the jump destination will be popped from the stack.

**Jump if zero**

Written as:
```
jz operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Changes the eip register to operand1 if the alu is currently set to zero. If no operand is used, the jump destination will be popped from the stack.

**Jump if not zero**

Written as:
```
jnz operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Changes the eip register to operand1 if the alu is currently set to any number other than zero. If no operand is used, the jump destination will be popped from the stack.

**Jump if greater than**

Written as:
```
jg operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Changes the eip register to operand1 if the alu is currently set to a number greater than zero. If no operand is used, the jump destination will be popped from the stack.

**Jump if less than**

Written as:
```
jl operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Changes the eip register to operand1 if the alu is currently set to a number less than zero. If no operand is used, the jump destination will be popped from the stack.

**Jump if greater than or equal to**

Written as:
```
jgq operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Changes the eip register to operand1 if the alu is currently set to a number greater than or equal to zero. If no operand is used, the jump destination will be popped from the stack.

**Jump if less than or equal to**

Written as:
```
jlq operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Changes the eip register to operand1 if the alu is currently set to a number less than or equal to zero. If no operand is used, the jump destination will be popped from the stack.

**Jump relative**

Written as:
```
jr operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Increments the eip register by the number of bytes specified in operand1. If no operand is used, the jump offset will be popped from the stack.

**Jump relative if zero**

Written as:
```
jrz operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Increments the eip register by the number of bytes specified in operand1 if the alu is currently set to zero. If no operand is used, the jump offset will be popped from the stack.

**Jump relative if not zero**

Written as:
```
jrnz operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Increments the eip register by the number of bytes specified in operand1 if the alu is currently set to any number other than zero. If no operand is used, the jump offset will be popped from the stack.

**Jump relative if greater than**

Written as:
```
jr operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Increments the eip register by the number of bytes specified in operand1 if the alu is currently set to a number greater than zero. If no operand is used, the jump offset will be popped from the stack.

**Jump relative if less than**

Written as:
```
jrl operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Increments the eip register by the number of bytes specified in operand1 if the alu is currently set to a number less than zero. If no operand is used, the jump offset will be popped from the stack.

**Jump relative if greater than or equal to**

Written as:
```
jrgq operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Increments the eip register by the number of bytes specified in operand1 if the alu is currently set to a number greater than or equal to zero. If no operand is used, the jump offset will be popped from the stack.

**Jump relative if less than or equal to**

Written as:
```
jrlq operand1
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |

Increments the eip register by the number of bytes specified in operand1 if the alu is currently set to a number less than or equal to zero. If no operand is used, the jump offset will be popped from the stack.

**Compare**

Written as:
```
cmp operand1 operand2
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 | X    | X      | X        |        |        |      |             |
| operand1 | X    | X      | X        | X      |        |      |             |
| operand1 | X    | X      |          |        |        |      |             |

Sets the alu to operand1 minus operand2 offset by operand3 (if operand2 is a memory address). If operand2 If only operand1 is used, the the value on top of the stack is popped and subtracted from operand1. If no operands are used, both values are popped from the stack and the first value popped is subtracted from the second.

**Test**

Written as:
```
test operand1 operand2 ... operand# &
```
| Operand  | None | Number | Register | Memory | String | Mode | Instruction |
|----------|------|--------|----------|--------|--------|------|-------------|
| operand1 |      | X      |          |        |        |      |             |
| operand2 |      |        | X        |        |        |      |             |
| operand3 |      | X      |          |        |        |      | X           |

Outputs a failed test message to the terminal if the value of operand2 differs from the operand3. Operand1 indicates the id number of the test. Test can take any number of operands as long as each operand indicating a register has a number or instruction operand directly following it. Testing the eir register will test the instruction directly before the test instruction. Testing the eir register requires an opcode in the following operand.

e.g.
```
test 1 eax 3 &
```
Will output a failure to the terminal if the eax register is not set to 3.
```
test 2 eax 3 edx 12 &
```
Will output a failure to the terminal if the eax reguster is not set to 3 or the edx register is not set to 12.
```
test 3 eir sub &
```
Will output a failure to the terminal if the most recently executed instruction was not a sub instruction.
```
test 4 eax 3 ebx 0 edx 0 eir mov alu 3 esp -1 flag 0 &
```
Will output a failure to the terminal if the eax register is not set to 3, the edx register is not set to 0, the most recently executed instruction was not a mov instruction, the alu is not set to 3, the stack is not empty, or the flag register is not set to zero.

**Reset**

Written as:
```
reset
```
resets all registers, the stack, the code, the alu. The only registers which are not reset are the eip and eir registers.

**Halt**

Written as:
```
hlt
```
goes at the end of every program and signifies the end of the program for the assembler.

## Assembling
Assembling code written in the Plastic Assembly Language is simple. Run Plastic in the terminal with the following command:
```shell
$ ./Plastic -a [filepath]
```
where '[filepath]' is the path to the .pls file containing the Plastic code.

## Disassembling
To disassemble the out.pbc file for review, type:
```shell
$ ./Plastic -d out.pbc
```
which will output Plastic's interpretation of the bytecode file into the terminal.

## Running
To finally run the generated assembly code, type:
```shell
$ ./Plastic -r out.pbc
```
and the emulator will run the assembled bytecode.