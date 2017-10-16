#ifndef _CD3DXSprite_h_
#define _CD3DXSprite_h_

#include "GameObject.h"
#include "D3DXTexture.h"

#define CP_MIDDLECENTER 0
#define CP_LEFT			1
#define CP_CENTER		2
#define CP_RIGHT		4
#define CP_TOP			8
#define CP_MIDDLE		16
#define CP_BOTTOM		32
#define ANTI_ALIASNIG_NONE FALSE
#define ANTI_ALIASNIG_ACTIVE TRUE

class CD3DXSprite : public CGameObject {
private:
	CD3DXTexture *D3DTexture;

	BYTE cPos;		//��_�̈ʒu

	// ���_�̈ʒu
	float origX;
	float origY;

	//�e�N�X�`���̕�(�c������T�C�Y���O��)
	int texWidth;

	// �X�v���C�g�̃T�C�Y
	int sWidth;
	int sHeight;

	// �]���͈�
	RECT drawRect;
	
	void Reset();

public:
	CD3DXSprite();
	CD3DXSprite(CD3DXTexture *source);
	
	void SetTexture(CD3DXTexture *source);

	void SetCenterPosition(BYTE pos);
	BYTE GetCenterPosition();

	// �A�j���[�V�����̃t���[��������̃L�����N�^�[�T�C�Y�̐ݒ�E�擾 
	void GetSpriteSize(int *width, int *height);
	void SetSpriteSize(int width, int height);

	// �t���[���P�ʂł͂Ȃ�����͈̔͂̃O���t�B�b�N��]�������Ƃ��ɂ�  
	// SetSpriteRect()�Œ��ڔ͈͂��w��ł���悤�ɂ��� 
	void SetSpriteRect(int left, int top, int right, int bottom);

	// �A�j���[�V�����̃t���[�����`��͈͂��v�Z����  
	void SetFrame(int frame);

	void Draw(float x, float y,								 int alpha = 255, BOOL antiAliasingFlag = ANTI_ALIASNIG_ACTIVE);
	void Draw(float x, float y,						float r, int alpha = 255, BOOL antiAliasingFlag = ANTI_ALIASNIG_ACTIVE);
	void Draw(float x, float y, float ex, float ey,			 int alpha = 255, BOOL antiAliasingFlag = ANTI_ALIASNIG_ACTIVE);
	void Draw(float x, float y, float ex, float ey, float r, int alpha = 255, BOOL antiAliasingFlag = ANTI_ALIASNIG_ACTIVE);

};

#endif // !_CD3DXSprite_h_