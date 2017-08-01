# MipSim
MipSim is a MIPS assembly simulator written in c++. The purpose is to open a text file of mips assembly code (.s)
and interpret, compile, and execute the code simulating MIPS as much as possible.

# Instruction Set
Here's a list of the current instructions MipSim can interpret:

## R-Type  
* sll (shift left logical)  
* srl (shift right logical)  
* sra (shift right arithmetic)  
* sllv (shift left logical variable)  
* srlv (shift right logical variable)  
* srav (shift right arithmetic variable)  
* jr (jump register)  
* jalr (jump and link register)  
* syscall (1, 4, 5, 8, 10, 11, 12)  
* mfhi (move from hi)  
* mthi (move to hi)  
* mflo (move from lo)  
* mtlo (move to lo)  
* mult (multiply with two register arguments)  
* multu (multiply unsigned with two register arguments)  
* div (divide with two register arguments)  
* divu (divide unsigned with two register arguments)  
* add (add with overflow exception)  
* addu (add without overflow exception)  
* sub (subtract with overflow exception)  
* subu (subtract without overflow exception)  
* and (logical and)  
* or (logical or)  
* xor (logical xor)  
* nor (logical nor)  
* slt (set less than)  
* sltu (set less than unsigned)  

## I-Type
* bltz (branch if less than zero)  
* j (jump)  
* jal (jump and link)  
* beq (branch if equal) 
* bne (branch if not equal)  
* blez (branch if less than or equal to zero)  
* bgtz (branch if greater than zero)   
* addi (add immediate with overflow exception)  
* addiu (add immediate without overflow exception)  
* slti (set if less than immediate)  
* sltiu (set if less than immediate unsigned)  
* andi (logical and with immediate)  
* ori (logical or with immediate)  
* xori (logical xor with immediate)  
* lui (load upper immediate)  
* lb (load byte)  
* lh (load halfword)  
* lw (load word)  
* lbu (load byte unsigned)  
* lhu (load halfword unsigned)  
* sb (store byte)  
* sh (store halfword)  
* sw (store word)  

# Pseudo Instructions
MipSim can interpret most native MIPS instructions, but in it's current stage can only handle a handful of pseudo instructions.
Here's a list of the pseudo instructions MipSim can run:  
* li (load immediate)  
* la (load address)  
* mov (move)  
* bnez (branch if not equal zero)  
* beqz (branch if equal zero)  

# Bugs
If you find any bugs or errors please contact me (Seth) at: sakasmann1@cougars.ccis.edu
