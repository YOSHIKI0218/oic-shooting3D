#include "Boss1.h"
#include "Score.h"
#include "EffectManager.h"

//ステージ番号(実体はGameScene.cpp)
extern int						gStageNo;

// ボスの登場アニメーション

// 手前から奥へ回転しながら登場
Anim_Data g_BossAnimPosZ1[2];
Anim_Data g_BossAnimRotZ1[2] = {
	{ 0.0f,MOF_MATH_2PI,Ease_Linear },
	{ 2.0f,0.0f,Ease_Sine_Out },
};
// 手前側に向くように回転
Anim_Data g_BossAnimRotX1[2] = {
	{ 2.0f,MOF_MATH_PI,Ease_Linear },
	{ 4.0f,0.0f,Ease_Sine_InOut },
};
// 浮上する
Anim_Data g_BossAnimPosY1[2] = {
	{ 4.0f,-10.0f,Ease_Linear },
	{ 6.0f,0.0f,Ease_Sine_InOut },
};

Anim_Data g_BossFlipRotY[5] = {
	{ 0.0f,0,Ease_Sine_In},
	{ 0.05f,MOF_MATH_QUADPI,Ease_Linear },
	{ 0.1f,MOF_MATH_HALFPI,Ease_Linear },
	{ 0.15f,MOF_MATH_HALFPI + MOF_MATH_QUADPI,Ease_Linear },
	{ 0.2f,0.0f,Ease_Sine_Out }
};

void CBoss1::UpdateShot(CEnemyShot* shot, int smax)
{
	//ステージ2
	if (gStageNo == 2)
	{
		switch (m_CurrentShotPattern)
		{
		case SHOTPATTERN_SINGLE:
			m_ShotInterval = 10;
			Shot(shot, smax, 0, false);
			break;
		case SHOTPATTERN_TRIPLE:
			m_ShotInterval = 20;
			Shot(shot, smax, 1, false);
			break;
		case SHOTPATTERN_CIRCLE:
			m_ShotInterval = 10;
			ShotCircleDir(shot, smax, 7);
			break;
		case SHOTPATTERN_PARALLEL_FOUR:
			m_ShotInterval = 10;
			ShotParallel(shot, smax, 1);
			break;
		case SHOTPATTERN_HORMING_TRIPLE:
			m_ShotInterval = 30;
			Shot(shot, smax, 1, true);
			break;
		case SHOTPATTERN_SINEWAVE:
			m_ShotInterval = 10;
			ShotSineWave(shot, smax, 2);
			break;
		default:
			break;
		}
	}
	else//ステージ3(ラスボス)
	{
		switch (m_CurrentShotPattern)
		{
		case SHOTPATTERN_SINGLE:
			m_AttackAreaTime = 0;
			m_ExtremeAttackWaitTime = 0.0f;
			m_PatternChangeInterval = 5.0f;
			m_ShotInterval = 10;
			Shot(shot, smax, 0, false);
			break;
		case SHOTPATTERN_TRIPLE:
			m_ShotInterval = 18;
			Shot(shot, smax, 1, false);
			break;
		case SHOTPATTERN_CIRCLE:
			m_ShotInterval = 10;
			ShotCircleDir(shot, smax, 7);
			break;
		case SHOTPATTERN_PARALLEL_FOUR:
			m_ShotInterval = 10;
			ShotParallel(shot, smax, 1);
			break;
		case SHOTPATTERN_HORMING_TRIPLE:
			m_ShotInterval = 30;
			Shot(shot, smax, 1, true);
			break;
		case SHOTPATTERN_SINEWAVE:
			m_ShotInterval = 10;
			ShotSineWave(shot, smax, 2);
			break;
		case SHOTPATTERN_EXTREMEPARALLEL:
			m_PatternChangeInterval = 7.0f;
			m_ShotInterval = 5;
			m_ExtremeAttackWaitTime += CUtilities::GetFrameSecond();
			
			//待ち時間が2秒を超えていれば攻撃開始
			if(m_ExtremeAttackWaitTime > 2.0f)
			ShotExtremeParallel(shot, smax, 3);
			break;
		default:
			break;
		}
	}
}

bool CBoss1::Load(LPCMofChar modelName)
{
	// ボス本体のメッシュ
	if (!m_Mesh.Load(modelName) || !m_ShieldMesh.Load("boss1_shield.mom") || !m_AttackAreaMesh.Load("plane_red.mom"))
	{
		return false;
	}
	g_BossAnimPosZ1[0] = { 0.0f,CGameDefine::GetFiledHalfZ(),Ease_Linear };
	g_BossAnimPosZ1[1] = { 2.0f,2.0f,Ease_Sine_Out };
	return true;
}

