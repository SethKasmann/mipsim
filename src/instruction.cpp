#include "instruction.h"

/* Hash table to convert a function name from the source code to it's
   respective opcode/funct code. */
const std::map<std::string, Instruction> Map_str_to_type =
{
    // R-Type
    { "sll",     Sll   }, { "srl",  Srl  }, { "sra",   Sra   }, { "sllv", Sllv }, 
    { "srlv",    Srlv  }, { "srav", Srav }, { "jr",    Jr    }, { "jalr", Jalr }, 
    { "syscall", Sys   }, { "mfhi", Mfhi }, { "mthi",  Mthi  }, { "mflo", Mflo }, 
    { "mtlo",    Mtlo  }, { "mult", Mult }, { "multu", Multu }, { "div",  Div  }, 
    { "divu",    Divu  }, { "add",  Add  }, { "addu",  Addu  }, { "sub",  Sub  }, 
    { "subu",    Subu  }, { "and",  And  }, { "or",    Or    }, { "xor",  Xor  }, 
    { "nor",     Nor   }, { "slt" , Slt  }, { "sltu",  Sltu  },
    // I-Type
    { "bltz",    Bltz  }, { "j",    J    }, { "jal",   Jal   }, { "beq",  Beq  },
    { "bne",     Bne   }, { "blez", Blez }, { "bgtz",  Bgtz  }, { "addi", Addi },
    { "addiu",   Addiu }, { "slti", Slti }, { "sltiu", Sltiu }, { "andi", Andi },
    { "ori",     Ori   }, { "xori", Xori }, { "lui",   Lui   }, { "lb",   Lb   },
    { "lh",      Lh    }, { "lw",   Lw   }, { "lbu",   Lbu   }, { "lhu",  Lhu  },
    { "sb",      Sb    }, { "sh",   Sh   }, { "sw",    Sw    },
    // Pseudo Instructions.
    { "li"  ,    Addi  }, { "move", Add  }, { "bnez",  Bne   }, {"beqz",  Beq  },
    { "la"  ,    Addi  }
};

/* Array of function pointers to the correct shift/op/alu function. The opcode
   or funct code is used as an index to the array. For R type instructions, the
   funct code is used directly. For I type instructions, 64 is added to the
   original opcode so R type and I type can be indexed from the same array. 
   Null is used if the respective functionality has not been added. */
const Alu alu[Max_instructions] = 
{
     _sll,  _null,   _srl,   _sra, _sllv, _null, _srlv, _srav,
      _jr,  _jalr,  _null,  _null,  _sys, _null, _null, _null,
    _mfhi,  _mthi,  _mflo,  _mtlo, _null, _null, _null, _null,
    _null,  _mult, _multu,   _div, _divu, _null, _null, _null,
     _add,  _addu,   _sub,  _subu,  _and,   _or,  _xor,  _nor,
    _null,  _null,  _null,   _slt, _sltu, _null, _null, _null,
    _null,  _null,  _null,  _null, _null, _null, _null, _null,
    _null,  _null,  _null,  _null, _null, _null, _null, _null,
    _null,  _bltz,     _j,   _jal,  _beq,  _bne, _blez, _bgtz,
    _addi, _addiu,  _slti, _sltiu, _andi,  _ori, _xori,  _lui,
    _null,  _null,  _null,  _null, _null, _null, _null, _null,
    _null,  _null,  _null,  _null, _null, _null, _null, _null,
      _lb,    _lh,    _lw,  _null,  _lbu,  _lhu, _null, _null,
      _sb,    _sh,    _sw,  _null, _null, _null, _null, _null,
    _null,  _null,  _null,  _null, _null, _null, _null, _null
};

void _null(Registers& r, Decoder& d, Memory& mem)
{
    std::cout << "NULL?...\n";
    return;
}

void _sll(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rt()] << d.shamt();
}

void _srl(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rt()] >> d.shamt();
}

