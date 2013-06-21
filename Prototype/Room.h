#pragma once

#include <vector>
#include "Framework\Define.h"
#include "Wall.h"

class Tile;
class Corridor;

/// A Room
class Room {
	private:
		std::vector<Wall>	m_sides;		///< Vector of sides
		std::list<Corridor*> m_corridors;	///< Pointer to all corridors connected to this room
		int					m_sideLength;	///< Length of each side
		float				m_curAngle;		///< current angle this room is rotated to
		float				m_angle;
		float				m_radius;		///< Radius (width of this room)
		int					m_roomNumber;	///< room ID number
		
	public:
		float				m_curX;			///< current X coord of this room
		float				m_curY;			///< current Y coord of this room
		float				m_centerX;		///< The screen coordinate which marks the center of this room.
		float				m_centerY;		///< The screen coordinate which marks the center of this room.
		BoundingSphere		m_BoundingSphere; ///< Bounds the room
		
		Room();
		Room(int roomNumber, int numSides, int sideLength, Tile* tiles);
		
		// Transformers
		void SetRoom(int roomNumber, int numSides, int sideLength, Tile* tiles);
		void RotateRoom(float angle);
		void MoveRoom(float x, float y);
		void TranslateRoom(float dx, float dy);

		void AddCorridor(Corridor* corridor);
		
		bool CheckCollisions(float radius, const POINT & p);
		virtual void Update();
		virtual void Render(bool full = true) const;

		float LengthBetween(float x, float y) const; //Returns length between point and center of room.
		
		// Setters
		void SetRotationCenterX(float x);
		void SetRotationCenterY(float x);
		void SetSideAsExit(int sideNum, int RoomNumber);
		void SetSideAsDoor(int sideNum, Items key);

		// Getters
		int GetNumSides() const;
		Wall* GetSide(int sideNumber);
		int GetSideLength() const;
		float GetRadius() const;
		float GetCenterX() const;
		float GetCenterY() const;
		float GetCurX() const;
		float GetCurY() const;
		int GetRoomNumber() const;
};