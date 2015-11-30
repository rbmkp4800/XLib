#include <Windows.h>

#include "XLib.Util.h"

#include "XLib.System.Threading.ConditionVariable.h"

constexpr uint32 s = sizeof(CRITICAL_SECTION);

ConditionVariable::ConditionVariable()
{
	static_assert(sizeof(ConditionVariable::handle) == sizeof(CONDITION_VARIABLE), 
		"XLib.System.Threading.ConditionVariable: size of ConditionVariable::handle must be same as size of CONDITION_VARIABLE");
	
	InitializeConditionVariable(&as<CONDITION_VARIABLE>(handle));
}

void ConditionVariable::sleep()
{

}

void ConditionVariable::wake()
{
}

void ConditionVariable::wakeAll()
{
}
