#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <map>
#include "instruction.h"
#include "misc.h"
#include "register.h"
#include "label.h"
#include "memory.h"
#include "decode.h"
#include "encode.h"

int main()
{
	Alu alu[128] = { _null };
	alu[12] = _syscall;
	alu[16] = _mfhi;
	alu[27] = _div;
	alu[32] = _add;
	alu[36] = _and;
	alu[65] = _bltz;
	alu[66] = _j;
	alu[68] = _beq;
	alu[69] = _bne;
	alu[72] = _addi;
	alu[96] = _lb;
	alu[104] = _sb;
    Memory mem;
    Decoder d;
    std::vector<Label> labels;
    init_data(mem, labels);
    init_text_labels(mem, labels);
    init_instructions(mem, labels);
    Registers regs(labels);

    while (!regs.get_exit())
    {
    	regs.set_ir(mem.fetch<Instruction>(regs[pc]));
    	regs[pc] += 4;
    	d.decode(regs.get_ir());
    	alu[d.fi()](regs, d, mem);
    	//std::cout << regs;
    	//int z;
    	//std::cin >> z;
    }

    return 0;
}