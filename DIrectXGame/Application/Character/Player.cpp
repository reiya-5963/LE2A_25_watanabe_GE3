#include "Player.h"
#include "ImGuiManager.h"
#include "R_Math.h"
#include "GlobalVariables.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(const std::vector<Model*>& models) {
	// NULLチェック
	assert(models[ModelIndexR_Arm]);
	objectName_ = int(ObjName::PLAYER);
	// インプット系の初期化
	input_ = Input::GetInstance();

	// ベース部分の初期化
	BaseCharacter::Initialize(models);
	objectWorldTrans_.translation_.x = 0.0f;
	objectWorldTrans_.translation_.y = 50.0f;
	objectWorldTrans_.translation_.z = 0.0f;

	// 各部位のワールドトランスフォーム初期化
	worldTransform_body_.Initialize();
	worldTransform_head_.Initialize();
	worldTransform_l_arm_.Initialize();
	worldTransform_r_arm_.Initialize();
	worldTransform_wepon_.Initialize();

	// 各部位の位置初期化 //
	worldTransform_body_.parent_ = &objectWorldTrans_;
	worldTransform_head_.parent_ = &worldTransform_body_;
	worldTransform_head_.translation_.y += 4.3f;

	worldTransform_l_arm_.parent_ = &worldTransform_body_;
	worldTransform_l_arm_.translation_.y += 4.0f;
	worldTransform_l_arm_.translation_.x -= 1.0f;

	worldTransform_r_arm_.parent_ = &worldTransform_body_;
	worldTransform_r_arm_.translation_.y += 4.0f;
	worldTransform_r_arm_.translation_.x += 1.0f;

	worldTransform_wepon_.parent_ = &worldTransform_body_;
	//****************//
	// 各ギミックの初期化
	InitializeFloatingGimmick();
	InitializeArmGimmick();
	objectWorldTrans_.rotation_.y = std::atan2(objectWorldTrans_.rotation_.x, objectWorldTrans_.rotation_.z);

	SetRadius({ 1.0f, 3.0f, 1.0f });
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables;
	const char* groupName = "Player";
	//
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Head Translation", worldTransform_head_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransform_l_arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransform_r_arm_.translation_);
	globalVariables->AddItem(groupName, "floatingCycle", floatingPeriod_);
	globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude);

}

void Player::BehaviorRootInitialize() {
	// 各ギミックの初期化
	InitializeFloatingGimmick();
	InitializeArmGimmick();
	objectWorldTrans_.rotation_.y = std::atan2(objectWorldTrans_.rotation_.x, objectWorldTrans_.rotation_.z);
}

void Player::BehaviorAttackInitialize() {
	attackCount_ = 0;
	isAttack_ = false;
	isAtkFinish_ = false;
	InitializeAttackArmGimmick();
	InitializeAttackWeponGimmick();
}


/// <summary>
/// 更新
/// </summary>
void Player::Update() {
	
	ApplyGlobalVariavles();

	if (parent_ != nullptr) {
		ImGui::Text("%f, %f, %f", parent_->translation_.x, parent_->translation_.y, parent_->translation_.z);
	}
	else {
		ImGui::Text("not parent");
	}

	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;

		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	//のっていないとき
	if (!isOnGround_) {
		objectWorldTrans_.translation_.y -= 1.0f;
		parent_ = nullptr;
		objectWorldTrans_.parent_ = nullptr;
	}
	else {
		if (parent_ != nullptr) {
			WorldTransform tmp;
			tmp.Initialize();
			tmp.scale_ = { 1.0f, 1.0f, 1.0f };

			tmp.translation_.x = parent_->translation_.x;
			tmp.translation_.y = parent_->translation_.y;

			tmp.rotation_.x = -parent_->rotation_.x;
			tmp.rotation_.y = -parent_->rotation_.y;
			tmp.rotation_.z = -parent_->rotation_.z;

			tmp.UpdateMatrix();
			objectWorldTrans_.parent_ = &tmp;
		}
	}


	// ベース部分の更新処理
	BaseCharacter::Update();

	// 各部位のワールドトランスフォームの更新処理
	worldTransform_body_.UpdateMatrix();
	worldTransform_head_.UpdateMatrix();
	worldTransform_l_arm_.UpdateMatrix();
	worldTransform_r_arm_.UpdateMatrix();
	worldTransform_wepon_.UpdateMatrix();

	SetMin(R_Math::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(R_Math::Add(GetWorldPosition(), GetRadius()));
	
	isOnGround_ = false;
	if (objectWorldTrans_.translation_.y <= -50.0f) {
		objectWorldTrans_.translation_.x = 0.0f;
		objectWorldTrans_.translation_.y = 50.0f;
		objectWorldTrans_.translation_.z = 0.0f;
		objectWorldTrans_.UpdateMatrix();
	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Player::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	models_[ModelIndexBody]->Draw(worldTransform_body_, viewProjection);
	models_[ModelIndexHead]->Draw(worldTransform_head_, viewProjection);
	models_[ModelIndexL_Arm]->Draw(worldTransform_l_arm_, viewProjection);
	models_[ModelIndexR_Arm]->Draw(worldTransform_r_arm_, viewProjection);
	if (behavior_ == Behavior::kAttack) {
		models_[ModelIndexWepon]->Draw(worldTransform_wepon_, viewProjection);

	}
}

void Player::OnCollisionEnter(int object) {
	if (object == int(ObjName::WORLD)) {
		if (!isOnGround_) {
			objectWorldTrans_.translation_.y = parent_->translation_.y + 5.0f;
			isOnGround_ = true;
		}
	}
	else if (object == int(ObjName::ENEMY)) {
		objectWorldTrans_.translation_.x = 0.0f;
		objectWorldTrans_.translation_.y = 50.0f;
		objectWorldTrans_.translation_.z = 0.0f;
		objectWorldTrans_.UpdateMatrix();

	}

}

//void Player::OnCollisionExit(){
//	if (isOnGround_) {
//		isOnGround_ = false;
//	}
//	worldTransform_.parent_ = nullptr;
//}

Vector3 Player::GetWorldPosition() {
	Vector3 result{};

	Vector3 offset = { 0.0f, 3.0f, 0.0f };
	//

	result = R_Math::TransformCoord(offset, objectWorldTrans_.matWorld_);
	return result;
}

void Player::SetViewProjection(const ViewProjection* viewProjection) {
	viewProjection_ = viewProjection;
}

void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;

	floatingPeriod_ = 60.0f;

	floatingAmplitude = 0.5f;
}

void Player::UpdateFloatingGimmick() {
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / floatingPeriod_;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを越えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * 3.14f);

	// 浮遊を座標に反映
	worldTransform_body_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude;
}

