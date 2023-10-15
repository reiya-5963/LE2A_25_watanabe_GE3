#include "MyGame.h"

// ウィンドウズアプリのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 
	Framework* myGame = new MyGame;

	// 
	myGame->Run();

	//
	delete myGame;

	return 0;
}