void _sra(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = (r[d.rt()] >> d.shamt()) | (r[d.rt()] & 0x80000000);
}

void _sllv(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rt()] << r[d.rs()];
}

void _srlv(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rt()] >> r[d.rs()];
}

void _srav(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = (r[d.rt()] >> r[d.rs()]) | (r[d.rt()] & 0x80000000);
}

void _jr(Registers& r, Decoder& d, Memory& mem)
{
    r[pc] = r[d.rd()];
}

void _jalr(Registers& r, Decoder& d, Memory& mem)
{
    r[ra] = r[pc] + 4;
    r[pc] = r[d.rd()];
}

void _sys(Registers& r, Decoder& d, Memory& mem)
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

void _mult(Registers& r, Decoder& d, Memory& mem)
{
    r[lo] = r[d.rd()] * r[d.rt()];
}

void _multu(Registers& r, Decoder& d, Memory& mem)
{
    r[lo] = r[d.rd()] * r[d.rt()];
}

void _div(Registers& r, Decoder& d, Memory& mem)
{
    r[lo] = r[d.rd()] / r[d.rt()];
    r[hi] = r[d.rd()] % r[d.rt()];
}

void _divu(Registers& r, Decoder& d, Memory& mem)
{
    r[lo] = r[d.rd()] / r[d.rt()];
    r[hi] = r[d.rd()] % r[d.rt()];
}

void _add(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] + r[d.rt()];
}

void _addu(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] + r[d.rt()];
}

void _sub(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] - r[d.rt()];
}

void _subu(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] + r[d.rt()];
}

void _and(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] & r[d.rt()];
}

void _or(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] | r[d.rt()];
}

void _xor(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rs()] ^ r[d.rt()];
}

void _nor(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = ~(r[d.rs()] | r[d.rt()]);
}

void _slt(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rt()] < r[d.rs()];
}

void _sltu(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rd()] = r[d.rt()] < r[d.rs()];
}

void _bltz(Registers& r, Decoder& d, Memory& mem)
{
    if (r[d.rs()] == r[d.rt()])
        r[pc] = d.imm();
}

void _j(Registers& r, Decoder& d, Memory& mem)
{   
    r[pc] = d.imm();
}

void _jal(Registers& r, Decoder& d, Memory& mem)
{
    r[ra] = r[pc] + 4;
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

void _blez(Registers& r, Decoder& d, Memory& mem)
{
    if (r[d.rt()] <= r[zero])
        r[pc] = d.imm();
}

void _bgtz(Registers& r, Decoder& d, Memory& mem)
{
    if (r[d.rt()] > r[zero])
        r[pc] = d.imm();
}

void _addi(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] + d.imm();
}

void _addiu(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] + d.imm();
}

void _slti(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] < d.imm();
}

void _sltiu(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] < d.imm();
}

void _andi(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] & d.imm();
}

void _ori(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] | d.imm();
}

void _xori(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = r[d.rs()] ^ d.imm();
}

void _lui(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = d.imm() << 16;
}

void _lb(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = static_cast<Register>(mem.fetch<Byte>(d.imm()));
}

void _lh(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = mem.fetch<HalfWord>(d.imm());
}

void _lbu(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = static_cast<Register>(mem.fetch<Byte>(d.imm()));
}

void _lhu(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = mem.fetch<HalfWord>(d.imm());
}

void _lw(Registers& r, Decoder& d, Memory& mem)
{
    r[d.rt()] = mem.fetch<Word>(d.imm());
}

void _sb(Registers& r, Decoder& d, Memory& mem)
{
    mem.store<Byte>(static_cast<Byte>(r[d.rt()]), r[d.rs()] + d.imm());
}

void _sh(Registers& r, Decoder& d, Memory& mem)
{
    mem.store<HalfWord>(r[d.rt()], r[d.rs()] + d.imm());
}

void _sw(Registers& r, Decoder& d, Memory& mem)
{
    mem.store<Word>(r[d.rt()], r[d.rs()] + d.imm());
}

