#include "GameUI.h"
#include "UISlider.h"
#include "Score.h"
#include "Player.h"

CUISlider gHPSlider;
CUISlider gBoss1Slider;
CUISlider gBoss2Slider;
CTexture gGameoverTexture;
//ステージ番号(実体はGameScene.cpp)
extern int						gStageNo;

CGameUI::CGameUI():
	m_WarningUI(),
	m_bWarning(false),
	m_TimeElapsed(0.0f),
	m_WarnTime(0),
	m_pPlayer(NULL),
	m_pBoss1(NULL),
	m_PrevPlayerHP(0),
	m_PrevBoss1HP(0),
	m_NextStageUI(),
	m_NextStageUITime(0),
	m_PrevBoss2HP(0),
	m_bNextStage(false),
	m_bShowNextStageUI(false),
	m_pBoss2(NULL)
{
}

CGameUI::~CGameUI()
{
}



void CGameUI::Render()
{
	gHPSlider.Render();
	if (m_pBoss1->GetShow())
	{
		gBoss1Slider.Render();
	}

	if (m_pBoss2->GetShow())
	{
		gBoss2Slider.Render();
	}
	//ウィンドウの幅、高さをキャッシュする
	int width = g_pGraphics->GetTargetWidth();
	int height = g_pGraphics->GetTargetHeight();
	//ショット種類の描画
	if (gStageNo >= 2) 
	{
		auto shot = m_pPlayer->GetBulletMode();
		CGraphicsUtilities::RenderString(width - 192, height - 158, MOF_COLOR_WHITE, "1");
		m_NormalShotUI.Render(width - 256, height - 128, shot == Mode_Laser ? MOF_ARGB(90,255,255,255) : MOF_ARGB(192, 255, 255, 255));

		CGraphicsUtilities::RenderString(width - 64, height - 158, MOF_COLOR_WHITE, "2");
		m_LineShotUI.Render(width - 128, height - 128, shot == Mode_Normal ? MOF_ARGB(90, 255, 255, 255) : MOF_ARGB(192, 255, 255, 255));
	}

	if (m_bWarning)
	{
		if (m_WarnTime > 120)
		{
			m_bWarning = false;
			m_Color.a = 0;
			
		}
		m_Color.r = 1;
		m_Color.g = 0;
		m_Color.b = 0;
		m_TimeElapsed += CUtilities::GetFrameSecond();
		m_Color.a = sinf(m_TimeElapsed * 6) * 0.25f + 0.25f;
		CGraphicsUtilities::RenderFillRect(0, 0, width, height, m_Color.ToU32Color());
		m_WarningUI.Render(width * 0.5f - m_WarningUI.GetWidth() * 0.5f, 100, MOF_COLOR_RED);
		m_WarnTime++;
	}
	else if(m_pPlayer->GetHP() < 10){
		m_Color.r = 1;
		m_Color.g = 0.25f;
		m_Color.b = 0;
		m_TimeElapsed += CUtilities::GetFrameSecond();
		m_Color.a = (sinf(m_TimeElapsed * 6) * 0.25f + 0.25f) * 0.25f;
		CGraphicsUtilities::RenderFillRect(0, 0, width, height, m_Color.ToU32Color());
	}
	if (m_bNextStage)
	{
		if (m_NextStageUITime > 180)
		{
			m_NextStageUITime = 0;
			m_bNextStage = false;
		}
		if (m_NextStageUITime % 30 == 0)
			m_bShowNextStageUI = !m_bShowNextStageUI;

		if(m_bShowNextStageUI)
		m_NextStageUI.Render(width * 0.5f - m_WarningUI.GetWidth() * 0.5f, 100, MOF_COLOR_WHITE);
		m_NextStageUITime++;
	}
	CGraphicsUtilities::RenderString(600, 10, MOF_COLOR_WHITE, "スコア: %d ハイスコア: %d", CScore::GetScore(), CScore::GetHighScore());

}

