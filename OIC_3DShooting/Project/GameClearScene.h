#pragma once
#include "SceneBase.h"
#include "Mof.h"

class CGameClearScene :
    public CSceneBase
{
public:
	CGameClearScene();
	~CGameClearScene();

	//基底クラスの関数
	void Load() override;
	void Initalize() override;
	void Update() override;
	void Render() override;
	void Release() override;
	void Reset() override;
	//基底クラスの関数
private:
	CTexture m_GameClearTexture;
};

