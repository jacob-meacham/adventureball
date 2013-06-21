#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "Framework\DXUtil.h"
#include "Framework\GraphicsDevice.h"
#include "Framework\System.h"
#include "Level.h"
#include "Framework\Sprite.h"
#include "Framework\Define.h"
#include "Framework\Physics_Core.h"
#include "Parser\tinyxml\ticpp.h"
#include "Parser\tinyxml\tinyxml.h"
#include "Wall.h"

#include "Framework\GuiMgr.h"
#include "Framework\GuiScreen.h"
#include "Framework\GuiWindow.h"
#include "Framework\GuiStatic.h"
#include "Framework\GuiButton.h"

Player player;

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
		std::stringstream s;

		GuiScreen*   m_pScreen;
		GuiWindow*   m_pWindow;
		GuiWindow*	 m_pSecondWindow;
		GuiWindow*   m_pInnerWindow;
		GuiStatic*	 m_pStatic;
		GuiButton*	 m_RegularButton;
		GuiButton*	 m_BigButton;
		GuiButton*	 m_LockedButton;

		virtual void onProcess();
		virtual void shutdown();

		bool processInput();

	public:
		Prototype();
		~Prototype() { shutdown(); }
		virtual bool onInit();

		virtual LRESULT CALLBACK MsgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};