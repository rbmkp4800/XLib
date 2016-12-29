#include <Windows.h>

#include "XLib.System.FileSystem.h"

using namespace XLib;

bool FileSystem::DeleteFile(const char* name) { return DeleteFileA(name) ? true : false; }
bool FileSystem::FileExists(const char* name)
{
	DWORD attributes = GetFileAttributesA(name);
	return attributes != INVALID_FILE_ATTRIBUTES &&
		(attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}