#include "Player.h"
#include "ImGuiManager.h"
#include "R_Math.h"
#include "GlobalVariables.h"
#include <cassert>

const std::array<Player::ConstAttack, Player::ComboNum>
Player::kConstAttacks_ = {
	{
		{0, 0, 20, 0, 0.0f, 0.0f, 0.15f},
		{15, 10, 15, 0, 0.2f, 0.0f, 0.0f},
		{15, 10, 15, 30, 0.2f, 0.0f, 0.0f},
	}
};

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(const std::vector<Model*>& models, const std::vector<Model*>& weponModels) {
	// NULLチェック
	assert(models[ModelIndexR_Arm]);
	objectName_ = int(ObjName::PLAYER);
	// インプット系の初期化
	//input_ = Input::GetInstance();

	// ベース部分の初期化
	BaseCharacter::Initialize(models);
	objectWorldTrans_.translation_.x = 0.0f;
	objectWorldTrans_.translation_.y = 50.0f;
	objectWorldTrans_.translation_.z = 0.0f;

	wepon_ = std::make_unique<Wepon>();

	WorldTransformInitialize();

	wepon_->Initialize(weponModels, &worldTransform_body_);
	//****************//
	// 各ギミックの初期化
	InitializeFloatingGimmick();
	InitializeArmGimmick();
	objectWorldTrans_.rotation_.y = std::atan2(objectWorldTrans_.rotation_.x, objectWorldTrans_.rotation_.z);

	SetRadius({ 1.0f, 3.0f, 1.0f });
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);

	InitializeGlovalVariables();
}

/// <summary>
/// 更新
/// </summary>
void Player::Update() {
	
	ApplyGlobalVariavles();

	if (isRespown_) {
		objectWorldTrans_.translation_.x = 0.0f;
		objectWorldTrans_.translation_.y = 50.0f;
		objectWorldTrans_.translation_.z = 0.0f;
		objectWorldTrans_.UpdateMatrix();
		isRespown_ = false;
	}

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
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		case Behavior::kJump:
			BehaviorJumpInitialize();
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
	case Behavior::kDash:
		BehaviorDashUpdate();
		break; 
	case Behavior::kJump:
		BehaviorJumpUpdate();
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
	//worldTransform_wepon_.UpdateMatrix();
	wepon_->SetPlayerPos(GetWorldPosition());
	wepon_->SetPlayerRotate(objectWorldTrans_.rotation_);
	wepon_->Update();

	SetMin(R_Math::Subtract(GetWorldPosition(), GetRadius()));
	SetMax(R_Math::Add(GetWorldPosition(), GetRadius()));
	
	isOnGround_ = false;
	if (objectWorldTrans_.translation_.y <= -50.0f) {
		objectWorldTrans_.translation_.x = 0.0f;
		objectWorldTrans_.translation_.y = 50.0f;
		objectWorldTrans_.translation_.z = 0.0f;
		objectWorldTrans_.UpdateMatrix();
	}

	ImGui::Text("%f, %f, %f", objectWorldTrans_.translation_.x, objectWorldTrans_.translation_.y, objectWorldTrans_.translation_.z);


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
		//models_[ModelIndexWepon]->Draw(worldTransform_wepon_, viewProjection);
		wepon_->Draw(viewProjection);
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
		isRespown_ = true;
	}
}

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

void Player::InitializeGlovalVariables() {
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

void Player::WorldTransformInitialize() {

	// 各部位のワールドトランスフォーム初期化
	worldTransform_body_.Initialize();
	worldTransform_head_.Initialize();
	worldTransform_l_arm_.Initialize();
	worldTransform_r_arm_.Initialize();
	//worldTransform_wepon_.Initialize();

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


}

#pragma region ギミック
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
	attackCount_ = 0;
	isAttack_ = false;
	isAtkFinish_ = false;

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
	wepon_->SetRotation({ 0.0f, 0.0f, 0.0f });

	//worldTransform_wepon_.rotation_.x = 0.0f;
	//worldTransform_wepon_.rotation_.x = 0.0f;
}

void Player::UpdateAttackWeponGimmick() {
	if (!isAttack_) {
		/*if (worldTransform_wepon_.rotation_.x < 1.0f) {
			worldTransform_wepon_.rotation_.x += 0.2f;
			worldTransform_wepon_.rotation_.x += 0.2f;
		}*/
		if (wepon_->GetRotation().x < 1.2f) {
			Vector3 tmpRotate = wepon_->GetRotation();
			tmpRotate.x += 0.4f;
			
			wepon_->SetRotation(tmpRotate);
		}
	}
}

