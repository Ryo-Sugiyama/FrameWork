#include "DirectSound.h"

LPDIRECTSOUND8 CDirectSound::lpDSound;

CDirectSound::CDirectSound() {
	lpDSBuffer = NULL;
}

CDirectSound::CDirectSound(const CDirectSound &s) {
	RELEASE(lpDSBuffer);
	if (lpDSound) lpDSound->DuplicateSoundBuffer(s.lpDSBuffer, &lpDSBuffer);
}

CDirectSound::CDirectSound(LPCTSTR fileName) {
	lpDSBuffer = NULL;
	LoadSound(fileName);
}

CDirectSound& CDirectSound::operator =(const CDirectSound &s) {
	RELEASE(lpDSBuffer);
	if (lpDSound) lpDSound->DuplicateSoundBuffer(s.lpDSBuffer, &lpDSBuffer);

	return (*this);
}

CDirectSound::~CDirectSound() {
	if (hEvent) CloseHandle(hEvent);
	RELEASE(lpDSNotify);
	RELEASE(lpDSBuffer);
}

void CDirectSound::CreateDirectSound(HWND hWnd) {

	// IDirectSound8�̍쐬
	DirectSoundCreate8(
		&DSDEVID_DefaultPlayback,	// �W���̃n�[�h�E�F�A�𗘗p  
		&lpDSound,					// �i�[����IDirectSound8�C���^�[�t�F�[�X 
		NULL						// ���s�o�[�W�����ł�NULL���w��  
	);

	// �D��x�̎w��
	lpDSound->SetCooperativeLevel(
		hWnd,				// ���y�Đ��ɗ��p����E�B���h�E�̃n���h��  
		DSSCL_PRIORITY		// �D��x  
	);
}

void CDirectSound::ReleaseDirectSound() {
	RELEASE(lpDSound);
}

BOOL CDirectSound::LoadSound(LPCTSTR fileName) {

	// wave�t�@�C�����J��
	FILE *fp;
	if (_tfopen_s(&fp, fileName, _T("rb"))) {
		oLog.Log("�t�@�C�����J���܂���B DirectSound.cpp L.40");
		return FALSE;
	}

	char buf[10];
	fread(buf, 4, 1, fp);
	if (memcmp(buf, "RIFF", 4) != 0) {
		oLog.Log("RIFF�t�H�[�}�b�g�ł͂���܂���B DirectSound.cpp L.47");
		return FALSE;
	}

	fseek(fp, 4, SEEK_CUR);

	fread(buf, 8, 1, fp);
	if (memcmp(buf, "WAVEfmt", 8) != 0) {
		oLog.Log("WAVE�t�H�[�}�b�g�ł͂Ȃ����A�t�H�[�}�b�g��`������܂���B DirectSound.cpp L.54");
		return FALSE;
	}

	// fmt�f�[�^�T�C�Y�G���A��ǂݔ�΂�  
	fseek(fp, 4, SEEK_CUR);

	// �t�H�[�}�b�g�����擾  
	WAVEFORMATEX wavFmt;
	fread(&wavFmt, sizeof(WAVEFORMATEX) - 2, 1, fp);

	// ���y�f�[�^�̊J�n���Ӗ�����udata�v�̕����񂪂��邩���ׂ�  
	ZeroMemory(buf, 10);
	while (strcmp("data", buf)) {
		buf[0] = fgetc(fp);
		if (buf[0] == EOF) {
			oLog.Log("�g�`�f�[�^��`��������܂���");

			fclose(fp);
			return FALSE;
		}
		if (buf[0] == 'd') fread(&buf[1], 1, 3, fp);
	}
	int waveSize;
	fread(&waveSize, sizeof(int), 1, fp);

	// CreateSoundBuffer�ɑ��M���邽�߂̉��y�����쐬  
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_LOCDEFER;
	desc.dwBufferBytes = waveSize;
	desc.lpwfxFormat = &wavFmt;

	RELEASE(lpDSBuffer);
	lpDSound->CreateSoundBuffer(&desc, &lpDSBuffer, NULL);

	// �A�N�Z�X�\�ȃo�b�t�@�̃T�C�Y   
	DWORD buf_size;

	// WAV�o�b�t�@�A�N�Z�X�|�C���g���i�[����ׂ̃|�C���^  
	LPVOID lpvAudioPtr;

	// �o�b�t�@���b�N
	lpDSBuffer->Lock(
		0, 0,						// �o�b�t�@�S�̂����b�N���邽�߁A���l�̎w��͕s�v 
		&lpvAudioPtr, &buf_size,	// �������ރo�b�t�@���擾���邽�߂̃|�C���^  
		NULL, NULL,					// 2�ɕ����ď������ނ��Ƃ��ł��� 
		DSBLOCK_ENTIREBUFFER		// �o�b�t�@���ׂĂ����b�N  
	);

	// �T�E���h�f�[�^���o�b�t�@�֏�������
	fread(lpvAudioPtr, buf_size, 1, fp);

	// ���b�N����  
	lpDSBuffer->Unlock(lpvAudioPtr, buf_size, NULL, NULL);

	fclose(fp);
	
	return TRUE;
}

