#include "Stage.h"

/**
 * コンストラクタ
 *
 */
CStage::CStage():
m_BG(),
m_Scroll(0.0f),
m_pEnemyStart(NULL),
m_EnemyMesh(),
m_EnemyNo(0){
}

/**
 * デストラクタ
 *
 */
CStage::~CStage(){
}

/**
 * 読み込み
 *
 */
bool CStage::Load(){
	// 背景テクスチャの読み込み
	if(!m_BG.CreateSprite("road.png"))
	{
		return false;
	}
	// ステージ内で登場する敵のメッシュを読み込む
	if(!m_EnemyMesh.Load("enemy.mom"))
	{
		return false;
	}
	return true;
}

/**
 * 初期化
 *
 * 引数
 * [in]			pSt				敵の開始情報配列
 */
void CStage::Initialize(ENEMYSTART *pSt){
	m_BG.m_Angle.x = MOF_MATH_HALFPI;
	m_BG.m_Angle.y = MOF_MATH_HALFPI;
	m_BG.m_Scale = Vector3(200,200,200);
	m_BG.m_Position.y = -15;
	m_BG.SetImageRect(0,0,m_BG.GetTexture()->GetWidth() * 4, m_BG.GetTexture()->GetHeight() * 4);
	m_Scroll = 0;
	m_EnemyNo = 0;
	m_pEnemyStart = pSt;
	m_HorizonMode = false;
	m_ModeNo = 0;
}

/**
 * 更新
 *
 * 引数
 * [in]			ene			敵の配列
 * [in]			ecnt		敵の配列の数
 */
void CStage::Update(CEnemy* ene,int ecnt){
	m_Scroll += SCROLL_SPEED;
	
	if(m_ModeNo < m_pEnemyStart->ChangeModeCount && m_Scroll >= m_pEnemyStart->ChangeMode[m_ModeNo])
	{
		m_HorizonMode = !m_HorizonMode;
		m_ModeNo++;
	}

	//敵の出現
	if(m_EnemyNo < m_pEnemyStart->Count && m_Scroll >= m_pEnemyStart->Scroll[m_EnemyNo])
	{
		for(int i = 0; i < ecnt; i++)
		{
			if(ene[i].GetShow())
			{
				continue;
			}
			ene[i].SetMesh(&m_EnemyMesh);
			ene[i].Start(Vector3(m_pEnemyStart->PosX[m_EnemyNo],0,0),0,m_HorizonMode);
			break;
		}
		m_EnemyNo++;
	}
}

/**
 * 描画
 *
 */
void CStage::Render(){
	for(int i = 0; i < 2; i++)
	{
		m_BG.m_Position.z = 50.0f + fmodf(-m_Scroll,m_BG.m_Scale.z) + i * m_BG.m_Scale.z;
		m_BG.Update();
		m_BG.Render();
	}
}

/**
 * デバッグ文字描画
 *
 */
void CStage::RenderDebugText(){
	//スクロール値の描画
	CGraphicsUtilities::RenderString(10,10,MOF_XRGB(0,0,0),
		"スクロール : %.0f , 敵の出現番号 : %d",m_Scroll,m_EnemyNo);
	CGraphicsUtilities::RenderString(580,10,MOF_XRGB(0,0,0),
		"モード変更番号 : %d , スクロール方向 : %s",m_ModeNo,m_HorizonMode?"横":"縦");
}

/**
 * 解放
 *
 */
void CStage::Release(){
	m_BG.Release();
	m_EnemyMesh.Release();
}
