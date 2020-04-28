#pragma once
#include <intrin.h>
#include <map>
#include <string>
#include "../core/ECS.h"
#include "../events/Events.h"
// TODO: this only works in a single threaded environment

#define DEBUG

#define MAX_RECORDS 128
#define TOKENPASTE_(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE_(x, y)

#pragma pack(push, 1)
struct debug_record
{
	char* funcname = nullptr;
	int line = 0;
	int hitCount = 0;
	unsigned long long cycles = 0ULL;
	char padding[12];
};
#pragma pack(pop)
extern unsigned int frame_debug_records;
extern debug_record* debug_records;
extern debug_record full_record;

#ifdef DEBUG
#define TIMED_BLOCK debug_perf_counter TOKENPASTE(Counter, __LINE__)(__LINE__, (char*)__FUNCTION__); FCS::SceneManager::GetActiveScene()->emit<Events::OnPCounterInitDebugInfo>(Events::OnPCounterInitDebugInfo())
#else
#define TIMED_BLOCK
#endif

#ifdef DEBUG
#define GLOBAL_DEBUG_BLOCK_INIT global_debug_perf_counters _gdpc;
#else
#define GLOBAL_DEBUG_BLOCK_INIT
#endif
void endRecordFrame();
void initPerfCounterRecords();
void cleanPerfCounterRecords();
void ncc_memcpy(char* dst, char* src, unsigned size);

struct global_debug_perf_counters
{
	global_debug_perf_counters()
	{
		initPerfCounterRecords();
	}

	~global_debug_perf_counters()
	{
		cleanPerfCounterRecords();
	}
};

struct debug_perf_counter
{
	debug_record* record;

	debug_perf_counter(int line, char* function)
	{
		record = debug_records + frame_debug_records++;
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