#include "EnemyShot.h"
#include "GameDefine.h"

CEnemyShot::CEnemyShot() :
	m_pMesh(NULL),
	m_Pos(0, 0, 0),
	m_Spd(0, 0, 0),
	m_bShow(false),
	m_RotY(0),
	m_LifeTime(0),
	m_PlayerPos(0, 0, 0),
	m_bHorming(false),
	m_HormingWaitTime(0.0f)
{
}

CEnemyShot::~CEnemyShot()
{
}

void CEnemyShot::Initalize()
{
	m_Pos = ZERO_VECTOR3;
	m_Spd = ZERO_VECTOR3;
	m_bShow = false;
}

void CEnemyShot::Fire(const Vector3& p, const Vector3& s)
{
	m_Pos = p;
	m_Spd = s;
	m_bShow = true;
	m_RotY = 0;
	//m_LifeTime = 240;
	m_HormingWaitTime = 0.0f;
}

void CEnemyShot::Update()
{
	if (!m_bShow)
		return;

	if (m_LifeTime <= 0.0f)
		m_bShow = false;
	m_HormingWaitTime += CUtilities::GetFrameSecond();
	Move();
	float inflate = 1.0f;
	float halfZ = CGameDefine::GetFiledHalfZ();
	if (m_Pos.x < -halfZ - inflate || halfZ + inflate < m_Pos.x || m_Pos.z < -halfZ - inflate || halfZ + inflate < m_Pos.z)
	{
		m_bShow = false;
	}

	//Rotate a bullet
	m_RotY += MOF_ToRadian(10);
	m_LifeTime -= CUtilities::GetFrameSecond();
}

void CEnemyShot::Render()
{
	if (!m_bShow)
		return;
	CMatrix44 matWorld;
	matWorld.RotationY(m_RotY);
	matWorld.SetTranslation(m_Pos);
	m_pMesh->Render(matWorld);
}

void CEnemyShot::RenderDebug()
{
	if (!m_bShow)
		return;
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));
}

CEnemyShot* CEnemyShot::FindAvailableShot(CEnemyShot* shot, int smax)
{
	for (int i = 0; i < smax; i++)
	{
		if (!shot[i].GetShow())
		{
			return &shot[i];
		}
	}
	return NULL;
}

void CEnemyShot::Move()
{
	//ホーミングが無効またはホーミング開始前の場合は通常通り移動させる。
	if (!m_bHorming || m_HormingWaitTime < 0.25f) {
		m_Pos += m_Spd;
		return;
	}
	//プレイヤーの相対位置
	Vector3 vec = m_PlayerPos - m_Pos;
	Vector3 nrm = ZERO_VECTOR3;
	//距離によって弾の速度が変わらないように単位ベクトルにする。
	CVector3Utilities::Normal(vec, nrm);
	//加速
	m_Spd += nrm * CUtilities::GetFrameSecond() * 0.1f;
	//無限に加速しないように減衰させる。
	m_Spd *= 0.985f;
	//移動
	m_Pos += m_Spd;
}
