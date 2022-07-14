#include	"Particle.h"

/**
* �R���X�g���N�^
*
*/
CParticle::CParticle() :
m_Time(0.0f) ,
m_State() ,
m_InitializeState() ,
m_UpdateBehavior() ,
m_ParameterBehavior() ,
m_EnvironmentBehavior() ,
m_pTexture(NULL) ,
m_pMesh(NULL),
m_ViewPosition() {
}

/**
* �f�X�g���N�^
*
*/
CParticle::~CParticle(){
	Release();
}

/**
* ������񂩂�p�[�e�B�N��������������
*
 * ����
 * [in]			info			�������
*/
void CParticle::Initialize(CREATEINFO& info){
	m_Time = 0;
	m_InitializeState = m_State = info.State;
	m_UpdateBehavior = info.UpdateBehavior;
	m_ParameterBehavior = info.ParameterBehavior;
	m_EnvironmentBehavior = info.EnvironmentBehavior;
	m_pTexture = info.pTexture;
	m_pMesh = info.pMesh;
}

/**
* �X�V
*
*/
void CParticle::Update(void){
	//���ݎ��Ԃ̍X�V
	m_Time++;
	//�I������
	if (m_Time >= m_ParameterBehavior.Existence)
	{
		return;
	}

	//�����ɂ��킹�čX�V
	m_State.Position += m_UpdateBehavior.Move + m_EnvironmentBehavior.Gravity * m_Time;
	m_State.Scale += m_UpdateBehavior.Scale;
	m_State.Rotation += m_UpdateBehavior.Rotation;
	m_State.Color += m_UpdateBehavior.Color;

	//�s��̍X�V
	m_State.World.RotationZXY(m_State.Rotation);
	m_State.World.Scaling(m_State.Scale, m_State.World);
	m_State.World.SetTranslation(m_State.Position);
}

/**
* �`��
*
*/
void CParticle::Render(void){
	if (m_Time >= m_ParameterBehavior.Existence)
	{
		return;
	}
	if (m_State.Color.w <= 0.0f)
	{
		return;
	}
	g_pGraphics->SetBlending(m_ParameterBehavior.Blending);
	//�`��p�}�g���N�X�̐錾
	CMatrix44 rMat = m_State.World;
	//�r���{�[�h�}�g���N�X�����[���h�}�g���N�X�ɓK�p����
	if (m_ParameterBehavior.Flag & BILLBOARD)
	{
		//�J�������擾����
		LPCamera pCam = CGraphicsUtilities::GetCamera();
		rMat.Multiply3x3(pCam->GetBillBoardMatrix());
	}
	if (m_pMesh)
	{
		m_pMesh->Render(rMat, m_State.Color);
	}
	if (m_pTexture)
	{
		m_pTexture->Render(rMat, m_State.Color.ToU32Color());
	}
}

/**
* ���
*
*/
void CParticle::Release(void){
}