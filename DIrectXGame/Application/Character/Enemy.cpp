#include "Enemy.h"
#include "R_Math.h"
#include <cassert>

void Enemy::Initialize(const std::vector<Model*>& models) {
	//
	assert(models[ModelIndexIceWeapon]);

	//
	BaseCharacter::Initialize(models);
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(~kCollisionAttributeEnemy);

	//
	worldTransform_body_.Initialize();
	worldTransform_f_weapon_.Initialize();
	worldTransform_i_weapon_.Initialize();

	worldTransform_body_.parent_ = &worldTrans_;
	worldTransform_f_weapon_.parent_ = &worldTransform_body_;
	worldTransform_f_weapon_.translation_.y += 1.5f;
	worldTransform_f_weapon_.translation_.x -= 1.5f;
	worldTransform_i_weapon_.parent_ = &worldTransform_body_;
	worldTransform_i_weapon_.translation_.y += 1.5f;
	worldTransform_i_weapon_.translation_.x += 1.5f;
}

void Enemy::Update() { 
	worldTrans_.rotation_.y += 0.01f;
	Vector3 tmpVelocity{0.0f, 0.0f, kMoveSpeed};

	velocity_ = R_Math::TransformNormal(tmpVelocity, worldTrans_.matWorld_);

	worldTrans_.translation_.x += velocity_.x;
	worldTrans_.translation_.y += velocity_.y;
	worldTrans_.translation_.z += velocity_.z;



	BaseCharacter::Update();
	worldTransform_body_.UpdateMatrix();
	worldTransform_f_weapon_.UpdateMatrix();
	worldTransform_i_weapon_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	models_[ModelIndexBody]->Draw(worldTransform_body_, viewProjection);
	models_[ModelIndexFireWepon]->Draw(worldTransform_f_weapon_, viewProjection);
	models_[ModelIndexIceWeapon]->Draw(worldTransform_i_weapon_, viewProjection);


}

void Enemy::OnCollisionEnter() {
}

//void Enemy::OnCollisionExit()
//{
//}

Vector3 Enemy::GetWorldPosition() {
	Vector3 result{};
	result.x = worldTrans_.matWorld_.m[3][0];
	result.y = worldTrans_.matWorld_.m[3][1];
	result.z = worldTrans_.matWorld_.m[3][2];
	return result;
}
