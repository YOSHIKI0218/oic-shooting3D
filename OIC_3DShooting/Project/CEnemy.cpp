#include "CEnemy.h"
#include "Score.h"
#include	"EffectManager.h"

//ステージ番号(実体はGameScene.cpp)
extern int						gStageNo;

//ステージ1の移動パターンY
Anim_Data g_EnemyAnimPosY[3] = {
	{1.0f, -20.0f, Ease_Sine_In},
	{3.0f, 0.0f, Ease_Sine_InOut},
	{6.0f, 0.0f, Ease_Sine_InOut}
};

//ステージ1の移動パターンZ
Anim_Data g_EnemyAnimPosZ[5] = {
	{0.0f, 0, Ease_Linear},
	{1.0f, 0, Ease_Linear},
	{2.0f, 0, Ease_Sine_Out},
	{3.0f, 0, Ease_Sine_In},
	{5.0f, 0, Ease_Linear}
};

//ステージ2の移動パターンX
Anim_Data g_EnemyAnimPosX2[3] = {
	{1.0f, -16.0f, Ease_Sine_In},
	{5.0f, 0.0f, Ease_Sine_In},
	{10.0f, 15.0f, Ease_Linear}
};
//ステージ2の移動パターンY
Anim_Data g_EnemyAnimPosY2[3] = {
	{1.0f, -20.0f, Ease_Sine_In},
	{3.0f, 0.0f, Ease_Sine_InOut},
	{6.0f, 0.0f, Ease_Sine_InOut}
};
//ステージ2の移動パターンZ
Anim_Data g_EnemyAnimPosZ2[2] = {
	{0.0f, 0, Ease_Linear},
	{6.0f, 8, Ease_Linear}
};

//ステージ3の移動パターンY
Anim_Data g_EnemyAnimPosY3[3] = {
	{1.0f, 20.0f, Ease_Sine_In},
	{3.0f, 0.0f, Ease_Sine_InOut},
	{6.0f, 0.0f, Ease_Sine_InOut}
};
//ステージ3の移動パターンZ
Anim_Data g_EnemyAnimPosZ3[7] = {
	{0.0f, 0, Ease_Linear},
	{1.0f, 0, Ease_Linear},
	{2.0f, 0, Ease_Linear},
	{3.0f, 0, Ease_Linear},
	{4.0f, 0, Ease_Sine_Out},
	{5.0f, 0, Ease_Sine_In},
	{7.0f, 0, Ease_Linear}
};



CEnemy::CEnemy():
m_pMesh(NULL),
m_vPos(0,0,0),
m_vRot(0, 0, 0),
m_bShow(false),
m_fAnimTime(0),
m_HP(0),
m_ShotWait(0),
m_ShotWaitSet(0),
m_TargetPos(0,0,0),
m_Type(0)
{}

CEnemy::~CEnemy() 
{
	
}
void CEnemy::Initialize() {
	m_vPos = Vector3(0, 0, 0);
	m_vRot = Vector3(0, 0, 0);
	m_bShow = false;
	m_fAnimTime = 0;
	m_ShotWait = 0;
	m_ShotWaitSet = 20;
	m_TargetPos = Vector3(0, 0, 0);

	float halfZ = CGameDefine::GetFiledHalfZ();
	g_EnemyAnimPosZ[0].fValue = -halfZ;
	g_EnemyAnimPosZ[1].fValue = halfZ - 10.0f;
	g_EnemyAnimPosZ[2].fValue = halfZ;
	g_EnemyAnimPosZ[3].fValue = halfZ - 10.0f;
	g_EnemyAnimPosZ[4].fValue = -halfZ;


	g_EnemyAnimPosZ3[0].fValue = halfZ - 30.0f;
	g_EnemyAnimPosZ3[1].fValue = halfZ - 10.0f;
	g_EnemyAnimPosZ3[2].fValue = -halfZ;
	g_EnemyAnimPosZ3[3].fValue = halfZ - 10.0f;
	g_EnemyAnimPosZ3[4].fValue = halfZ;
	g_EnemyAnimPosZ3[5].fValue = halfZ - 10.0f;
	g_EnemyAnimPosZ3[6].fValue = -halfZ;

	
}

void CEnemy::Update(CEnemyShot* shot, int smax) {

	if (!m_bShow) { return; }
	// 見えたら？
	m_fAnimTime += CUtilities::GetFrameSecond();

	switch (m_Type)
	{
	case 0:		UpdateType0(shot, smax);		break;
	case 1:
	case 2:
	case 3:		UpdateBossParts(shot, smax);	break;
	}
}

void CEnemy::Render() {

	if (!m_bShow) 
	{
		return;
	}

	// 見えたら？

	CMatrix44 matWorld;
	matWorld.RotationZXY(m_vRot);
	matWorld.SetTranslation(m_vPos);
	m_pMesh->Render(matWorld);
}
void CEnemy::RenderDebug3D(int i) {
	CGraphicsUtilities::RenderString(10, 70 + i * 24, MOF_XRGB(0, 0, 0),
		"敵[%d] %s, 位置 X : %.1f , Y : %.1f , Z : %.1f",i+1,(GetShow()?"表示":"非表示"), m_vPos.x, m_vPos.y, m_vPos.z);
	if (!m_bShow)
		return;
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));
}

