#include "Input.h"
#include "WinApp.h"
#include <cassert>


#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

namespace {

	bool IsPress(const DIMOUSESTATE2& mouseState, int32_t buttonNumber) {
		assert(0 <= buttonNumber && buttonNumber < _countof(mouseState.rgbButtons));
		return (mouseState.rgbButtons[buttonNumber] & 0x80) != 0;
	}


}

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}



Input::~Input() {
	if (deviceKeyboard_) {
		deviceKeyboard_->Unacquire();
	}	
	if (deviceMouse_) {
		deviceMouse_->Unacquire();
	}

}

void Input::Initialize() {
	WinApp* winApp = WinApp::GetInstance();

	hwnd_ = winApp->GetHwnd();
	HRESULT result = S_FALSE;

	// DirectInputの初期化
	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directinput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directinput_->CreateDevice(GUID_SysKeyboard, &deviceKeyboard_, NULL);
	assert(SUCCEEDED(result));
	
	// 入力データ形式のセット
	result = deviceKeyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = deviceKeyboard_->SetCooperativeLevel(
		hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// マウスデバイスの生成
	result = directinput_->CreateDevice(GUID_SysMouse, &deviceMouse_, nullptr);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = deviceMouse_->SetDataFormat(&c_dfDIMouse2); // 拡張8ボタンまで
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = deviceMouse_->SetCooperativeLevel(hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

}

void Input::Update() {

	// キーボード情報の取得開始
	deviceKeyboard_->Acquire();
	// マウス情報の取得開始
	deviceMouse_->Acquire();

	// 前フレームの入力を代入
	preKey_ = key_;
	preMouse_ = mouse_;

	// キーの入力
	key_.fill(0);
	deviceKeyboard_->GetDeviceState((DWORD)size(key_), key_.data());

	// マウスの入力
	std::memset(&mouse_, 0, sizeof(mouse_));
	deviceMouse_->GetDeviceState(sizeof(mouse_), &mouse_);


}

bool Input::PushKey(BYTE keyNumber) const {
	
	// 0でなければ押している
	if (key_[keyNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) const {

	// 前フレームでは押していない && 今フレームでは押されている
	// ならtrue
	if (!preKey_[keyNumber] && key_[keyNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::IsPressMouse(int32_t mouseNumber) const {
	return IsPress(mouse_, mouseNumber);
}

bool Input::IsTriggerMouse(int32_t mouseNumber) const {
	// 前フレーム押していなくて、今フレーム押していればトリガー
	bool currentPush = IsPressMouse(mouseNumber);
	return currentPush && !IsPress(preMouse_, mouseNumber);
}

const DIMOUSESTATE2& Input::GetAllMouse() const {
	return mouse_;
}

int32_t Input::GetWhieel() const {
	return mouse_.lZ;
}