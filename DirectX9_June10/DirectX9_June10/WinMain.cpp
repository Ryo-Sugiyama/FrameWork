// @description
/* 
*　ウインドウの生成・メインループの管理
*　copyright © Ryo Sugiyama All write reserved.
*/
#include <tchar.h>  
#include <windows.h>

#include "GameObject.h"
//#include "DirectSound.h"
#include "D3DTaskHead.h"
#include "D3DTaskTail.h"
#include "OLogFile.h"
#include "DxFontTest.h"
#include "SpriteTest.h"
#include "Animate.h"
#include "font.h"

#pragma comment (lib,"winmm.lib")
#define MaxINT 214783647

namespace {
	constexpr int width = 640;	//ウインドウのの幅
	constexpr int height = 480;	//ウインドウの高さ
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;

	default:
		return (DefWindowProc(hWnd, msg, wParam, lParam));
		break;
	}
	return (0);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow) {

	timeBeginPeriod(1);

	COLogFile oLog;
	oLog.SetUp();

	WNDCLASS wc;

	wc.cbClsExtra = 0;										// 構造体のサイズを拡張するバイト数
	wc.cbWndExtra = 0;										// ウィンドウのサイズを拡張するバイト数
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ウィンドウの背景を塗りつぶすためのブラシ。 
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);			// ウィンドウ上にマウスカーソルがきたときのマウスの形状 
	wc.hIcon = NULL;										// ウィンドウの左上に表示されるアイコン。
	wc.hInstance = hInstance;								// ウィンドウを作るためのインスタンスハンドル
	wc.lpfnWndProc = WndProc;								// ウィンドウから通知されたイベントを処理するための関数
	wc.lpszClassName = _T("Game");							// 登録するウィンドウのクラス名。
	wc.lpszMenuName = NULL;									// メニューのリソースID。
	wc.style = NULL;										//ウィンドウクラスのスタイル。

	// ウィンドウクラスの登録(失敗で終了) 
	if (RegisterClass(&wc) == NULL) {
		oLog.Log("ウインドウクラスの登録に失敗しました。WinMain.cpp L.40");
		return 0;
	}
	else
		oLog.Log("ウインドウクラスの登録に成功しました。");

	HWND hWnd = CreateWindow(
		wc.lpszClassName,       // 登録したクラス名 
		_T("Algorithm"),		// ウィンドウに表示するタイトル 
		WS_OVERLAPPEDWINDOW,	// ウィンドウのスタイル（タイトルバー、システムメニュー、最小化ボタンを使用）
		CW_USEDEFAULT,			// ウィンドウのx位置（システム標準値を使用）
		CW_USEDEFAULT,			// ウィンドウのy位置（システム標準値を使用）
		width,					// ウィンドウの幅 
		height,					// ウィンドウの高さ
		NULL,					// 親ウィンドウのハンドル  
		NULL,					// メニューハンドル  
		hInstance,				// インスタンスハンドル  
		NULL
	);

	//DirectXの初期化
	CGameObject gameObject;
	gameObject.SetUp(hWnd, hInstance);
	gameObject.AppendObject(new SpriteTest(), 5, true);
	gameObject.AppendObject(new font(), 7, true);
	gameObject.AppendObject(new CAnimationTest(), 6, true);
	gameObject.AppendObject(new CD3DTaskHead(), 0, true);
	gameObject.AppendObject(new CD3DTaskTail(), MaxINT, true);

	gameObject.AppendObject(new CDxFontTestStart(), 2, true);

	// ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	//メインループ
	MSG msg;
	while (TRUE) {
		// ウィンドウに通達されようとしているイベントを取得 
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT) break;

			// 仮想キーメッセージを文字メッセージへ変換  
			TranslateMessage(&msg);

			// ウィンドウへイベントを通達
			DispatchMessage(&msg);
		}
		else {
			gameObject.DoAllTask();
		}
	}

	//DirectXの終了
	gameObject.TearDown();
	return (0);
}