void CEnemy::Start(const Vector3& p, bool isRight, int t = 0) {



	m_vPos = p;
	m_vRot = Vector3(0, 0, 0);
	m_bShow = true;
	m_fAnimTime = 0;
	m_HP = CGameDefine::GetEnemyHP();
	m_ShotWait = 0;
	m_ShotWaitSet = 10;
	m_TargetPos = Vector3(0, 0, 0);
	m_bRight = isRight;
	m_Type = t;
	switch (m_Type)
	{
	case 1:
	case 2:
	case 3:
		m_HP = 100;
		m_ShotWaitSet = 100;
		m_ShotWait = m_ShotWaitSet;
		break;
	}
}

void CEnemy::SetMesh(CMeshContainer* pm) 
{
	m_pMesh = pm;
}
bool CEnemy::GetShow() 
{
	return m_bShow;
}

void CEnemy::Damage(int dmg)
{
	m_HP -= dmg;
	CScore::AddScore(1);
	if (m_HP <= 0)
	{
		m_bShow = false;
		auto eff = CEffectManager::Spawn(EFFECT_EXPLOSION);
		if (eff)
		{
			eff->SetPosition(m_vPos);
			eff->SetSize(2);
		}

	}
}

void CEnemy::Release()
{

}

/**
 * 更新
 * m_Type が 0 の敵の更新関数
 * Update 関数から switch でタイプが一致した場合のみ実行
 */
void CEnemy::UpdateType0(CEnemyShot* shot, int smax) {
	// 時間を進める
	m_fAnimTime += CUtilities::GetFrameSecond();
	// アニメーション
	switch (gStageNo)
	{
	case 1:
		m_vPos.y = InterpolationAmin(m_fAnimTime, g_EnemyAnimPosY, 3);
		m_vPos.z = InterpolationAmin(m_fAnimTime, g_EnemyAnimPosZ, 5);
		break;
	case 2:
		m_vPos.x = InterpolationAmin(m_fAnimTime, g_EnemyAnimPosX2, 3);
		m_vPos.y = InterpolationAmin(m_fAnimTime, g_EnemyAnimPosY2, 3);
		m_vPos.z = InterpolationAmin(m_fAnimTime, g_EnemyAnimPosZ2, 2);
		break;
	case 3:
		m_vPos.y = InterpolationAmin(m_fAnimTime, g_EnemyAnimPosY3, 3);
		m_vPos.z = InterpolationAmin(m_fAnimTime, g_EnemyAnimPosZ3, 7);
		break;
	default:
		break;
	}
	
	// プレイヤーと同じ高さまで移動したら
	if (g_EnemyAnimPosY[1].fTime < m_fAnimTime)
	{
		// 弾の発射
		if (m_ShotWait <= 0)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (newShot)
			{
				m_ShotWait = m_ShotWaitSet;
				// 目標地点に向かうための方向
				Vector3 direction = m_TargetPos - m_vPos;
				// 目標地点までの距離を求める
				float distance = CVector3Utilities::Length(direction);
				// 距離が0以下=完全に同じ位置の場合は発射をしない
				if (distance > 0)
				{
					// 方向を正規化
					direction /= distance;
					newShot->SetHorming(m_bRight);
					newShot->SetLifeTime(m_bRight ? 4.0f : 5.0f);
					newShot->Fire(m_vPos, direction * 0.075f);
				}
			}
		}
		else
		{
			m_ShotWait--;
		}
	}
	// アニメーション
	switch (gStageNo)
	{
	case 1:
		if (g_EnemyAnimPosZ[4].fTime < m_fAnimTime)
			m_bShow = false;
		break;
	case 2:
		if (g_EnemyAnimPosX2[2].fTime < m_fAnimTime)
			m_bShow = false;
		break;
	case 3:
		if (g_EnemyAnimPosZ3[6].fTime < m_fAnimTime)
			m_bShow = false;
		break;
	default:
		break;
	}
	// アニメーション終了で消去

}

/**
 * 更新
 * m_Type が 1,2,3 の敵の更新関数
 * Update 関数から switch でタイプが一致した場合のみ実行
 */
void CEnemy::UpdateBossParts(CEnemyShot* shot, int smax) {
	if (m_ShotWait <= 0)
	{
		m_ShotWait = m_ShotWaitSet;
	}
	else
	{
		m_ShotWait--;
	}

	if (m_ShotWait % 10 == 0 && m_ShotWait / 10 < 3)
	{
		int sCnt = m_Type - 1;
		for (int cnt = -sCnt; cnt <= sCnt; cnt++)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (!newShot)
				continue;

			Vector3 pos = m_vPos;
			Vector3 direction = m_TargetPos - pos;

			float distance = CVector3Utilities::Length(direction);
			if (distance <= 0)
				continue;

			direction /= distance;
			float ad = atan2(direction.z, direction.x) + cnt * MOF_ToRadian(10);
			Vector3 vt(cos(ad), 0, sin(ad));
			newShot->SetLifeTime(4);
			newShot->Fire(pos, vt * 0.2f);
			
			newShot->SetHorming(false);
		}
	}
}