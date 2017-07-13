#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>

const int MAX_REG = 32;
typedef uint32_t Instruction;

int opcode(Instruction i)
{
	return (i & 0xfc000000) >> 26;
}

int rs(Instruction i)
{
	return (i & 0x3e00000) >> 21;
}

int rt(Instruction i)
{
	return (i & 0x1f0000) >> 16;
}

int rd(Instruction i)
{
	return (i & 0xf800) >> 11;
}

int shift(Instruction i)
{
	return (i & 0x7c0) >> 6;
}

int funct(Instruction i)
{
	return i & 0x3f;
}

int imm(Instruction i)
{
	return i & 0xffff;
}

int jaddress(Instruction i)
{
	return i & 0x3ffffff;
}

enum DataType
{
	none,
	word,
	byte,
	ascii
};

enum LabelType
{
	data,
	text
};

struct Data
{
	Data(int v, DataType t)
	  : value(v), type(t)
	{}
	int value;
	DataType type;
};

class Label
{
public:
	Label(const std::string& line, LabelType lt, std::size_t index)
	  : loc(index)
	{
		// Set label.
		std::string::const_iterator it0, it1;
		for (it0 = line.begin(); it0 < line.end(), isspace(*it0); ++it0);
		for (it1 = it0; it1 < line.end(), *it1 != ':'; ++it1);
		name = std::string(it0, it1);
		type = lt;
	}
	int loc;
	LabelType type;
	std::string name;
private:
};

struct Register
{
	int value;
	bool address; // True if the value is an index to mem.
};

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
	else if (s == "t7")
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

// Returns the correct escape character. Assumes c follows a '\'
char get_escape(char c)
{
	assert(c == 'n' || c == 't' || c == '0');
	return c == 'n' ? '\n' : c == 't' ? '\t' : '\0';
}

void ascii_to_mem(const std::string& s, std::vector<Data>& mem_d)
{
	//auto it0, it1;
	auto it0 = std::find( s.begin(),  s.end(), '"');
	for (auto it1 = s.end(); it1 >= s.begin(); --it1)
	{
		if (*it1 == '"')
		{
			for (++it0; it0 < it1; ++it0)
				if (*it0 == '\\')
					mem_d.push_back(Data(static_cast<int>(get_escape(*++it0)), ascii));
				else
					mem_d.push_back(Data(static_cast<int>(*it0), ascii));
			break;
		}
	}
}

void asciiz_to_mem(const std::string& s, std::vector<Data>& mem_d)
{
	ascii_to_mem(s, mem_d);
	mem_d.push_back(Data(static_cast<int>('\0'), ascii));
}

void byte_to_mem(const std::string& s, std::vector<Data>& mem_d)
{
	// Find the first apostrophe.
	auto it = std::find(s.begin(), s.end(), '\'');
	while(it != s.end())
	{
		assert(it + 1 != s.end());
		++it;
		// Handle escape characters
		if (*it == '\\')
			mem_d.push_back(Data(static_cast<int>(get_escape(*++it)), ascii));
		else
			mem_d.push_back(Data(static_cast<int>(*it), ascii));
		it = std::find(it, s.end(), ',');
		// Return for a single character or at the end of the array.
		if (it == s.end())
			return;
		// Find the next apostrophe.
		it = std::find(it, s.end(), '\'');
	}
}

void word_to_mem(const std::string& s, std::vector<Data>& mem_d)
{
	// Don't rule out the possibility there is a number in the lable. First,
	// find the colon at the end of the label. Then iterate and search for 
	// numbers using isdigit()
	std::string integer = "";
	for (auto it = std::find(s.begin(), s.end(), ':'); it != s.end(); ++it)
	{
		if (isdigit(*it))
			integer.push_back(*it);
		else if (integer != "")
		{
			mem_d.push_back(Data(std::stoi(integer), word));
			integer = "";
		}
	}
	if (integer != "")
		mem_d.push_back(Data(std::stoi(integer), word));
}

