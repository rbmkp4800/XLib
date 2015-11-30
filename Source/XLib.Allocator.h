#pragma once

#include "XLib.Types.h"
#include "XLib.Containers.Vector.h"

template <uint32 blockSize>
class Allocator
{
private:
	static constexpr uint32 initialPoolsBufferSize = 4;

	struct Pool
	{
		byte *ptr;
		uint32 size;
		uint32 usedSize;
	};

	Vector<Pool, initialPoolsBufferSize> pools;

public:
	void* allocate();
	void free();
};