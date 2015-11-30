#include <Windows.h>

#include "XLib.System.Heap.h"

void* Heap::Allocate(uintptr size)
{
	void *ptr = HeapAlloc(GetProcessHeap(), 0, size);
	if (!ptr)
		throw;
	return ptr;
}
void* Heap::Reallocate(void* block, uintptr size)
{
	void *newBlock = HeapReAlloc(GetProcessHeap(), 0, block, size);
	if (!newBlock)
		throw;
	return newBlock;
}
bool Heap::ReallocateInPlace(void* block, uintptr size)
{
	void *ptr = HeapReAlloc(GetProcessHeap(), 0, block, size);
	if (!ptr)
		return false;
	block = ptr;
	return true;
}
void Heap::Free(void* block)
{
	HeapFree(GetProcessHeap(), 0, block);
}
uintptr Heap::GetBlockSize(void* block)
{
	return HeapSize(GetProcessHeap(), 0, block);
}