void CBoss1::Initialize()
{
	m_Pos = ZERO_VECTOR3;
	m_Rot = ZERO_VECTOR3;
	m_matWorld.Identity();
	m_bShow = false;
	m_MaxHP = CGameDefine::GetBoss1MaxHP();
	m_HP = m_MaxHP;
	m_ShotWait = 0;
	m_TargetPos = Vector3(0, 0, 0);
	m_AnimTime = 0;
	m_bDestroyed = false;
	m_bDestroyedAndRemoved = false;
	m_FlipTime = 0;
	m_bFlip = false;
}

void CBoss1::Start()
{
	m_Pos = ZERO_VECTOR3;
	m_Rot = ZERO_VECTOR3;
	m_matWorld.Identity();
	m_bShow = true;
	
	m_HP = m_MaxHP;
	m_ShotWait = 0;
	m_TargetPos = ZERO_VECTOR3;
	m_AnimTime = 0;
	m_bDestroyed = false;
	m_bDestroyedAndRemoved = false;
	m_CurrentShotPattern = SHOTPATTERN_SINGLE;
	
	m_ExtremeAttackWaitTime = 0.0f;
	m_TimeElapsed = 0.0f;
	m_FlipTime = false;
	m_bFlip = false;
	m_PatternChangeInterval = gStageNo == 2 ? 5.0f : 6.0f;
}

void CBoss1::Update(CEnemyShot* shot, int smax)
{
	if (!GetShow())
		return;
	//1フレームの時間。通常は16ms
	float deltaTime = CUtilities::GetFrameSecond();

	if (m_bDestroyed)
	{
		m_Pos.y -= 3.0f * deltaTime;
		m_Rot.y += MOF_ToRadian(150 * deltaTime);
		m_Rot.z += MOF_ToRadian(200 * deltaTime);
		m_matWorld.RotationZXY(m_Rot);
		m_matWorld.SetTranslation(m_Pos);
		if (m_Pos.y < -20)
		{
			m_bShow = false;
			m_bDestroyedAndRemoved = true;
		}
		if (m_ExplosionInterval == 0)
		{
			auto eff = CEffectManager::Spawn(EFFECT_EXPLOSION);
			if (eff)
			{
				eff->SetPosition(m_Pos + BACK_VECTOR3 * 6);
				eff->SetSize(4);
			}
			m_ExplosionInterval = 30;
		}
		m_ExplosionInterval--;
		return;
	}
	if (m_AnimTime < g_BossAnimPosY1[1].fTime)
	{
		m_AnimTime += deltaTime;

		m_Pos.y = InterpolationAmin(m_AnimTime, g_BossAnimPosY1, 2);
		m_Pos.z = InterpolationAmin(m_AnimTime, g_BossAnimPosZ1, 2);

		m_Rot.x = InterpolationAmin(m_AnimTime, g_BossAnimRotX1, 2);
		m_Rot.z = InterpolationAmin(m_AnimTime, g_BossAnimRotZ1, 2);
	}
	else
	{
		RotateTarget();
		UpdateShield();

		//一定時間経過で種類を切り替える
		m_ChangeTime += deltaTime;
		if (m_ChangeTime > m_PatternChangeInterval)
		{
			m_CurrentShotPattern++;
			//ステージ2のボスであれな強攻撃はしない
			if (gStageNo == 2)
			{
				if (m_CurrentShotPattern >= SHOTPATTERN_EXTREMEPARALLEL)
					m_CurrentShotPattern = SHOTPATTERN_SINGLE;
			}
			else
			{
				if (m_CurrentShotPattern >= SHOTPATTERN_COUNT)
					m_CurrentShotPattern = SHOTPATTERN_SINGLE;
			}

			m_ChangeTime = 0.0f;
		}
		m_TimeElapsed += deltaTime * 4;

		//攻撃処理
		UpdateShot(shot, smax);
	}
	//直前のパターンと現在のパターンが違うときは回転
	if (m_PrevShotPattern != m_CurrentShotPattern)
		m_bFlip = true;
	if (m_bFlip)
	{
		m_FlipTime+= deltaTime;
		if (m_FlipTime > 0.25f)
		{
			m_bFlip = false;
			m_FlipTime = 0;
		}
	}

	m_PrevShotPattern = m_CurrentShotPattern;
	m_matWorld.RotationZXY(m_Rot);
	m_matWorld.SetTranslation(m_Pos);
}

