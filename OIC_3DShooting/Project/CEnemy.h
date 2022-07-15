#pragma once

#include "GameDefine.h"
#include "EnemyShot.h"
#include	"Particle.h"

class CEnemy
{
private:
	CMeshContainer* m_pMesh;
	CVector3		m_vPos;
	CVector3		m_vRot;
	bool			m_bShow;
	bool			m_bRight;//‰E‘¤‚Ì“G‚©
	float			m_fAnimTime;
	int				m_HP;
	int				m_ShotWait;
	int				m_ShotWaitSet;
	Vector3			m_TargetPos;
	int             m_Type;

public:
	CEnemy();
	~CEnemy();
	void Initialize();
	void Update(CEnemyShot* shot, int smax);
	void Render();
	void RenderDebug3D(int i);
	void UpdateType0(CEnemyShot* shot, int smax);
	void UpdateBossParts(CEnemyShot* shot, int smax);
	void Start(const Vector3& p, bool isRight, int t);
	void SetMesh(CMeshContainer* pm);
	bool GetShow();
	void SetPosition(const Vector3& p) { m_vPos = p; }
	void SetRotation(const Vector3& r) { m_vRot = r; }
	void SetTargetPos(const Vector3& t) { m_TargetPos = t; }

	CSphere GetSphere() {
		switch (m_Type)
		{
		case 1:
		case 2:
		case 3:
			return CSphere(m_vPos, 1.5f);
		}
		return CSphere(m_vPos, 0.5f);
	}

	void Damage(int dmg);

	void Release();
};

