#pragma once

#include "GuiCommon.h"
#include "GuiMgr.h"
#include "Font.h"

class GuiComponent {
	friend class GuiMgr;
protected:
	std::string					m_Label;
	unsigned int				m_Type;
	unsigned int				m_Flags;
	bool						m_Visible;
	bool						m_Focus;
	bool						m_MouseOver;
	GuiComponent*				m_Parent;
	POINT						m_Location;
	int							m_Width;
	int							m_Height;
	bool				        m_renderLabel;
    POINT						m_LabelPos;

	std::list<GuiComponent *>	m_SubComponents;
	GuiComponent*				m_FocusComponent;
	GuiComponent*				m_MouseOverComponent;
	GuiComponent*				m_LockComponent;

	Font *						m_GuiFont;

public:
	GuiComponent();
	GuiComponent(int x,int y,int width,int height, const char* pLabel=0);
	virtual ~GuiComponent() { if(m_GuiFont) delete m_GuiFont; }
	virtual void init() { }

	int attachChildComponent(GuiComponent *component);
	int insertChildComponent(GuiComponent *component, u32 idx);
	int removeChildComponent(GuiComponent *component);
	int removeChildComponent(const char* pname);

	void setRenderLabel(bool render) { m_renderLabel = render; }
    bool getRenderLabel() { return m_renderLabel; }

    void setLabelPos(int x, int y) { m_LabelPos.x = x; m_LabelPos.y = y; }
    void getLabelPos(POINT &label) { label = m_LabelPos; }
    void setPosition(int x, int y);

	GuiComponent* GetLock() const;
	int SetLock(GuiComponent* pComponent);
	int Unlock();

	void SetTop(GuiComponent* pComponent);

	GuiComponent* getFocus() const { return NULL; }
	int setFocus(GuiComponent* pComponent) { return 0; }

	bool getHasFocus() const { return m_Focus; }
	void setHasFocus(bool focus) { m_Focus = focus; }
	
	GuiComponent* Find(const unsigned int id) const;
	int FindIndex(const GuiComponent* pComponent) const;

	void setParent(GuiComponent* pParent) { m_Parent = pParent; }
	GuiComponent* getParent() { return m_Parent; }
	
	Font* getFont() const { return m_GuiFont; }
	void setFont(Font* pFont) { m_GuiFont = pFont; }

	void setLabel(const char* pLabel) { m_Label = pLabel; }
	std::string getLabel() const { return m_Label; }
	
	virtual bool TestHit(int x,int y);
	virtual int TestHitElements(int x,int y) { return 0; }

	virtual void setPos(int x, int y) 
	{ 
		m_Location.x = x; m_Location.y = y; 
		m_LabelPos.x = x; m_LabelPos.y = y;
	}
	virtual void Move(int x, int y) 
	{ 
		m_Location.x += x; m_Location.y += y; 
		m_LabelPos.x += x; m_LabelPos.y += y; 
	
	}
		
	POINT getPos() const { return m_Location; }

	void setSize(int width, int height) { m_Width = width; m_Height = height; }
	POINT getSize() const { POINT temp; temp.x = m_Width; temp.y = m_Height; return temp; }

	void getFullRect(RECT &rect);
	virtual void getUsableRect(RECT &rect);

	void Show() { m_Visible = true; }
	void Hide() { m_Visible = false; }
	bool GetVisible() const { return m_Visible; }
	bool GetMouseOver() const { return m_MouseOver; }
	void SetMouseOver(bool m) { m_MouseOver = m; }

	void BringToFront();

	virtual void Render() { 
		RenderChildren();	
	}
	void RenderChildren();

	virtual int  HandleEvent(int type);
	virtual int  CommandHandle(GuiComponent *component, int cmd, int param) { return 0; }
};