#pragma once

#include "GuiCommon.h"
#include "GuiComponent.h"

class GuiScreen : public GuiComponent{
protected:
	
public:
	GuiScreen(short x,short y,short w,short h,const char * pLabel = 0) : GuiComponent(x, y, w, h, pLabel)
	{
		m_Location.x = x;
		m_Location.y = y;
		m_Width = w;
		m_Height = h;
		if(pLabel)
			m_Label = pLabel;
	
		m_Parent = NULL;
		m_Type = GUI_UNKNOWN;
		m_Flags = 0;
	
		m_Focus = false;
		m_MouseOver = false;
		//m_Visible = false;
		setFont(gGuiMgr.getDefaultFont());
	}
	virtual void Render() { RenderChildren(); }
	virtual int  HandleEvent(int type) { return GuiComponent::HandleEvent(type); }
	virtual int  CommandHandle(GuiComponent *component, int cmd, int param) { return 0; }

};