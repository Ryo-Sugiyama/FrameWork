#include "D3DXSprite.h"



CD3DXSprite::CD3DXSprite() {
	Reset();
}

CD3DXSprite::CD3DXSprite(CD3DXTexture *source) {
	Reset();
	SetTexture(source);
}

void CD3DXSprite::Reset() {
	D3DTexture = NULL;
	cPos = CP_MIDDLECENTER;
	origX = origY = 0.0f;
	texWidth = 0;
	sWidth = sHeight = 0;
	SetRect(&drawRect, 0, 0, 0, 0);
}

void CD3DXSprite::SetTexture(CD3DXTexture *source) {
	if (source == NULL) return;
	D3DTexture = source;

	int h;
	D3DTexture->GetTextureSize(&texWidth, &h);
	SetSpriteSize(texWidth, h);

	
}

void CD3DXSprite::SetSpriteSize(int width, int height) {

	// �X�v���C�g�̃T�C�Y��ύX
	sWidth = width;
	sHeight = height;

	// ���_�ʒu�̍X�V
	SetCenterPosition(cPos);

	// �t���[���̃��Z�b�g
	SetFrame(0);
}

void CD3DXSprite::GetSpriteSize(int *width, int *height) {
	*width = sWidth;
	*height = sHeight;
}

void CD3DXSprite::SetSpriteRect(int left, int top, int right, int bottom) {

	sWidth = right - left;
	sHeight = bottom - top;

	drawRect.left = left;
	drawRect.right = right;
	drawRect.top = top;
	drawRect.bottom = bottom;

	SetCenterPosition(cPos);
}

void CD3DXSprite::SetCenterPosition(BYTE pos) {

	// ���_�̈ʒu���v�Z  
	cPos = pos;

	if (pos & CP_LEFT) {
		origX = 0.0f;
	}
	else if (pos & CP_RIGHT) {
		origX = (float)sWidth;
	}
	else {
		origX = (float)sWidth / 2.0f;
	}

	if (pos & CP_TOP) {
		origY = 0.0f;
	} 
	else if (pos & CP_BOTTOM) {
		origY = (float)sHeight;
	}
	else {
		origY = (float)sHeight / 2.0f;
	}
}

BYTE CD3DXSprite::GetCenterPosition() {
	return cPos;
}

void CD3DXSprite::SetFrame(int frame) {

	//�w�肵���t���[���l���]���͈͂��v�Z
	int d = texWidth / sWidth;
	if (d == 0) return;

	int xPos = frame % d;
	int yPos = frame / d;

	drawRect.left = xPos * sWidth;
	drawRect.right = drawRect.left + sWidth;
	drawRect.top = yPos * sHeight;
	drawRect.bottom = drawRect.top + sHeight;
}

void CD3DXSprite::Draw(float x, float y, int alpha, BOOL antiAliasingFlag) {
	Draw(x, y, 1.0f, 1.0f, alpha, antiAliasingFlag);
}

void CD3DXSprite::Draw(float x, float y, float ex, float ey, int alpha, BOOL antiAliasingFlag) {
	Draw(x, y, ex, ey, 0.0f, alpha, antiAliasingFlag);
}

void CD3DXSprite::Draw(float x, float y, float r, int alpha, BOOL antiAliasingFlag) {
	Draw(x, y, 1.0f, 1.0f, r, alpha, antiAliasingFlag);
}

void CD3DXSprite::Draw(float x, float y, float ex, float ey, float r, int alpha, BOOL antiAliasing) {

	if (D3DTexture == NULL) {
		oLog.Log("�e�N�X�`�����ǂݍ��܂�Ă��܂���B");
		return;
	}

	if (antiAliasing == ANTI_ALIASNIG_NONE) {

		//�A���`�G�C���A�X�̖���  
		lpD3Ddevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
		lpD3Ddevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	}

	D3DXMATRIX mtrx1, mtrx2;

	// ���_���d�ˍ��킹�镽�s�ړ�  
	D3DXMatrixTranslation(&mtrx1, -origX, -origY, 0.0f);

	// �g��s��ƍ���  
	if (ex != 1.0f || ey != 1.0f) {
		D3DXMatrixScaling(&mtrx2, ex, ey, 1.0f);
		D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);
	}

	// ��]�s��Ƃ̍���  
	if (r != 0.0f) {
		D3DXMatrixRotationZ(&mtrx2, r);
		D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);
	}

	// �w��̏ꏊ�ֈړ�����s��Ƃ̍��� 
	D3DXMatrixTranslation(&mtrx2, x, y, 0.0f);
	D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);

	// �X�v���C�g�`��J�n 
	lpSprite->Begin(NULL);

	lpSprite->SetTransform(&mtrx1);

	lpSprite->Draw(
		D3DTexture->GetTexture(),		// �`��ɗp����e�N�X�`���C���[�W  
		&drawRect,						// �C���[�W�̎g�p����͈́iNULL�Ńe�N�X�`���S�́j
		NULL,							// �e�N�X�`���̒��S�ʒu�iNULL�ō���j
		NULL,							// �X�v���C�g�̕`��ʒu 
		0xFFFFFFFF | (BYTE)alpha << 24	// ���l������
	);

	// �X�v���C�g�`��I��  
	lpSprite->End();

}