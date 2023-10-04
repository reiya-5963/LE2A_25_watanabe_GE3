#include "MyGame.h"

// ウィンドウズアプリのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	MyGame* myGame = new MyGame;

	//初期化処理
	myGame->Initialize();

	// ゲームループ
	while (true) {
		// もしウィンドウが破棄されたらループを抜ける
		if (myGame->IsEndRequest()) {
			break;
		}
		else {
			// 毎フレーム更新処理
			myGame->Update();

			// 描画処理
			myGame->Draw();
		}
	}

	// 終了処理
	myGame->Finalize();
	delete myGame;

	return 0;
}

