#include "Boss.h"
#include "Score.h"
#include "EffectManager.h"

// ボスの登場アニメーション

// 手前から奥へ回転しながら登場
Anim_Data g_BossAnimPosZ[2];
Anim_Data g_BossAnimRotZ[2] = {
	{ 0.0f,MOF_MATH_2PI,Ease_Linear },
	{ 2.0f,0.0f,Ease_Sine_Out },
};
// 手前側に向くように回転
Anim_Data g_BossAnimRotX[2] = {
	{ 2.0f,MOF_MATH_PI,Ease_Linear },
	{ 4.0f,0.0f,Ease_Sine_InOut },
};
// 浮上する
Anim_Data g_BossAnimPosY[2] = {
	{ 4.0f,-10.0f,Ease_Linear },
	{ 6.0f,-3.5f,Ease_Sine_InOut },
};

int ExplosionInterval;

/**
 * コンストラクタ
 *
 */
CBoss::CBoss() :
	m_Mesh(),
	m_PartsMesh(),
	m_Pos(0, 0, 0),
	m_Rot(0, 0, 0),
	m_matWorld(),
	m_bShow(false),
	m_HP(200),
	m_ShotWait(0),
	m_TargetPos(0, 0, 0),
	m_AnimTime(0),
	m_Parts(),
	m_bDestroyed(false)
{
}

/**
 * デストラクタ
 *
 */
CBoss::~CBoss() {
}

/**
 * 読み込み
 *
 */
bool CBoss::Load() {
	// ボス本体のメッシュ
	if (!m_Mesh.Load("boss_core.mom"))
	{
		return false;
	}
	// ボスのパーツのメッシュ
	if (!m_PartsMesh.Load("boss_parts.mom"))
	{
		return false;
	}

	g_BossAnimPosZ[0] = { 0.0f,-CGameDefine::GetFiledHalfZ(),Ease_Linear };
	g_BossAnimPosZ[1] = { 2.0f,2.0f,Ease_Sine_Out };

	return true;
}

/**
 * 初期化
 *
 */
void CBoss::Initialize() {
	m_Pos = ZERO_VECTOR3;
	m_Rot = ZERO_VECTOR3;
	m_matWorld.Identity();
	m_bShow = false;
	m_HP = 200;
	m_ShotWait = 0;
	m_TargetPos = Vector3(0, 0, 0);
	m_AnimTime = 0;
	m_bDestroyed = false;

	for (int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].Initialize();
	}
}

/**
 * 開始
 *
 */
void CBoss::Start() {
	m_Pos = ZERO_VECTOR3;
	m_Rot = ZERO_VECTOR3;
	m_matWorld.Identity();
	m_bShow = true;
	m_HP = 200;
	m_ShotWait = 0;
	m_TargetPos = Vector3(0, 0, 0);
	m_AnimTime = 0;
	m_bDestroyed = false;
	//各パーツの初期化
	for (int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].SetMesh(&m_PartsMesh);
		m_Parts[i].Start(Vector3(0, 0, 0), false, 1 + i);
	}
	ExplosionInterval = 0;
}

/**
 * 更新
 *
 */
void CBoss::Update(CEnemyShot* shot, int smax) {
	// 非表示
	if (!GetShow())
	{
		return;
	}
	int partsCount = GetPartsCount();
	if (m_bDestroyed)
	{
		m_Pos.y -= 3.0f * CUtilities::GetFrameSecond();
		m_Rot.y += MOF_ToRadian(20 * CUtilities::GetFrameSecond());
		m_matWorld.RotationZXY(m_Rot);
		m_matWorld.SetTranslation(m_Pos);
		for (int i = 0; i < BOSS_PARTS_MAX; i++)
		{
			UpdateParts(i, shot, smax, i == BOSS_PARTS_MAX - partsCount);
		}

		
		if (ExplosionInterval == 0) 
		{
			auto eff = CEffectManager::Spawn(EFFECT_EXPLOSION);
			if (eff)
			{
				eff->SetPosition(m_Pos + BACK_VECTOR3 * 6);
				eff->SetSize(4);
			}
			ExplosionInterval = 30;
		}
		ExplosionInterval--;
		if (m_Pos.y < -20)
			m_bShow = false;
	}
	else 
	{
		if (m_AnimTime < g_BossAnimPosY[1].fTime)
		{
			m_AnimTime += CUtilities::GetFrameSecond();

			m_Pos.y = InterpolationAmin(m_AnimTime, g_BossAnimPosY, 2);
			m_Pos.z = InterpolationAmin(m_AnimTime, g_BossAnimPosZ, 2);

			m_Rot.x = InterpolationAmin(m_AnimTime, g_BossAnimRotX, 2);
			m_Rot.z = InterpolationAmin(m_AnimTime, g_BossAnimRotZ, 2);
		}
		else
		{
			RotateTarget();
			m_Rot.z += (MOF_ToRadian((3 - partsCount) * 120.0f) - m_Rot.z) * 0.05f;
			if (partsCount == 0)
			{
				m_Pos.y *= 0.95f;
				ShotAllDirShot(shot, smax, 16);

			}
		}

		m_matWorld.RotationZXY(m_Rot);
		m_matWorld.SetTranslation(m_Pos);

		for (int i = 0; i < BOSS_PARTS_MAX; i++)
		{
			UpdateParts(i, shot, smax, i == BOSS_PARTS_MAX - partsCount);
		}
	}
}

