#pragma once

#include "GuiCommon.h"
#include "GuiComponent.h"
class GuiButton : public GuiComponent
{
protected:
    int		     m_CurrentTextureNum;
    GuiTexture*  m_ButtonTiles;

    bool        m_Pressed;
    bool        m_Locked;
public:
    GuiButton();
    GuiButton(int x, int y, int width, int height, const char* text);
	virtual ~GuiButton();
    
	void setGraphics(int textureNum);
    virtual void Render() const;
    virtual int HandleEvent(int type);
    void setLocked(bool lock);
    bool getLocked() { return m_Locked; }
	virtual void setPos(int x, int y);
};