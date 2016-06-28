#pragma once

#include "XLib.Types.h"
#include "XLib.Containers.Vector.h"

enum class PoolAllocatorPolicy
{
	HighFragmentation,	// remove O(1)
	LowFragmentation,	// remove O(logN), N - internal free blocks count
};

template <uint32 blockSize, PoolAllocatorPolicy policy>
class PoolAllocator;

template <uint32 blockSize>
class PoolAllocator<blockSize, PoolAllocatorPolicy::HighFragmentation>
{
private:

public:
	void* allocate();
	void free();
};