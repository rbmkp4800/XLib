#include <Windows.h>

#include "XLib.Types.h"
#include "XLib.System.AsyncIO.h"
#include "XLib.System.Network.Socket.h"

constexpr ULONG_PTR xlibCompletitionKey = 0x786C6962;
constexpr ULONG_PTR xlibTerminateCompletitionKey = 0x786C6963;

void AsyncIOHost::initialize()
{
	static_assert(sizeof(HostedAsyncData::overlapped) == sizeof(OVERLAPPED),
		"XLib.System.AsyncIO.Internal.HostedAsyncData.overlapped size != OVERLAPPED size");

	destroy();
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
}
AsyncIOHost::~AsyncIOHost()
{
	if (hIOCP)
	{
		CloseHandle(hIOCP);
		hIOCP = nullptr;
	}
}

void AsyncIOHost::associate(Socket& socket)
	{ CreateIoCompletionPort(socket.getHandle(), hIOCP, xlibCompletitionKey, 0); }

void AsyncIOHost::dispatchAll()
{
	for (;;)
	{
		DWORD transferredSize = 0;
		ULONG_PTR key = 0;
		HostedAsyncData* asyncData = nullptr;
		BOOL result = GetQueuedCompletionStatus(hIOCP, &transferredSize,
			&key, (OVERLAPPED**)&asyncData, INFINITE);

		if (result == FALSE && !asyncData)
			throw;
		if (key == xlibTerminateCompletitionKey)
			return;
		if (key != xlibCompletitionKey)
			continue;

		switch (asyncData->state)
		{
			case HostedAsyncData::State::SocketReceive:
			case HostedAsyncData::State::SocketSend:
			{
				TransferCompletedHandler handler(asyncData->rawHandler);
				uintptr userKey = asyncData->key;
				asyncData->clear();

				handler.call(result ? true : false, uint32(transferredSize), userKey);
			}
			break;

			case HostedAsyncData::State::SocketAccept:
			{
				TCPListenSocket::ClientAcceptedHandler handler(asyncData->rawHandler);
				uintptr userKey = asyncData->key;
				asyncData->clear();

				TCPListenSocket::HostedAsyncData *acceptAsyncData =
					(TCPListenSocket::HostedAsyncData*)asyncData;
				handler.call(result ? true : false, acceptAsyncData->acceptedSocket,
					acceptAsyncData->extractIPAddress(), userKey);
			}
			break;

			default: continue;
		}
	}
}
//void AsyncIOHost::dispatchPending(){}

void AsyncIOHost::postMessage_socketReceiveCompleted(
	HostedAsyncData& asyncData, uint32 transferredSize, uintptr key)
{
	asyncData.key = key;
	PostQueuedCompletionStatus(hIOCP, transferredSize,
		xlibCompletitionKey, (OVERLAPPED*)&asyncData.overlapped);
}

void AsyncIOHost::quit()
	{ PostQueuedCompletionStatus(hIOCP, 0, xlibTerminateCompletitionKey, nullptr); }