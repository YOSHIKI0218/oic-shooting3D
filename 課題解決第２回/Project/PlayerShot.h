#pragma once

#include "GameDefine.h"

// ˆÚ“®‘¬“x
#define PLAYERSHOT_SPEED	(0.4f)

class CPlayerShot{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	bool				m_bShow;
public:
	CPlayerShot();
	~CPlayerShot();
	void Initialize();
	void Fire(const Vector3& p);
	void Update();
	void Render();
	void RenderDebug();
	void SetMesh(CMeshContainer *pm){ m_pMesh = pm; }
	bool GetShow(){ return m_bShow; }
	void SetShow(bool bs){ m_bShow = bs; }
	CSphere GetSphere(){ return CSphere(m_Pos,0.2f); }
	const Vector3& GetPosition(){ return m_Pos; }
};