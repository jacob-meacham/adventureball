#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "Framework\Define.h"
#include "Framework\System.h"
#include "Framework\Texture.h"
#include "Framework\Font.h"
#include "Framework\Input.h"
#include "Ball.h"

class Prototype : public System
{
	private:
		Tile			m_Tiles; ///< The tile set to use.
		Font			mainFont; ///< The game's main font.
		DWORD			g_dwLastTick; ///< Time of last tick.
		DWORD			g_dCurTime; ///< Current system time.
		DWORD			g_dLastTime; ///< Time at the beginning of last tick.
		float			g_fAnimationTimer; ///< Timer to determine when to update/render the game.
		float			g_fElpasedTime; ///< Time elapsed since last render.
		float           m_rotation;
		Ball			m_Ball;
		int				m_fps;
		bool			m_printfps;
		std::stringstream fpsString;

		Input			m_Keyboard;
		Input			m_Mouse;

		virtual void onProcess();
		virtual void shutdown();

		void initGui();
		bool processInput();

	public:
		Prototype();
		~Prototype() { shutdown(); }
		virtual bool onInit();

		virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};