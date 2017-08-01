#ifndef REGISTER_H
#define REGISTER_H

#include "label.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <stdint.h>

typedef int32_t Register;
typedef uint32_t Instruction;
const int MAX_REG = 32;

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
    pc,   ir,   lo,   hi
};

/* Class to simulate Mips registers */
class Registers
{
public:
    Registers();
    void init();
    void init(std::vector<Label>& labels);
    bool get_exit() const;
    Instruction get_ir() const;
    void set_exit(bool e);
    void set_ir(Instruction i) ;
    Register& operator[](int i);
    friend std::ostream& operator<<(std::ostream& o, Registers& r);
private:
    Register _r[MAX_REG];
    Register _pc;
    Register _lo;
    Register _hi;
    Instruction _ir;
    // Exit flag is set to true when the program is terminated.
    bool _exit;
};

inline bool operator==(int i, RegisterName rn)
{
    return static_cast<RegisterName>(i) == rn;
}

#endif