#pragma once

#include "ExLib.Types.h"
#include "ExLib.INonCopyable.h"
#include "ExLib.System.Threading.h"
#include "ExLib.System.Threading.Atomics.h"

class LockFreeCyclicBarrier : public INonCopyable
{
private:
	Atomic<uint32> threadCounter;
	uint32 threadCount;
	bool globalPhase;

public:
	class ThreadToken
	{
		friend LockFreeCyclicBarrier;

	private:
		bool phase;

		inline ThreadToken(bool _phase) : phase(_phase) {}
	};

	inline LockFreeCyclicBarrier(uint32 _threadCount = 0) : threadCounter(0), threadCount(_threadCount), globalPhase(false) {}
	inline void initialize(uint32 _threadCount)
	{
		threadCounter.set(0);
		threadCount = _threadCount;
		globalPhase = false;
	}

	inline ThreadToken getThreadToken() { return ThreadToken(!globalPhase); }

	template <typename CallbackType>
	inline void wait(ThreadToken& threadToken, CallbackType callback)
	{
		uint32 readyThreadCount = threadCounter.increment();
		if (readyThreadCount < threadCount)
		{
			while (globalPhase != threadToken.phase);
		}
		else
		{
			callback();
			threadCounter.set(0);
			globalPhase = threadToken.phase;
		}
		threadToken.phase = !globalPhase;
	}
};