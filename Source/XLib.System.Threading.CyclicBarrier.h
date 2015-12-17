#pragma once

#include "XLib.NonCopyable.h"
#include "XLib.System.Threading.Event.h"
#include "XLib.System.Threading.Atomics.h"

class CyclicBarrier : public NonCopyable
{
private:
	Event events[2];
	Atomic<uint32> counter;
	uint32 threadCount;
	uint8 eventSelector;

public:
	inline CyclicBarrier() : counter(0), threadCount(0), eventSelector(0) {}
	inline CyclicBarrier(uint32 _threadCount) { initialize(_threadCount); }
	inline ~CyclicBarrier() { destroy(); }

	inline void initialize(uint32 _threadCount)
	{
		if (_threadCount)
		{
			counter.value = 0;
			events[0].initialize(false);
			events[1].initialize(false);
			threadCount = _threadCount;
			eventSelector = 0;
		}
		else
			destroy();
	}
	inline void destroy()
	{
		counter.value = 0;
		events[0].destroy();
		events[1].destroy();
		threadCount = 0;
		eventSelector = 0;
	}
	inline bool isInitialized() { return threadCount ? true : false; }

	template <typename CallbackType>
	inline void wait(CallbackType callback)
	{
		if (!isInitialized())
			throw;

		Event &event = events[eventSelector];
		event.reset();
		uint32 readyThreadCount = counter.increment();
		if (readyThreadCount < threadCount)
			event.wait();
		else
		{
			callback();
			counter.value = 0;
			eventSelector = (eventSelector + 1) % 2;
			event.set();
		}
	}
	inline void wait() { wait([](){}); }
};