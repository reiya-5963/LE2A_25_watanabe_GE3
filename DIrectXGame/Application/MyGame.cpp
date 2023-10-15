#include "MyGame.h"

void MyGame::Initialize() {
	Framework::Initialize();

	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();

	
}

void MyGame::Finalize() {

	Framework::Finalize();
}

void MyGame::Update() {
	Framework::PreUpdate();

	// グローバル変数の更新
	GlobalVariables::GetInstance()->Update();

	Framework::PostUpdate();
}

void MyGame::Draw() {
	// 描画前処理
	dxCommon_->preDraw();

	// シーンの描画
	sceneManager_->Draw();

	// ImGui描画処理
	imGuiManager_->Draw();

	// 描画後処理
	dxCommon_->postDraw();
}
