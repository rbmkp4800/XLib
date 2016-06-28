#pragma once

#include "XLib.Types.h"
#include "XLib.System.FileHandle.h"

class NamedPipe : public ISystemFileHandle
{
public:
	// server
	void create(const wchar* name, bool single, uint32 outBufferSize = 4096, uint32 inBufferSize = 4096);
	void accept();

	// client
	bool connect(const wchar* name);

	// global
	bool read(void* buffer, uint32 size);
	bool write(void* buffer, uint32 size);
	bool asyncRead();
	bool asyncWrite();

	inline void destroy() { this->~NamedPipe(); }
};