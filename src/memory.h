#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <iostream>

typedef unsigned char Byte;
typedef uint32_t Instruction;
typedef int32_t Word;
typedef int16_t HalfWord;

class Memory
{
public:
	template<typename T> void push(T t);
	template<typename T> void store(T t, int i);
	template<typename T> T fetch(int i);
	size_t size() const
	{
		return _mem.size();	
	}
	bool probe(int i) const;
private:
	std::vector<Byte> _mem;
};

bool Memory::probe(int i) const
{
	int bytes = 3;
	for (auto it = _mem.begin() + i; it != _mem.end(); ++it, --bytes)
	{
		if (bytes == 0)
			return true;
	}
	return false;
}

template<typename T>
T Memory::fetch(int i)
{
	if (probe(i))
	{
		return  static_cast<T>(_mem[i    ])
		     | (static_cast<T>(_mem[i + 1]) << 8 )
		     | (static_cast<T>(_mem[i + 2]) << 16)
		     | (static_cast<T>(_mem[i + 3]) << 24);
	}
	else
	{
		std::cout << "Error: Not enough memory avaliable to fetch." << std::endl;
	}
}

template<>
Byte Memory::fetch<Byte>(int i)
{
	return _mem[i];
}

template<typename T>
void Memory::store(T t, int i)
{
	if (probe(i))
	{
		_mem[i    ] = static_cast<Byte>(t & 0x000000ff        );
		_mem[i + 1] = static_cast<Byte>((t & 0x0000ff00) >> 8 );
		_mem[i + 2] = static_cast<Byte>((t & 0x00ff0000) >> 16);
		_mem[i + 3] = static_cast<Byte>((t & 0xff000000) >> 24);
	}
	else
	{
		std::cout << "Error: Not enough memory avaliable to store." << std::endl;
	}
}

template<>
void Memory::store<Byte>(Byte b, int i)
{
	_mem[i] = b;
}

template<typename T>
void Memory::push(T t)
{
	_mem.push_back(static_cast<Byte>(t & 0x000000ff      ));
	_mem.push_back(static_cast<Byte>((t & 0x0000ff00) >> 8 ));
	_mem.push_back(static_cast<Byte>((t & 0x00ff0000) >> 16));
	_mem.push_back(static_cast<Byte>((t & 0xff000000) >> 24));
}

template<>
void Memory::push<Byte>(Byte b)
{
	_mem.push_back(b);
}


#endif