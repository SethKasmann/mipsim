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

int reg_to_int(const std::string& s);
void ascii_to_mem(const std::string& s, Memory& mem);
void asciiz_to_mem(const std::string& s, Memory& mem);
void byte_to_mem(const std::string& s, Memory& mem);
void word_to_mem(const std::string& s, Memory& mem);
bool contains_label(const std::string& line);
void init_data(std::string& filename, Memory& mem, std::vector<Label>& labels);
std::vector<std::string> tokenize(std::string& line);
void encode_text_labels(std::string& filename, size_t address, std::vector<Label>& labels);
bool is_i_type(Instruction i);
Instruction encode(std::vector<std::string> tokens, std::vector<Label>& labels);
void encode_instructions(std::string& filename, Memory& mem, std::vector<Label>& labels);

#endif