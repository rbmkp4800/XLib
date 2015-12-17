#pragma once

#include "XLib.Types.h"
#include "XLib.Util.h"
#include "XLib.NonCopyable.h"
#include "XLib.Containers.Buffer.h"

template <typename Type, uint32 initialBufferSize = 16>
class Vector : public NonCopyable
{
private:
	Buffer<Type> buffer;
	uint32 vectorSize;

	inline void expandBuffer(uint32 minNewSize)
	{
		uint32 vectorSize = buffer.size();
		if (vectorSize < minNewSize)
		{
			if (vectorSize)
				vectorSize = maxval(vectorSize + vectorSize / 2 + 1, minNewSize);
			else
				vectorSize = maxval(initialBufferSize, minNewSize);
			buffer.resize(vectorSize);
		}
	}

public:
	explicit Vector(uint32 initSize = 0) : buffer(initSize), vectorSize(initSize) {}
	Vector(uint32 initSize, const Type* initData) : buffer(initSize, initData), vectorSize(initSize) {}
	~Vector() { vectorSize = 0; }

	Vector(Vector&& that) : buffer(move(that.buffer)), vectorSize(that.vectorSize)
	{
		that.vectorSize = 0;
	}
	Vector& operator = (Vector&& that)
	{
		swap(buffer, that.buffer);
		swap(vectorSize, that.vectorSize);
		return *this;
	}

	inline Type& pushBack(const Type& value)
	{
		expandBuffer(vectorSize + 1);
		buffer[vectorSize] = value;
		return buffer[vectorSize++];
	}
	inline Type popBack()
	{
		if (vectorSize)
			return buffer[--vectorSize];
		else
			throw;
	}
	inline Type& back() { return buffer[vectorSize - 1]; }

	template <typename KeyType>
	Type* find(const KeyType& key)
	{
		for (Type *current = buffer, *end = buffer + vectorSize; current < end; current++)
		{
			if (*current == key)
				return current;
		}
		return nullptr;
	}

	template <typename KeyType>
	uint32 findIndex(const KeyType& key)
	{
		for (uint32 i = 0; i < vectorSize; i++)
		{
			if (buffer[i] == key)
				return i;
		}
		return indexNotFound;
	}

	void resize(uint32 newSize)
	{
		vectorSize = newSize;
		expandBuffer(vectorSize);
	}
	void compact()
	{
		buffer.resize(vectorSize);
	}

	Vector<Type> copy()
	{
		return Vector<Type>(vectorSize, buffer.getData());
	}
	Buffer<Type> convertToBuffer()
	{
		buffer.resize(vectorSize);
		vectorSize = 0;
		return move(buffer);
	}

	inline operator Type*() const { return buffer; }
	inline uint32 size() const { return vectorSize; }
	inline uint32 byteSize() const { return vectorSize * sizeof(Type); }
	inline bool isEmpty() const { return vectorSize ? false : true; }

	inline Type* begin() const { return buffer; }
	inline Type* end() const { return buffer + vectorSize; }

	static constexpr uint32 indexNotFound = uint32(-1);

	using type = Type;
};