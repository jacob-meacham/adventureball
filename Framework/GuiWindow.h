#pragma once

#include "GuiCommon.h"
#include "GuiComponent.h"
#define WINDOW_TITLEBAR 20
class GuiWindow : public GuiComponent 
{
protected:
	GuiTexture*        m_TitleBar;
	GuiTexture*        m_Background;
	bool				m_bCanDrag;
	bool				m_bCanResize;
	
public:
	GuiWindow(int x,int y,int width,int height, const char* pLabel=0);
	virtual ~GuiWindow() { }

	void setGraphics(int titleBarTN, int backgroundTN);

	virtual int  HandleEvent(int type);
	virtual void Render();
	virtual void setPos(int x, int y);
	//virtual void Move(float x, float y);
	virtual void getUsableRect(RECT &rect);
};
