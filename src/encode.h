#ifndef ENCODE_H
#define ENCODE_H

#include <string>
#include <fstream>
#include <unordered_map>
#include "memory.h"
#include "label.h"
#include "misc.h"

const std::string Data_seg = ".data";
const std::string Text_seg = ".text";
const std::string Whitespace = " \t\n";

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

void set_opcode(Instruction& i, const std::string& op)
{
    assert(op != "");
    std::cout << "setting... " << op << '\n';
    i |= Map_str_to_type.at(op);
}

void set_shift(Instruction& i, const std::string& num)
{
    assert(std::stoi(num) > 0);
    i |= (std::stoi(num) & 0x1f) << 6;
}

void set_imm(Instruction& i, const std::string& num)
{
    i |= static_cast<Instruction>(std::stoi(num) & 0xffff);
}

void set_rs(Instruction& i, const std::string& reg)
{
    std::cout << "\t\trs " << reg_to_int(reg) << '\n';
    i |= reg_to_int(reg) << 21;
}

void set_rt(Instruction& i, const std::string& reg)
{
    std::cout << "\t\trt " << reg_to_int(reg) << '\n';
    i |= reg_to_int(reg) << 16;
}

void set_rd(Instruction& i, const std::string& reg)
{
    std::cout << "\t\trd " << reg_to_int(reg) << '\n';
    i |= reg_to_int(reg) << 11;
}

void set_reg(Instruction& i, const std::string& reg)
{
    static const int rd = 0x1f << 11;
    static const int rt = 0x1f << 16;
    static const int rs = 0x1f << 21;
    // R Type
    if (i & 0x31)
    {
        if (i & rt)
        {
            set_rs(i, reg);
        }
        else if (i & rd)
            set_rt(i, reg);
        else
            set_rd(i, reg);
    }
    // I Type
    else
    {
        if (i & rt)
            set_rs(i, reg);
        else
            set_rt(i, reg);
    }
}

void set_label(Instruction& i, const std::string& lab, std::vector<Label>& labels)
{
    for (auto it = labels.begin(); it != labels.end(); ++it)
    {
        if (it->name == lab)
        {
            std::cout << "\tlab " << it->name << ' ' << it->loc << '\n';
            i |= it->loc;
            return;
        }
    }
    std::cout << "Error: label \"" << lab << "\" not found in labels vector\n";
}

void set_token(Instruction& i, std::string::const_iterator& it, const std::string& line, std::vector<Label>& labels)
{
    // Immediate value.
    if (*it == '-' || isdigit(*it))
    {
        std::string num = "";
        while (it != line.end() && !isspace(*it) && *it != ',' && *it != '(')
        {
            num.push_back(*it);
            ++it;
        }
        // For an R-Type instructions an immediate value can only be a
        // 5 bit shift. For all others, set as an immediate value.
        //std::cout << "\tPushing imm: " << num << '\n';
        if (i & 0x31)
            set_shift(i, num);
        else
            set_imm(i, num);
    }
    // Register
    else if (*it == '$')
    {
        std::string reg = "";
        ++it;
        while (it != line.end() && !isspace(*it) && *it != ',' && *it != ')')
        {
            reg.push_back(*it);
            ++it;
        }
        //std::cout << "\tPushing reg: " << reg << '\n';
        set_reg(i, reg);
    }
    // Label
    else
    {
        std::string lab = "";
        while (it != line.end() && !isspace(*it))
        {
            lab.push_back(*it);
            ++it;
        }
        //std::cout << "\tPushing lab: " << lab << '\n';
        set_label(i, lab, labels);
    }
}

bool next_token(std::string::const_iterator& it, std::string& line)
{
    while (it != line.end() && (isspace(*it) || *it == ',' || *it == ')' || *it == '('))
        ++it;
    return it != line.end();
}

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
                {
                    if (get_escape(int(*(it0 + 1))) == 0)
                    {
                        std::cout <<" wait a second...\n";
                    }
                    mem.push<Byte>(get_escape(*++it0));
                }
                else
                {
                    mem.push<Byte>(*it0);
                    if (int(*it0) == 0)
                    {
                        std::cout << "0 found.. waiiiit...\n";
                    }
                }
            break;
        }
    }
}

void asciiz_to_mem(const std::string& s, Memory& mem)
{
    std::cout << "adding to mem...:" << s << '\n';
    ascii_to_mem(s, mem);
    mem.push<Byte>('\0');
}

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


// Check if a line contains a label.
bool contains_label(const std::string& line)
{
	auto it = std::find(line.begin(), line.end(), ':');
	return it != line.end();
}

