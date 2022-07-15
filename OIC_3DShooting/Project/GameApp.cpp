/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
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


//�V�[���ύX�ԍ� �O���Q��(���̂�GameApp.cpp)
int				gChangeSceneNo;

//���݂̃V�[��
int						gSceneNo = SCENENO_TITLE;

bool					gbShowDebugLog = false;
bool					gbFadeing = false;
bool					gbFadeIn = true;
int						gAlpha = 0;
int						gRunningSceneNo = SCENENO_TITLE;

//�V�[���N���X
CSceneBase* gptitle;
CSceneBase*	gpMainGame;
CSceneBase* gpGameOver;
CSceneBase* gpGameClear;

// �J����
CCamera				gCamera;

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");
	CUtilities::SetFPS(60);
	HWND hWnd;
	//GetForegroundWindow���ƋN����ɁA�ق��̃E�B���h�E���O�ɂ���ƁA����̖��O���ς���Ă��܂�
	//���̂��߁AFindWindow�Ŏ��g�̃E�B���h�E��T���B
	hWnd = FindWindowA(NULL,"MofWindow");//Mof���C�u�����̃f�t�H���g�̃E�B���h�E��
	//hWnd = GetForegroundWindow();
	SetWindowTextA(hWnd, "OiC 3D Shooting");

	CGameDefine::LoadParameter();

	// �J����������
	gCamera.SetViewPort();
	//                    �J�����̈ʒu�A�@�^�[�Q�b�g�̈ʒu�@�A��
	gCamera.LookAt(Vector3(0, 6.0f, -17.0f), Vector3(0, 0, -10), UP_VECTOR3);
	//						����p(���W�A��)		�A�X�y�N�g��	  �j�A�N���b�v(�`��̍ŏ��͈�)  �t�@�[�N���b�v(�`��̍ő�͈�)
	gCamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(&gCamera);

	//�V�[���̃C���X�^���X��
	gptitle = new CTitleScene();
	gpMainGame = new CGameScene();
	gpGameOver = new CGameoverScene();
	gpGameClear = new CGameClearScene();

//�V�[���̏������Ɠǂݍ���
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
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();
	//�V�[���̍X�V
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

	//�V�[���ԍ����ύX����Ă���΃V�[���̃��Z�b�g
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
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`�揈��
	g_pGraphics->RenderStart();
	// ��ʂ̃N���A
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

	// �`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void) {
	CScore::Save();
	CSoundManager::Instance().Release();
	gptitle->Release();
	gpMainGame->Release();
	gpGameOver->Release();
	gpGameClear->Release();
	
	//new�̌�͕K��delete����!!C#��Java�݂�����GC�Ŏ����I�ɉ������邱�Ƃ͂Ȃ�
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


