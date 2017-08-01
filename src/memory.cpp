#include "memory.h"

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

size_t Memory::size() const
{
	return _mem.size();	
}

void Memory::init_stack()
{
	_stack = _mem.size();
	std::vector<Byte> stack(Stack_size);
	_mem.insert(_mem.end(), stack.begin(), stack.end());
}