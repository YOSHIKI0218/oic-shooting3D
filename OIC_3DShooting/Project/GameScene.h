#pragma once
#include "SceneBase.h"
class CGameScene : public CSceneBase
{
public:
	CGameScene();
	~CGameScene();
	//���N���X�̊֐�
	void Load() override;
	void Initalize() override;
	void Update() override;
	void Render() override;
	void Release() override;
	void Reset() override;
	//���N���X�̊֐�
	void InitalizeEffects();
	bool IsClear() { return gbClear; }
	bool IsDead();
	
private:
	void UpdateCamera();
	
	void InitializeCamera();
	void InitializeLight();
	bool				gbClear;
	bool				gbEnemyDestroyed;
	bool				m_bFirstInialize;
};

