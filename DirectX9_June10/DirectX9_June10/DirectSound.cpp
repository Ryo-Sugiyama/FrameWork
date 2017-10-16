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

	// IDirectSound8の作成
	DirectSoundCreate8(
		&DSDEVID_DefaultPlayback,	// 標準のハードウェアを利用  
		&lpDSound,					// 格納するIDirectSound8インターフェース 
		NULL						// 現行バージョンではNULLを指定  
	);

	// 優先度の指定
	lpDSound->SetCooperativeLevel(
		hWnd,				// 音楽再生に利用するウィンドウのハンドル  
		DSSCL_PRIORITY		// 優先度  
	);
}

void CDirectSound::ReleaseDirectSound() {
	RELEASE(lpDSound);
}

BOOL CDirectSound::LoadSound(LPCTSTR fileName) {

	// waveファイルを開く
	FILE *fp;
	if (_tfopen_s(&fp, fileName, _T("rb"))) {
		oLog.Log("ファイルが開けません。 DirectSound.cpp L.40");
		return FALSE;
	}

	char buf[10];
	fread(buf, 4, 1, fp);
	if (memcmp(buf, "RIFF", 4) != 0) {
		oLog.Log("RIFFフォーマットではありません。 DirectSound.cpp L.47");
		return FALSE;
	}

	fseek(fp, 4, SEEK_CUR);

	fread(buf, 8, 1, fp);
	if (memcmp(buf, "WAVEfmt", 8) != 0) {
		oLog.Log("WAVEフォーマットではないか、フォーマット定義がありません。 DirectSound.cpp L.54");
		return FALSE;
	}

	// fmtデータサイズエリアを読み飛ばす  
	fseek(fp, 4, SEEK_CUR);

	// フォーマット情報を取得  
	WAVEFORMATEX wavFmt;
	fread(&wavFmt, sizeof(WAVEFORMATEX) - 2, 1, fp);

	// 音楽データの開始を意味する「data」の文字列があるか調べる  
	ZeroMemory(buf, 10);
	while (strcmp("data", buf)) {
		buf[0] = fgetc(fp);
		if (buf[0] == EOF) {
			oLog.Log("波形データ定義が見つかりません");

			fclose(fp);
			return FALSE;
		}
		if (buf[0] == 'd') fread(&buf[1], 1, 3, fp);
	}
	int waveSize;
	fread(&waveSize, sizeof(int), 1, fp);

	// CreateSoundBufferに送信するための音楽情報を作成  
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_LOCDEFER;
	desc.dwBufferBytes = waveSize;
	desc.lpwfxFormat = &wavFmt;

	RELEASE(lpDSBuffer);
	lpDSound->CreateSoundBuffer(&desc, &lpDSBuffer, NULL);

	// アクセス可能なバッファのサイズ   
	DWORD buf_size;

	// WAVバッファアクセスポイントを格納する為のポインタ  
	LPVOID lpvAudioPtr;

	// バッファロック
	lpDSBuffer->Lock(
		0, 0,						// バッファ全体をロックするため、数値の指定は不要 
		&lpvAudioPtr, &buf_size,	// 書き込むバッファを取得するためのポインタ  
		NULL, NULL,					// 2つに分けて書き込むこともできる 
		DSBLOCK_ENTIREBUFFER		// バッファすべてをロック  
	);

	// サウンドデータをバッファへ書き込む
	fread(lpvAudioPtr, buf_size, 1, fp);

	// ロック解除  
	lpDSBuffer->Unlock(lpvAudioPtr, buf_size, NULL, NULL);

	fclose(fp);
	
	return TRUE;
}

// 別スレッドに渡すデータを格納する構造体
struct PlayThreadData {
	HANDLE hEvent;
	CDirectSound *soundObj;
};

void CDirectSound::SoundPlay() {

	if (lpDSBuffer) {
		lpDSBuffer->SetCurrentPosition(0),	//　再生場所を先頭に指定
			lpDSBuffer->Play(
				0,			//　必ず0を指定
				0,			//　再生の優先度。0が最も低く、0xFFFFFFFFが最も高い
				0			//　再生オプション。例えばDSBPLAY_LOOPINGならループになる
			);
		if (lpDSNotify) {

			// 再生終了を監視するスレッドを立ち上げる  
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

	// 再生終了通知が来るまで待機 
	WaitForMultipleObjects(1, &data->hEvent, FALSE, INFINITE);

	//　終了したら対象のオブジェクトを消去
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

	// IDirectSoundNotifyの作成
	HRESULT hr = lpDSBuffer->QueryInterface(
		IID_IDirectSoundNotify,
		(LPVOID*)&lpDSNotify
	);

	// 通知イベントに使用するハンドルの作成  
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// イベント発生に関する情報を作成  
	DSBPOSITIONNOTIFY pn;
	pn.dwOffset = DSBPN_OFFSETSTOP; // 終了まで再生  
	pn.hEventNotify = hEvent;       // ハンドラを関連づける

	// イベントデータを登録  
	lpDSNotify->SetNotificationPositions(1, &pn);
}

