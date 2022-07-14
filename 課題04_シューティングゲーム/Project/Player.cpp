#include "Player.h"

/**
 * コンストラクタ
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_Rot(0.0f,0.0f,0.0f),
m_bDead(false),
m_ShotMesh(),
m_ShotArray(),
m_ShotWait(){
}

/**
 * デストラクタ
 */
CPlayer::~CPlayer(){
}

/**
 * 読み込み
 */
bool CPlayer::Load(void){
	// メッシュの読み込み
	if(!m_Mesh.Load("player.mom"))
	{
		return false;
	}
	// 弾のメッシュ
	if(!m_ShotMesh.Load("pshot.mom"))
	{
		return false;
	}
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].SetMesh(&m_ShotMesh);
	}
	return true;
}

/**
 * 初期化
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0.0f,0.0f,-FIELD_HALF_Z+2.0f);
	m_Rot = Vector3(0.0f,0.0f,0.0f);
	m_bDead = false;
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Initialize();
	}
	m_TurnFlag = false;
	m_TurnTime = 0.0f;
	m_TurnPos = Vector3(0.0f,0.0f,0.0f);
}

/**
 * 更新
 */
void CPlayer::Update(bool HorizonMode){
	if(m_bDead)
	{
		return;
	}
	// 回転方向
	float Roll = 0;
	if(!m_TurnFlag)
	{
		// キーボードでの移動
		if(!HorizonMode)
		{
			//縦スクロール時の移動
			if(g_pInput->IsKeyHold(MOFKEY_LEFT))
			{
				m_Pos.x = max(m_Pos.x - PLAYER_SPEED,-FIELD_HALF_X);
				Roll -= MOF_MATH_PI;
			}
			if(g_pInput->IsKeyHold(MOFKEY_RIGHT))
			{
				m_Pos.x = min(m_Pos.x + PLAYER_SPEED,FIELD_HALF_X);
				Roll += MOF_MATH_PI;
			}
			if(g_pInput->IsKeyHold(MOFKEY_UP))
			{
				m_Pos.z = min(m_Pos.z + PLAYER_SPEED,FIELD_HALF_Z);
			}
			if(g_pInput->IsKeyHold(MOFKEY_DOWN))
			{
				m_Pos.z = max(m_Pos.z - PLAYER_SPEED,-FIELD_HALF_Z);
			}
		}
		else
		{
			//課題３－１　横スクロール時の移動
			//移動方向を左右キーでZ方向、上下キーでY方向に変更する。
			//プレイヤーの回転処理は上下キーの入力で実行する



		}
		
		//課題３－２　横スクロールへの補間
		//縦スクロール時はm_Pos.y、横スクロール時はm_Pos.xを0.1の速度で0に近づけていく



		//宙返り開始
		if(g_pInput->IsKeyPush(MOFKEY_LSHIFT))
		{
			m_TurnFlag = true;	//宙返り中のフラグ
			m_TurnPos = m_Pos;	//宙返りを開始した座標
			m_TurnTime = 0.0f;	//宙返りを開始してからの経過時間
		}
	}
	//宙返り
	else
	{
		//時間経過とイージング
		m_TurnTime += CUtilities::GetFrameSecond();
		float t = MOF_LINSTEP(0.0f,8,m_TurnTime);
		t = sinf(t * MOF_MATH_HALFPI);

		//課題１　宙返りの動作
		//縦に宙返りさせるため、m_Rot.xを時間(t)0～1の間に1回転＝360度（２π）回転させる
		//宙返りの中心はプレイヤーの宙返り開始座標の上に1.5の位置として、
		//プレイヤーの座標は半径1.5でX軸にm_Rot.x回転した座標になる


		m_Rot.x = -MOF_MATH_2PI * t;
		//m_Pos += CVector3(0,sin(),sin())
		//m_Pos.y = MOF_LERP(m_Pos.y + 0.75, cosf(MOF_MATH_2PI), t);
		//m_Pos.z = MOF_LERP(m_Pos.z, cosf(-MOF_MATH_QUADPI), t);
		
		CVector3 tmpPos = Vector3(0, -1.5, 0);
		tmpPos.RotationX(m_Rot.x);
		tmpPos.y += 1.5f;
		m_Pos = m_TurnPos + tmpPos;

		//宙返り終了
		if(1.0f <= t)
		{
			 
			m_TurnFlag = false;
			
		}
	}

	// 回転
	float RotSpeed = MOF_ToRadian(10);
	if(Roll == 0)
	{
		RotSpeed = min(abs(m_Rot.z)*0.1f,RotSpeed);
	}
	if(abs(m_Rot.z) <= RotSpeed || signbit(m_Rot.z) != signbit(Roll))
	{
		m_Rot.z += Roll;
	}
	m_Rot.z -= copysignf(min(RotSpeed,abs(m_Rot.z)),m_Rot.z);
	
	// 弾の発射
	if(m_ShotWait <= 0)
	{
		if(g_pInput->IsKeyHold(MOFKEY_SPACE) && !m_TurnFlag)
		{
			for(int cnt = 0; cnt < 2; cnt++)
			{
				for(int i = 0; i < PLAYERSHOT_COUNT; i++)
				{
					if(m_ShotArray[i].GetShow())
					{
						continue;
					}
					CVector3 ShotPos(0.4f * (cnt * 2 - 1), 0, 0);
					ShotPos.RotationZ(m_Rot.z);
					ShotPos += m_Pos;
					m_ShotWait = PLAYERSHOT_WAIT;
					m_ShotArray[i].Fire(ShotPos);
					break;
				}
			}
		}
	}
	else
	{
		m_ShotWait--;
	}
	// 弾の更新
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Update();
	}
}

