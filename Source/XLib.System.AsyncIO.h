#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"
#include "XLib.Delegate.h"
#include "XLib.Memory.h"

namespace XLib
{
	class AsyncIODispatcher;

	using TransferCompletedHandler = Delegate<void, bool, uint32, uintptr>;
	using CustomHandler = Delegate<void>;

	class Socket;
	class TCPSocket;
	class TCPListenSocket;
	class NamedPipe;

	class DispatchedAsyncTask : public NonCopyable
	{
		friend AsyncIODispatcher;

		friend TCPSocket;
		friend TCPListenSocket;
		friend NamedPipe;

	private:
		enum class State : uint32
		{
			None = 0,
			Transfer = 0xA1B2C301,
			SocketAccept = 0xA1B2C302,
			//SocketConnect = 0xA1B2C304,
		};

		static constexpr uint32 overlappedSize = sizeof(void*) == 4 ? 20 : 32;

		byte overlapped[overlappedSize] = { 0 };	// must be first
		RawDelegate rawHandler;
		uintptr key = 0;
		State state = State::None;

		inline void clear()
			{ Memory::Set(this, 0, sizeof(*this)); }

	public:
		void cancel();

		inline bool isActive() const { return state != State::None; }
	};

	class AsyncIODispatcher : public NonCopyable
	{
	private:
		void *hIOCP;

	public:
		inline AsyncIODispatcher() : hIOCP(nullptr) {}
		~AsyncIODispatcher();

		void initialize();
		inline void destroy() { this->~AsyncIODispatcher(); }

		void associate(Socket& socket);
		void associate(NamedPipe& pipe);

		void dispatchAll();
		void dispatchPending();
		void invokeShutdown();

		void invoke(CustomHandler handler);

		inline bool isInitialized() { return hIOCP != nullptr; }
	};
}