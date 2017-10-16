#include "D3DTaskHead.h"


void CD3DTaskHead::Initialize() {
	lastTime = 0;	// �l�̏����� 
}

void CD3DTaskHead::Update() {

	const DWORD WaitTime = 16;	//�@��60FPS

	//�@�Ō�ɍs������������̎��Ԃ𒲂ׂ�
	DWORD nTime = timeGetTime();
	DWORD rTime = nTime - lastTime;
	lastTime = nTime;

	if (rTime < WaitTime) {
		//�E�G�C�g����
		Sleep(WaitTime - rTime);
	}

	//�@�摜�̃N���A
	lpD3Ddevice->Clear(
		0,							// �N���A����̈�̔z���  
		NULL,						// �N���A����̈�̔z��  
		D3DCLEAR_TARGET,			// �Ώۂ��w��̐F�ŃN���A����  
		D3DCOLOR_XRGB(0, 0, 64),	// �N���A����F�����F�Ɏw��  
		1.0f,						// z�����̃N���A�i1.0f�ł��ׂĂ��N���A�j
		0							// �X�e���V���o�b�t�@�̃N���A
	);

	// �J�n�錾
	lpD3Ddevice->BeginScene();
}