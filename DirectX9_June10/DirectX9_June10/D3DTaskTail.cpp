#include "D3DTaskTail.h"


void CD3DTaskTail::Update() {

	//�@�X�v���C�g�̕\��
	lpD3Ddevice->EndScene();

	lpD3Ddevice->Present(
		NULL,	// �]�����̗̈�iNULL�őS��w��j
		NULL,	// �]����̗̈�iNULL�őS��w��j 
		NULL,	// �]����̃E�B���h�E�������n���h���iNULL�ŕW���̃E�B���h�E�j
		NULL	// ���s�o�[�W�����ł͏��NULL���w�� 
		);
}
