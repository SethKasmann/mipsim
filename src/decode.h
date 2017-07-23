#ifndef DECODE_H
#define DECODE_H

typedef uint32_t Instruction;

const uint32_t I_flag = 0x40;

enum FunctionType
{
	r_type,
	i_type,
	j_type
};

class Decoder
{
public:
	void decode(Instruction i)
	{
		opcode = param1 = param2 = param3 = 0;
		FunctionType t = get_type(i);

		if (t == r_type)
			decode<r_type>(i);
		else if (t == i_type)
			decode<i_type>(i);
		else if (t == j_type)
			decode<j_type>(i);
	}
	template<FunctionType> void decode(Instruction i);
	friend std::ostream& operator<<(std::ostream& o, const Decoder & d);
private:
	uint32_t opcode(Instruction i) const
	{
	    return (i & 0xfc000000) >> 26;
	}
	uint32_t rs(Instruction i) const
	{
	    return (i & 0x3e00000) >> 21;
	}
	uint32_t rs() const
	{
		return param1;
	}
	uint32_t rt() const
	{
		return param2;
	}
	uint32_t rd() const
	{
		return param3;
	}
	uint32_t imm() const
	{
		return param3;
	}
	uint32_t addaress() const
	{
		return param1;
	}
	uint32_t shamt() const
	{
		return param3;
	}
	uint32_t rt(Instruction i) const
	{
	    return (i & 0x1f0000) >> 16;
	}
	uint32_t rd(Instruction i) const
	{
	    return (i & 0xf800) >> 11;
	}
	uint32_t imm(Instruction i) const
	{
	    return i & 0xffff;
	}
	uint32_t funct(Instruction i) const
	{
	    return i & 0x3f;
	}
	uint32_t shamt(Instruction i) const
	{
		return (i & 0x7c0) >> 6;
	}
	uint32_t address(Instruction i) const
	{
		return i & 0x3ffffff;
	}
	FunctionType get_type(Instruction i)
	{
		uint32_t op = opcode(i);
		return op == 0 ? r_type : op == 2 || op == 3 ? j_type : i_type;
	}
	uint32_t fun_index;
	uint32_t param1;
	uint32_t param2;
	uint32_t param3;
};

std::ostream& operator<<(std::ostream& o, const Decoder & d)
{
	o << "<Decoder"
	  << ", fun_index:" << d.fun_index 
	  << ", param1:"    << param1      
	  << ", param2:"    << param2 
	  << ", param3:"    << param3 
	  << ">"            << std::endl;
}

template<>
inline void Decoder::decode<r_type>(Instruction i)
{
	fun_index = funct(i);
	param1 = rd(i);
	param2 = rt(i);
	// Special case for shifts.
	if (opcode <= 3)
		param3 = shamt(i);
	else
		param3 = rs(i);
}

template<>
inline void Decoder::decode<i_type>(Instruction i)
{
	fun_index = opcode(i) | I_flag;
	param1 = rs(i);
	param2 = rt(i);
	param3 = imm(i);
}

template<>
inline void Decoder::decode<j_type>(Instruction i)
{
	fun_index = opcode(i);
	param1 = address(i);
}

#endif