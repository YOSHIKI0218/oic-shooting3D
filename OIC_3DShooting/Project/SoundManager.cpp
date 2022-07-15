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
	//音の解放
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
		//停止フラグがオンの音楽は、音量を徐々に0にする
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
	//オフなら１にする
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
		//構造体のインスタンス化
		AudioData* pMus = new AudioData();
		//インスタンスに必要なデータをセット
		pMus->SoundPtr = new CSoundBuffer();
		pMus->Name = name;
		//ロードに失敗したら、インスタンスを破棄する
		if (!pMus->SoundPtr->Load((path + name + dot + extension).c_str()))
		{
			MOF_PRINTLOG("Music not found\n");
			pMus->SoundPtr->Release();
			//newで確保したのは必ずdeleteで消す!!!
			delete pMus->SoundPtr;
			delete pMus;
			return;
		}
		pMus->Volume = 0.0f;
		pMus->SoundPtr->SetLoop(true);

		//ベクターに追加する
		m_Musics.push_back(pMus);
	}
	else
	{
		bool somethingPlaying = false;
		//名前が一致するものを探す。
		for (int i = 0; i < m_Musics.size(); i++)
		{
			//全部の音楽の停止フラグをオンにする
			m_Musics[i]->Stopping = true;
			if (m_Musics[i]->Name == name)
			{
				//名前と一致するものだけフラグをオフにする
				m_Musics[i]->Stopping = false;
				somethingPlaying = true;
			}
		}
		if (somethingPlaying)return;//一つも再生されなかった場合は、追加を試みる
		//構造体のインスタンス化
		AudioData* pMus = new AudioData();
		//インスタンスに必要なデータをセット
		pMus->SoundPtr = new CSoundBuffer();
		pMus->Name = name;
		//ロードに失敗したら、インスタンスを破棄する
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
		//ベクターに追加する
		m_Musics.push_back(pMus);
	}
}

void CSoundManager::PlayEffect(std::string name, std::string extension)
{
	std::string path = "Sounds/se/";
	std::string dot = ".";
	if (m_Effects.empty())
	{
		//構造体のインスタンス化
		AudioData* pMus = new AudioData();
		//インスタンスに必要なデータをセット
		pMus->SoundPtr = new CSoundBuffer();
		
		pMus->Name = name;
		//ロードに失敗したら、インスタンスを破棄する
		if (!pMus->SoundPtr->Load((path + name + dot +extension).c_str()))
		{
			MOF_PRINTLOG("Effect not found\n");
			pMus->SoundPtr->Release();
			//newで確保したのは必ずdeleteで消す!!!
			delete pMus->SoundPtr;
			delete pMus;
			return;
		}
		pMus->SoundPtr->Play();
		//ベクターに追加する
		m_Effects.push_back(pMus);
	}
	else
	{
		//名前が一致するものを探す。
		for (int i = 0; i < m_Effects.size(); i++)
		{
			if (m_Effects[i]->Name == name)
			{
				m_Effects[i]->SoundPtr->Play();
				return;
			}
		}

		//なかったら、追加を試みる。
		//構造体のインスタンス化
		AudioData* pMus = new AudioData();
		//インスタンスに必要なデータをセット
		pMus->SoundPtr = new CSoundBuffer();
		pMus->Name = name;
		//ロードに失敗したら、インスタンスを破棄する
		if (!pMus->SoundPtr->Load((path + name + dot + extension).c_str()))
		{
			MOF_PRINTLOG("Effect not found\n");
			pMus->SoundPtr->Release();
			delete pMus->SoundPtr;
			delete pMus;
			return;
		}
		pMus->SoundPtr->Play();
		//ベクターに追加する
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

