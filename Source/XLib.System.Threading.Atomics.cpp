#include <Windows.h>

#include "XLib.System.Threading.Atomics.h"

uint32 _Private::_AtomicBase<4>::_increment(volatile void* ptr)
{
	return _InterlockedIncrement((volatile uint32*)ptr);
}