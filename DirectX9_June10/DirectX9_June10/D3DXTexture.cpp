#include "D3DXTexture.h"



CD3DXTexture::CD3DXTexture() {
	lpD3DTexture = NULL;
}

CD3DXTexture::CD3DXTexture(LPCTSTR fileName) {
	lpD3DTexture = NULL;
	LoadGraph(fileName);
}

CD3DXTexture::~CD3DXTexture() {
	RELEASE(lpD3DTexture);
}

void CD3DXTexture::GetTextureSize(int *width, int *height) {
	*width = texWidth;
	*height = texHeight;
}

BOOL CD3DXTexture::LoadGraph(LPCTSTR fileName) {
	RELEASE(lpD3DTexture);

	BOOL b = SUCCEEDED(D3DXCreateTextureFromFile(lpD3Ddevice, fileName, &lpD3DTexture));
	if (b == TRUE) {
		D3DSURFACE_DESC desc;

		// �ŏ�ʂ̃e�N�X�`�������擾  
		lpD3DTexture->GetLevelDesc(0, &desc);

		texWidth = (int)desc.Width;
		texHeight = (int)desc.Height;
	}
	else 
		oLog.Log("�摜�̓ǂݍ��݂Ɏ��s���܂����BD3DXSpriteTexture.cpp L.26");
	
	return b;
}