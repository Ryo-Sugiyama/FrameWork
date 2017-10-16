#ifndef _CDirectSound_h_
#define _CDirectSound_h_

#include "GameObject.h"
#include <dsound.h>
#pragma comment (lib,"dsound.lib")
#pragma comment (lib,"dxguid.lib")

class CDirectSound : public CGameObject {

private:
	friend int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int);
	static LPDIRECTSOUND8 lpDSound;
	LPDIRECTSOUNDBUFFER lpDSBuffer;

	// �Đ��I�����֒m 
	LPDIRECTSOUNDNOTIFY lpDSNotify;
	HANDLE hEvent;
	static void CreateDirectSound(HWND hWnd);
	static void ReleaseDirectSound();

	// �V�K�X���b�h�ɗp����֐�
	static DWORD WINAPI NotifyFunction(LPVOID param);

public:
	CDirectSound();

	// �R�s�[�R���X�g���N�^�̐錾  
	CDirectSound(const CDirectSound&);
	CDirectSound(LPCTSTR fileName);

	// ������Z�̊֐�
	CDirectSound& operator=(const CDirectSound&);
	~CDirectSound();

	void EnableDeleteByEnd();
	void Reset();

	BOOL LoadSound(LPCTSTR fileName);
	void SoundPlay();

	void Stop();
	void SetVolume(LONG volume);
	void SetPan(LONG lPan);
};

#endif // !_CDirectSound_h_