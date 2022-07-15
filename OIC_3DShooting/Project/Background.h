#pragma once
#include "GameDefine.h"

class CBackground
{
private:
	CSprite3D		m_BG;
	float			m_fScroll;
public:
	void Load();
	void Initalize();
	void Render();
	void Release();
	void Update();
	void SetScroll(float value) { m_fScroll = value; }
	float GetScroll() { return m_fScroll; }
	//シングルトンインスタンスを取得するための唯一のアクセス
	static CBackground& Instance()
	{
		static CBackground obj;
		return obj;
	}
};

