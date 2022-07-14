#pragma once

#include "GameDefine.h"
#include "EnemyShot.h"
#include "EffectManager.h"

class CEnemy{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	Vector3				m_Rot;
	bool				m_bShow;

	int					m_HP;

	int					m_ShotWait;
	int					m_ShotWaitSet;

	Vector3				m_TargetPos;

	int					m_Type;
	float				m_AnimTime;

public:
	CEnemy();
	~CEnemy();
	void Initialize();
	void Start(const Vector3& p,int t = 0);
	void Update(CEnemyShot* shot,int smax);
	void UpdateType0(CEnemyShot* shot,int smax);
	void UpdateBossParts(CEnemyShot* shot,int smax);
	void Damage(int dmg);
	void Render();
	void RenderDebug();
	void RenderDebugText(int i);
	void SetMesh(CMeshContainer* pm){ m_pMesh = pm; }
	void SetPosition(const Vector3& p){ m_Pos = p; }
	void SetRotation(const Vector3& r){ m_Rot = r; }
	void SetTargetPos(const Vector3& t){ m_TargetPos = t; }
	bool GetShow(){ return m_bShow; }
	CSphere GetSphere(){
		switch(m_Type)
		{
		case 1:
		case 2:
		case 3:
			return CSphere(m_Pos,1.5f);
		}
		return CSphere(m_Pos,0.5f);
	}
};