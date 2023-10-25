#include "Math.h"
#include "Scaffold.h"
#include "ImGuiManager.h"
/// <summary>
///
/// </summary>
/// <param name="model"></param>
/// <param name="position"></param>
void Scaffold::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	objectName_ = int(ObjName::WORLD);

	objectWorldTrans_.Initialize();
	objectWorldTrans_.translation_ = position;
	objectWorldTrans_.scale_ = { 60.0f, 5.0f, 60.0f };
	//worldTrans_.UpdateMatrix();

	Collider::Initialize();
	SetRadius({ 60.0f, 5.0f, 60.0f });
	SetCollisionAttribute(kCollisionAttributeWorld);
	SetCollisionMask(~kCollisionAttributeWorld);
	//isMove_ = false;
}

/// <summary>
///
/// </summary>
void Scaffold::Update() {
	//isMove_ = false;

	Matrix4x4 movetrans = R_Math::MakeTranslateMatrix(objectWorldTrans_.translation_);
	const float speed = 0.7f;
	Vector3 move = { 0.0f, 0.0f, 0.0f };
	ImGui::Begin("Debug");

	ImGui::End();

	if (isMove_) {
		if (isMoveLeft_) {
			move.x = -1.0f;
		}
		else if (!isMoveLeft_) {
			move.x = 1.0f;
		}
	}
	else {
		if (isMoveLeft_) {
			move.z = -1.0f;
		}
		else if (!isMoveLeft_) {
			move.z = 1.0f;
		}
	}
	move = R_Math::Normalize(move);
	move.x *= speed;
	move.y *= speed;
	move.z *= speed;

	objectWorldTrans_.translation_ = R_Math::TransformCoord(move, movetrans);
	//ImGui::Text("%f, %f, %f", worldTrans_.matWorld_.m[3][0], worldTrans_.matWorld_.m[3][1], worldTrans_.matWorld_.m[3][2]);
	ImGui::SliderFloat3("position", &objectWorldTrans_.translation_.x, 0.0f, 400.0f);

	objectWorldTrans_.UpdateMatrix();
	UpdateWorldTransform();

	SetMin(R_Math::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(R_Math::Add(GetWorldPosition(), GetRadius()));
	if (isMove_) {
		if (objectWorldTrans_.matWorld_.m[3][0] >= 30.0f) {
			isMoveLeft_ = true;
		}
		else if (objectWorldTrans_.matWorld_.m[3][0] <= -30.0f) {
			isMoveLeft_ = false;
		}
	}
	else {
		if (objectWorldTrans_.matWorld_.m[3][2] >= 30.0f) {
			isMoveLeft_ = true;
		}
		else if (objectWorldTrans_.matWorld_.m[3][2] <= -30.0f) {
			isMoveLeft_ = false;
		}
	}


}

/// <summary>
///
/// </summary>
/// <param name="viewProjection"></param>
void Scaffold::Draw(ViewProjection& viewProjection) {
	model_->Draw(colliderWorldTransform_, viewProjection);
}

void Scaffold::SetIsMove(bool isMove) {
	isMove_ = isMove;
}

void Scaffold::OnCollisionEnter(int)
{
}

void Scaffold::OnCollisionExit()
{
}

Vector3 Scaffold::GetWorldPosition() {
	Vector3 result{};

	result.x = objectWorldTrans_.matWorld_.m[3][0];
	result.y = objectWorldTrans_.matWorld_.m[3][1];
	result.z = objectWorldTrans_.matWorld_.m[3][2];
	return result;
}
