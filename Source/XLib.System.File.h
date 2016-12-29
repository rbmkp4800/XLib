#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"

namespace XLib
{
	enum class FileAccessMode : uint32
	{
		Read = 0x8000'0000,
		Write = 0x4000'0000,
		ReadWrite = 0xC000'0000,
	};

	enum class FileOpenMode : uint32
	{
		Override = 2,
		OpenExisting = 3,
	};

	enum class FilePosition : uint32
	{
		Begin = 0,
		Current = 1,
		End = 2,
	};

	class File : public NonCopyable
	{
	private:
		void* handle;

	public:
		inline File() : handle(nullptr) {}
		inline ~File() { close(); }

		inline File(File&& that)
		{
			handle = that.handle;
			that.handle = nullptr;
		}
		inline void operator = (File&& that)
		{
			close();
			handle = that.handle;
			that.handle = nullptr;
		}

		bool open(const char* name, FileAccessMode accessMode,
			FileOpenMode openMode = FileOpenMode::OpenExisting);
		void close();

		bool read(void* buffer, uint32 size);
		bool read(void* buffer, uint32 bufferSize, uint32& readSize);
		bool write(const void* buffer, uint32 size);
		void flush();

		template <typename Type>
		inline bool read(Type& value)
		{
			uint32 readSize = 0;
			bool result = read(&value, sizeof(Type), readSize);
			return readSize == sizeof(Type) && result;
		}
		template <typename Type>
		inline bool write(const Type& value)
		{
			return write(&value, sizeof(Type));
		}

		uint64 getSize();
		uint64 getPosition();
		uint64 setPosition(sint64 offset, FilePosition origin = FilePosition::Begin);

		inline bool isInitialized() { return handle ? true : false; }
		inline void* getHandle() { return handle; }
	};
}