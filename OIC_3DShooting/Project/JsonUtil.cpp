//�쐬:2020.11 �ҏW:2021.06

#pragma once
#include "JsonUtil.h"
#include "library/json.hpp"
#include <WinSock2.h>
#include <windows.h>
#include<iostream>
#include<Shlwapi.h>
#include "Mof.h"

using json = nlohmann::json;
using namespace std;

CJsonUtil::CJsonUtil()
{
}

CJsonUtil::~CJsonUtil()
{
}

string CJsonUtil::FindTextFromJson(char * FileName, char * KeyName)
{
	if (!PathFileExists(FileName))
	{
		MOF_PRINTLOG("FindFloatFromJson Error: �t�@�C����������܂���B\n");
		return string("ERROR");
	}
	ifstream ifs(FileName);
	string str = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
	json js;//JSON�C���X�^���X
	try {
		js = json::parse(str);
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindTextFromJson Error: JSON�̃p�[�X�Ɏ��s���܂����B\n");
		return string("ERROR");
	}
	string tmp;
	try {
		tmp = js[string(KeyName)];
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindTextFromJson Error: JSON�̃p�[�X�Ɏ��s���܂����B\n");
		tmp = string("ERROR");
	}
	return tmp;
}

float CJsonUtil::FindFloatFromJson(char * FileName, char * KeyName)
{
	if (!PathFileExists(FileName))
	{
		MOF_PRINTLOG("FindFloatFromJson Error: �t�@�C����������܂���B\n");
		return MOF_FLOAT_QNAN;
	}
	ifstream ifs(FileName);
	string str = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
	json js;//�p�[�X
	try {
		js = json::parse(str);
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindFloatFromJson Error: JSON�̃p�[�X�Ɏ��s���܂����B\n");
		return MOF_FLOAT_QNAN;
	}
	float tmp;
	try {
		tmp = js[string(KeyName)];
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindFloatFromJson Error: JSON�̃p�[�X�Ɏ��s���܂����B\n");
		tmp = MOF_FLOAT_QNAN;
	}
	return tmp;
}

int CJsonUtil::FindIntFromJson(char * FileName, char * KeyName)
{
	if (!PathFileExists(FileName))
	{
		MOF_PRINTLOG("FindIntFromJson Error: �t�@�C����������܂���B\n");
		return -1;
	}
	ifstream ifs(FileName);
	string str = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
	json js;//�p�[�X
	try {
		js = json::parse(str);
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindIntFromJson Error: JSON�̃p�[�X�Ɏ��s���܂����B\n");
		return -1;
	}
	int tmp;
	try {
		tmp = js[string(KeyName)];
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindIntFromJson Error: JSON�̃p�[�X�Ɏ��s���܂����B\n");
		tmp = -1;
	}
	return tmp;
}

bool CJsonUtil::FindBoolFromJson(char * FileName, char * KeyName)
{
	if (!PathFileExists(FileName))
	{
		MOF_PRINTLOG("FindBoolFromJson Error: �t�@�C����������܂���B\n");
		return false;
	}
	ifstream ifs(FileName);
	string str = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
	json js;//�p�[�X
	try {
		js = json::parse(str);
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindBoolFromJson Error: JSON�̃p�[�X�Ɏ��s���܂����B\n");
		return false;
	}
	bool tmp;
	try {
		tmp = js[string(KeyName)];
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindBoolFromJson Error: JSON�̃p�[�X�Ɏ��s���܂����B\n");
		return false;
	}
	return tmp;
}

std::string CJsonUtil::GetFileText(char * FileName)
{
	ifstream ifs(FileName);
	return string(istreambuf_iterator<char>(ifs),
		istreambuf_iterator<char>());
}