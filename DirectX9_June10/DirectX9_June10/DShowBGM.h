#ifndef _CDShowBGM_h_
#define _CDShowBGM_h_

#include <strmif.h>
#include <control.h>
#include <uuids.h>
#include "GameObject.h"

#pragma comment(lib,"strmiids.lib")

class CDShowBGM : public CGameObject {

private: 
	IGraphBuilder *pBuilder;
	IMediaControl *pMediaCtrl;
	IMediaSeeking *pMediaSeeking;
	IBasicAudio   *pBasicAudio;

	BOOL bLoop;
	BOOL bFading;

	void InitializeBGM();
	void Release();

public:
	CDShowBGM();
	CDShowBGM(LPCTSTR fileName);
	~CDShowBGM();

	void LoadBGM(LPCTSTR fileName);
	void Play();
	void Stop();
};

#endif // !_CDShowBGM_h_
