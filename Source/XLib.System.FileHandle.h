#pragma once

#include "XLib.NonCopyable.h"

class ISystemFileHandle : public NonCopyable
{
protected:
	void *handle;

public:
	inline ISystemFileHandle() : handle(nullptr) {}
	~ISystemFileHandle();

	inline void destroy() { this->~ISystemFileHandle(); }

	inline bool isInitialized() { return handle ? true : false; }
};