#pragma endregion

#pragma region 通常

void Player::BehaviorRootInitialize() {
	// 各ギミックの初期化
	InitializeFloatingGimmick();
	InitializeArmGimmick();
	objectWorldTrans_.rotation_.y = std::atan2(objectWorldTrans_.rotation_.x, objectWorldTrans_.rotation_.z);
	isAttack_ = true;
	isAtkFinish_ = true;
}

void Player::BehaviorRootUpdate() {
	Matrix4x4 movetrans = R_Math::MakeTranslateMatrix(objectWorldTrans_.translation_);
	 velocity_ = { 0, 0, 0 };
	XINPUT_STATE joyState;

	// もしコントローラーでのプレイなら
	if (Input::GetInstance()->GetJoyStickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		velocity_ = { (float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY };

		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
			behaviorRequest_ = Behavior::kAttack;
		}
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_B) {
			behaviorRequest_ = Behavior::kJump;
			/*isJump_ = true;
			jumpPower_ = 6.0f;*/
		}

		//
		velocity_ = R_Math::Normalize(velocity_);
		velocity_.x *= speed;
		/*if (jumpPower_ < 0.0f) {
			move.y = 0.0f;
			isJump_ = false;
		}
		else if (jumpPower_ >= 0.0f) {
			jumpPower_ -= 0.4f;

		}*/
		velocity_.z *= speed;

		Matrix4x4 moveMat = R_Math::MakeTranslateMatrix(velocity_);
		Matrix4x4 rotateMat = R_Math::Multiply(
			R_Math::Multiply(
				R_Math::MakeRotateXMatrix(viewProjection_->rotation_.x),
				R_Math::MakeRotateYMatrix(viewProjection_->rotation_.y)),
			R_Math::MakeRotateZMatrix(viewProjection_->rotation_.z));

		moveMat = R_Math::Multiply(moveMat, rotateMat);
		velocity_.x = moveMat.m[3][0];
		velocity_.y = jumpPower_;
		velocity_.z = moveMat.m[3][2];
		// 位置の移動
		objectWorldTrans_.translation_ = R_Math::TransformCoord(velocity_, movetrans);

		objectWorldTrans_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	}
	else {

		// 速さ
		const float speed = 0.6f;

		if (input_->PushKey(DIK_W)) {
			velocity_.z = 1.0f;
		}
		if (input_->PushKey(DIK_S)) {
			velocity_.z = -1.0f;
		}
		if (input_->PushKey(DIK_A)) {
			velocity_.x = -1.0f;
		}
		if (input_->PushKey(DIK_D)) {
			velocity_.x = 1.0f;
		}
		if (input_->PushKey(DIK_LSHIFT)) {
			behaviorRequest_ = Behavior::kDash;
		}
		
		if (input_->IsTriggerMouse(0)) {
			behaviorRequest_ = Behavior::kAttack;
		}
		if (input_->TriggerKey(DIK_SPACE)) {
			behaviorRequest_ = Behavior::kJump; 
			
			/*isJump_ = true;
			jumpPower_ = 6.0f;*/

		}

		velocity_ = R_Math::Normalize(velocity_);
		velocity_.x *= speed;
		velocity_.z *= speed;

		/*move.x *= dash_;
		move.z *= dash_;*/
		/*if (jumpPower_ < 0.0f) {
			move.y = 0.0f;
			isJump_ = false;
		}
		else if (jumpPower_ >= 0.0f) {
			jumpPower_ -= 0.4f;

		}*/

		Matrix4x4 moveMat = R_Math::MakeTranslateMatrix(velocity_);
		Matrix4x4 rotateMat = R_Math::Multiply(
			R_Math::Multiply(
				R_Math::MakeRotateXMatrix(viewProjection_->rotation_.x),
				R_Math::MakeRotateYMatrix(viewProjection_->rotation_.y)),
			R_Math::MakeRotateZMatrix(viewProjection_->rotation_.z));

		moveMat = R_Math::Multiply(moveMat, rotateMat);
		velocity_.x = moveMat.m[3][0];
		velocity_.y = jumpPower_;
		velocity_.z = moveMat.m[3][2];

		objectWorldTrans_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	
		// 位置の移動
		objectWorldTrans_.translation_ = R_Math::TransformCoord(velocity_, movetrans);

}

	// 各ギミックの更新処理
	UpdateFloatingGimmick();
	UpdateArmGimmick();

}

#pragma endregion

#pragma region 攻撃

void Player::BehaviorAttackInitialize() {
	workAttack_.attackParamater_ = 0;
}

