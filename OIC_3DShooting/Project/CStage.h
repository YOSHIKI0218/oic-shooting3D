#pragma once
#include "CEnemy.h"
#include	"StageBase.h"
#include "Boss.h"


class CStage : public CStageBase
{
private:
	CBoss				m_Boss;

	void CheckEnemyDestroyed(CEnemyShot* shot, int smax);

public:
	CStage();
	~CStage();
	//基底クラスの関数オーバーライド
	void Load() override;
	void Initialize(EnemyStart *pSt) override;
	void Update(int ecnt, int smax) override;
	void Render() override;
	void RenderDebug2D() override;
	void Release() override;
	void InitializeBG() override;
	void RenderDebug3D() override;
	
};