/*
	void set_data(std::string& s)
	{
		assert(type != type_none);

		if (type == type_int)
		{
			// Count the number of commas to figure out the size of the array.

		}
		if (type == type_str)
		{
			std::size_t start, end;
			start = s.find_first_of('\"') + 1;
			end   = s.find_last_of('\"');
			std::string string_data = s.substr(start, end - start);
			std::cout << string_data << '\n';
			// Assume the first character is your starting " of the string.
			size = string_data.length() - std::count(string_data.begin(), string_data.end(), '\\');

			data = new int[size];
			std::string::const_iterator it;
			int index = 0;
			for (it = string_data.begin(); it < string_data.end(); ++it, ++index)
			{
				if (*it == '\\')
				{
					data[index] = *(it + 1) == 't' ? static_cast<int>('\t') : static_cast<int>('\n');
					++it;
					continue;
				}
				data[index] = static_cast<int>(*it);
				assert(index < size);
			}
		}
	}
	*/

std::string remove_comments(const std::string& line)
{
	std::string::const_iterator it;
	it = find(line.begin(), line.end(), '#');
	if (it != line.end())
		return std::string(line.begin(), it);
	else
		return line;
}

/*
DataType get_type(const std::string& line)
{
	std::size_t begin, end;
	begin = line.find(".");
	end   = line.find(" ", begin) - begin;

	assert(begin > 0 && end > 0);
	assert(begin != std::string::npos && end != std::string::npos);

	std::string type =  line.substr(begin, end);

    return type == ".word"   ? type_int
	     : type == ".asciiz" ? type_str
	     : type == ".byte"   ? type_char
	     : type_none;
}

void get_data(const std::string& line, Data& d)
{
	std::string::const_iterator it = line.begin();
	std::string label;
	DataType type;
	std::string data;

	// Remove any whitespace.
	while (it < line.end() && isspace(*it))
		++it;
	// Get the label of the data.
	while (it < line.end() && *it != ':')
		label.push_back(*it++);
	// Find the beginning of the data type.
	while (it < line.end() && *it != '.')
		++it;
	// Increment once to check the next character.
	++it;

	assert(it != line.end());

	type = *it == 'w' ? type_int
	     : *it == 'b' ? type_char
	     : *it == 'a' ? type_str
	     : type_none;

	// Find the next whitespace.
	while (it < line.end() && !isspace(*it))
		++it;
	// Now that we have found the next whitespace, look for non white space.
	while (it < line.end() && isspace(*it))
		++it;

	// The remainder of the line is our data.
	data = std::string(it, line.end());

	d.set_label(label);
	d.set_type(type);
	d.set_data(data);
}
*/

void init_data(std::vector<Data>& mem_d, std::vector<Label>& labels)
{
	const std::string data_seg = ".data";
	const std::string text_seg = ".text";
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
		if (line.find(data_seg) != std::string::npos)
		{
			std::cout << "found data seg!\n";

			while (std::getline(file, line))
			{	
				line = remove_comments(line);
				// Make sure we don't run into the text segment.
				if (line.find(text_seg) != std::string::npos)
				{
					std::cout << "ran into text seg...\n";
					break;
				}

				// Make sure we skip lines with only whitespace.
				if (line.find_first_not_of("\t\n ") == std::string::npos)
				{
					std::cout << "only whitespace...\n";
					continue;
				}

				labels.push_back(Label(line, data, mem_d.size()));
				if (line.find(".word") != std::string::npos)
					word_to_mem(line, mem_d);
				else if (line.find(".byte") != std::string::npos)
					byte_to_mem(line, mem_d);
				else if (line.find(".asciiz") != std::string::npos)
					asciiz_to_mem(line, mem_d);
				else if (line.find(".ascii") != std::string::npos)
					ascii_to_mem(line, mem_d);
			}
		}
	}
	file.close();
}

int main()
{
	std::vector<Data> mem_d;
	std::vector<int>  mem_i;
	std::vector<Label> labels;
	Register reg[32];
	init_data(mem_d, labels);

	for (auto it = labels.begin(); it != labels.end(); ++it)
	{
		std::cout << it->name << '\n';
		int i = it->loc;
		while (i < mem_d.size() && mem_d[i].type == ascii && static_cast<char>(mem_d[i].value) != '\0')
		{
			std::cout << static_cast<char>(mem_d[i].value);
			++i;
		}
		if (mem_d[i].type == word)
		{
			std::cout << mem_d[i].value;
		}
		else if (mem_d[i].type == byte)
		{
			std::cout << static_cast<char>(mem_d[i].value);
		}
		std::cout << std::endl;
	}

	return 0;
}