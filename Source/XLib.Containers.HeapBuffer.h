#pragma once

#include "XLib.Types.h"
#include "XLib.Util.h"
#include "XLib.NonCopyable.h"
#include "XLib.System.Heap.h"
#include "XLib.Memory.h"

template <typename Type>
class HeapBuffer : public NonCopyable
{
private:
	Type *buffer;
	uint32 _size;

public:
	explicit HeapBuffer(uint32 initSize = 0) : _size(initSize)
	{
		buffer = _size ? Heap::Allocate<Type>(_size) : nullptr;
	}
	HeapBuffer(uint32 initSize, const Type *initData) : _size(initSize)
	{
		if (_size)
		{
			buffer = Heap::Allocate<Type>(_size);
			Memory::Copy(buffer, initData, _size * sizeof(Type));
		}
		else
			buffer = nullptr;
	}
	~HeapBuffer()
	{
		if (buffer)
		{
			Heap::Free(buffer);
			buffer = nullptr;
			_size = 0;
		}
	}

	HeapBuffer(HeapBuffer&& that)
	{
		buffer = that.buffer;
		_size = that._size;
		that.buffer = nullptr;
		that._size = 0;
	}
	HeapBuffer& operator = (HeapBuffer&& that)
	{
		swap(buffer, that.buffer);
		swap(_size, that._size);
		return *this;
	}

	void resize(uint32 newSize)
	{
		_size = newSize;
		if (_size)
		{
			if (buffer)
				buffer = Heap::Reallocate<Type>(buffer, _size);
			else
				buffer = Heap::Allocate<Type>(_size);
		}
		else if (buffer)
		{
			Heap::Free(buffer);
			buffer = nullptr;
		}
	}
	HeapBuffer<Type> copy() const { return HeapBuffer<Type>(_size, buffer); }

	inline operator Type*() const { return buffer; }
	inline uint32 size() const { return _size; }
	inline uint32 byteSize() const { return _size * sizeof(Type); }

	inline Type* begin() const { return buffer; }
	inline Type* end() const { return buffer + _size; }
};