#include "EffectManager.h"


/**
 * �R���X�g���N�^
 */
CEffectManager::CEffectManager(){
	//������
	for(int i = 0; i < EMITTERTYPE_MAX; i++)
	{
		m_LifeTime[i] = 0;
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[i];
		cinfo.ParameterBehavior.Existence.SetDefaultParameter(0);
		cinfo.ParameterBehavior.Blending = BLEND_NORMAL;
		cinfo.ParameterBehavior.Flag = 0;
		cinfo.ParameterBehavior.pTexture = NULL;
		cinfo.ParameterBehavior.pMesh = NULL;
		cinfo.CreateBehavior.Time = 0;
		cinfo.CreateBehavior.Count = 0;
	}
}

/**
 * �f�X�g���N�^
 */
CEffectManager::~CEffectManager(){
	Release();
}

/**
 * ���\�[�X�̓ǂݍ���
 */
bool CEffectManager::Load(){
	if(!m_BlueFlash.Load("BlueFlash.png"))
	{
		return false;
	}
	if(!m_WhiteFlash.Load("WhiteFlash.png"))
	{
		return false;
	}
	if(!m_Smoke.Load("Smoke.png"))
	{
		return false;
	}
	if(!m_Fire.Load("Fire.png"))
	{
		return false;
	}
	if(!m_RedFlash.Load("RedFlash.png"))
	{
		return false;
	}
	if(!m_PlayerMesh.Load("player.mom"))
	{
		return false;
	}
	m_PlayerMesh.GetGeometry(0)->GetMaterial()->SetBlending(BLEND_USER);
	if(!m_Particle.Load("Particle.png"))
	{
		return false;
	}
		
	InitCreateInfo();
	return true;
}

/**
 * �ۑ�@�G�~�b�^�[�̓���p�����[�^�[�ݒ�
 */
