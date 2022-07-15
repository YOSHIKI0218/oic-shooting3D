#include "Stage3.h"
#include "Background.h"
#include	"Player.h"
#include "GameDefine.h"
#include	"GameUI.h"

//�O���Q��
extern int			gStageNo;
extern CPlayer		gPlayer;
extern CEnemy		gEnemyArray[];
extern CGameUI		gUI;
extern CEnemyShot			gShotArray[];

void CStage3::CheckEnemyDestroyed(CEnemyShot* shot, int smax)
{
	//�N���A���Ă��Ȃ��Ƃ��ɓG���S�ł��A�{�X����\���ł���΃{�X1���o��
	if (!m_bEnemyDestroyed && IsAllEnemy() && !gPlayer.IsDead())
	{

		//�G���̑S�ł̊m�F
		bool btmp = true;
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			if (gEnemyArray[i].GetShow())
			{
				btmp = false;
			}
		}

		m_bEnemyDestroyed = btmp;

		if (!m_Boss.GetShow() && !m_Boss.IsDestroyedAndRemoved())
		{
			gUI.SetShowWarning(true);
			m_Boss.Start();
			m_Boss.SetTargetPos(gPlayer.GetPosition());
			m_Boss.Update(gShotArray, smax);
		}
		if (m_Boss.IsDestroyedAndRemoved())
		{
			gStageNo++;
			m_bEnemyDestroyed = false;
		}

	}
}

void CStage3::Load()
{
	m_EnemyMesh.Load("enemy1.mom");
	m_Boss.Load("oic_logo_boss.mom");
}

void CStage3::Initialize(EnemyStart* pSt)
{
	InitializeBG();
	m_bEnemyDestroyed = false;
	gUI.SetBoss2Ptr(&m_Boss);
	m_Boss.Initialize();
	m_EnemyNo = 0;
	m_pEnemyStart = pSt;
}

void CStage3::Update(int ecnt, int smax)
{
	if ((m_EnemyNo < m_pEnemyStart->Count) && (CBackground::Instance().GetScroll() >= (m_pEnemyStart->Scroll[m_EnemyNo])))
	{
		for (int i = 0; i < ecnt; i++)
		{
			if (gEnemyArray[i].GetShow()) { continue; }
			gEnemyArray[i].SetMesh(&m_EnemyMesh);
			gEnemyArray[i].Start(Vector3(m_pEnemyStart->PosX[m_EnemyNo], 0, 0), m_pEnemyStart->PosX[m_EnemyNo] > 0, 0);
			break;
		}
		m_EnemyNo++;
	}


	m_Boss.SetTargetPos(gPlayer.GetPosition());
	m_Boss.Update(gShotArray, smax);

	gPlayer.CollisionBoss1(m_Boss);

	//�G���S�Ŋm�F
	CheckEnemyDestroyed(gShotArray, smax);

	if (m_Boss.IsDestroyedAndRemoved())
	{
		CBackground::Instance().SetScroll(0.0f);
		gStageNo++;
	}
	gUI.UpdateBoss2Slider();
}

void CStage3::Render()
{
	m_Boss.Render();
}

void CStage3::RenderDebug2D()
{
}

void CStage3::Release()
{
	m_EnemyMesh.Release();
	m_Boss.Release();
}

void CStage3::InitializeBG()
{
}

void CStage3::RenderDebug3D()
{
}
