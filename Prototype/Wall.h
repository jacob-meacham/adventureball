#pragma once

#include <windows.h>
#include <list>
#include <string>
#include <sstream>
#include "Framework\System.h"
#include "Framework\PhysicsCore.h"
#include "Ball.h"

struct BOUNDINGLINE {
			float x1, y1;
			float x2, y2;
			BOUNDINGLINE() { x1 = y1 = x2 = y2 = 0; }
			BOUNDINGLINE(float pt1, float pt2, float pt3, float pt4) {
				x1 = pt1;
				y1 = pt2;
				x2 = pt3;
				y2 = pt4;
			}
		};

class Tile;
/// A Room Wall
/** Wall are bounded by a single line, rather than by a rectangle. */
class Wall : public PhysicalEntity {
	protected:
		Tile			*m_Tiles; ///< Pointer to the tile set that this sprite uses
		char			m_TextureNum; ///< Texture number from the Tile.
		
		float			m_Length;
		float			m_rotationAngle;
		bool			m_visible;
		int				m_exitRoomNumber;
		int				m_willExitTo;	//The number of the room this will exit to when this door is open

		Items			m_key; //If the wall is a door, this key will turn it into an exit
		

	public:
		BOUNDINGLINE	m_BoundingLine;

		// Constructor/Destructor.
		Wall();
		~Wall();
		
		//Specifies which tileset to use
		bool UseTiles(Tile *Tiles, char TextureNum); 

		// Returns the tile set used by this Sprite.
		Tile* GetTiles() const;

		// Returns the texture number used by this sprite.
		char GetTextureNum() const;

		// Creates a renderable sprite.
		void CreateWallSegment(float XPos1, float YPos1, float XPos2, float YPos2);
		void CreateWallVector(float XPos, float YPos, float rotationAngle, float length);

		void TranslateWall(float dx, float dy);

		void SetBoundingLine(float, float, float, float);
		BOUNDINGLINE GetBoundingLine() const;

		virtual void Collide(PhysicalEntity *other);

		// Checks if this wall is an entity of type e. Will return false on all types except WALL.
		virtual bool checkType(EntityType e) const;
		
		// Renders the sprite.
		bool Render() const;
				
		// Setters.
		void Rotate(float angle);
		void SetVisible(bool v);
		void SetAngle(float angle);
		void SetExit(int roomNumber, int exitToRoom);
		void SetExit(int roomNumber);
		void SetDoor(Items key); //Sets this wall as a door that opens when the player has the input key. This method should only be called after SetExit
		
		// Getters.		
		bool GetIsVisible() const;
		int GetExitRoom() const;
		bool GetIsExit() const;
		float GetAngle() const;
};