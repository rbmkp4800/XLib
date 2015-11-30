#pragma once

#include "XLib.Types.h"

enum class VirtualKey : uint32
{
	Escape = 0x1b,
	Backspace = 0x08,
	MouseLeftButton = 0x01,
	MouseMiddleButton = 0x04,
	MouseRightButton = 0x02,
	Left = 0x25,
	Up = 0x26,
	Right = 0x27,
	Down = 0x28,
};

enum class MouseButton : uint32
{
	Left = 1,
	Middle = 2,
	Right = 3,
};

namespace _Private
{
	class _WindowInternal;
}

class Window
{
	friend _Private::_WindowInternal;

private:
	void *handle;

public:
	Window();
	~Window();

	void create(uint32 width, uint32 height, wchar* title);
	void show();
	void hide();
	void destroy();

	bool dispatchPending();
	void dispatchAll();

	inline void* getHandle() const { return handle; }
	inline bool isInitialized() const { return handle ? true : false; }

protected:
	virtual void onCreate(uint16 width, uint16 height) {}
	virtual void onDestroy() {}
	virtual void onResize(uint16 width, uint16 height) {}
	virtual void onKeyDown(VirtualKey key) {}
	virtual void onMouseMove(sint16 x, sint16 y, bool leftButton, bool middleButton, bool rightButton) {}
	virtual void onMouseUp(sint16 x, sint16 y, MouseButton button, bool leftButton, bool middleButton, bool rightButton) {}
	virtual void onMouseDown(sint16 x, sint16 y, MouseButton button, bool leftButton, bool middleButton, bool rightButton) {}
	virtual void onMouseWheel(float32 delta) {}
};