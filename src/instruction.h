#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <map>
#include "misc.h"
#include "register.h"

typedef uint32_t Instruction;
const int Null_instruction = 0;

const Instruction Addi = 1 << 26;
const Instruction Addiu = 2 << 26;
const Instruction Andi = 3 << 26;
const Instruction Beq = 4 << 26;
const Instruction Bne = 5 << 26;
const Instruction Lbu = 6 << 26;
const Instruction Lhu = 7 << 26;
const Instruction Lui = 8 << 26;
const Instruction Lw = 9 << 26;
const Instruction Ori = 10 << 26;
const Instruction Sb = 11 << 26;
const Instruction Sh = 12 << 26;
const Instruction Slti = 13 << 26;
const Instruction Sltiu = 14 << 26;
const Instruction Sw = 15 << 26;
const Instruction J = 16 << 26;
const Instruction Jal = 17 << 26;
const Instruction Li = 18 << 26;
const Instruction La = 19 << 26;
const Instruction Lb = 20 << 26;
const Instruction Rem = 21 << 26;
const Instruction Bnez = 22 << 26;
const Instruction Sys = 31 << 26;

const Instruction Add = 1;
const Instruction Addu = 2;
const Instruction And = 3;
const Instruction Div = 4;
const Instruction Divu = 5;
const Instruction Jr = 6;
const Instruction Mfc0 = 7;
const Instruction Mfhi = 8;
const Instruction Mflo = 9;
const Instruction Mult = 10;
const Instruction Multu = 11;
const Instruction Nor = 12;
const Instruction Or = 13;
const Instruction Sll = 14;
const Instruction Slt = 15;
const Instruction Sltu = 16;
const Instruction Sra = 17;
const Instruction Srl = 18;
const Instruction Sub = 19;
const Instruction Subu = 20;
const Instruction Xor = 21;
const Instruction Move = 22;

const std::map<std::string, Instruction> Map_str_to_type =
{
    { "addi", Addi }, { "addiu", Addiu }, { "andi", Andi }, { "beq",   Beq   },
    { "bne",  Bne  }, { "lbu",   Lbu   }, { "lhu",  Lhu  }, { "lui",   Lui   },
    { "lw",   Lw   }, { "ori",   Ori   }, { "sb",   Sb   }, { "sh",    Sh    },
    { "slti", Slti }, { "sltiu", Sltiu }, { "sw",   Sw   }, { "j",     J     },
    { "jal",  Jal  }, { "add",   Add   }, { "addu", Addu }, { "and",   And   },
    { "div",  Div  }, { "divu",  Divu  }, { "jr",   Jr   }, { "mfc0",  Mfc0  },
    { "mfhi", Mfhi }, { "mflo",  Mflo  }, { "mult", Mult }, { "multu", Multu },
    { "nor",  Nor  }, { "or",    Or    }, { "sll",  Sll  }, { "slt",   Slt   },
    { "sltu", Sltu }, { "sra",   Sra   }, { "srl",  Srl  }, { "sub",   Sub   },
    { "subu", Subu }, { "xor",   Xor   }, { "li" ,  Li   }, { "la" ,   La    },
    { "lb"  , Lb   }, { "rem",   Rem   }, { "syscall", Sys }, {"bnez", Bnez  },
    { "move", Move }
};

void li(Register& rs, Register rt, int imm, std::vector<Instruction>& mem)
{
    rs = imm;
}

void la(Register& rs, Register rt, int imm, std::vector<Instruction>& mem)
{
    rs = imm;
}

void lb(Register& rs, Register rt, int imm, std::vector<Instruction>& mem)
{
    rs = imm;
}

void sb(Register& rs, Register rt, int imm, std::vector<Instruction>& mem)
{
    mem[rt + imm] = rs;
}

bool syscall(Register& v0, Register a0, Register a1, std::vector<Instruction>& mem)
{
    if (v0 == 1)
    {
        // Print int.
        assert(a0 < mem.size());
        std::cout << mem[a0];
    }
    else if (v0 == 4)
    {
        // Print string.
        assert(a0 < mem.size());
        auto it = mem.begin() + a0;
        while (it != mem.end() && *it != '\0')
            std::cout << static_cast<char>(*it);
    }
    else if (v0 == 5)
    {
        std::cin >> v0;
    }
    else if (v0 == 10)
    {
        return true;
    }
    else if (v0 == 11)
    {
        assert(a0 < mem.size());
        std::cout << static_cast<char>(mem[a0]);
    }
    else if (v0 == 12)
    {
        char c;
        std::cin >> c;
        v0 = static_cast<int>(c);
    }
    return false;
}

bool j_type(Instruction i)
{
    return i == J || i == Jal || i == Jr;
}

bool i_type(Instruction i)
{
    return i & 0xfc000000;
}

bool r_type(Instruction i)
{
    return i & 0x3f;
}

Instruction generate(const std::string& s)
{
    std::string token = "";
    Instruction i;
    auto it = s.begin();
    while (it != s.end() && *it != '$')
    {
        if (isspace(*it))
        {
            if (token == "")
                continue;
            else
                break;
        }
        token.push_back(*it);
    }
    lowercase(token);
    // May need to check if there is no instruction (eg, next to a label)
    i = Map_str_to_type.at(token);
    return 0;
}

int opcode(Instruction i)
{
    return (i & 0xfc000000) >> 26;
}

int rs(Instruction i)
{
    return (i & 0x3e00000) >> 21;
}

int set_rs(int i)
{
    return i << 21;
}

int rt(Instruction i)
{
    return (i & 0x1f0000) >> 16;
}

int set_rt(int i)
{
    return i << 16;
}

int rd(Instruction i)
{
    return (i & 0xf800) >> 11;
}

int set_rd(int i)
{
    return i << 11;
}

int shift(Instruction i)
{
    return (i & 0x7c0) >> 6;
}

int funct(Instruction i)
{
    return i & 0x3f;
}

int imm(Instruction i)
{
    return i & 0xffff;
}

int jaddress(Instruction i)
{
    return i & 0x3ffffff;
}

#endif