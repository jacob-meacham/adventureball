#include "GuiStatic.h"
GuiStatic::GuiStatic() { }
GuiStatic::GuiStatic(int x, int y, int width, int height, const char* pLabel) : GuiComponent(x, y, width, height, pLabel)
{
    setRenderLabel(true);
    setLabelPos(x + 5, y + 5);
    setFont(gGuiMgr.getDefaultFont());
}

GuiStatic::~GuiStatic()
{
    if(m_Font) { delete m_Font; m_Font = NULL; }
    if(m_Back) { delete m_Back; m_Back = NULL; }
}

void GuiStatic::setGraphics(int textureNum)
{
	m_Back = new GuiTexture();
	m_Back->UseTiles(gGuiMgr.m_Tiles, textureNum);
	m_Back->Create(0, m_Location.x, m_Location.y, m_Width, m_Height);
}

void GuiStatic::setFont(Font* font, D3DCOLOR fontColor) { m_Font = font; m_fontColor = fontColor; }

void GuiStatic::Render()
{
    POINT labelPos;
    getLabelPos(labelPos);
	RECT guiRect;
	getUsableRect(guiRect);
	m_Font->render((char*)getLabel().c_str(), labelPos.x, labelPos.y, guiRect.right, guiRect.bottom, m_fontColor);
    if(m_Back)
		m_Back->Render();
	GuiComponent::Render();
}

void GuiStatic::setPos(int x, int y)
{
	if(m_Back)
		m_Back->SetXYPos(x, y);
	GuiComponent::setPos(x, y);
}
