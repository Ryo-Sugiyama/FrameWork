#include "OLogFile.h"
#include <cstdlib>
#include <iostream>

void COLogFile::SetUp() {
	//Log.txt�t�@�C�����������݃��[�h�ŌĂяo���B
	//ofs.close();���łɂ������ꍇ�͔j�����ĐV�K�쐬
	ofs.open("Log.txt", std::ios::out);
	ofs.close();
}



void COLogFile::Log(const char *logString) {

	//�ǉ��������݃��[�h
	ofs.open("Log.txt", std::ios::app);
	
	if (!ofs) {
		std::cerr << "Falled Open file" << std::endl;
		exit(1);
	}

	ofs << logString << std::endl;

	ofs.close();
}