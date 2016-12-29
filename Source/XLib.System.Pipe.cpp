#include <Windows.h>

#include "XLib.System.Pipe.h"
#include "XLib.Debug.h"

using namespace XLib;

void NamedPipe::destroy()
{
	if (handle)
	{
		CloseHandle(handle);
		handle = nullptr;
	}
}

bool NamedPipe::create(const char* name, uint32 outBufferSize, uint32 inBufferSize)
{
	destroy();

	HANDLE _handle = CreateNamedPipeA(name, FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
		PIPE_UNLIMITED_INSTANCES, outBufferSize, inBufferSize, 0, nullptr);
	if (_handle != INVALID_HANDLE_VALUE)
		handle = _handle;
	else
	{
		Debug::LogLastSystemError(SysErrorDbgMsgFmt);
		return false;
	}

	return true;
}

bool NamedPipe::connect()
{
	Debug::CrashConditionOnDebug(!handle, DbgMsgFmt("not initialized"));
	if (!ConnectNamedPipe(handle, nullptr))
	{
		Debug::LogLastSystemError(SysErrorDbgMsgFmt);
		return false;
	}
	return true;
}

bool NamedPipe::asyncConnect(DispatchedAsyncTask& task, NamedPipeConnectedHandler handler)
{
	Debug::CrashConditionOnDebug(!handle, DbgMsgFmt("not initialized"));
	if (!ConnectNamedPipe(handle, (OVERLAPPED*)&task.overlapped))
	{
		Debug::LogLastSystemError(SysErrorDbgMsgFmt);
		return false;
	}
	return true;
}

bool NamedPipe::open(const char* name)
{
	HANDLE _handle = CreateFileA(name, GENERIC_READ | GENERIC_WRITE,
		0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (_handle == INVALID_HANDLE_VALUE)
		return false;

	handle = _handle;
	return true;
}

void NamedPipe::cancelIO()
{
	CancelIoEx(handle, nullptr);
}

bool NamedPipe::read(void* buffer, uint32 size)
{
	DWORD readSize = 0;
	BOOL result = ReadFile(handle, buffer, size, &readSize, nullptr);
	return result != 0;
}

bool NamedPipe::write(const void* buffer, uint32 size)
{
	DWORD writtenSize = 0;
	BOOL result = WriteFile(handle, buffer, size, &writtenSize, nullptr);
	return result != 0;
}

bool NamedPipe::asyncRead(void* buffer, uint32 size, DispatchedAsyncTask& task,
	TransferCompletedHandler handler, uintptr key)
{
	task.rawHandler = handler.toRaw();
	task.key = key;
	task.state = DispatchedAsyncTask::State::Transfer;

	BOOL result = ReadFile(handle, buffer, size, nullptr, (OVERLAPPED*)&task.overlapped);
	if (result == 0 && GetLastError() != ERROR_IO_PENDING)
		return false;
	return true;
}