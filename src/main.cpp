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
    Memory mem;
    std::vector<Label> labels;
    init_data(mem, labels);
    init_text_labels(mem, labels);
    init_instructions(mem, labels);
    Registers regs(labels);

    return 0;
}