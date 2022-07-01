#pragma once

#include	"Particle.h"

//最小と最大から発生パラメーターを生成するためのテンプレート
template < typename T >
class MINMAXPARAMETER {
public:
	T					Min;
	T					Max;

	MINMAXPARAMETER() :
	Min() ,
	Max() {
	}

	void SetParameter(const T& v1, const T& v2){
		Min = v1;
		Max = v2;
	}

	void SetDefaultParameter(const T& v){
		Min = v;
		Max = v;
	}

	template < typename PT = T >
	void GetParameterImpl(PT& out){
		out = Min + (Max - Min) * CUtilities::Random(1001) * 0.001f;
	}

	template <  >
	void GetParameterImpl(CVector3& out){
		out.x = Min.x + (Max.x - Min.x) * CUtilities::Random(1001) * 0.001f;
		out.y = Min.y + (Max.y - Min.y) * CUtilities::Random(1001) * 0.001f;
		out.z = Min.z + (Max.z - Min.z) * CUtilities::Random(1001) * 0.001f;
	}

	template <  >
	void GetParameterImpl(CVector4& out){
		out.x = Min.x + (Max.x - Min.x) * CUtilities::Random(1001) * 0.001f;
		out.y = Min.y + (Max.y - Min.y) * CUtilities::Random(1001) * 0.001f;
		out.z = Min.z + (Max.z - Min.z) * CUtilities::Random(1001) * 0.001f;
		out.w = Min.w + (Max.w - Min.w) * CUtilities::Random(1001) * 0.001f;
	}

	void GetParameter(T& out){
		GetParameterImpl(out);
	}
};

//中間と幅から発生パラメーターを生成するためのテンプレート
template < typename T >
class AMPLITUDEPARAMETER {
public:
	T					Basis;
	T					Amplitude;

	AMPLITUDEPARAMETER() :
	Basis() ,
	Amplitude() {
	}

	void SetParameter(const T& v1, const T& v2){
		Basis = v1;
		Amplitude = v2;
	}

	void SetDefaultParameter(const T& v){
		Basis = v;
	}

	template < typename PT = T >
	void GetParameterImpl(PT& out){
		out = Basis + Amplitude * (1.0f - CUtilities::Random(2001) * 0.001f);
	}

	template <  >
	void GetParameterImpl(CVector3& out){
		out.x = Basis.x + Amplitude.x * (1.0f - CUtilities::Random(2001) * 0.001f);
		out.y = Basis.y + Amplitude.y * (1.0f - CUtilities::Random(2001) * 0.001f);
		out.z = Basis.z + Amplitude.z * (1.0f - CUtilities::Random(2001) * 0.001f);
	}

	template <  >
	void GetParameterImpl(CVector4& out){
		out.x = Basis.x + Amplitude.x * (1.0f - CUtilities::Random(2001) * 0.001f);
		out.y = Basis.y + Amplitude.y * (1.0f - CUtilities::Random(2001) * 0.001f);
		out.z = Basis.z + Amplitude.z * (1.0f - CUtilities::Random(2001) * 0.001f);
		out.w = Basis.w + Amplitude.w * (1.0f - CUtilities::Random(2001) * 0.001f);
	}

	void GetParameter(T& out){
		GetParameterImpl(out);
	}
};

//発生パラメータ生成用テンプレートクラスを指定するエミッター
template < template < typename > class PT >
class CEmitter {
public:

	//状態保存用
	typedef struct tag_STATE {
		CVector3			Position;
		CVector3			Scale;
		CVector3			Rotation;
		CVector4			Color;
		CMatrix44			World;

		tag_STATE() :
			Position(),
			Scale(1, 1, 1),
			Rotation(),
			Color(1, 1, 1, 1),
			World() {
		}
	}STATE;

	//更新に関連する挙動パラメーター
	typedef struct tag_UPDATEBEHAVIOR {
		PT< CVector3 >		Move;
		PT< CVector3 >		Scale;
		PT< CVector3 >		Rotation;
		PT< CVector4 >		Color;
	}UPDATEBEHAVIOR;

	//設定に関連する挙動パラメーター
	typedef struct tag_PARAMETERBEHAVIOR {
		PT< MofFloat >		Existence;
		Blending			Blending;
		MofU32				Flag;
		LPTexture			pTexture;
	}PARAMETERBEHAVIOR;

	//発生環境による挙動パラメーター
	typedef struct tag_ENVIRONMENTBEHAVIOR {
		CVector3			Gravity;
	}ENVIRONMENTBEHAVIOR;

	//生成に関連する挙動パラメーター
	typedef struct tag_CREATEBEHAVIOR {
		MofFloat			Time;
		MofU32				Count;

		PT< CVector3 >		Position;
		PT< CVector3 >		Scale;
		PT< CVector3 >		Rotation;
		PT< CVector4 >		Color;
	}CREATEBEHAVIOR;

	//生成用情報
	typedef struct tag_CREATEINFO {
		UPDATEBEHAVIOR		UpdateBehavior;
		PARAMETERBEHAVIOR	ParameterBehavior;
		ENVIRONMENTBEHAVIOR	EnvironmentBehavior;
		CREATEBEHAVIOR		CreateBehavior;
	}CREATEINFO;

protected:
	MofFloat				m_Time;

	STATE					m_State;

	UPDATEBEHAVIOR			m_UpdateBehavior;

	PARAMETERBEHAVIOR		m_ParameterBehavior;

	ENVIRONMENTBEHAVIOR		m_EnvironmentBehavior;

	CREATEBEHAVIOR			m_CreateBehavior;
public:
	/**
	* コンストラクタ
	*
	*/
	CEmitter() :
		m_Time(0.0f),
		m_State(),
		m_UpdateBehavior(),
		m_ParameterBehavior(),
		m_EnvironmentBehavior(),
		m_CreateBehavior() {
	}
	/**
	* デストラクタ
	*
	*/
	virtual ~CEmitter(){
	}

	/**
	* 初期化
	*
	* 引数
	* [in]			info			生成情報
	*/
	void Initialize(CREATEINFO& info){
	}

	/**
	* 更新
	*
	* 引数
	* [in]			pList			パーティクルリスト
	*/
	virtual void Update(std::list< CParticle* >* pList){
	}

	/**
	* 座標設定
	*/
	virtual void SetPosition(const CVector3& v){ m_State.Position = v; }
	/**
	* スケール設定
	*/
	virtual void SetScale(const CVector3& v){ m_State.Scale = v; }
	/**
	* 回転設定
	*/
	virtual void SetRotation(const CVector3& v){ m_State.Rotation = v; }
	/**
	* 色設定
	*/
	virtual void SetColor(const CVector4& v){ m_State.Color = v; }
	/**
	* ワールドマトリクス設定
	*/
	virtual void SetWorldMatrix(const CMatrix44& v){ m_State.World = v; }
	/**
	* 時間設定
	*/
	virtual void SetTime(MofFloat t){ m_Time = t; }

	/**
	* 座標取得
	*/
	virtual CVector3& GetPosition(void){ return m_State.Position; }
	/**
	* スケール取得
	*/
	virtual CVector3& GetScale(void){ return m_State.Scale; }
	/**
	* 回転取得
	*/
	virtual CVector3& GetRotation(void){ return m_State.Rotation; }
	/**
	* 色取得
	*/
	virtual CVector4& GetColor(void){ return m_State.Color; }
	/**
	* ワールドマトリクス取得
	*/
	virtual CMatrix44& GetWorldMatrix(void){ return m_State.World; }
};