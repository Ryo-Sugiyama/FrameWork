#include "DxFontTest.h"

void CDxFontTestStart::Initialize() {
	AppendObject(new CID3DXFont(), -1, "font", true);

	CMoveFont *moveFont;

	// 30ピクセル間隔で、移動するフォントのオブジェクトを配置  
	for (int x = 0; x < 640; x += 30) {
		moveFont = new CMoveFont();
		moveFont->SetStartState(x, rand() % (480 - 16), rand() % 4 + 1);

		AppendObject(moveFont, 1000 + x, true);
	}

	// 自分自身は必要なくなったので消去 
	RemoveObject(this);
}

void CMoveFont::SetStartState(int posX, int posY, int speed) {
	x = posX;
	y = posY;
	m = speed;
}

void CMoveFont::Initialize() {

	// オブジェクトデータを参照として格納  
	pFont = (CID3DXFont*)FindObject("font");
}

void CMoveFont::Update() {

	if (pFont) {
		if (y < 0 || y > 480 - 16) m = -m;
		y += m;
	}

	pFont->Draw(_T("フジタ"), x, y);
}