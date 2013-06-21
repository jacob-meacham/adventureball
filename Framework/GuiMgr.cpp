#include "GuiMgr.h"
#include "System.h"
#include "Texture.h"

GuiMgr gGuiMgr;
//////////////////////////////////////////////////////////////////////////////////
GuiMgr::~GuiMgr() {
	delete m_Tiles;
	m_Tiles = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
void GuiMgr::RenderGui() { m_pActiveScreen->Render(); }
//////////////////////////////////////////////////////////////////////////////////
void GuiMgr::SetActiveComponent(GuiComponent* pActive) 
{ 
	m_pActiveScreen = pActive; 
	m_Active = true;
}
//////////////////////////////////////////////////////////////////////////////////
void GuiMgr::SendEvent(int type, int key, int scanCode, int x, int y)
{
	POINT pointer = {x, y};

	m_CurrentEvent.type = type;
	m_CurrentEvent.keyboardChar = 0;
	m_CurrentEvent.keyboardFlags = 0;
	m_CurrentEvent.key = key;

	if(pointer.x != -1)
	{
		POINT mousePrevPos = m_CurrentEvent.mouseCurPos;
		m_CurrentEvent.mouseCurPos = pointer;
		m_CurrentEvent.mouseDelta.x = m_CurrentEvent.mouseCurPos.x - mousePrevPos.x;
		m_CurrentEvent.mouseDelta.y = m_CurrentEvent.mouseCurPos.y - mousePrevPos.y;
	}

	if(type == GUI_LMOUSEBUTTONDOWN || type == GUI_RMOUSEBUTTONDOWN)
	{
		//SetCapture(g_SystemCore->GetHWnd());
	}
	else if(type == GUI_LMOUSEBUTTONUP || type == GUI_RMOUSEBUTTONUP)
	{
	//	ReleaseCapture();
	}

	unsigned char keyState[256];
	GetKeyboardState(keyState);

	if(keyState[VK_SHIFT]   & 0x80) m_CurrentEvent.keyboardFlags |= KB_SHIFT;
	if(keyState[VK_CONTROL] & 0x80) m_CurrentEvent.keyboardFlags |= KB_CTRL;
	if(keyState[VK_MENU]    & 0x80) m_CurrentEvent.keyboardFlags |= KB_ALT;
	if(keyState[VK_CAPITAL] & 0x1)  m_CurrentEvent.keyboardFlags |= KB_CAPS;
	if(keyState[VK_NUMLOCK] & 0x1)  m_CurrentEvent.keyboardFlags |= KB_NUM;

	if(type == GUI_KEYDOWN || type==GUI_KEYUP)
	{
		ToAscii(key, scanCode, keyState, &m_CurrentEvent.keyboardChar, 0);
	}
	if(m_pActiveScreen) 
		m_pActiveScreen->HandleEvent(m_CurrentEvent.type);	
}
//////////////////////////////////////////////////////////////////////////////////