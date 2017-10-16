// @description
/*
* オブジェクトの一元管理をするクラス
* Copyright © Ryo Sugiyama All write resereved.
*/
#ifndef _GameObject_h_
#define _GameObject_h_

#include <Windows.h>
#include <tchar.h>
#include <list>

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <string>
using namespace std;
#include <hash_map>
using namespace stdext;


#include <d3d9.h>
#include <d3dx9.h>

#if _DEBUG
#include <crtdbg.h>
#define new  new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif

#include "OLogFile.h"

#define RELEASE(x) if(x) x->Release();	// インターフェイス解放マクロ
#define NameSize 32

class CGameObject;

inline float d2r(float d) {
	return (d / 180.0f * D3DX_PI);
}

// リストの管理情報
struct ListData {
	int priority;			//　実行順位
	char name[NameSize];	//	管理名称
	CGameObject* gameObj;	//　オブジェクトのポインタ
	bool autoDelete;		//　リスト削除と同時にオブジェクトも削除 
};

class CGameObject {
private:
	static HWND hWnd;			
	static HINSTANCE hInstance;	

	// アイテムボックスの宣言  
	static hash_map<string, CGameObject*> itemBox;

	// 格納するリスト
	static list<ListData> objectList;	

	// タスク処理時における仮イテレータ
	static bool itMoved;
	static list<ListData>::iterator itTask;

	

protected:

	// D3Dインターフェイスの宣言
	static LPDIRECT3D9 lpD3D;
	static LPDIRECT3DDEVICE9 lpD3Ddevice;
	static LPD3DXSPRITE lpSprite;
	COLogFile oLog;

	virtual void Initialize() {}
	virtual void Update() {}

public:
	virtual ~CGameObject() {}

	// WinMain()関数の引数のhWndを取得する関数
	inline static const HWND GetHWnd() { return hWnd; }

	// WinMain()関数の引数のhInstanceを取得する関数
	inline static const HINSTANCE GetHInstance() { return hInstance; }

	// リストにデータを追加する関数(オーバーロード +3)
	static void AppendObject(ListData &listData);
	static void AppendObject(CGameObject *gameObject, int priority, bool autoDelete);
	static void AppendObject(CGameObject *gameObject, int priority, LPCSTR name, bool autoDelete);

	//　リストからデータを消去する関数(オーバーロード　+2)
	static void RemoveObject(CGameObject *target);
	static void RemoveObject(int pBegin, int pEnd);

	// オブジェクトを格納する関数
	static void AppendItemBox(LPCSTR name, CGameObject *object);

	// 指定したキーに格納されているオブジェクトを取り除く関数　※実際に削除する場合はRemoveObject()を使う
	static void RemoveItemBox(LPCSTR name);

	// アイテムボックス内のデータをすべて取り除く
	static void ResetItemBox();

	// 指定した文字列に対応するオブジェクトを取得する  
	static CGameObject* FindItemBox(LPCSTR name);

	// 特定のオブジェクトを参照する関数
	CGameObject* FindObject(char *name);

	// DirectXの初期化をする関数
	BOOL SetUp(HWND hWnd, HINSTANCE hInstance);	

	// DirectXの終了をする関数
	void TearDown();	

	// すべての処理をする関数
	void DoAllTask();

	// WinMain()内での呼び出しに対応	
	friend int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int);
};

#endif // !_GameObject_h_