#include "DirectInput.h"



CDirectInput::CDirectInput() {
	lpInput = NULL;
	lpKeyDevice = NULL;
}


CDirectInput::~CDirectInput() {
	RELEASE(lpKeyDevice);
	RELEASE(lpInput);
}

void CDirectInput::Initialize() {
	if (lpInput == NULL) {
		HRESULT hr;
		//インターフェイスの取得
		hr = DirectInput8Create(
			GetHInstance(),			// ソフトのインスタンスハンドル  
			DIRECTINPUT_VERSION,	// DirectInputのバージョン
			IID_IDirectInput8,		// 取得するインターフェイスのタイプ 
			(LPVOID*)&lpInput,		// インターフェイスの格納先
			NULL					// COM集成の制御オブジェクト（使わないのでNULL）  
		);
		if (FAILED(hr)) {
			oLog.Log("DirectInputの初期化に失敗しました。CDirectInput.cpp L.20");
			return;
		}
		else {
			oLog.Log("DirectInputの初期化に成功しました");
		}

		hr = lpInput->CreateDevice(
			GUID_SysKeyboard,	// 受け付ける入力デバイス 
			&lpKeyDevice,		// IDirectInputDevice8格納先
			NULL				// COM集成の制御オブジェクト（使わないのでNULL） 
		);

		if (FAILED(hr)) {
			oLog.Log("DirectInputDeviceの初期化に失敗しました。CDirectInput.cpp L.35");
			return;
		}

		// データフォーマットの設定
		lpKeyDevice->SetDataFormat(&c_dfDIKeyboard);

		// 協調モードの設定  
		lpKeyDevice->SetCooperativeLevel(GetHWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		//キーバッファの初期化
		ZeroMemory(keyData, 256);
	}
}

void CDirectInput::Update() {
	 
	if (lpKeyDevice) {

		// 入力の受け付け開始 
		lpKeyDevice->Acquire();

		// データを取得（256は取得するデータのサイズ）
		lpKeyDevice->GetDeviceState(256, keyData);
	}
}

BOOL CDirectInput::IsKeyDown(int key) {

	if (lpKeyDevice == NULL) return FALSE;

	// 最上位１ビットに「押されているか」どうかのデータが格納されていることに注意  
	// 最下位１ビットには「トグル（オン・オフの状態）」が格納されている  
	return (keyData[key] & 0x80) ? TRUE : FALSE;
}

BOOL CDirectInput::IsKeyPressed(int key) {

	if (lpKeyDevice == NULL) return FALSE;

	// 現在押されていてかつ直前に押されていないときTRUEを返す 
	if ((keyData[key] & 0x80) && !(lastKeyData[key] & 0x80)) return TRUE;

	return FALSE;
}

BOOL CDirectInput::IsKeyReleased(int key) {

	if (lpKeyDevice == NULL) return FALSE;

	// 現在押されておらずかつ直前には押されていたときTRUEを返す 
	if (!(keyData[key] & 0x80) && (lastKeyData[key] & 0x80)) return TRUE;

	return FALSE;
}