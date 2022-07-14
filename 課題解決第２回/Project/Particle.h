#pragma once

#include	"Mof.h"

class CParticle {
public:

	//パーティクルの挙動制御フラグ
	enum tag_FLAG {
		BILLBOARD		=	0x00000001,
	};

	//状態保存用
	typedef struct tag_STATE {
		CVector3			Position;
		CVector3			Scale;
		CVector3			Rotation;
		CVector4			Color;
		CMatrix44			World;

		/**
		* コンストラクタ
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

	//更新に関連する挙動パラメーター
	typedef struct tag_UPDATEBEHAVIOR {
		CVector3			Move;
		CVector3			Scale;
		CVector3			Rotation;
		CVector4			Color;
	}UPDATEBEHAVIOR;

	//設定に関連する挙動パラメーター
	typedef struct tag_PARAMETERBEHAVIOR {
		MofFloat			Existence;
		Blending			Blending;
		MofU32				Flag;
	}PARAMETERBEHAVIOR;

	//発生環境による挙動パラメーター
	typedef struct tag_ENVIRONMENTBEHAVIOR {
		CVector3			Gravity;
	}ENVIRONMENTBEHAVIOR;

	//生成用情報
	typedef struct tag_CREATEINFO {
		STATE				State;
		UPDATEBEHAVIOR		UpdateBehavior;
		PARAMETERBEHAVIOR	ParameterBehavior;
		ENVIRONMENTBEHAVIOR EnvironmentBehavior;
		LPTexture			pTexture;
		LPMeshContainer		pMesh;
	}CREATEINFO;

protected:
	MofFloat				m_Time;

	STATE					m_State;
	STATE					m_InitializeState;

	UPDATEBEHAVIOR			m_UpdateBehavior;

	PARAMETERBEHAVIOR		m_ParameterBehavior;

	ENVIRONMENTBEHAVIOR		m_EnvironmentBehavior;

	LPTexture				m_pTexture;
	LPMeshContainer			m_pMesh;

	//ソート用ビュー座標
	CVector3				m_ViewPosition;
public:
	CParticle();
	~CParticle();
	void Initialize(CREATEINFO& info);
	void Update(void);
	void Render(void);
	void Release(void);
	MofBool IsEnd(void){	return m_Time >= m_ParameterBehavior.Existence ? TRUE : FALSE; }
	void UpdateViewPosition(void){
		LPCamera pCam = CGraphicsUtilities::GetCamera();
		m_State.World.GetTranslation(m_ViewPosition);
		m_ViewPosition *= pCam->GetViewMatrix();
	}
	static bool ViewComp(CParticle*& v1, CParticle*& v2) {
		if (v1->m_ViewPosition.z > v2->m_ViewPosition.z)
		{
			return true;
		}
		return false;
	}
};