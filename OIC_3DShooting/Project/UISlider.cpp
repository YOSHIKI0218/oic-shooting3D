#include "UISlider.h"
#include "GameDefine.h"

using namespace std;

CFont	gFont;

CUISlider::CUISlider():
	m_CurrentValue(0),
	m_MaxValue(0),
	m_SliderHeight(32),
	m_SliderWidth(512),
	m_PixelsPerValue(0),
	m_Title(),
	m_bAutoChangeColorByValue(false),
	m_bShowTitle(false),
	m_bShowValueText(false),
	m_bInitalized(false)
{
}

CUISlider::~CUISlider()
{
	gFont.Release();
}

void CUISlider::SetMaxValue(float value)
{
	if (value <= 0) 
	{
		MOF_PRINTLOG("スライダーで0以下の値は使用できません。\n");
		return;
	}

	m_MaxValue = value;
	m_PixelsPerValue = m_SliderWidth / value;
}

void CUISlider::SetValue(float value)
{
	//値の範囲制限。てかCLIPINGってスペルミス？
	m_CurrentValue=MOF_CLIPING(value, 0.0f, m_MaxValue);
}

void CUISlider::Render()
{
	//初期化されていなければ、初期化する
	if (!m_bInitalized)
		Initalize();
	if (m_CurrentValue < 0)
		return;
	float max = m_MaxValue;
	float current = m_CurrentValue;
	int targetPosY = m_bShowTitle ? m_SliderHeight : 0;
	if(m_bShowTitle)
		gFont.RenderString(m_Position.x, m_Position.y + 5, MOF_COLOR_BLACK, m_Title);
	if (m_bShowValueText)
	{
		string text = format("%.0f", m_CurrentValue) + "/" + format("%.0f", m_MaxValue);
		gFont.RenderString(m_Position.x + 20 + m_PixelsPerValue * m_MaxValue, m_Position.y + targetPosY + 5, text.c_str());
	}
	
	//枠の描画
	CGraphicsUtilities::RenderRect(
		m_Position.x,
		m_Position.y + targetPosY,
		m_Position.x + m_PixelsPerValue * m_MaxValue,
		m_Position.y + m_SliderHeight + targetPosY,
		MOF_COLOR_WHITE);

	//スライダー本体の描画
	//数値によって色を変える場合
	if (m_bAutoChangeColorByValue) {
		float factor = current / max;
		if (factor > 0.5f)
		{
			//半透明の緑
			CGraphicsUtilities::RenderFillRect(m_Position.x, m_Position.y + targetPosY, m_Position.x + m_PixelsPerValue * m_CurrentValue, m_Position.y + m_SliderHeight + targetPosY, 0xB51EFF1E);
			
		}
		else if (factor > 0.25f)
		{
			//半透明の橙
			CGraphicsUtilities::RenderFillRect(m_Position.x, m_Position.y + targetPosY, m_Position.x + m_PixelsPerValue * m_CurrentValue, m_Position.y + m_SliderHeight + targetPosY, 0xB5FFFF1E);
		}
		else
		{
			//半透明の赤
			CGraphicsUtilities::RenderFillRect(m_Position.x, m_Position.y + targetPosY, m_Position.x + m_PixelsPerValue * m_CurrentValue, m_Position.y + m_SliderHeight + targetPosY, 0xB5FF1E1E);
		}
	}
	else //そうでない
	{
		CGraphicsUtilities::RenderFillRect(m_Position.x, m_Position.y + targetPosY, m_Position.x + m_PixelsPerValue * m_CurrentValue, m_Position.y + m_SliderHeight + targetPosY, m_Color.ToU32Color());
	}
}

void CUISlider::SetHeight(float value)
{
	m_SliderHeight = value;
}

void CUISlider::SetWidth(float value)
{
	m_SliderWidth = value;
	if (m_MaxValue == 0)
		return;
	m_PixelsPerValue = m_SliderWidth / m_MaxValue;
}

void CUISlider::SetDefaultColor()
{
	m_Color.a = 0xB5;
	m_Color.r = 0x1E;
	m_Color.g = 0xFF;
	m_Color.b = 0x1E;
}

void CUISlider::Initalize()
{
	if (m_bInitalized)
		return;
	gFont.Create(28, "Meiryo UI");
	
	m_bInitalized = true;
}
