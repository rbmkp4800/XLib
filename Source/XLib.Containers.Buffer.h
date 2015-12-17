#pragma once

#include "XLib.Types.h"
#include "XLib.Util.h"
#include "XLib.NonCopyable.h"
#include "XLib.System.Heap.h"
#include "XLib.Memory.h"

template <typename Type>
class Buffer : public NonCopyable
{
private:
	Type *buffer;
	uint32 bufferSize;

public:
	explicit Buffer(uint32 initSize = 0) : bufferSize(initSize)
	{
		buffer = bufferSize ? Heap::Allocate<Type>(bufferSize) : nullptr;
	}
	Buffer(uint32 initSize, const Type *initData) : bufferSize(initSize)
	{
		if (bufferSize)
		{
			buffer = Heap::Allocate<Type>(bufferSize);
			Memory::Copy(buffer, initData, bufferSize * sizeof(Type));
		}
		else
			buffer = nullptr;
	}
	~Buffer()
	{
		if (buffer)
		{
			Heap::Free(buffer);
			buffer = nullptr;
			bufferSize = 0;
		}
	}

	Buffer(Buffer&& that)
	{
		buffer = that.buffer;
		bufferSize = that.bufferSize;
		that.buffer = nullptr;
		that.bufferSize = 0;
	}
	Buffer& operator = (Buffer&& that)
	{
		swap(buffer, that.buffer);
		swap(bufferSize, that.bufferSize);
		return *this;
	}

	void resize(uint32 newSize)
	{
		bufferSize = newSize;
		if (bufferSize)
		{
			if (buffer)
				buffer = Heap::Reallocate<Type>(buffer, bufferSize);
			else
				buffer = Heap::Allocate<Type>(bufferSize);
		}
		else if (buffer)
		{
			Heap::Free(buffer);
			buffer = nullptr;
		}
	}
	Buffer<Type> copy() const { return Buffer<Type>(bufferSize, buffer); }

	inline operator Type*() const { return buffer; }
	inline uint32 size() const { return bufferSize; }
	inline uint32 byteSize() const { return bufferSize * sizeof(Type); }

	inline Type* begin() const { return buffer; }
	inline Type* end() const { return buffer + bufferSize; }

	using type = Type;
};