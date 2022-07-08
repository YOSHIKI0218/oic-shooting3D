#pragma once

#include "GameDefine.h"
#include "Enemy.h"

#define BOSS_PARTS_MAX (3)

class CBoss{
private:
	CMeshContainer		m_Mesh;
	CMeshContainer		m_PartsMesh;
	Vector3				m_Pos;
	Vector3				m_Rot;
	CMatrix44			m_matWorld;
	bool				m_bShow;

	int					m_HP;
	int					m_ShotWait;
	Vector3				m_TargetPos;	
	float				m_AnimTime;

	CEnemy				m_Parts[BOSS_PARTS_MAX];

public:
	CBoss();
	~CBoss();
	bool Load();
	void Initialize();
	void Start();
	void Update(CEnemyShot* shot,int smax);
	void Damage(int dmg);
	void Render();
	void RenderDebug();
	bool GetShow(){ return m_bShow; }
	CSphere GetSphere(){ return CSphere(m_Pos,3.1f); }
	void SetTargetPos(const Vector3& t){ m_TargetPos = t; }
	CEnemy& GetParts(int i){ return m_Parts[i]; }
	int GetPartsCount();
	void Release();
};