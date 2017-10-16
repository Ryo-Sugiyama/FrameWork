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
		lpD3Ddevice,					// �擾���ƂȂ�f�o�C�X  
		fontSize,						// ��
		NULL,							// �����i�K��̍����j
		FW_DONTCARE,					// �����i�K��̑����j
		NULL,							// �~�b�v�}�b�v���x���i�Ȃ��j 
		FALSE,							// �Άi�Ȃ��j
		ANSI_CHARSET,					// �����Z�b�g�iANSI�j
		OUT_DEFAULT_PRECIS,				// �o�͐��x�i�W���j
		DEFAULT_QUALITY,				// �o�͕i���i�W���j
		DEFAULT_PITCH | FF_DONTCARE,	// �t�H���g�s�b�`&�t�H���g�t�@�~��
		_T("MS UI Gothic"),				// �t�H���g��  
		&lpD3DxFont						// �i�[��  
	);

	if (result == 0) {
		oLog.Log("�t�H���g�̍쐬�ɐ������܂����B");
		return FALSE;
	}
	else {
		oLog.Log("�t�H���g�̍쐬�Ɏ��s���܂����BCID3DxFont.cpp L.23");
		return TRUE;
	}
}

void CID3DXFont::Draw(LPCTSTR text, int count, LPRECT pRect, DWORD format, D3DXCOLOR color) {

	if (lpD3DxFont) {

		// �Q�c�̕`��J�n��錾 
		lpSprite->Begin(NULL);	

		D3DXMATRIX mtrx;
		D3DXMatrixTranslation(&mtrx, 0.0f, 0.0f, 0.0f);
		lpSprite->SetTransform(&mtrx);

		lpD3DxFont->DrawText(
			lpSprite,	// �`��ɗp����X�v���C�g�I�u�W�F�N�g  
			text,		// �\�����镶����
			count,		// �\�����镶�����B-1�ŕ����񂷂ׂ�  
			pRect,		// �������`�悷��͈�  
			format,		// ������`��ݒ�  
			color		// ������`��F  
		);

		// �`��I���̐錾 
		lpSprite->End();	
	}
}

void CID3DXFont::Draw(LPCTSTR text, int x, int y) {
	RECT rect = { x, y, x, y };
	Draw(text,-1,&rect,DT_NOCLIP | DT_SINGLELINE, 0xFFFFFFFF);

}