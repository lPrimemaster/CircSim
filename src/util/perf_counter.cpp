#include "perf_counter.h"
#include <iostream>

debug_record* debug_records;
debug_record full_record;
unsigned int frame_debug_records = 0;

void ncc_memcpy(char* dst, char* src, unsigned size)
{
	char* dst_end = dst + size;
	while (dst != dst_end) {
		__m128i res = _mm_stream_load_si128((__m128i*)src);
		*((__m128i*)dst) = res;
		src += 16;
		dst += 16;
	}
}

void ppPerfCounterRecords()
{
	for (int i = 0; i < frame_debug_records; i++)
	{
		auto record = debug_records + i;
		printf("(%d) %-30s: %-20llu cy/h | Total: %-20llu cy\n", record->line, record->funcname, record->cycles / (unsigned long long)record->hitCount, full_record.cycles);
	}
	frame_debug_records = 0;
}

void initPerfCounterRecords()
{
	debug_records = (debug_record*)calloc(MAX_RECORDS, sizeof(debug_record));
}

void cleanPerfCounterRecords()
{
	free(debug_records);
}
