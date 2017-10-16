#include "ID3DXFont.h"



CID3DXFont::CID3DXFont() {
	lpD3DxFont = NULL;
	Create(16);
}

CID3DXFont::CID3DXFont(int fontSize) {
	lpD3DxFont = NULL;
	Create(fontSize);
}

CID3DXFont::~CID3DXFont() {
	RELEASE(lpD3DxFont);
}

BOOL CID3DXFont::Create(int fontSize) {

	RELEASE(lpD3DxFont);

	int result = D3DXCreateFont(
		lpD3Ddevice,					// 取得元となるデバイス  
		fontSize,						// 幅
		NULL,							// 高さ（規定の高さ）
		FW_DONTCARE,					// 太さ（規定の太さ）
		NULL,							// ミップマップレベル（なし） 
		FALSE,							// 斜体（なし）
		ANSI_CHARSET,					// 文字セット（ANSI）
		OUT_DEFAULT_PRECIS,				// 出力精度（標準）
		DEFAULT_QUALITY,				// 出力品質（標準）
		DEFAULT_PITCH | FF_DONTCARE,	// フォントピッチ&フォントファミリ
		_T("MS UI Gothic"),				// フォント名  
		&lpD3DxFont						// 格納先  
	);

	if (result == 0) {
		oLog.Log("フォントの作成に成功しました。");
		return FALSE;
	}
	else {
		oLog.Log("フォントの作成に失敗しました。CID3DxFont.cpp L.23");
		return TRUE;
	}
}

void CID3DXFont::Draw(LPCTSTR text, int count, LPRECT pRect, DWORD format, D3DXCOLOR color) {

	if (lpD3DxFont) {

		// ２Ｄの描画開始を宣言 
		lpSprite->Begin(NULL);	

		D3DXMATRIX mtrx;
		D3DXMatrixTranslation(&mtrx, 0.0f, 0.0f, 0.0f);
		lpSprite->SetTransform(&mtrx);

		lpD3DxFont->DrawText(
			lpSprite,	// 描画に用いるスプライトオブジェクト  
			text,		// 表示する文字列
			count,		// 表示する文字数。-1で文字列すべて  
			pRect,		// 文字列を描画する範囲  
			format,		// 文字列描画設定  
			color		// 文字列描画色  
		);

		// 描画終了の宣言 
		lpSprite->End();	
	}
}

void CID3DXFont::Draw(LPCTSTR text, int x, int y) {
	RECT rect = { x, y, x, y };
	Draw(text,-1,&rect,DT_NOCLIP | DT_SINGLELINE, 0xFFFFFFFF);

}