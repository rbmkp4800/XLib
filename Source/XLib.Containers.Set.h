#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"
#include "XLib.PoolAllocator.h"

// TODO: remove all "preallocated" shit
// TODO: refactor allocators (make it template argument)
// TODO: rewrite using intrusive version
// TODO: change comparator to "less"
// TODO: if element already exists - return invalid iterator instead of
//			replacing old element

namespace XLib
{
	struct SetStoragePolicy abstract final
	{
		/*template <uint32 bufferSize>
		class InternalStatic abstract final {};*/

		template <uint32 minBufferSizeLog2, uint32 maxBufferSizeLog2>
		class InternalHeapBuffer abstract final {};
	};

	template <typename Type, typename StoragePolicy>
	class Set;

	template <typename Type, uint32 minBufferSizeLog2, uint32 maxBufferSizeLog2>
	class Set<Type, SetStoragePolicy::InternalHeapBuffer<minBufferSizeLog2, maxBufferSizeLog2>> : public NonCopyable
	{
	private:
		struct AVLTreeNode
		{
			Type value;
			AVLTreeNode *left, *right, *parent;
			sint8 height;

			inline void resetHeight() { height = max(right ? right->height : 0, left ? left->height : 0) + 1; }
			inline sint8 subtreesHeightDelta() { return (right ? right->height : 0) - (left ? left->height : 0); }

			inline AVLTreeNode* rotateRight()
			{
				AVLTreeNode* newParent = left;
				left = newParent->right;
				if (left)
					left->parent = this;
				newParent->right = this;
				newParent->parent = parent;
				parent = newParent;
				resetHeight();
				newParent->resetHeight();
				return newParent;
			}

			inline AVLTreeNode* rotateLeft()
			{
				AVLTreeNode* newParent = right;
				right = newParent->left;
				if (right)
					right->parent = this;
				newParent->left = this;
				newParent->parent = parent;
				parent = newParent;
				resetHeight();
				newParent->resetHeight();
				return newParent;
			}

			inline AVLTreeNode* balance()
			{
				resetHeight();
				if (subtreesHeightDelta() == 2)
				{
					if (right->subtreesHeightDelta() < 0)
						right = right->rotateRight();
					return rotateLeft();
				}
				if (subtreesHeightDelta() == -2)
				{
					if (left->subtreesHeightDelta() > 0)
						left = left->rotateLeft();
					return rotateRight();
				}
				return this;
			}
		};

		using Allocator = PoolAllocator<AVLTreeNode,
			PoolAllocatorHeapUsagePolicy::MultipleStaticChunks<minBufferSizeLog2, maxBufferSizeLog2>>;

		Allocator allocator;
		AVLTreeNode *root;

		inline void _insert(AVLTreeNode* node)
		{
			if (root)
			{
				AVLTreeNode *current = root;
				for (;;)
				{
					if (node->value > current->value)
					{
						if (current->right)
							current = current->right;
						else
						{
							node->parent = current;
							current->right = node;
							break;
						}
					}
					else if (current->value > node->value)
					{
						if (current->left)
							current = current->left;
						else
						{
							node->parent = current;
							current->left = node;
							break;
						}
					}
					else
					{
						node->parent = current->parent;
						if (node->parent)
						{
							if (node->parent->left == current)
								node->parent->left = node;
							else if (node->parent->right == current)
								node->parent->right = node;
						}
						allocator.release(current);
						current = node;
						break;
					}
				}

				while (current->parent)
				{
					AVLTreeNode* newCurrent = current->balance();
					AVLTreeNode *parent = newCurrent->parent;
					if (parent->left == current)
						parent->left = newCurrent;
					else if (parent->right == current)
						parent->right = newCurrent;
					current = parent;
				}

				root = current->balance();
			}
			else
			{
				root = node;
				root->parent = nullptr;
			}
		}

	public:
		class Iterator
		{
			friend class Set;

		private:
			AVLTreeNode *current;
			inline Iterator(AVLTreeNode *_current) : current(_current) {}

		public:
			inline Type& get() { return current->value; }
			inline bool isValid() { return current != nullptr; }
		};

		class PreAllocatedElementHandle
		{
			friend class Set;

		private:
			AVLTreeNode *node;
			inline PreAllocatedElementHandle(AVLTreeNode* _node) : node(_node) {}

		public:
			inline PreAllocatedElementHandle() : node(nullptr) {}
			inline Type& get() { return node->value; }
		};

		inline Set() : root(nullptr) {}
		~Set() = default;

		inline PreAllocatedElementHandle preAllocate()
		{
			AVLTreeNode *node = allocator.allocate();
			node->left = nullptr;
			node->right = nullptr;
			node->parent = nullptr;
			return PreAllocatedElementHandle(node);
		}

		inline void insertPreAllocated(PreAllocatedElementHandle handle)
		{
			Debug::CrashConditionOnDebug(handle.node == nullptr || handle.node->left || handle.node->right
				|| handle.node->parent, DbgMsgFmt("invalid preallocated handle"));

			handle.node->left = nullptr;
			handle.node->right = nullptr;
			handle.node->parent = nullptr;
			handle.node->height = 1;
			_insert(handle.node);
		}

		inline Iterator insert(const Type& value)
		{
			AVLTreeNode *node = allocator.allocate();
			node->value = value;
			node->left = nullptr;
			node->right = nullptr;
			node->parent = nullptr;
			node->height = 1;
			_insert(node);
			return Iterator(node);
		}

		/*template <typename KeyType>
		inline void remove(const KeyType& key)
		{

		}*/

		template <typename KeyType>
		inline Iterator find(const KeyType& key)
		{
			AVLTreeNode *current = root;
			while (current)
			{
				if (key > current->value)
					current = current->right;
				else if (current->value > key)
					current = current->left;
				else
					return Iterator(current);
			}
			return Iterator(nullptr);
		}
	};
}