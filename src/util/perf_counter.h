#pragma once
#include <intrin.h>
#include <map>
#include <string>
// TODO: this only works in a single threaded environment

#define DEBUG

#define TOKENPASTE_(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE_(x, y)

#ifdef DEBUG
#define MAX_COUNTERS 128
struct debug_record
{
	char* funcname = nullptr;
	int line = 0;

	int hitCount = 0;
	unsigned long long cycles = 0ULL;
};
extern std::map<std::string, debug_record> debug_records;
extern debug_record full_record;
#define TIMED_BLOCK debug_perf_counter TOKENPASTE(Counter, __LINE__)(__LINE__, (char*)__FUNCTION__)
#else
#define TIMED_BLOCK
#endif
void ppPerfCounterRecords();

struct debug_perf_counter
{
	debug_record* record = nullptr;

	debug_perf_counter(int line, char* function)
	{
		/*_mm_load_si128();
		_mm_stream_si128();*/
		std::string s = std::string(function) + "_" + std::to_string(line);
		record = &debug_records[s];
		record->funcname = function;
		record->line = line;
		record->cycles -= __rdtsc();
		++record->hitCount;
	}

	~debug_perf_counter()
	{
		record->cycles += __rdtsc();
	}
};