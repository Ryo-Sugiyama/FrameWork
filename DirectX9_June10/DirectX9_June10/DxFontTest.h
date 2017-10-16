#pragma once
#include "ID3DXFont.h"
class CDxFontTestStart : public CGameObject {

protected:
	    void Initialize();
};


class CMoveFont : public CGameObject {
public:
	void SetStartState(int pos_x, int pos_y, int speed);
protected:
	void Initialize();
	void Update();
private:
	int x, y, m;
	CID3DXFont *pFont;
};

