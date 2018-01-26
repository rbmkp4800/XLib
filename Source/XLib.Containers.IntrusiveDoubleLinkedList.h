#pragma once

namespace XLib
{
	struct IntrusiveDoubleLinkedListNodeHook
	{

	};

	template <typename NodeType, IntrusiveDoubleLinkedListNodeHook (NodeType::*Hook)>
	class IntrusiveDoubleLinkedList
	{
	private:

	public:

	};
}