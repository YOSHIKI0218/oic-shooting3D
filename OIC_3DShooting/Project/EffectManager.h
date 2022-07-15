#pragma once
#include "GameDefine.h"
#include "Particle.h"
class CEffectManager
{
private:
	//エフェクトの可変長配列
	static CDynamicArray<CParticle*> m_Effects;

public:
	//エフェクトのスポーン(再生)
	static CParticle* Spawn(int id);
	//作成情報を利用してエフェクトをスポーン(再生)
	static CParticle* Spawn(int id, CParticle::CREATEINFO& info);
	//エフェクトを作成。作成のみで、再生しない
	static CParticle* Create(CParticle::CREATEINFO& info);
	//すべてのエフェクトの更新
	static void Update();
	//すべてのエフェクトの描画
	static void Render();
	//解放
	static void Release();
};

