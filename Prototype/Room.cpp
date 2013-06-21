#include "Room.h"
#include "Corridor.h"
//////////////////////////////////////////////////////////////////////////////////
/// Default constructor.
Room::Room() { m_curAngle = 0.0f; }
//////////////////////////////////////////////////////////////////////////////////
/// Constructor which calls Room::SetRoom().
Room::Room(int roomNumber, int numSides, int sideLength, Tile* tiles) {
	SetRoom(roomNumber, numSides, sideLength, tiles);
}
//////////////////////////////////////////////////////////////////////////////////
/// Creates a room at the center of the screen.
/**
\param roomNumber The id of this room.
\param numSides Number of side this room has.
\param sideLength The length of each of the sides of this room
\param tiles The tileset for the walls of this room.
*/
void Room::SetRoom(int roomNumber, int numSides, int sideLength, Tile* tiles) {
	float x;
	float y;
	m_roomNumber = roomNumber;
	m_sideLength = sideLength * 4;
	
	// Determine inner angle for each wall
	m_angle = 360.0f / (float)numSides;
	
	// Set radius, center point and current center.
	m_radius = (float)m_sideLength / (2*tan(PI/(float)numSides));
	m_centerX = SCREENCENTERX;
	m_centerY = SCREENCENTERY;
	m_curX = m_centerX;
	m_curY = m_centerY;

	x = SCREENCENTERX - m_sideLength / 2.0f;
	y = SCREENCENTERY - m_radius;
	
	// Create each wall in this Room.
	for(int i =0; i<numSides; i++) {
		float angle = (i*m_angle)/DEGREES_PER_RADIAN;
		float c = cos(angle);
		float s = sin(angle);
		Wall temp;
		temp.UseTiles(tiles, 1);
		temp.CreateWallVector(x, y, angle, (float)m_sideLength);
		x += c * m_sideLength;
		y += s * m_sideLength;
		m_sides.push_back(temp);
	}

	//We must also give it a bounding sphere
	m_BoundingSphere.center.x = m_curX;
	m_BoundingSphere.center.y = m_curY;
	m_BoundingSphere.radius = m_radius;
}
//////////////////////////////////////////////////////////////////////////////////
/// Rotate this room by 'angle' NOTE: This shouldn't need to affect the bounding box.
void Room::RotateRoom(float angle) { 
	// Determine sin and cosine
	float c = cosf(angle);
	float s = sinf(angle);

	// Move the room to the origin.
	float tempX = m_curX - SCREENCENTERX;
	float tempY = m_curY - SCREENCENTERY;

	// Update the current x and current y, and translate back
	m_curX = c*tempX - s*tempY;
	m_curY = s*tempX + c*tempY;
	m_curX += SCREENCENTERX;
	m_curY += SCREENCENTERY;
	
	// Rotate each Wall.
	std::vector<Wall>::iterator iWall;
	for(iWall = m_sides.begin(); iWall != m_sides.end(); iWall++) {
		(*iWall).Rotate(angle);
	}
}
//////////////////////////////////////////////////////////////////////////////////
/// Move the room to an absolute position
/** TODO: This name is confusing, consider changing. */
void Room::MoveRoom(float x, float y) {
	std::vector<Wall>::iterator iWall;
	float deltaX = x - m_curX;
	float deltaY = y - m_curY;
	for(iWall = m_sides.begin(); iWall != m_sides.end(); iWall++) {
		(*iWall).TranslateWall(deltaX, deltaY);
	}
	std::list<Corridor*>::iterator iCorridor;
	for(iCorridor = m_corridors.begin(); iCorridor != m_corridors.end(); iCorridor++) {
		(*iCorridor)->TranslateCorridor(deltaX, deltaY);
	}
	m_curX += deltaX;
	m_curY += deltaY;

	//Need to move the BoundingSphere
	m_BoundingSphere.center.x = m_curX;
	m_BoundingSphere.center.y = m_curY;
}
//////////////////////////////////////////////////////////////////////////////////
/// Add a corridor to this room's corridors
void Room::AddCorridor(Corridor* corridor) {
	m_corridors.push_back(corridor);
}
//////////////////////////////////////////////////////////////////////////////////
/// Move the room by a translation vector.
void Room::TranslateRoom(float dx, float dy) {
	std::vector<Wall>::iterator iWall;
	for(iWall = m_sides.begin(); iWall != m_sides.end(); iWall++) {
		(*iWall).TranslateWall(dx, dy);
	}

	std::list<Corridor*>::iterator iCorridor;
	for(iCorridor = m_corridors.begin(); iCorridor != m_corridors.end(); iCorridor++) {
		(*iCorridor)->TranslateCorridor(dx, dy);
	}

	m_curX += dx;
	m_curY += dy;

	//Make sure to translate the BoundingSphere
	m_BoundingSphere.center.x = m_curX;
	m_BoundingSphere.center.y = m_curY;
}
//////////////////////////////////////////////////////////////////////////////////
float Room::LengthBetween(float x, float y) const {
	return sqrt((x - m_curX)*(x - m_curX) + (y - m_curY)*(y - m_curY));
}
//////////////////////////////////////////////////////////////////////////////////
/// Getter to return the number of sides of this room.
int Room::GetNumSides() const { return m_sides.size(); }
//////////////////////////////////////////////////////////////////////////////////
/// Getter to return the length of each side of this room
int Room::GetSideLength() const { return m_sideLength; }
//////////////////////////////////////////////////////////////////////////////////
void Room::Update() { }
//////////////////////////////////////////////////////////////////////////////////
void Room::Render(bool full) const {
	std::vector<Wall>::const_iterator iWall;
	for(iWall = m_sides.begin(); iWall != m_sides.end(); iWall++) {
		(*iWall).Render();
	}
}
//////////////////////////////////////////////////////////////////////////////////
float Room::GetCenterX() const { return m_centerX; }
//////////////////////////////////////////////////////////////////////////////////
float Room::GetCenterY() const { return m_centerY; }
//////////////////////////////////////////////////////////////////////////////////
float Room::GetCurX() const { return m_curX; }
//////////////////////////////////////////////////////////////////////////////////
float Room::GetCurY() const { return m_curY; }
//////////////////////////////////////////////////////////////////////////////////
int Room::GetRoomNumber() const { return m_roomNumber; }
//////////////////////////////////////////////////////////////////////////////////
bool Room::CheckCollisions(float radius, const POINT & p) {
	return false;
}
//////////////////////////////////////////////////////////////////////////////////
void Room::SetSideAsExit(int sideNum, int roomNumber) {
	m_sides.at(sideNum).SetExit(roomNumber);
}
//////////////////////////////////////////////////////////////////////////////////
//Only call this AFTER SetSideAsExit
void Room::SetSideAsDoor(int sideNum, Items key) {
	m_sides.at(sideNum).SetDoor(key);
}
//////////////////////////////////////////////////////////////////////////////////
Wall* Room::GetSide(int sideNumber) {
	return &m_sides.at(sideNumber);
}
//////////////////////////////////////////////////////////////////////////////////
float Room::GetRadius() const { return m_radius; }	