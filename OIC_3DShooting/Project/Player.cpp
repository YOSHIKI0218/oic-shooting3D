#include "Player.h"
#include	"HealItem.h"
#include	"EffectManager.h"

//�X�e�[�W�ԍ�(���̂�GameScene.cpp)
extern int						gStageNo;

//�A�C�e���z��(���̂�GameScene.cpp)
extern CItemBase* gItemArray[];

int gDefaultRotateSpeed;
/**
 * �R���X�g���N�^
 */
CPlayer::CPlayer() :
m_Mesh(),
m_vPos(0.0f,0.0f,0.0f),
m_fRotZ(0.0f),
m_fRoll(0.0f),
m_bDead(false),
m_HP(0),
m_MaxHP(0),
m_RoateSpeed(0),
m_ShotWait(0),
m_Mode()
{
}

/**
 * �f�X�g���N�^
 */
CPlayer::~CPlayer(){
	
}

/**
 * �ǂݍ���
 */
void CPlayer::Load(void){
	// ���b�V���̓ǂݍ���
	m_Mesh.Load("player.mom");	

	m_ShotMesh.Load("pshot.mom");
	for (int i = 0; i < PLAYER_SHOT_COUNT; i++)	{
		m_ShotArray[i].SetMesh(&m_ShotMesh);
	}
	gDefaultRotateSpeed = CGameDefine::GetPlayerRotationSpeed();
}

/**
 * ������
 */
void CPlayer::Initialize(void){
	// Initialize Start Position
	m_vPos = Vector3(0.0f, 0.0f, -CGameDefine::GetFiledHalfZ() + 2.0f);
	m_fRotZ = 0.0f;
	m_fRoll = 0.0f;
	m_Mode = Mode_Normal;
	m_bDead = false;
	

	for (int i = 0; i < PLAYER_SHOT_COUNT; i++) {
		m_ShotArray[i].Initialize();
	}

}

/**
 * �X�V
 */
void CPlayer::Update(void){
	if (m_bDead)
		return;
	m_fRoll = 0.0f;
	m_RoateSpeed = gDefaultRotateSpeed;

	UpdateMove();
	UpdateRotate(m_RoateSpeed);
	UpdateShot();

	if (m_HP <= 0)
	{
		m_bDead = true;
	}
}

/**
 * �`��
 */
void CPlayer::Render(void)
{
	if (m_bDead)
		return;
	// ���[���h�s��쐬
	CMatrix44 matWorld;

	matWorld.RotationZ(m_fRotZ);
	matWorld.SetTranslation(m_vPos);

	// ���b�V���̕`��
	m_Mesh.Render(matWorld);

	for (int i = 0; i < PLAYER_SHOT_COUNT; i++) {
		m_ShotArray[i].Render();
	}
}

/**
 * �f�o�b�O�����`��
 */
void CPlayer::RenderDebug3D(void){
	// �ʒu�̕`��
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"�v���C���[�ʒu X : %.1f , Y : %.1f , Z : %.1f", m_vPos.x, m_vPos.y, m_vPos.z);

	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));

	for (int i = 0; i < PLAYER_SHOT_COUNT; i++)
	{
		m_ShotArray[i].RenderDebug2D();
	}
}

/**
 * ���
 */
void CPlayer::Release(void){
	m_Mesh.Release();
	m_ShotMesh.Release();
}

const CVector3 CPlayer::GetPosition()
{
	return m_vPos; 
}


void CPlayer::UpdateMove()
{
	float speed = PLAYER_SPEED;

	// boost ?
	if (g_pInput->IsKeyHold(MOFKEY_LSHIFT))
	{
		speed *= 3;
		m_RoateSpeed *= 3;
	}
	float halfx = CGameDefine::GetFiledHalfX();
	float halfy = CGameDefine::GetFiledHalfY();
	float halfz = CGameDefine::GetFiledHalfZ();

	// �L�[�{�[�h�ł̈ړ�
	if (g_pInput->IsKeyHold(MOFKEY_A))
	{
		m_vPos.x = max(m_vPos.x - speed, -halfx);
		m_fRoll -= MOF_MATH_PI;
	}
	if (g_pInput->IsKeyHold(MOFKEY_D))
	{
		m_vPos.x = min(m_vPos.x + speed, halfx);
		m_fRoll += MOF_MATH_PI;
	}
	if (g_pInput->IsKeyHold(MOFKEY_W))
	{
		m_vPos.z = min(m_vPos.z + PLAYER_SPEED, halfz);

	}
	if (g_pInput->IsKeyHold(MOFKEY_S))
	{
		m_vPos.z = max(m_vPos.z - PLAYER_SPEED, -halfz);
	}
}

void CPlayer::UpdateRotate(int speed)
{
	// ��]
	{
		float RotSpeed = MOF_ToRadian(speed);
		if (m_fRoll == 0)
		{
			RotSpeed = min(abs(m_fRotZ) * 0.1f, RotSpeed);
		}
		if (abs(m_fRotZ) <= RotSpeed || signbit(m_fRotZ) != signbit(m_fRoll))
		{
			m_fRotZ += m_fRoll;
		}

		m_fRotZ -= copysignf(min(RotSpeed, abs(m_fRotZ)), m_fRotZ);
	}
}

