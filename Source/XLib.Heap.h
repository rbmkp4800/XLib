#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"

namespace XLib_Internal
{
	struct Heap abstract final
	{
		static void* Allocate(uintptr size);
		static void* ReAllocate(void* ptr, uintptr size);
		static bool ReAllocateInplace(void* ptr, uintptr size);
		static void Free(void* ptr);
	};
}

template <typename Type>
class HeapPtr : public NonCopyable
{
private:
	Type *ptr;

public:
	inline explicit HeapPtr(uintptr size) : ptr((Type*) XLib_Internal::Heap::Allocate(size * sizeof(Type))) {}
	inline HeapPtr() : ptr(nullptr) {}
	~HeapPtr() {  }

	inline HeapPtr(HeapPtr&& that) : ptr(that.ptr) { that.ptr = nullptr; }
	inline void operator = (HeapPtr&& that) { swap(ptr, that.ptr); }

	void free()
	{
		if (ptr)
		{
			XLib_Internal::Heap::Free(ptr);
			ptr = nullptr;
		}
	}
	void resize(uintptr size) { ptr = (Type*) XLib_Internal::Heap::ReAllocate(ptr, size * sizeof(Type)); }
	bool resizeInplace(uintptr size) { return XLib_Internal::Heap::ReAllocateInplace(ptr, size * sizeof(Type)); }

	inline operator Type* () { return ptr; }
	inline bool isAllocated() { return ptr ? true : false; }

	template <typename OtherType>
	inline OtherType to() { return OtherType(ptr); }
};