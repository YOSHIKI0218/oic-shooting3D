#include "Stage2.h"
#include "Background.h"
#include	"Player.h"
#include "GameDefine.h"
#include	"GameUI.h"

//外部参照
extern int			gStageNo;
extern CPlayer		gPlayer;
extern CEnemy		gEnemyArray[];
extern CGameUI		gUI;
extern CEnemyShot			gShotArray[];

void CStage2::CheckEnemyDestroyed(CEnemyShot* shot, int smax)
{
	//クリアしていないときに敵が全滅し、ボスが非表示であればボス1が出現
	if (!m_bEnemyDestroyed && IsAllEnemy() && !gPlayer.IsDead())
	{

		//雑魚の全滅の確認
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

void CStage2::Load()
{
	m_EnemyMesh.Load("enemy1.mom");
	m_Boss.Load("enemy.mom");
	m_Boss.SetMeshScale(3.0f);
}

void CStage2::Initialize(EnemyStart* pSt)
{
	InitializeBG();
	m_bEnemyDestroyed = false;
	gUI.SetBoss1Ptr(&m_Boss);
	m_Boss.Initialize();
	m_EnemyNo = 0;
	m_pEnemyStart = pSt;
}

void CStage2::Update(int ecnt, int smax)
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

	//雑魚全滅確認
	CheckEnemyDestroyed(gShotArray, smax);

	if (m_Boss.IsDestroyedAndRemoved())
	{
		gUI.SetShowNextStage(true);
		for (int i = 0; i < ecnt; i++)
		{
			gEnemyArray[i].Initialize();
		}
		CBackground::Instance().SetScroll(0.0f);
		gStageNo++;
	}
}

void CStage2::Render()
{
	m_Boss.Render();
}

void CStage2::RenderDebug2D()
{
}

void CStage2::Release()
{
	m_EnemyMesh.Release();
	m_Boss.Release();
}

void CStage2::InitializeBG()
{
}

void CStage2::RenderDebug3D()
{
}
