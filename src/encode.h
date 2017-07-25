#ifndef ENCODE_H
#define ENCODE_H

#include <string>
#include <fstream>
#include "memory.h"
#include "label.h"
#include "misc.h"

const std::string Data_seg = ".data";
const std::string Text_seg = ".text";

int reg_to_int(std::string& s)
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
    // Handle register number;
    else
        assert(std::stoi(s) >= 0 && std::stoi(s) <= 31);
    return std::stoi(s);
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
                    mem.push<Byte>(get_escape(*++it0));
                else
                    mem.push<Byte>(*it0);
            break;
        }
    }
}

void asciiz_to_mem(const std::string& s, Memory& mem)
{
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
        line = remove_comments(line);
        if (line.find(Data_seg) != std::string::npos)
        {

            while (std::getline(file, line))
            {
                line = remove_comments(line);
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

void get_op(std::string::const_iterator& it, const std::string& line, std::string& op)
{
	if (contains_label(line))
		it = std::find(line.begin(), line.end(), ':');

	// Increment as long as we're on a colon or space.
	while (it != line.end() && (*it == ':' || isspace(*it)))
		++it;

	// Iterate the function name until we reach a space.
	while (it != line.end() && !isspace(*it))
	{
		op.push_back(*it);
		++it;
	}
	
	return op;
}


size_t pseudo_instruction(std::string& line)
{

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
        line = remove_comments(line);
        if (line.find(Text_seg) != std::string::npos)
        {
            loc = mem.size();
            while (std::getline(file, line))
            {
                line = remove_comments(line);
                // Make sure we don't run into the data segment.
                if (line.find(Data_seg) != std::string::npos)
                    break;

                // Make sure we skip lines with only whitespace. Also skip lines
                // such as '.global main' (e.g. lines that begin with '.')
                if (is_whitespace(line) || first_char(line) == '.')
                    continue;

                // Look for a text label.
                auto it = std::find(line.begin(), line.end(), ':');
                if (it != line.end())
                    labels.push_back(Label(line, loc));

                if (is_pseudo(line))
                    loc += 8;
                else
                    loc += 4;
            }
            break;
        }
    }
    file.close();
}

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
        line = remove_comments(line);
        if (line.find(Text_seg) != std::string::npos)
        {
            while (std::getline(file, line))
            {
                //std::cout << line << " " << mem.size() << '\n';
                line = remove_comments(line);
                // Make sure we don't run into the data segment.
                if (line.find(Data_seg) != std::string::npos)
                    break;

                // Make sure we skip lines with only whitespace.
                if (is_whitespace(line) || first_char(line) == '.')
                    continue;

                // If a label is found, increment the iterator 1 past the label.
                // If no label is found, start at the beginning of the line.
                auto it = std::find(line.begin(), line.end(), ':');
                if (it != line.end())
                    ++it;
                else
                    it = line.begin();

                Instruction i = Null_instruction;
                std::string name = "";

                // 1. Scan until we do not find a whitespace.
                while (it != line.end() && isspace(*it))
                    ++it;

                // 2. Extract the instruction name.
                while (it != line.end() && !isspace(*it) && *it != '$')
                {
                    name.push_back(*it);
                    ++it;
                }

                // 3. Or the opt/funct code to the instruction.
                if (name != "")
                {
                    i |= Map_str_to_type.at(name);
                }

                // 4. Scan until we do not find a whitespace.
                while (it != line.end() && isspace(*it))
                    ++it;

                // 5. Check for rs register.
                name = "";
                if (it != line.end() && *it == '$')
                {
                    ++it;
                    while (it != line.end() && !isspace(*it) && *it != ',')
                    {
                        name.push_back(*it);
                        ++it;
                    }
                    i |= set_rs(reg_to_int(name));
                }

                // 6. Scan until we do not find a whitespace or comma.
                while (it != line.end() && (isspace(*it) || *it == ','))
                    ++it;

                // 7. Check for rt register.
                name = "";
                if (it != line.end() && *it == '$')
                {
                    ++it;
                    while (it != line.end() && !isspace(*it) && *it != ',')
                    {
                        name.push_back(*it);
                        ++it;
                    }
                    i |= set_rt(reg_to_int(name));
                }

                // 8. Scan until we do not find a whitespace or comma.
                while (it != line.end() && (isspace(*it) || *it == ','))
                    ++it;

                // 9. Check for rd register.
                name = "";
                if (it != line.end() && *it == '$')
                {
                    ++it;
                    while (it != line.end() && !isspace(*it) && *it != ',')
                    {
                        name.push_back(*it);
                        ++it;
                    }
                    i |= set_rd(reg_to_int(name));
                    // Push back instruction and continue.
                    mem.push<Instruction>(i);
                    continue;
                }
                // 10. Convert any R-Type to I-Type if R-Type was used as a 
                // pseudo instruction.
                if ((i & 0x3f) == Add)
                {
                    i |= Add << 26;
                }

                // 10. Check for immediate value.
                name = "";
                // Start with a label name.
                if (it != line.end() && isalpha(*it))
                {
                    while (it != line.end() && !isspace(*it))
                    {
                        name.push_back(*it);
                        ++it;
                    }
                    for (auto lab = labels.begin(); lab != labels.end(); ++lab)
                    {
                        if (lab->name == name)
                        {
                            i |= lab->loc;
                        }
                    }
                    mem.push<Instruction>(i);
                }
                else if (it != line.end() && isdigit(*it))
                {
                    bool offset = false;
                    while (it != line.end() && !isspace(*it))
                    {
                        if (*it == '(')
                        {
                            offset = true;
                            break;
                        }
                        name.push_back(*it);
                        ++it;
                    }
                    // Handle Pseudo Instructions. Here, we have a immediate value,
                    // followed by a label. We need to insert an instruction to store
                    // the immiate value into the register $at (position 1).
                    if (is_pseudo(line))
                    {
                        Instruction pi = Null_instruction;
                        pi |= Li;
                        pi |= set_rs(1);
                        pi |= std::stoi(name);
                        mem.push<Instruction>(pi);

                        while (it != line.end() && (isdigit(*it) || isspace(*it)))
                            ++it;

                        if (it == line.end() || !isalpha(*it))
                            std::cout << "Error: Pseudo Instruction found, but no second label found.\n";

                        name = "";
                        while (it != line.end() && !isspace(*it))
                        {
                            name.push_back(*it);
                            ++it;
                        }

                        std::cout << "looking for pseudo:" << name << '\n';

                        for (auto lab = labels.begin(); lab != labels.end(); ++lab)
                        {
                            if (lab->name == name)
                            {
                                i |= lab->loc;
                            }
                        }
                        i |= set_rt(1);
                        mem.push<Instruction>(i);
                        continue;
                    }
                    else
                    {
                        i |= std::stoi(name);
                    }

                    // Handle offsets.
                    if (offset)
                    {
                        name = "";  
                        it += 2;
                        while (it != line.end() && *it != ')')
                        {
                            name += *it;
                            ++it;
                        }
                        i |= set_rt(reg_to_int(name));
                    }
                    mem.push<Instruction>(i);
                }
                else
                {
                    mem.push<Instruction>(i);
                }

            }
        }
    }
    file.close();
}

#endif