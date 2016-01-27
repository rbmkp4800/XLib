#pragma once

#include "XLib.Types.h"
#include "XLib.NonCopyable.h"

enum class VirtualKey : uint8
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
	Space = 0x20,
};
enum class MouseButton : uint8
{
	Left = 1,
	Middle = 2,
	Right = 3,
};

struct CreationArgs
{
	uint16 width, height;
};
struct ResizingArgs
{
	uint16 width, height;
};
struct KeyEventArgs
{
	VirtualKey key;
};
struct MouseState
{
	sint16 x, y;
	bool leftButton, middleButton, rightButton;
};

namespace XLib_Internal
{
	class WindowInternal;
}

class Window : public NonCopyable
{
	friend XLib_Internal::WindowInternal;

private:
	void *handle;

protected:
	virtual void onCreate(CreationArgs& args) {}
	virtual void onRedraw() {}
	virtual void onDestroy() {}
	virtual void onResize(ResizingArgs& args) {}
	virtual void onKeyDown(KeyEventArgs& args) {}
	virtual void onKeyUp(KeyEventArgs& args) {}
	virtual void onMouseMove(MouseState& state) {}
	virtual void onMouseButtonDown(MouseState& state, MouseButton button) {}
	virtual void onMouseButtonUp(MouseState& state, MouseButton button) {}
	virtual void onMouseWheel(MouseState& state, float32 delta) {}

public:
	Window();
	~Window();

	void create(uint16 width, uint16 height, wchar* title);
	void show();
	void hide();
	void destroy();

	bool dispatchPending();
	void dispatchAll();

	inline void* getHandle() const { return handle; }
};