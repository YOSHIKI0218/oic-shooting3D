#include "Background.h"

float gBgScrollSpeed;

void CBackground::Load()
{
	m_BG.CreateSprite("Textures/Misc/Road.png");
}

void CBackground::Initalize()
{
	m_BG.m_Angle.x = MOF_MATH_HALFPI;
	m_BG.m_Angle.y = MOF_MATH_HALFPI;
	m_BG.m_Scale = Vector3(200, 200, 200);
	m_BG.m_Position.y = -15;
	m_BG.SetImageRect(0, 0,
		m_BG.GetTexture()->GetWidth() * 4,
		m_BG.GetTexture()->GetHeight() * 4
	);
	//«Visual Studio‚ÌƒoƒO‚ÅÔ”gü‚É‚È‚é‚ª‹C‚É‚µ‚È‚¢‚æ‚¤‚É
	gBgScrollSpeed = CGameDefine::GetBackgroundScrollSpeed();
	m_fScroll = 0.0f;
}

void CBackground::Render()
{
	for (int i = 0; i < 2; i++) {
		m_BG.m_Position.z = 50.0f + fmodf(-m_fScroll, m_BG.m_Scale.z) + i * m_BG.m_Scale.z;
		m_BG.Update();
		m_BG.Render();
	}
}

void CBackground::Release()
{
	m_BG.Release();
}

void CBackground::Update()
{
	m_fScroll += gBgScrollSpeed;
}
