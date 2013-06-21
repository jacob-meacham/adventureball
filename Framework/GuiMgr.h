#pragma once

#include "GuiCommon.h"
#include "GuiComponent.h"
#include "Font.h"

class GuiComponent;

class GuiMgr {
protected:
	GuiComponent*		m_pActiveScreen; 
	Font *				m_DefaultFont;
	GuiEvent			m_CurrentEvent;
	POINT				m_MouseLastPos;
	bool				m_Active;
public:
	~GuiMgr();

	Tile*				m_Tiles;
	void SetActiveComponent(GuiComponent* pActive);
	GuiComponent* GetActiveScreen() { return NULL; }
	void SetActive(bool a) { m_Active = a; }
	bool GetActive() const { return m_Active; }
	
	void RenderGui();
	void SendEvent(int type, int key, int scanCode, int x, int y);
	Font* getDefaultFont() const { return m_DefaultFont; }
	void setDefaultFont(Font* font) { m_DefaultFont = font; }
	const GuiEvent* getCurrentEvent() const { return &m_CurrentEvent; }

};

extern GuiMgr gGuiMgr;