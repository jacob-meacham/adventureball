#include "Ball.h"

Ball::Ball() {
	m_inventory.clear();
	m_activeItem = (Items)0;
}
/////////////////////////////////////////////////////////////////////////////
void Ball::Collide(PhysicalEntity *other) {
	
}
/////////////////////////////////////////////////////////////////////////////
bool Ball::checkType(EntityType e) const {
	if (e == BALL) return true;
	return Sprite::checkType(e);
}
/////////////////////////////////////////////////////////////////////////////
/// Performs a bounce on this Ball.  Bounces about a normal.
/** This bounce is completely inelastic. */
void Ball::Bounce(float x, float y) {
	float dot = m_Velocity.x * x + m_Velocity.y * y;
	m_Velocity.x -= 2 * dot * x;
	m_Velocity.y -= 2 * dot * y;
}
/////////////////////////////////////////////////////////////////////////////
//Adds an item to the inventory
void Ball::AddItem(Items item) {
	m_inventory.push_back(item);
}
/////////////////////////////////////////////////////////////////////////////
//Sets the location'th item to be the active item NOTE: If a location not in the
//inventory is supplied, the active item will not change.
void Ball::SetActiveItem(u32 location) {
	if(location < m_inventory.size())
		m_activeItem = (Items)location;
}
/////////////////////////////////////////////////////////////////////////////
//The next item in the inventory is set as the active item
void Ball::SetNextActiveItem() {
	u32 next_item = (u32)m_activeItem+1;
	if(next_item >= m_inventory.size())
		next_item = 0;

	m_activeItem = (Items)next_item;
}