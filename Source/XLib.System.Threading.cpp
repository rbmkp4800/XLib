#include <Windows.h>

#include "XLib.System.Threading.h"

void ISystemHandle::destroy()
{
	if (handle)
	{
		CloseHandle(handle);
		handle = nullptr;
	}
}

bool IWaitable::wait()
{
	DWORD result = WaitForSingleObject(handle, INFINITE);
	return result == WAIT_OBJECT_0;
}
bool IWaitable::wait(uint32 timeout)
{
	DWORD result = WaitForSingleObject(handle, timeout);
	return result == WAIT_OBJECT_0;
}

void Thread::_create(ThreadMainProc<void> threadMainProc, void* args, bool suspended)
{
	DWORD threadId = 0;
	handle = CreateThread(nullptr, 0, threadMainProc, args, suspended ? CREATE_SUSPENDED : 0, &threadId);
}
void Thread::terminate(uint32 exitCode)
{
	TerminateThread(handle, exitCode);
}
void Thread::suspend()
{
	SuspendThread(handle);
}
void Thread::resume()
{
	ResumeThread(handle);
}

bool _private::WaitAll(void** handles, uint32 handleCount)
{
	WaitForMultipleObjects(handleCount, handles, true, INFINITE);
	return true;
}