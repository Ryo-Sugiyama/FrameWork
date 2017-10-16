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

	// Direct3D�I�u�W�F�N�g�𐶐� 
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(lpD3D)) {
		oLog.Log("DirectXDevice�̏������Ɏ��s���܂����BGameObject.cpp L.16");
		return FALSE;
	}
	else
		oLog.Log("DirectXDevice�̏������ɐ������܂����B");
	
	D3DDISPLAYMODE d3ddm;

	// Direct3DDevice�I�u�W�F�N�g�𐶐�
	if (FAILED(lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		oLog.Log("DirectX3DDevice�̏������Ɏ��s���܂����BGameObject.cpp L.24");
		return FALSE;
	}
	else
		oLog.Log("DirectX3DDevice�̏������ɐ������܂����B");

	// �ׂ�����ʃ��[�h�̐ݒ�  
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// �\���̂̒��g��0�ŏ���
	d3dpp.Windowed = TRUE;										// �E�B���h�E���[�h
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �f�B�X�v���C�h���C�o�ɍł������I�ȕ��@��I��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;					// �o�b�N�O���E���h�`��̕��@�̓V�X�e���Ɉ�C
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �`��X�V�̎w�������莟��X�V
																
	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬 (�v���C�}���f�B�X�v���C�ɕ`��, �O���t�B�b�N�{�[�h�̗��p��D�悵�ď���, �\������E�B���h�E, �O���t�B�b�N�{�[�h�̗��p��D�悵�Ē��_����, ��ʃ��[�h�̏�Ԃ��i�[�����\����, �i�[����Direct3DDevice9�ւ̃|�C���^)
	if (FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3Ddevice))) {
		oLog.Log("3DDeviceObject�̏������Ɏ��s���܂����BGameObject.cpp L.39");
		return FALSE;
	}
	else
		oLog.Log("3DDeviceObject�̏������ɐ������܂����B");

	// �A���t�@�u�����h�̐ݒ�
	lpD3Ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �`���ɂ���摜�ɑ΂���u�����h���@���A���t�@�l�ɂ��`��Ɏw��
	lpD3Ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // �`�挳�̉摜�ɑ΂���u�����h���@���摜�̏�Ԃɂ��킹�ĕ`���摜�̃A���t�@�l���ς��悤�Ɏw�� 
	lpD3Ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// �A���t�@�u�����h��L���ɂ���  

	// �X�v���C�g�I�u�W�F�N�g�̍쐬
	if (FAILED(D3DXCreateSprite(lpD3Ddevice, &lpSprite))) {
		oLog.Log("SpriteObject�̍쐬�Ɏ��s���܂����BGameObject.cpp L.50");
		return FALSE;
	}
	else
		oLog.Log("SpriteObject�̍쐬�ɐ������܂����B");

	return TRUE;
}

void CGameObject::TearDown() {

	RELEASE(lpSprite);
	RELEASE(lpD3Ddevice);
	RELEASE(lpD3D);

	// ���X�g��new�Œǉ������f�[�^�����ׂď���
	std::list<ListData>::iterator it;
	for (it = objectList.begin(); it != objectList.end(); it++) {
		if ((*it).autoDelete == true) delete (*it).gameObj;
	}
}

void CGameObject::AppendObject(ListData &listData) {
	if (listData.gameObj == NULL) return;

	// �������֐������s 
	listData.gameObj->Initialize();

	// ���X�g�̐擪�ɒǉ� 
	if (objectList.size() == 0) {
		objectList.push_front(listData);
		return;
	}

	// �I�u�W�F�N�g�̏��ʂɏ]���}��  
	std::list<ListData>::iterator it;
	for (it = objectList.begin(); it != objectList.end(); it++) {
		
		//�@�����D�揇�ʂ����������炻�̏ꏊ�ɑ}��
		if ((*it).priority > listData.priority) {
			objectList.insert(it, listData);
			return;
		}
	}
	// �Ԃɓ����Ƃ��낪�Ȃ���΁A���X�g�̍Ō�ɒǉ�
	objectList.push_back(listData);
}

void CGameObject::AppendObject(CGameObject *gameObject, int priority, bool autoDelete) {

	//�@���O�𖢋L�������ɂ��ē]��
	AppendObject(gameObject, priority, NULL, autoDelete);
}

void CGameObject::AppendObject(CGameObject *gameObject, int priority, LPCSTR name, bool autoDelete) {

	ListData Id;
	Id.priority = priority;

	
	if (name != NULL)
		// ���O���L���ς݂ł���΁A�I�u�W�F�N�g�ɖ��O��t����
		strcpy_s(Id.name, NameSize, name);
	else
		//�@�����łȂ���΁Aname�̗̈敪�[���ŏ�����
		ZeroMemory(Id.name, NameSize * sizeof(char));
	
	Id.gameObj = gameObject;
	Id.autoDelete = autoDelete;

	// ���ۂɃ��X�g�֊i�[
	AppendObject(Id);
}

void CGameObject::RemoveObject(CGameObject *target) {
	std::list<ListData>::iterator it = objectList.begin();

	while (it != objectList.end()) {

		// ��������C�e���[�^�����݃^�X�N�������̂��̂ł����
		if ((*it).gameObj == target) {

			// �t���O�𗧂ĂāA�^�X�N�̃C�e���[�^�����ֈړ�
			if (itTask == it) {
				itMoved = true;
				itTask++;
			}
			// �Q�[���I�u�W�F�N�g�����
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

	// �D��x���w�肵�ẴI�u�W�F�N�g�폜
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
		// �^�X�N�ǉ����ɗp����������ƌ����ɗp����L�[������Ɗ��S��v
		if (strcmp((*it).name, name) == 0) return (*it).gameObj;
	}

	return NULL;
}

void CGameObject::AppendItemBox(LPCSTR name, CGameObject *object) {
	
	// �f�[�^��ǉ�����ɂ́Amake_pair�֐���p���č쐬���ꂽ�f�[�^�łȂ���΂Ȃ�Ȃ�
	itemBox.insert(make_pair(name, object));
}

void CGameObject::RemoveItemBox(LPCSTR name) {

	//�@�Ή�����f�[�^���ꗗ����폜
	itemBox.erase(name);
}

void CGameObject::ResetItemBox() {

	//�@���ׂẴf�[�^���폜
	itemBox.clear();
}

CGameObject* CGameObject::FindItemBox(LPCSTR name) {

	hash_map<string, CGameObject*>::iterator it;

	//�@�L�[�ɑΉ�����v�f���擾
	it = itemBox.find(name);

	//�@�ꗗ�ɑΉ�����f�[�^�����݂��Ȃ��ꍇ��NULL��Ԃ�
	if (it == itemBox.end()) return NULL;

	// �Ή�����f�[�^������΁A���̈ʒu�������C�e���[�^��Ԃ�
	return it->second;
}

void CGameObject::DoAllTask() {

	itTask = objectList.begin();

	itMoved = false;

	while (itTask != objectList.end()) {
		itTask->gameObj->Update();

		if (itMoved == false) 	
			// ���Ɉړ�
			itTask++;	
		else	
			// �t���O�����Z�b�g
			itMoved = false;
	}
}