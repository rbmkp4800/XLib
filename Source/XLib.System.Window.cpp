#include <Windows.h>
#include <WindowsX.h>

#include "XLib.System.Window.h"

static constexpr wchar windowClassName[] = L"XLib.Window";

namespace _Private
{
	class _WindowInternal abstract final
	{
	public:
		static LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			Window *window = nullptr;
			if (message == WM_NCCREATE)
			{
				CREATESTRUCT *createStruct = (CREATESTRUCT*)lParam;
				window = (Window*)createStruct->lpCreateParams;
				SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(window));
			}
			else
			{
				window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			}

			switch (message)
			{
				case WM_CREATE:
				{
					window->handle = hWnd;
					RECT clientRect = { 0 };
					GetClientRect(hWnd, &clientRect);
					window->onCreate(uint16(clientRect.right - clientRect.left),
						uint16(clientRect.bottom - clientRect.top));
				}
					break;

				case WM_DESTROY:
					window->onDestroy();
					break;

				case WM_SIZE:
					window->onResize(LOWORD(lParam), HIWORD(lParam));

				case WM_KEYDOWN:
					window->onKeyDown(VirtualKey(wParam));
					break;

				case WM_KEYUP:
					window->onKeyUp(VirtualKey(wParam));
					break;

				case WM_MOUSEMOVE:
					window->onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
						wParam & MK_LBUTTON, wParam & MK_MBUTTON, wParam & MK_RBUTTON);
					break;

				case WM_LBUTTONDOWN:
					window->onMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MouseButton::Left,
						wParam & MK_LBUTTON, wParam & MK_MBUTTON, wParam & MK_RBUTTON);
					break;

				case WM_LBUTTONUP:
					window->onMouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MouseButton::Left,
						wParam & MK_LBUTTON, wParam & MK_MBUTTON, wParam & MK_RBUTTON);
					break;

				case WM_RBUTTONDOWN:
					window->onMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MouseButton::Right,
						wParam & MK_LBUTTON, wParam & MK_MBUTTON, wParam & MK_RBUTTON);
					break;

				case WM_RBUTTONUP:
					window->onMouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MouseButton::Right,
						wParam & MK_LBUTTON, wParam & MK_MBUTTON, wParam & MK_RBUTTON);
					break;

				case WM_MOUSEWHEEL:
					window->onMouseWheel(float32(GET_WHEEL_DELTA_WPARAM(wParam)) / float32(WHEEL_DELTA));
					break;

				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
					break;
			}

			return 0;
		}
	};
}

Window::Window() : handle(nullptr) {}
Window::~Window()
{
	if (handle)
	{
		destroy();
		dispatchAll();
		handle = nullptr;
	}
}

void Window::create(uint32 width, uint32 height, wchar* title)
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	static bool windowClassRegistered = false;
	if (!windowClassRegistered)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = _Private::_WindowInternal::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = windowClassName;
		wcex.hIconSm = nullptr;
		RegisterClassEx(&wcex);
	}

	RECT rect = { 0, 0, LONG(width), LONG(height) };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	CreateWindow(windowClassName, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, this);
}
void Window::show()
{
	ShowWindow(HWND(handle), SW_SHOW);
}
void Window::hide()
{
	ShowWindow(HWND(handle), SW_HIDE);
}
void Window::destroy()
{
	DestroyWindow(HWND(handle));
}

bool Window::dispatchPending()
{
	if (handle)
	{
		MSG message = { 0 };
		while (PeekMessage(&message, HWND(handle), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		if (!IsWindow(HWND(handle)))
		{
			handle = nullptr;
			return false;
		}
		return true;
	}
	return false;
}
void Window::dispatchAll()
{
	if (handle)
	{
		MSG message = { 0 };
		for (;;)
		{
			BOOL result = GetMessage(&message, HWND(handle), 0, 0);
			if (result == 0)
				break;
			if (result == -1)
			{
				if (!IsWindow(HWND(handle)))
					handle = nullptr;
				break;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}