#ifndef ENCODE_H
#define ENCODE_H

#include <string>
#include <fstream>
#include <unordered_map>
#include "memory.h"
#include "label.h"
#include "misc.h"
#include "instruction.h"

const std::string Data_seg = ".data";
const std::string Text_seg = ".text";
const std::string Whitespace = " \t\n";

/* Return an integer representation of a string register name */
int reg_to_int(const std::string& s)
{
    if (s == "zero")
        return 0;
    else if (s == "at")
        return 1;
    else if (s == "v0")
        return 2;
    else if (s == "v1")
        return 3;
    else if (s == "a0")
        return 4;
    else if (s == "a1")
        return 5;
    else if (s == "a2")
        return 6;
    else if (s == "a3")
        return 7;
    else if (s == "t0")
        return 8;
    else if (s == "t1")
        return 9;
    else if (s == "t2")
        return 10;
    else if (s == "t3")
        return 11;
    else if (s == "t4")
        return 12;
    else if (s == "t5")
        return 13;
    else if (s == "t6")
        return 14;
    else if (s == "t7")
        return 15;
    else if (s == "s0")
        return 16;
    else if (s == "s1")
        return 17;
    else if (s == "s2")
        return 18;
    else if (s == "s3")
        return 19;
    else if (s == "s4")
        return 20;
    else if (s == "s5")
        return 21;
    else if (s == "s6")
        return 22;
    else if (s == "s7")
        return 23;
    else if (s == "t8")
        return 24;
    else if (s == "t9")
        return 25;
    else if (s == "k0")
        return 26;
    else if (s == "k1")
        return 27;
    else if (s == "gp")
        return 28;
    else if (s == "sp")
        return 29;
    else if (s == "fp")
        return 30;
    else if (s == "ra")
        return 31;
    else if (s == "lo")
        return lo;
    else if (s == "hi")
        return hi;
    // Handle register number;
    else
        assert(std::stoi(s) >= 0 && std::stoi(s) <= 31);
    return std::stoi(s);
}

/* Store a line of .ascii from the data segment in memory */
void ascii_to_mem(const std::string& s, Memory& mem)
{
    //auto it0, it1;
    auto it0 = std::find(s.begin(), s.end(), '"');
    for (auto it1 = s.end(); it1 >= s.begin(); --it1)
    {
        if (*it1 == '"')
        {
            for (++it0; it0 < it1; ++it0)
                if (*it0 == '\\')
                    mem.push<Byte>(get_escape(*++it0));
                else
                    mem.push<Byte>(*it0);
            break;
        }
    }
}

/* Store a line of .asciiz from the data segment in memory */
void asciiz_to_mem(const std::string& s, Memory& mem)
{
    ascii_to_mem(s, mem);
    mem.push<Byte>('\0');
}

/* Store a line of .byte from the data segment in memory */
void byte_to_mem(const std::string& s, Memory& mem)
{
    // Find the first apostrophe.
    auto it = std::find(s.begin(), s.end(), '\'');
    while (it != s.end())
    {
        assert(it + 1 != s.end());
        ++it;
        // Handle escape characters
        if (*it == '\\')
            mem.push<Byte>(get_escape(*++it));
        else
            mem.push<Byte>(*it);
        it = std::find(it, s.end(), ',');
        // Return for a single character or at the end of the array.
        if (it == s.end())
            return;
        // Find the next apostrophe.
        it = std::find(it, s.end(), '\'');
    }
}

/* Store a line of .word from the data segment in memory */
void word_to_mem(const std::string& s, Memory& mem)
{
    // Don't rule out the possibility there is a number in the lable. First,
    // find the colon at the end of the label. Then iterate and search for 
    // numbers using isdigit()
    std::string word = "";
    for (auto it = std::find(s.begin(), s.end(), ':'); it != s.end(); ++it)
    {
        if (isdigit(*it))
            word.push_back(*it);
        else if (word != "")
        {
            mem.push<Word>(std::stoi(word));
            word = "";
        }
    }
    if (word != "")
        mem.push<Word>(std::stoi(word));
}


/* Check to see if a line contains a label */
bool contains_label(const std::string& line)
{
	auto it = std::find(line.begin(), line.end(), ':');
	return it != line.end();
}

/* Converts a line of MIPS source code in the data segment and pushes
   the data to the memory vector */
void init_data(std::string& filename, Memory& mem, std::vector<Label>& labels)
{
    std::ifstream file;
    std::string line;
    file.open(filename);
    if (!file)
    {
        std::cout << "File could not be opened.\n";
        return;
    }
    while (std::getline(file, line))
    {
        remove_comments(line);
        if (line.find(Data_seg) != std::string::npos)
        {

            while (std::getline(file, line))
            {
                remove_comments(line);
                // Make sure we don't run into the text segment.
                if (line.find(Text_seg) != std::string::npos)
                    break;

                // Make sure we skip lines with only whitespace.
                if (is_whitespace(line))
                    continue;

                labels.push_back(Label(line, mem.size()));
                if (line.find(".word") != std::string::npos)
                    word_to_mem(line, mem);
                else if (line.find(".byte") != std::string::npos)
                    byte_to_mem(line, mem);
                else if (line.find(".asciiz") != std::string::npos)
                    asciiz_to_mem(line, mem);
                else if (line.find(".ascii") != std::string::npos)
                    ascii_to_mem(line, mem);
            }
        }
    }
    file.close();
}

