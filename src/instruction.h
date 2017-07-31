#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <map>
#include <cassert>
#include "register.h"
#include "memory.h"
#include "decode.h"

typedef uint32_t Instruction;
typedef void (*Alu)(Registers& r, Decoder& d, Memory& mem);

const int Max_instructions = 128;
const Instruction Null_instruction = 0;

const Instruction Sll   = 0x0;
const Instruction Srl   = 0x2;
const Instruction Sra   = 0x3;
const Instruction Sllv  = 0x4;
const Instruction Srlv  = 0x6;
const Instruction Srav  = 0x7;
const Instruction Jr    = 0x8;
const Instruction Jalr  = 0x9;
const Instruction Sys   = 0xc;
const Instruction Mfhi  = 0x10;
const Instruction Mthi  = 0x11;
const Instruction Mflo  = 0x12;
const Instruction Mtlo  = 0x13;
const Instruction Mult  = 0x19;
const Instruction Multu = 0x1a;
const Instruction Div   = 0x1b;
const Instruction Divu  = 0x1c;
const Instruction Add   = 0x20;
const Instruction Addu  = 0x21;
const Instruction Sub   = 0x22;
const Instruction Subu  = 0x23;
const Instruction And   = 0x24;
const Instruction Or    = 0x25;
const Instruction Xor   = 0x26;
const Instruction Nor   = 0x27;
const Instruction Slt   = 0x2a;
const Instruction Sltu  = 0x2b;

const Instruction Bltz  = 0x1  << 26;
const Instruction J     = 0x2  << 26;
const Instruction Jal   = 0x3  << 26;
const Instruction Beq   = 0x4  << 26;
const Instruction Bne   = 0x5  << 26;
const Instruction Blez  = 0x6  << 26;
const Instruction Bgtz  = 0x7  << 26;
const Instruction Addi  = 0x8  << 26;
const Instruction Addiu = 0x9  << 26;
const Instruction Slti  = 0xa  << 26;
const Instruction Sltiu = 0xb  << 26;
const Instruction Andi  = 0xc  << 26;
const Instruction Ori   = 0xd  << 26;
const Instruction Xori  = 0xe  << 26;
const Instruction Lui   = 0xf  << 26;
const Instruction Lb    = 0x20 << 26;
const Instruction Lh    = 0x21 << 26;
const Instruction Lw    = 0x22 << 26;
const Instruction Lbu   = 0x24 << 26;
const Instruction Lhu   = 0x25 << 26;
const Instruction Sb    = 0x28 << 26;
const Instruction Sh    = 0x29 << 26;
const Instruction Sw    = 0x2c << 26;

extern const std::map<std::string, Instruction> Map_str_to_type;
extern const Alu alu[Max_instructions];

/* R-Type instruction set. */
void _null(Registers& r, Decoder& d, Memory& mem);
void _sll(Registers& r, Decoder& d, Memory& mem);
void _srl(Registers& r, Decoder& d, Memory& mem);
void _sra(Registers& r, Decoder& d, Memory& mem);
void _sllv(Registers& r, Decoder& d, Memory& mem);
void _srlv(Registers& r, Decoder& d, Memory& mem);
void _srav(Registers& r, Decoder& d, Memory& mem);
void _jr(Registers& r, Decoder& d, Memory& mem);
void _jalr(Registers& r, Decoder& d, Memory& mem);
void _sys(Registers& r, Decoder& d, Memory& mem);
void _mfhi(Registers& r, Decoder& d, Memory& mem);
void _mthi(Registers& r, Decoder& d, Memory& mem);
void _mflo(Registers& r, Decoder& d, Memory& mem);
void _mtlo(Registers& r, Decoder& d, Memory& mem);
void _mult(Registers& r, Decoder& d, Memory& mem);
void _multu(Registers& r, Decoder& d, Memory& mem);
void _div(Registers& r, Decoder& d, Memory& mem);
void _divu(Registers& r, Decoder& d, Memory& mem);
void _add(Registers& r, Decoder& d, Memory& mem);
void _addu(Registers& r, Decoder& d, Memory& mem);
void _sub(Registers& r, Decoder& d, Memory& mem);
void _subu(Registers& r, Decoder& d, Memory& mem);
void _and(Registers& r, Decoder& d, Memory& mem);
void _or(Registers& r, Decoder& d, Memory& mem);
void _xor(Registers& r, Decoder& d, Memory& mem);
void _nor(Registers& r, Decoder& d, Memory& mem);
void _slt(Registers& r, Decoder& d, Memory& mem);
void _sltu(Registers& r, Decoder& d, Memory& mem);

/* I-Type instruction set. */
void _bltz(Registers& r, Decoder& d, Memory& mem);
void _j(Registers& r, Decoder& d, Memory& mem);
void _jal(Registers& r, Decoder& d, Memory& mem);
void _beq(Registers& r, Decoder& d, Memory& mem);
void _bne(Registers& r, Decoder& d, Memory& mem);
void _blez(Registers& r, Decoder& d, Memory& mem);
void _bgtz(Registers& r, Decoder& d, Memory& mem);
void _addi(Registers& r, Decoder& d, Memory& mem);
void _addiu(Registers& r, Decoder& d, Memory& mem);
void _slti(Registers& r, Decoder& d, Memory& mem);
void _sltiu(Registers& r, Decoder& d, Memory& mem);
void _andi(Registers& r, Decoder& d, Memory& mem);
void _ori(Registers& r, Decoder& d, Memory& mem);
void _xori(Registers& r, Decoder& d, Memory& mem);
void _lui(Registers& r, Decoder& d, Memory& mem);
void _lb(Registers& r, Decoder& d, Memory& mem);
void _lh(Registers& r, Decoder& d, Memory& mem);
void _lw(Registers& r, Decoder& d, Memory& mem);
void _lbu(Registers& r, Decoder& d, Memory& mem);
void _lhu(Registers& r, Decoder& d, Memory& mem);
void _sb(Registers& r, Decoder& d, Memory& mem);
void _sh(Registers& r, Decoder& d, Memory& mem);
void _sw(Registers& r, Decoder& d, Memory& mem);

#endif