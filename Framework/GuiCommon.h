#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include <list>
#include "DXUtil.h"
#include "GraphicsDevice.h"
#include "System.h"
#include "GuiTexture.h"
#include "Define.h"

enum GuiMessage
{
	GUI_KEYDOWN	= 1,
	GUI_KEYUP,
	GUI_LMOUSEBUTTONDOWN,
	GUI_LMOUSEBUTTONUP,
	GUI_RMOUSEBUTTONDOWN,
	GUI_RMOUSEBUTTONUP,
	GUI_MOUSEMOVE,
	GUI_LOCK,
	GUI_UNLOCK,
	GUI_ENTER,
	GUI_LEAVE,
	GUI_ENTERFOCUS,
	GUI_LEAVEFOCUS,
	GUI_PARENT_RESIZE,
	GUI_PARENT_ATTACH,
	GUI_PARENT_DETACH,
};

struct GuiEvent
{
	int type;
	POINT mouseCurPos;
	POINT mouseDelta;
	int key;
	unsigned short keyboardChar;
	int keyboardFlags;
};

enum KeyboardFlags
{
	KB_SHIFT	= (1 << 0),
	KB_CTRL		= (1 << 1),
	KB_ALT		= (1 << 2),
	KB_CAPS		= (1 << 3),
	KB_NUM		= (1 << 4),
	KB_LAST		= (1 << 20),
};


enum GuiType
{
	GUI_SCREEN = 1,
	GUI_WINDOW,
	GUI_STATIC,
	GUI_UNKNOWN,
};

enum ButtonState
{
	BUTTON_NORMAL = 0,
	BUTTON_HOVER,
	BUTTON_CLICKED,
	BUTTON_LOCK,
};


#define GET_SCAN_CODE(lParam) (LOBYTE(HIWORD(lParam)))