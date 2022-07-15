#include "GameClearScene.h"
#include "GameDefine.h"
#include "Score.h"
//�O���Q��(���̂�GameApp.cpp)
extern int				gChangeSceneNo;

CGameClearScene::CGameClearScene()
{
}

CGameClearScene::~CGameClearScene()
{
}

void CGameClearScene::Load()
{
	m_GameClearTexture.Load("Textures/UI/Clear.png");
}

void CGameClearScene::Initalize()
{
}

void CGameClearScene::Update()
{
	if (g_pInput->IsKeyPush(MOFKEY_RETURN))
	{
		gChangeSceneNo = SCENENO_TITLE;
	}
}

void CGameClearScene::Render()
{
	g_pGraphics->SetDepthEnable(FALSE);
	m_GameClearTexture.Render(0, 0);
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_WHITE, "Press Enter Key");
	CGraphicsUtilities::RenderString(600, 10, MOF_COLOR_WHITE, "�X�R�A: %d �n�C�X�R�A: %d", CScore::GetScore(), CScore::GetHighScore());
}

void CGameClearScene::Release()
{
	m_GameClearTexture.Release();
}

void CGameClearScene::Reset()
{
}
