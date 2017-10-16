#pragma once
#include "D3DXSprite.h"
#include "D3DXTexture.h"
#include "GameObject.h"
class font : public CGameObject {
private:
	CD3DXSprite sprite;
	CD3DXTexture tex;
	char message[128];

public:
	void Initialize();
	void Update();
};

