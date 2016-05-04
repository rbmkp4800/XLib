#include <Windows.h>

#include "XLib.System.Window.Controls.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

void Button::create(WindowBase& parent, uint16 x, uint16 y, uint16 width, uint16 height, wchar* caption, bool visible)
{
	/*CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"OK",      // Button text 
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		10,         // y position 
		80,        // Button width
		40,        // Button height
		HWND(parent.getHandle()),     // Parent window
		NULL,       // No menu.
		GetModuleHandle(nullptr),
		this);      // Pointer not needed.
	*/
}