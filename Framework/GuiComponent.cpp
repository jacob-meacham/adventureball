#include "GuiComponent.h"
//////////////////////////////////////////////////////////////////////////////////
GuiComponent::GuiComponent() { }
//////////////////////////////////////////////////////////////////////////////////
GuiComponent::GuiComponent(int x,int y,int w,int h, const char* pLabel) {
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
	m_Visible = false;
	setFont(gGuiMgr.getDefaultFont());
	m_MouseOverComponent = NULL;
	m_FocusComponent = NULL;
	m_LockComponent = NULL;
}
//////////////////////////////////////////////////////////////////////////////////
int GuiComponent::attachChildComponent(GuiComponent *component)
{
	component->setParent(this);
	m_SubComponents.push_back(component);
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
int GuiComponent::insertChildComponent(GuiComponent* component, u32 idx)
{
	if(idx > m_SubComponents.size()) { return 0; }
	//m_SubComponents.insert(component, idx);
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
int GuiComponent::removeChildComponent(GuiComponent *component) 
{
	std::list<GuiComponent*>::iterator iComponent;
	for(iComponent = m_SubComponents.begin(); iComponent != m_SubComponents.end(); iComponent++)
	{
		if((*iComponent)->getLabel().compare(component->getLabel()) == 0)
		{
			m_SubComponents.erase(iComponent);
			return 1;
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
int GuiComponent::removeChildComponent(const char* pname) 
{
	std::list<GuiComponent*>::iterator iComponent;
	for(iComponent = m_SubComponents.begin(); iComponent != m_SubComponents.end(); iComponent++)
	{
		if((*iComponent)->getLabel().compare(pname) == 0)
		{
			m_SubComponents.erase(iComponent);
			return 1;
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
GuiComponent* GuiComponent::GetLock() const
{
	return m_LockComponent;
}
//////////////////////////////////////////////////////////////////////////////////
int GuiComponent::SetLock(GuiComponent* pComponent)
{
	if(m_LockComponent)
		m_LockComponent->HandleEvent(GUI_UNLOCK);
	m_LockComponent = pComponent;
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
int GuiComponent::Unlock()
{
	if(m_LockComponent)
	{
		m_LockComponent->HandleEvent(GUI_UNLOCK);
		m_LockComponent = NULL;
		return 1;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
void GuiComponent::SetTop(GuiComponent* pComponent)
{
	bool found = false;
	std::list<GuiComponent*>::iterator iComponent;
	for(iComponent = m_SubComponents.begin(); iComponent != m_SubComponents.end(); iComponent++)
	{
		if(pComponent == (*iComponent))
		{
			found = true;
			m_SubComponents.erase(iComponent);
			break;
		}
	}
	if(found)
		m_SubComponents.push_back(pComponent);
}
//////////////////////////////////////////////////////////////////////////////////
GuiComponent* GuiComponent::Find(const u32 id) const 
{
	std::list<GuiComponent*>::const_iterator iComponent;
	for(iComponent = m_SubComponents.begin(); iComponent != m_SubComponents.end(); iComponent++)
	{
	}
	return new GuiComponent;
}
//////////////////////////////////////////////////////////////////////////////////
int GuiComponent::FindIndex(const GuiComponent* pComponent) const
{
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////
bool GuiComponent::TestHit(int x,int y) const
{
	RECT compRect;
	getFullRect(compRect);
	if (x<compRect.left) 
		return false;
	if (x>compRect.right)
		return false;
	if (y<compRect.top) 
		return false;
	if (y>compRect.bottom) 
		return false;

  return true;
}
//////////////////////////////////////////////////////////////////////////////////
void GuiComponent::getFullRect(RECT &rect) const
{
	rect.left = m_Location.x;
	rect.top = m_Location.y;
	rect.right = m_Location.x + m_Width;
	rect.bottom = m_Location.y + m_Height;
}
//////////////////////////////////////////////////////////////////////////////////
void GuiComponent::getUsableRect(RECT &rect) const
{
	getFullRect(rect);
}

void GuiComponent::RenderChildren() const
{
	std::list<GuiComponent*>::const_iterator iComponent;
	for(iComponent = m_SubComponents.begin(); iComponent != m_SubComponents.end(); iComponent++)
	{
		(*iComponent)->Render();
	}
}
//////////////////////////////////////////////////////////////////////////////////
void GuiComponent::setPosition(int x, int y)
{
    m_Location.x = x;
    m_Location.y = y;

    std::list<GuiComponent*>::iterator iComponent;
    for(iComponent = m_SubComponents.begin(); iComponent != m_SubComponents.end(); iComponent++)
    {   
        (*iComponent)->setPosition(x, y);
    }
}
//////////////////////////////////////////////////////////////////////////////////
int GuiComponent::HandleEvent(int type)
{
	const GuiEvent* gEvent = gGuiMgr.getCurrentEvent();
	switch(type)
	{
	case GUI_MOUSEMOVE:
		{
		if(m_LockComponent) 
			return m_LockComponent->HandleEvent(type);
		
		GuiComponent *pOver = NULL;
		std::list<GuiComponent *>::reverse_iterator	iComponents;
		for(iComponents = m_SubComponents.rbegin(); iComponents != m_SubComponents.rend(); iComponents++)
		{
			if((*iComponents)->TestHit(gEvent->mouseCurPos.x, gEvent->mouseCurPos.y))
			{
				pOver = (*iComponents);
				break;
			}
		}
		if(pOver)
		{
			if(pOver == m_MouseOverComponent)
			{
				return m_MouseOverComponent->HandleEvent(type);
			}
			else
			{
				if(m_MouseOverComponent)
				{
					m_MouseOverComponent->SetMouseOver(false);
					m_MouseOverComponent->HandleEvent(GUI_LEAVE);
				}
				m_MouseOverComponent = pOver;
				m_MouseOverComponent->SetMouseOver(true);
				m_MouseOverComponent->HandleEvent(GUI_ENTER);
			}
		}
		else
		{
			if(m_MouseOverComponent)
			{
				m_MouseOverComponent->SetMouseOver(false);
				m_MouseOverComponent->HandleEvent(GUI_LEAVE);
			}
			m_MouseOverComponent = NULL;
		}
		}
		break;
	case GUI_LMOUSEBUTTONDOWN:
	case GUI_RMOUSEBUTTONDOWN:
		{
			if(m_MouseOverComponent)
			{
				if(m_MouseOverComponent == m_FocusComponent)
				{
					 m_FocusComponent->HandleEvent(type);
				}
				else
				{
					if(m_FocusComponent)
					{
						m_FocusComponent->setHasFocus(false);
						m_FocusComponent->HandleEvent(GUI_LEAVEFOCUS);
					}
					m_FocusComponent = m_MouseOverComponent;
					m_FocusComponent->setHasFocus(true);
					m_FocusComponent->HandleEvent(GUI_ENTERFOCUS);
					m_FocusComponent->HandleEvent(type);
				}
			}
			else
			{
				if(m_FocusComponent)
				{
					m_FocusComponent->setHasFocus(false);
					m_FocusComponent->HandleEvent(GUI_LEAVEFOCUS);
				}
			}
		}
		break;
	case GUI_LMOUSEBUTTONUP:
	case GUI_RMOUSEBUTTONUP:
		{
			if(m_MouseOverComponent && m_FocusComponent && m_MouseOverComponent == m_FocusComponent)
			{
				m_FocusComponent->HandleEvent(type);
			}
		}
		break;
	case GUI_KEYDOWN:
	case GUI_KEYUP:
		{
			if(m_FocusComponent)
			{
				return m_FocusComponent->HandleEvent(type);
			}
		}
		break;
	case GUI_LEAVE:
		{
			if(m_MouseOverComponent)
			{
				m_MouseOverComponent->SetMouseOver(false); 
				m_MouseOverComponent->HandleEvent(GUI_LEAVE);
				m_MouseOverComponent = NULL;
			}
		}
		break;
	case GUI_LEAVEFOCUS:
		{
			if(m_FocusComponent)
			{ 
				m_FocusComponent->setHasFocus(false);
				m_FocusComponent->HandleEvent(GUI_LEAVEFOCUS);
				m_FocusComponent = NULL;
			}
		}
		break;
	}
	return 0;
}