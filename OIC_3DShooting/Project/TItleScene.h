#pragma once
#include "Mof.h"
#include "SceneBase.h"

class CTitleScene : public CSceneBase
{
public:
	CTitleScene();
	~CTitleScene();
	//���N���X�̊֐�
	void Load() override;
	void Initalize() override;
	void Update() override;
	void Render() override;
	void Release() override;
	void Reset() override;
	//���N���X�̊֐�
private:
	CVector3 m_Rot;
	CVector3 m_Pos;
};

