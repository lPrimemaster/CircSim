#include "memtrace.h"

#ifdef ENABLE_MEMORY_TRACK
//void* operator new(size_t size, char* file, int line, char* function)
//{
//	std::cerr << "Alocation -> " << file << ":" << line << " - " << function << " [" << size << "]" << std::endl;
//	return malloc(size);
//}

void* operator new(size_t size) throw(std::bad_alloc)
{
	if (size != 0)
	{
		void* pointer = malloc(size);
		if (pointer != nullptr)
			return pointer;
		else
			return nullptr;
	}
	else
	{
		throw std::bad_alloc();
	}
	std::cerr << "Alocation -> " << size << std::endl;
}

#endif