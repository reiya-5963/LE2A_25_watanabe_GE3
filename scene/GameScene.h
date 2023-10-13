#pragma once
#include "Input.h"
#include "DXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "viewProjection.h"
#include "WorldTransform.h"
#include "Triangle.h"

#include "CollisionManager.h"

#include <memory>
#include "Ground.h"
#include "Enemy.h"
#include "skydome.h"

#include "Player.h"
#include "FollowCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public:
	 
	/// <summary>
	///	コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
private:
	// dxCommon
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;	


	// ビュープロジェクション
	ViewProjection viewProjection_;

	// モデル
	std::unique_ptr<Model> P_model_body = nullptr;
	// モデル
	std::unique_ptr<Model> P_model_head = nullptr;
	// モデル
	std::unique_ptr<Model> P_model_l_arm = nullptr;
	// モデル
	std::unique_ptr<Model> P_model_r_arm = nullptr;
	//
	std::unique_ptr<Model> P_model_wepon = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;


	//// モデル
	//std::unique_ptr<Model> E_model_body = nullptr;
	//// モデル
	//std::unique_ptr<Model> E_model_I_Wepon = nullptr;
	//// モデル
	//std::unique_ptr<Model> E_model_F_Wepon = nullptr;
	//// 敵
	//std::unique_ptr<Enemy> enemy_ = nullptr;


	// モデル
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// モデル
	std::unique_ptr<Model> groundModel_ = nullptr;
	// 地面
	std::unique_ptr<Ground> ground_ = nullptr;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	std::unique_ptr<CollisionManager> colliderManager_ = nullptr;
};