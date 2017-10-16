#include "OLogFile.h"
#include <cstdlib>
#include <iostream>

void COLogFile::SetUp() {
	//Log.txtファイルを書き込みモードで呼び出す。
	//ofs.close();すでにあった場合は破棄して新規作成
	ofs.open("Log.txt", std::ios::out);
	ofs.close();
}



void COLogFile::Log(const char *logString) {

	//追加書き込みモード
	ofs.open("Log.txt", std::ios::app);
	
	if (!ofs) {
		std::cerr << "Falled Open file" << std::endl;
		exit(1);
	}

	ofs << logString << std::endl;

	ofs.close();
}