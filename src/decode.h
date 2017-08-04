#ifndef DECODE_H
#define DECODE_H

typedef uint32_t Instruction;

/* Generates a compile time bitmask between bit indecies X and Y inclusive. */
template<int X, int Y, bool B=false>
struct BitMask
{
	static const Instruction value = 1 << X | BitMask<X - 1, Y, X == Y>::value;
};

/* Base case specialization. */
template<int X, int Y>
struct BitMask<X, Y, true>
{
	static const Instruction value = 0;
};

/* ANDS the instruction to the bitmask and shifts bits to the lsb. */
template<int X, int Y = X>
struct Bits
{
	int operator()(Instruction x)
	{
		return (x & BitMask<X, Y>::value) >> Y;
	}
};

/* Specialization when determining the opcode (bits 31 - 26). If the opcode
   is not 0, add the I-Flag (64). */
template<int Y>
struct Bits<31, Y>
{
	int operator()(Instruction x)
	{
		x = (x & BitMask<31, Y>::value) >> Y;
		return x ? x + 64 : x;
	}
};

/* Decoder class used to store each part of a Mips instruction.
	    opcode - Indicates operation, or arithmetic family of operations.
	             Instead of a separate funct code variable for R-Type 
	             instructions, if the opcode is 0 (R-Type) the funct code 
	             is stored. If the opcode is not 0 (I-Type) then a flag (64) 
	             is added to the opcode and stored. This way R-Type and I-Type
	             instrcutions can index the same array (0 - 127).
	        rs - Source register for R-Type and I-Type.
	        rt - Source register for R-Type, destination register for I-Type.
	        rd - Destination register for R-Type.
	     shamt - Shift amount.
	     addrs - 26 bit address for J-Type.
	       imm - 16-bit signed immediate value for I-Type. */
struct Decoder
{
	// Stores the opcode and function arguments of an instruction.
	void decode(Instruction i)
	{
		opcode = Bits<31, 26>()(i);
		rs     = Bits<25, 21>()(i);
		rt     = Bits<20, 16>()(i);

		// I-Type and J-Type
		if (opcode)
		{
			imm    = Bits<15, 0 >()(i);
			addrs  = Bits<25, 0>()(i);
		}
		// R-Type
		else
		{
			rd     = Bits<15, 11>()(i);
			shamt  = Bits<10, 6 >()(i);
			opcode = Bits< 5, 0 >()(i);
		}
	}
	int opcode;
	int rs;
	int rt;
	int rd;
	int shamt;
	int addrs;
	short imm;
};

#endif