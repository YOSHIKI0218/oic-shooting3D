#include "GameDefine.h"
#include "JsonUtil.h"





float InterpolationAmin(float animtime, Anim_Data* animdata, int cnt)
{
    int phase = 0;
    for (phase = 1; phase < cnt; phase++) {
        if (animtime < animdata[phase].fTime) {
            break;
        }
    }
    phase = MOF_MIN(phase, cnt - 1);
    
    float nt = animtime - animdata[phase - 1].fTime;
    float at = animdata[phase].fTime - animdata[phase - 1].fTime;
    float t = nt / at;
    
    t = MOF_CLIPING(t, 0.0f, 1.0f);

    switch (animdata[phase].EasingType)
    {
    case Ease_Sine_In:
        t = 1.0f - MOF_COS(MOF_MATH_HALFPI * t);
        break;
    case Ease_Sine_Out:
        t = MOF_SIN(MOF_MATH_HALFPI * t);
        break;
    case Ease_Sine_InOut:
        t = 0.5f - 0.5f * MOF_COS(MOF_MATH_PI * t);
        break;
    case Ease_Linear:
    default:
        break;
    }

    float from = animdata[phase - 1].fValue;
    float to = animdata[phase].fValue;

    return from + (to - from) * t;
}

//静的メンバの実体生成。関数と同じように実装しないとリンカーがエラーを出す。

CVector3 CGameDefine::m_FiledHalf;
float	CGameDefine::m_BackgroundScrollSpeed;
float CGameDefine::m_PlayerSpeed;
int CGameDefine::m_PlayerRotateSpeed;
float CGameDefine::m_PlayerByulletSpeed;
int CGameDefine::m_PlayerShotCount;
int CGameDefine::m_PlayerShotInterval;
int CGameDefine::m_EnemyCount;
float CGameDefine::m_EnemyBulletSpeed;
int CGameDefine::m_MaxPlayerHP;
float CGameDefine::m_Boss1PatternChangeInterval;
int  CGameDefine::m_Boss1MaxHp;
int CGameDefine::m_EnemyHP;

void CGameDefine::LoadParameter()
{
    m_FiledHalf.x = CJsonUtil::FindFloatFromJson("params.json", "field_half_x");
    m_FiledHalf.y = CJsonUtil::FindFloatFromJson("params.json", "field_half_y");
    m_FiledHalf.z = CJsonUtil::FindFloatFromJson("params.json", "field_half_z");
    m_BackgroundScrollSpeed = CJsonUtil::FindFloatFromJson("params.json", "background_scroll_speed");
    m_PlayerSpeed = CJsonUtil::FindFloatFromJson("params.json", "player_speed");
    m_PlayerRotateSpeed = CJsonUtil::FindIntFromJson("params.json", "player_rotation_speed");
    m_PlayerByulletSpeed = CJsonUtil::FindFloatFromJson("params.json", "player_bullet_speed");
    m_PlayerShotCount = CJsonUtil::FindIntFromJson("params.json", "player_shot_count");
    m_PlayerShotInterval = CJsonUtil::FindIntFromJson("params.json", "player_shot_interval");
    m_EnemyCount = CJsonUtil::FindIntFromJson("params.json", "enemy_count");
    m_EnemyBulletSpeed = CJsonUtil::FindFloatFromJson("params.json", "enemy_bullet_speed");
    m_MaxPlayerHP = CJsonUtil::FindIntFromJson("params.json", "player_max_hp");
    m_Boss1PatternChangeInterval = CJsonUtil::FindFloatFromJson("params.json", "boss1_pattern_change_interval");
    m_Boss1MaxHp = CJsonUtil::FindIntFromJson("params.json", "boss1_max_hp");
    m_MaxPlayerHP = CJsonUtil::FindIntFromJson("params.json", "player_max_hp");
    m_EnemyHP = CJsonUtil::FindIntFromJson("params.json", "enemey_hp");
}

float CGameDefine::GetBackgroundScrollSpeed()
{
    return m_BackgroundScrollSpeed;
}

float CGameDefine::GetPlayerRotationSpeed()
{
    return m_PlayerRotateSpeed;
}

float CGameDefine::GetFiledHalfX()
{
    return m_FiledHalf.x;
}

float CGameDefine::GetFiledHalfY()
{
    return m_FiledHalf.y;
}

float CGameDefine::GetFiledHalfZ()
{
    return m_FiledHalf.z;
}

int CGameDefine::GetBoss1MaxHP()
{
    return m_Boss1MaxHp;
}

int CGameDefine::GetPlayerMaxHP()
{
    return m_MaxPlayerHP;
}

int CGameDefine::GetEnemyHP()
{
    return m_EnemyHP;
}
