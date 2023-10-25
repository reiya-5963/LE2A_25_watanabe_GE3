#pragma once
#include "BaseScene.h"

#include <memory>
#include "Scaffold.h"
#include "Ground.h"
#include "Enemy.h"
#include "skydome.h"

#include "Player.h"
#include "FollowCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {
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
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;
private:

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


	// モデル
	std::unique_ptr<Model> E_model_body = nullptr;
	// モデル
	std::unique_ptr<Model> E_model_I_Wepon = nullptr;
	// モデル
	std::unique_ptr<Model> E_model_F_Wepon = nullptr;
	// 敵
	std::unique_ptr<Enemy> enemy_ = nullptr;


	// モデル
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// モデル
	std::unique_ptr<Model> groundModel_ = nullptr;
	// 地面
	std::unique_ptr<Ground> ground1_ = nullptr;
	std::unique_ptr<Ground> ground2_ = nullptr;
	std::unique_ptr<Ground> ground3_ = nullptr;

	std::unique_ptr<Scaffold> moveGround_ = nullptr;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	std::unique_ptr<CollisionManager> colliderManager_ = nullptr;
};