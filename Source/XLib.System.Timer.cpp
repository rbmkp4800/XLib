#include <Windows.h>

#include "XLib.System.Timer.h"

TimerRecord Timer::getRecord()
{
	uint64 record = 0;
	QueryPerformanceCounter(PLARGE_INTEGER(&record));
	return TimerRecord(record);
}

float32 Timer::getElapsedTimeMs(TimerRecord record1, TimerRecord record2)
{
	static float32 frequency = 0.0f;
	if (frequency == 0.0f)
	{
		uint64 frequencyU64 = 0;
		QueryPerformanceFrequency(PLARGE_INTEGER(&frequencyU64));
		frequency = float32(frequencyU64) / 1000.0f;
	}

	return float32(record2 - record1) / frequency;
}