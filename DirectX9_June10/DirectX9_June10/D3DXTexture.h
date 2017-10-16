#ifndef _D3DXSpriteTexture_h_
#define _D3DXSpriteTexture_h_

#include "GameObject.h"
#include "OLogFile.h"

class CD3DXTexture : public CGameObject {

private:
	// テクスチャを格納するインタフェイス  
	LPDIRECT3DTEXTURE9 lpD3DTexture;
	COLogFile oLog;
	int texWidth;
	int texHeight;
public:
	CD3DXTexture();
	CD3DXTexture(LPCTSTR fileName);
	~CD3DXTexture();

	// 画像ファイルからテクスチャを作成する関数
	BOOL LoadGraph(LPCTSTR fileName);

	// テクスチャサイズを取得する関数
	void GetTextureSize(int *width, int *height);

	// テクスチャデータを直接取得する関数
	inline const LPDIRECT3DTEXTURE9 GetTexture() { return lpD3DTexture; }
};

#endif // !_D3DXSpriteTexture_h_