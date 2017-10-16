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

	// スプライトのサイズを変更
	sWidth = width;
	sHeight = height;

	// 原点位置の更新
	SetCenterPosition(cPos);

	// フレームのリセット
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

	// 原点の位置を計算  
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

	//指定したフレーム値より転送範囲を計算
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
		oLog.Log("テクスチャが読み込まれていません。");
		return;
	}

	if (antiAliasing == ANTI_ALIASNIG_NONE) {

		//アンチエイリアスの無効  
		lpD3Ddevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
		lpD3Ddevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	}

	D3DXMATRIX mtrx1, mtrx2;

	// 原点を重ね合わせる平行移動  
	D3DXMatrixTranslation(&mtrx1, -origX, -origY, 0.0f);

	// 拡大行列と合成  
	if (ex != 1.0f || ey != 1.0f) {
		D3DXMatrixScaling(&mtrx2, ex, ey, 1.0f);
		D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);
	}

	// 回転行列との合成  
	if (r != 0.0f) {
		D3DXMatrixRotationZ(&mtrx2, r);
		D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);
	}

	// 指定の場所へ移動する行列との合成 
	D3DXMatrixTranslation(&mtrx2, x, y, 0.0f);
	D3DXMatrixMultiply(&mtrx1, &mtrx1, &mtrx2);

	// スプライト描画開始 
	lpSprite->Begin(NULL);

	lpSprite->SetTransform(&mtrx1);

	lpSprite->Draw(
		D3DTexture->GetTexture(),		// 描画に用いるテクスチャイメージ  
		&drawRect,						// イメージの使用する範囲（NULLでテクスチャ全体）
		NULL,							// テクスチャの中心位置（NULLで左上）
		NULL,							// スプライトの描画位置 
		0xFFFFFFFF | (BYTE)alpha << 24	// α値を合成
	);

	// スプライト描画終了  
	lpSprite->End();

}