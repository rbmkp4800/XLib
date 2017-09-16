#pragma once

#include "XLib.Types.h"

namespace XLib
{
	class Strings abstract final
	{
	private:
		static bool ParseSInt32(const char* string, sint32& value, const char** end);
		static bool ParseFloat32(const char* string, float32& value, const char** end);

	public:
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
		static bool Parse(const char* string, Type& value, const char** end = nullptr) { static_assert("invalid type"); }

		template <> static inline bool Parse<sint32>(const char* string, sint32& value, const char** end) { return ParseSInt32(string, value, end); }
		template <> static inline bool Parse<float32>(const char* string, float32& value, const char** end) { return ParseFloat32(string, value, end); }
	};
}