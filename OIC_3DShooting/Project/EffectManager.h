#pragma once
#include "GameDefine.h"
#include "Particle.h"
class CEffectManager
{
private:
	//�G�t�F�N�g�̉ϒ��z��
	static CDynamicArray<CParticle*> m_Effects;

public:
	//�G�t�F�N�g�̃X�|�[��(�Đ�)
	static CParticle* Spawn(int id);
	//�쐬���𗘗p���ăG�t�F�N�g���X�|�[��(�Đ�)
	static CParticle* Spawn(int id, CParticle::CREATEINFO& info);
	//�G�t�F�N�g���쐬�B�쐬�݂̂ŁA�Đ����Ȃ�
	static CParticle* Create(CParticle::CREATEINFO& info);
	//���ׂẴG�t�F�N�g�̍X�V
	static void Update();
	//���ׂẴG�t�F�N�g�̕`��
	static void Render();
	//���
	static void Release();
};

