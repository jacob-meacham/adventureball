#include "GuiTexture.h"
#include <iostream>
#include <sstream>

/// Default constructor.
GuiTexture::GuiTexture() {	m_Tiles = NULL; }
GuiTexture::GuiTexture(int x, int y, int width, int height)
{
	m_Location.x = x;
	m_Location.y = y;
	m_Width = width;
	m_Height = height;
}

/// Default destructor.
GuiTexture::~GuiTexture() {	Free(); }

/// Frees a GuiTexture's memory.
void GuiTexture::Free() { }
		
/// Specifies the Tile set to use, and which Texture within the tileset to use.
/** Example usage:
	m_Tiles.Create(&m_Graphics, 1);
	m_Tiles.Load(0, "Data\\Ego.png", 180, 240);
	m_Ego.UseTiles(&m_Tiles, 0);
	\return TRUE if the tileset is not null
*/
bool GuiTexture::UseTiles(Tile *Tiles, char TextureNum) 
{
	m_Tiles = Tiles;
	m_TextureNum = TextureNum;
	if(m_Tiles == NULL) return false;
	return true;
}

/// Returns the tile set used by this GuiTexture.
Tile* GuiTexture::GetTiles() { return m_Tiles; }

/// Returns the texture number used by this GuiTexture.
char GuiTexture::GetTextureNum() { return m_TextureNum; }

/// Creates a renderable GuiTexture.
/** Create should be called only after UseTiles().
	\param TileNum Starting tile number to use (generally 0).
	\param name Name of the GuiTexture (should be unique for parsing purposes)
	\param XPos X-Coordinate of the GuiTexture.
	\param YPos Y-Coordinate of the GuiTexture.
*/
void GuiTexture::Create(char TileNum, int x, int y, int width, int height) 
{
	m_Width = width;
	m_Height = height;

	int m_trueWidth = m_Tiles->GetWidth(m_TextureNum);
	int m_trueHeight = m_Tiles->GetHeight(m_TextureNum);

	m_XScale = (float)m_Width/(float)m_trueWidth;
	m_YScale = (float)m_Height/(float)m_trueHeight;
	
	m_Location.x = x;
	m_Location.y = y;

	m_TileNum = TileNum;
	m_visible = true;
}

void GuiTexture::Resize(int x, int y, int width, int height)
{
	m_Location.x = x;
	m_Location.y = y;

	m_Width = width;
	m_Height = height;

	int m_trueWidth = m_Tiles->GetWidth(m_TextureNum);
	int m_trueHeight = m_Tiles->GetHeight(m_TextureNum);

	m_XScale = (float)m_Width/(float)m_trueWidth;
	m_YScale = (float)m_Height/(float)m_trueHeight;
}

void Create(char TileNum, int x, int y, int width, int height);
		
		void Resize(int x,int y,int width,int height); 

/// Renders the GuiTexture.
/** If the GuiTexture is invisible or dead, it is not rendered.
	\return TRUE if rendering is possible, or if the GuiTexture is dead or invisible.
*/
bool GuiTexture::Render() 
{
	if(!m_Tiles) { OutputDebugString("in GuiTexture::Render, tileset is Null!"); return false; }
	if(!m_visible) { return true; }

	// Simply uses class Tile's draw function, and draws the current frame
	// at the current scale and x,y coordinates.
	m_Tiles->Draw(m_TextureNum, m_TileNum, (float)m_Location.x, (float)m_Location.y, 0xFFFFFFFF, m_XScale, m_YScale);
	return true;
}

/// Returns the X-position of the GuiTexture.
long GuiTexture::GetXPos() 
{
	return m_Location.x;
}

/// Returns the Y-position of the GuiTexture.
long GuiTexture::GetYPos() 
{
	return m_Location.y;
}

/// Returns the width of the GuiTexture's tiles.
long GuiTexture::GetWidth()
{
	return m_Width;
}

/// Returns the height of the GuiTexture's tiles.
long GuiTexture::GetHeight() 
{
	return m_Height;
}

/// Sets the X, Y position to x, and y.
void GuiTexture::SetXYPos(long x, long y) 
{
	m_Location.x = x;
	m_Location.y = y;
}

/// Returns the current X-Scale of this GuiTexture.
float GuiTexture::GetXScale() 
{
	return m_XScale;
}

/// Returns the current Y-Scale of this GuiTexture.
float GuiTexture::GetYScale() 
{
	return m_YScale;
}

/// Sets the current X-Scale to Scale.
/** The scale is absolute, not cumulative.
	SetXScale(0.5f) followed by SetXScale(0.5f), sets the absolute scale to 0.5 the original.
*/
void GuiTexture::SetXScale(float Scale) 
{
	// because the original rect never changes,
	// we can reset m_Width.
	//scale m_Width by the scaling factor.
	m_Width *= (int)Scale;
	m_XScale = Scale;
}

/// Sets the current Y-Scale to Scale.
/** The scale is absolute, not cumulative.
	SetYScale(0.5f) followed by SetYScale(0.5f), sets the absolute scale to 0.5 the original.
*/
void GuiTexture::SetYScale(float Scale) 
{
	// because the original rect never changes,
	// we can reset m_Height.	
	//scale m_Height by the scaling factor.
	m_Height *= (int)Scale;
	m_YScale = Scale;
}


/// Return the visibility status of the GuiTexture.
bool GuiTexture::GetIsVisible() { return m_visible; }

/// Set the visibility status of the GuiTexture to v.
void GuiTexture::SetVisible(bool v) { m_visible = v; }

void GuiTexture::Translate(int x, int y) 
{
	m_Location.x += x;
	m_Location.y += y;
}