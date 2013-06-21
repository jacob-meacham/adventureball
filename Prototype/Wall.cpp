#include "Wall.h"
#include "..\Framework\Physics_Core.h"
#include "..\Framework\Define.h"
#include <iostream>
#include <sstream>
#include <math.h>
#define Error(x) MessageBox(NULL, x, "Error", MB_OK);

extern Player player;

bool Wall::checkType(EntityType e) {
	return (e == WALL);
}

void Wall::Collide(PhysicalEntity *other) { 
	//Check if this m_key does not equal -1 and if the physical entity is a ball. If so, check if the Player's
	//active item is m_key. If so, set wall to exit, set m_key to -1. 
	if(m_key != -1 && other->checkType(BALL)) {
		if(player.GetActiveItem() == m_key) {
			SetExit(m_willExitTo);
			m_key = (Items)-1; 
		}
	}
}



/// Default constructor.
Wall::Wall() {
	m_Tiles = NULL;
	m_exitRoomNumber = -1;
	m_key = (Items)-1; //As long as this is set to -1, this object is not considered a door
//	m_roomNumber = -1; //What is this for?
}

/// Default destructor.
Wall::~Wall() {
	Free();
}

/// Frees a Wall's memory.
void Wall::Free() { }
		
/// Specifies the Tile set to use, and which Texture within the tileset to use.
bool Wall::UseTiles(Tile *Tiles, char TextureNum) {
	m_Tiles = Tiles;
	m_TextureNum = TextureNum;
	if(m_Tiles == NULL) return false;
	return true;
}

/// Returns the tile set used by this Wall.
Tile* Wall::GetTiles() { return m_Tiles; }

/// Returns the texture number used by this Wall.
char Wall::GetTextureNum() { return m_TextureNum; }

/// Creates a Wall, using a starting point and an ending point.
/** Create should be called only after UseTiles().
	This function is slower than Wall::CreateWallVector() due to an inverse cosine.
*/
void Wall::CreateWallSegment(float XPos1, float YPos1, float XPos2, float YPos2) {
	// Calculate length based on distance formula
	m_Length = sqrt((XPos2-XPos1)*(XPos2-XPos1) + (YPos2-YPos1)*(YPos2-YPos1));

	// Determine rotation angle
	if(m_Length == 0) { m_rotationAngle = 0; }
	else { 	m_rotationAngle = acos((XPos1 - XPos2)/m_Length)+PI; }
	

	// Set bounding line
	m_BoundingLine.x1 = XPos1;
	m_BoundingLine.y1 = YPos1;
	m_BoundingLine.x2 = XPos2;
	m_BoundingLine.y2 = YPos2;

	// Set bounding sphere
	m_BoundingSphere.center.x = (XPos1 + XPos2)/2;
	m_BoundingSphere.center.y = (YPos1 + YPos2)/2;
	m_BoundingSphere.radius = m_Length/2;
	m_visible = true;
}

/// Creates a Wall, using a starting point, a rotation, and a length.
/** Create should be called only after UseTiles().
	This function is faster than Wall::CreateWallSegment() and should be used when possible.
*/
void Wall::CreateWallVector(float XPos, float YPos, float rotationAngle, float length) {
	// Set length and rotation
	m_Length = length;
	m_rotationAngle = rotationAngle;

	// Calculate bounding line
	m_BoundingLine.x1 = XPos;
	m_BoundingLine.y1 = YPos;
	m_BoundingLine.x2 = m_BoundingLine.x1 + m_Length * cosf(m_rotationAngle);
	m_BoundingLine.y2 = m_BoundingLine.y1 + m_Length * sinf(m_rotationAngle);

	// Calculate bounding sphere
	m_BoundingSphere.center.x = (m_BoundingLine.x1 + m_BoundingLine.x2)/2;
	m_BoundingSphere.center.y = (m_BoundingLine.x1 + m_BoundingLine.x2)/2;
	m_BoundingSphere.radius = m_Length/2;
	m_visible = true;
}

