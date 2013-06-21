#include "Level.h"
#include "..\Framework\Sprite.h"


#define DOWNPAN SCREENCENTERY
#define UPPAN SCREENCENTERY
#define LEFTPAN SCREENCENTERX
#define RIGHTPAN SCREENCENTERX

Level gLevel;
//////////////////////////////////////////////////////////////////////////////////
/// Default constructor.
Level::Level() {
	m_rooms.clear();
	m_rooms.reserve(100);
	m_corridors.clear();
	m_currentRoom = NULL;
	lastRoomAdded = 0;
	m_cameraVelocityX = m_cameraVelocityY = 0;

	////////HACK/////////
	firstTime = true;
	////////HACK/////////
	
}
//////////////////////////////////////////////////////////////////////////////////
/// Adds the player Ball to this level, and registers the ball.
void Level::AddPlayer(Ball *ball) {
	m_ball = ball;
	m_CollisionManager.RegisterEntity("Sprite", m_ball);
}
//////////////////////////////////////////////////////////////////////////////////
/// Specifies the tileset to use for Walls in this level.
void Level::UseTiles(Tile* tiles) {
	m_tiles = tiles;
}
//////////////////////////////////////////////////////////////////////////////////
/// Adds a Room to this level.
void Level::AddRoom(Room m) { m_rooms.push_back(m); }
//////////////////////////////////////////////////////////////////////////////////
/// Adds a room with the specified number of sides and side length
void Level::AddRoom(int numSides, int sideLength) {
	Room temp(lastRoomAdded++, numSides, sideLength, m_tiles);
	AddRoom(temp);
}
//////////////////////////////////////////////////////////////////////////////////
/// Pans the camera over the level.
void Level::PanLevel(float x, float y) {
	TranslateLevel(x, y);
	m_ball->Translate(x, y);
}
//////////////////////////////////////////////////////////////////////////////////
/// Adds a corridor between rooms.
/** This is the function that actually places rooms.
*/
void Level::AddCorridor(int room1, int side1, int room2, int side2, int length) {
	Room* r1 = &m_rooms.at(room1);
	Room* r2 = &m_rooms.at(room2);
	Wall* s1 = r1->GetSide(side1);
	Wall* s2 = r2->GetSide(side2);

	r1->SetSideAsExit(side1, room1);
	r2->SetSideAsExit(side2, room2);
	/*
	r1->SetSideAsDoor(side1, RED_KEY);
	r2->SetSideAsDoor(side2, RED_KEY);
	*/

	float sa1 = s1->GetAngle();

	r1CurX = r1->m_curX;
	r1CurY = r1->m_curY;

	// Center level
	CenterLevel(room1);
	
	// Center room
	r2->MoveRoom(SCREENCENTERX, SCREENCENTERY);
		
	// Rotate rooms to 0, so that corridor is easier to calculate.
	// TODO: Calculate corridor, rather than rotating everything.
	r1->RotateRoom(-sa1);
	r2->RotateRoom(s1->GetAngle() - s2->GetAngle() + PI);

	BOUNDINGLINE b1 = s1->GetBoundingLine();
	BOUNDINGLINE b2 = s2->GetBoundingLine();

	// Determine mid point in each of the affected sides.
	float midX1 = (b1.x1 + b1.x2)/2.0f;
	float midY1 = (b1.y1 + b1.y2)/2.0f;

	float midX2 = (b2.x1 + b2.x2)/2.0f;
	float midY2 = (b2.y1 + b2.y2)/2.0f;

	// Move the second room to its proper position
	r2->MoveRoom(midX1 - (midX2 - SCREENCENTERX), midY1 - (midY2 - SCREENCENTERY));
	r2->MoveRoom(r2->m_curX, r2->m_curY-length);

	b1 = s1->GetBoundingLine();
	b2 = s2->GetBoundingLine();
	
	BOUNDINGLINE t;
	t.x1 = b1.x2;
	t.x2 = b2.x1;
	t.y1 = b1.y2;
	t.y2 = b2.y1;
	
	// Create a corridor between the sides.
	Corridor temp(BOUNDINGLINE(b1.x1, b1.y1, b2.x2, b2.y2), t, s1, s2, m_tiles);

	m_corridors.push_back(temp);
	
	r1->AddCorridor(&m_corridors.back());
	
	// Rotate to the correct place
	r1->RotateRoom(sa1);
	r2->RotateRoom(sa1);
	m_corridors.back().RotateCorridor(sa1);
}
//////////////////////////////////////////////////////////////////////////////////
/// Updates the level, all sprites in the level, and manages collisions.
void Level::UpdateLevel() {
	m_cameraVelocityX = 0;
	m_cameraVelocityY = 0;
	
	if(m_ball->GetYPos() > DOWNPAN && m_ball->GetYVel() > 0) {
		m_cameraVelocityY = -m_ball->GetYVel();
	}
	else if(m_ball->GetYPos() < UPPAN && m_ball->GetYVel() < 0) {
		m_cameraVelocityY = -m_ball->GetYVel();
	}
	
	if(m_ball->GetXPos() > RIGHTPAN && m_ball->GetXVel() > 0) {
		m_cameraVelocityX = -m_ball->GetXVel();
	}
	else if(m_ball->GetXPos() < LEFTPAN && m_ball->GetXVel() < 0) {
		m_cameraVelocityX = -m_ball->GetXVel();
	}

	if(m_cameraVelocityX != 0 || m_cameraVelocityY != 0)
		PanLevel(m_cameraVelocityX, m_cameraVelocityY);

	m_ball->Update();

	m_CollisionManager.FindAndResolveCollisions();
}
//////////////////////////////////////////////////////////////////////////////////
/// Rotates the entire level by 'angle' (not including sprites in the level)
void Level::RotateGame(float angle) {
	std::vector<Room>::iterator iRoom;
	for(iRoom = m_rooms.begin(); iRoom != m_rooms.end(); iRoom++) {
		(*iRoom).RotateRoom(angle);
	}
	std::list<Corridor>::iterator iCorridor;
	for(iCorridor = m_corridors.begin(); iCorridor != m_corridors.end(); iCorridor++) {
		(*iCorridor).RotateCorridor(angle);
	}
	
}
//////////////////////////////////////////////////////////////////////////////////
/// Render the full level, including all sprites.
void Level::RenderLevel() const {
	std::vector<Room>::const_iterator iRoom;
	for(iRoom = m_rooms.begin(); iRoom != m_rooms.end(); iRoom++) {
			(*iRoom).Render();
	}
	std::list<Corridor>::const_iterator iCorridor;
	for(iCorridor = m_corridors.begin(); iCorridor != m_corridors.end(); iCorridor++) {
		(*iCorridor).Render();
	}

	m_ball->Render();
}
//////////////////////////////////////////////////////////////////////////////////
/// Move the level by a translation vector.
void Level::TranslateLevel(float x, float y) {
	std::vector<Room>::iterator iRoom;
	for(iRoom = m_rooms.begin(); iRoom != m_rooms.end(); iRoom++) 
	{
			(*iRoom).TranslateRoom(x, y);
	}
}
//////////////////////////////////////////////////////////////////////////////////
/// Center the camera on the room whose roomNumber is the input
void Level::CenterLevel(int roomNumber) {
	Room *temp = &m_rooms[roomNumber];
	m_ball->Translate(temp->GetCenterX()-temp->GetCurX(), temp->GetCenterY()-temp->GetCurY());
	TranslateLevel(temp->GetCenterX()-temp->GetCurX(), temp->GetCenterY()-temp->GetCurY());
}
//////////////////////////////////////////////////////////////////////////////////
/// Register all the walls and exits for this room
void Level::RegisterRoom(Room *room) {
	for(int i = 0; i < room->GetNumSides(); i++) {
		m_CollisionManager.RegisterEntity("Wall", room->GetSide(i));
	}
}
//////////////////////////////////////////////////////////////////////////////////
void Level::ActivateRoomsWithSprite(Sprite* s) {
	//We need to check what bounding box the player is in.
	std::vector<Room>::iterator iRoom;
	for(iRoom = m_rooms.begin(); iRoom != m_rooms.end(); iRoom++) 
	{
		float length = (*iRoom).LengthBetween((float)s->GetXPos(), (float)s->GetYPos());
		if(length < (*iRoom).GetRadius()) {
			//This room is inhabited by a sprite. Make sure it is checked for collisions
			RegisterRoom(&(*iRoom));

			////////HACK/////////
			if(firstTime) {
				CenterLevel((*iRoom).GetRoomNumber());
				firstTime = false;
			}
			////////HACK/////////
		}
	}

	std::list<Corridor>::iterator iCorridor;
	for(iCorridor = m_corridors.begin(); iCorridor != m_corridors.end(); iCorridor++) {
		m_CollisionManager.RegisterEntity("Wall", &((*iCorridor).m_upperLine));
		m_CollisionManager.RegisterEntity("Wall", &((*iCorridor).m_lowerLine));
	}
}