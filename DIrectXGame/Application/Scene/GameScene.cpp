#include "GameScene.h"
#include <cassert>
#include "R_Math.h"
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

#pragma region 天球
	skydomeModel_.reset(Model::CreateFlomObj("skydome"));
	// 天球の生成
	skydome_ = std::make_unique<Skydome>();
	// 天球の初期化
	skydome_->Initialize(skydomeModel_.get(), { 0, 0, 0 });
#pragma endregion
	//groundModel_.reset(Model::CreateFlomObj("cube"));

#pragma region 地面
	groundModel_.reset(Model::CreateFlomObj("cube"));
	// 地面の生成
	ground1_ = std::make_unique<Ground>();
	// 地面の初期化
	ground1_->Initialize(groundModel_.get(), { 0.0f, 0.0f, 0.0f });

	// 地面の生成
	ground2_ = std::make_unique<Ground>();
	// 地面の初期化
	ground2_->Initialize(groundModel_.get(), { 0.0f, 0.0f, 260.0f });

	// 地面の生成
	ground3_ = std::make_unique<Ground>();
	// 地面の初期化
	ground3_->Initialize(groundModel_.get(), { 0.0f, 0.0f, 260.0f * 2.0f });

	// 地面の生成
	moveGround1_ = std::make_unique<Scaffold>();
	// 地面の初期化
	moveGround1_->Initialize(groundModel_.get(), { 0.0f, 0.0f, 130.0f });
	moveGround1_->SetIsMove(true);

	// 地面の生成
	moveGround2_ = std::make_unique<Scaffold>();
	// 地面の初期化
	moveGround2_->Initialize(groundModel_.get(), { 0.0f, 0.0f, 260.0f + 130.0f });
	moveGround2_->SetIsMove(true);
	// 地面の生成
	goal_ = std::make_unique<Goal>();
	// 地面の初期化
	goal_->Initialize(groundModel_.get(), { 0.0f, 6.0f, 260.0f * 2.0f });

#pragma endregion

#pragma region 自キャラ
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
		P_model_r_arm.get() };

	std::vector<Model*> weponModels = {
		P_model_wepon.get() };
	// プレイヤーの初期化
	Vector3 pos = { 0.0f, 50.0f, 0.0f };
	player_->Initialize(playerModels, weponModels, pos);
#pragma endregion

#pragma region 追従カメラ
	// 追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
#pragma endregion

#pragma region
	E_model_body.reset(Model::CreateFlomObj("EnemyTest_Body"));
	E_model_F_Wepon.reset(Model::CreateFlomObj("EnemyTest_F_Wepon"));
	E_model_I_Wepon.reset(Model::CreateFlomObj("EnemyTest_I_Wepon"));

	// 敵の初期化
	std::vector<Model*> enemyModels = {
		E_model_body.get(),
		E_model_F_Wepon.get(),
		E_model_I_Wepon.get() };

	Vector3 enemyPos[5];
	enemyPos[0] = { -30.0f, 6.0f, 240.0f };
	enemyPos[1] = { -25.0f, 6.0f, 270.0f };
	enemyPos[2] = { -20.0f, 6.0f, 265.0f };
	enemyPos[3] = { -15.0f, 6.0f, 260.0f };
	enemyPos[4] = { -35.0f, 6.0f, 255.0f };

	//-30.0f;
	//objectWorldTrans_.translation_.y = 6.0f;
	//objectWorldTrans_.translation_.z = 260.0f;
	for (uint32_t i = 0; i < 5; i++) {
		// 敵の生成
		enemies_.push_back(std::unique_ptr<Enemy>(new Enemy()));
	}
	uint32_t i = 0;
	std::list<unique_ptr<Enemy>>::iterator it;
	for (it = enemies_.begin(); it != enemies_.end(); it++) {
		// 敵の初期化
		(*it)->Initialize(enemyModels, enemyPos[i]);
		(*it)->SetVelocity({ 0, 0, 1 });
		i++;
	}


#pragma endregion

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
	followCamera_->SetLockOn(lockOn_.get());
	player_->SetLockOn(lockOn_.get());

	colliderManager_ = std::make_unique<CollisionManager>();
	colliderManager_->Initialize();
}

void GameScene::Finalize() {
	P_model_body.release();
	P_model_head.release();
	P_model_l_arm.release();
	P_model_r_arm.release();
	P_model_wepon.release();


	E_model_body.release();
	E_model_F_Wepon.release();
	E_model_I_Wepon.release();
}

