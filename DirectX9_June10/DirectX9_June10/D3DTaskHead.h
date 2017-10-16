// @description
/*
*　描画の開始処理をするクラス
*　copyright © Ryo Sugiyama All write reserved.
*/
#ifndef _D3DTaskHead_h_
#define _D3DTaskHead_h_

#include "GameObject.h"

class CD3DTaskHead : public CGameObject {
private:
	DWORD lastTime;

protected:
	void Initialize() override;
	void Update() override;

};

#endif // !_D3DTaskHead_h_