#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
//Tips:������using namespace std;������̂͂悭�Ȃ��B
//�P�Ȃ�byte��std::byte�Ȃ̂�����ʂ��Ȃ��Ȃ��ăR���p�C���G���[�ɂȂ�

//JSON�Ǎ����[�e�B���e�B
class CJsonUtil
{
public:
	CJsonUtil();
	~CJsonUtil();
	static std::string FindTextFromJson(char* FileName, char *KeyName);
	static float FindFloatFromJson(char* FileName, char *KeyName);
	static int FindIntFromJson(char* FileName, char *KeyName);
	static bool FindBoolFromJson(char* FileName, char *KeyName);
	static std::string GetFileText(char* FileName);


};

