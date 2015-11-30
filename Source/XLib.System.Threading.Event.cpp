#include <Windows.h>

#include "XLib.System.Threading.Event.h"

void Event::initialize(bool state, bool manualReset)
{
	destroy();
	handle = CreateEvent(nullptr, manualReset, state, nullptr);
}
void Event::set()
{
	SetEvent(handle);
}
void Event::reset()
{
	ResetEvent(handle);
}
void Event::pulse()
{
	PulseEvent(handle);
}