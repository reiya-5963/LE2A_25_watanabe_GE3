#include "Enemy.h"
#include "R_Math.h"
#include <cassert>

void Enemy::Initialize(const std::vector<Model*>& models) {
	//
	assert(models[ModelIndexIceWeapon]);
	objectName_ = int(ObjName::ENEMY);

	//
	BaseCharacter::Initialize(models);
	SetRadius({ 3.0f, 3.0f, 3.0f });
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(~kCollisionAttributeEnemy);
	objectWorldTrans_.translation_.x = -30.0f;
	objectWorldTrans_.translation_.y = 6.0f;
	objectWorldTrans_.translation_.z = 260.0f;

	//
	worldTransform_body_.Initialize();
	worldTransform_f_weapon_.Initialize();
	worldTransform_i_weapon_.Initialize();

	worldTransform_body_.parent_ = &objectWorldTrans_;
	worldTransform_f_weapon_.parent_ = &worldTransform_body_;
	worldTransform_f_weapon_.translation_.y += 1.5f;
	worldTransform_f_weapon_.translation_.x -= 1.5f;
	worldTransform_i_weapon_.parent_ = &worldTransform_body_;
	worldTransform_i_weapon_.translation_.y += 1.5f;
	worldTransform_i_weapon_.translation_.x += 1.5f;
	isDead_ = false;
}

void Enemy::Update() { 
	if (!isDead_) {
		objectWorldTrans_.rotation_.y += 0.08f;
		Vector3 tmpVelocity{ 0.0f, 0.0f, kMoveSpeed };

		velocity_ = R_Math::TransformNormal(tmpVelocity, objectWorldTrans_.matWorld_);

		objectWorldTrans_.translation_.x += velocity_.x;
		objectWorldTrans_.translation_.y += velocity_.y;
		objectWorldTrans_.translation_.z += velocity_.z;
	}
	



	BaseCharacter::Update();
	worldTransform_body_.UpdateMatrix();
	worldTransform_f_weapon_.UpdateMatrix();
	worldTransform_i_weapon_.UpdateMatrix();

	SetMin(R_Math::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(R_Math::Add(GetWorldPosition(), GetRadius()));
}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	models_[ModelIndexBody]->Draw(worldTransform_body_, viewProjection);
	models_[ModelIndexFireWepon]->Draw(worldTransform_f_weapon_, viewProjection);
	models_[ModelIndexIceWeapon]->Draw(worldTransform_i_weapon_, viewProjection);


}

void Enemy::OnCollisionEnter(int object) {
	if (object == int(ObjName::WEPON) && !isDead_ ) {
		isDead_ = true;
	}
}

//void Enemy::OnCollisionExit()
//{
//}

Vector3 Enemy::GetWorldPosition() {
	Vector3 result{};
	result.x = objectWorldTrans_.matWorld_.m[3][0];
	result.y = objectWorldTrans_.matWorld_.m[3][1];
	result.z = objectWorldTrans_.matWorld_.m[3][2];
	return result;
}
