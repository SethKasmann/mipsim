#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <map>
#include <cassert>
#include "misc.h"
#include "register.h"
#include "memory.h"

typedef uint32_t Instruction;
const int Null_instruction = 0;

const Instruction J = 0x2;
const Instruction Jal = 0x3;
const Instruction Beq = 0x4;
const Instruction Bne = 0x5;
const Instruction Blez = 0x6;
const Instruction Bgtz = 0x7;
const Instruction Addi = 0x8;
const Instruction Addiu = 0x9;
const Instruction Slti = 0xa;
const Instruction Sltiu = 0xb;
const Instruction Andi = 0xc;
const Instruction Ori = 0xd;
const Instruction Xori = 0xe;
const Instruction Lui = 0xf;
const Instruction Lb = 0x20;
const Instruction Lh = 0x21;
const Instruction Lw = 0x22;
const Instruction Lbu = 0x24;
const Instruction Lhu = 0x25;
const Instruction Sb = 0x28;
const Instruction Sh = 0x29;
const Instruction Sw = 0x2c;
/*
const Instruction La = 2 << 26;
const Instruction Bnez = 7 << 26;
const Instruction Li = 8 << 26;
const Instruction Rem = 10 << 26;
const Instruction Sys = 31 << 26;
*/

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

void addi(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] + d.imm();
}

void li(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "In LI" << '\n';
    //std::cout << "rs:" << rs << " rt:" << rt << " imm:" << imm << '\n';
    r[rs] = imm;
    r++;
}

void la(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "In LA" << '\n';
    r[rs] = imm;
    r++;
}

void lb(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "In LB" << '\n';    
    r[rs] = static_cast<Register>(mem.fetch<Byte>(imm));
    r++;
}

void sb(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "In SB" << '\n';
    mem.store<Byte>(static_cast<Byte>(r[rs]), r[rt] + imm);
    r++;
}

void beq(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "beq\n";
    std::cout << r[rs] << " " << r[rt] << '\n';
    if (r[rs] == r[rt])
    {
        std::cout << "in here...\n";
        r = imm;
    }
    else
        r++;
}

void bne(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "In BNE" << '\n';    
    if (r[rs] != r[rt])
        r = imm;
    else
        r++;
}

void bnez(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "In BNEZ" << '\n';    
    if (r[rs] != r[0])
        r = imm;
    else
        r++;
}

void j(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "In J" << '\n';    
    r = imm;
    //r++;
}

void rem(Registers& r, int rs, int rt, int imm, Memory& mem)
{
    std::cout << "In REM" << '\n';    
    assert(imm > 0);
    r[rs] = r[rt] % imm;
    r++;
}

void add(Registers& r, int rs, int rt, int rd, Memory& mem)
{
    r[rs] = r[rt] + r[rd];
    r++;
    }

    void _and(Registers& r, int rs, int rt, int rd, Memory& mem)
{
    r[rs] = r[rt] & r[rd];
    r++;
}

void move(Registers& r, int rs, int rt, int rd, Memory& mem)
{
    std::cout << "In MOVE" << '\n';  
    std::cout << r[rs] << " " << r[rt] << '\n';
    r[rs] = r[rt];
    std::cout << r[rs] << " " << r[rt] << '\n';
    r++;
}

bool syscall(Registers& r, Memory& mem)
{
    std::cout << "In SYSCALL" << '\n';    
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
        return true;
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
    r++;
    return false;
}

/*
bool i_type(Instruction i)
{
    return i & 0xfc000000;
}

bool r_type(Instruction i)
{
    return i & 0x3f;
}*/

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