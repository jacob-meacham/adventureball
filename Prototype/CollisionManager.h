#pragma once

#include "..\Framework\Physics_Core.h"
#include "Wall.h"
#include "..\Framework\Sprite.h"

class Sprite;
class Wall;
class Level;

/// Wall layer
class StaticLayer : public BaseLayer<Wall> {
	public:
		StaticLayer();
		void InterTest(BaseLayer<Sprite>* other);
		void IntraTest();

	private:
		bool WallCollision(Wall* wall, PhysicalEntity* other);
};

/// Moving Sprite layer.
class SpriteLayer : public BaseLayer<Sprite> {
	public:
		SpriteLayer();
		void InterTest(BaseLayer<Sprite>* other);
		void IntraTest();
};

/// Singleton Collision Manager.
/** The collision manager resides in Level, knows about all physical entities, and monitors and resolves collisions.
*/
class CollisionManager {
	private:
		StaticLayer m_Walls;
//		ExitLayer m_Exits;
		SpriteLayer m_Sprites;
		//DESIGN: Do we want a seperate sprite layer for enemies and items?
		//SpriteLayer m_enemies;
		//SpriteLayer m_items;
		Level *m_parent;
	public:
		CollisionManager();
		void SetParent(Level *p);
		bool RegisterEntity(std::string layerName, PhysicalEntity* entity);
		bool FreeLayer(std::string layerName);
		void FindAndResolveCollisions();
		void MakeRoomsActive();
};