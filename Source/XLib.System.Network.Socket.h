#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"
#include "XLib.Delegate.h"
#include "XLib.System.Network.h"
#include "XLib.System.AsyncIO.h"
#include "XLib.System.FileHandle.h"

enum class ProtocolType : uint32
{
	None = 0,
	TCP = 1,	// SOCK_STREAM
	UDP = 2,	// SOCK_DGRAM
};

enum class SocketError : uint32
{
	None = 0,
	SocketsSystemNotInitialized,	// WSANOTINITIALISED
	TimeOut,						// WSAETIMEDOUT
};

struct Sockets abstract final
{
	static bool Startup();
};

using TransferCompletedHandler = Delegate<void, bool, uint32, uintptr>;
using SocketAcceptedHandler = Delegate<void, bool, TCPSocket&, IPAddress, uintptr>;

class Socket abstract : public ISystemFileHandle
{
	friend class TCPSocket;
	friend class TCPListenSocket;
	friend class UDPSocket;

private:
	inline void fromHandle(void* _handle)
	{
		destroy();
		handle = _handle;
	}
	bool initialize(AddressFamily family, ProtocolType type);
	bool bind(IPAddress address, uint16 port);
	void* moveToHandle()
	{
		void* result = handle;
		handle = nullptr;
		return result;
	}

public:
	inline Socket(Socket&& that)
	{
		handle = that.handle;
		that.handle = nullptr;
	}
	inline void operator = (Socket&& that)
	{
		destroy();
		handle = that.handle;
		that.handle = nullptr;
	}

	inline void* getHandle() { return handle; }
	inline bool isInitialized() { return handle ? true : false; }
};

class TCPSocket : public Socket
{
public:
	inline bool initialize(AddressFamily family) { return Socket::initialize(family, ProtocolType::TCP); }

	bool connect(IPAddress address, uint16 port);
	bool send(void* buffer, uint32 size);
	bool receive(void* buffer, uint32 bufferSize, uint32& receivedSize);
	bool receiveAll(void* buffer, uint32 size);

	//bool asyncConnect();
	void asyncSend(void* buffer, uint32 size, HostedAsyncTask& task,
		TransferCompletedHandler handler, uintptr key = 0);
	void asyncReceive(void* buffer, uint32 size, HostedAsyncTask& task,
		TransferCompletedHandler handler, bool waitAll = false, uintptr key = 0);
};

class TCPListenSocket : public Socket
{
public:
	class HostedAsyncTask : public ::HostedAsyncTask
	{
		friend AsyncIOHost;
		friend TCPListenSocket;

	private:
		byte acceptData[64];
		TCPSocket acceptedSocket;

		IPAddress extractIPAddress();

		inline void clear()
			{ Memory::Set(this, 0, sizeof(*this)); }
	};

	inline bool initialize(IPAddress address, uint16 port)
	{
		if (!Socket::initialize(address.family, ProtocolType::TCP))
			return false;
		if (!Socket::bind(address, port))
			return false;
		return true;
	}
	bool start(uint32 backlog = 5);

	bool accept(TCPSocket& socket, IPAddress& address);
	void asyncAccept(TCPListenSocket::HostedAsyncTask& task,
		SocketAcceptedHandler handler, uintptr key = 0);
};

class UDPSocket : public Socket
{
public:
	inline bool initialize(AddressFamily family) { return Socket::initialize(family, ProtocolType::UDP); }

	bool sendTo(void* data, uint32 size, IPAddress address, uint16 port);
	bool receiveFrom(void* buffer, uint32 bufferSize, uint32& datagramLength, IPAddress& address, uint16& port);
	void asyncSendTo(void* data, uint32 size, IPAddress address, uint16 port, HostedAsyncTask& task);
	void asyncReceiveFrom(HostedAsyncTask& task);
};