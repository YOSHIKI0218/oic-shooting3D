#pragma once

#include "Mof.h"
#include <string>
#include <cstdio>
#include <vector>

// �v���C���[�֘A
// �ړ����x
#define PLAYER_SPEED		(0.1f)
#define PLAYER_ROTATE_SPEED	(10.0f)

//�@�v���C���[�e�֘A
#define PLAYER_SHOT_SPEED	(0.4f)
#define PLAYER_SHOT_COUNT	100
#define PLAYER_SHOT_WAIT	5

// �G
#define ENEMY_COUNT			40
#define ENEMY_BULLET_SPEED  (2.5f)

//�v���C���[
#define PLAYER_MAX_HP	20

//�{�X1�̍U���p�^�[���؂�ւ�����
#define BOSS1_PATTERN_CHANGE_INTERVAL 10.0f

//�A�C�e���^�C�v
enum tag_ITEMTYPE
{
	//�S��
	ITEMTYPE_HEAL,
	//�V�[���h
	ITEMTYPE_SHIELD,
	//�񋓑̂̐�
	ITEMTYPE_COUNT
};

//�悭�g���x�N�g���̃}�N��

//���ׂ�0(0,0,0)�̃x�N�g��
#define ZERO_VECTOR3 Vector3(0,0,0)
//���ׂ�1(1,1,1)�̃x�N�g��
#define ONE_VECTOR3 Vector3(1,1,1)
//���ׂ�-1(-1,-1,-1)�̃x�N�g��
#define MINUSONE_VECTOR3 Vector3(-1,-1,-1)
//�����(0,1,0)�̃x�N�g��
#define UP_VECTOR3 Vector3(0,1,0)
//������(0,-1,0)�̃x�N�g��
#define DOWN_VECTOR3 Vector3(0,-1,0)
//�E����(1,0,0)�̃x�N�g��
#define RIGHT_VECTOR3 Vector3(1,0,0)
//������(-1,0,0)�̃x�N�g��
#define LEFT_VECTOR3 Vector3(-1,0,0)
//�O����(0,0,1)�̃x�N�g��
#define FRONT_VECTOR3 Vector3(0,0,1)
//�����(0,0,-1)�̃x�N�g��
#define BACK_VECTOR3 Vector3(0,0,-1)

//�C�[�W���O���
enum Easing_Type {
	//���`���(���j�A)
	Ease_Linear,
	//���(�T�C���A�C��)
	Ease_Sine_In,
	//���(�T�C���A�A�E�g)
	Ease_Sine_Out,
	//���(�T�C���A�C���A�E�g)
	Ease_Sine_InOut
};

//�A�j���[�V�����f�[�^
struct Anim_Data {
	//�J�n����
	float	fTime;
	//�l
	float	fValue;
	//�C�[�W���O�̎��
	Easing_Type EasingType;
};

//�V�[���ԍ�
enum tag_SCENENO
{
	SCENENO_TITLE,
	SCENENO_GAME,
	SCENENO_GAMEOVER,
	SCENENO_GAMECLEAR
};

//�{�X1�̍U���p�^�[��
enum tag_SHOTPATTERN
{
	//�v���C���[�Ɍ�������1����
	SHOTPATTERN_SINGLE,
	//�v���C���[�Ɍ�������3����
	SHOTPATTERN_TRIPLE,
	//�~�`(���݂�)
	SHOTPATTERN_CIRCLE,
	//4�A���s
	SHOTPATTERN_PARALLEL_FOUR,
	//�v���C���[�Ɍ������ăz�[�~���O3��
	SHOTPATTERN_HORMING_TRIPLE,
	//�v���C���[�Ɍ������ăT�C���g
	SHOTPATTERN_SINEWAVE,
	//���s�E������
	SHOTPATTERN_EXTREMEPARALLEL,
	//�񋓑̂̐�
	SHOTPATTERN_COUNT
};
//�G�t�F�N�gID
enum tag_EFFECTID
{
	EFFECT_EXPLOSION,
	EFFECT_HIT
};

//     ��ԁ@Easing
float InterpolationAmin(float animtime, Anim_Data* animdata, int cnt);

template <typename ... Args>
//string�̃t�H�[�}�b�g
std::string format(const std::string& fmt, Args ... args)
{
	size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args ...);
	std::vector<char> buf(len + 1);
	std::snprintf(&buf[0], len + 1, fmt.c_str(), args ...);
	return std::string(&buf[0], &buf[0] + len);
}

class CGameDefine
{
public:
	static void LoadParameter();
	static float GetBackgroundScrollSpeed();
	static float GetPlayerRotationSpeed();
	static float GetFiledHalfX();
	static float GetFiledHalfY();
	static float GetFiledHalfZ();
	static int GetBoss1MaxHP();
	static int GetPlayerMaxHP();
	static int GetEnemyHP();
private:
	/*
	JSON����ǂݍ���Őݒ肷��ϐ��B
	�}�N���ɂ���Ƃ��������R���p�C�����Ȃ��Ɛ��l�ς����Ȃ����ߖʓ|������
	�̂�JSON���炷�΂₭�ȒP�Ƀp�����[�^��ύX�ł���悤�ɂ���B
	*/
	
	static CVector3 m_FiledHalf;
	static float	m_BackgroundScrollSpeed;
	static float m_PlayerSpeed;
	static int m_PlayerRotateSpeed;
	static float m_PlayerByulletSpeed;
	static int m_PlayerShotCount;
	static int m_PlayerShotInterval;
	static int m_EnemyCount;
	static float m_EnemyBulletSpeed;
	static int m_MaxPlayerHP;
	static float m_Boss1PatternChangeInterval;
	static int m_Boss1MaxHp;
	static int m_PlayerMaxHP;
	static int m_EnemyHP;
};