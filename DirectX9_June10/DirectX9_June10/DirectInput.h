#ifndef _DirectInput_h_
#define _DirectInput_h_

#include "GameObject.h"

// ���p����DirectInput�o�[�W�����̐錾 
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// DirectInput�Ɋ֘A���郉�C�u�����̃����N��錾  
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class CDirectInput : public CGameObject {

private:
	LPDIRECTINPUT8 lpInput;
	LPDIRECTINPUTDEVICE8 lpKeyDevice;
	BYTE keyData[256];
	BYTE lastKeyData[256];

protected:
	void Initialize();
	void Update();

public:
	CDirectInput();
	~CDirectInput();

	BOOL IsKeyDown(int key);
	BOOL IsKeyPressed(int key);
	BOOL IsKeyReleased(int key);

};

#endif // !_DirectInput_h_