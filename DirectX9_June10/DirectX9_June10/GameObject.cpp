#include "GameObject.h"

HWND		CGameObject::hWnd = NULL;
HINSTANCE	CGameObject::hInstance = NULL;

bool		CGameObject::itMoved;

list<ListData> CGameObject::objectList;
list<ListData>::iterator CGameObject::itTask;

hash_map<string, CGameObject*> CGameObject::itemBox;

LPDIRECT3D9         CGameObject::lpD3D = NULL;
LPDIRECT3DDEVICE9   CGameObject::lpD3Ddevice = NULL;
LPD3DXSPRITE        CGameObject::lpSprite = NULL;

BOOL CGameObject::SetUp(HWND hWnd, HINSTANCE hInstance) {

	this->hWnd = hWnd;
	this->hInstance = hInstance;

	// Direct3Dオブジェクトを生成 
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(lpD3D)) {
		oLog.Log("DirectXDeviceの初期化に失敗しました。GameObject.cpp L.16");
		return FALSE;
	}
	else
		oLog.Log("DirectXDeviceの初期化に成功しました。");
	
	D3DDISPLAYMODE d3ddm;

	// Direct3DDeviceオブジェクトを生成
	if (FAILED(lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		oLog.Log("DirectX3DDeviceの初期化に失敗しました。GameObject.cpp L.24");
		return FALSE;
	}
	else
		oLog.Log("DirectX3DDeviceの初期化に成功しました。");

	// 細かい画面モードの設定  
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// 構造体の中身を0で初期
	d3dpp.Windowed = TRUE;										// ウィンドウモード
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ディスプレイドライバに最も効率的な方法を選択
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;					// バックグラウンド描画の方法はシステムに一任
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// 描画更新の指示があり次第更新
																
	// D3Dデバイスオブジェクトの作成 (プライマリディスプレイに描画, グラフィックボードの利用を優先して処理, 表示するウィンドウ, グラフィックボードの利用を優先して頂点処理, 画面モードの状態を格納した構造体, 格納するDirect3DDevice9へのポインタ)
	if (FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3Ddevice))) {
		oLog.Log("3DDeviceObjectの初期化に失敗しました。GameObject.cpp L.39");
		return FALSE;
	}
	else
		oLog.Log("3DDeviceObjectの初期化に成功しました。");

	// アルファブレンドの設定
	lpD3Ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// 描画先にある画像に対するブレンド方法をアルファ値による描画に指定
	lpD3Ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // 描画元の画像に対するブレンド方法を画像の状態にあわせて描画先画像のアルファ値が変わるように指定 
	lpD3Ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// アルファブレンドを有効にする  

	// スプライトオブジェクトの作成
	if (FAILED(D3DXCreateSprite(lpD3Ddevice, &lpSprite))) {
		oLog.Log("SpriteObjectの作成に失敗しました。GameObject.cpp L.50");
		return FALSE;
	}
	else
		oLog.Log("SpriteObjectの作成に成功しました。");

	return TRUE;
}

void CGameObject::TearDown() {

	RELEASE(lpSprite);
	RELEASE(lpD3Ddevice);
	RELEASE(lpD3D);

	// リストにnewで追加したデータをすべて消去
	std::list<ListData>::iterator it;
	for (it = objectList.begin(); it != objectList.end(); it++) {
		if ((*it).autoDelete == true) delete (*it).gameObj;
	}
}

void CGameObject::AppendObject(ListData &listData) {
	if (listData.gameObj == NULL) return;

	// 初期化関数を実行 
	listData.gameObj->Initialize();

	// リストの先頭に追加 
	if (objectList.size() == 0) {
		objectList.push_front(listData);
		return;
	}

	// オブジェクトの順位に従い挿入  
	std::list<ListData>::iterator it;
	for (it = objectList.begin(); it != objectList.end(); it++) {
		
		//　もし優先順位が高かったらその場所に挿入
		if ((*it).priority > listData.priority) {
			objectList.insert(it, listData);
			return;
		}
	}
	// 間に入れるところがなければ、リストの最後に追加
	objectList.push_back(listData);
}

void CGameObject::AppendObject(CGameObject *gameObject, int priority, bool autoDelete) {

	//　名前を未記入扱いにして転送
	AppendObject(gameObject, priority, NULL, autoDelete);
}

void CGameObject::AppendObject(CGameObject *gameObject, int priority, LPCSTR name, bool autoDelete) {

	ListData Id;
	Id.priority = priority;

	
	if (name != NULL)
		// 名前が記入済みであれば、オブジェクトに名前を付ける
		strcpy_s(Id.name, NameSize, name);
	else
		//　そうでなければ、nameの領域分ゼロで初期化
		ZeroMemory(Id.name, NameSize * sizeof(char));
	
	Id.gameObj = gameObject;
	Id.autoDelete = autoDelete;

	// 実際にリストへ格納
	AppendObject(Id);
}

void CGameObject::RemoveObject(CGameObject *target) {
	std::list<ListData>::iterator it = objectList.begin();

	while (it != objectList.end()) {

		// 消去するイテレータが現在タスク処理中のものであれば
		if ((*it).gameObj == target) {

			// フラグを立てて、タスクのイテレータを次へ移動
			if (itTask == it) {
				itMoved = true;
				itTask++;
			}
			// ゲームオブジェクトを解放
			if (it->autoDelete) delete it->gameObj;

			objectList.erase(it);
			return;
		}
		else
			it++;
	}
}

void CGameObject::RemoveObject(int pBegin, int pEnd) {
	std::list<ListData>::iterator it = objectList.begin();

	// 優先度を指定してのオブジェクト削除
	while (it != objectList.end()) {
		if ((*it).priority >= pBegin) {
			if (itTask == it) {
				itMoved = true;
				itTask++;
			}
			if (it->autoDelete) delete it->gameObj;
			it = objectList.erase(it);
		}
		else if ((*it).priority > pEnd)
			break;
		else
			it++;
	}
} 

CGameObject* CGameObject::FindObject(char *name) {
	std::list<ListData>::iterator it;

	for (it = objectList.begin(); it != objectList.end(); it++) {
		// タスク追加時に用いた文字列と検索に用いるキー文字列と完全一致
		if (strcmp((*it).name, name) == 0) return (*it).gameObj;
	}

	return NULL;
}

void CGameObject::AppendItemBox(LPCSTR name, CGameObject *object) {
	
	// データを追加するには、make_pair関数を用いて作成されたデータでなければならない
	itemBox.insert(make_pair(name, object));
}

void CGameObject::RemoveItemBox(LPCSTR name) {

	//　対応するデータを一覧から削除
	itemBox.erase(name);
}

void CGameObject::ResetItemBox() {

	//　すべてのデータを削除
	itemBox.clear();
}

CGameObject* CGameObject::FindItemBox(LPCSTR name) {

	hash_map<string, CGameObject*>::iterator it;

	//　キーに対応する要素を取得
	it = itemBox.find(name);

	//　一覧に対応するデータが存在しない場合はNULLを返す
	if (it == itemBox.end()) return NULL;

	// 対応するデータがあれば、その位置を示すイテレータを返す
	return it->second;
}

void CGameObject::DoAllTask() {

	itTask = objectList.begin();

	itMoved = false;

	while (itTask != objectList.end()) {
		itTask->gameObj->Update();

		if (itMoved == false) 	
			// 次に移動
			itTask++;	
		else	
			// フラグをリセット
			itMoved = false;
	}
}