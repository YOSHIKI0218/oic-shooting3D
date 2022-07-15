#include "GameoverScene.h"
#include "GameDefine.h"
#include "Score.h"
//�O���Q��(���̂�GameApp.cpp)
extern int				gChangeSceneNo;

CGameoverScene::CGameoverScene():
	m_GameOverTexture()
{
}

CGameoverScene::~CGameoverScene()
{
}

void CGameoverScene::Load()
{
	m_GameOverTexture.Load("Textures/UI/GameOver.png");
}

void CGameoverScene::Initalize()
{
}

void CGameoverScene::Update()
{
	if (g_pInput->IsKeyPush(MOFKEY_RETURN))
	{
		gChangeSceneNo = SCENENO_TITLE;
	}
}

void CGameoverScene::Render()
{
	g_pGraphics->SetDepthEnable(FALSE);
	m_GameOverTexture.Render(0, 0);
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_WHITE, "Press Enter Key");
	CGraphicsUtilities::RenderString(600, 10, MOF_COLOR_WHITE, "�X�R�A: %d �n�C�X�R�A: %d", CScore::GetScore(), CScore::GetHighScore());
}

void CGameoverScene::Release()
{
	m_GameOverTexture.Release();
}

void CGameoverScene::Reset()
{
}
