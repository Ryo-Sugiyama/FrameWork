#pragma once
#include "D3DXSprite.h"
#include "D3DXTexture.h"
#include "GameObject.h"
class CAnimationTest : public CGameObject
{
private:
	CD3DXTexture tex;
	CD3DXSprite sprite;
protected:
	void Initialize();
	void Update();
};

