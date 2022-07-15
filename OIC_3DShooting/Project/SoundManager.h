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
	//BGM構造体。
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
	 * シングルトンインスタンスを取得するための唯一のアクセス
	*/
	static CSoundManager& Instance()
	{
		static CSoundManager obj;
		return obj;
	}
};