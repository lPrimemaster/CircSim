#include "perf_counter.h"
#include <iostream>

#ifdef  DEBUG
std::map<std::string, debug_record> debug_records;
debug_record full_record;
#endif //  DEBUG

void ppPerfCounterRecords()
{
	for (auto record : debug_records)
	{
		auto rec = record.second;
		printf("(%d) %-30s: %-20llu cy/h | Total: %-20llu cy\n", rec.line, rec.funcname, rec.cycles / (unsigned long long)rec.hitCount, full_record.cycles);
	}
}
