#include "SoundManager.h"

CSoundManager::CSoundManager():
	m_MusicVolume(0.0f)
{
}

CSoundManager::~CSoundManager()
{
}

void CSoundManager::Release(void)
{
	//���̉��
	for (int i = 0; i < m_Musics.size(); i++)
	{
		m_Musics[i]->SoundPtr->Release();
		delete m_Musics[i]->SoundPtr;
		delete m_Musics[i];
	}
	m_Musics.shrink_to_fit();

	for (int i = 0; i < m_Effects.size(); i++)
	{
		m_Effects[i]->SoundPtr->Release();
		delete m_Effects[i]->SoundPtr;
		delete m_Effects[i];
	}
	m_Effects.shrink_to_fit();
}

void CSoundManager::Update(void)
{
	int playingIndex = -1;
	for (int i = 0; i < m_Musics.size(); i++)
	{
		//��~�t���O���I���̉��y�́A���ʂ����X��0�ɂ���
		if (m_Musics[i]->Stopping)
		{
			m_Musics[i]->Volume = MOF_CLIPING(m_Musics[i]->Volume - CUtilities::GetFrameSecond(), 0.0f, 1.0f);
			if (m_Musics[i]->Volume == 0.0f && m_Musics[i]->SoundPtr->IsPlay())
			{
				m_Musics[i]->SoundPtr->Stop();
			}
			else
			{
				m_Musics[i]->SoundPtr->SetVolume(m_Musics[i]->Volume);
			}
		}
		else playingIndex = i;
	}
	if (playingIndex == -1)return;
	//�I�t�Ȃ�P�ɂ���
	if (m_Musics[playingIndex]->Volume == 0.0f && !m_Musics[playingIndex]->SoundPtr->IsPlay())
	{
		m_Musics[playingIndex]->SoundPtr->Play();
	}
	else
	{
		m_Musics[playingIndex]->SoundPtr->SetVolume(m_Musics[playingIndex]->Volume);
	}
	m_Musics[playingIndex]->Volume = MOF_CLIPING(m_Musics[playingIndex]->Volume + CUtilities::GetFrameSecond(), 0.0f, 1.0f);
}

void CSoundManager::PlayMusic(std::string name, std::string extension)
{
	std::string path = "Sounds/bg/";
	std::string dot = ".";
	if (m_Musics.empty())
	{
		//�\���̂̃C���X�^���X��
		AudioData* pMus = new AudioData();
		//�C���X�^���X�ɕK�v�ȃf�[�^���Z�b�g
		pMus->SoundPtr = new CSoundBuffer();
		pMus->Name = name;
		//���[�h�Ɏ��s������A�C���X�^���X��j������
		if (!pMus->SoundPtr->Load((path + name + dot + extension).c_str()))
		{
			MOF_PRINTLOG("Music not found\n");
			pMus->SoundPtr->Release();
			//new�Ŋm�ۂ����͕̂K��delete�ŏ���!!!
			delete pMus->SoundPtr;
			delete pMus;
			return;
		}
		pMus->Volume = 0.0f;
		pMus->SoundPtr->SetLoop(true);

		//�x�N�^�[�ɒǉ�����
		m_Musics.push_back(pMus);
	}
	else
	{
		bool somethingPlaying = false;
		//���O����v������̂�T���B
		for (int i = 0; i < m_Musics.size(); i++)
		{
			//�S���̉��y�̒�~�t���O���I���ɂ���
			m_Musics[i]->Stopping = true;
			if (m_Musics[i]->Name == name)
			{
				//���O�ƈ�v������̂����t���O���I�t�ɂ���
				m_Musics[i]->Stopping = false;
				somethingPlaying = true;
			}
		}
		if (somethingPlaying)return;//����Đ�����Ȃ������ꍇ�́A�ǉ������݂�
		//�\���̂̃C���X�^���X��
		AudioData* pMus = new AudioData();
		//�C���X�^���X�ɕK�v�ȃf�[�^���Z�b�g
		pMus->SoundPtr = new CSoundBuffer();
		pMus->Name = name;
		//���[�h�Ɏ��s������A�C���X�^���X��j������
		if (!pMus->SoundPtr->Load((path + name + dot + extension).c_str()))
		{
			MOF_PRINTLOG("Music not found\n");
			pMus->SoundPtr->Release();
			delete pMus->SoundPtr;
			delete pMus;
			return;
		}
		pMus->Volume = 0.0f;
		pMus->SoundPtr->SetLoop(true);
		//�x�N�^�[�ɒǉ�����
		m_Musics.push_back(pMus);
	}
}

