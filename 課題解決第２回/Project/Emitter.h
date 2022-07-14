#pragma once

#include	"Particle.h"
#include	<list>

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
		LPMeshContainer		pMesh;
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
	MofFloat				m_LifeTime;

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
		m_LifeTime(0.0f),
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
	void Initialize(CREATEINFO& info,float LifeTime){
		m_LifeTime = LifeTime;
		m_Time = 0.0f;
		m_UpdateBehavior = info.UpdateBehavior;
		m_ParameterBehavior = info.ParameterBehavior;
		m_EnvironmentBehavior = info.EnvironmentBehavior;
		m_CreateBehavior = info.CreateBehavior;
	}

	/**
	* 更新
	*
	* 引数
	* [in]			pList			パーティクルリスト
	*/
	virtual void Update(std::list< CParticle* >* pList){
		//寿命の減少
		if(0 < m_LifeTime)
		{
			m_LifeTime -= 1;
			if(m_LifeTime <= 0)
			{
				m_LifeTime = 0;
			}
		}

		//存在時間の更新
		m_Time++;

		//行列の更新
		m_State.World.RotationZXY(m_State.Rotation);
		m_State.World.Scaling(m_State.Scale, m_State.World);
		m_State.World.SetTranslation(m_State.Position);

		//発生時間経過
		if (m_Time < m_CreateBehavior.Time)
		{
			return;
		}
		//パーティクルの生成情報
		CParticle::CREATEINFO cinfo;
		for (MofU32 i = 0; i < m_CreateBehavior.Count; i++)
		{
			//初期状態の設定
			m_CreateBehavior.Position.GetParameter(cinfo.State.Position);
			cinfo.State.Position *= m_State.World;
			m_CreateBehavior.Scale.GetParameter(cinfo.State.Scale);
			cinfo.State.Scale *= m_State.Scale;
			m_CreateBehavior.Rotation.GetParameter(cinfo.State.Rotation);
			cinfo.State.Rotation += m_State.Rotation;
			m_CreateBehavior.Color.GetParameter(cinfo.State.Color);
			cinfo.State.Color *= m_State.Color;
			cinfo.State.World.RotationZXY(cinfo.State.Rotation);
			cinfo.State.World.Scaling(cinfo.State.Scale, cinfo.State.World);
			cinfo.State.World.SetTranslation(cinfo.State.Position);
			//更新情報の生成
			m_UpdateBehavior.Move.GetParameter(cinfo.UpdateBehavior.Move);
			m_UpdateBehavior.Scale.GetParameter(cinfo.UpdateBehavior.Scale);
			m_UpdateBehavior.Rotation.GetParameter(cinfo.UpdateBehavior.Rotation);
			m_UpdateBehavior.Color.GetParameter(cinfo.UpdateBehavior.Color);
			//環境情報の生成
			cinfo.EnvironmentBehavior.Gravity = m_EnvironmentBehavior.Gravity;
			//パラメーター情報の生成
			m_ParameterBehavior.Existence.GetParameter(cinfo.ParameterBehavior.Existence);
			cinfo.ParameterBehavior.Blending = m_ParameterBehavior.Blending;
			cinfo.ParameterBehavior.Flag = m_ParameterBehavior.Flag;
			//テクスチャの設定
			cinfo.pTexture = m_ParameterBehavior.pTexture;
			//メッシュの設定
			cinfo.pMesh = m_ParameterBehavior.pMesh;
			//パーティクルを生成して初期化
			CParticle* pPartice = new CParticle();
			pPartice->Initialize(cinfo);
			//リストに追加
			pList->push_back(pPartice);
		}
		//生成後の時間減少
		m_Time -= m_CreateBehavior.Time;
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
	* 寿命取得
	*/
	virtual float GetLifeTime(void){ return m_LifeTime; }
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