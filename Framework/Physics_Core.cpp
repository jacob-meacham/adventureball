#include <string>
#include "Physics_Core.h"
#include "Sprite.h"

/// Setter to set the name of this entity.
void PhysicalEntity::SetName(std::string name) { m_Name = name; }

/// Setter to set the interactive state of this entity.
void PhysicalEntity::SetIsInteractive(bool i) { m_interactive = i; }

/// Getter to return the interactive state of this entity.
bool PhysicalEntity::GetIsInteractive() { return m_interactive; }

/// Getter to return the name of this entity.
std::string PhysicalEntity::GetName() { return m_Name; }

/// Moves the bounding sphere of this entity by a translation vector
void PhysicalEntity::Move(float x, float y) { m_BoundingSphere.center.x += x; m_BoundingSphere.center.y += y; }

/// Set the X velocity of this entity.
void PhysicalEntity::SetXVel(float x) {
	m_Velocity.m_x = x;
}

/// Set the Y velocity of this entity.
void PhysicalEntity::SetYVel(float y) {
	m_Velocity.m_y = y;
}

/// Set the X and Y velocity of this entity.
void PhysicalEntity::SetXYVel(float x, float y) {
	m_Velocity.m_x = x;
	m_Velocity.m_y = y;
}

/// Getter to return the X velocity of this entity.
float PhysicalEntity::GetXVel() { return m_Velocity.m_x; }

/// Getter to return the name of this entity.
float PhysicalEntity::GetYVel() { return m_Velocity.m_y; }

/// Getter to return the name of this entity.  Pass in 2 floats for the velocity.
void PhysicalEntity::GetXYVel(float &x, float &y) {
	x = m_Velocity.m_x;
	y = m_Velocity.m_y;
}

//Checks if this PhysicalEntity is of type e. Returns true if that is the case
bool PhysicalEntity::checkType(EntityType e) {
	return (e == PHYSICALENTITY);
}