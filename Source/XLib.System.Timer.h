#pragma once

#include "XLib.Types.h"

using TimerRecord = uint64;

class Timer abstract final
{
public:
	static TimerRecord getRecord();
	static float32 getElapsedTimeMs(TimerRecord record1, TimerRecord record2 = Timer::getRecord());
};