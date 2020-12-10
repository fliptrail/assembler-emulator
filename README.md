AUTHOR: Amish Mittal (fliptrail)

* It is compilable with: g++ assembler.cpp -o asm and g++ emu.cpp -o emu
* To execute: ./asm test1.asm and ./emu test5.o

# Program format and structure:

1. The assembler:
	* Uses single routine for both pases to assemble the program
	* Handles extra spaces between operands and extra whitespaces except for labels for which space is necessary after semicolon.
	* Handles comments in the same line and separate lines
	* Can get data in decimal, octal and hexadecimal 2s complement
	* It can detect the following errors:
		* Incorrect mnemonic
		* Label name is a programming keyword
		* Incorrect label naming format (allows for underscore in beginning but doesn't allow numerics - in start or other special characters)
		* Label repeated
		* Label missing
		* Missing operand
		* More operands than expected (incorrect operand format)
	* It can detect the following warnings:
		* Unused labels
		* Possible numeric overflow

	* Keeps a track of mnemonic, opcodes and possible operands
	* Keeps a track of the labels
	* Generates a log file
	* Generates an advanced listing file (showing the bytes produced for each instruction, and that instruction's mnemonic)
	* Generates an object file
	* Listing and object files are only created if there are no errors. They may be created if there are only warnings
	* Assembles the pseudo instruction SET
	
2. The emulator:

	* Can load object file
	* can produce memory dump
	* can execute test program
	* has -t (trace), -dump and -all options
	* can detect errors:
		* Incorrect machine code
		* Segmentation fault
		* Stack overflow
		* Invalid opcode
	* can print metadata such as number of instructions executed, hexadecimal forms, memory dump, commands can be changed during runtime

## Sample codes: 

1. The assembler:

I have tested six examples shown as below. The errors only occurred in the file test2.asm and test6.asm. Bubble.asm (bubble sort) is working.

The others were tested without any errors (warnings might be present) detected and produced three output files: listing file, 
error log file and machine readable object file, for each example. 
 

1)
#Input: ./asm test1.asm
#Output: 
Compilation Successful. Log file with warnings: test1.log
The listing file is: test1.l
The object file is: test1.o

Warnings:
Warning: label "label" not used

2)
#Input: ./asm test2.asm
#Output: 
Compilation errors encountered. Log file with errors and warnings: test2.log
Errors:
Label already used at line: 4
Label is empty or starts with a character other than an alphabet and underscore at line: 10
nonesuch label does not exist, line: 5
Wrong Operand format at line: 6
Operand absent at position line: 7
Invalid or extra operand at line: 8
More number of operands at line: 9
Invalid mnemonic at line: 11
Invalid mnemonic at line: 12
Warning: label "label" not used

Object and Listing file is not created here due to compilation errors.

3)
#Input: ./asm test3.asm
#Output: 
Compilation Successful. Log file with warnings: test3.log (this file is empty due to no warning)
The listing file is: test3.l
The object file is: test3.o

4)
#Input: ./asm test4.asm (space added at line 39 after label triangle)
#Output: 
Compilation Successful. Log file with warnings: test4.log (this file is empty due to no warning)
The listing file is: test4.l
The object file is: test4.o

5)
#Input: ./asm test5.asm
#Output: 
Compilation Successful. Log file with warnings: test5.log
The listing file is: test5.l
The object file is: test5.o

Warnings:
Warning: label "mylabel" not used

6)
#Input: ./asm test6.asm
#Output: 
Compilation errors encountered. Log file with errors and warnings: test6.log
Errors:
Label is empty or starts with a character other than an alphabet and underscore at line: 3

(The label started with number 2)

7)
#Input: bubble.asm
#Output:
Compilation Successful. Log file with warnings: bubble.log (this file is empty due to no warning)
The listing file is: bubble.l
The object file is: bubble.o

2. The emulator:

1) 
#Input: ./emu test1.o
#Output:
-t for trace
-dump for memory dump
-all for all commands
Enter with hyphen
Emulator input: -t
ldc     00000000
A = 00000000, B = 00000000, PC = 00000001, SP = 00000000
Emulator input: -t
ldc     FFFFFFFB
A = FFFFFFFB, B = 00000000, PC = 00000002, SP = 00000000
Emulator input: -t
ldc     00000005
A = 00000005, B = FFFFFFFB, PC = 00000003, SP = 00000000

and so on

2) Not possible to emulatate test2.asm as the assembly code is incorrect (so no object file).

3)
#Input: ./emu test3.o
#Output:
-t for trace
-dump for memory dump
-all for all commands
Enter with hyphen
Emulator input: -t
        00000000
Invalid opcode. Incorrect machine code. Aborting(base)

Working as expected - the SET instruction was at start of the file. Assembly code was correct, but corresponding machine code was not present. 
By programmer it has to placed in the end. But the emulator is correct.

4)
#Input: ./emu test4.o
#Output:
Using -all
Last few lines:
A = 00000000, B = 00000000, PC = 00000008, SP = 00000FFF
adj     00000001
A = 00000000, B = 00000000, PC = 00000009, SP = 00001000
HALT    00000000
Total instructions executed: 47654

Working as expected

5)
#Input: ./emu test5.o
#Output:
-t for trace
-dump for memory dump
-all for all commands
Enter with hyphen
Emulator input: -t
ldc     00000005
A = 00000005, B = 00000000, PC = 00000001, SP = 00000000
Emulator input: -t
ldc     FFFFFFFB
A = FFFFFFFB, B = 00000005, PC = 00000002, SP = 00000000
Emulator input: -dump
Base address: 0
No. of values: 12
00000000 00000500 FFFFFB00 00000012 00000000
00000004 00000000 00000000 00000000 00000000
00000008 00000000 00000000 00000000 00000000
Emulator input: -all
HALT    00000000
Total instructions executed: 3

Working as expected. Memory dump correct.

6) Not possible to emulatate test6.asm as the assembly code is incorrect (so no object file).

7)
#Input: ./emu bubble.o
#Output:
A = 00000000, B = 00000001, PC = 00000017, SP = 00000FFB
brz     00000026
A = 00000000, B = 00000001, PC = 0000003E, SP = 00000FFB
ldl     00000003
A = 00000006, B = 00000000, PC = 0000003F, SP = 00000FFB
adj     00000005
A = 00000006, B = 00000000, PC = 00000040, SP = 00001000
return  00000000
A = 00000000, B = 00000000, PC = 00000007, SP = 00001000
HALT    00000000
Total instructions executed: 1044

Working as expected
