#include "FollowCamera.h"
#include "MyMath.h"
#include "input/Input.h"

void FollowCamera::Initialize() { 
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	
}

void FollowCamera::Update() {
	
	//XINPUT_STATE joyState;
	//// もしコントローラーでのプレイなら
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	//
	//	float speed = 0.2f;

	//	viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speed;

	//}
	//// そうでないならキーマウ
	//else {
		GetCursorPos(&mousePos_);

		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePos_);

		float speed = 0.003f;

		float mouseDistance = float(mousePos_.x) - float(preMousePos_.x);

		viewProjection_.rotation_.y += mouseDistance * speed;
		preMousePos_ = mousePos_;
	//}


	// もし追従対象がいれば
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 100.0f, -100.0f};

		Matrix4x4 rotateMat = MyMath::Multiply(
		    MyMath::Multiply(
		        MyMath::MakeRotateXMatrix(viewProjection_.rotation_.x),
		        MyMath::MakeRotateYMatrix(viewProjection_.rotation_.y)),
		    MyMath::MakeRotateZMatrix(viewProjection_.rotation_.z));

		offset = MyMath::TransformNormal(offset, rotateMat);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = MyMath::Add(target_->translation_, offset);
	}

	// ビュー行列の更新と転送
	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

}


void FollowCamera::SetTarget(const WorldTransform* target) {
	// 追従対象を設定
	target_ = target;
}

ViewProjection& FollowCamera::GetViewProjection() {
	return viewProjection_;
}
