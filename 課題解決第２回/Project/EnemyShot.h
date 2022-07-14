#pragma once

#include "GameDefine.h"

class CEnemyShot{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	Vector3				m_Spd;
	bool				m_bShow;
	float				m_RotY;
public:
	CEnemyShot();
	~CEnemyShot();
	void Initialize();
	void Fire(const Vector3& p,const Vector3& s);
	void Update();
	void Render();
	void RenderDebug();
	void SetMesh(CMeshContainer* pm){ m_pMesh = pm; }
	bool GetShow(){ return m_bShow; }
	void SetShow(bool bs){ m_bShow = bs; }
	CSphere GetSphere(){ return CSphere(m_Pos,0.2f); }
	static CEnemyShot* FindAvailableShot(CEnemyShot* shot,int smax);
};