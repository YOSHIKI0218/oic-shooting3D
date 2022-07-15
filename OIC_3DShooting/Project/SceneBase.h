/*************************************************************************//*!

					@file	SceneBase.h
					@brief	�V�[���̊��N���X�B

															@author	���O
*//**************************************************************************/
#pragma once
class CSceneBase
{
public:
	virtual void Load() = 0;
	virtual void Initalize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void Reset() = 0;
};

