#pragma once
#include "Framework.h"

//#include "GameScene.h"
#include "GlobalVariables.h"


/// <summary>
/// ゲームクラス
/// </summary>
class MyGame : public Framework {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 毎フレーム更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

private:
};