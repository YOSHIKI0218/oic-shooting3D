//作成:2020.11 編集:2021.06

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
		MOF_PRINTLOG("FindFloatFromJson Error: ファイルが見つかりません。\n");
		return string("ERROR");
	}
	ifstream ifs(FileName);
	string str = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
	json js;//JSONインスタンス
	try {
		js = json::parse(str);
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindTextFromJson Error: JSONのパースに失敗しました。\n");
		return string("ERROR");
	}
	string tmp;
	try {
		tmp = js[string(KeyName)];
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindTextFromJson Error: JSONのパースに失敗しました。\n");
		tmp = string("ERROR");
	}
	return tmp;
}

float CJsonUtil::FindFloatFromJson(char * FileName, char * KeyName)
{
	if (!PathFileExists(FileName))
	{
		MOF_PRINTLOG("FindFloatFromJson Error: ファイルが見つかりません。\n");
		return MOF_FLOAT_QNAN;
	}
	ifstream ifs(FileName);
	string str = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
	json js;//パース
	try {
		js = json::parse(str);
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindFloatFromJson Error: JSONのパースに失敗しました。\n");
		return MOF_FLOAT_QNAN;
	}
	float tmp;
	try {
		tmp = js[string(KeyName)];
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindFloatFromJson Error: JSONのパースに失敗しました。\n");
		tmp = MOF_FLOAT_QNAN;
	}
	return tmp;
}

int CJsonUtil::FindIntFromJson(char * FileName, char * KeyName)
{
	if (!PathFileExists(FileName))
	{
		MOF_PRINTLOG("FindIntFromJson Error: ファイルが見つかりません。\n");
		return -1;
	}
	ifstream ifs(FileName);
	string str = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
	json js;//パース
	try {
		js = json::parse(str);
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindIntFromJson Error: JSONのパースに失敗しました。\n");
		return -1;
	}
	int tmp;
	try {
		tmp = js[string(KeyName)];
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindIntFromJson Error: JSONのパースに失敗しました。\n");
		tmp = -1;
	}
	return tmp;
}

bool CJsonUtil::FindBoolFromJson(char * FileName, char * KeyName)
{
	if (!PathFileExists(FileName))
	{
		MOF_PRINTLOG("FindBoolFromJson Error: ファイルが見つかりません。\n");
		return false;
	}
	ifstream ifs(FileName);
	string str = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
	json js;//パース
	try {
		js = json::parse(str);
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindBoolFromJson Error: JSONのパースに失敗しました。\n");
		return false;
	}
	bool tmp;
	try {
		tmp = js[string(KeyName)];
	}
	catch (exception ex) {
		MOF_PRINTLOG("FindBoolFromJson Error: JSONのパースに失敗しました。\n");
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