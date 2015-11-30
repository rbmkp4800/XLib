#pragma once

#include "XLib.Types.h"
#include "XLib.Util.h"

template <typename BufferType>
void QuickSort(BufferType& buffer, uint32 elementsCount)
{
	using ElementType = typename RemoveReference<decltype(buffer[0])>::Type;

	constexpr uint32 maxQSortElementsLog2 = 32;
	struct StackFrame
	{
		sint32 low, high;
		inline void set(sint32 _low, sint32 _high) { low = _low; high = _high; }
	} stack[maxQSortElementsLog2 * 2];
	stack[0].set(0, elementsCount - 1);

	sint32 stackHigh = 1;
	while (stackHigh >= 1)
	{
		stackHigh--;
		StackFrame frame = stack[stackHigh];
		sint32 i = frame.low, j = frame.high;
		ElementType pivot = buffer[(i + j) / 2];
		do
		{
			while (pivot > buffer[i]) i++;
			while (buffer[j] > pivot) j--;
			if (i <= j)
			{
				swap(buffer[i], buffer[j]);
				i++;
				j--;
			}
		} while (i <= j);
		if (i < frame.high)
			stack[stackHigh++].set(i, frame.high);
		if (frame.low < j)
			stack[stackHigh++].set(frame.low, j);
	}
}