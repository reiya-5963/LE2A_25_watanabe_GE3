#include "MyGame.h"

void MyGame::Initialize() {

#pragma region WindowAPIクラス初期化
	// WinAppのシングルトンの取得
	winApp_ = WinApp::GetInstance();

	// ウィンドウの作成
	winApp_->Initialize(L"LE2A_22_ワタナベエンジン");
#pragma endregion

#pragma region DirectX汎用クラス初期化
	// dxCommonのシングルトンの取得
	dxCommon_ = DirectXCommon::GetInstance();

	// directX関連の初期化処理
	dxCommon_->Initialize(winApp_);
#pragma endregion

	// DXC生成
	DXCManager::GetInstance()->CreateDXC();

#pragma region 機能の初期化

#pragma region ImGui管理クラスの初期化
	// ImGuiManagerのシングルトンの取得
	imGuiManager_ = ImGuiManager::GetInstance();
	// ImGuiの初期化処理
	imGuiManager_->Initialize(winApp_, dxCommon_);
#pragma endregion

#pragma region 入力クラスの初期化
	// 入力の初期化
	input_ = Input::GetInstance();
	input_->Initialize();
#pragma endregion

#pragma region テクスチャ管理クラスの初期化
	// TextureManagerの初期化処理 (シングルトン)
	TextureManager::GetInstance()->Initialize(dxCommon_->GetDevice());
	// 1x1の白画像を読み込む
	TextureManager::Load("white1x1.png");

	// スプライトの静的初期化
	Sprite::StaticInitialize(WinApp::kWindowWidth, WinApp::kWindowHeight);
	Triangle::StaticInitialize(dxCommon_->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);
	Model::StaticInitialize();
#pragma endregion

#pragma endregion

	// ゲームシーンの初期化処理
	gameScene_ = new GameScene();
	gameScene_->Initialize();
}

void MyGame::Finalize() {
	// ゲームシーンのdelete処理
	//SafeDelete(gameScene);

	// ImGuiのRelease処理
	imGuiManager_->Finalize();

	// ウィンドウの破棄の処理
	winApp_->Finalize();
}

void MyGame::Update() {
	// 入力関連の毎フレーム処理
	input_->Update();

	// ImGuiの前処理
	imGuiManager_->Begin();

	// ゲームシーンの更新処理
	gameScene_->Update();

	// ImGuiの後処理
	imGuiManager_->End();

	// もしウィンドウを破棄するならゲーム終了リクエストを送る
	if (winApp_->ProcessMessage())
		endRequest_ = true;
}

void MyGame::Draw() {
	// 描画前処理
	dxCommon_->preDraw();

	// 描画処理
	gameScene_->Draw();

	// ImGui描画処理
	imGuiManager_->Draw();

	// 描画後処理
	dxCommon_->postDraw();
}
