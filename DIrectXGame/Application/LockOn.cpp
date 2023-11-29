#include "LockOn.h"
#include "Input.h"
#include "ImGuiManager.h"
void LockOn::Initialize() {
	uint32_t lockOnTex = TextureManager::Load("reticle.png");
	lockOnMark_.reset(Sprite::Create(lockOnTex, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));

}

void LockOn::Update(const std::list< std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewPro) {
	ImGui::Begin("debug : reticle");
	ImGui::Text("%f, %f", lockOnMark_->GetTranslate().x, lockOnMark_->GetTranslate().y);
	ImGui::End();
	// ロックオン状態なら
	if (target_) {
		//C
		// ロックオンボタンをトリガーしたら
		if (Input::GetInstance()->TriggerKey(DIK_G)) {
			//ロックオンを外す
			target_ = nullptr;
		}
		// 範囲外設定
		else if (Judge2RangeOut(viewPro)) {
			// ロックオンを外す
			target_ = nullptr;
		}
	}
	else {
		//A
		// ロックオンボタンをトリガーしたら
		if (Input::GetInstance()->TriggerKey(DIK_G)) {
			// ロックオン対象の検索
			SearchTarget(enemies, viewPro);
		}
	}

	// ロックオン状態なら
	if (target_) {
		//
		Vector3 positionWorld = target_->GetWorldPosition();

		Vector3 positionScreen = World2ScreenPos(positionWorld, viewPro);

		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);

		lockOnMark_->SetPosition(positionScreenV2);
	}
}

void LockOn::Draw() {


	if (target_) {
		lockOnMark_->Draw();
	}
}

void LockOn::SearchTarget(const std::list< std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewPro) {
	//1
	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// 全ての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		//敵のロックオン座標取得
		Vector3 positionWorld = enemy->GetWorldPosition();

		// ワールド->ビュー
		Vector3 positionView = R_Math::TransformCoord(positionWorld, viewPro.matView);

		// 距離条件チェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
			
			// カメラ前方との角度を計算
			float arcTangent = std::atan2(
				std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),
				positionView.z);

			// 角度条件チェック (コーンにおさまっているか)
			if (std::abs(arcTangent) <= angleRange_) {
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}
	}
	//2
	// ロックオン対象をリセット
	target_ = nullptr;
	if (targets.size() != 0) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象とする
		target_ = targets.front().second;
	}

	//3
	if (target_) {
		//
		Vector3 positionWorld = target_->GetWorldPosition();

		Vector3 positionScreen = World2ScreenPos(positionWorld, viewPro);

		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);

		lockOnMark_->SetPosition(positionScreenV2);
	}
}

bool LockOn::Judge2RangeOut(const ViewProjection& viewPro){
	//敵のロックオン座標取得
	Vector3 positionWorld = target_->GetWorldPosition();

	// ワールド->ビュー
	Vector3 positionView = R_Math::TransformCoord(positionWorld, viewPro.matView);

	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {

		// カメラ前方との角度を計算
		float arcTangent = std::atan2(
			std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),
			positionView.z);

		// 角度条件チェック (コーンにおさまっているか)
		if (std::abs(arcTangent) <= angleRange_) {
			return false;
		}
	}

	return true;
}

Vector3 LockOn::World2ScreenPos(Vector3 world, const ViewProjection& viewPro) {
	Matrix4x4 matViewport = R_Math::MakeViewPortMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	//
	Matrix4x4 matViewProjectionViewport = R_Math::Multiply(
		viewPro.matView, R_Math::Multiply(viewPro.matProjection, matViewport)
	);
	Vector3 positionReticle = R_Math::TransformCoord(world, matViewProjectionViewport);
	return positionReticle;
}

Vector3 LockOn::GetTargetPosition() const {
	if (target_) {
		return target_->GetWorldPosition();
	}
	return Vector3();
}

