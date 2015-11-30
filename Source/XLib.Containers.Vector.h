#pragma once

#include "XLib.Types.h"
#include "XLib.Util.h"
#include "XLib.NonCopyable.h"
#include "XLib.Containers.HeapBuffer.h"

template <typename Type, uint32 initialBufferSize = 16>
class Vector : public NonCopyable
{
private:
	HeapBuffer<Type> buffer;
	uint32 bufferSize;

	inline void expandBuffer(uint32 minNewSize)
	{
		uint32 bufferSize = buffer.size();
		if (bufferSize < minNewSize)
		{
			if (bufferSize)
				bufferSize = maxval(bufferSize + bufferSize / 2 + 1, minNewSize);
			else
				bufferSize = maxval(initialBufferSize, minNewSize);
			buffer.resize(bufferSize);
		}
	}

public:
	explicit Vector(uint32 initSize = 0) : buffer(initSize), bufferSize(initSize) {}
	Vector(uint32 initSize, const Type* initData) : buffer(initSize, initData), bufferSize(initSize) {}
	~Vector() { bufferSize = 0; }

	Vector(Vector&& that) : buffer(move(that.buffer)), bufferSize(that.bufferSize)
	{
		that.bufferSize = 0;
	}
	Vector& operator = (Vector&& that)
	{
		swap(buffer, that.buffer);
		swap(bufferSize, that.bufferSize);
		return *this;
	}

	inline Type& pushBack(const Type& value)
	{
		expandBuffer(bufferSize + 1);
		buffer[bufferSize] = value;
		return buffer[bufferSize++];
	}
	inline Type popBack()
	{
		if (bufferSize)
			return buffer[--bufferSize];
		else
			throw;
	}
	inline Type& back() { return buffer[bufferSize - 1]; }

	template <typename KeyType>
	Type* find(const KeyType& key)
	{
		for (Type *current = buffer, *end = buffer + bufferSize; current < end; current++)
		{
			if (*current == key)
				return current;
		}
		return nullptr;
	}

	template <typename KeyType>
	uint32 findIndex(const KeyType& key)
	{
		for (uint32 i = 0; i < bufferSize; i++)
		{
			if (buffer[i] == key)
				return i;
		}
		return indexNotFound;
	}

	void resize(uint32 newSize)
	{
		bufferSize = newSize;
		expandBuffer(bufferSize);
	}
	void compact()
	{
		buffer.resize(bufferSize);
	}

	Vector<Type> copy()
	{
		return Vector<Type>(bufferSize, buffer.getData());
	}
	HeapBuffer<Type> convertToHeapArray()
	{
		buffer.resize(bufferSize);
		bufferSize = 0;
		return move(buffer);
	}

	inline operator Type*() const { return buffer; }
	inline uint32 size() const { return bufferSize; }
	inline uint32 byteSize() const { return bufferSize * sizeof(Type); }
	inline bool isEmpty() const { return bufferSize ? false : true; }

	inline Type* begin() const { return buffer; }
	inline Type* end() const { return buffer + bufferSize; }

	static constexpr uint32 indexNotFound = uint32(-1);
};