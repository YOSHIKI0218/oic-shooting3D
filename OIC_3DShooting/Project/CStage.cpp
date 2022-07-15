#include "CStage.h"
#include "Background.h"
#include	"Player.h"
#include "GameDefine.h"
#include	"GameUI.h"

//�O���Q��
extern int						gStageNo;
extern CPlayer				gPlayer;
extern CEnemy	gEnemyArray[];
extern	CGameUI gUI;
extern CEnemyShot			gShotArray[];

void CStage::CheckEnemyDestroyed(CEnemyShot* shot, int smax)
{
	//�v���C���[���������Ă���A�G�����S�ł�����{�X�o��
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
		//�G�S�łŃ{�X�o��
		if (m_bEnemyDestroyed)
		{
			gUI.SetShowWarning(true);
			m_Boss.Start();
			m_Boss.SetTargetPos(gPlayer.GetPosition());
			m_Boss.Update(shot, smax);
		}


	}
}

CStage::CStage()
{
}
CStage::~CStage()
{

}

void CStage::Load()
{
	m_EnemyMesh.Load("enemy1.mom");
	m_Boss.Load();
}

void CStage::Initialize(EnemyStart* pSt)
{
	InitializeBG();
	m_bEnemyDestroyed = false;
	m_EnemyNo = 0;
	m_pEnemyStart = pSt;
	m_Boss.Initialize();
}

void CStage::Update(int ecnt, int smax)
{
	m_Boss.SetTargetPos(gPlayer.GetPosition());
	m_Boss.Update(gShotArray, smax);
	
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
	gPlayer.ConllisionBoss(m_Boss);

	//�G���S�Ŋm�F
	CheckEnemyDestroyed(gShotArray, smax);

	//�{�X�����j���ꂽ�玟�X�e�[�W��
	if (m_Boss.IsDestroyedAndRemoved())
	{
		CBackground::Instance().SetScroll(0.0f);
		m_EnemyNo = 0;
		gStageNo++;
		gUI.SetShowNextStage(true);
		for (int i = 0; i < ecnt; i++)
		{
			gEnemyArray[i].Initialize();
		}
	}

}
void CStage::Render()
{
	m_Boss.Render();
}
void CStage::RenderDebug2D()
{
	CGraphicsUtilities::RenderString(10, 10, MOF_XRGB(0, 0, 0),
		"�X�N���[�� �F %.2f, �G�̏o���ԍ��F%d ", CBackground::Instance().GetScroll(),m_EnemyNo);
}
void CStage::Release()
{
	m_EnemyMesh.Release();
	m_Boss.Release();
}

void CStage::InitializeBG()
{

}

void CStage::RenderDebug3D()
{
	m_Boss.RenderDebug();
}
