#include <Windows.h>

#include "XLib.Application.h"

int main()
{
	return Application::Run();
}

int __stdcall WinMain(HINSTANCE, HINSTANCE, char*, int)
{
	return Application::Run();
}