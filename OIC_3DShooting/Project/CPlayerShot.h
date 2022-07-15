#pragma once

#include "GameDefine.h"

class CPlayerShot
{
private:
	CMeshContainer*	m_pMesh;
	CVector3		m_vPos;
	bool			m_bShow;
public:
	CPlayerShot();
	~CPlayerShot();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug2D();

	void Fire(const Vector3& p);
	void SetMesh(CMeshContainer* pm);
	bool GetShow();
	void SetShow(bool b) { m_bShow = b; }
	Vector3 GetPosition(){ return m_vPos; }
	CSphere GetSphere() { return CSphere(m_vPos, 0.2f); }
};

