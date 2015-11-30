#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"

class CriticalSection : public NonCopyable
{
private:
	static constexpr uint32 sizeof_CRITICAL_SECTION = 24;

	struct
	{
		byte data[sizeof_CRITICAL_SECTION];
	} handle;

public:
	class Lock : public NonCopyable
	{
		friend CriticalSection;

	private:
		bool locked;

		Lock(CriticalSection& criticalSection);

	public:
		inline ~Lock() { unlock(); }
		void unlock();
	};

	CriticalSection();
	~CriticalSection();

	Lock lock();
};