#pragma once

#include "GameDefine.h"
#include "Enemy.h"

// スクロール速度
#define		SCROLL_SPEED	(1.0f)

// 敵の出現情報構造体
typedef struct tag_ENEMYSTART{
	int					Count;
	float*				PosX;
	float*				Scroll;
	int					ChangeModeCount;
	float*				ChangeMode;
}ENEMYSTART;

class CStage{
private:
	CSprite3D			m_BG;
	float				m_Scroll;

	ENEMYSTART*			m_pEnemyStart;
	CMeshContainer		m_EnemyMesh;
	int					m_EnemyNo;

	bool				m_HorizonMode;	//trueなら横スクロール
	int					m_ModeNo;
public:
	CStage();
	~CStage();
	bool Load();
	void Initialize(ENEMYSTART *pSt);
	void Update(CEnemy* ene,int ecnt);
	void Render();
	void RenderDebugText();
	bool IsAllEnemy(){ return m_EnemyNo >= m_pEnemyStart->Count; }
	bool IsHorizonMode(){ return m_HorizonMode; }
	void Release();
};