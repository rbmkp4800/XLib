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
	uint32 _size;

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
	explicit Vector(uint32 initSize = 0) : buffer(initSize), _size(initSize) {}
	Vector(uint32 initSize, const Type* initData) : buffer(initSize, initData), _size(initSize) {}
	~Vector() { _size = 0; }

	Vector(Vector&& that) : buffer(move(that.buffer)), _size(that._size)
	{
		that._size = 0;
	}
	Vector& operator = (Vector&& that)
	{
		swap(buffer, that.buffer);
		swap(_size, that._size);
		return *this;
	}

	inline Type& pushBack(const Type& value)
	{
		expandBuffer(_size + 1);
		buffer[_size] = value;
		return buffer[_size++];
	}
	inline Type popBack()
	{
		if (_size)
			return buffer[--_size];
		else
			throw;
	}
	inline Type& back() { return buffer[_size - 1]; }

	template <typename KeyType>
	Type* find(const KeyType& key)
	{
		for (Type *current = buffer, *end = buffer + _size; current < end; current++)
		{
			if (*current == key)
				return current;
		}
		return nullptr;
	}

	template <typename KeyType>
	uint32 findIndex(const KeyType& key)
	{
		for (uint32 i = 0; i < _size; i++)
		{
			if (buffer[i] == key)
				return i;
		}
		return indexNotFound;
	}

	void resize(uint32 newSize)
	{
		_size = newSize;
		expandBuffer(_size);
	}
	void compact()
	{
		buffer.resize(_size);
	}

	Vector<Type> copy()
	{
		return Vector<Type>(_size, buffer.getData());
	}
	HeapBuffer<Type> convertToHeapArray()
	{
		buffer.resize(_size);
		_size = 0;
		return move(buffer);
	}

	inline operator Type*() const { return buffer; }
	inline uint32 size() const { return _size; }
	inline uint32 byteSize() const { return _size * sizeof(Type); }
	inline bool isEmpty() const { return _size ? false : true; }

	inline Type* begin() const { return buffer; }
	inline Type* end() const { return buffer + _size; }

	static constexpr uint32 indexNotFound = uint32(-1);
};