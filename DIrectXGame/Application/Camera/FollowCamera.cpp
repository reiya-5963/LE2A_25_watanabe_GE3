#include "FollowCamera.h"
#include "R_Math.h"
#include "input/Input.h"
#include "GlobalVariables.h"
#include "LockOn.h"
#include <cmath>

void FollowCamera::Initialize() {
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables;
	const char* groupName = "FollowCamera";
	//
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "t_", t_);

}

void FollowCamera::Update() {
	XINPUT_STATE joyState;

	if (isRespown_) {
		Reset();
	}

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	t_ = globalVariables->GetFloatValue(groupName, "t_");

	if (lockOn_->ExistTarget()) {
		// ロックオン座標
		Vector3 lockOnTargetPosition = lockOn_->GetTargetPosition();
		
		Vector3 TargetWorldPosition = { target_->matWorld_.m[3][0],target_->matWorld_.m[3][1],target_->matWorld_.m[3][2] };
		
		// 追従対象からロックオン対象へのベクトル
		Vector3 sub = TargetWorldPosition - lockOnTargetPosition;

		// y軸周りの角度
		viewProjection_.rotation_.y = std::atan2(sub.x, sub.z);
	}
	else {
		// もしコントローラーでのプレイなら
		if (Input::GetInstance()->GetJoyStickState(0, joyState)) {
			//
			float speed = 0.2f;

			/*viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speed;
			viewProjection_.rotation_.x -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * speed;*/

			destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speed;

			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_THUMB) {
				destinationAngleY_ = 0.0f;
			}

		}
		else {

			// 現在のマウス位置を取得
			GetCursorPos(&mousePos_);

			HWND hwnd = WinApp::GetInstance()->GetHwnd();
			ScreenToClient(hwnd, &mousePos_);

			float speed = 0.016f;

			float mouseDistance = float(mousePos_.x) - float(preMousePos_.x);
			//float mouseDistancey = float(mousePos_.y) - float(preMousePos_.y);

			destinationAngleY_ += mouseDistance * speed;

			if (Input::GetInstance()->PushKey(DIK_RETURN)) {
				destinationAngleY_ = 0.0f;
			}

			preMousePos_ = mousePos_;
		}
	}

	viewProjection_.rotation_.y = R_Math::lerp(t_, viewProjection_.rotation_.y, destinationAngleY_);
	// もし追従対象がいれば
	if (target_) {
		interTarget_ = R_Math::lerp(t_, interTarget_, target_->translation_);


		// ターゲットとカメラの距離
		if (isFps_) {
			offset_ = fpsView_;
		}
		else if (!isFps_) {
			offset_ = tpsView_;
		}

		

		offset_ = CalcOffset();

		// ターゲットから調整した距離分を加算
		viewProjection_.translation_ = R_Math::Add(interTarget_, offset_);
	}

	// ビュー行列の更新と転送
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

}


void FollowCamera::SetTarget(const WorldTransform* target) {
	// 追従対象を設定
	target_ = target;
	Reset();
}

ViewProjection& FollowCamera::GetViewProjection() {
	return viewProjection_;
}

void FollowCamera::Reset() {
	if (target_) {
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}
	offset_ = CalcOffset();
	viewProjection_.translation_ = interTarget_ + offset_;
}

Vector3 FollowCamera::CalcOffset() const {
	Vector3 offset = offset_;
	// 回転行列の生成
	Matrix4x4 rotateMat = R_Math::Multiply(
		R_Math::Multiply(
			R_Math::MakeRotateXMatrix(viewProjection_.rotation_.x),
			R_Math::MakeRotateYMatrix(viewProjection_.rotation_.y)),
		R_Math::MakeRotateZMatrix(viewProjection_.rotation_.z));
	
	// 回転行列でどの方向を見ても同じ距離になるよう調整
	offset = R_Math::TransformNormal(offset_, rotateMat);

	return offset;
}