void CEffectManager::InitCreateInfo(){
	
	//�v���C���[�e���˃G�t�F�N�g
	{
		m_LifeTime[EMITTER_SHOT] = 1;	//�G�~�b�^�[�̎���
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[EMITTER_SHOT];

		cinfo.UpdateBehavior.Color.SetDefaultParameter(CVector4(0,0,0,-1.0f/7));
		cinfo.UpdateBehavior.Scale.SetDefaultParameter(CVector3(-1.0f / 7,-1.0f/ 7,-1.0f/ 7));
		cinfo.UpdateBehavior.Move.SetParameter(CVector3(-0.1f,-0.1f,-0.1f),CVector3(0.1f,0.1f,0.1f));

		cinfo.ParameterBehavior.Existence.SetDefaultParameter(7);
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.ParameterBehavior.pTexture = &m_BlueFlash;
		cinfo.ParameterBehavior.pMesh = NULL;

		cinfo.CreateBehavior.Position.SetDefaultParameter(CVector3(0,0,0.5f));
		cinfo.CreateBehavior.Color.SetDefaultParameter(CVector4(1,1,1,1));
		cinfo.CreateBehavior.Scale.SetDefaultParameter(CVector3(1.0f,1.0f,1.0f));
		cinfo.CreateBehavior.Time = 0;
		cinfo.CreateBehavior.Count = 4;
	}

	//�ۑ�P�@�v���C���[�e�q�b�g�G�t�F�N�g
	{
		m_LifeTime[EMITTER_HIT] = 1;	//�G�~�b�^�[�̎���
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[EMITTER_HIT];

		cinfo.UpdateBehavior.Color.SetDefaultParameter(CVector4(0,0,0,-1.0f/7));
		cinfo.UpdateBehavior.Scale.SetDefaultParameter(CVector3(-1.0f / 100,-1.0f/ 100,-1.0f/ 100));
		cinfo.UpdateBehavior.Move.SetParameter(CVector3(-0.1f,-0.1f,-0.1f),CVector3(0.1f,0.1f,0.1f));

		cinfo.ParameterBehavior.Existence.SetDefaultParameter(7);
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.ParameterBehavior.pTexture = &m_BlueFlash;
		cinfo.ParameterBehavior.pMesh = NULL;

		cinfo.CreateBehavior.Position.SetDefaultParameter(CVector3(0,0,-1.0f));
		cinfo.CreateBehavior.Color.SetDefaultParameter(CVector4(1,1,1,1));
		cinfo.CreateBehavior.Scale.SetDefaultParameter(CVector3(2.0f,2.0f,2.0f));
		cinfo.CreateBehavior.Time = 0;
		cinfo.CreateBehavior.Count = 1;
	}
	
	//�ۑ�Q�@�����G�t�F�N�g(�t���b�V��)
	{
		m_LifeTime[EMITTER_EXPLOTION_FLASH] = 1;	//�G�~�b�^�[�̎���
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[EMITTER_EXPLOTION_FLASH];

		cinfo.UpdateBehavior.Color.SetDefaultParameter(CVector4(0, 0, 0, -1.0f / 7));
		cinfo.UpdateBehavior.Scale.SetDefaultParameter(CVector3(1.0f / 7, 1.0f / 7, 1.0f / 7));
		cinfo.UpdateBehavior.Move.SetParameter(CVector3(-0.1f, -0.1f, -0.1f), CVector3(0.1f, 0.1f, 0.1f));

		cinfo.ParameterBehavior.Existence.SetDefaultParameter(7);
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.ParameterBehavior.pTexture = &m_BlueFlash;
		cinfo.ParameterBehavior.pMesh = NULL;

		cinfo.CreateBehavior.Position.SetDefaultParameter(CVector3(0, 0, -1.0f));
		cinfo.CreateBehavior.Color.SetDefaultParameter(CVector4(1, 1, 1, 1));
		cinfo.CreateBehavior.Scale.SetDefaultParameter(CVector3(3.0f, 3.0f, 3.0f));
		cinfo.CreateBehavior.Time = 0;
		cinfo.CreateBehavior.Count = 1;

	}
	//�ۑ�Q�@�����G�t�F�N�g(��)
	{
		m_LifeTime[EMITTER_EXPLOTION_SMOKE] = 5;	//�G�~�b�^�[�̎���
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[EMITTER_EXPLOTION_SMOKE];

		cinfo.UpdateBehavior.Color.SetDefaultParameter(CVector4(0, 0, 0, -1.0f / 7));
		cinfo.UpdateBehavior.Scale.SetDefaultParameter(CVector3(0.2f / 7, 0.2f / 7, 0.2f / 7));
		cinfo.UpdateBehavior.Move.SetParameter(CVector3(-0.1f, -0.1f, -0.1f), CVector3(0.1f, 0.1f, 0.1f));

		cinfo.ParameterBehavior.Existence.SetDefaultParameter(7);
		cinfo.ParameterBehavior.Blending = BLEND_NORMAL;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.ParameterBehavior.pTexture = &m_Smoke;
		cinfo.ParameterBehavior.pMesh = NULL;

		cinfo.CreateBehavior.Position.SetDefaultParameter(CVector3(0, 0, -1.0f));
		cinfo.CreateBehavior.Color.SetDefaultParameter(CVector4(1, 1, 1, 1));
		cinfo.CreateBehavior.Scale.SetDefaultParameter(CVector3(5.0f, 5.0f, 5.0f));
		cinfo.CreateBehavior.Time = 5;
		cinfo.CreateBehavior.Count = 3;

	}
	//�ۑ�Q�@�����G�t�F�N�g(��)
	{
		m_LifeTime[EMITTER_EXPLOTION_FIRE] = 5;	//�G�~�b�^�[�̎���
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[EMITTER_EXPLOTION_FIRE];

		cinfo.UpdateBehavior.Color.SetDefaultParameter(CVector4(0, 0, 0, -1.0f / 15.0f));
		cinfo.UpdateBehavior.Scale.SetDefaultParameter(CVector3(-1.0f / 15.0f, -1.0f / 15.0f, -1.0f / 15.0f));
		cinfo.UpdateBehavior.Move.SetParameter(CVector3(-0.1f, -0.1f, -0.1f), CVector3(0.1f, 0.1f, 0.1f));

		cinfo.ParameterBehavior.Existence.SetDefaultParameter(15.0f);
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.ParameterBehavior.pTexture = &m_Fire;
		cinfo.ParameterBehavior.pMesh = NULL;

		cinfo.CreateBehavior.Position.SetDefaultParameter(CVector3(0, 0, -1.0f));
		cinfo.CreateBehavior.Color.SetDefaultParameter(CVector4(1, 1, 1, 1));
		cinfo.CreateBehavior.Scale.SetDefaultParameter(CVector3(3.0f, 3.0f, 3.0f));
		cinfo.CreateBehavior.Time = 3.0f;
		cinfo.CreateBehavior.Count = 3;

	}
	//�ۑ�Q�@�����G�t�F�N�g(�΂̕�)
	{
		m_LifeTime[EMITTER_EXPLOTION_SPARK] = 5;	//�G�~�b�^�[�̎���
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[EMITTER_EXPLOTION_SPARK];

		cinfo.UpdateBehavior.Color.SetDefaultParameter(CVector4(0, 0, 0, -1.0f / 15.0f));
		cinfo.UpdateBehavior.Scale.SetDefaultParameter(CVector3(-1.0f / 15.0f, -1.0f / 15.0f, -1.0f / 15.0f));
		cinfo.UpdateBehavior.Move.SetParameter(CVector3(-0.1f, -0.1f, -0.1f), CVector3(0.1f, 0.1f, 0.1f));

		cinfo.ParameterBehavior.Existence.SetDefaultParameter(15.0f);
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.ParameterBehavior.pTexture = &m_RedFlash;
		cinfo.ParameterBehavior.pMesh = NULL;

		cinfo.CreateBehavior.Position.SetDefaultParameter(CVector3(0, 0, -1.0f));
		cinfo.CreateBehavior.Color.SetDefaultParameter(CVector4(1, 1, 1, 1));
		cinfo.CreateBehavior.Scale.SetDefaultParameter(CVector3(3.0f, 3.0f, 3.0f));
		cinfo.CreateBehavior.Time = 3.0f;
		cinfo.CreateBehavior.Count = 7;

	}
	
	//�ۑ�R�@�v���C���[�̎c��
	{
		m_LifeTime[EMITTER_AFTERIMAGE] = 0;	//�G�~�b�^�[�̎���
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[EMITTER_AFTERIMAGE];

		cinfo.UpdateBehavior.Color.SetDefaultParameter(CVector4(0, 0, 0, -1.0f / 15.0f));
		cinfo.UpdateBehavior.Scale.SetDefaultParameter(CVector3(-1.0f / 15.0f, -1.0f / 15.0f, -1.0f / 15.0f));
		cinfo.UpdateBehavior.Move.SetParameter(CVector3(-0.1f, -0.1f, -0.1f), CVector3(0.1f, 0.1f, 0.1f));

		cinfo.ParameterBehavior.Existence.SetDefaultParameter(15.0f);
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.ParameterBehavior.pTexture = &m_RedFlash;
		cinfo.ParameterBehavior.pMesh = NULL;

		cinfo.CreateBehavior.Position.SetDefaultParameter(CVector3(0, 0, -1.0f));
		cinfo.CreateBehavior.Color.SetDefaultParameter(CVector4(1, 1, 1, 1));
		cinfo.CreateBehavior.Scale.SetDefaultParameter(CVector3(3.0f, 3.0f, 3.0f));
		cinfo.CreateBehavior.Time = 3.0f;
		cinfo.CreateBehavior.Count = 7;

	}
	//�ۑ�R�@�v���C���[�̎c�����q
	{
		m_LifeTime[EMITTER_AFTERIMAGE_PARTICLE] = 0;	//�G�~�b�^�[�̎���
		CEmitter<MINMAXPARAMETER>::CREATEINFO& cinfo = m_CreateInfo[EMITTER_AFTERIMAGE_PARTICLE];

	}
}

