#pragma once

#include "XLib.Types.h"

using TimerRecord = uint64;

class Timer abstract final
{
public:
	static TimerRecord GetRecord();
	static float32 GetElapsedTimeMs(TimerRecord record1, TimerRecord record2 = Timer::GetRecord());
};