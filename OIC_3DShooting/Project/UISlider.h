#pragma once
#include "Mof.h"

//�X���C�_�[
class CUISlider
{
public:
	CUISlider();
	~CUISlider();

	//�ő�l��ݒ肷��
	void SetMaxValue(float value);
	//���݂̒l��ݒ肷��
	void SetValue(float value);
	//�`��
	void Render();
	//�^�C�g����\�����邩��ݒ肷��
	void SetShowTitle(bool b) { m_bShowTitle = b; }
	//���݂̒l�̕�����\�����邩��؂�ւ���B
	void SetShowValueText(bool b) { m_bShowValueText = b; }
	//�X���C�_�[�̕\���ʒu(����)��ݒ肷��B
	void SetPosition(const Vector2& vec) { m_Position = vec; }
	//�l�ɂ���Ď����I�ɐF��ς��邩�Htrue�̏ꍇ�͗΁��򁨐Ԃ̏��ŐF���ς��B
	void SetAutoChangeColorByValue(bool b) { m_bAutoChangeColorByValue = b; }

	//�X���C�_�[�̍����ɕ\�������^�C�g����ݒ肷��B
	void SetTitleText(LPCMofChar text) { m_Title = text; }
	//�X���C�_�[�̍�����ݒ肷��
	void SetHeight(float value);
	//�X���C�_�[�̕���ݒ肷��
	void SetWidth(float value);

	//�F��ύX����(�F�̎����ύX�������Ȏ��̂ݎg�p�����)
	void SetColor(const Vector4& col) { m_Color = col; }

	//�F�������l�ɖ߂�(�������̗�)
	void SetDefaultColor();

	//������(�����Ŏ��s����܂�)
	void Initalize();
private:
	//�X���C�_�[�̍ő�l
	float m_MaxValue;
	//�X���C�_�[�̌��ݒl
	float m_CurrentValue;
	//�X���C�_�[�̕�
	float m_SliderWidth;
	//�X���C�_�[�̍���
	float m_SliderHeight;

	//���݂̐F
	CVector4	m_Color;

	

	//�X���C�_�[�̒l1������̃s�N�Z����
	float m_PixelsPerValue;

	//�\���^�C�g��
	LPCMofChar m_Title;
	//�^�C�g���\���t���O
	bool m_bShowTitle;

	//���ݒl�̕\���t���O
	bool	m_bShowValueText;

	//�F�ύX�t���O
	bool	m_bAutoChangeColorByValue;

	//�������t���O
	bool	m_bInitalized;

	//�X���C�_�[�̈ʒu
	CVector2	m_Position;
};

