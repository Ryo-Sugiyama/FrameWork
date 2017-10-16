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

	BYTE cPos;		//基点の位置

	// 原点の位置
	float origX;
	float origY;

	//テクスチャの幅(縦横同一サイズが前提)
	int texWidth;

	// スプライトのサイズ
	int sWidth;
	int sHeight;

	// 転送範囲
	RECT drawRect;
	
	void Reset();

public:
	CD3DXSprite();
	CD3DXSprite(CD3DXTexture *source);
	
	void SetTexture(CD3DXTexture *source);

	void SetCenterPosition(BYTE pos);
	BYTE GetCenterPosition();

	// アニメーションのフレーム当たりのキャラクターサイズの設定・取得 
	void GetSpriteSize(int *width, int *height);
	void SetSpriteSize(int width, int height);

	// フレーム単位ではなく特定の範囲のグラフィックを転送したときには  
	// SetSpriteRect()で直接範囲を指定できるようにする 
	void SetSpriteRect(int left, int top, int right, int bottom);

	// アニメーションのフレームより描画範囲を計算する  
	void SetFrame(int frame);

	void Draw(float x, float y,								 int alpha = 255, BOOL antiAliasingFlag = ANTI_ALIASNIG_ACTIVE);
	void Draw(float x, float y,						float r, int alpha = 255, BOOL antiAliasingFlag = ANTI_ALIASNIG_ACTIVE);
	void Draw(float x, float y, float ex, float ey,			 int alpha = 255, BOOL antiAliasingFlag = ANTI_ALIASNIG_ACTIVE);
	void Draw(float x, float y, float ex, float ey, float r, int alpha = 255, BOOL antiAliasingFlag = ANTI_ALIASNIG_ACTIVE);

};

#endif // !_CD3DXSprite_h_