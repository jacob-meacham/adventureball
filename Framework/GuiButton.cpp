#include "GuiButton.h"
GuiButton::GuiButton() { m_CurrentTextureNum = 0; m_ButtonTiles = NULL; }
GuiButton::GuiButton(int x, int y, int width, int height, const char* text) 
: GuiComponent(x, y, width, height, text) {
    m_CurrentTextureNum = 0;
    m_ButtonTiles = NULL;
	m_Locked = false;
}

GuiButton::~GuiButton()
{
    if(m_ButtonTiles)
    {
        m_ButtonTiles->Free();
        delete m_ButtonTiles;
        m_ButtonTiles = NULL;
    }
}
void GuiButton::setLocked(bool lock)
{ 
	m_Locked = lock;
	if(m_Locked && m_ButtonTiles)
	{
		m_ButtonTiles->setTileNum(BUTTON_LOCK);
	}
}


void GuiButton::setGraphics(int textureNum)
{
    m_ButtonTiles = new GuiTexture();
	m_ButtonTiles->UseTiles(gGuiMgr.m_Tiles, textureNum);
	m_ButtonTiles->Create(BUTTON_NORMAL, m_Location.x, m_Location.y, m_Width, m_Height);

	if(m_Locked)
        m_ButtonTiles->setTileNum(BUTTON_LOCK);
}

int GuiButton::HandleEvent(int type)
{
    if(m_Locked)
        return 0;
    switch(type)
    {
    case GUI_ENTER:
        {
            if(m_ButtonTiles)
               m_ButtonTiles->setTileNum(BUTTON_HOVER);
        }
        break;
    case GUI_LEAVE:
        {
            if(m_ButtonTiles)
                m_ButtonTiles->setTileNum(BUTTON_NORMAL);
			m_Pressed = false;
        }
        break;
    case GUI_LMOUSEBUTTONDOWN:
        {
            if(m_ButtonTiles)
                m_ButtonTiles->setTileNum(BUTTON_CLICKED);
            m_Pressed = true;
			return 1;
        }
        break;
    case GUI_LMOUSEBUTTONUP:
        {
			if(m_ButtonTiles && m_Pressed)
			{
				if(!GetMouseOver())
					m_ButtonTiles->setTileNum(BUTTON_NORMAL);
				else
					m_ButtonTiles->setTileNum(BUTTON_HOVER);
				m_Pressed = false;
			}
        }
        break;
	};
    return GuiComponent::HandleEvent(type);
}

void GuiButton::Render()
{
	if(m_ButtonTiles)
		m_ButtonTiles->Render();
    if(getRenderLabel())
	{
		RECT guiRect;
		getUsableRect(guiRect);
        m_GuiFont->render((char*)getLabel().c_str(), 
						guiRect.left + 2, 
						guiRect.top + 2, 
						m_Width, 
						m_Height);
	}
    GuiComponent::Render();
}

void GuiButton::setPos(int x, int y) 
{
	LONG dx = x - m_Location.x;
	LONG dy = y - m_Location.y;
	if(m_ButtonTiles)
		m_ButtonTiles->SetXYPos(x, y);

	std::list<GuiComponent*>::iterator iComponents;
	for(iComponents = m_SubComponents.begin(); iComponents != m_SubComponents.end(); iComponents++)
	{
			POINT pos = (*iComponents)->getPos();
			(*iComponents)->setPos(pos.x + dx, pos.y + dy);
			
	}
	GuiComponent::setPos(x, y);
}