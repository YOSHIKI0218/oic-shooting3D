#pragma once
#include "Mof.h"
#include	"UISlider.h"
#include "Player.h"
#include "Boss1.h"
class CGameUI
{
public:
	CGameUI();
	~CGameUI();


	void Render();
	void Load();
	void UpdatePlayerHpSlider();
	void Release();
	void Initalize();
	void SetShowWarning(bool b);
	void SetShowNextStage(bool b);
	void SetPlayerPtr(CPlayer* p) { m_pPlayer = p; }
	void SetBoss1Ptr(CBoss1* b) { m_pBoss1 = b; }
	void SetBoss2Ptr(CBoss1* b) { m_pBoss2 = b; }
	void UpdateBoss1Slider();
	void UpdateBoss2Slider();
private:
	CVector4		m_Color;
	CTexture		m_WarningUI;
	CTexture		m_NextStageUI;
	CTexture		m_NormalShotUI;
	CTexture		m_LineShotUI;
	bool			m_bWarning;
	bool			m_bShowNextStageUI;//NEXT STAGEのテクスチャ描画フラグ
	bool			m_bNextStage;
	float			m_TimeElapsed;
	CPlayer*		m_pPlayer;
	CBoss1*			m_pBoss1;
	CBoss1*			m_pBoss2;

	int				m_NextStageUITime;//NEXT STAGEのタイマー
	int				m_WarnTime;
	int				m_PrevPlayerHP;
	int				m_PrevBoss1HP;
	int				m_PrevBoss2HP;
};

