#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"
#include "XLib.Util.h"

// TODO: do we need NonCopyable on IntrusiveSetNodeHook?
// TODO: do we need to zero IntrusiveSetNodeHook members?
// TODO: implement const iterator

#define hookLeft(node)		(((node).*(Hook (Node::*))nodeHook).left)
#define hookRight(node)		(((node).*(Hook (Node::*))nodeHook).right)
#define hookParent(node)	(((node).*(Hook (Node::*))nodeHook).parent)
#define hookHeight(node)	(((node).*(Hook (Node::*))nodeHook).height)

namespace XLib
{
	class IntrusiveSetNodeHook
	{
	public:
		void *left, *right, *parent;
		uint8 height;
	};

	template <typename Node, IntrusiveSetNodeHook (Node::*nodeHook)>
	class IntrusiveSet : public NonCopyable
	{
	private:
		struct Hook
		{
			Node *left, *right, *parent;
			uint8 height;
		};

		static_assert(sizeof(Hook) == sizeof(IntrusiveSetNodeHook), "invalid private hook size");

	public:
		class Iterator
		{
			friend class IntrusiveSet;

		private:
			Node *node = nullptr;

			inline Iterator(Node *node) : node(node) {}

		public:
			Iterator() = default;
			~Iterator() = default;

			inline Node& get() { return *node; }
			inline void next() { node = hookRight(*node); }
			inline void prev() { node = hookLeft(*node); }
			inline bool isValid() { return node != nullptr; }

			inline Node& operator * () { return *node; }
			inline bool operator == (const Iterator& that) const { return node == that.node; }
			inline bool operator != (const Iterator& that) const { return node != that.node; }
			inline void operator ++ () { next(); }
			inline void operator -- () { prev(); }
		};

	private:
		Node *root = nullptr;

		static inline void resetHeight(Node* self)
		{
			hookHeight(*self) = max(
				hookRight(*self) ? hookHeight(*hookRight(*self)) : 0,
				hookLeft(*self) ? hookHeight(*hookLeft(*self)) : 0) + 1;
		}
		static inline sint8 subtreesHeightDelta(const Node* self)
		{
			return (hookRight(*self) ? hookHeight(*hookRight(*self)) : 0) -
				(hookLeft(*self) ? hookHeight(*hookLeft(*self)) : 0);
		}

		static inline Node* rotateRight(Node* self)
		{
			Node* newParent = hookLeft(*self);
			hookLeft(*self) = hookRight(*newParent);
			if (hookLeft(*self))
				hookParent(*hookLeft(*self)) = self;
			hookRight(*newParent) = self;
			hookParent(*newParent) = hookParent(*self);
			hookParent(*self) = newParent;
			resetHeight(self);
			resetHeight(newParent);
			return newParent;
		}
		static inline Node* rotateLeft(Node* self)
		{
			Node* newParent = hookRight(*self);
			hookRight(*self) = hookLeft(*newParent);
			if (hookRight(*self))
				hookParent(*hookRight(*self)) = self;
			hookLeft(*newParent) = self;
			hookParent(*newParent) = hookParent(*self);
			hookParent(*self) = newParent;
			resetHeight(self);
			resetHeight(newParent);
			return newParent;
		}
		static inline Node* balance(Node* self)
		{
			resetHeight(self);
			if (subtreesHeightDelta(self) == 2)
			{
				if (subtreesHeightDelta(hookRight(*self)) < 0)
					hookRight(*self) = rotateRight(hookRight(*self));
				return rotateLeft(self);
			}
			if (subtreesHeightDelta(self) == -2)
			{
				if (subtreesHeightDelta(hookLeft(*self)) > 0)
					hookLeft(*self) = rotateLeft(hookLeft(*self));
				return rotateRight(self);
			}
			return self;
		}

	public:
		IntrusiveSet() = default;
		~IntrusiveSet() = default;

		template <typename KeyType>
		inline Iterator find(const KeyType& key)
		{

		}

		inline Iterator insert(Node& node)
		{
			if (!root)
			{
				root = &node;
				hookLeft(node) = nullptr;
				hookRight(node) = nullptr;
				hookParent(node) = nullptr;
				hookHeight(node) = 0;
				return Iterator(&node);
			}

			// insert new node
			Node *current = root;
			for (;;)
			{
				if (*current < node)
				{
					if (hookRight(*current))
						current = hookRight(*current);
					else
					{
						hookRight(*current) = &node;
						break;
					}
				}
				else if (node < *current)
				{
					if (hookLeft(*current))
						current = hookLeft(*current);
					else
					{
						hookLeft(*current) = &node;
						break;
					}
				}
				else // element with the same key exists
					return Iterator();
			}

			hookLeft(node) = nullptr;
			hookRight(node) = nullptr;
			hookParent(node) = current;
			hookHeight(node) = 0;	// TODO: ??

			// restore balance
			while (hookParent(*current))
			{
				Node* newCurrent = balance(current);
				Node *parent = hookParent(*newCurrent);
				if (hookLeft(*parent) == current)
					hookLeft(*parent) = newCurrent;
				else if (hookRight(*parent) == current)
					hookRight(*parent) = newCurrent;
				current = parent;
			}

			return Iterator(&node);
		}

		inline bool remove(Node& node)
		{

		}

		template <typename KeyType>
		inline bool remove(const KeyType& key)
		{

		}

		inline Iterator begin()
		{
			if (!root)
				return Iterator();

			Node *current = root;
			for (;;)
			{
				Node *next = hookLeft(*current);
				if (!next)
					return Iterator(current);
				current = next;
			}
		}

		inline Iterator end() { return Iterator(); }
	};
}

#undef hookLeft
#undef hookRight
#undef hookParent
#undef hookHeight