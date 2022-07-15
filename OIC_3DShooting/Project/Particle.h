#pragma once
#include "Mof.h"
class CParticle
{
public:
	enum tag_FLAG
	{
		BILLBOARD = 0x00000001
	};

	typedef struct tag_STATE
	{
		CVector3 Position;
		CVector3 Scale;
		CVector3 Rotation;
		CVector4 Color;
		CMatrix44 World;

		tag_STATE():
			Position(),
			Scale(1,1,1),
			Rotation(),
			Color(1,1,1,1),
			World()
		{}
	}STATE;

	typedef struct tag_UPDATEBEHAVIOR
	{
		CVector3 Move;
		CVector3 Scale;
		CVector3 Rotation;
		CVector4 Color;
	}UPDATEBEHAVIOR;

	typedef struct tag_PARAMETERBEHAVIOR
	{
		MofFloat		Existence;
		Blending		Blending;
		MofU32				Flag;
		SpriteAnimationCreate Motion;
		
	}PARAMETERBEHAVIOR;

	typedef struct tag_ENVIROMENTBEHAVIOR
	{
		CVector3	Gravity;

	}ENVIROMENTBEHAVIOR;

	typedef struct tag_CREATEINFO
	{
		STATE	State;
		UPDATEBEHAVIOR	UpdateBehavior;
		PARAMETERBEHAVIOR	ParameterBehavior;
		ENVIROMENTBEHAVIOR EnviromentBehavior;
		LPTexture			pTexture;
		CSoundBuffer* pBuffer;
		int				ID;
	}CREATEINFO;

protected:
	MofFloat		m_Time;
	STATE			m_State;
	STATE			m_InitalizeState;
	UPDATEBEHAVIOR	m_UpdateBehavior;
	PARAMETERBEHAVIOR	m_ParameterBehavior;
	ENVIROMENTBEHAVIOR m_EnviromentBehavior;
	LPTexture			m_pTexture;
	CSpriteMotionController m_Motion;
	CSoundBuffer*			m_pSound;
	bool					m_Started;
	int						m_ID;
public:
	CParticle();
	~CParticle();
	void Initalize(CREATEINFO& info);
	void Update();
	void Render();
	void Release();
	void Start();
	void SetSize(float size);
	void SetPosition(const Vector3& vec) { m_State.Position = vec; }
	int	GetID() { return m_ID; }
	MofBool IsEnd() { return m_Time >= m_ParameterBehavior.Existence || m_Time < 0; }
};

