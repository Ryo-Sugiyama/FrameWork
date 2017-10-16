#ifndef _CID3DXFont_h_
#define _CID3DXFont_h_

#include "GameObject.h"
#include "OLogFile.h"

class CID3DXFont : public CGameObject {
private:
	LPD3DXFONT lpD3DxFont;
	COLogFile oLog;
public:
	CID3DXFont();
	CID3DXFont(int fontSize);
	~CID3DXFont();

	BOOL Create(int fontSize);

	// 最低限の指定だけですむものぐさ版 
	// [x, y]の位置を左上にして文字列が描画される  
	void Draw(LPCTSTR text, int x, int y);

	//すべてのパラメータを指定して描画する完全版
	void Draw(LPCTSTR text, int count, LPRECT pRect, DWORD format, D3DXCOLOR color);

	inline const LPD3DXFONT GetD3DXFont() { return lpD3DxFont; }
};

#endif // !_CID3DXFont_h_