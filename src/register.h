#ifndef REGISTER_H
#define REGISTER_H

#include "label.h"
#include <string>

const int MAX_REG = 32;

typedef int32_t Register;

enum RegisterName
{
    zero, at,   v0,   v1,
    a0,   a1,   a2,   a3,
    t0,   t1,   t2,   t3,
    t4,   t5,   t6,   t7,
    s0,   s1,   s2,   s3,
    s4,   s5,   s6,   s7,
    t8,   t9,   k0,   k1,
    gp,   sp,   fp,   ra, 
    pc,   lo,   hi
};

class Registers
{
public:
    Registers() : _pc(0), _ir(0), _lo(0), _hi(0)
    {
        for (int i = 0; i < MAX_REG; ++i)
        {
            _r[i] = 0;
        }
    }
    Registers(std::vector<Label>& labels) : Registers()
    {
        for (auto it = labels.begin(); it != labels.end(); ++it)
        {
            if (it->name == "main")
            {
                _pc = it->loc;
                break;
            }
        }
    }
    int get_pc() const;
    uint32_t get_ir() const;
    int opcode() const;
    int rs() const;
    int rt() const;
    int rd() const;
    int funct() const;
    int imm() const;
    void set_ir(uint32_t i);
    void operator++(int);
    void operator=(int i);
    int& operator[](int i);
    int fetch(int reg) const;
    void write_back(int reg, int val);
private:
    int _r[MAX_REG];
    int _pc;
    int _lo;
    int _hi;
    uint32_t _ir;
};

bool operator==(int i, RegisterName rn)
{
    return static_cast<RegisterName>(i) == rn;
}

/*
int shift(Instruction i)
{
    return (i & 0x7c0) >> 6;
}

int jaddress(Instruction i)
{
    return i & 0x3ffffff;
}
*/

#endif