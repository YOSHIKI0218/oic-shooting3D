#pragma once
#include "Mof.h"
class CSoundManager {
private:
	float		m_MusicVolume;
public:
	CSoundManager();
	~CSoundManager();
	void Release(void);
	void Update(void);
	//BGM�\���́B
	typedef struct
	{
		std::string Name;
		CSoundBuffer* SoundPtr;
		float		Volume;
		bool		Stopping;
	} AudioData;

	std::vector<AudioData*> m_Musics;
	std::vector<AudioData*> m_Effects;

	void PlayMusic(std::string name, std::string extension);
	void PlayEffect(std::string name, std::string extension);

	void ToggleMusic(std::string name);

	bool IsMusicPlaying(std::string name);

	void StopMusic(std::string name);

	void StopEffect(std::string name);

	/**
	 * �V���O���g���C���X�^���X���擾���邽�߂̗B��̃A�N�Z�X
	*/
	static CSoundManager& Instance()
	{
		static CSoundManager obj;
		return obj;
	}
};