void Player::BehaviorAttackUpdate() {
	// 予備動作の時間
	//uint32_t anticipationTime = kConstAttacks_[workAttack_.comboIndex].anticipationTime;

	// コントローラー
	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	// コンボ上限に達していない
	if (ComboNum > workAttack_.comboIndex) {
		if (Input::GetInstance()->GetJoyStickState(0, joyState) && 
			Input::GetInstance()->GetJoyStickStatePrevious(0, joyStatePre)) {
			
			// 攻撃ボタンをトリガーしたら
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
				// コンボ有効
				workAttack_.comboNext = true;
			}
			
		}
	}

	// 1コンボの合計時間
	uint32_t samTime = kConstAttacks_[workAttack_.comboIndex].anticipationTime +
		kConstAttacks_[workAttack_.comboIndex].changeTime +
		kConstAttacks_[workAttack_.comboIndex].recoveryTime +
		kConstAttacks_[workAttack_.comboIndex].swingtime;

	// 既定の時間経過で通常行動に戻る
	if (++workAttack_.attackParamater_ >= samTime) {
		// コンボ継続なら次のコンボに進む
		if (workAttack_.comboNext) {
			// コンボ継続フラグをリセット
			workAttack_.comboNext = false;

			// 攻撃用の変数などを初期化
			InitializeAttackArmGimmick();
			InitializeAttackWeponGimmick();

			// コンボ切り替わりの瞬間だけ入力を受け付ける

			// 各パーツの角度などの初期化

		}
		// コンボ継続でないなら攻撃を終了して通常状態に戻る
		else {
			behaviorRequest_ = Behavior::kRoot;
		}
	}

	switch (workAttack_.comboIndex) {
	case 0:
		UpdateAttackArmGimmick();
		UpdateAttackWeponGimmick();

		// コンボ0用
		break;
	case 1:		
		UpdateAttackArmGimmick();
		UpdateAttackWeponGimmick();


		// コンボ1用
		break;
	case 2:
	default:
		UpdateAttackArmGimmick();
		UpdateAttackWeponGimmick();


		// コンボ2用
		break;

	}


}

#pragma endregion

#pragma region ダッシュ

void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	
}

void Player::BehaviorDashUpdate() {
	Matrix4x4 movetrans = R_Math::MakeTranslateMatrix(objectWorldTrans_.translation_);

	Vector3 direction = {0.0f, 0.0f, 1.0f};
	const float speed = 10.0f;
	direction.z *= speed;

	Matrix4x4 moveMat = R_Math::MakeTranslateMatrix(direction);

	Matrix4x4 rotateMat = R_Math::Multiply(
		R_Math::Multiply(
			R_Math::MakeRotateXMatrix(objectWorldTrans_.rotation_.x),
			R_Math::MakeRotateYMatrix(objectWorldTrans_.rotation_.y)),
		R_Math::MakeRotateZMatrix(objectWorldTrans_.rotation_.z));

	moveMat = R_Math::Multiply(moveMat, rotateMat);
	direction.x = moveMat.m[3][0];
	direction.y = 0.0f;
	direction.z = moveMat.m[3][2];

	objectWorldTrans_.translation_ = R_Math::TransformCoord(direction, movetrans);

	const uint32_t behaviorDashTime = 3;
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
}
#pragma endregion

#pragma region ジャンプ
void Player::BehaviorJumpInitialize() {
	worldTransform_body_.translation_.y = 0.0f;
	worldTransform_l_arm_.rotation_.x = 0.0f;
	worldTransform_r_arm_.rotation_.x = 0.0f;

	// ジャンプ初速
	const float kJumpFirstSpeed = 2.0f;
	// ジャンプ初速を与える
	velocity_.y = kJumpFirstSpeed;
}

void Player::BehaviorJumpUpdate() {
	// 移動
	objectWorldTrans_.translation_.x += velocity_.x;
	objectWorldTrans_.translation_.y += velocity_.y;
	objectWorldTrans_.translation_.z += velocity_.z;

	// 重力加速度
	const float kGravityAcceleration = 0.05f;
	// 加速度ベクトル
	Vector3 accelerationVector = { 0.0f, -kGravityAcceleration, 0.0f };
	// 加速する
	velocity_.x += accelerationVector.x;
	velocity_.y += accelerationVector.y;
	velocity_.z += accelerationVector.z;


	// 着地
	if (objectWorldTrans_.translation_.y <= 5.1f) {
		objectWorldTrans_.translation_.y = 5.1f;
		// ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}
}

#pragma endregion