void GameScene::Update() {
	//Vector3 ambientColor = LightGroup::GetInstance()->GetAmbientColor();
	//Vector3 lightDir = LightGroup::GetInstance()->GetDirectLightDir(0);
	//
	//ImGui::Begin("light");
	//ImGui::DragFloat3("ambientCol", &ambientColor.x, 0.2f, 0.0f, 1.0f);
	//ImGui::DragFloat3("lightDir", &lightDir.x, 0.2f, -100.0f, 100.0f);
	//ImGui::End();

	//if (ambientColor.x != LightGroup::GetInstance()->GetAmbientColor().x ||
	//	ambientColor.y != LightGroup::GetInstance()->GetAmbientColor().y ||
	//	ambientColor.z != LightGroup::GetInstance()->GetAmbientColor().z) {
	//	LightGroup::GetInstance()->SetAmbientColor(ambientColor);
	//}	
	//if (lightDir.x != LightGroup::GetInstance()->GetDirectLightDir(0).x ||
	//	lightDir.y != LightGroup::GetInstance()->GetDirectLightDir(0).y ||
	//	lightDir.z != LightGroup::GetInstance()->GetDirectLightDir(0).z) {
	//	LightGroup::GetInstance()->SetDirectLightDir(0, lightDir);
	//}

	////LightGroup::GetInstance()->SetDirectLightDir(0, lightDir);

	//LightGroup::GetInstance()->Update();

	// プレイヤーの更新
	player_->Update();

	// 地面の更新
	ground1_->Update();
	ground2_->Update();
	ground3_->Update();
	moveGround1_->Update();
	moveGround2_->Update();
	goal_->Update();
	std::list<unique_ptr<Enemy>>::iterator it;
	for (it = enemies_.begin(); it != enemies_.end();it++) {
		if ((*it)) {
			// 敵の更新
			(*it)->Update();
			if ((*it)->IsDead()) {
				(*it).release();
				(*it) = nullptr;
			}
		}
	}

	// コライダーリストの初期化
	colliderManager_->ClearColliders();

	// それぞれ当たり判定があるものをリストに追加	
	colliderManager_->AddColliders(moveGround1_.get());
	colliderManager_->AddColliders(ground1_.get());
	colliderManager_->AddColliders(player_.get());
	colliderManager_->AddColliders(ground2_.get());
	colliderManager_->AddColliders(ground3_.get());
	colliderManager_->AddColliders(moveGround2_.get());
	colliderManager_->AddColliders(goal_.get());
	if (player_->GetWeponCollider()) {
		colliderManager_->AddColliders(player_->GetWeponCollider());
	}

	for (it = enemies_.begin(); it != enemies_.end(); it++) {
		if ((*it)) {
			colliderManager_->AddColliders((*it).get());
		}
	}


	// 当たり判定チェック
	colliderManager_->CheckAllCollisions();
	colliderManager_->UpdateWorldTransform();


	if (player_->IsRespown()) {
		Vector3 enemyPos[5];
		enemyPos[0] = { -30.0f, 6.0f, 240.0f };
		enemyPos[1] = { -25.0f, 6.0f, 270.0f };
		enemyPos[2] = { -20.0f, 6.0f, 265.0f };
		enemyPos[3] = { -15.0f, 6.0f, 260.0f };
		enemyPos[4] = { -35.0f, 6.0f, 255.0f };

		// 敵の初期化
		std::vector<Model*> enemyModels = {
			E_model_body.get(),
			E_model_F_Wepon.get(),
			E_model_I_Wepon.get() };

		int i = 0;
		for (it = enemies_.begin(); it != enemies_.end(); it++) {
			if((*it) == nullptr) {
				// 敵の生成
				(*it) = std::make_unique<Enemy>();
				// 敵の初期化
				(*it)->Initialize(enemyModels, enemyPos[i]);
				(*it)->SetVelocity({ 0, 0, 1 });
			}
			i++;
		}
	}

	lockOn_->Update(enemies_, viewProjection_);

	// 追従カメラの更新
	followCamera_->SetIsRespown(player_->IsRespown());
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();

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
	ground1_->Draw(viewProjection_);
	ground2_->Draw(viewProjection_);
	ground3_->Draw(viewProjection_);
	moveGround1_->Draw(viewProjection_);
	moveGround2_->Draw(viewProjection_);

	// プレイヤーの描画
	player_->Draw(viewProjection_);

	colliderManager_->Draw(viewProjection_);
	std::list<unique_ptr<Enemy>>::iterator it;
	for (it = enemies_.begin(); it != enemies_.end(); it++) {
		if ((*it)) {
			// プレイヤーの描画
			(*it)->Draw(viewProjection_);
		}
	}

	goal_->Draw(viewProjection_);

	Model::PostDraw();

	Sprite::PreDraw(commandList);
	//

	lockOn_->Draw();




	Sprite::PostDraw();

	//ParticleManager::Draw(commandList, ParticleManager::BlendMode::kAdd);
}

