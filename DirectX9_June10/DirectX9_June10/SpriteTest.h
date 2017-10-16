#pragma once
#include "D3DXSprite.h"
#include "GameObject.h"
class SpriteTest :
	public CGameObject
{
private:
	float x, y, mx, my;
	CD3DXTexture tex;
	CD3DXSprite sprite;

protected:
	virtual void Initialize() override;
	virtual void Update() override;

public:
	SpriteTest();
};

