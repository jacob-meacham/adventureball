#pragma once

#include <list>
#include "Room.h"
#include "Corridor.h"
#include "..\Framework\Define.h"
#include "CollisionManager.h"
#include "Ball.h"

class Tile;
class Sprite;

extern class Level gLevel;
/// Level, where the game is actually played.
class Level {
	private:
		std::vector<Room> m_rooms;					///< List of all rooms in this level.
		std::list<Corridor> m_corridors;			///< List of all corridors in this level.
		Room *m_currentRoom;						///< Pointer to the current room
		Ball *m_ball;								///< Pointer to the player.
		Tile *m_tiles;								///< Tileset for the walls of this level
		int lastRoomAdded;							///< Index of the last room added
		float r1CurX, r1CurY;						///< Debug.
		CollisionManager m_CollisionManager;		///< Singleton CollionManager.
		float m_cameraVelocityX, m_cameraVelocityY; ///< velocity of the camera.

		////////HACK/////////
		bool firstTime;								// if true, the level has just begun.
		////////HACK/////////

	public:
		bool m_inCorridor;							///< Debug.
			
		Level();		
		void UseTiles(Tile* tiles);
		
		void AddPlayer(Ball *ball);
		void AddRoom(Room m);
		void AddRoom(int numSides, int sideLength);
		void AddCorridor(int room1, int side1, int room2, int side2, int length);
		
		// Transformers
		void CenterLevel(int roomNumber);
		void TranslateLevel(float x, float y);
		void RotateGame(float angle);
		void PanLevel(float x, float y);

		void RegisterRoom(Room *room);

		void ActivateRoomsWithSprite(Sprite* s);
		
		void UpdateLevel();
		void RenderLevel();

		bool CheckCollisions(float radius, POINT p);
		void ProcessCollisions();
		
		CollisionManager* GetCollisionManager() { return &m_CollisionManager; }
		
};