void CBoss1::Damage(int dmg, const Vector3& bulletPos)
{
	if (m_ShieldTime > 0.0f)
	{
		auto eff = CEffectManager::Spawn(EFFECT_HIT);
		if (eff)
		{
			eff->SetPosition(bulletPos);
			eff->SetSize(1.0f);
		}
		return;
	}

	if (!GetShow() || m_bDestroyed)
		return;
	CScore::AddScore(1);
	m_HP -= dmg;
	if (m_HP <= 0)
	{
		m_bDestroyed = true;
		CScore::AddScore(50);
	}
}

void CBoss1::Render()
{
	if (!GetShow())
		return;
	m_Mesh.Render(m_matScale * m_matWorld);
	if (m_ShieldTime > 0.0f)
	{
		m_ShieldMesh.Render(m_matShield);
	}

	//強攻撃前の攻撃範囲の描画
	if (m_ExtremeAttackWaitTime < 2.0f && m_CurrentShotPattern == SHOTPATTERN_EXTREMEPARALLEL)
	{
		m_AttackAreaTime++;
		if (m_AttackAreaTime % 5 == 0) {
			m_bShowAttackArea = !m_bShowAttackArea;
		}

		if (m_bShowAttackArea)
		{
			CMatrix44 matWorld;
			matWorld.SetTranslation(m_Pos);
			m_AttackAreaMesh.Render(matWorld);
		}
		
	}
}

void CBoss1::RenderDebug()
{

}

CSphere CBoss1::GetSphere()
{
	if (m_ShieldTime > 0.0f)
	{
		return CSphere(m_Pos, 6.0f);
	}
	else
	{
		return CSphere(m_Pos, 3.1f);
	}
}

void CBoss1::Release()
{
	m_ShieldMesh.Release();
	m_Mesh.Release();
	m_AttackAreaMesh.Release();
}

void CBoss1::RotateTarget()
{
	if (!m_bFlip)
	{
		m_Rot.y = 0;
		return;
	}
	m_Rot.y = InterpolationAmin(m_FlipTime, g_BossFlipRotY, 5);
}

void CBoss1::UpdateShield()
{
	//ステージ3以外のボスはシールドを使用できない
	if (gStageNo != 3)
		return;

	//1フレームの時間。通常は16ms
	float deltaTime = CUtilities::GetFrameSecond();
	float hpFactor = (float)m_HP / (float)m_MaxHP;

	if (!m_bUsedShield && hpFactor < 0.5f)
	{
		m_bUsedShield = true;
		m_ShieldTime = 8.0f;
	}

	if (m_ShieldTime > 0.0f)
	{
		m_ShieldTime -= deltaTime;
		CMatrix44 matRot;
		matRot.RotationY(MOF_ToRadian(40) * deltaTime);
		m_matShield *= matRot;
	}
}

void CBoss1::ShotAllDirShot(CEnemyShot* shot, int smax, int sCnt)
{
}

void CBoss1::ShotCircleDir(CEnemyShot* shot, int smax, int sCnt)
{
	if (m_ShotWait > 0)
	{
		m_ShotWait--;
		return;
	}

	float ad = m_TargetAngleP;
	float sd = m_TargetAngleM;
	m_TargetAngleP += 11.0f;
	m_TargetAngleM -= 11.0f;
	for (int cnt = 0; cnt < sCnt; cnt++)
	{
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (newShot)
		{
			m_ShotWait = m_ShotInterval;
			float rad = MOF_ToRadian(ad);
			Vector3 vt(cos(rad), 0, sin(rad));
			newShot->SetLifeTime(4);
			newShot->SetHorming(false);
			newShot->Fire(m_Pos, vt * 0.2f);
		}
		ad += 360.0f / sCnt;
	}

	for (int cnt = 0; cnt < sCnt; cnt++)
	{
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (newShot)
		{
			m_ShotWait = m_ShotInterval;
			float rad = MOF_ToRadian(sd);
			Vector3 vt(cos(rad), 0, sin(rad));
			newShot->SetHorming(false);
			newShot->SetLifeTime(4);
			newShot->Fire(m_Pos, vt * 0.2f);
		}
		sd -= 360.0f / sCnt;
	}
}

