#include "CollisionManager.h"
#include "Level.h"
#include "..\Framework\Define.h"

/// Checks to see if 2 bounding spheres are colliding.  
/** If the spheres are colliding, this moves entity1 out of collision.
\return true if we are colliding */
template <class T>
bool BaseLayer<T>::BoundingSphereCollision(PhysicalEntity* entity1, PhysicalEntity* entity2);

/// Base Constructor
StaticLayer::StaticLayer() {
	SetName("Static");
}

/// Test if another sprite has collided with a Wall.
void StaticLayer::InterTest(BaseLayer<Sprite>* other) { 
	// Only tests sprites, not static sprites.
	if(other->GetName().compare("StaticSprite") == 0) { return; }
	if(other->GetName().compare("Sprite") == 0) {
		std::list<Wall*>::iterator iWall;
		std::list<Sprite*>::iterator iSprite;
		for(iSprite = other->m_members.begin(); iSprite != other->m_members.end(); iSprite++) {
			for(iWall = m_members.begin(); iWall != m_members.end(); iWall++) {
					if(WallCollision((*iWall), (*iSprite))) {
						// TODO: make it so that this works!
						//(*iSprite)->Collide((*iWall));
					}
			}
		}
	}
	else { return; }
}

/// Performs a circle/line collision test, and bounces other if there is a collision.
bool StaticLayer::WallCollision(Wall* wall, PhysicalEntity* other) { 
	BoundingSphere p = *(other->GetBoundingSphere());

	Vector2 AP, AB;
	
	// Find the line between the sphere and one point on the line
	AP.x = p.center.x - wall->m_BoundingLine.x1;
	AP.y = p.center.y - wall->m_BoundingLine.y1;

	// Find the bounding line
	AB.x = wall->m_BoundingLine.x2 - wall->m_BoundingLine.x1;
	AB.y = wall->m_BoundingLine.y2 - wall->m_BoundingLine.y1;

	// Calculate u (dot product)
	float u = ((AP.x * AB.x)+(AP.y * AB.y))/((AB.x*AB.x)+(AB.y*AB.y));

	// Only if u is between 0 and 1 is the sphere possible colliding with the line segment
	if(u < 0 || u > 1) { return false; }

	// Determine the closest point from the line to the center of the sphere.
	Vector2 pt;
	pt.x = wall->m_BoundingLine.x1 + u * (wall->m_BoundingLine.x2 - wall->m_BoundingLine.x1);
	pt.y = wall->m_BoundingLine.y1 + u * (wall->m_BoundingLine.y2 - wall->m_BoundingLine.y1);

	// Determine the distance between this point and the sphere.
	float distance = sqrt((p.center.x - pt.x)*(p.center.x - pt.x) + 
						  (p.center.y - pt.y)*(p.center.y - pt.y));

	// If that distance > 0, no collision
	float diff = distance - p.radius;
	if(diff >= 0) { return false; }

	Ball* b;
	if(other->checkType(BALL)) {
		b = dynamic_cast<Ball *>(other);
	}

	Vector2 normalPenetrationVector;
	normalPenetrationVector.x = (p.center.x - pt.x) / distance;
	normalPenetrationVector.y = (p.center.y - pt.y) / distance;

	//If this wall is not an exit, bounce the ball
	if(!(wall->GetIsExit())) {
		b->Move(-normalPenetrationVector.x*diff, -normalPenetrationVector.y*diff);
		//If they have collided, run the collision methods
		b->Bounce(-normalPenetrationVector.x, -normalPenetrationVector.y);
	}	

	wall->Collide(other); 
	other->Collide(wall);

	return true;
}

/// Does nothing (static layer).
void StaticLayer::IntraTest() { return; }

/// Default constructor.
SpriteLayer::SpriteLayer() {
	SetStatic(false);
	SetName("Sprite");
}

/// Tests this sprite layer against another sprite layer, using a bounding sphere collision.
void SpriteLayer::InterTest(BaseLayer<Sprite>* other) {
	if(other->GetName().compare("Static") == 0) { return; }
	if(other->GetName().compare("Sprite") == 0) {
		std::list<Sprite*>::iterator iSprite;
		std::list<Sprite*>::iterator iOther;
		for(iOther = other->m_members.begin(); iOther != other->m_members.end(); iOther++) {
			for(iSprite = m_members.begin(); iSprite != m_members.end(); iSprite++) {
				if(BoundingSphereCollision((*iOther), (*iSprite))) {
					(*iOther)->Collide((*iSprite));
					(*iSprite)->Collide((*iOther));
				}
			}
		}
	}
}

/// Test each sprite in this layer against every other.
void SpriteLayer::IntraTest() {
	if(m_members.size() <= 1) { return; }
	std::list<Sprite*>::iterator i1 = m_members.begin();
	std::list<Sprite*>::iterator i2 = i1;
	i2++;
	for(; i2 != m_members.end(); i1++, i2++) {
		if(BoundingSphereCollision((*i1), (*i2))) {
			(*i1)->Collide((*i2));
			(*i2)->Collide((*i1));
		}
	}
}
/// Default constructor.
CollisionManager::CollisionManager() { }

/// Sets the parent Level of this CM.
//void CollisionManager::SetParent(Level *p) { m_parent = p; }

/// Registers a physical entity into the layer 'layerName'.
/** Layer names are: Sprite, Wall, Exit.
*/
bool CollisionManager::RegisterEntity(std::string layerName, PhysicalEntity* entity) {
	if(layerName.compare("Sprite") == 0) {
		m_Sprites.RegisterEntity((Sprite*)entity);
		return true;
	}
	else if(layerName.compare("Wall") == 0) {
		m_Walls.RegisterEntity((Wall*)entity);
		return true;
	}
	else { return false; }
}

///Figures out what rooms are occupied by Sprites and
void CollisionManager::MakeRoomsActive() {
	//Clear all the walls from collision detector so we only add the ones we need.
	FreeLayer("Wall");

	// Only tests sprites, not static sprites.
	if(m_Sprites.GetName().compare("StaticSprite") == 0) { return; }
	if(m_Sprites.GetName().compare("Sprite") == 0) {
		std::list<Sprite*>::iterator iSprite;
		for(iSprite = m_Sprites.m_members.begin(); iSprite != m_Sprites.m_members.end(); iSprite++) {
			gLevel.ActivateRoomsWithSprite(*iSprite);
		}
	}
	else { return; }

}

/// Frees the specified layer (does not destroy objects)
bool CollisionManager::FreeLayer(std::string layerName) {
	if(layerName.compare("Sprite") == 0) {
		m_Sprites.FreeLayer();
		return true;
	}
	else if(layerName.compare("Wall") == 0) {
		m_Walls.FreeLayer();
		return true;
	}
	else { return false; }
}

/// Finds all collisions in the managed layers, and takes care of them.
void CollisionManager::FindAndResolveCollisions() {
	// Clear out all walls form the collision manager
	FreeLayer("Wall");

	// Put the appropriate walls in the collision detector
	MakeRoomsActive();

	// Test sprites against walls.
	m_Walls.InterTest(&m_Sprites);

	// Test sprites against each other.
	m_Sprites.IntraTest();
}