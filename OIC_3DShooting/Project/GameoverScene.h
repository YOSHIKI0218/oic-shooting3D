#pragma once
#include "Mof.h"
#include "SceneBase.h"

class CGameoverScene : public CSceneBase
{
public:
	CGameoverScene();
	~CGameoverScene();

	//���N���X�̊֐�
	void Load() override;
	void Initalize() override;
	void Update() override;
	void Render() override;
	void Release() override;
	void Reset() override;
	//���N���X�̊֐�
private:
	CTexture m_GameOverTexture;
};

