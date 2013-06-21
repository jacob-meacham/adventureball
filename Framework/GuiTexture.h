#pragma once

#include <windows.h>
#include <list>
#include <string>
#include <sstream>
#include "GraphicsDevice.h"
#include "Texture.h"
#include "System.h"
#include "Physics_Core.h"

/// All rendered objects extend from the Sprite base class.
/** The Sprite class handles all rendering of any game object.  This includes
	animations, scale, and position.
*/
class GuiTexture {
		
	protected:
		std::string		m_Name;
		int				m_Width; ///< Width of the sprite tile.
		int				m_Height; ///< Height of the sprite tile.
		POINT			m_Location;
		bool			m_visible;
		Tile			*m_Tiles; ///< Pointer to the tile set that this sprite uses
		int				m_TileNum; ///< Current tile number in the texture.
		char			m_TextureNum; ///< Texture number from the Tile.
		float			m_XScale; ///< X-scaling factor. 1.0f is no scaling.
		float			m_YScale; ///< Y-scaling factor. 1.0f is no scaling.
		
	public:
		
		// Constructor/Destructor.
		GuiTexture();
		GuiTexture(int x, int y, int width, int height);
		
		virtual ~GuiTexture();

		// Frees a sprite's memory.
		void Free();
		
		//Specifies which tileset to use
		bool UseTiles(Tile *Tiles, char TextureNum); 

		// Returns the tile set used by this Sprite.
		Tile* GetTiles();

		// Returns the texture number used by this sprite.
		char GetTextureNum();

		// Creates a renderable sprite.
		void Create(char TileNum, int x, int y, int width, int height);
		void setTileNum(int tilenum) { m_TileNum = tilenum; }
		
		void Resize(int x,int y,int width,int height); 

		// Renders the sprite.
		bool Render();
		
		// Setters.
		void SetXScale(float Scale);
		void SetYScale(float Scale);
		void SetXYPos(long x, long y);
		void SetVisible(bool v);

		void Translate(int x, int y);

		// Getters.		
		long GetXPos();
		long GetYPos();
		long GetWidth();
		long GetHeight();
		float GetXScale();
		float GetYScale();

		bool GetIsVisible();
};