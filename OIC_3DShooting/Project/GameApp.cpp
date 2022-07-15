/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#pragma once
#include	"GameApp.h"
#include	"TitleScene.h"
#include	"GameScene.h"
#include	"GameoverScene.h"
#include	"GameDefine.h"
#include	"GameClearScene.h"
#include	"Score.h"
#include	"SoundManager.h"


//シーン変更番号 外部参照(実体はGameApp.cpp)
int				gChangeSceneNo;

//現在のシーン
int						gSceneNo = SCENENO_TITLE;

bool					gbShowDebugLog = false;
bool					gbFadeing = false;
bool					gbFadeIn = true;
int						gAlpha = 0;
int						gRunningSceneNo = SCENENO_TITLE;

//シーンクラス
CSceneBase* gptitle;
CSceneBase*	gpMainGame;
CSceneBase* gpGameOver;
CSceneBase* gpGameClear;

// カメラ
CCamera				gCamera;

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");
	CUtilities::SetFPS(60);
	HWND hWnd;
	//GetForegroundWindowだと起動後に、ほかのウィンドウが前にあると、それの名前が変わってしまう
	//そのため、FindWindowで自身のウィンドウを探す。
	hWnd = FindWindowA(NULL,"MofWindow");//Mofライブラリのデフォルトのウィンドウ名
	//hWnd = GetForegroundWindow();
	SetWindowTextA(hWnd, "OiC 3D Shooting");

	CGameDefine::LoadParameter();

	// カメラ初期化
	gCamera.SetViewPort();
	//                    カメラの位置、　ターゲットの位置　、上
	gCamera.LookAt(Vector3(0, 6.0f, -17.0f), Vector3(0, 0, -10), UP_VECTOR3);
	//						視野角(ラジアン)		アスペクト比	  ニアクリップ(描画の最小範囲)  ファークリップ(描画の最大範囲)
	gCamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(&gCamera);

	//シーンのインスタンス化
	gptitle = new CTitleScene();
	gpMainGame = new CGameScene();
	gpGameOver = new CGameoverScene();
	gpGameClear = new CGameClearScene();

//シーンの初期化と読み込み
	gptitle->Load();
	gptitle->Initalize();

	gpMainGame->Load();
	gpMainGame->Initalize();

	gpGameOver->Load();
	gpGameOver->Initalize();

	gpGameClear->Load();
	gpGameClear->Initalize();
	CScore::LoadSavedScore();
	CSoundManager::Instance().PlayMusic("GB-Shooter-A05-2", "mp3");
	return TRUE;
}

/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();
	//シーンの更新
	switch (gRunningSceneNo)
	{
	case SCENENO_TITLE:
		gptitle->Update();
		break;
	case SCENENO_GAME:
		gpMainGame->Update();
		break;
	case SCENENO_GAMEOVER:
		gpGameOver->Update();
		break;
	case SCENENO_GAMECLEAR:
		gpGameClear->Update();
		break;
	default:
		break;
	}

	//シーン番号が変更されていればシーンのリセット
	if (gChangeSceneNo != gSceneNo && !gbFadeing)
	{
		gbFadeIn = true;
		gbFadeing = true;
	}

	if (gbFadeing) {
		gAlpha+= gbFadeIn ? 10 : -10;
		gAlpha = MOF_CLIPING(gAlpha, 0, 255);

		if (gAlpha >= 255) {
			TransitionSecene();
			gbFadeIn = false;
		}
		else if (gAlpha <= 0) {
			gbFadeing = false;
		}

	}

	CSoundManager::Instance().Update();
	
	return TRUE;
}

/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画処理
	g_pGraphics->RenderStart();
	// 画面のクリア
	g_pGraphics->ClearTarget(0.0f,0.4f, 0.2f ,0.0f,1.0f,0);
	switch (gRunningSceneNo)
	{
	case SCENENO_TITLE:
		gptitle->Render();
		break;
	case SCENENO_GAME:
		gpMainGame->Render();
		break;
	case SCENENO_GAMEOVER:
		gpGameOver->Render();
		break;
	case SCENENO_GAMECLEAR:
		gpGameClear->Render();
		break;
	default:
		break;
	}

	if (gbFadeing) {
		CGraphicsUtilities::RenderFillRect(0, 0, g_pGraphics->GetTargetWidth(), g_pGraphics->GetTargetHeight(), MOF_ARGB(gAlpha, 0, 0, 0));
	}

	// 描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void) {
	CScore::Save();
	CSoundManager::Instance().Release();
	gptitle->Release();
	gpMainGame->Release();
	gpGameOver->Release();
	gpGameClear->Release();
	
	//newの後は必ずdeleteする!!C#やJavaみたいにGCで自動的に解放されることはない
	delete gptitle;
	delete gpMainGame;
	delete gpGameOver;
	delete gpGameClear;
	
	return TRUE;
}

void CGameApp::TransitionSecene()
{
	gRunningSceneNo = gChangeSceneNo;
	switch (gChangeSceneNo)
	{
	case SCENENO_TITLE:
		CScore::Save();
		gptitle->Reset();
		break;
	case SCENENO_GAME:
		gpMainGame->Reset();
		CScore::LoadSavedScore();
		CScore::Reset();
		break;
	case SCENENO_GAMEOVER:
		CScore::Save();
		gpGameOver->Reset();
		break;
	case SCENENO_GAMECLEAR:
		CScore::Save();
		gpGameClear->Reset();
		break;
	default:
		MOF_PRINTLOG("Scene Number %d not found\n", gChangeSceneNo);
		break;
	}

	gSceneNo = gChangeSceneNo;
}


