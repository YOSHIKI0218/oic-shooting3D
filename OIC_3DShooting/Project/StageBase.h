#pragma once
#include "GameDefine.h"
#include "CEnemy.h"

struct EnemyStart
{
	int		Count;
	float* PosX;
	float* Scroll;
};
class CStageBase
{
protected:
	// “G‚ÉŠÖ‚·‚é•Ï”
	EnemyStart* m_pEnemyStart;
	CMeshContainer	m_EnemyMesh;
	int				m_EnemyNo;
	bool			m_bClear;
	bool			m_bEnemyDestroyed;
public:
	virtual void Load() = 0;
	virtual void Initialize(EnemyStart* pSt) = 0;
	virtual void Update(int ecnt, int smax) = 0;
	virtual void Render() = 0;
	virtual void RenderDebug2D() = 0;
	virtual void Release() = 0;
	virtual void RenderDebug3D() = 0;
	virtual void InitializeBG() = 0;

	bool	IsClear() { return m_bClear; }
	bool IsAllEnemy() { return m_EnemyNo >= m_pEnemyStart->Count; }
};

