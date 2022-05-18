The VC8000 Computer
The VC8000 is a decimal computer with 1,000,000 words of memory. Each word consists of 9 decimal digits. There is an additional 10 words of memory called registers. These are on chip memory.  The machine language instructions for the VC8000 is of the following form  Note: each has the same number of digits.

 

                             |2 digits             | 1 digit                 |6 digits             |
                             |operation code | register number |address portion|  

or

                             |2 digits             | 1 digit                 |1 digit               |  5 digits     |
                             |operation code | register number |register number| unused      |

 A machine language program is a sequence of machine language instructions stored in memory. The computer runs a machine language program by executing machine instructions stored in successive words of memory. The VC8000 machine assumes that the first instruction to be executed will be at location 100. The following are the machine language instructions for the VC8000.

NAME           OP. CODE     MEANING

ADD                     01           Reg <-- c(Reg) + c(ADDR)   (The contents of the register specified in the instruction and of the memory location specified by the address portion of the instruction are added together. The result is placed in the register.):

SUBTRACT         02            Reg <-- c(Reg) - c(ADDR)

MULTIPLY           03            Reg <-- c(Reg) * c(ADDR)

DIVIDE                 04            Reg <-- c(Reg) / c(ADDR)

LOAD                   05            Reg <-- c(ADDR)

STORE                06            ADDR <-- c(Reg)

ADD REG            07            REG1 <--c(REG1) + c(REG2)   (Note: this is the second instruction format where two registers are specified.)

SUB REG            08            REG1 <--c(REG1) - c(REG2)  

MULT REG          09            REG1 <--c(REG1) * c(REG2)  

DIV REG              10            REG1 <--c(REG1) / c(REG2)  

READ                   11            A line is read in and the number found there is recorded in the specified memory address.  The register value is ignored.

WRITE                 12            c(ADDR) is displayed  The register value is ignored.

BRANCH              13           go to ADDR for next instruction.  The register value is ignored.

BRANCH MINUS  14          go to ADDR if c(Reg) < 0

BRANCH ZERO    15          go to ADDR if c(Reg) = 0

BRANCH POSITIVE 16       go to ADDR if c(Reg) > 0

HALT                    17           terminate execution.  The register value and address are ignored.

Example of a test program: (Yes, this program does not do much.) 
;this is a test
        org    100
hi     read    x;this comment is immediately after statement
        load    1,x
hay   store   1,y ; This is the another comment.
          write    1,x
        bp      1,hi
        halt
    ;test comment
x      dc      5
y      ds      99
b      dc      555
a      dc      100
        end

![output 1](https://user-images.githubusercontent.com/94247776/169140270-c3494ba8-21a7-4a3d-9505-ce44abc7bf8e.png)


Another Example: 
The following is an assembler language program which will read in a number "n" and then compute and print the value of n!.
 
```
                org 100
                read 0, n
more        load  1, n; This is a comment

;Here is a comment that sit on its own line.
                mult 1, fac
                store 1, fac
                load 1, n
                sub 1, one
                store 1, n
                bp 1,more
                write 0, fac
                halt
n              ds 100; just to show that you code can handle big areas.
fac           dc 1
one          dc 1
test          dc 1234 ; show your program can handle big constants.
                end
```                
![fac](https://user-images.githubusercontent.com/94247776/169140545-1538dce6-ea03-48f0-923a-621482f56799.png)



      
