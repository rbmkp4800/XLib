#pragma once

#include "XLib.Types.h"
#include "XLib.Util.h"
#include "XLib.NonCopyable.h"
#include "XLib.Heap.h"

template <typename Type, uint32 initialBufferSegmentCount = 16>
class Vector : public NonCopyable
{
private:
	HeapPtr<Type> buffer;
	uint32 bufferSize, vectorSize;

	inline void expandBuffer(uint32 minNewSize)
	{
		if (bufferSize < minNewSize)
		{
			if (bufferSize)
				bufferSize = max(vectorSize * 2, minNewSize);
			else
				bufferSize = max(initialBufferSegmentCount, minNewSize);
			buffer.resize(bufferSize);
		}
	}

public:
	explicit inline Vector(uint32 initSize = 0) : buffer(initSize), bufferSize(initSize), vectorSize(initSize) {}
	inline ~Vector() { bufferSize = 0; vectorSize = 0; }

	inline Vector(Vector&& that) : buffer(move(that.buffer))
	{
		bufferSize = that.bufferSize;
		vectorSize = that.vectorSize;
		that.bufferSize = 0;
		that.vectorSize = 0;
	}
	inline void operator = (Vector&& that)
	{
		swap(buffer, that.buffer);
		swap(bufferSize, that.bufferSize);
		swap(vectorSize, that.vectorSize);
		return *this;
	}

	inline Type& pushBack(const Type& value)
	{
		expandBuffer(vectorSize + 1);
		buffer[vectorSize] = value;
		return buffer[vectorSize++];
	}
	inline Type popBack() { return buffer[--vectorSize]; }
	inline Type& back() { return buffer[vectorSize - 1]; }

	void resize(uint32 newSize)
	{
		vectorSize = newSize;
		expandBuffer(vectorSize);
	}
	void compact() { buffer.resize(vectorSize); }

	inline operator Type*() const { return buffer; }
	inline uint32 size() const { return vectorSize; }
	inline uint32 byteSize() const { return vectorSize * sizeof(Type); }
	inline bool isEmpty() const { return vectorSize ? false : true; }

	template <typename OtherType>
	inline OtherType to() { return OtherType(ptr); }

	inline Type* begin() const { return buffer; }
	inline Type* end() const { return buffer + vectorSize; }
};