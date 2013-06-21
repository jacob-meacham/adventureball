#include "Ball.h"

void Ball::Collide(PhysicalEntity *other) {

}

bool Ball::checkType(EntityType e) {
	return (e == BALL);
}

void Ball::SetCollisionInfo(BoundingSphere p, Vector2 pt, float distance) {
	m_bs = p;
	m_fp = pt;
	m_dist = distance;
}

/// Performs a bounce on this Ball.  Bounces about a normal.
/** This bounce is completely inelastic. */
void Ball::Bounce(float x, float y) {
	float dot = m_Velocity.x * x + m_Velocity.y * y;
	m_Velocity.x -= 2 * dot * x;
	m_Velocity.y -= 2 * dot * y;
}