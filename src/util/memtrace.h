#pragma once
#include <iostream>
//FIX operator new track
//#define ENABLE_MEMORY_TRACK

#ifdef ENABLE_MEMORY_TRACK
//#undef new

//void* operator new(size_t size, char* file, int line, char* function);
void* operator new(size_t size);

#define NEW new(__FILE__, __LINE__, __FUNCTION__)
#else
//#define NEW new (???)
#endif