void init_data(Memory& mem, std::vector<Label>& labels)
{
    std::ifstream file;
    std::string line;
    file.open("toread/tictactoe.s");
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

// Returns the opcode as a string. If no opcode is found, return the empty
// string.
std::string get_op(std::string::const_iterator& it, const std::string& line)
{
    std::string::const_iterator start;

    if (contains_label(line))
    {
        start = std::find(line.begin(), line.end(), ':');
        safe_increment<std::string::const_iterator, std::string>(start, line, 1);
    }
    else
        start = line.begin();

    // Increment as long as we're on a colon or space.
    while (start != line.end() && isspace(*start))
        ++start;

    if (start == line.end())
        return std::string("");

    it = std::find_first_of(start, line.end(), Whitespace.begin(), Whitespace.end());

    return std::string(start, it);
}

// Check if a line contains a pseudo instruction. Pseudo instructions are
// instructions not native to mips. Pseudo instructions are converted
// into one or more native instructions. Return 0 if the line does not
// contain a pseudo instruction. Otherwise, return the number of native
// instructions needed to represent the pseudo instruction.
size_t instruction_size(std::string& line)
{
    std::string op;
    std::string::const_iterator it = line.begin();

    // Find the opcode.
    op = get_op(it, line);

    // Return 0 if no opcode was found.
    if (op.empty())
        return 0;

    //std::cout << "Found OP: " << op;

    // Return 2 for pseudo instructions that convert into 2 native instructions.
    if (op == "la"  || op == "bge" || op == "bgt"
     || op == "ble" || op == "blt" || op == "ble")
    {
        //std::cout << " Adding 2\n";
        return 2;
    }
    else
    {
        //std::cout << " Adding 1\n";
        return 1;
    }
}

void init_text_labels(Memory& mem, std::vector<Label>& labels)
{
    std::ifstream file;
    std::string line, name;
    int loc;
    file.open("toread/tictactoe.s");
    if (!file)
    {
        std::cout << "File could not be opened.\n";
        return;
    }
    while (std::getline(file, line))
    {
        remove_comments(line);
        if (line.find(Text_seg) != std::string::npos)
        {
            loc = mem.size();
            while (std::getline(file, line))
            {
                remove_comments(line);
                // Make sure we don't run into the data segment.
                if (line.find(Data_seg) != std::string::npos)
                    break;

                // Make sure we skip lines with only whitespace. Also skip lines
                // such as '.global main' (e.g. lines that begin with '.')
                if (is_whitespace(line) || first_char(line) == '.')
                    continue;

                // Look for a text label.
                if (contains_label(line))
                    labels.push_back(Label(line, loc));

                std::string::const_iterator it = line.begin();
                if (get_op(it, line).empty())
                    continue;

                loc += 4;//instruction_size(line) * 4;
            }
            break;
        }
    }
    file.close();
}

bool is_pseudo(const std::string& op)
{
    return Map_str_to_type.find(op) == Map_str_to_type.end();
}

/*
void encode_pseudo(std::string::const_iterator& it, 
                   const std::string& name,
                   const std::string& line,
                   Memory& mem, std::vector<Label>& labels)
{
    Instruction i Null_instruction;

    while (next_token(it, line))
        set_token(i, it, line, labels);

    Instruction j = i;

    if (name == "bge")
    {
        set_opcode(j, "slt");
        set_opcode(i, "beq");
        set_rd(j, "at");
        set_rt(i, "at");
        set_rs(i, "zero");
    }
    if (name == "bgt")
    {
        set_opcode(j, "slt");
        set_opcode(i, "bne");
        set_rd(j, "at");
        set_rt(i, "at");
        set_rs(i, "zero");
    }
}
*/

void init_instructions(Memory& mem, std::vector<Label>& labels)
{
    std::ifstream file;
    std::string line, name;
    file.open("toread/tictactoe.s");
    if (!file)
    {
        std::cout << "File could not be opened.\n";
        return;
    }
    while (std::getline(file, line))
    {
        if (line.find(Text_seg) != std::string::npos)
        {
            while (std::getline(file, line))
            {
                remove_comments(line);
                // Make sure we don't run into the data segment.
                if (line.find(Data_seg) != std::string::npos)
                    break;

                // Make sure we skip lines with only whitespace.
                if (is_whitespace(line) || first_char(line) == '.')
                    continue;

                std::string::const_iterator it;

                // If a label is found, increment the iterator 1 past the label.
                // If no label is found, start at the beginning of the line.
                it = std::find(line.begin(), line.end(), ':');
                if (it == line.end())
                    it = line.begin();
                else
                {
                    // Check the special case of lines with only a label.
                    if (it + 1 == line.end())
                        continue;
                    else
                        ++it;
                }
                //std::cout << "Line: " << line << '\n';

                Instruction i = Null_instruction;
                name = get_op(it, line);

                // Check for a pseudo instruction
                if (is_pseudo(name))
                {
                    std::cout << "PSEUDO!!! : " << name << '\n';
                    continue;
                }

                set_opcode(i, name);

                while (next_token(it, line))
                    set_token(i, it, line, labels);

                mem.push<Instruction>(i);
            }
        }
    }
    file.close();
}

#endif