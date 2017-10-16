#include "D3DTaskHead.h"


void CD3DTaskHead::Initialize() {
	lastTime = 0;	// 値の初期化 
}

void CD3DTaskHead::Update() {

	const DWORD WaitTime = 16;	//　約60FPS

	//　最後に行った処理からの時間を調べる
	DWORD nTime = timeGetTime();
	DWORD rTime = nTime - lastTime;
	lastTime = nTime;

	if (rTime < WaitTime) {
		//ウエイト処理
		Sleep(WaitTime - rTime);
	}

	//　画像のクリア
	lpD3Ddevice->Clear(
		0,							// クリアする領域の配列個数  
		NULL,						// クリアする領域の配列  
		D3DCLEAR_TARGET,			// 対象を指定の色でクリアする  
		D3DCOLOR_XRGB(0, 0, 64),	// クリアする色を紺色に指定  
		1.0f,						// z方向のクリア（1.0fですべてをクリア）
		0							// ステンシルバッファのクリア
	);

	// 開始宣言
	lpD3Ddevice->BeginScene();
}