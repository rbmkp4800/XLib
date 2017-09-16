#pragma once

#include "XLib.Types.h"

namespace XLib
{
	struct Strings abstract final
	{
		static inline uintptr Length(const char* string)
		{
			uintptr length = 0;
			while (*string)
			{
				length++;
				string++;
			}
			return length;
		}
		static inline uintptr Length(const wchar* string)
		{
			uintptr length = 0;
			while (*string)
			{
				length++;
				string++;
			}
			return length;
		}
		static inline uintptr Length(const char* string, uintptr limit)
		{
			uintptr length = 0;
			while (*string && length < limit)
			{
				length++;
				string++;
			}
			return length;
		}
		
		//template <uintptr length> static constexpr uintptr Length(const char(&)[length]) { return length - 1; }
		//template <uintptr length> static constexpr uintptr Length(const wchar(&)[length]) { return length - 1; }

		template <typename Type>
		static inline bool Parse(const char* string, Type& value, const char** end = nullptr) { static_assert("invalid type"); }

		template <>
		static inline bool Parse<sint32>(const char* string, sint32& value, const char** end)
		{
			bool minus = false;
			char c = *string;
			if (c >= '0' && c <= '9')
				goto label_parsingDigits;
			
			if (c == '-')
				minus = true;
			else if (c != '+')
				return nullptr;

			string++;
			c = *string;

		label_parsingDigits:
			sint32 result = c - '0';
			string++;
			c = *string;
			while (c >= '0' && c <= '9')
			{
				result *= 10;
				result += c - '0';

				string++;
				c = *string;
			}

			value = minus ? -result : result;
			if (end)
				*end = string;
			return true;
		}
	};
}