void CSoundManager::PlayEffect(std::string name, std::string extension)
{
	std::string path = "Sounds/se/";
	std::string dot = ".";
	if (m_Effects.empty())
	{
		//�\���̂̃C���X�^���X��
		AudioData* pMus = new AudioData();
		//�C���X�^���X�ɕK�v�ȃf�[�^���Z�b�g
		pMus->SoundPtr = new CSoundBuffer();
		
		pMus->Name = name;
		//���[�h�Ɏ��s������A�C���X�^���X��j������
		if (!pMus->SoundPtr->Load((path + name + dot +extension).c_str()))
		{
			MOF_PRINTLOG("Effect not found\n");
			pMus->SoundPtr->Release();
			//new�Ŋm�ۂ����͕̂K��delete�ŏ���!!!
			delete pMus->SoundPtr;
			delete pMus;
			return;
		}
		pMus->SoundPtr->Play();
		//�x�N�^�[�ɒǉ�����
		m_Effects.push_back(pMus);
	}
	else
	{
		//���O����v������̂�T���B
		for (int i = 0; i < m_Effects.size(); i++)
		{
			if (m_Effects[i]->Name == name)
			{
				m_Effects[i]->SoundPtr->Play();
				return;
			}
		}

		//�Ȃ�������A�ǉ������݂�B
		//�\���̂̃C���X�^���X��
		AudioData* pMus = new AudioData();
		//�C���X�^���X�ɕK�v�ȃf�[�^���Z�b�g
		pMus->SoundPtr = new CSoundBuffer();
		pMus->Name = name;
		//���[�h�Ɏ��s������A�C���X�^���X��j������
		if (!pMus->SoundPtr->Load((path + name + dot + extension).c_str()))
		{
			MOF_PRINTLOG("Effect not found\n");
			pMus->SoundPtr->Release();
			delete pMus->SoundPtr;
			delete pMus;
			return;
		}
		pMus->SoundPtr->Play();
		//�x�N�^�[�ɒǉ�����
		m_Effects.push_back(pMus);
	}
}

void CSoundManager::ToggleMusic(std::string name)
{
	for (int i = 0; i < m_Musics.size(); i++)
	{
		if (m_Musics[i]->Name == name)
		{
			if(m_Musics[i]->SoundPtr->IsPlay())
				m_Musics[i]->SoundPtr->Pause();
			else
				m_Musics[i]->SoundPtr->Play();
		}
	}
}

bool CSoundManager::IsMusicPlaying(std::string name)
{
	for (int i = 0; i < m_Musics.size(); i++)
	{
		if(m_Musics[i]->Name== name)
			return m_Musics[i]->SoundPtr->IsPlay();
	}
	return false;
}

void CSoundManager::StopMusic(std::string name)
{
	for (int i = 0; i < m_Musics.size(); i++)
	{
		if (m_Musics[i]->Name == name)
			m_Musics[i]->SoundPtr->Stop();
	}
}

void CSoundManager::StopEffect(std::string name)
{
	for (int i = 0; i < m_Effects.size(); i++)
	{
		if (m_Effects[i]->Name == name)
			m_Effects[i]->SoundPtr->Stop();
	}
}

