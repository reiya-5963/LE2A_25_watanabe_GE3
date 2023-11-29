#include "Wepon.h"
#include <cassert>
#include "ImGuiManager.h"
void Wepon::Initialize(const std::vector<Model*>& models, WorldTransform* parent) {
	assert(models[0]);
	models_ = models;
	objectName_ = int(ObjName::WEPON);

	objectWorldTrans_.Initialize();
	assert(parent);
	parent_ = parent;
	objectWorldTrans_.parent_ = parent;
	//objectWorldTrans_.UpdateMatrix();

	Collider::Initialize();
	SetRadius({ 2.0f, 10.0f, 7.0f });

	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);


}

void Wepon::WorldTransformInitialize()
{
}

void Wepon::Update() {
	
	objectWorldTrans_.UpdateMatrix();
	UpdateWorldTransform();

	SetMin(R_Math::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(R_Math::Add(GetWorldPosition(), GetRadius()));

	
	Vector3 offset = { 0.0f, 10.0f, 0.0f };

	// 回転行列の生成
	Matrix4x4 rotateMat = R_Math::Multiply(
		R_Math::Multiply(
			R_Math::MakeRotateXMatrix(objectWorldTrans_.rotation_.x),
			R_Math::MakeRotateYMatrix(objectWorldTrans_.rotation_.y)),
		R_Math::MakeRotateZMatrix(objectWorldTrans_.rotation_.z));

	// 回転行列でどの方向を見ても同じ距離になるよう調整
	offset = R_Math::TransformNormal(offset, rotateMat);

	rotateMat = R_Math::Multiply(
		R_Math::Multiply(
			R_Math::MakeRotateXMatrix(playerRotate_.x),
			R_Math::MakeRotateYMatrix(playerRotate_.y)),
		R_Math::MakeRotateZMatrix(playerRotate_.z));

	// 回転行列でどの方向を見ても同じ距離になるよう調整
	offset = R_Math::TransformNormal(offset, rotateMat);


	colliderWorldTransform_.translation_ = R_Math::Add(playerPos_, offset);
	colliderWorldTransform_.rotation_ = playerRotate_;
	colliderWorldTransform_.UpdateMatrix();

	ImGui::Begin("wepon");
	ImGui::Text("%f, %f, %f", objectWorldTrans_.matWorld_.m[3][0], objectWorldTrans_.matWorld_.m[3][1], objectWorldTrans_.matWorld_.m[3][2]);
	ImGui::End();

}

void Wepon::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(objectWorldTrans_, viewProjection);
}

void Wepon::OnCollisionEnter(int ) {
}

Vector3  Wepon::GetWorldPosition() const {
	Vector3 result{};
	result.x = objectWorldTrans_.matWorld_.m[3][0];
	result.y = objectWorldTrans_.matWorld_.m[3][1];
	result.z = objectWorldTrans_.matWorld_.m[3][2];
	return result;
}
