// @description
/*
* コードの実行ログをファイルに書き出すクラス
* copyright © Ryo Sugiyama All write reserved.
*/
#ifndef _COLogFile_h_
#define _COLogFile_h_

#include <Windows.h>
#include <tchar.h>
#include <fstream>

class COLogFile {

private:
	std::ofstream ofs;	//ファイル出力用変数

public:

	void SetUp();

	//Log(std::string ログに書き込む内容);
	void Log(const char *logString);
};

#endif // !_COLogFile_h_