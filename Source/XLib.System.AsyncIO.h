#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"
#include "XLib.Delegate.h"
#include "XLib.Memory.h"
#include "XLib.System.FileHandle.h"

class HostedAsyncTask : public NonCopyable
{
	friend class AsyncIOHost;

	friend class TCPSocket;
	friend class TCPListenSocket;

private:
	enum class State : uint32
	{
		None = 0,
		SocketReceive = 0xA1B2C301,
		SocketSend = 0xA1B2C302,
		SocketAccept = 0xA1B2C303,
		//SocketConnect = 0xA1B2C304,
	};

	byte overlapped[20] = { 0 };	// must be first
	RawDelegate rawHandler;
	uintptr key = 0;
	State state = State::None;

	inline void clear()
		{ Memory::Set(this, 0, sizeof(*this)); }

public:
	void cancel();
};

class AsyncIOHost : public NonCopyable
{
private:
	void *hIOCP;

public:
	inline AsyncIOHost() : hIOCP(nullptr) {}
	~AsyncIOHost();

	void initialize();
	inline void destroy() { this->~AsyncIOHost(); }

	void associate(ISystemFileHandle& handle);

	void dispatchAll();
	void dispatchPending();
	void quit();

	void postMessage_socketReceiveCompleted(HostedAsyncTask& asyncTask,
		uint32 transferredSize, uintptr key);

	inline bool isInitialized() { return hIOCP ? true : false; }
};