/**
 * 敵との当たり判定
 * 引数の敵に対して当たり判定を実行する。
 *
 * 引数
 * [in]			ene				判定を行う敵
 */
void CPlayer::CollisionEnemy(CEnemy& ene){
	if(!ene.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere es = ene.GetSphere();
	if(ps.CollisionSphere(es))
	{
		m_bDead = true;
	}
	// 弾との判定
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if(!m_ShotArray[i].GetShow())
		{
			continue;
		}
		CSphere ss = m_ShotArray[i].GetSphere();
		if(ss.CollisionSphere(es))
		{
			ene.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
}

/**
 * 敵弾との当たり判定
 * 引数の敵弾に対して当たり判定を実行する。
 *
 * 引数
 * [in]			shot			判定を行う敵弾
 */
void CPlayer::CollisionEnemyShot(CEnemyShot& shot){
	CSphere ps = GetSphere();
	if(!shot.GetShow())
	{
		return;
	}
	CSphere ss = shot.GetSphere();
	if(ss.CollisionSphere(ps))
	{
		m_bDead = true;
		shot.SetShow(false);
	}
}

/**
 * 当たり判定
 * 引数のボスに対して当たり判定を実行する。
 *
 * 引数
 * [in]			boss			判定を行うボス
 */
void CPlayer::CollisionBoss(CBoss& boss){
	if(!boss.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere bs = boss.GetSphere();
	if(ps.CollisionSphere(bs))
	{
		m_bDead = true;
	}
	// 弾との判定
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if(!m_ShotArray[i].GetShow())
		{
			continue;
		}
		CSphere ss = m_ShotArray[i].GetSphere();
		if(ss.CollisionSphere(bs))
		{
			boss.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
	// パーツとの判定
	for(int i = 0; i < BOSS_PARTS_MAX; i++)
	{
		CollisionEnemy(boss.GetParts(i));
	}
}

/**
 * 描画
 */
void CPlayer::Render(void){
	if(m_bDead)
	{
		return;
	}
	// ワールド行列作成
	CMatrix44 matWorld;
	matWorld.RotationZXY(m_Rot);
	matWorld.SetTranslation(m_Pos);
	// メッシュの描画
	m_Mesh.Render(matWorld);

	// 弾の描画
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Render();
	}
}

/**
 * デバッグ描画
 */
void CPlayer::RenderDebug(void){
	// 当たり判定の表示
	CGraphicsUtilities::RenderSphere(GetSphere(),Vector4(0,1,0,0.3f));
	// 弾の描画
	for(int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].RenderDebug();
	}
}

/**
 * デバッグ文字描画
 */
void CPlayer::RenderDebugText(void){
	// 位置の描画
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"プレイヤー位置 X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
}

/**
 * 解放
 */
void CPlayer::Release(void){
	m_Mesh.Release();
	m_ShotMesh.Release();
}