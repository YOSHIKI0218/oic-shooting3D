#pragma once
#include	"GameDefine.h"
class CItemBase
{
protected:
	CMatrix44			m_WorldMatrix;
	CVector3            m_Position;
	bool				m_bShow;
	float				m_Rotation;
public:
	virtual void Load() = 0;
	virtual void Initalize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual int GetType() = 0;
	virtual CSphere GetSphere() = 0;
	virtual void Start(const Vector3& pos) = 0;
	void SetShow(bool b) { m_bShow = b; }
	bool GetShow() { return m_bShow; }
};