void CBoss1::ShotSineWave(CEnemyShot* shot, int smax, int sCnt)
{
	if (m_ShotWait > 0)
	{
		m_ShotWait--;
		return;
	}
	m_ShotWait = m_ShotInterval;
	if (sCnt == 0)
	{
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (!newShot)
			return;

		Vector3 pos = m_Pos;
		Vector3 direction = m_TargetPos - pos;

		float distance = CVector3Utilities::Length(direction);
		if (distance <= 0)
			return;

		direction /= distance;
		float ad = atan2(direction.z, direction.x) + sinf(m_TimeElapsed);
		Vector3 vt(cos(ad), 0, sin(ad));
		newShot->SetLifeTime(4);

		newShot->Fire(pos, vt * 0.2f);
		newShot->SetHorming(false);
	}
	else
	{
		for (int cnt = -sCnt; cnt <= sCnt; cnt++)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (!newShot)
				continue;

			Vector3 pos = m_Pos;
			Vector3 direction = m_TargetPos - pos;

			float distance = CVector3Utilities::Length(direction);
			if (distance <= 0)
				continue;

			direction /= distance;
			float ad = atan2(direction.z, direction.x) + cnt * MOF_ToRadian(10) + sinf(m_TimeElapsed);
			Vector3 vt(cos(ad), 0, sin(ad));
			newShot->SetLifeTime(4);

			newShot->Fire(pos, vt * 0.2f);
			newShot->SetHorming(false);
		}
	}
}

void CBoss1::ShotExtremeParallel(CEnemyShot* shot, int smax, int sCnt)
{
	if (m_ShotWait > 0)
	{
		m_ShotWait--;
		return;
	}
	m_ShotWait = m_ShotInterval;
	float deltaTime = CUtilities::GetFrameSecond();
	for (int cnt = -sCnt; cnt <= sCnt; cnt++)
	{
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (!newShot)
			continue;

		Vector3 pos = m_Pos;
		Vector3 direction = m_TargetPos - pos;

		float distance = CVector3Utilities::Length(direction);
		if (distance <= 0)
			continue;

		newShot->SetLifeTime(4);

		newShot->Fire(pos + RIGHT_VECTOR3 * cnt, BACK_VECTOR3 * deltaTime * 40);
		newShot->SetHorming(false);
	}
}

void CBoss1::ShotParallel(CEnemyShot* shot, int smax, int sCnt)
{
	if (m_ShotWait > 0)
	{
		m_ShotWait--;
		return;
	}
	m_ShotWait = m_ShotInterval;
	float deltaTime = CUtilities::GetFrameSecond();
	for (int cnt = -sCnt; cnt <= sCnt; cnt++)
	{
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (!newShot)
			continue;

		Vector3 pos = m_Pos;
		Vector3 direction = m_TargetPos - pos;

		float distance = CVector3Utilities::Length(direction);
		if (distance <= 0)
			continue;

		newShot->SetLifeTime(4);

		newShot->Fire(pos + RIGHT_VECTOR3 * cnt * 2, BACK_VECTOR3 * deltaTime * 15);
		newShot->SetHorming(false);
	}
}

void CBoss1::Shot(CEnemyShot* shot, int smax, int sCnt, bool horming)
{
	if (m_ShotWait > 0)
	{
		m_ShotWait--;
		return;
	}
	m_ShotWait = m_ShotInterval;
	if (sCnt == 0) 
	{
		CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
		if (!newShot)
			return;

		Vector3 pos = m_Pos;
		Vector3 direction = m_TargetPos - pos;

		float distance = CVector3Utilities::Length(direction);
		if (distance <= 0)
			return;

		direction /= distance;
		float ad = atan2(direction.z, direction.x);
		Vector3 vt(cos(ad), 0, sin(ad));
		newShot->SetLifeTime(horming ? 2 : 4);

		newShot->Fire(pos, vt * 0.3f);
		newShot->SetHorming(horming);
	}
	else
	{
		for (int cnt = -sCnt; cnt <= sCnt; cnt++)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (!newShot)
				continue;

			Vector3 pos = m_Pos;
			Vector3 direction = m_TargetPos - pos;

			float distance = CVector3Utilities::Length(direction);
			if (distance <= 0)
				continue;

			direction /= distance;
			float ad = atan2(direction.z, direction.x) + cnt * MOF_ToRadian(10);
			Vector3 vt(cos(ad), 0, sin(ad));
			newShot->SetLifeTime(horming ? 2 : 4);

			newShot->Fire(pos, vt * 0.2f);
			newShot->SetHorming(horming);
		}
	}
}

void CBoss1::SetMeshScale(float value)
{
	m_matScale.Scaling(value);
}


