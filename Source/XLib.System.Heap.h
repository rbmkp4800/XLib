#pragma once

#include "XLib.Types.h"

class Heap abstract final
{
public:
	static void* Allocate(uintptr size);
	static void* Reallocate(void* block, uintptr size);
	static bool ReallocateInPlace(void* block, uintptr size);
	static void Free(void* block);
	static uintptr GetBlockSize(void* block);

	template <typename Type>
	static inline Type* Allocate(uintptr count = 1)
	{
		return (Type*)Allocate(count * sizeof(Type));
	}
	template <typename Type>
	static inline Type* Reallocate(Type* block, uintptr count)
	{
		return (Type*)Reallocate((void*)block, count * sizeof(Type));
	}
};