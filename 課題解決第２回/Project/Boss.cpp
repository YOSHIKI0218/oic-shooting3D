#include "Boss.h"

// �{�X�̓o��A�j���[�V����

// ��O���牜�։�]���Ȃ���o��
ANIM_DATA g_BossAnimPosZ[2] = {
	{ 0.0f,-FIELD_HALF_Z,EASE_LINEAR },
	{ 2.0f,2.0f,EASE_OUT_SINE },
};
ANIM_DATA g_BossAnimRotZ[2] = {
	{ 0.0f,MOF_MATH_2PI,EASE_LINEAR },
	{ 2.0f,0.0f,EASE_OUT_SINE },
};
// ��O���Ɍ����悤�ɉ�]
ANIM_DATA g_BossAnimRotX[2] = {
	{ 2.0f,MOF_MATH_PI,EASE_LINEAR },
	{ 4.0f,0.0f,EASE_INOUT_SINE },
};
// ���シ��
ANIM_DATA g_BossAnimPosY[2] = {
	{ 4.0f,-10.0f,EASE_LINEAR },
	{ 6.0f,-3.5f,EASE_INOUT_SINE },
};

/**
 * �R���X�g���N�^
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
 * �f�X�g���N�^
 *
 */
CBoss::~CBoss(){
}

/**
 * �ǂݍ���
 *
 */
bool CBoss::Load(){
	// �{�X�{�̂̃��b�V��
	if(!m_Mesh.Load("boss_core.mom"))
	{
		return false;
	}
	// �{�X�̃p�[�c�̃��b�V��
	if(!m_PartsMesh.Load("boss_parts.mom"))
	{
		return false;
	}
	return true;
}

/**
 * ������
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
 * �J�n
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
 * �X�V
 *
 */
void CBoss::Update(CEnemyShot* shot,int smax){
	// ��\��
	if(!GetShow())
	{
		return;
	}
	
	// �p�[�c�̐����J�E���g
	int partsCnt = GetPartsCount();

	if(m_AnimTime<g_BossAnimPosY[1].Time)
	{
		// ���Ԃ�i�߂�
		m_AnimTime += CUtilities::GetFrameSecond();

		// ��ԏ���
		m_Pos.y = InterpolationAnim(m_AnimTime,g_BossAnimPosY,2);
		m_Pos.z = InterpolationAnim(m_AnimTime,g_BossAnimPosZ,2);
		m_Rot.x = InterpolationAnim(m_AnimTime,g_BossAnimRotX,2);
		m_Rot.z = InterpolationAnim(m_AnimTime,g_BossAnimRotZ,2);
	}
	// �A�j���[�V�����I����̏���
	else
	{
		// ���@�̕���������
		Vector3 direction = m_TargetPos - (m_Pos);
		float dr = atan2(direction.x, direction.z) + MOF_MATH_PI;
		float r = fmodf((dr - m_Rot.y) + MOF_MATH_PI, MOF_MATH_2PI);
		m_Rot.y += ((0 < r) ? r - MOF_MATH_PI : r + MOF_MATH_PI) * 0.1f;

		// ���j����邲�Ƃɉ�]
		m_Rot.z += (MOF_ToRadian((3 - partsCnt)*120.0f) - m_Rot.z)*0.05f;

		// �S�Ẵp�[�c���j�󂳂ꂽ�Ƃ�
		if(partsCnt == 0)
		{
			// Y���W��0�ɋ߂Â���
			m_Pos.y *= 0.95f;

			// �e�̔���
			if(m_ShotWait <= 0)
			{
				// �e�̓������ː�
				int sCnt = 16;
				// �e�̔��ˊp�x
				float ad = ((float)rand() / RAND_MAX)*360.0f / sCnt;
				for(int cnt = 0; cnt < sCnt; cnt++)
				{
					CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot,smax);
					if(newShot)
					{
						m_ShotWait = 20;
						// �p�x�̕����ɒe��i�߂�
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

	// �p�[�c�̍X�V
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		CEnemy& parts = m_Parts[i];
		
		float angle = -(MOF_MATH_2PI * i / BOSS_PARTS_MAX);
		// �ʒu
		CVector3 p(0.0f,3.5f,0.0f);
		p.RotationZ(angle);
		p *= m_matWorld;
		parts.SetPosition(p);
		// ��]
		CVector3 r(m_Rot);
		r.z += angle;
		parts.SetRotation(r);

		// �e�̔���
		if(g_BossAnimPosY[1].Time <= m_AnimTime && i == BOSS_PARTS_MAX - partsCnt)
		{
			parts.SetTargetPos(m_TargetPos);
			parts.Update(shot,smax);
		}
	}
}

/**
 * �_���[�W����
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
 * �`��
 *
 */
void CBoss::Render(){
	if(!GetShow())
	{
		return;
	}

	m_Mesh.Render(m_matWorld);

	// �p�[�c�̕`��
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].Render();
	}
}

/**
 * �f�o�b�O�`��
 *
 */
void CBoss::RenderDebug(){
	// ��\��
	if(!GetShow())
	{
		return;
	}
	// �����蔻��̕\��
	CGraphicsUtilities::RenderSphere(GetSphere(),Vector4(1,0,0,0.3f));
	
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		m_Parts[i].RenderDebug();
	}
}

/**
 * �\�����̃p�[�c�̐�
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
 * ���
 *
 */
void CBoss::Release(){
	m_Mesh.Release();
	m_PartsMesh.Release();
}
