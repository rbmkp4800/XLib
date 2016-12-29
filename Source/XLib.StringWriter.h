#pragma once

#include "XLib.Types.h"

namespace XLib
{
	template <uint8 digitCount = 0, bool leadingZeros = false>
	struct hex
	{
		uint32 value;
		explicit hex(uint32 _value) : value(_value) {}
	};

	static constexpr char endOfString = '\0';

	class StringWriter
	{
	private:
		char *current, *end;

		inline bool _put(char value)
		{
			if (current < end)
			{
				*current = value;
				current++;
				return true;
			}
			return false;
		}

		bool _put(uint32 value);
		bool _put(const char* value);
		bool _putHex(uint32 value, uint8 digitCount, bool leadingZeros);

		template <uint8 digitCount, bool leadingZeros>
		inline bool _put(const hex<digitCount, leadingZeros> value)
			{ return _putHex(value.value, digitCount, leadingZeros); }

	public:
		inline StringWriter(char* buffer, uintptr size)
			: current(buffer), end(buffer + size) {}
		template <uintptr length>
		inline StringWriter(char(&buffer)[length])
			: current(buffer), end(buffer + length) {}

		template <typename ... Types>
		inline bool put(Types ... args)
		{
			bool results[] = { _put(args) ... };
			bool totalResult = true;
			for each (bool result in results)
				totalResult &= result;
			return totalResult;
		}

		inline bool skip(uintptr length)
		{
			current += length;
			if (current > end)
			{
				current = end;
				return false;
			}
			return true;
		}
		inline char* getPosition() { return current; }
		inline void setPosition(char* position) { current = position; }

		inline bool isFull() { return current >= end; }
	};
}