#pragma once
#include "Mof.h"

//スライダー
class CUISlider
{
public:
	CUISlider();
	~CUISlider();

	//最大値を設定する
	void SetMaxValue(float value);
	//現在の値を設定する
	void SetValue(float value);
	//描画
	void Render();
	//タイトルを表示するかを設定する
	void SetShowTitle(bool b) { m_bShowTitle = b; }
	//現在の値の文字を表示するかを切り替える。
	void SetShowValueText(bool b) { m_bShowValueText = b; }
	//スライダーの表示位置(左上基準)を設定する。
	void SetPosition(const Vector2& vec) { m_Position = vec; }
	//値によって自動的に色を変えるか？trueの場合は緑→橙→赤の順で色が変わる。
	void SetAutoChangeColorByValue(bool b) { m_bAutoChangeColorByValue = b; }

	//スライダーの左側に表示されるタイトルを設定する。
	void SetTitleText(LPCMofChar text) { m_Title = text; }
	//スライダーの高さを設定する
	void SetHeight(float value);
	//スライダーの幅を設定する
	void SetWidth(float value);

	//色を変更する(色の自動変更が無効な時のみ使用される)
	void SetColor(const Vector4& col) { m_Color = col; }

	//色を初期値に戻す(半透明の緑)
	void SetDefaultColor();

	//初期化(自動で実行されます)
	void Initalize();
private:
	//スライダーの最大値
	float m_MaxValue;
	//スライダーの現在値
	float m_CurrentValue;
	//スライダーの幅
	float m_SliderWidth;
	//スライダーの高さ
	float m_SliderHeight;

	//現在の色
	CVector4	m_Color;

	

	//スライダーの値1あたりのピクセル数
	float m_PixelsPerValue;

	//表示タイトル
	LPCMofChar m_Title;
	//タイトル表示フラグ
	bool m_bShowTitle;

	//現在値の表示フラグ
	bool	m_bShowValueText;

	//色変更フラグ
	bool	m_bAutoChangeColorByValue;

	//初期化フラグ
	bool	m_bInitalized;

	//スライダーの位置
	CVector2	m_Position;
};

