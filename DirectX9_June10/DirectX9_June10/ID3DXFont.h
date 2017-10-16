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

	// �Œ���̎w�肾���ł��ނ��̂����� 
	// [x, y]�̈ʒu������ɂ��ĕ����񂪕`�悳���  
	void Draw(LPCTSTR text, int x, int y);

	//���ׂẴp�����[�^���w�肵�ĕ`�悷�銮�S��
	void Draw(LPCTSTR text, int count, LPRECT pRect, DWORD format, D3DXCOLOR color);

	inline const LPD3DXFONT GetD3DXFont() { return lpD3DxFont; }
};

#endif // !_CID3DXFont_h_