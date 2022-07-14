#include "Player.h"

/**
 * �R���X�g���N�^
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_Rot(0.0f,0.0f,0.0f),
m_bDead(false),
m_ShotMesh(),
m_ShotArray(),
m_ShotWait(){
}

/**
 * �f�X�g���N�^
 */
CPlayer::~CPlayer(){
}

/**
 * �ǂݍ���
 */
bool CPlayer::Load(void){
	// ���b�V���̓ǂݍ���
	if(!m_Mesh.Load("player.mom"))
	{
		return false;
	}
	// �e�̃��b�V��
	if(!m_ShotMesh.Load("pshot.mom"))
	{
		return false;
	}
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].SetMesh(&m_ShotMesh);
	}
	return true;
}

/**
 * ������
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0.0f,0.0f,-FIELD_HALF_Z+2.0f);
	m_Rot = Vector3(0.0f,0.0f,0.0f);
	m_bDead = false;
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Initialize();
	}
	m_TurnFlag = false;
	m_TurnTime = 0.0f;
	m_TurnPos = Vector3(0.0f,0.0f,0.0f);
}

/**
 * �X�V
 */
void CPlayer::Update(bool HorizonMode){
	if(m_bDead)
	{
		return;
	}
	// ��]����
	float Roll = 0;
	if(!m_TurnFlag)
	{
		// �L�[�{�[�h�ł̈ړ�
		if(!HorizonMode)
		{
			//�c�X�N���[�����̈ړ�
			if(g_pInput->IsKeyHold(MOFKEY_LEFT))
			{
				m_Pos.x = max(m_Pos.x - PLAYER_SPEED,-FIELD_HALF_X);
				Roll -= MOF_MATH_PI;
			}
			if(g_pInput->IsKeyHold(MOFKEY_RIGHT))
			{
				m_Pos.x = min(m_Pos.x + PLAYER_SPEED,FIELD_HALF_X);
				Roll += MOF_MATH_PI;
			}
			if(g_pInput->IsKeyHold(MOFKEY_UP))
			{
				m_Pos.z = min(m_Pos.z + PLAYER_SPEED,FIELD_HALF_Z);
			}
			if(g_pInput->IsKeyHold(MOFKEY_DOWN))
			{
				m_Pos.z = max(m_Pos.z - PLAYER_SPEED,-FIELD_HALF_Z);
			}
		}
		else
		{
			//�ۑ�R�|�P�@���X�N���[�����̈ړ�
			//�ړ����������E�L�[��Z�����A�㉺�L�[��Y�����ɕύX����B
			//�v���C���[�̉�]�����͏㉺�L�[�̓��͂Ŏ��s����



		}
		
		//�ۑ�R�|�Q�@���X�N���[���ւ̕��
		//�c�X�N���[������m_Pos.y�A���X�N���[������m_Pos.x��0.1�̑��x��0�ɋ߂Â��Ă���



		//���Ԃ�J�n
		if(g_pInput->IsKeyPush(MOFKEY_LSHIFT))
		{
			m_TurnFlag = true;	//���Ԃ蒆�̃t���O
			m_TurnPos = m_Pos;	//���Ԃ���J�n�������W
			m_TurnTime = 0.0f;	//���Ԃ���J�n���Ă���̌o�ߎ���
		}
	}
	//���Ԃ�
	else
	{
		//���Ԍo�߂ƃC�[�W���O
		m_TurnTime += CUtilities::GetFrameSecond();
		float t = MOF_LINSTEP(0.0f,8,m_TurnTime);
		t = sinf(t * MOF_MATH_HALFPI);

		//�ۑ�P�@���Ԃ�̓���
		//�c�ɒ��Ԃ肳���邽�߁Am_Rot.x������(t)0�`1�̊Ԃ�1��]��360�x�i�Q�΁j��]������
		//���Ԃ�̒��S�̓v���C���[�̒��Ԃ�J�n���W�̏��1.5�̈ʒu�Ƃ��āA
		//�v���C���[�̍��W�͔��a1.5��X����m_Rot.x��]�������W�ɂȂ�


		m_Rot.x = -MOF_MATH_2PI * t;
		//m_Pos += CVector3(0,sin(),sin())
		//m_Pos.y = MOF_LERP(m_Pos.y + 0.75, cosf(MOF_MATH_2PI), t);
		//m_Pos.z = MOF_LERP(m_Pos.z, cosf(-MOF_MATH_QUADPI), t);
		
		CVector3 tmpPos = Vector3(0, -1.5, 0);
		tmpPos.RotationX(m_Rot.x);
		tmpPos.y += 1.5f;
		m_Pos = m_TurnPos + tmpPos;

		//���Ԃ�I��
		if(1.0f <= t)
		{
			 
			m_TurnFlag = false;
			
		}
	}

	// ��]
	float RotSpeed = MOF_ToRadian(10);
	if(Roll == 0)
	{
		RotSpeed = min(abs(m_Rot.z)*0.1f,RotSpeed);
	}
	if(abs(m_Rot.z) <= RotSpeed || signbit(m_Rot.z) != signbit(Roll))
	{
		m_Rot.z += Roll;
	}
	m_Rot.z -= copysignf(min(RotSpeed,abs(m_Rot.z)),m_Rot.z);
	
	// �e�̔���
	if(m_ShotWait <= 0)
	{
		if(g_pInput->IsKeyHold(MOFKEY_SPACE) && !m_TurnFlag)
		{
			for(int cnt = 0; cnt < 2; cnt++)
			{
				for(int i = 0; i < PLAYERSHOT_COUNT; i++)
				{
					if(m_ShotArray[i].GetShow())
					{
						continue;
					}
					CVector3 ShotPos(0.4f * (cnt * 2 - 1), 0, 0);
					ShotPos.RotationZ(m_Rot.z);
					ShotPos += m_Pos;
					m_ShotWait = PLAYERSHOT_WAIT;
					m_ShotArray[i].Fire(ShotPos);
					break;
				}
			}
		}
	}
	else
	{
		m_ShotWait--;
	}
	// �e�̍X�V
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Update();
	}
}

