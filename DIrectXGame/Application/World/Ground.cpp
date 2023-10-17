#include "MyMath.h"
#include "Ground.h"

/// <summary>
///
/// </summary>
/// <param name="model"></param>
/// <param name="position"></param>
void Ground::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTrans_.Initialize();
	worldTrans_.translation_ = position;

	Collider::Initialize();
	SetRadius({ 20.0f, 1.0f, 20.0f });
	SetCollisionAttribute(kCollisionAttributeWorld);
	SetCollisionMask(!kCollisionAttributeWorld);

}

/// <summary>
///
/// </summary>
void Ground::Update() {
	Matrix4x4 movetrans = MyMath::MakeTranslateMatrix(worldTrans_.translation_);
	const float speed = 1.0f;
	Vector3 move = { 0.0f, 0.0f, 0.0f };
	if (isMoveLeft_) {
		move.x = -1.0f;
	}
	else if (!isMoveLeft_) {
		move.x = 1.0f;
	}
	move = MyMath::Normalize(move);
	move.x *= speed;
	move.y *= speed;
	move.z *= speed;
	
	worldTrans_.translation_ = MyMath::TransformCoord(move, movetrans);

	worldTrans_.UpdateMatrix();

	SetMin(MyMath::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(MyMath::Add(GetWorldPosition(), GetRadius()));
	
	if (worldTrans_.matWorld_.m[3][0] >= 30.0f) {
		isMoveLeft_ = true;
	}
	else if (worldTrans_.matWorld_.m[3][0] <= -30.0f) {
		isMoveLeft_ = false;
	}

}

/// <summary>
///
/// </summary>
/// <param name="viewProjection"></param>
void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Ground::OnCollision()
{
}

Vector3 Ground::GetWorldPosition() {
	Vector3 result{};

	result.x = worldTrans_.matWorld_.m[3][0];
	result.x = worldTrans_.matWorld_.m[3][0];
	result.x = worldTrans_.matWorld_.m[3][0];
	return result;
}
