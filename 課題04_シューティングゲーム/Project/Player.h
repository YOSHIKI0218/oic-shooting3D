#pragma once

#include "GameDefine.h"
#include "PlayerShot.h"
#include "Enemy.h"
#include "Boss.h"

// 移動速度
#define PLAYER_SPEED		(0.1f)

// 弾の発射限界数
#define PLAYERSHOT_COUNT	(40)

// 弾の発射間隔
#define PLAYERSHOT_WAIT		(5)

class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CVector3		m_Pos;
	CVector3		m_Rot;

	bool			m_bDead;

	CMeshContainer	m_ShotMesh;
	CPlayerShot		m_ShotArray[PLAYERSHOT_COUNT];
	int				m_ShotWait;

	bool			m_TurnFlag;	//宙返り中のフラグ
	float			m_TurnTime;	//宙返りを開始してからの経過時間
	CVector3		m_TurnPos;	//宙返りを開始した座標
public:
	CPlayer();
	~CPlayer();
	bool Load();
	void Initialize();
	void Update(bool HorizonMode);
	void CollisionEnemy(CEnemy& ene);
	void CollisionEnemyShot(CEnemyShot& shot);
	void CollisionBoss(CBoss& boss);
	void Render();
	void RenderDebug();
	void RenderDebugText();
	const CVector3 GetPosition(){ return m_Pos; }
	CSphere GetSphere(){ return CSphere(m_Pos,0.4f); }
	bool IsDead(){ return m_bDead; }
	void Release();
};