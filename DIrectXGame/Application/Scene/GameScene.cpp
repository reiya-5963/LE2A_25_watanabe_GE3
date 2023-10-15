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
	//textureHandle_ = TextureManager::Load("sample.png");
		// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	skydomeModel_.reset(Model::CreateFlomObj("skydome"));
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化
	skydome_->Initialize(skydomeModel_.get(), { 0, 0, 0 });


	groundModel_.reset(Model::CreateFlomObj("Ground"));
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	// 地面の初期化
	ground_->Initialize(groundModel_.get(), { 0, 0, 0 });

	// モデルの生成
	P_model_body.reset(Model::CreateFlomObj("TestPlayerver_body"));
	P_model_head.reset(Model::CreateFlomObj("TestPlayerver_head"));
	P_model_l_arm.reset(Model::CreateFlomObj("TestPlayerver_l_arm"));
	P_model_r_arm.reset(Model::CreateFlomObj("TestPlayerver_r_arm"));
	P_model_wepon.reset(Model::CreateFlomObj("wepon"));


	// プレイヤーの生成
	player_ = std::make_unique<Player>();

	// プレイヤーの初期化
	std::vector<Model*> playerModels = {
		P_model_body.get(),
		P_model_head.get(),
		P_model_l_arm.get(),
		P_model_r_arm.get(),
		P_model_wepon.get() };

	// プレイヤーの初期化
	player_->Initialize(playerModels);

	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());


	//E_model_body.reset(Model::CreateFlomObj("EnemyTest_Body"));
	//E_model_F_Wepon.reset(Model::CreateFlomObj("EnemyTest_F_Wepon"));
	//E_model_I_Wepon.reset(Model::CreateFlomObj("EnemyTest_I_Wepon"));

	//// 敵の生成
	//enemy_ = std::make_unique<Enemy>();
	//// 敵の初期化
	//std::vector<Model*> enemyModels = {
	//	E_model_body.get(),
	//	E_model_F_Wepon.get(),
	//	E_model_I_Wepon.get() };
	//// 敵の初期化
	//enemy_->Initialize(enemyModels);
	//enemy_->SetVelocity({ 0, 0, 1 });

	colliderManager_ = std::make_unique<CollisionManager>();
	colliderManager_->Initialize();
}

void GameScene::Finalize() {
}

void GameScene::Update() {
	colliderManager_->ClearColliders();
	// 地面の更新
	ground_->Update();

	// 追従カメラの更新
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	// プレイヤーの更新
	player_->Update();
	colliderManager_->AddColliders(player_.get());

	viewProjection_.TransferMatrix();

	//enemy_->Update();
	//colliderManager_->AddColliders(enemy_.get());
	colliderManager_->UpdateWorldTransform();

	colliderManager_->CheckAllCollisions();
}

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);
	////この間に背景スプライトの描画を入れる
	Sprite::PostDraw();


	dxCommon_->ClearDepthBuffer();


	Model::PreDraw(commandList, Model::BlendMode::kNone);
	// 天球の描画
	skydome_->Draw(viewProjection_);
	// 地面の描画
	ground_->Draw(viewProjection_);

	// プレイヤーの描画
	player_->Draw(viewProjection_);

	colliderManager_->Draw(viewProjection_);
	// プレイヤーの描画
	//enemy_->Draw(viewProjection_);

	Model::PostDraw();

	Sprite::PreDraw(commandList);
	//
	Sprite::PostDraw();

}

