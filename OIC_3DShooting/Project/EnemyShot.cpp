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
	//�z�[�~���O�������܂��̓z�[�~���O�J�n�O�̏ꍇ�͒ʏ�ʂ�ړ�������B
	if (!m_bHorming || m_HormingWaitTime < 0.25f) {
		m_Pos += m_Spd;
		return;
	}
	//�v���C���[�̑��Έʒu
	Vector3 vec = m_PlayerPos - m_Pos;
	Vector3 nrm = ZERO_VECTOR3;
	//�����ɂ���Ēe�̑��x���ς��Ȃ��悤�ɒP�ʃx�N�g���ɂ���B
	CVector3Utilities::Normal(vec, nrm);
	//����
	m_Spd += nrm * CUtilities::GetFrameSecond() * 0.1f;
	//�����ɉ������Ȃ��悤�Ɍ���������B
	m_Spd *= 0.985f;
	//�ړ�
	m_Pos += m_Spd;
}
