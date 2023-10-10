#pragma once
#include "Input.h"
#include "DXCommon.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "viewProjection.h"
#include "WorldTransform.h"
#include "Triangle.h"


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
	
	// camera
	Vector3 cScale{};
	Vector3 cRotate{};
	Vector3 cTranslate{};
	Matrix4x4 cameraMatrix{};
	Matrix4x4 viewMatrix{};
	Matrix4x4 projectionMatrix{};
	Matrix4x4 viewProjectionMatrix{};


	// スプライト
	std::vector<Sprite*> sprites_;
	// 最大数
	static const int kMaxSprite_ = 10;

	// 三角形
	std::vector<Triangle*> triangles_;
	// 最大数
	static const int kMaxTriangle_ = 10;

	Model* test_ = nullptr;



	// テクスチャハンドル
	uint32_t uvCheckTex_ = 0;
	uint32_t whiteBaseTex_ = 0;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};