/**
 * �X�V����
 */
void CEffectManager::Update(){
	//�G�~�b�^�[�̍X�V
	for(EmitterList::iterator it = m_EmitterList.begin(); it != m_EmitterList.end();)
	{
		if ((*it)->GetLifeTime() != 0)
		{
			(*it)->Update(&m_ParticleList);
		}
		//�I�������G�~�b�^�[�̍폜
		else
		{
			delete (*it);
			it =m_EmitterList.erase(it);
			continue;
		}
		++it;
	}
	//�p�[�e�B�N���̍X�V
	for (std::list< CParticle* >::iterator it = m_ParticleList.begin(); it != m_ParticleList.end(); )
	{
		(*it)->Update();
		//�I�������p�[�e�B�N���̍폜
		if ((*it)->IsEnd())
		{
			delete (*it);
			it =m_ParticleList.erase(it);
			continue;
		}
		++it;
	}
}

/**
 * �`�揈��
 */
void CEffectManager::Render(){
	//�\�[�g�̎��s
	for (std::list< CParticle* >::iterator it = m_ParticleList.begin(); it != m_ParticleList.end(); ++it)
	{
		(*it)->UpdateViewPosition();
	}
	m_ParticleList.sort(CParticle::ViewComp);
	//�p�[�e�B�N���̕`��
	for (std::list< CParticle* >::iterator it = m_ParticleList.begin(); it != m_ParticleList.end(); ++it)
	{
		(*it)->Render();
	}
	g_pGraphics->SetBlending(BLEND_NORMAL);
}

