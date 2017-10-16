#ifndef _D3DXSpriteTexture_h_
#define _D3DXSpriteTexture_h_

#include "GameObject.h"
#include "OLogFile.h"

class CD3DXTexture : public CGameObject {

private:
	// �e�N�X�`�����i�[����C���^�t�F�C�X  
	LPDIRECT3DTEXTURE9 lpD3DTexture;
	COLogFile oLog;
	int texWidth;
	int texHeight;
public:
	CD3DXTexture();
	CD3DXTexture(LPCTSTR fileName);
	~CD3DXTexture();

	// �摜�t�@�C������e�N�X�`�����쐬����֐�
	BOOL LoadGraph(LPCTSTR fileName);

	// �e�N�X�`���T�C�Y���擾����֐�
	void GetTextureSize(int *width, int *height);

	// �e�N�X�`���f�[�^�𒼐ڎ擾����֐�
	inline const LPDIRECT3DTEXTURE9 GetTexture() { return lpD3DTexture; }
};

#endif // !_D3DXSpriteTexture_h_