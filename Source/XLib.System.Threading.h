#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"

// TODO: create Thread::destroy();

class ISystemHandle abstract : public NonCopyable
{
protected:
	void *handle;

public:
	inline ISystemHandle() : handle(nullptr) {}
	inline ~ISystemHandle() { destroy(); }

	void destroy();

	inline bool isInitialized() { return handle ? true : false; }
};

template <typename Type>
using ThreadMainProc = uint32(__stdcall*)(Type*);

namespace _private
{
	bool WaitAll(void** handles, uint32 handleCount);
}

class IWaitable abstract : public ISystemHandle
{
public:
	/*template <typename Type>
	static inline void WaitAll(Type *waitableVector, uint32 waitableCount)
	{

	}*/
	template <typename Type, uint32 count>
	static inline void WaitAll(Type(&waitables)[count], uint32 waitableCount)
	{
		_private::WaitAll((void**)waitables, waitableCount);
		static_assert(sizeof(Type) == sizeof(void*), "Waitable must inherit IWaitable interface, member vars not allowed");
	}

	bool wait();
	bool wait(uint32 timeout);
};

class Thread : public IWaitable
{
private:
	void _create(ThreadMainProc<void> threadMainProc, void* args, bool suspended);

public:
	Thread() = default;
	template <typename Type>
	inline Thread(ThreadMainProc<Type> threadMainProc, Type* args = nullptr, bool suspended = false)
	{
		_create(ThreadMainProc<void>(threadMainProc, args, suspended));
	}
	inline ~Thread()
	{
		terminate();
	}

	void terminate(uint32 exitCode = 0);
	void suspend();
	void resume();

	template <typename Type>
	inline void create(ThreadMainProc<Type> threadMainProc, Type* args = nullptr, bool suspended = false)
	{
		_create(ThreadMainProc<void>(threadMainProc), args, suspended);
	}
};