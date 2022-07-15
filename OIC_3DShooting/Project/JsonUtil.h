#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
//Tips:ここでusing namespace std;をするのはよくない。
//単なるbyteとstd::byteなのかが区別つかなくなってコンパイルエラーになる

//JSON読込ユーティリティ
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

