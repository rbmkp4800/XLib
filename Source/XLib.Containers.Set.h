#pragma once

#include "XLib.Types.h"
#include "XLib.System.Heap.h"
#include "XLib.NonCopyable.h"

namespace _private
{
	template <typename Type, uint32 sizeLimitLog2>
	class _SetBase abstract	: public NonCopyable // AVL tree
	{
	private:
		struct Node
		{
			Type value;

			Node *left, *right;
			uint8 heightDelta;
		} *root;

	protected:
		inline _SetBase() : root(nullptr) {}

		template <typename MemoryAllocateCallback>
		inline Type& add(const Type& value, MemoryAllocateCallback allocate)
		{
			Node *currentNode = root;
			while (currentNode)
			{
				if (value < currentNode->value)
					currentNode = currentNode->left;
				else if (currentNode->value < value)
					currentNode = currentNode->right;
				else
					currentNode->value;
			}
			return ((Node*)allocate())->value;
		}

		template <typename MemoryFreeCallback>
		inline void remove(const Type& value, MemoryFreeCallback free)
		{
			Node *currentNode = root;
			while (currentNode)
			{
				if (value < currentNode->value)
					currentNode = currentNode->left;
				else if (currentNode->value < value)
					currentNode = currentNode->right;
				else
					free(currentNode);
			}
		}

		template <typename MemoryFreeCallback>
		inline void destroy(MemoryFreeCallback free)
		{
			Node *stack[sizeLimitLog2];
			uint32 currentFrame = 1;
			stack[0] = root;
			while (currentFrame > 0)
			{
				currentFrame--;
				Node *node = stack[currentFrame]->left;
				if (node->left)
				{
					currentFrame++;
					stack[currentFrame] = node->left;
					node->left = nullptr;
					continue;
				}
				if (node->right)
				{
					currentFrame++;
					stack[currentFrame] = node->right;
					node->right = nullptr;
					continue;
				}
				free(stack[currentFrame]);
			}
		}

		inline Type* find(const Type& value)
		{
			Node *currentNode = root;
			while (currentNode)
			{
				if (value < currentNode->value)
					currentNode = currentNode->left;
				else if (currentNode->value < value)
					currentNode = currentNode->right;
				else
					return currentNode->value;
			}
			return nullptr;
		}

		static constexpr uint32 bytesPerElement = sizeof(Node);

		using type = Type;
	};
}

enum class SetAllocationPolicy
{
	GlobalAllocator = 0,
	CustomAllocator = 1,
	Default = GlobalAllocator,
};

template <typename Type, SetAllocationPolicy policy = Default, uint32 sizeLimitLog2 = 32>
class Set;

template <typename Type, uint32 sizeLimitLog2>
class Set<Type, SetAllocationPolicy::GlobalAllocator, sizeLimitLog2> : public _private::_SetBase<Type, sizeLimitLog2>
{
public:
	Set() = default;
	inline ~Set()
	{
		destroy([](void* block) { Heap::Free(block); });
	}

	inline Type& add(const Type& value)
	{
		return _SetBase::add(value, []() -> void* { return Heap::Allocate(_SetBase::bytesPerElement); });
	}
	inline void remove(const Type& value)
	{
		_SetBase::remove(value, [](void* block) { Heap::Free(block); });
	}
	inline 
};