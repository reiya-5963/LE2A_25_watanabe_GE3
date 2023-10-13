#include "Collider.h"

void Collider::SetRadius(const float radius) {
	radius_ = radius;
}

uint32_t Collider::GetCollisionAttribute() {
	return 0;
}

void Collider::SetCollisionAttribute(uint32_t collisionAttribute) {
	collisionAttribute_ = collisionAttribute;
}

uint32_t Collider::GetCollisionMask() {
	return 0;
}

void Collider::SetCollisionMask(uint32_t collisionMask) {
	CollisionMask_ = collisionMask;
}
