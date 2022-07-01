#pragma once

#include	"Mof.h"

class CParticle {
public:

	enum tag_FLAG
	{
		BILLBOARD = 0x00000001,
	};

	//��ԕۑ��p
	typedef struct tag_STATE {
		CVector3			Position;
		CVector3			Scale;
		CVector3			Rotation;
		CVector4			Color;
		CMatrix44			World;

		/**
		* �R���X�g���N�^
		*
		*/
		tag_STATE() :
		Position() ,
		Scale(1,1,1) ,
		Rotation() ,
		Color(1,1,1,1) ,
		World() {
		}
	}STATE;

	//�X�V�Ɋ֘A���鋓���p�����[�^�[
	typedef struct tag_UPDATEBEHAVIOR {
		CVector3			Move;
		CVector3			Scale;
		CVector3			Rotation;
		CVector4			Color;
	}UPDATEBEHAVIOR;

	//�ݒ�Ɋ֘A���鋓���p�����[�^�[
	typedef struct tag_PARAMETERBEHAVIOR {
		MofFloat			Existence;
		Blending			Blending;
		MofU32				Flag;
	}PARAMETERBEHAVIOR;

	//�������ɂ�鋓���p�����[�^�[
	typedef struct tag_ENVIRONMENTBEHAVIOR {
		CVector3			Gravity;
	}ENVIRONMENTBEHAVIOR;

	//�����p���
	typedef struct tag_CREATEINFO {
		STATE				State;
		UPDATEBEHAVIOR		UpdateBehavior;
		PARAMETERBEHAVIOR	ParameterBehavior;
		ENVIRONMENTBEHAVIOR EnvironmentBehavior;
		LPTexture			pTexture;
	}CREATEINFO;

protected:
	MofFloat				m_Time;

	STATE					m_State;
	STATE					m_InitializeState;

	UPDATEBEHAVIOR			m_UpdateBehavior;

	PARAMETERBEHAVIOR		m_ParameterBehavior;

	ENVIRONMENTBEHAVIOR		m_EnvironmentBehavior;

	LPTexture				m_pTexture;
public:
	CParticle();
	~CParticle();
	void Initialize(CREATEINFO& info);
	void Update(void);
	void Render(void);
	void Release(void);
	MofBool IsEnd(void){	return m_Time >= m_ParameterBehavior.Existence ? TRUE : FALSE; }
};