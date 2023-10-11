#include "GameScene.h"
#include <cassert>
#include "MyMath.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene() {}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("sample.png");

	// モデルの生成
	model_.reset(Model::CreateFlomObj("cube"));

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// プレイヤーの生成
	player_ = std::make_unique<Player>();
	// プレイヤーの初期化
	player_->Initialize(model_.get(), textureHandle_);



}

void GameScene::Update() {
	// プレイヤーの更新	
	player_->Update();

}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);
	////この間に背景スプライトの描画を入れる
	//for (Sprite* sprite : sprites_) {
	//	sprite->Draw();
	//}
	Sprite::PostDraw();


	dxCommon_->ClearDepthBuffer();
	
	
	Model::PreDraw(commandList, Model::BlendMode::kNone);
	
	// プレイヤーの描画
	player_->Draw(viewProjection_);
	
	Model::PostDraw();

	Triangle::PreDraw(commandList);
	Triangle::PostDraw();
	Sprite::PreDraw(commandList);
	//
	Sprite::PostDraw();

}