/* Converts a line of MIPS source into a vector of string tokens */
std::vector<std::string> tokenize(std::string& line)
{
    std::vector<std::string> tokens;
    std::string token;

    // Replace parantheses with spaces. This will make tokenizing offsets
    // easier.
    std::replace(line.begin(), line.end(), '(', ' ');
    std::replace(line.begin(), line.end(), ')', ' ');

    // Create an input stream of the line.
    std::istringstream is0(line);

    while (is0 >> std::skipws >> token)
    {
        // Return if a comment or accessibility command is found.
        if (*token.begin() == '.' || *token.begin() == '#')
            return tokens;

        // Create an input stream of the token. We can parse the token by
        // commas using std::getline.
        std::istringstream is1(token);

        // Once parsed, push the token to our vector.
        while (std::getline(is1, token, ','))
            tokens.push_back(token);
    }
    return tokens;
}

/* Finds labels in the text segment and computes their memory location */
void encode_text_labels(std::string& filename, size_t address, std::vector<Label>& labels)
{
    std::ifstream file;
    std::string line, token;
    file.open(filename);
    if (!file)
    {
        std::cout << "File could not be opened.\n";
        return;
    }

    // Search for the text segment.
    while (std::getline(file, line))
    {
        if (line.find(Text_seg) != std::string::npos)
            break;
    }

    while (std::getline(file, line))
    {
        // Break if we find the data segment.
        if (line.find(Data_seg) != std::string::npos)
            break;

        // Tokenize the source code. Text labels will be the first tokens in
        // the vector.
        std::vector<std::string> tokens = tokenize(line);

        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            // If a label is found, push it to the labels vector along wth
            // the address.
            if (it->find(':') != std::string::npos)
            {
                labels.push_back(Label(*it, address));
            }
            // The address is only incremented if a label is not found and
            // another token is present. This would occur if the instruction
            // was written on the same line as the label.
            else
            {
                address += 4;
                break;
            }
        }
    }
}   

/* Returns true if the instruction is a r-type instruction */
bool is_r_type(Instruction i)
{
    return i & 0x31;
}

/* Converts a vector of string tokens to a MIPS instruction */
Instruction encode(std::vector<std::string> tokens, std::vector<Label>& labels)
{
    Instruction ret = Null_instruction;

    // Return the null instruction (0) if the tokens vector is empty.
    if (tokens.empty())
        return ret;

    // Create an iterator and skip text labels.
    std::vector<std::string>::const_iterator it = tokens.begin();
    while (it->find(':') != std::string::npos)
    {
        ++it;
        if (it == tokens.end())
            return ret;
    }

    // OR the opcode.
    assert(it != tokens.end() && Map_str_to_type.find(*it) != Map_str_to_type.end());
    ret |= Map_str_to_type.at(*(it++));

    // OR the remaining operands. We need to handle 3 cases:
    // 1. Register value
    // 2. Immediate/Offset value
    // 3. Shifts
    int reg_shift = is_r_type(ret) ? 11 : 16;
    int imm_shift = is_r_type(ret) ?  6 :  0;
    for (it; it != tokens.end(); ++it)
    {
        // Handle registers.
        if (*it->begin() == '$')
        {
            ret |= reg_to_int(std::string(it->begin() + 1, it->end())) << reg_shift;
            reg_shift += 5;
        }
        // Handle immediate values.
        else if (isdigit(*it->begin()) || *it->begin() == '-')
        {
            ret |= static_cast<Instruction>(std::stoi(*it) & 0xffff) << imm_shift;
        }
        // Handle lables.
        else
        {
            for (auto label = labels.begin(); label != labels.end(); ++label)
            {
                if (label->name == *it)
                {
                    ret |= label->loc;
                }
            }
        }
    }
    return ret;
}

/* Converts a line of MIPS source code in the text segment to a
   MIPS instruction and pushes it to the memory vector */
void encode_instructions(std::string& filename, Memory& mem, std::vector<Label>& labels)
{
    std::ifstream file;
    std::string line;
    file.open(filename);
    if (!file)
    {
        std::cout << "File could not be opened.\n";
        return;
    }

    // Search for the text segment.
    while (std::getline(file, line))
    {
        if (line.find(Text_seg) != std::string::npos)
            break;
    }

    while (std::getline(file, line))
    {
        // Break if we find the data segment.
        if (line.find(Data_seg) != std::string::npos)
            break;

        // Encode the instruction.
        Instruction i = encode(tokenize(line), labels);

        if (i != Null_instruction)
            mem.push<Instruction>(i);
    }
    file.close();
}

#endif