#include "DirectInput.h"



CDirectInput::CDirectInput() {
	lpInput = NULL;
	lpKeyDevice = NULL;
}


CDirectInput::~CDirectInput() {
	RELEASE(lpKeyDevice);
	RELEASE(lpInput);
}

void CDirectInput::Initialize() {
	if (lpInput == NULL) {
		HRESULT hr;
		//�C���^�[�t�F�C�X�̎擾
		hr = DirectInput8Create(
			GetHInstance(),			// �\�t�g�̃C���X�^���X�n���h��  
			DIRECTINPUT_VERSION,	// DirectInput�̃o�[�W����
			IID_IDirectInput8,		// �擾����C���^�[�t�F�C�X�̃^�C�v 
			(LPVOID*)&lpInput,		// �C���^�[�t�F�C�X�̊i�[��
			NULL					// COM�W���̐���I�u�W�F�N�g�i�g��Ȃ��̂�NULL�j  
		);
		if (FAILED(hr)) {
			oLog.Log("DirectInput�̏������Ɏ��s���܂����BCDirectInput.cpp L.20");
			return;
		}
		else {
			oLog.Log("DirectInput�̏������ɐ������܂���");
		}

		hr = lpInput->CreateDevice(
			GUID_SysKeyboard,	// �󂯕t������̓f�o�C�X 
			&lpKeyDevice,		// IDirectInputDevice8�i�[��
			NULL				// COM�W���̐���I�u�W�F�N�g�i�g��Ȃ��̂�NULL�j 
		);

		if (FAILED(hr)) {
			oLog.Log("DirectInputDevice�̏������Ɏ��s���܂����BCDirectInput.cpp L.35");
			return;
		}

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		lpKeyDevice->SetDataFormat(&c_dfDIKeyboard);

		// �������[�h�̐ݒ�  
		lpKeyDevice->SetCooperativeLevel(GetHWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		//�L�[�o�b�t�@�̏�����
		ZeroMemory(keyData, 256);
	}
}

void CDirectInput::Update() {
	 
	if (lpKeyDevice) {

		// ���͂̎󂯕t���J�n 
		lpKeyDevice->Acquire();

		// �f�[�^���擾�i256�͎擾����f�[�^�̃T�C�Y�j
		lpKeyDevice->GetDeviceState(256, keyData);
	}
}

BOOL CDirectInput::IsKeyDown(int key) {

	if (lpKeyDevice == NULL) return FALSE;

	// �ŏ�ʂP�r�b�g�Ɂu������Ă��邩�v�ǂ����̃f�[�^���i�[����Ă��邱�Ƃɒ���  
	// �ŉ��ʂP�r�b�g�ɂ́u�g�O���i�I���E�I�t�̏�ԁj�v���i�[����Ă���  
	return (keyData[key] & 0x80) ? TRUE : FALSE;
}

BOOL CDirectInput::IsKeyPressed(int key) {

	if (lpKeyDevice == NULL) return FALSE;

	// ���݉�����Ă��Ă����O�ɉ�����Ă��Ȃ��Ƃ�TRUE��Ԃ� 
	if ((keyData[key] & 0x80) && !(lastKeyData[key] & 0x80)) return TRUE;

	return FALSE;
}

BOOL CDirectInput::IsKeyReleased(int key) {

	if (lpKeyDevice == NULL) return FALSE;

	// ���݉�����Ă��炸�����O�ɂ͉�����Ă����Ƃ�TRUE��Ԃ� 
	if (!(keyData[key] & 0x80) && (lastKeyData[key] & 0x80)) return TRUE;

	return FALSE;
}