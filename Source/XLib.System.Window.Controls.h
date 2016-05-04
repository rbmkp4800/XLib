#pragma once

#include "XLib.System.Window.h"

class Button : public WindowBase
{
public:
	void create(WindowBase& parent, uint16 x, uint16 y, uint16 width, uint16 height, wchar* caption, bool visible = true);
};