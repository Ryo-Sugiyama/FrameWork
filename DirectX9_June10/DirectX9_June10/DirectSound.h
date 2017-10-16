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

	// 再生終了を関知 
	LPDIRECTSOUNDNOTIFY lpDSNotify;
	HANDLE hEvent;
	static void CreateDirectSound(HWND hWnd);
	static void ReleaseDirectSound();

	// 新規スレッドに用いる関数
	static DWORD WINAPI NotifyFunction(LPVOID param);

public:
	CDirectSound();

	// コピーコンストラクタの宣言  
	CDirectSound(const CDirectSound&);
	CDirectSound(LPCTSTR fileName);

	// 代入演算の関数
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