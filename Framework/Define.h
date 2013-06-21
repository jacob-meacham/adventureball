#ifndef _Define_h_included_
#define _Define_h_included_
#include <string>
#include <windows.h>
#include <assert.h>

#define LEAN_AND_MEAN

// SCREEN
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREENCENTERX 320
#define SCREENCENTERY 240

#define DX_RELEASE(x) if(x) { x->Release(); x = NULL; }

#define KEY_RELEASED       FALSE
#define KEY_PRESSED         TRUE

#define KEY_UNLOCKED       FALSE
#define KEY_LOCKED		    TRUE

#define BUTTON_RELEASED    FALSE
#define BUTTON_PRESSED      TRUE

#define BUTTON_UNLOCKED    FALSE
#define BUTTON_LOCKED       TRUE

#define MOUSE_LBUTTON          0
#define MOUSE_RBUTTON          1
#define MOUSE_MBUTTON          2

enum InputDevices {
  NONE = 0,
  KEYBOARD,
  MOUSE
};

#define ASSERT(x) assert(x)
#define ASSERT_FAILED assert(0)

#ifdef _DEBUG
static void TRACE(const char * pformat, ...)
{
	char buf[4096], *p = buf;
	
	va_list args;
    va_start(args, pformat);
    int n = vsnprintf_s(p, sizeof(buf) - 1, sizeof(buf) - 1, pformat, args);
    va_end(args);

	if(n < 0)
	{
		p += sizeof(buf) - 1;
		p = '\0';
	}
    
	OutputDebugString(buf);
}
#else
static void TRACE(const char * pformat, ...) { }
#endif

#endif;