// �ʃX���b�h�ɓn���f�[�^���i�[����\����
struct PlayThreadData {
	HANDLE hEvent;
	CDirectSound *soundObj;
};

void CDirectSound::SoundPlay() {

	if (lpDSBuffer) {
		lpDSBuffer->SetCurrentPosition(0),	//�@�Đ��ꏊ��擪�Ɏw��
			lpDSBuffer->Play(
				0,			//�@�K��0���w��
				0,			//�@�Đ��̗D��x�B0���ł��Ⴍ�A0xFFFFFFFF���ł�����
				0			//�@�Đ��I�v�V�����B�Ⴆ��DSBPLAY_LOOPING�Ȃ烋�[�v�ɂȂ�
			);
		if (lpDSNotify) {

			// �Đ��I�����Ď�����X���b�h�𗧂��グ��  
			PlayThreadData *data = new PlayThreadData;
			data->hEvent = hEvent;
			data->soundObj = this;
			DWORD threadID;
			CreateThread(NULL, 0, CDirectSound::NotifyFunction, data, 0, &threadID);
		}
	}
}

DWORD CDirectSound::NotifyFunction(LPVOID param) {

	PlayThreadData *data = (PlayThreadData*)param;

	// �Đ��I���ʒm������܂őҋ@ 
	WaitForMultipleObjects(1, &data->hEvent, FALSE, INFINITE);

	//�@�I��������Ώۂ̃I�u�W�F�N�g������
	delete data->soundObj;

	delete data;

	return 0L;
}

void CDirectSound::Stop() {
	if (lpDSBuffer) lpDSBuffer->Stop();
}

void CDirectSound::SetVolume(LONG volume) {
	if (lpDSBuffer) lpDSBuffer->SetVolume(volume);
}

void CDirectSound::SetPan(LONG lPan) {
	if (lpDSBuffer) lpDSBuffer->SetPan(lPan);
}

void CDirectSound::Reset() {
	lpDSBuffer = NULL;
	lpDSNotify = NULL;
	hEvent = NULL;
}

void CDirectSound::EnableDeleteByEnd() {
	if (lpDSBuffer == NULL) return;

	// IDirectSoundNotify�̍쐬
	HRESULT hr = lpDSBuffer->QueryInterface(
		IID_IDirectSoundNotify,
		(LPVOID*)&lpDSNotify
	);

	// �ʒm�C�x���g�Ɏg�p����n���h���̍쐬  
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// �C�x���g�����Ɋւ�������쐬  
	DSBPOSITIONNOTIFY pn;
	pn.dwOffset = DSBPN_OFFSETSTOP; // �I���܂ōĐ�  
	pn.hEventNotify = hEvent;       // �n���h�����֘A�Â���

	// �C�x���g�f�[�^��o�^  
	lpDSNotify->SetNotificationPositions(1, &pn);
}

