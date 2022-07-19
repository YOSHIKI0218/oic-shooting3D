#include "Boss.h"

// ボスの登場アニメーション

// 手前から奥へ回転しながら登場
ANIM_DATA g_BossAnimPosZ[2] = {
	{ 0.0f,-FIELD_HALF_Z,EASE_LINEAR },
	{ 2.0f,2.0f,EASE_OUT_SINE },
};
ANIM_DATA g_BossAnimRotZ[2] = {
	{ 0.0f,MOF_MATH_2PI,EASE_LINEAR },
	{ 2.0f,0.0f,EASE_OUT_SINE },
};
// 手前側に向くように回転
ANIM_DATA g_BossAnimRotX[2] = {
	{ 2.0f,MOF_MATH_PI,EASE_LINEAR },
	{ 4.0f,0.0f,EASE_INOUT_SINE },
};
// 浮上する
ANIM_DATA g_BossAnimPosY[2] = {
	{ 4.0f,-10.0f,EASE_LINEAR },
	{ 6.0f,-3.5f,EASE_INOUT_SINE },
};

/**
 * コンストラクタ
 *
 */
CBoss::CBoss():
m_Mesh(),
m_PartsMesh(),
m_Pos(0,0,0),
m_Rot(0,0,0),
m_matWorld(),
m_bShow(false),
m_HP(200),
m_ShotWait(0),
m_TargetPos(0,0,0),
m_AnimTime(0),
m_Parts(){
}

/**
 * デストラクタ
 *
 */
CBoss::~CBoss(){
}

/**
 * 読み込み
 *
 */
bool CBoss::Load(){
	// ボス本体のメッシュ
	if(!m_Mesh.Load("boss_core.mom"))
	{
		return false;
	}
	// ボスのパーツのメッシュ
	if(!m_PartsMesh.Load("boss_parts.mom"))
	{
		return false;
	}
	return true;
}

/**
 * 初期化
 *
 */
void CBoss::Initialize(){
	m_Pos = Vector3(0,0,0);
	m_Rot = Vector3(0,0,0);
	m_matWorld.Identity();
	m_bShow = false;
	m_HP = 200;
	m_ShotWait = 0;
	m_TargetPos = Vector3(0,0,0);
	m_AnimTime = 0;
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].Initialize();
	}
}

/**
 * 開始
 *
 */
void CBoss::Start(){
	m_Pos = Vector3(0,0,0);
	m_Rot = Vector3(0,0,0);
	m_matWorld.Identity();
	m_bShow = true;
	m_HP = 200;
	m_ShotWait = 0;
	m_TargetPos = Vector3(0,0,0);
	m_AnimTime = 0;

	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].SetMesh(&m_PartsMesh);
		m_Parts[i].Start(Vector3(0,0,0),1+i);
	}
}

/**
 * 更新
 *
 */
void CBoss::Update(CEnemyShot* shot,int smax){
	// 非表示
	if(!GetShow())
	{
		return;
	}
	
	// パーツの数をカウント
	int partsCnt = GetPartsCount();

	if(m_AnimTime<g_BossAnimPosY[1].Time)
	{
		// 時間を進める
		m_AnimTime += CUtilities::GetFrameSecond();

		// 補間処理
		m_Pos.y = InterpolationAnim(m_AnimTime,g_BossAnimPosY,2);
		m_Pos.z = InterpolationAnim(m_AnimTime,g_BossAnimPosZ,2);
		m_Rot.x = InterpolationAnim(m_AnimTime,g_BossAnimRotX,2);
		m_Rot.z = InterpolationAnim(m_AnimTime,g_BossAnimRotZ,2);
	}
	// アニメーション終了後の処理
	else
	{
		// 自機の方向を向く
		Vector3 direction = m_TargetPos - (m_Pos);
		float dr = atan2(direction.x, direction.z) + MOF_MATH_PI;
		float r = fmodf((dr - m_Rot.y) + MOF_MATH_PI, MOF_MATH_2PI);
		m_Rot.y += ((0 < r) ? r - MOF_MATH_PI : r + MOF_MATH_PI) * 0.1f;

		// 撃破されるごとに回転
		m_Rot.z += (MOF_ToRadian((3 - partsCnt)*120.0f) - m_Rot.z)*0.05f;

		// 全てのパーツが破壊されたとき
		if(partsCnt == 0)
		{
			// Y座標を0に近づける
			m_Pos.y *= 0.95f;

			// 弾の発射
			if(m_ShotWait <= 0)
			{
				// 弾の同時発射数
				int sCnt = 16;
				// 弾の発射角度
				float ad = ((float)rand() / RAND_MAX)*360.0f / sCnt;
				for(int cnt = 0; cnt < sCnt; cnt++)
				{
					CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot,smax);
					if(newShot)
					{
						m_ShotWait = 20;
						// 角度の方向に弾を進める
						float rad = MOF_ToRadian(ad);
						Vector3 vt(cos(rad), 0, sin(rad));
						newShot->Fire(m_Pos, vt * 0.2f);
					}
					ad += 360.0f / sCnt;
				}
			}
			else
			{
				m_ShotWait--;
			}
		}
	}
	
	m_matWorld.RotationZXY(m_Rot);
	m_matWorld.SetTranslation(m_Pos);

	// パーツの更新
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		CEnemy& parts = m_Parts[i];
		
		float angle = -(MOF_MATH_2PI * i / BOSS_PARTS_MAX);
		// 位置
		CVector3 p(0.0f,3.5f,0.0f);
		p.RotationZ(angle);
		p *= m_matWorld;
		parts.SetPosition(p);
		// 回転
		CVector3 r(m_Rot);
		r.z += angle;
		parts.SetRotation(r);

		// 弾の発射
		if(g_BossAnimPosY[1].Time <= m_AnimTime && i == BOSS_PARTS_MAX - partsCnt)
		{
			parts.SetTargetPos(m_TargetPos);
			parts.Update(shot,smax);
		}
	}
}

/**
 * ダメージ処理
 *
 */
void CBoss::Damage(int dmg){
	m_HP -= dmg;
	if(m_HP <= 0)
	{
		
		gEffectManager.AddEmitter(EMITTER_EXPLOTION_FLASH, m_Pos);
		gEffectManager.AddEmitter(EMITTER_EXPLOTION_SMOKE, m_Pos);
		gEffectManager.AddEmitter(EMITTER_EXPLOTION_FIRE, m_Pos);
		gEffectManager.AddEmitter(EMITTER_EXPLOTION_SPARK, m_Pos);
		
		m_bShow = false;
	}
}

/**
 * 描画
 *
 */
void CBoss::Render(){
	if(!GetShow())
	{
		return;
	}

	m_Mesh.Render(m_matWorld);

	// パーツの描画
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].Render();
	}
}

/**
 * デバッグ描画
 *
 */
void CBoss::RenderDebug(){
	// 非表示
	if(!GetShow())
	{
		return;
	}
	// 当たり判定の表示
	CGraphicsUtilities::RenderSphere(GetSphere(),Vector4(1,0,0,0.3f));
	
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].RenderDebug();
	}
}

/**
 * 表示中のパーツの数
 *
 */
int CBoss::GetPartsCount(){
	int partsCnt = 0;
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		if(m_Parts[i].GetShow())
		{
			partsCnt++;
		}
	}
	return partsCnt;
}

/**
 * 解放
 *
 */
void CBoss::Release(){
	m_Mesh.Release();
	m_PartsMesh.Release();
}
