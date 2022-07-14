#pragma once

#include "Emitter.h"


/**
 * �O���[�o���ϐ��ō쐬
 */
class CEffectManager;
extern CEffectManager gEffectManager;

/**
 * �G�~�b�^�[�̎��
 */
enum EMITTERTYPE{
	EMITTER_SHOT,					//�v���C���[�e���˃G�t�F�N�g
	EMITTER_HIT,					//�v���C���[�e�q�b�g�G�t�F�N�g
	EMITTER_EXPLOTION_FLASH,		//�����G�t�F�N�g(�t���b�V��)
	EMITTER_EXPLOTION_SMOKE,		//�����G�t�F�N�g(��)
	EMITTER_EXPLOTION_FIRE,			//�����G�t�F�N�g(��)
	EMITTER_EXPLOTION_SPARK,		//�����G�t�F�N�g(�΂̕�)
	EMITTER_AFTERIMAGE,				//�v���C���[�̎c��
	EMITTER_AFTERIMAGE_PARTICLE,	//�v���C���[�̎c�����q
	EMITTERTYPE_MAX,				//�G�~�b�^�[�̎�ސ�
};

//typedef
typedef std::list<CEmitter<MINMAXPARAMETER>*> EmitterList;

/**
 * �G�t�F�N�g�Ǘ��N���X
 */
class CEffectManager{
private:
	//�p�[�e�B�N��
	std::list< CParticle* >	m_ParticleList;
	//�G�~�b�^�[
	EmitterList				m_EmitterList;

	//�G�t�F�N�g�̃��\�[�X
	CTexture			m_BlueFlash;
	CTexture			m_WhiteFlash;
	CTexture			m_Smoke;
	CTexture			m_Fire;
	CTexture			m_RedFlash;
	CMeshContainer		m_PlayerMesh;
	CTexture			m_Particle;

	//CREATEINFO
	CEmitter<MINMAXPARAMETER>::CREATEINFO	m_CreateInfo[EMITTERTYPE_MAX];
	//�G�~�b�^�[�̎���
	float									m_LifeTime[EMITTERTYPE_MAX];

public:
	//�R���X�g���N�^
	CEffectManager();
	//�f�X�g���N�^
	~CEffectManager();
	//���\�[�X�̓ǂݍ���
	bool Load();
	//�G�~�b�^�[�̓���p�����[�^�[�ݒ�
	void InitCreateInfo();
	//�X�V����
	void Update();
	//�`�揈��
	void Render();
	//�S�ẴG�t�F�N�g���폜
	void Clear();
	//�������
	void Release();
	//�G�~�b�^�[�̒ǉ�
	CEmitter<MINMAXPARAMETER>* AddEmitter(EMITTERTYPE type,const CVector3& pos);
	//�������w�肵�ăG�~�b�^�[�̒ǉ�
	CEmitter<MINMAXPARAMETER>* AddEmitter(EMITTERTYPE type,const CVector3& pos,float LifeTime);
	//�G�~�b�^�[�̍폜
	void DeleteEmitter(CEmitter<MINMAXPARAMETER>**ppEmitter);
};