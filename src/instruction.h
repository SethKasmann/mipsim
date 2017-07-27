#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <map>
#include <cassert>
#include "misc.h"
#include "register.h"
#include "memory.h"
#include "decode.h"

typedef uint32_t Instruction;
typedef void (*Alu)(Registers& r, Decoder& d, Memory& mem);
const int Null_instruction = 0;

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

const std::map<std::string, Instruction> Map_str_to_type =
{
    { "bltz", Bltz }, { "j",     J     }, { "jal",  Jal  }, { "beq",  Beq  },
    { "bne",  Bne  }, { "blez",  Blez  }, { "bgtz", Bgtz }, { "addi", Addi },
    { "ori",  Ori  }, { "xori",  Xori  }, { "lui",  Lui  }, { "lb",   Lb   },
    { "lh",   Lh   }, { "lw",    Lw    }, { "lbu",  Lbu  }, { "lhu",  Lhu  },
    { "sb",   Sb   }, { "sh",    Sh    }, { "sw",   Sw   }, { "sll",  Sll  },
    { "srl",  Srl  }, { "sra",   Sra   }, { "sllv", Sllv }, { "srlv", Srlv },
    { "srav", Srav }, { "jr",    Jr    }, { "jalr", Jalr }, { "sys",  Sys  },
    { "mfhi", Mfhi }, { "mthi",  Mthi  }, { "mflo", Mflo }, { "mtlo", Mtlo },
    { "mult", Mult }, { "multu", Multu }, { "div",  Div  }, { "divu", Divu },
    { "add",  Add  }, { "addu",  Addu  }, { "sub",  Sub  }, { "subu", Subu },
    { "and",  And  }, { "or",    Or    }, { "xor",  Xor  }, { "nor",  Nor  },
    { "slt" , Slt  }, { "sltu",  Sltu  },
    { "li"  , Addi }, { "move" ,  Add  }, { "bnez", Bne  }, {"beqz",  Beq  },
    { "la"  , Addi },
    { "syscall" , Sys }
};

void _null(Registers& r, Decoder& d, Memory& mem)
{
    std::cout << "NULL?...\n";
    return;
}

void _bltz(Registers& r, Decoder& d, Memory& mem)
{
    if (r[d.rs()] == r[d.rt()])
        r[pc] = d.imm();
}

void _beq(Registers& r, Decoder& d, Memory& mem)
{
    if (r[d.rs()] == r[d.rt()])
        r[pc] = d.imm();
}

void _bne(Registers& r, Decoder& d, Memory& mem)
{
    if (r[d.rs()] != r[d.rt()])
        r[pc] = d.imm();
}

void _addi(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] + d.imm();
}

void _ori(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] | d.imm();
}

void _lui(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = d.imm() << 16;
}

void _lb(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = static_cast<Register>(mem.fetch<Byte>(d.imm()));
}

void _sb(Registers& r, Decoder& d, Memory& mem)
{
    mem.store<Byte>(static_cast<Byte>(r[d.rt()]), r[d.rs()] + d.imm());
}

void _j(Registers& r, Decoder& d, Memory& mem)
{   
    r[pc] = d.imm();
}

void _add(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] + r[d.rt()];
}

void _and(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] & r[d.rt()];
}

void _div(Registers& r, Decoder& d, Memory& mem)
{
    r[lo] = r[d.rd()] / r[d.rt()];
    r[hi] = r[d.rd()] % r[d.rt()];
}

void _syscall(Registers& r, Decoder& d, Memory& mem)
{
    if (r[v0] == 1)
    {
        // Print int.
        assert(r[a0] < mem.size());
        std::cout << mem.fetch<Word>(r[a0]);
    }
    else if (r[v0] == 4)
    {
        // Print string.
        assert(r[a0] < mem.size());
        for (int i = r[a0]; i < mem.size(); ++i)
        {
            Byte b = mem.fetch<Byte>(i);
            if (b == '\0')
                break;
            std::cout << b;
        }
    }
    else if (r[v0] == 5)
    {
        std::cin >> r[v0];
    }
    else if (r[v0] == 10)
    {
        r.set_exit(true);
    }
    else if (r[v0] == 11)
    {
        assert(r[a0] < mem.size());
        std::cout << mem.fetch<Byte>(r[a0]);
    }
    else if (r[v0] == 12)
    {
        char c;
        std::cin >> c;
        r[v0] = static_cast<Register>(c);
    }
}

void _mfhi(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[hi];
}

void _mthi(Registers& r, Decoder& d, Memory& mem)
{
    r[hi] = r[d.rs()];
}

void _mflo(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[lo];
}

void _mtlo(Registers& r, Decoder& d, Memory& mem)
{
    r[lo] = r[d.rs()];
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

int set_rs(int i)
{
    return i << 21;
}
int set_rt(int i)
{
    return i << 16;
}
int set_rd(int i)
{
    return i << 11;
}

#endif