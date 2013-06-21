#pragma once

#include "GuiCommon.h"
#include "GuiComponent.h"
class GuiStatic : public GuiComponent
{
protected:
    GuiTexture*		m_Back;
    Font*			m_Font;
	D3DCOLOR		m_fontColor;
public:
    GuiStatic();
    GuiStatic(int x, int y, int width, int height, const char* pLabel);
    virtual ~GuiStatic();
    void setGraphics(int textureNum);
	void setFont(Font* font, D3DCOLOR fontColor=0xFFFFFFFF);
	virtual void setPos(int x, int y);
    virtual int HandleEvent() { return 0; }
    virtual void Render() const;
};



