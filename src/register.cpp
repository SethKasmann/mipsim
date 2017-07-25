#include "register.h"

int Registers::get_pc() const
{
	return _pc;
}

uint32_t Registers::get_ir() const
{
    return _ir;
}

void Registers::set_ir(uint32_t i)
{
    _ir = i;
}
void Registers::operator++(int)
{
    _pc += 4;
}
void Registers::operator=(int i)
{
    _pc = i;
}
int& Registers::operator[](int i)
{
    return i == pc ? _pc
         : i == lo ? _lo
         : i == hi ? _hi
         : _r[i];
}
int Registers::opcode() const
{
    return (_ir & 0xfc000000) >> 26;
}
int Registers::rs() const
{
    return (_ir & 0x3e00000) >> 21;
}
int Registers::rt() const
{
    return (_ir & 0x1f0000) >> 16;
}
int Registers::rd() const
{
    return (_ir & 0xf800) >> 11;
}
int Registers::imm() const
{
    return _ir & 0xffff;
}
int Registers::funct() const
{
    return _ir & 0x3f;
}
int Registers::fetch(int reg) const
{
	return _r[reg];
}
void Registers::write_back(int reg, int val)
{
	_r[reg] = val;
}