/**
 * �S�ẴG�t�F�N�g���폜
 */
void CEffectManager::Clear(){
	//�p�[�e�B�N���̉��
	for (std::list< CParticle* >::iterator it = m_ParticleList.begin(); it != m_ParticleList.end(); ++it)
	{
		MOF_SAFE_DELETE(*it);
	}
	m_ParticleList.clear();
	//�G�~�b�^�[�̉��
	for(EmitterList::iterator it = m_EmitterList.begin(); it != m_EmitterList.end(); ++it)
	{
		MOF_SAFE_DELETE(*it);
	}
	m_EmitterList.clear();
}

/**
 * �������
 */
void CEffectManager::Release(){
	Clear();
		
	m_BlueFlash.Release();
	m_WhiteFlash.Release();
	m_Smoke.Release();
	m_Fire.Release();
	m_RedFlash.Release();
	m_PlayerMesh.Release();
	m_Particle.Release();
}

/**
 * �G�~�b�^�[�̒ǉ�
 */
CEmitter<MINMAXPARAMETER>* CEffectManager::AddEmitter(EMITTERTYPE type,const CVector3& pos){
	if(type < 0 || EMITTERTYPE_MAX <= type)
	{
		return NULL;
	}
	return AddEmitter(type,pos,m_LifeTime[type]);
}

/**
 * �������w�肵�ăG�~�b�^�[��ǉ�
 */
CEmitter<MINMAXPARAMETER>* CEffectManager::AddEmitter(EMITTERTYPE type,const CVector3& pos,float LifeTime){
	if(type < 0 || EMITTERTYPE_MAX <= type)
	{
		return NULL;
	}
	CEmitter<MINMAXPARAMETER>* pEmitter = new CEmitter<MINMAXPARAMETER>();
	pEmitter->Initialize(m_CreateInfo[type],LifeTime);
	pEmitter->SetPosition(pos);
	m_EmitterList.push_back(pEmitter);
	return pEmitter;
}

/**
 * �G�~�b�^�[�̍폜
 */
void CEffectManager::DeleteEmitter(CEmitter<MINMAXPARAMETER>**ppEmitter){
	if(ppEmitter == NULL || *ppEmitter == NULL)
	{
		return;
	}
	EmitterList::iterator it = std::find(m_EmitterList.begin(),m_EmitterList.end(),*ppEmitter);
	if(it != m_EmitterList.end())
	{
		delete (*it);
		m_EmitterList.erase(it);
	}
	*ppEmitter = NULL;
}