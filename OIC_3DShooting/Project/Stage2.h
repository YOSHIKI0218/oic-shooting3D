#pragma once
#include "StageBase.h"
#include "CEnemy.h"
#include "Boss1.h"

class CStage2 :
    public CStageBase
{
private:
	CBoss1				m_Boss;

	void CheckEnemyDestroyed(CEnemyShot* shot, int smax);
public:
	//基底クラスの関数オーバーライド
	void Load() override;
	void Initialize(EnemyStart* pSt) override;
	void Update(int ecnt, int smax) override;
	void Render() override;
	void RenderDebug2D() override;
	void Release() override;
	void InitializeBG() override;
	void RenderDebug3D() override;
};