void Player::InitializeArmGimmick() {
	armParameter_ = 0.0f;
	armPeriod_ = 40.0f;
	armAmplitude = 0.15f;
}

void Player::UpdateArmGimmick() {
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / armPeriod_;

	// パラメータを1ステップ分加算
	armParameter_ += step;
	// 2πを越えたら0に戻す
	armParameter_ = std::fmod(armParameter_, 2.0f * 3.14f);

	// 浮遊を座標に反映
	worldTransform_l_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude;
	worldTransform_r_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude;
}

void Player::InitializeAttackArmGimmick() {
	worldTransform_l_arm_.rotation_.x = 3.14f;
	worldTransform_r_arm_.rotation_.x = 3.14f;
}

void Player::UpdateAttackArmGimmick() {
	if (!isAttack_) {
		if (worldTransform_l_arm_.rotation_.x < (3.14f * 1.5f)) {
			worldTransform_l_arm_.rotation_.x += 0.5f;
			worldTransform_r_arm_.rotation_.x += 0.5f;

		}
		else {
			isAttack_ = true;
		}
	}
	else if (isAttack_) {
		if (attackCount_ >= kMaxAtkCount) {
			isAtkFinish_ = true;
		}
		else if (attackCount_ < kMaxAtkCount) {
			attackCount_++;
		}
	}
	if (isAtkFinish_) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::InitializeAttackWeponGimmick() {
	worldTransform_wepon_.rotation_.x = 0.0f;
	worldTransform_wepon_.rotation_.x = 0.0f;
}

void Player::UpdateAttackWeponGimmick() {
	if (!isAttack_) {
		if (worldTransform_wepon_.rotation_.x < 1.0f) {
			worldTransform_wepon_.rotation_.x += 0.2f;
			worldTransform_wepon_.rotation_.x += 0.2f;
		}
	}
}

void Player::BehaviorRootUpdate() {
	Matrix4x4 movetrans = R_Math::MakeTranslateMatrix(objectWorldTrans_.translation_);
	Vector3 move = { 0, 0, 0 };

	
	// 速さ
	const float speed = 0.3f;

	if (input_->PushKey(DIK_W)) {
		move.z = 1.0f;
	}
	if (input_->PushKey(DIK_S)) {
		move.z = -1.0f;
	}
	if (input_->PushKey(DIK_A)) {
		move.x = -1.0f;
	}
	if (input_->PushKey(DIK_D)) {
		move.x = 1.0f;
	}
	if (input_->IsTriggerMouse(0)) {
		behaviorRequest_ = Behavior::kAttack;
	}
	if (!isJump_ && input_->PushKey(DIK_SPACE)) {
		isJump_ = true;
	}
	if (isJump_) {
		move.y = 1.0f;
		isJump_ = false;
	}

	move = R_Math::Normalize(move);
	move.x *= speed;
	move.y *= 5.0f;
	move.z *= speed;

	Matrix4x4 moveMat = R_Math::MakeTranslateMatrix(move);
	Matrix4x4 rotateMat = R_Math::Multiply(
		R_Math::Multiply(
			R_Math::MakeRotateXMatrix(viewProjection_->rotation_.x),
			R_Math::MakeRotateYMatrix(viewProjection_->rotation_.y)),
		R_Math::MakeRotateZMatrix(viewProjection_->rotation_.z));

	moveMat = R_Math::Multiply(moveMat, rotateMat);
	move.x = moveMat.m[3][0];
	move.y;
	move.z = moveMat.m[3][2];

	objectWorldTrans_.rotation_.y = std::atan2(move.x, move.z);

	// 位置の移動
	objectWorldTrans_.translation_ = R_Math::TransformCoord(move, movetrans);

	// 各ギミックの更新処理
	UpdateFloatingGimmick();
	UpdateArmGimmick();

}

void Player::BehaviorAttackUpdate() {
	UpdateAttackArmGimmick();
	UpdateAttackWeponGimmick();
}

void Player::ApplyGlobalVariavles() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransform_head_.translation_ =
		globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransform_l_arm_.translation_ =
		globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransform_r_arm_.translation_ =
		globalVariables->GetVector3Value(groupName, "ArmR Translation");
	floatingPeriod_ = globalVariables->GetFloatValue(groupName, "floatingCycle");
	floatingAmplitude = globalVariables->GetFloatValue(groupName, "floatingAmplitude");
}
