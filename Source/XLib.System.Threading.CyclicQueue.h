#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"
#include "XLib.System.Threading.Atomics.h"
#include "XLib.System.Threading.Event.h"

// TODO:
//		1. swap front and back (wrong notation)
//		2. add CyclicQueueStoratePolicy

template <typename Type, uint32 sizeLog2, uint32 spinCount = 2000>
class ThreadSafeCyclicQueue : public NonCopyable
{
private:
	static constexpr uint32 size = 1 << sizeLog2;
	static_assert(spinCount != 0, "ThreadSafeCyclicQueue spinCount can't be 0");

	Type buffer[size];
	Event frontEvent, backEvent;
	Atomic<uint32> frontIdx, backIdx;
	volatile uint32 readyFrontIdx, readyBackIdx;
	Atomic<uint16> waitingFrontCount, waitingBackCount;

public:
	inline ThreadSafeCyclicQueue() : frontIdx(0), backIdx(0),
		readyFrontIdx(0), readyBackIdx(0), waitingFrontCount(0), waitingBackCount(0) {}

	inline void initialize()
	{
		frontIdx.value = 0;
		backIdx.value = 0;
		readyFrontIdx = 0;
		readyBackIdx = 0;
		waitingFrontCount.value = 0;
		waitingBackCount.value = 0;

		frontEvent.initialize(false);
		backEvent.initialize(false);
	}

	inline void push(const Type& value)
	{
		for (;;)
		{
			uint32 lastLocalReadyBackIdx = readyBackIdx;
			for (sint32 spin = 0; spin < spinCount; spin++)
			{
				uint32 localFrontIdx = frontIdx.load(), localReadyBackIdx = readyBackIdx;
				if (localFrontIdx - localReadyBackIdx < size)
				{
					if (frontIdx.compareExchange(localFrontIdx + 1, localFrontIdx))
					{
						buffer[localFrontIdx % size] = value;

						while (readyFrontIdx != localFrontIdx)
							{ }
						readyFrontIdx++;

						if (waitingBackCount.load())
							backEvent.set();

						return;
					}
					spin = -1;
				}
				if (lastLocalReadyBackIdx != localReadyBackIdx)
				{
					spin = -1;
					lastLocalReadyBackIdx = localReadyBackIdx;
				}
			}

			frontEvent.reset();
			waitingFrontCount.increment();
			if (lastLocalReadyBackIdx != readyBackIdx)
			{
				frontEvent.set();
				waitingFrontCount.decrement();
				continue;
			}

			frontEvent.wait();
			waitingFrontCount.decrement();
		}
	}

	inline Type pop()
	{
		for (;;)
		{
			uint32 lastLocalReadyFrontIdx = readyFrontIdx;
			for (sint32 spin = 0; spin < spinCount; spin++)
			{
				uint32 localBackIdx = backIdx.load(), localReadyFrontIdx = readyFrontIdx;
				if (localReadyFrontIdx - localBackIdx > 0)
				{
					if (backIdx.compareExchange(localBackIdx + 1, localBackIdx))
					{
						Type result = buffer[localBackIdx % size];

						while (readyBackIdx != localBackIdx)
							{ }
						readyBackIdx++;

						if (waitingFrontCount.load())
							frontEvent.set();

						return result;
					}
					spin = -1;
				}
				if (lastLocalReadyFrontIdx != localReadyFrontIdx)
				{
					spin = -1;
					lastLocalReadyFrontIdx = localReadyFrontIdx;
				}
			}

			backEvent.reset();
			waitingBackCount.increment();
			if (lastLocalReadyFrontIdx != readyFrontIdx)
			{
				backEvent.set();
				waitingBackCount.decrement();
				continue;
			}

			backEvent.wait();
			waitingBackCount.decrement();
		}
	}

	// non thread safe
	inline uint32 elementCount() { return frontIdx.value - backIdx.value; }
	inline bool isEmpty() { return frontIdx.value == backIdx.value; }
	inline bool isFull() { return frontIdx.value - backIdx.value >= size; }
};