#pragma once
#include "Mof.h"
#include "SceneBase.h"

class CGameoverScene : public CSceneBase
{
public:
	CGameoverScene();
	~CGameoverScene();

	//基底クラスの関数
	void Load() override;
	void Initalize() override;
	void Update() override;
	void Render() override;
	void Release() override;
	void Reset() override;
	//基底クラスの関数
private:
	CTexture m_GameOverTexture;
};

