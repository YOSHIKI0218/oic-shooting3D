#include "HealItem.h"

void CHealItem::Load()
{
	m_Mesh.Load("item_heart.mom");
}

void CHealItem::Initalize()
{

}

void CHealItem::Update()
{
	if (!GetShow())
		return;
	m_Position += BACK_VECTOR3 * CUtilities::GetFrameSecond() * 3.0f;
	float halfz = CGameDefine::GetFiledHalfZ();
	if (m_Position.z < -halfz)
	{
		SetShow(false);
	}
}

void CHealItem::Render()
{
	if (!GetShow())
		return;
	m_Rotation += CUtilities::GetFrameSecond() * 60.0f;
	CMatrix44 matRot;
	matRot.RotationY(MOF_ToRadian(m_Rotation));
	CMatrix44 matScale;
	matScale.Scaling(0.5f);
	m_WorldMatrix.SetTranslation(m_Position);
	m_Mesh.Render(matRot * matScale * m_WorldMatrix);
}

void CHealItem::Release()
{
	m_Mesh.Release();
}

int CHealItem::GetType()
{
	return ITEMTYPE_HEAL;
}

CSphere CHealItem::GetSphere()
{
	return CSphere(m_Position, 1.0f);
}

void CHealItem::Start(const Vector3& pos)
{
	SetShow(true);
	m_Rotation = 0.0f;
	m_Position = pos;
}