void CGameUI::Load()
{
	m_NextStageUI.Load("Textures/UI/ui_text_nextstage.dds");
	m_WarningUI.Load("Textures/UI/ui_text_warning.dds");
	gGameoverTexture.Load("Textures/UI/game_over.dds");
	m_NormalShotUI.Load("Textures/UI/ui_panel_normal.dds");
	m_LineShotUI.Load("Textures/UI/ui_panel_line.dds");
}

void CGameUI::UpdatePlayerHpSlider()
{
	//プレイヤーHP取得
	int hp = m_pPlayer->GetHP();
	//HPが直前の値と異なる場合のみ、スライダーの値をセットする
	if (hp != m_PrevPlayerHP)
		gHPSlider.SetValue(hp);
	m_PrevPlayerHP = hp;
}

void CGameUI::Release()
{
	m_NextStageUI.Release();
	m_WarningUI.Release();
	gGameoverTexture.Release();
	m_NormalShotUI.Release();
	m_LineShotUI.Release();
}

void CGameUI::Initalize()
{
	m_WarnTime = 0;
	int hp = CGameDefine::GetPlayerMaxHP();
	gHPSlider.SetMaxValue(hp);
	gHPSlider.SetValue(hp);
	gHPSlider.SetAutoChangeColorByValue(true);
	gHPSlider.SetTitleText("PLAYER HP");
	gHPSlider.SetShowTitle(true);
	gHPSlider.SetShowValueText(true);
	gHPSlider.SetPosition(Vector2(5, 5));
	gHPSlider.SetHeight(32);
	gHPSlider.SetWidth(512);

	gBoss1Slider.SetMaxValue(m_pBoss1->GetMaxHP());
	gBoss1Slider.SetValue(m_pBoss1->GetMaxHP());
	gBoss1Slider.SetAutoChangeColorByValue(false);
	gBoss1Slider.SetPosition(Vector2(5, 80));
	gBoss1Slider.SetHeight(32);
	gBoss1Slider.SetWidth(512);
	gBoss1Slider.SetShowTitle(true);
	gBoss1Slider.SetShowValueText(true);

	gBoss1Slider.SetTitleText("「除け者にされた雑魚」");
	gBoss1Slider.SetColor(Vector4(1, 0.5f, 0.5f, 0.8f));

	gBoss2Slider.SetMaxValue(m_pBoss2->GetMaxHP());
	gBoss2Slider.SetValue(m_pBoss2->GetMaxHP());
	gBoss2Slider.SetAutoChangeColorByValue(false);
	gBoss2Slider.SetPosition(Vector2(5, 80));
	gBoss2Slider.SetHeight(32);
	gBoss2Slider.SetWidth(512);
	gBoss2Slider.SetShowTitle(true);
	gBoss2Slider.SetShowValueText(true);

	gBoss2Slider.SetTitleText("「OiC」");
	gBoss2Slider.SetColor(Vector4(1, 0.5f, 0.5f, 0.8f));
	m_Color.a = 0;

}

void CGameUI::SetShowWarning(bool b)
{
	m_bWarning = b;
	m_WarnTime = 0;
	m_TimeElapsed = 0.0f;
	m_Color.a = 0;
}

void CGameUI::SetShowNextStage(bool b)
{
	m_bNextStage = b;
	if (!b)
	{
		m_bShowNextStageUI = true;
		m_NextStageUITime = 0;
	}
}



void CGameUI::UpdateBoss1Slider()
{
	int hp = m_pBoss1->GetHP();
	if(hp != m_PrevBoss1HP)
		gBoss1Slider.SetValue(hp);
	m_PrevBoss1HP = hp;
}

void CGameUI::UpdateBoss2Slider()
{
	int hp = m_pBoss2->GetHP();
	if (hp != m_PrevBoss2HP)
		gBoss2Slider.SetValue(hp);
	m_PrevBoss2HP = hp;
}

