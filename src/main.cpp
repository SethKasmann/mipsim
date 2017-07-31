#include <iostream>
#include <string>
#include "register.h"
#include "memory.h"
#include "decode.h"
#include "mips.h"

int main()
{
    std::string file;
    mips::intro();

    while (mips::select(file))
    {
        Memory    mem;
        Decoder   dec;
        Registers regs;

        mips::encode(file, mem, regs);

        while (mips::program(regs))
        {
            mips::fetch(mem, regs);
            mips::update(regs);
            mips::decode(regs, dec);
            mips::execute(mem, regs, dec);
        }
    } 

    return 0;
}