#include "DxFontTest.h"

void CDxFontTestStart::Initialize() {
	AppendObject(new CID3DXFont(), -1, "font", true);

	CMoveFont *moveFont;

	// 30�s�N�Z���Ԋu�ŁA�ړ�����t�H���g�̃I�u�W�F�N�g��z�u  
	for (int x = 0; x < 640; x += 30) {
		moveFont = new CMoveFont();
		moveFont->SetStartState(x, rand() % (480 - 16), rand() % 4 + 1);

		AppendObject(moveFont, 1000 + x, true);
	}

	// �������g�͕K�v�Ȃ��Ȃ����̂ŏ��� 
	RemoveObject(this);
}

void CMoveFont::SetStartState(int posX, int posY, int speed) {
	x = posX;
	y = posY;
	m = speed;
}

void CMoveFont::Initialize() {

	// �I�u�W�F�N�g�f�[�^���Q�ƂƂ��Ċi�[  
	pFont = (CID3DXFont*)FindObject("font");
}

void CMoveFont::Update() {

	if (pFont) {
		if (y < 0 || y > 480 - 16) m = -m;
		y += m;
	}

	pFont->Draw(_T("�t�W�^"), x, y);
}