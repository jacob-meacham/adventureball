#pragma once

#include <vector>
#include "CollisionManager.h"
#include "..\Framework\Sprite.h"


#define NUM_ITEMS 5 //Max number of items currently supported by the game. Set low for testing.

/*This class will hold information for the sprite ball the player will direct through the game.*/
class Ball : public Sprite {
	private:
		Level* m_level;

		//Holding collision info
		BoundingSphere m_bs; 
		Vector2 m_fp; 
		float m_dist;

	public:
		virtual void Collide(PhysicalEntity *other);
		virtual bool checkType(EntityType e);
		void SetCollisionInfo(BoundingSphere p, Vector2 pt, float distance);
		void SetLevel(Level* level);
		void Bounce(float x, float y);
};