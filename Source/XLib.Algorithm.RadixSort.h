#pragma once

#include "XLib.Types.h"

struct RadixSort abstract final
{
	static void Sort(uint32 *data, uintptr size, uint32 *tempBuffer = nullptr);
	static void Sort(uint64 *data, uintptr size, uint64 *tempBuffer = nullptr);
	static void SortByKey()
};