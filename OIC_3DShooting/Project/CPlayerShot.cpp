#include "CPlayerShot.h"

CPlayerShot::CPlayerShot():
m_pMesh(NULL),
m_vPos(0.0f,0.0f,0.0f),
m_bShow(false)
{}

CPlayerShot::~CPlayerShot() {}

void CPlayerShot::Initialize(){
	m_vPos = Vector3(0.0f, 0.0f, 0.0f);
	m_bShow = false;
}

void CPlayerShot::Update() {
	if (!m_bShow) { return; }
	// å©Ç¶ÇΩÇÁÅH
	m_vPos.z += PLAYER_SHOT_SPEED;
	if (CGameDefine::GetFiledHalfZ() < m_vPos.z) {
		m_bShow = false;
	}
}

void CPlayerShot::Render() {
	if (!m_bShow) { return; }
	// å©Ç¶ÇΩÇÁÅH
	CMatrix44 wMat;
	wMat.Translation(m_vPos);
	m_pMesh->Render(wMat);
}

void CPlayerShot::RenderDebug2D()
{
	if (!m_bShow)
		return;
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));
}


void CPlayerShot::Fire(const Vector3& p) 
{
	m_vPos = p;
	m_bShow = true;
}

void CPlayerShot::SetMesh(CMeshContainer* pm) {
	m_pMesh = pm;
}

bool CPlayerShot::GetShow() {
	return m_bShow;
}