/**
 * �G�Ƃ̓����蔻��
 * �����̓G�ɑ΂��ē����蔻������s����B
 *
 * ����
 * [in]			ene				������s���G
 */
void CPlayer::CollisionEnemy(CEnemy& ene){
	if(!ene.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere es = ene.GetSphere();
	if(ps.CollisionSphere(es))
	{
		m_bDead = true;
	}
	// �e�Ƃ̔���
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if(!m_ShotArray[i].GetShow())
		{
			continue;
		}
		CSphere ss = m_ShotArray[i].GetSphere();
		if(ss.CollisionSphere(es))
		{
			ene.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
}

/**
 * �G�e�Ƃ̓����蔻��
 * �����̓G�e�ɑ΂��ē����蔻������s����B
 *
 * ����
 * [in]			shot			������s���G�e
 */
void CPlayer::CollisionEnemyShot(CEnemyShot& shot){
	CSphere ps = GetSphere();
	if(!shot.GetShow())
	{
		return;
	}
	CSphere ss = shot.GetSphere();
	if(ss.CollisionSphere(ps))
	{
		m_bDead = true;
		shot.SetShow(false);
	}
}

/**
 * �����蔻��
 * �����̃{�X�ɑ΂��ē����蔻������s����B
 *
 * ����
 * [in]			boss			������s���{�X
 */
void CPlayer::CollisionBoss(CBoss& boss){
	if(!boss.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere bs = boss.GetSphere();
	if(ps.CollisionSphere(bs))
	{
		m_bDead = true;
	}
	// �e�Ƃ̔���
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if(!m_ShotArray[i].GetShow())
		{
			continue;
		}
		CSphere ss = m_ShotArray[i].GetSphere();
		if(ss.CollisionSphere(bs))
		{
			boss.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
	// �p�[�c�Ƃ̔���
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		CollisionEnemy(boss.GetParts(i));
	}
}

/**
 * �`��
 */
void CPlayer::Render(void){
	if(m_bDead)
	{
		return;
	}
	// ���[���h�s��쐬
	CMatrix44 matWorld;
	matWorld.RotationZXY(m_Rot);
	matWorld.SetTranslation(m_Pos);
	// ���b�V���̕`��
	m_Mesh.Render(matWorld);

	// �e�̕`��
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Render();
	}
}

/**
 * �f�o�b�O�`��
 */
void CPlayer::RenderDebug(void){
	// �����蔻��̕\��
	CGraphicsUtilities::RenderSphere(GetSphere(),Vector4(0,1,0,0.3f));
	// �e�̕`��
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].RenderDebug();
	}
}

/**
 * �f�o�b�O�����`��
 */
void CPlayer::RenderDebugText(void){
	// �ʒu�̕`��
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"�v���C���[�ʒu X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
}

/**
 * ���
 */
void CPlayer::Release(void){
	m_Mesh.Release();
	m_ShotMesh.Release();
}