#include "memory.h"

/* A Mips instruction is 4 bytes. */
template<> size_t size_of<Instruction>()
{
	return static_cast<size_t>(4);
}

/* A Mips word is 4 bytes. */
template<> size_t size_of<Word>()
{
	return static_cast<size_t>(4);
}

/* A Mips halfword is 2 bytes. */
template<> size_t size_of<HalfWord>()
{
	return static_cast<size_t>(2);
}

/* A Mips byte is 1 byte. */
template<> size_t size_of<Byte>()
{
	return static_cast<size_t>(1);
}

/* Returns the size of memory vector. */
size_t Memory::size() const
{
	return _mem.size();	
}

/* Sets the stack variable to the first index of the stack and adds the stack
   on to the memory vector. */
void Memory::init_stack()
{
	_stack = _mem.size();
	std::vector<Byte> stack(Stack_size);
	_mem.insert(_mem.end(), stack.begin(), stack.end());
}

/* Checks to see if a fetch or store routine will cross the stack index
   variable or is greater than the size of the memory vector. If so, 
   throw an out_of_range exception. */
template<typename T>
void Memory::probe(int first) const
{
	int last = first + size_of<T>() - 1;
	if ((first <= _stack && last >= _stack) || last >= _mem.size())
		throw std::out_of_range("stack overflow occurred.");
}

/* Explicit instantiation */
template void Memory::probe<Instruction>(int) const;
template void Memory::probe<Word>(int) const;
template void Memory::probe<HalfWord>(int) const;
template void Memory::probe<Byte>(int) const;

/* Fetches and returns a value from the memory vector. */
template<typename T>
inline T Memory::fetch(int index) const
{
	probe<T>(index);
	T ret = static_cast<T>(0);

	for (int i = 0; i < size_of<T>(); ++i)
		ret |= static_cast<T>(_mem[index + i]) << (i * 8);

	return ret;
}

/* Explicit instantiation */
template Instruction Memory::fetch<Instruction>(int) const;
template Word Memory::fetch<Word>(int) const;
template HalfWord Memory::fetch<HalfWord>(int) const;
template Byte Memory::fetch<Byte>(int) const;
 
/* Stores a value to the memory vector. */
template<typename T>
inline void Memory::store(T t, int index)
{
	static const T mask = static_cast<T>(0xff);
	probe<T>(index);
	for (size_t i = 0; i < size_of<T>(); ++i)
	{
		int shift = i * Octet_size;
		_mem[index + i] = static_cast<Byte>((t & mask << shift) >> shift);
	}
}

/* Explicit instantiation */
template void Memory::store<Instruction>(Instruction, int);
template void Memory::store<Word>(Word, int);
template void Memory::store<HalfWord>(HalfWord, int);
template void Memory::store<Byte>(Byte, int);
 
/* Pushes a value to the memory vector, increasing it's size. Only used during
   the encoding routine. */
template<typename T>
inline void Memory::push(T t)
{
	static const T mask = static_cast<T>(0xff);
	for (size_t i = 0; i < size_of<T>(); ++i)
	{
		int shift = i * Octet_size;
		_mem.push_back(static_cast<Byte>((t & mask << shift) >> shift));
	}
}

/* Explicit instantiation */
template void Memory::push<Instruction>(Instruction);
template void Memory::push<Word>(Word);
template void Memory::push<HalfWord>(HalfWord);
template void Memory::push<Byte>(Byte);