/// Sets this Wall's bounding line
void Wall::SetBoundingLine(float ax, float ay, float bx, float by) {
	m_BoundingLine.x1 = ax;
	m_BoundingLine.y1 = ay;
	m_BoundingLine.x2 = bx;
	m_BoundingLine.y2 = by;

}

/// translates the Wall by a deltaX and deltaY - movement is not absolute.
void Wall::TranslateWall(float dx, float dy) {
	m_BoundingLine.x1 += dx;
	m_BoundingLine.y1 += dy;
	m_BoundingLine.x2 += dx;
	m_BoundingLine.y2 += dy;
}

/// Renders the Wall.
/** If the Wall is invisible or dead, it is not rendered.
	\return TRUE if rendering is possible, or if the Wall is dead or invisible.
*/
bool Wall::Render() {
	if(m_Tiles == NULL) { Error("in Line::Render, tileset is Null!"); return false; }
	if(!m_visible) { return true; }

	// Simply uses class Tile's draw function, and draws the current frame
	// at the current scale and x,y coordinates.
	m_Tiles->Draw(m_TextureNum, 0, m_BoundingLine.x1, m_BoundingLine.y1, -1, m_Length/4, 
				 -1, 0, 0, m_rotationAngle);
	return true;
}

/// Return the visibility status of the Line.
bool Wall::GetIsVisible() const { return m_visible; }

/// Set the visibility status of the Line to v.
void Wall::SetVisible(bool v) { m_visible = v; }

/// Rotates that Wall by a delta angle
void Wall::Rotate(float angle) {
	float c;
	float s;
	
	// Add to rotation angle
	m_rotationAngle += angle;
	if(m_rotationAngle >= 2*PI) {
	m_rotationAngle -= 2*PI;
	}

	// Determine sin and cosin
	c = cosf(angle);
	s = sinf(angle);
	
	// Translate wall to the origin
	float tempX1 = m_BoundingLine.x1 - SCREENCENTERX;
	float tempY1 = m_BoundingLine.y1 - SCREENCENTERY;
	float tempX2 = m_BoundingLine.x2 - SCREENCENTERX;
	float tempY2 = m_BoundingLine.y2 - SCREENCENTERY;

	// Rotate bounding lines, and retranslate.
	m_BoundingLine.x1 = c*tempX1 - s*tempY1;
	m_BoundingLine.y1 = s*tempX1 + c*tempY1;
	m_BoundingLine.x1 += SCREENCENTERX;
	m_BoundingLine.y1 += SCREENCENTERY;
	m_BoundingLine.x2 = c*tempX2 - s*tempY2;
	m_BoundingLine.y2 = s*tempX2 + c*tempY2;
	m_BoundingLine.x2 += SCREENCENTERX;
	m_BoundingLine.y2 += SCREENCENTERY;
}

/// Sets the angle of this wall.  Note: This only changes the render angle, not the Collision angle.
void Wall::SetAngle(float angle) {
	m_rotationAngle = angle;
}


/// Getter function to get the rotation angle.
float Wall::GetAngle() const { return m_rotationAngle; }


/// Getter function to get the bounding line.
BOUNDINGLINE Wall::GetBoundingLine() {
	return m_BoundingLine;
}

/*
void Wall::SetExit(int roomNumber, int exitToRoom) {
	SetVisible(false);
	m_roomNumber = roomNumber;
	m_exitRoomNumber = exitToRoom;
}
*/

/// Sets this Wall's as an exit to room 'roomNumber'
void Wall::SetExit(int roomNumber) {
	SetVisible(false);
	m_exitRoomNumber = roomNumber;
}

//Sets this wall as a door that opens when the player has the input key. This method
//should only be called after SetExit
void Wall::SetDoor(Items key) {
	SetVisible(true);
	m_willExitTo = m_exitRoomNumber;
	m_exitRoomNumber = -1;
	m_key = key;
}

/// Returns the room number that this wall exits to.
/**
\return -1 if there is no exit, the room number otherwise.
*/
int Wall::GetExitRoom() const { return m_exitRoomNumber; }

/// Returns true if this wall is an exit.
bool Wall::GetIsExit() const { 
	if(m_exitRoomNumber != -1) { return true; }  
	return false;
}