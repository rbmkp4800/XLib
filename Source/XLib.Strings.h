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
	};
}