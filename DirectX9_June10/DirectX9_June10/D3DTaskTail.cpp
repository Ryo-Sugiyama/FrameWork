#include "D3DTaskTail.h"


void CD3DTaskTail::Update() {

	//　スプライトの表示
	lpD3Ddevice->EndScene();

	lpD3Ddevice->Present(
		NULL,	// 転送元の領域（NULLで全域指定）
		NULL,	// 転送先の領域（NULLで全域指定） 
		NULL,	// 転送先のウィンドウを示すハンドル（NULLで標準のウィンドウ）
		NULL	// 現行バージョンでは常にNULLを指定 
		);
}
