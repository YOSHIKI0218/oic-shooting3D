#include "Particle.h"
#include "GameDefine.h"
CParticle::CParticle() :
	m_Time(0.0f),
	m_State(),
	m_InitalizeState(),
	m_UpdateBehavior(),
	m_ParameterBehavior(),
	m_EnviromentBehavior(),
	m_pTexture(NULL),
	m_pSound(NULL),
	m_Started(false)
{
}

CParticle::~CParticle()
{
	//Release();
}

void CParticle::Initalize(CREATEINFO& info)
{
	m_Time = -1;
	m_InitalizeState = m_State = info.State;
	m_UpdateBehavior = info.UpdateBehavior;
	m_ParameterBehavior = info.ParameterBehavior;
	m_EnviromentBehavior = info.EnviromentBehavior;
	m_pTexture = info.pTexture;
	m_Motion.Create(info.ParameterBehavior.Motion);
	m_pSound = info.pBuffer;
	m_ID = info.ID;
	m_Started = false;
}

void CParticle::Update()
{
	
	if (IsEnd())
		return;
	m_Time++;
	if (!m_Started) {
		if (m_pSound)
		{
			m_pSound->Play();
			//MOF_PRINTLOG("Play Sound!! \n");
		}
		m_Started = true;
	}
	m_State.Position += m_UpdateBehavior.Move + m_EnviromentBehavior.Gravity * m_Time;
	m_State.Scale += m_UpdateBehavior.Scale;
	m_State.Rotation += m_UpdateBehavior.Rotation;
	m_State.Color += m_UpdateBehavior.Color;

	m_State.World.RotationZXY(m_State.Rotation);
	m_State.World.Scaling(m_State.Scale, m_State.World);
	m_State.World.SetTranslation(m_State.Position);
	m_Motion.AddTimer(CUtilities::GetFrameSecond());
}

void CParticle::Render()
{
	if (IsEnd())
		return;
	if (m_State.Color.w <= 0.0f)return;
	Mof::Blending prevBlend = g_pGraphics->GetBlending();
	g_pGraphics->SetBlending(m_ParameterBehavior.Blending);
	CMatrix44 rMat = m_State.World;
	if (m_ParameterBehavior.Flag & CParticle::BILLBOARD) 
	{
		LPCamera pCam = CGraphicsUtilities::GetCamera();
		rMat.Multiply3x3(pCam->GetBillBoardMatrix());
	}
	if (m_pTexture)
	{
		m_pTexture->Render(rMat, m_Motion.GetSrcRect(), m_State.Color.ToU32Color());
		
	}
	//ここでブレンドモードを戻さないと、ほかのオブジェクトの表示がおかしくなる。
	g_pGraphics->SetBlending(prevBlend);
}

void CParticle::Release()
{
	if (m_pTexture )
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	if (m_pSound )
	{
		m_pSound->Release();
		m_pSound = NULL;
	}
	m_Motion.Release();
}

void CParticle::Start()
{
	m_Time = 0;
	if (m_pSound)
		m_pSound->Play();
	m_Motion.SetTime(0.0f);
	m_State.Color.a = 1.0f;
}

void CParticle::SetSize(float size)
{
	m_State.Scale = ONE_VECTOR3 * size;
}
