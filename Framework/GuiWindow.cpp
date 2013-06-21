#include "GuiWindow.h"
GuiWindow::GuiWindow(int x,int y,int width,int height, const char* pLabel) : GuiComponent(x,y,width,height,pLabel) 
{ 
	m_bCanDrag = false;
	m_bCanResize = false;
	m_TitleBar = NULL;
	m_Background = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
void GuiWindow::setGraphics(int titleBarTN, int backgroundTN) {
    m_TitleBar = new GuiTexture();
    m_Background = new GuiTexture();

	m_TitleBar->UseTiles(gGuiMgr.m_Tiles, titleBarTN);
	m_TitleBar->Create(0, m_Location.x, m_Location.y, m_Width, WINDOW_TITLEBAR);

	m_Background->UseTiles(gGuiMgr.m_Tiles, backgroundTN);
	m_Background->Create(0, m_Location.x, m_Location.y + WINDOW_TITLEBAR, m_Width, m_Height - WINDOW_TITLEBAR);
}
//////////////////////////////////////////////////////////////////////////////////
int GuiWindow::HandleEvent(int type)
{
	std::string s;
	switch(type)
	{
	case GUI_ENTER: { } break;
	case GUI_LEAVE:
		{
			if(m_bCanDrag && getParent())
			{
				getParent()->Unlock();
			}
			m_bCanDrag = false;
			m_bCanResize = false;
		}
		break;
	case GUI_RMOUSEBUTTONDOWN:
		{
			if(getParent())
				getParent()->SetTop(this);
		}
		break;
	case GUI_LMOUSEBUTTONDOWN:
		{
			s = ("Hitting left button on " + getLabel());
			TRACE(s.c_str());
			const GuiEvent* geEvent;
			geEvent = gGuiMgr.getCurrentEvent();
			if(getParent())
			{
				getParent()->SetTop(this);
			}
			if(geEvent->mouseCurPos.y > m_Location.y &&
				geEvent->mouseCurPos.y < m_Location.y + WINDOW_TITLEBAR)
				m_bCanDrag = true;
			if(m_bCanDrag && getParent())
			{				
				getParent()->SetLock(this);
				return 1;
			}
		}
		break;
	case GUI_LMOUSEBUTTONUP:
		{
			s = ("left button up on " + getLabel());
			OutputDebugString(s.c_str());
			if(m_bCanDrag)
			{
				m_bCanDrag = false;
				if(getParent())
					getParent()->Unlock();
				return 1;
			}
		}
		break;
	case GUI_MOUSEMOVE:
		{
			if(m_bCanDrag)
			{
				const GuiEvent* curEvent;
				curEvent = gGuiMgr.getCurrentEvent();
				RECT parentRect;
				POINT componentSize;

				getParent()->getUsableRect(parentRect);
				componentSize = getSize();

				int curX = m_Location.x + curEvent->mouseDelta.x;
				int curY = m_Location.y + curEvent->mouseDelta.y;

				if(curX < parentRect.left) 
					curX = parentRect.left + 1;
				else if(curX + componentSize.x > parentRect.right) 
					curX = parentRect.right - componentSize.x - 1;

				if(curY < parentRect.top) 
					curY = parentRect.top + 1;
				else if(curY + componentSize.y > parentRect.bottom) 
					curY = parentRect.bottom - componentSize.y - 1;

				setPos(curX, curY);
				
				return 1;
			}
		}
		break;
	}
		return GuiComponent::HandleEvent(type);
}
//////////////////////////////////////////////////////////////////////////////////
void GuiWindow::setPos(int x, int y) 
{
	LONG dx = x - m_Location.x;
	LONG dy = y - m_Location.y;
	m_Background->SetXYPos(x, y + WINDOW_TITLEBAR);
	m_TitleBar->SetXYPos(x,y);

	std::list<GuiComponent*>::iterator iComponents;
	for(iComponents = m_SubComponents.begin(); iComponents != m_SubComponents.end(); iComponents++)
	{
			POINT pos = (*iComponents)->getPos();
			(*iComponents)->setPos(pos.x + dx, pos.y + dy);
			
	}
	GuiComponent::setPos(x, y);
}
//////////////////////////////////////////////////////////////////////////////////
void GuiWindow::Render() const
{
	m_Background->Render();
	m_TitleBar->Render();
	if(getRenderLabel())
		gGuiMgr.getDefaultFont()->render((char*)getLabel().c_str(), 
											m_Location.x + 10, 
											m_Location.y + 3, 
											m_Width, 
											WINDOW_TITLEBAR);
	GuiComponent::Render();
}
//////////////////////////////////////////////////////////////////////////////////
void GuiWindow::getUsableRect(RECT &rect) const
{
	rect.top = m_Location.y + WINDOW_TITLEBAR;
	rect.left = m_Location.x;
	rect.bottom = m_Location.y + m_Height;
	rect.right = m_Location.x + m_Width;
}
//////////////////////////////////////////////////////////////////////////////////