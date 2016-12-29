#pragma once

#include "XLib.Types.h"

namespace XLib
{
	enum class MultiSequentialSetStoragePolicy : uint32
	{
		InternalStatic,
		InternalHeap,
	};

	template <typename Type, uint32 sequenceCount, MultiSequentialSetStoragePolicy storagePolicy, uint16 staticStorageSize = 0>
	class MultiSequentialSet;
	//{ static_assert(false, "XLib.Containers.MultiSequentialSet: invalid MultidirectionalSetStoragePolicy value"); };

	template <typename Type, uint32 sequenceCount, uint16 staticStorageSize>
	class MultiSequentialSet<Type, sequenceCount, MultiSequentialSetStoragePolicy::InternalStatic, staticStorageSize>
	{
	private:
		struct Node
		{
			Type value;
			union
			{
				uint16 nextFreeNode;
				struct { uint16 left, right; } links[sequenceCount];
			};
		};

		Node nodesBuffer[staticStorageSize];
		uint16 roots[sequenceCount];
		uint16 firstFreeNode;

		template <uint32 sequence>
		class checkSequenceNumber abstract final
		{
			static_assert(sequence < sequenceCount, "XLib.Containers.MultiSequentialSet: wrong sequence value");
		};

	public:
		inline MultiSequentialSet()
		{
			for each (uint16& root in roots)
				root = uint16(-1);
		}

		template <uint32 sequence>
		inline Type& insert(Type& value)
		{
			checkSequenceNumber<sequence>;
		}

		template <uint32 sequence, typename KeyType>
		inline Type& find(const KeyType& key)
		{
			checkSequenceNumber<sequence>;
		}

		inline bool isEmpty() { }
		inline bool isFull() { }
	};
}