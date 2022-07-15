#pragma once

#include "GameDefine.h"
#include "CPlayerShot.h"
#include "CEnemy.h"
#include "Boss.h"
#include "Boss1.h"

enum BulletMode
{
	Mode_Normal,
	Mode_Laser,	
};

class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CVector3		m_vPos;
	float			m_fRotZ;
	float			m_fRoll;
	int				m_RoateSpeed;	
	BulletMode		m_Mode;

	CMeshContainer	m_ShotMesh;
	CPlayerShot		m_ShotArray[PLAYER_SHOT_COUNT];
	int				m_ShotWait;

	bool			m_bDead;
	int				m_HP;
	int				m_MaxHP;

public:
	CPlayer();
	~CPlayer();

	void Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebug3D();
	void Release();
		
	const CVector3 GetPosition();

	void UpdateMove();
	void UpdateRotate(int speed);	
	void UpdatePlayerShot_Normal();
	void UpdatePlayerShot_Laser();
	void UpdateShot();

	void CollisionEnemy(CEnemy& ene);
	void ConllisionBoss(CBoss& boss);
	void CollisioEnemyShot(CEnemyShot& shot);
	void CollisionBoss1(CBoss1& boss);
	bool IsDead() { return m_bDead; }
	void SetMaxHP(int value,bool overrideMaxHp);
	int GetMaxHP() { return m_MaxHP; }

	void Damage(int dmg);

	int GetHP() { return m_HP; }

	CSphere GetSphere() { return CSphere(m_vPos, 0.4f); }

	BulletMode	GetBulletMode() { return m_Mode; }

	void CollisionItems();

};