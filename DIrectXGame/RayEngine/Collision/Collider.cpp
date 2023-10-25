#include "Collider.h"

void Collider::Initialize() {
	colliderWorldTransform_.Initialize();
}

void Collider::UpdateWorldTransform() {
	colliderWorldTransform_.translation_ = GetWorldPosition();
	colliderWorldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {
	model->Draw(colliderWorldTransform_, viewProjection);
}

void Collider::SetRadius(const Vector3 radius) {
	radius_ = radius;
	colliderWorldTransform_.scale_ = radius_;
	colliderWorldTransform_.matWorld_ = R_Math::MakeAffineMatrix(colliderWorldTransform_.scale_, colliderWorldTransform_.rotation_, colliderWorldTransform_.translation_);
}

void Collider::SetMin(const Vector3 min){
	min_ = min;
}

void Collider::SetMax(const Vector3 max) {
	max_ = max;
}

uint32_t Collider::GetCollisionAttribute() {
	return collisionAttribute_;
}

void Collider::SetCollisionAttribute(uint32_t collisionAttribute) {
	collisionAttribute_ = collisionAttribute;
}

uint32_t Collider::GetCollisionMask() {
	return CollisionMask_;
}

void Collider::SetCollisionMask(uint32_t collisionMask) {
	CollisionMask_ = collisionMask;
}
