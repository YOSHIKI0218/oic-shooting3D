#pragma once
#include "SceneBase.h"
#include "Mof.h"

class CGameClearScene :
    public CSceneBase
{
public:
	CGameClearScene();
	~CGameClearScene();

	//���N���X�̊֐�
	void Load() override;
	void Initalize() override;
	void Update() override;
	void Render() override;
	void Release() override;
	void Reset() override;
	//���N���X�̊֐�
private:
	CTexture m_GameClearTexture;
};

