#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"

class ConditionVariable : public NonCopyable
{
private:
	void *handle;

public:
	ConditionVariable();
	~ConditionVariable();

	void sleep();
	void wake();
	void wakeAll();
};