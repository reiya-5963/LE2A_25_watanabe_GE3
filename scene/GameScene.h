#pragma once
#include "Input.h"
#include "DXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "viewProjection.h"
#include "WorldTransform.h"
#include "Triangle.h"


#include <memory>
#include "Ground.h"
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

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;


	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;


};