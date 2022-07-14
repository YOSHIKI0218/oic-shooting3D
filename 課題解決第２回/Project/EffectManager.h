#pragma once

#include "Emitter.h"


/**
 * グローバル変数で作成
 */
class CEffectManager;
extern CEffectManager gEffectManager;

/**
 * エミッターの種類
 */
enum EMITTERTYPE{
	EMITTER_SHOT,					//プレイヤー弾発射エフェクト
	EMITTER_HIT,					//プレイヤー弾ヒットエフェクト
	EMITTER_EXPLOTION_FLASH,		//爆発エフェクト(フラッシュ)
	EMITTER_EXPLOTION_SMOKE,		//爆発エフェクト(煙)
	EMITTER_EXPLOTION_FIRE,			//爆発エフェクト(炎)
	EMITTER_EXPLOTION_SPARK,		//爆発エフェクト(火の粉)
	EMITTER_AFTERIMAGE,				//プレイヤーの残像
	EMITTER_AFTERIMAGE_PARTICLE,	//プレイヤーの残像粒子
	EMITTERTYPE_MAX,				//エミッターの種類数
};

//typedef
typedef std::list<CEmitter<MINMAXPARAMETER>*> EmitterList;

/**
 * エフェクト管理クラス
 */
class CEffectManager{
private:
	//パーティクル
	std::list< CParticle* >	m_ParticleList;
	//エミッター
	EmitterList				m_EmitterList;

	//エフェクトのリソース
	CTexture			m_BlueFlash;
	CTexture			m_WhiteFlash;
	CTexture			m_Smoke;
	CTexture			m_Fire;
	CTexture			m_RedFlash;
	CMeshContainer		m_PlayerMesh;
	CTexture			m_Particle;

	//CREATEINFO
	CEmitter<MINMAXPARAMETER>::CREATEINFO	m_CreateInfo[EMITTERTYPE_MAX];
	//エミッターの寿命
	float									m_LifeTime[EMITTERTYPE_MAX];

public:
	//コンストラクタ
	CEffectManager();
	//デストラクタ
	~CEffectManager();
	//リソースの読み込み
	bool Load();
	//エミッターの動作パラメーター設定
	void InitCreateInfo();
	//更新処理
	void Update();
	//描画処理
	void Render();
	//全てのエフェクトを削除
	void Clear();
	//解放処理
	void Release();
	//エミッターの追加
	CEmitter<MINMAXPARAMETER>* AddEmitter(EMITTERTYPE type,const CVector3& pos);
	//寿命を指定してエミッターの追加
	CEmitter<MINMAXPARAMETER>* AddEmitter(EMITTERTYPE type,const CVector3& pos,float LifeTime);
	//エミッターの削除
	void DeleteEmitter(CEmitter<MINMAXPARAMETER>**ppEmitter);
};