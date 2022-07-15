#pragma once

#include "Mof.h"
#include <string>
#include <cstdio>
#include <vector>

// プレイヤー関連
// 移動速度
#define PLAYER_SPEED		(0.1f)
#define PLAYER_ROTATE_SPEED	(10.0f)

//　プレイヤー弾関連
#define PLAYER_SHOT_SPEED	(0.4f)
#define PLAYER_SHOT_COUNT	100
#define PLAYER_SHOT_WAIT	5

// 敵
#define ENEMY_COUNT			40
#define ENEMY_BULLET_SPEED  (2.5f)

//プレイヤー
#define PLAYER_MAX_HP	20

//ボス1の攻撃パターン切り替え時間
#define BOSS1_PATTERN_CHANGE_INTERVAL 10.0f

//アイテムタイプ
enum tag_ITEMTYPE
{
	//全回復
	ITEMTYPE_HEAL,
	//シールド
	ITEMTYPE_SHIELD,
	//列挙体の数
	ITEMTYPE_COUNT
};

//よく使うベクトルのマクロ

//すべて0(0,0,0)のベクトル
#define ZERO_VECTOR3 Vector3(0,0,0)
//すべて1(1,1,1)のベクトル
#define ONE_VECTOR3 Vector3(1,1,1)
//すべて-1(-1,-1,-1)のベクトル
#define MINUSONE_VECTOR3 Vector3(-1,-1,-1)
//上方向(0,1,0)のベクトル
#define UP_VECTOR3 Vector3(0,1,0)
//下方向(0,-1,0)のベクトル
#define DOWN_VECTOR3 Vector3(0,-1,0)
//右方向(1,0,0)のベクトル
#define RIGHT_VECTOR3 Vector3(1,0,0)
//左方向(-1,0,0)のベクトル
#define LEFT_VECTOR3 Vector3(-1,0,0)
//前方向(0,0,1)のベクトル
#define FRONT_VECTOR3 Vector3(0,0,1)
//後方向(0,0,-1)のベクトル
#define BACK_VECTOR3 Vector3(0,0,-1)

//イージング種類
enum Easing_Type {
	//線形補間(リニア)
	Ease_Linear,
	//補間(サイン、イン)
	Ease_Sine_In,
	//補間(サイン、アウト)
	Ease_Sine_Out,
	//補間(サイン、インアウト)
	Ease_Sine_InOut
};

//アニメーションデータ
struct Anim_Data {
	//開始時間
	float	fTime;
	//値
	float	fValue;
	//イージングの種類
	Easing_Type EasingType;
};

//シーン番号
enum tag_SCENENO
{
	SCENENO_TITLE,
	SCENENO_GAME,
	SCENENO_GAMEOVER,
	SCENENO_GAMECLEAR
};

//ボス1の攻撃パターン
enum tag_SHOTPATTERN
{
	//プレイヤーに向かって1つだけ
	SHOTPATTERN_SINGLE,
	//プレイヤーに向かって3つ同時
	SHOTPATTERN_TRIPLE,
	//円形(交互に)
	SHOTPATTERN_CIRCLE,
	//4連並行
	SHOTPATTERN_PARALLEL_FOUR,
	//プレイヤーに向かってホーミング3つ
	SHOTPATTERN_HORMING_TRIPLE,
	//プレイヤーに向かってサイン波
	SHOTPATTERN_SINEWAVE,
	//並行・超高速
	SHOTPATTERN_EXTREMEPARALLEL,
	//列挙体の数
	SHOTPATTERN_COUNT
};
//エフェクトID
enum tag_EFFECTID
{
	EFFECT_EXPLOSION,
	EFFECT_HIT
};

//     補間　Easing
float InterpolationAmin(float animtime, Anim_Data* animdata, int cnt);

template <typename ... Args>
//stringのフォーマット
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
	JSONから読み込んで設定する変数。
	マクロにするといちいちコンパイルしないと数値変えられないため面倒くさい
	のでJSONからすばやく簡単にパラメータを変更できるようにする。
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