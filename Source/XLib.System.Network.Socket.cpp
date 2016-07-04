#include <WinSock2.h>
#include <Mswsock.h>

#include "XLib.System.Network.Socket.h"

#include "XLib.Util.h"

static_assert(uint32(ProtocolType::TCP) == SOCK_STREAM, "XLib.System.Network.Socket.ProtocolType::TCP != SOCK_STREAM");
static_assert(uint32(ProtocolType::UDP) == SOCK_DGRAM, "XLib.System.Network.Socket.ProtocolType::UDP != SOCK_DGRAM");

inline sockaddr_in IPAddressToSockaddrIn(IPAddress address, uint16 port)
{
	sockaddr_in addr = {};
	addr.sin_family = ADDRESS_FAMILY(AddressFamily::IPv4);
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = address.address.ipv4.address;
	return addr;
}
inline IPAddress SockaddrInToIPAddress(sockaddr_in addr)
{
	IPAddress address;
	address.family = AddressFamily::IPv4;
	address.address.ipv4.address = addr.sin_addr.s_addr;
	return address;
}

inline WSABUF WSABuf(void* buffer, uint32 size)
{
	WSABUF buf;
	buf.buf = (CHAR*)buffer;
	buf.len = size;
	return buf;
}

//------------------------------------------------------------------------//
//-------------------------------- Socket --------------------------------//

bool Sockets::Startup()
{
	WSAData wsaData = {};
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	return true;
}

bool Socket::initialize(AddressFamily family, ProtocolType type)
{
	static_assert(sizeof(handle) == sizeof(SOCKET),
		"XLib.System.Network.Socket.handle size != SOCKET size");

	destroy();

	void *newHandle = (void*)WSASocketW(int(family), int(type), IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	if (SOCKET(newHandle) == INVALID_SOCKET)
		return false;

	handle = newHandle;
	return true;
}

bool Socket::bind(IPAddress address, uint16 port)
{
	sockaddr_in addr = IPAddressToSockaddrIn(address, port);
	int result = ::bind(SOCKET(handle), (sockaddr*)&addr, sizeof(addr));
	return result != SOCKET_ERROR;
}

//------------------------------------------------------------------------//
//------------------------------- TCPSocket ------------------------------//

bool TCPSocket::connect(IPAddress address, uint16 port)
{
	sockaddr_in destAddr = IPAddressToSockaddrIn(address, port);
	return ::connect(SOCKET(handle), (sockaddr*)&destAddr, sizeof(destAddr)) == 0;
}
bool TCPSocket::send(void* buffer, uint32 size)
{
	DWORD transferredSize = 0;
	int result = WSASend(SOCKET(handle), &WSABuf(buffer, size), 1,
		&transferredSize, 0, nullptr, nullptr);
	if (transferredSize != size)
		return false;
	return result == 0;
}
bool TCPSocket::receiveAll(void* buffer, uint32 size)
{
	DWORD transferredSize = 0;
	DWORD flags = MSG_WAITALL;
	int result = WSARecv(SOCKET(handle), &WSABuf(buffer, size), 1,
		&transferredSize, &flags, nullptr, nullptr);
	if (transferredSize != size)
		return false;
	return result == 0;
}

void TCPSocket::asyncSend(void* buffer, uint32 size, HostedAsyncTask& asyncTask,
	TransferCompletedHandler handler, uintptr key)
{
	asyncData.rawHandler = handler.toRaw();
	asyncData.key = key;
	asyncData.state = HostedAsyncData::State::SocketSend;

	int result = WSASend(SOCKET(handle), &WSABuf(buffer, size), 1,
		nullptr, 0, (OVERLAPPED*)&asyncData.overlapped, nullptr);

	if (result && WSAGetLastError() != WSA_IO_PENDING)
	{
		handler.call(false, 0, key);
	}
}
void TCPSocket::asyncReceive(void* buffer, uint32 size, HostedAsyncData& asyncData,
	TransferCompletedHandler handler, bool waitAll, uintptr key)
{
	asyncData.rawHandler = handler.toRaw();
	asyncData.key = key;
	asyncData.state = HostedAsyncData::State::SocketReceive;

	DWORD flags = waitAll ? MSG_WAITALL : 0;
	int result = WSARecv(SOCKET(handle), &WSABuf(buffer, size), 1,
		nullptr, &flags, (OVERLAPPED*)&asyncData.overlapped, nullptr);

	if (result && WSAGetLastError() != WSA_IO_PENDING)
	{
		handler.call(false, 0, key);
	}
}

//------------------------------------------------------------------------//
//---------------------------- TCPListenSocket ---------------------------//

bool TCPListenSocket::start(uint32 backlog)
{
	return ::listen(SOCKET(handle), backlog) != SOCKET_ERROR;
}
bool TCPListenSocket::accept(TCPSocket& socket, IPAddress& address)
{
	sockaddr_in remoteSockaddr = {};
	int remoteSockaddrLength = sizeof(remoteSockaddr);
	SOCKET hAcceptedSocket = ::accept(SOCKET(handle),
		(sockaddr*)&remoteSockaddr, &remoteSockaddrLength);
	if (hAcceptedSocket == INVALID_SOCKET)
		return false;

	socket.fromHandle(handle);
	address = SockaddrInToIPAddress(remoteSockaddr);

	return true;
}
void TCPListenSocket::asyncAccept(TCPListenSocket::HostedAsyncData& asyncData,
	ClientAcceptedHandler handler, uintptr key)
{
	static_assert(sizeof(asyncData.acceptData) == sizeof(sockaddr_in) * 2 + 32,
		"XLib.System.Network.TCPListenSocket.HostedAsyncData.acceptData size != sizeof(sockaddr_in) * 2 + 32");

	asyncData.rawHandler = handler.toRaw();
	asyncData.key = key;
	asyncData.state = HostedAsyncData::State::SocketAccept;
	asyncData.acceptedSocket.initialize(AddressFamily::IPv4);

	AcceptEx(SOCKET(handle), SOCKET(asyncData.acceptedSocket.getHandle()), asyncData.acceptData,
		0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, nullptr, (OVERLAPPED*)&asyncData.overlapped);
}

IPAddress TCPListenSocket::HostedAsyncData::extractIPAddress()
{
	sockaddr_in *localSockaddr = nullptr, *remoteSockaddr = nullptr;
	int localSockaddrLength = 0, remoteSockaddrLength = 0;
	GetAcceptExSockaddrs(acceptData, 0,
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
		(sockaddr**)&localSockaddr, &localSockaddrLength,
		(sockaddr**)&remoteSockaddr, &remoteSockaddrLength);
	return SockaddrInToIPAddress(*remoteSockaddr);
}

//------------------------------------------------------------------------//
//------------------------------- UDPSocket ------------------------------//