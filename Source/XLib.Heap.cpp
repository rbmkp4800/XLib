#include <Windows.h>

#include "XLib.Heap.h"

void* XLib_Internal::Heap::Allocate(uintptr size)
{
	void *ptr = HeapAlloc(GetProcessHeap(), 0, size);
	if (!ptr)
		throw;
	return ptr;
}
void* XLib_Internal::Heap::ReAllocate(void* ptr, uintptr size)
{
	if (size)
	{
		void *newPtr = ptr ? HeapReAlloc(GetProcessHeap(), 0, ptr, size) : HeapAlloc(GetProcessHeap(), 0, size);
		if (!newPtr)
			throw;
		return newPtr;
	}
	if (ptr)
		HeapFree(GetProcessHeap(), 0, ptr);
	return nullptr;
}
bool XLib_Internal::Heap::ReAllocateInplace(void* ptr, uintptr size)
{
	void *newPtr = HeapReAlloc(GetProcessHeap(), 0, ptr, size);
	return newPtr ? true : false;
}
void XLib_Internal::Heap::Free(void* ptr)
{
	HeapFree(GetProcessHeap(), 0, ptr);
}