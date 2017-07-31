#include <iostream>
#include <string>
#include <sstream>

#include "label.h"
#include "encode.h"
#include "instruction.h"
#include "decode.h"

namespace mips
{
    /* Print the program intro */
    void intro()
    {
        std::cout << ">> Welcome to Mipsim created by Seth Kasmann\n";
    }

    /* Get the filename from the user. Return false if the user types quit */
    bool select(std::string& filename)
    {
        while (1)
        {
            std::cout << ">> file path: ";
            std::getline(std::cin, filename);

            if (filename == "quit")
                return false;

            std::ifstream file;
            file.open(filename);
            if (!file)
            {
                std::cout << "<< " << filename << " could not be openend.\n";
                continue;
            }
            break;
        }
        return true;
    }

    /* Encode the source code and store in memory. Store the location of
       main: inside the $pc register */
    bool encode(std::string& file, Memory& mem, Registers& regs)
    {
        std::vector<Label> labels;
        init_data(file, mem, labels);
        encode_text_labels(file, mem.size(), labels);
        encode_instructions(file, mem, labels);
        regs.init(labels);
    }

    /* Verify the program is running. */
    bool program(Registers& regs)
    {
        return !regs.get_exit();
    }

    /* Fetch the register from memory $pc is pointing to and store it
       in the $ir register */
    void fetch(Memory& mem, Registers& regs)
    {
        regs.set_ir(mem.fetch<Instruction>(regs[pc]));
    }

    /* Update the $pc register */
    void update(Registers& regs)
    {
        regs[pc] += 4;
    }

    /* Decode the instruction and deduce the function index along with up to
       three operands */
    void decode(Registers& regs, Decoder& dec)
    {
        dec.decode(regs.get_ir());
    }

    /* Index the alu (or function array) to call the correct function */
    void execute(Memory& mem, Registers& regs, Decoder& dec)
    {
        alu[dec.fi()](regs, dec, mem);
    }
}