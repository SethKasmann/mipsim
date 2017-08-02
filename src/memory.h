#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <iostream>
#include <stdexcept>

typedef unsigned char Byte;
typedef uint32_t Instruction;
typedef int32_t Word;
typedef int16_t HalfWord;

/* Stack size of 1MB */
const size_t Stack_size = 1000 * 1000;
const size_t Octet_size = 8;

/* Returns the size in bytes of a native Mips type while avoiding the
   ambiguity of sizeof */
template<typename T> size_t size_of();
template<> size_t size_of<Instruction>();
template<> size_t size_of<Word>();
template<> size_t size_of<HalfWord>();
template<> size_t size_of<Byte>();

class Memory
{
public:
	void init_stack();
	size_t size() const;
	template<typename T> void push(T t);
	template<typename T> void store(T t, int i);
	template<typename T> T fetch(int i) const;
	template<typename T> void probe(int first) const;
private:
	std::vector<Byte> _mem;
	int _stack;
};

#endif