#include "TitleScene.h"
#include "GameDefine.h"

extern int		gChangeSceneNo;

CMeshContainer gMesh;
CMatrix44		matWorld;


CTitleScene::CTitleScene():
	m_Pos(0,0,0),
	m_Rot(0,0,0)
{
}
CTitleScene::~CTitleScene()
{
}
void CTitleScene::Update()
{
	//Xを±90度にするとジンバルロック(gimbal lock)を起こす。YとZを回しても本来のY軸しか回転しない。Z→X→Yの順に回転させていることが原因の一つ。X軸を90度にするとY軸がZ軸と重なる
	m_Rot.x = MOF_ToRadian(90);
	m_Rot.y += MOF_ToRadian(50) * CUtilities::GetFrameSecond();
	m_Rot.z += MOF_ToRadian(25) * CUtilities::GetFrameSecond();
	CMatrix44 matScale;

	matScale.Scaling(1.0f);

	
	matWorld.RotationZXY(m_Rot);
	matWorld.SetTranslation(m_Pos);
	matWorld *= matScale;
	if (g_pInput->IsKeyPush(MOFKEY_RETURN)) 
		gChangeSceneNo = SCENENO_GAME;
}

void CTitleScene::Initalize()
{
	Reset();
	//Create a texture
	int width = g_pGraphics->GetTargetWidth();
	int height = g_pGraphics->GetTargetHeight();
}

void CTitleScene::Render()
{
	
	g_pGraphics->SetDepthEnable(TRUE);
	gMesh.Render(matWorld);
	g_pGraphics->SetDepthEnable(FALSE);
	CGraphicsUtilities::RenderString(g_pGraphics->GetTargetWidth() * 0.5f - 100, 400, MOF_COLOR_WHITE, "Press Enter to start");
}

void CTitleScene::Load()
{
	
	gMesh.Load("enemy.mom");
}

void CTitleScene::Release()
{
	gMesh.Release();
}

void CTitleScene::Reset()
{
	m_Rot.y = 0;
	m_Pos.z = 0;
	LPCamera pCam = CGraphicsUtilities::GetCamera();
	if (!pCam)
		return;

	pCam->LookAt(CVector3(0, 2, -3), m_Pos + UP_VECTOR3, CVector3(0, 1, 0));
	pCam->PerspectiveFov(MOF_ToRadian(40.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	pCam->Update();
}
