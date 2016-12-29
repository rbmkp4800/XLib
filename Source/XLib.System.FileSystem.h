#pragma once

#include "XLib.Types.h"

#undef DeleteFile

namespace XLib
{
	struct FileSystem abstract final
	{
		static bool DeleteFile(const char* name);
		static bool FileExists(const char* filename);
	};
}