#include "WinMain.h"
#include <windows.h>
#include "Framework\DataSet.h"
#include "Framework\GuiMgr.h"
#include "Level.h"
//////////////////////////////////////////////////////////////////////////////////
/// Default constructor.  Creates the window.
Prototype::Prototype() 
{
	g_dwLastTick = 0;
	g_dCurTime = 0;
	g_dLastTime = 0;
	g_fAnimationTimer = 0;
	g_fElpasedTime = 0;
	m_printfps = false;
	windowed = true;
}
//////////////////////////////////////////////////////////////////////////////////
/// Called before the game enters the message pump.
/** This function initializes graphics and input devices, loads the main font,
*/
bool Prototype::onInit() 
{
	if(!System::onInit())
		return false;

	m_Keyboard.Init(getWindow().getHWnd(), getWindow().getHInst());
	m_Keyboard.Create(KEYBOARD, windowed);

	m_Mouse.Init(getWindow().getHWnd(), getWindow().getHInst());
	m_Mouse.Create(MOUSE, windowed);

	m_rotation = 0;

	// Create the main font.
	mainFont.create("Arial", 19, 1);

	// Create tiles.
	m_Tiles.Create(2);
	ASSERT(&m_Tiles);
	
	// Load tiles.
	m_Tiles.Load(0, "Data\\Ball.png", 30, 30);
	m_Tiles.Load(1, "Data\\Side.png", 4, 3);
	m_Ball.UseTiles(&m_Tiles, 0);

	// Create ball, and add ball to level.
	m_Ball.Create(0, "ball", SCREENCENTERX, SCREENCENTERY);
	m_Ball.AddItem(RED_KEY);
	m_Ball.SetNextActiveItem();
	gLevel.AddPlayer(&m_Ball);

	gLevel.UseTiles(&m_Tiles);

	initGui();
	
	// Add rooms.
	gLevel.AddRoom(8, 40);
	gLevel.AddRoom(6, 20);
	gLevel.AddRoom(5, 100);
	gLevel.AddRoom(6, 40);
	gLevel.AddRoom(20, 10);
	gLevel.AddRoom(10, 50);
	gLevel.AddRoom(4, 50);
	gLevel.AddRoom(7, 50);
	gLevel.AddCorridor(0, 0, 1, 0, 60);
	gLevel.AddCorridor(0, 4, 2, 0, 400);
	gLevel.AddCorridor(1, 4, 3, 0, 300);
	gLevel.AddCorridor(2, 2, 4, 0, 100);
	gLevel.AddCorridor(4, 12, 5, 0, 100);
	gLevel.AddCorridor(4, 5, 6, 0, 100);
	gLevel.AddCorridor(6, 3, 7, 0, 300);	

	// Begin level.
	m_Ball.SetXYVel(2, 2);

	g_dwLastTick = timeGetTime();
	return TRUE; 
}
//////////////////////////////////////////////////////////////////////////////////
void Prototype::initGui()
{
	gGuiMgr.m_Tiles = new Tile();
	gGuiMgr.m_Tiles->Create(6);
	gGuiMgr.m_Tiles->Load(0, "Data\\windowBG1.png", 512, 512);
	gGuiMgr.m_Tiles->Load(1, "Data\\windowBG2.png", 512, 512);
	gGuiMgr.m_Tiles->Load(2, "Data\\windowTB1.png", 512, 20);
	gGuiMgr.m_Tiles->Load(3, "Data\\windowTB2.png", 512, 20);
	gGuiMgr.m_Tiles->Load(4, "Data\\static1.png", 100, 64);
	gGuiMgr.m_Tiles->Load(5, "Data\\buttonA.png", 64, 40);

	gGuiMgr.setDefaultFont(&mainFont);

	/*
	m_pScreen = new GuiScreen(0, 0, 10000, 10000);
	//m_pScreen = new GuiScreen(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	m_pWindow = new GuiWindow(40, 40, 500, 500, "Outer");
	m_pWindow->setGraphics(2, 0);

	m_pSecondWindow = new GuiWindow(80, 80, 500, 500, "Second Window");
	m_pSecondWindow->setGraphics(3, 1);

	m_pInnerWindow = new GuiWindow(100, 100, 50, 250, "Inner");
	m_pInnerWindow->setGraphics(2, 0);

	m_pStatic = new GuiStatic(300, 300, 100, 30, "Static1");
	m_pStatic->setGraphics(4);
	m_pStatic->setFont(gGuiMgr.getDefaultFont());

	m_RegularButton = new GuiButton(200, 200, 64, 40, "ButtonA");
	m_RegularButton->setRenderLabel(true);
	m_RegularButton->setGraphics(5);
	m_BigButton = new GuiButton(300, 200, 128, 80, "ButtonB");
	m_BigButton->setRenderLabel(true);
	m_BigButton->setGraphics(5);

	m_LockedButton = new GuiButton(200, 300, 64, 40, "ButtonC");
	m_LockedButton->setLocked(true);
	m_LockedButton->setRenderLabel(true);
	m_LockedButton->setGraphics(5);

	gGuiMgr.SetActiveComponent(m_pScreen);
	m_pWindow->attachChildComponent(m_pInnerWindow);
	m_pSecondWindow->attachChildComponent(m_pStatic);
	m_pSecondWindow->attachChildComponent(m_RegularButton);
	m_pSecondWindow->attachChildComponent(m_BigButton);
	m_pSecondWindow->attachChildComponent(m_LockedButton);
	m_pScreen->attachChildComponent(m_pWindow);
	m_pScreen->attachChildComponent(m_pSecondWindow);
*/
}
//////////////////////////////////////////////////////////////////////////////////
/// Shutdown function, called after the message pump has completed.
void Prototype::shutdown() 
{
	
}
//////////////////////////////////////////////////////////////////////////////////
void Prototype::onProcess() 
{	
	// Timer updates.
	g_dCurTime     = timeGetTime();
	g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
	g_dLastTime    = g_dCurTime;
	g_fAnimationTimer += g_fElpasedTime;
	
	// FPS calculation, for debugging
	m_fps++;
	if( g_fAnimationTimer >= 1.0f ) { 
		g_fAnimationTimer = 0.0f; 
		m_printfps = true; 
		fpsString.str("");
	}
	
	processInput();

	gLevel.UpdateLevel();
	
	// Begin render state.
	if(gGraphics.beginScene()) 
	{
		gGraphics.clear();
		gGraphics.beginSprite();
		gLevel.RenderLevel();

		//gGuiMgr.RenderGui();
		
		if(m_printfps) {
			fpsString << m_fps << " " << g_fElpasedTime << ", (" << m_Ball.GetXPos() << ", " << m_Ball.GetYPos() << ")";
		m_printfps = false;
		m_fps = 0;
		}

		mainFont.render((char*)fpsString.str().c_str(), 0, 0, 500, 100);
		gGraphics.endSprite();
		gGraphics.endScene();
	}
	// Display graphics.
	gGraphics.display();
}
//////////////////////////////////////////////////////////////////////////////////
bool Prototype::processInput() 
{
	// Read keyboard and mouse state.
	m_Keyboard.Read();
	m_Mouse.Read();
	
	if(m_Mouse.GetButtonState(MOUSE_LBUTTON) == TRUE) {
		m_Mouse.SetLock(0);
	}
    // End the game on escape.
	if(m_Keyboard.GetKeyState(DIK_ESCAPE) == TRUE) {
		return FALSE;
	}

	if(m_Keyboard.GetKeyState(DIK_Z) == TRUE) {
		m_rotation += 0.01f;
		gLevel.RotateGame(0.04f);
			
	}
	if(m_Keyboard.GetKeyState(DIK_X) == TRUE) {
		m_rotation -= 0.01f;
		gLevel.RotateGame(-0.04f);	
			
	}
	if(m_Keyboard.GetKeyState(DIK_W) == TRUE) {
		m_Ball.SetYVel(-5);
	}
		
	if(m_Keyboard.GetKeyState(DIK_S) == TRUE) {
		m_Ball.SetYVel(5);
	}	
			
	if(m_Keyboard.GetKeyState(DIK_A) == TRUE) {
		m_Ball.SetXVel(-5);
	}			
	
	if(m_Keyboard.GetKeyState(DIK_D) == TRUE) {
		m_Ball.SetXVel(5);
	}	
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK Prototype::MsgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if(gGuiMgr.GetActive())
	{
		switch(Msg) {
		// Keyboard and Mouse Events
		// Scan code is bits 16-23 or lParam
		case WM_KEYDOWN:
			gGuiMgr.SendEvent(GUI_KEYDOWN, wParam, GET_SCAN_CODE(lParam), -1, -1);
			return 0;
		case WM_KEYUP:
			gGuiMgr.SendEvent(GUI_KEYUP, wParam, GET_SCAN_CODE(lParam), -1, -1);
			return 0;
		case WM_LBUTTONDOWN:
			gGuiMgr.SendEvent(GUI_LMOUSEBUTTONDOWN, VK_LBUTTON, 0, LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_RBUTTONDOWN:
			gGuiMgr.SendEvent(GUI_RMOUSEBUTTONDOWN, VK_RBUTTON, 0, LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_LBUTTONUP:
			gGuiMgr.SendEvent(GUI_LMOUSEBUTTONUP, VK_LBUTTON, 0, LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_RBUTTONUP:
			gGuiMgr.SendEvent(GUI_RMOUSEBUTTONUP, VK_RBUTTON, 0, LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_MOUSEMOVE:
			gGuiMgr.SendEvent(GUI_MOUSEMOVE, 0, 0, LOWORD(lParam), HIWORD(lParam));
			return 0;
		};
	}
	else
	{
		// Do WM_INPUT
	}
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
//////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	_CrtSetDbgFlag(flag); // Set flag to the new value

	_crtBreakAlloc = -1; // Sets a user breakpoint at the specified allocation.
#endif 

	Prototype app;
	app.parseCommandLine(::__argc, ::__argv);

	int rval = 0;
	if(app.onInit())
	{
		rval = 1;
		app.messageLoop();
	}

	return rval;
}