void CPlayer::UpdatePlayerShot_Normal()
{
	if (m_ShotWait <= 0) {
		if (g_pInput->IsKeyHold(MOFKEY_SPACE)) {
			for (int cnt = 0; cnt < 2; cnt++) {
				for (int i = 0; i < PLAYER_SHOT_COUNT; i++) {

					if (m_ShotArray[i].GetShow()) { continue; }
					CVector3 shotpos(0.4f * (cnt * 2 - 1), 0, 0);
					shotpos.RotationZ(m_fRotZ);
					shotpos += m_vPos;
					m_ShotWait = PLAYER_SHOT_WAIT;
					m_ShotArray[i].Fire(shotpos);
					break;
				}
			}
		}
	}
	else {
		m_ShotWait--;
	}

	for (int i = 0; i < PLAYER_SHOT_COUNT; i++) {
		m_ShotArray[i].Update();
	}
}

void CPlayer::UpdatePlayerShot_Laser()
{
	if (m_ShotWait <= 0)
	{
		if (g_pInput->IsKeyHold(MOFKEY_SPACE)) {
			for (int i = 0; i < PLAYER_SHOT_COUNT; i++) {

				if (m_ShotArray[i].GetShow()) { continue; }
				m_ShotArray[i].Fire(m_vPos);
				m_ShotWait = PLAYER_SHOT_WAIT / 2;
				break;
			}
		}
	}
	else {
		m_ShotWait--;
	}

	for (int i = 0; i < PLAYER_SHOT_COUNT; i++) {
		m_ShotArray[i].Update();
	}
}

void CPlayer::UpdateShot()
{
	if (gStageNo >= 2)
	{
		if (g_pInput->IsKeyPush(MOFKEY_1))
		{
			m_Mode = Mode_Normal;
		}
		else if (g_pInput->IsKeyPush(MOFKEY_2))
		{
			m_Mode = Mode_Laser;
		}
	}

	switch (m_Mode)
	{
	case Mode_Normal:
		UpdatePlayerShot_Normal();
		break;
	case Mode_Laser:
		UpdatePlayerShot_Laser();
		break;
	default:
		break;
	}
}

void CPlayer::CollisionEnemy(CEnemy& ene)
{
	if (!ene.GetShow())
	{
		return;
	}

	CSphere ps = GetSphere();
	CSphere es = ene.GetSphere();
	if (ps.CollisionSphere(es))
	{
		Damage(1);
	}

	//Collide bullets
	for (int i = 0; i < PLAYER_SHOT_COUNT; i++)
	{
		if (!m_ShotArray[i].GetShow())
		{
			continue;
		}

		CSphere ss = m_ShotArray[i].GetSphere();
		if (ss.CollisionSphere(es))
		{
			ene.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
}

void CPlayer::ConllisionBoss(CBoss& boss)
{
	if (m_bDead)
		return;
	if (!boss.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere bs = boss.GetSphere();
	if (ps.CollisionSphere(bs))
	{
		Damage(1);
	}

	for (int i = 0; i < PLAYER_SHOT_COUNT; i++)
	{
		if (!m_ShotArray[i].GetShow())
			continue;
		CSphere ss = m_ShotArray[i].GetSphere();
		if (ss.CollisionSphere(bs))
		{
			boss.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}

	for (int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		CollisionEnemy(boss.GetParts(i));
	}
}

void CPlayer::CollisioEnemyShot(CEnemyShot& shot)
{
	if (m_bDead)
		return;
	CSphere ps = GetSphere();
	if (!shot.GetShow())
	{
		return;
	}
	CSphere ss = shot.GetSphere();
	if (ss.CollisionSphere(ps))
	{
		Damage(1);
		shot.SetShow(false);
	}
}

void CPlayer::CollisionBoss1(CBoss1& boss)
{
	if (m_bDead)
		return;
	if (!boss.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere bs = boss.GetSphere();
	if (ps.CollisionSphere(bs))
	{
		Damage(1);
	}

	for (int i = 0; i < PLAYER_SHOT_COUNT; i++)
	{
		if (!m_ShotArray[i].GetShow())
			continue;
		CSphere ss = m_ShotArray[i].GetSphere();
		if (ss.CollisionSphere(bs))
		{
			boss.Damage(1, m_ShotArray[i].GetPosition());
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
}

void CPlayer::SetMaxHP(int value, bool overrideMaxHp)
{
	m_HP = value;
	if(overrideMaxHp)
	m_MaxHP = value;
}

void CPlayer::Damage(int dmg)
{
	m_HP -= dmg;
	auto eff = CEffectManager::Spawn(EFFECT_HIT);
	if (eff)
	{
		eff->SetPosition(m_vPos + UP_VECTOR3);
		eff->SetSize(1.0f);
	}
}

void CPlayer::CollisionItems()
{
	//�����̋�
	CSphere ps = GetSphere();
	//���ׂẴA�C�e���Ƃ̔���
	for (int i = 0; i < 1; i++)
	{
		//gItemArray[i]�����݂��A�\������Ă��邩�H
		if (gItemArray[i] && gItemArray[i]->GetShow())
		{
			//�A�C�e���̋�
			CSphere bs = gItemArray[i]->GetSphere();
			//����������A�C�e���̎�ނ��Ƃ̏���
			if (ps.CollisionSphere(bs))
			{
				switch (gItemArray[i]->GetType())
				{
				case ITEMTYPE_HEAL:
					m_HP = m_MaxHP;
					break;
				default:
					break;
				}
				gItemArray[i]->SetShow(false);
			}
		}
	}
}

