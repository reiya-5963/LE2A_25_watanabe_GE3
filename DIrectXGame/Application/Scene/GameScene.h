#pragma once
#include "BaseScene.h"

#include <memory>

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

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	std::unique_ptr<CollisionManager> colliderManager_ = nullptr;
};