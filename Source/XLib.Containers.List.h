#pragma once

enum class ListAllocationPolicy
{
	GlobalAllocator = 0,
	CustomAllocator = 1,
	LocalCustomAllocator = 2,
};

/*template <typename Type>
class DoublyLinkedList
{
private:
	struct Node
	{
		Type value;
		Node *prev, *next;

		inline Node(const Type& _value, Node *_prev = nullptr, Node *_next = nullptr)
			: value(_value), prev(_prev), next(_next) {}
	};

	Node *firstNode, *lastNode;

public:
	inline DoublyLinkedList() : firstNode(nullptr), lastNode(nullptr) {}

	class Enumerator
	{
		friend DoublyLinkedList;

	private:
		Node *node;

		inline Enumerator(Node *_node) : node(_node) {}

	public:
		inline Type& getValue() const { return node->value; }
		inline void goForward() { node = node->next; }
		inline void goBackward() { node = node->prev; }

		inline bool isLast() const { return node->next ? false : true; }
		inline bool isFirst() const { return node->prev ? false : true; }
		inline bool isValid() const { return node ? true : false; }
	};

	inline void pushBack()
	{

	}
	inline void pushFront()
	{

	}
	inline Type popBack()
	{

	}
	inline Type popFront()
	{

	}

	inline void pushAfter(Enumerator enumerator, const Type& value)
	{
		if (enumerator.isValid())
			enumerator.node->next = new Node(value, enumerator.node, enumerator.node->next);
		else if (isEmpty())
			firstNode = lastNode = new Node(value);
	}
	inline void pushBefore(Enumerator enumerator, const Type& value)
	{
		if (enumerator.isValid())
			enumerator.node->prev = new Node(value, enumerator.node->prev, enumerator.node);
		else if (isEmpty())
			firstNode = lastNode = new Node(value);
	}

	inline bool isEmpty() { return firstNode ? false : true; }
	inline Enumerator getFront() const { return Enumerator(firstNode); }
	inline Enumerator getBack() const { return Enumerator(lastNode); }
};*/