/**
 * ダメージ処理
 *
 */
void CBoss::Damage(int dmg) {
	if (GetPartsCount() > 0 || m_bDestroyed)
		return;
	CScore::AddScore(1);
	m_HP -= dmg;
	if (m_HP <= 0)
	{
		m_bDestroyed = true;
		CScore::AddScore(50);
	}
}

/**
 * 相手の方向へ回転する
 *
 */
void CBoss::RotateTarget() {
	CVector3 direction = m_TargetPos - (m_Pos);
	float dr = atan2(direction.x, direction.z) + MOF_MATH_PI;
	float r = fmodf((dr - m_Rot.y) + MOF_MATH_PI, MOF_MATH_2PI);
	//						↓を=にするとなぜか攻撃開始時に回転する・・・floatの誤差によって回転の回数が決まる？
	m_Rot.y += ((0 < r) ? r - MOF_MATH_PI : r + MOF_MATH_PI) * 0.1f;
}

/**
 * 全方位への弾発射
 *
 */
void CBoss::ShotAllDirShot(CEnemyShot* shot, int smax, int sCnt) {
	if (m_ShotWait > 0)
	{
		m_ShotWait--;
		return;
	}

	float ad = ((float)rand() / RAND_MAX) * 360.0f / sCnt;
	for (int cnt = 0; cnt < sCnt; cnt++)
	{
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (newShot)
		{
			m_ShotWait = 20;
			float rad = MOF_ToRadian(ad);
			Vector3 vt(cos(rad), 0, sin(rad));
			newShot->SetLifeTime(3);
			newShot->SetHorming(false);
			newShot->Fire(m_Pos, vt * 0.2f);
		}
		ad += 360.0f / sCnt;
	}
}

/**
 * パーツ更新
 *
 */
void CBoss::UpdateParts(int idx, CEnemyShot* shot, int smax, bool bShot) {
	CEnemy& parts = m_Parts[idx];
	float angle = -(MOF_MATH_2PI * idx / BOSS_PARTS_MAX);

	CVector3 p(0.0f, 3.5f, 0.0f);
	p.RotationZ(angle);
	p *= m_matWorld;
	parts.SetPosition(p);
	CVector3 r(m_Rot);
	r.z += angle;
	parts.SetRotation(r);
	if (g_BossAnimPosY[1].fTime <= m_AnimTime && bShot)
	{
		parts.SetTargetPos(m_TargetPos);
		parts.Update(shot, smax);
	}
}

/**
 * 描画
 *
 */
void CBoss::Render() {
	if (!GetShow())
		return;
	m_Mesh.Render(m_matWorld);

	for (int i = 0; i < BOSS_PARTS_MAX; i++)
		m_Parts[i].Render();
}

/**
 * デバッグ描画
 *
 */
void CBoss::RenderDebug() {
	// 非表示
	if (!GetShow())
	{
		return;
	}
	// 当たり判定の表示
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(1, 0, 0, 0.3f));

	for (int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].Render();
	}
}

/**
 * 表示中のパーツの数
 *
 */
int CBoss::GetPartsCount() {
	int partsCnt = 0;
	for (int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		if (m_Parts[i].GetShow())
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
void CBoss::Release() {
	m_Mesh.Release();
	m_PartsMesh.Release();
}
