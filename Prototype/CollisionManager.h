#pragma once

#include "Framework\PhysicsCore.h"
#include "Framework\Sprite.h"

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
		SpriteLayer m_Sprites;
	public:
		CollisionManager();
		bool RegisterEntity(const std::string & layerName, PhysicalEntity* entity);
		bool FreeLayer(const std::string & layerName);
		void FindAndResolveCollisions();
		void MakeRoomsActive();
};