#include "GameScene.h"
#include	"Player.h"
#include	"CStage.h"
#include	"Stage1.h"
#include	"Stage2.h"
#include	"Stage3.h"
#include	"Boss.h"
#include    "GameUI.h"
#include	"Boss1.h"
#include	"EffectManager.h"
#include	"Background.h"
#include	"HealItem.h"

//外部参照(実体はGameApp.cpp)
extern int				gChangeSceneNo;

//現在のステージ番号
int						gStageNo;
int						gPrevStageNo;

LPCamera gCamera;

// ライト
CDirectionalLight	gLight;
// プレイヤー
CPlayer				gPlayer;

CEnemy				gEnemyArray[ENEMY_COUNT];

CItemBase*			gItemArray[1];

//ステージ
CStageBase*			gpStage1;
CStageBase*			gpStage2;
CStageBase*			gpStage3;

#define				ENEMYSHOT_COUNT	(200)

CEnemyShot			gShotArray[ENEMYSHOT_COUNT];
CMeshContainer		gEnemyShotMesh;

CGameUI				gUI;

// デバッグ表示フラグ
bool				gbDebug = false;



CTexture gTexture;

//CBoss1 gBoss1;

CSoundBuffer gExplosionSoundBuff;
CSoundBuffer	gHitSoundbuff;

Vector3 gCamAngle(0, 0, 0);

CGameScene::CGameScene() :
	gbClear(false),
	gbEnemyDestroyed(false),
	m_bFirstInialize(true)
{
}

CGameScene::~CGameScene()
{
}

void CGameScene::Load()
{
	//ステージクラスのインスタンスの生成
	gpStage1 = new CStage();
	gpStage2 = new CStage2();
	gpStage3 = new CStage3();
	//リソース読込
	gEnemyShotMesh.Load("eshot1.mom");
	gTexture.Load("Textures/Effects/Effect02.png");
	
	gItemArray[0] = new CHealItem();

	for (int i = 0; i < 1; i++)
	{
		if (gItemArray[i])
		{
			gItemArray[i]->Load();
		}
	}

	// プレイヤーの素材読み込み
	gPlayer.Load();
	gUI.Load();
	gpStage1->Load();
	gpStage2->Load();
	gpStage3->Load();
	gExplosionSoundBuff.Load("big-explosion-1.mp3");
	gHitSoundbuff.Load("shot_hit.mp3");
	CBackground::Instance().Load();
	
}

void CGameScene::Initalize()
{
	InitializeCamera();
	InitializeLight();

	gExplosionSoundBuff.SetLoop(false);
	gExplosionSoundBuff.SetVolume(0.5f);

	CBackground::Instance().Initalize();

	gHitSoundbuff.SetVolume(0.5f);
	gHitSoundbuff.SetLoop(false);
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		//注意! ここでSetMeshをしないとあとでメモリアクセス違反になる。
		gShotArray[i].SetMesh(&gEnemyShotMesh);

	}
	//注意! WinMainでMOF_FRAMEWORK_SOUNDINITIALIZEフラグを使ってサウンドの初期化をしないとサウンドは使用不能。
	
	//UIにプレイヤーとボス1のポインタを渡す
	gUI.SetPlayerPtr(&gPlayer);
	
	//初期値を設定
	Reset();
}

void CGameScene::Update()
{
	// プレイヤーの更新
	if (gPlayer.IsDead())
	{
		gChangeSceneNo = SCENENO_GAMEOVER;
		return;
	}
	else if (gbClear)
	{
		gChangeSceneNo = SCENENO_GAMECLEAR;
		return;
	}
	CBackground::Instance().Update();

	//アイテム更新
	for (int i = 0; i < 1; i++)
	{
		if (gItemArray[i])
		{
			gItemArray[i]->Update();
		}
	}

	//プレイヤーの更新
	gPlayer.Update();
	gPlayer.CollisionItems();



	//ステージ更新
	switch (gStageNo)
	{
	case 1:
		gpStage1->Update(ENEMY_COUNT, ENEMYSHOT_COUNT);
		break;
	case 2:
		gpStage2->Update(ENEMY_COUNT, ENEMYSHOT_COUNT);
		break;
	case 3:
		gpStage3->Update(ENEMY_COUNT, ENEMYSHOT_COUNT);
		break;
	default:
		break;
	}

	//ステージ3が開始すれば回復アイテム出現
	if (gStageNo != gPrevStageNo && gStageNo == 3)
	{
		gItemArray[0]->Start(FRONT_VECTOR3 * CGameDefine::GetFiledHalfZ() * 0.25f);
	}

	//雑魚更新
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].SetTargetPos(gPlayer.GetPosition());
		gEnemyArray[i].Update(gShotArray, ENEMYSHOT_COUNT);
		gPlayer.CollisionEnemy(gEnemyArray[i]);
	}

	//敵弾更新
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Update();
		gShotArray[i].SetPlayerPos(gPlayer.GetPosition());
		gPlayer.CollisioEnemyShot(gShotArray[i]);
	}

	// デバッグ表示の切り替え
	if (g_pInput->IsKeyPush(MOFKEY_F2))
	{
		gbDebug = !gbDebug;
	}
	

	//ステージ番号が4ならゲームクリア
	if (!gbClear && gStageNo == 4)
	{
		gbClear = true;
	}
	//Shift+Enterで最初から
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && g_pInput->IsKeyHold(MOFKEY_LSHIFT))
		Reset();

	//いくつかの更新処理
	CEffectManager::Update();
	gUI.UpdatePlayerHpSlider();
	gUI.UpdateBoss1Slider();
	UpdateCamera();

	gPrevStageNo = gStageNo;
}

void CGameScene::Render()
{
	// 深度バッファ有効化
	g_pGraphics->SetDepthEnable(TRUE);

	CBackground::Instance().Render();
	// プレイヤー描画
	gPlayer.Render();
	//ステージ
	switch (gStageNo)
	{
	case 1:
		gpStage1->Render();
		break;
	case 2:
		gpStage2->Render();
		break;
	case 3:
		gpStage3->Render();
		break;
	default:
		break;
	}

	//アイテム
	for (int i = 0; i < 1; i++)
	{
		if (gItemArray[i])
		{
			gItemArray[i]->Render();
		}
	}

	//敵
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Render();
	}
	//エフェクト
	CEffectManager::Render();
	//弾
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Render();
		if (gbDebug)
			gShotArray[i].RenderDebug();
	}

	// 3Dデバッグ描画
	if (gbDebug)
	{
		// 移動可能範囲の表示
		CMatrix44 matWorld;
		matWorld.Scaling(CGameDefine::GetFiledHalfX() * 2, 1, CGameDefine::GetFiledHalfZ() * 2);
		CGraphicsUtilities::RenderPlane(matWorld, Vector4(1, 1, 1, 0.4f));
	}

	// 深度バッファ無効化
	g_pGraphics->SetDepthEnable(FALSE);
	gUI.Render();
	// 2Dデバッグ描画
	if (gbDebug)
	{
		// プレイヤーのデバッグ文字描画
		gpStage1->RenderDebug2D();
		gPlayer.RenderDebug3D();
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			gEnemyArray[i].RenderDebug3D(i);
		}
	}

}

void CGameScene::Release()
{
	CBackground::Instance().Release();
	gPlayer.Release();
	gEnemyShotMesh.Release();
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Release();
	}
	gExplosionSoundBuff.Release();
	gHitSoundbuff.Release();
	gUI.Release();
	CEffectManager::Release();

	//アイテム
	for (int i = 0; i < 1; i++)
	{
		if (gItemArray[i])
		{
			gItemArray[i]->Release();
			delete gItemArray[i];
		}
	}

	//ステージ解放
	gpStage1->Release();
	gpStage2->Release();
	gpStage3->Release();

	//インスタンスの破棄
	delete gpStage1;
	delete gpStage2;
	delete gpStage3;
}

void CGameScene::UpdateCamera()
{
	// プレイヤーの動きに合わせてカメラを動かす
	float posX = gPlayer.GetPosition().x * 0.4f;

	CVector3 cpos = gCamera->GetViewPosition();
	CVector3 tpos = gCamera->GetTargetPosition();
	CVector3 vup = CVector3(0, 1, 0);
	cpos.x = posX;
	tpos.x = posX;
	vup.RotationZ(gPlayer.GetPosition().x / CGameDefine::GetFiledHalfX() * MOF_ToRadian(10.0f));
	gCamera->LookAt(cpos, tpos, vup);
	gCamera->Update();
}

bool CGameScene::IsDead()
{
	return gPlayer.IsDead();
}

void CGameScene::Reset()
{
	InitalizeEffects();
	gStageNo = 1;
	gbEnemyDestroyed = false;
	gbClear = false;
	gPlayer.Initialize();
	gpStage1->Initialize(&gStgEnemyStart);
	gpStage2->Initialize(&gStgEnemyStart);
	gpStage3->Initialize(&gStg2EnemyStart);
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Initialize();
	}

	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Initalize();
	}
	
	gPlayer.SetMaxHP(CGameDefine::GetPlayerMaxHP(), true);
	gUI.Initalize();

	InitializeCamera();
	CBackground::Instance().SetScroll(0.0f);
}

void CGameScene::InitalizeEffects()
{
	//初回時に爆発を8個作成
	if (!m_bFirstInialize)
		return;

	SpriteAnimationCreate anim = {
"エフェクト",
0, 0,
120, 120,
FALSE, {
	{ 3, 0, 0 }, { 3, 1, 0 }, { 3, 2, 0 }, { 3, 3, 0 }, { 3, 4, 0 },
	{ 3, 5, 0 }, { 3, 6, 0 }, { 3, 7, 0 }, { 3, 8, 0 }, { 3, 9, 0 },
	{ 3, 10, 0 }, { 3, 11, 0 }, { 3, 12, 0 }, { 3, 13, 0 }, { 3, 14, 0 }, { 3, 15, 0 }, }
	};
	for (int i = 0; i < 8; i++)
	{

		CParticle::CREATEINFO cinfo;
		cinfo.ID = EFFECT_EXPLOSION;
		cinfo.pBuffer = &gExplosionSoundBuff;
		cinfo.State.Scale = CVector3(2, 2, 2);
		cinfo.State.Position = ZERO_VECTOR3;
		cinfo.ParameterBehavior.Motion = anim;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.UpdateBehavior.Color.a = -1 / 30.0f;
		cinfo.ParameterBehavior.Existence = 30;
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.pTexture = &gTexture;
		CEffectManager::Create(cinfo);
	}
	anim = {
"エフェクト",
0, 0,
120, 120,
FALSE, {
	{ 1, 0, 0 }, { 1, 1, 0 }, { 1, 2, 0 }, { 1, 3, 0 }, { 1, 4, 0 },
	{ 1, 5, 0 }, { 1, 6, 0 }, { 1, 7, 0 }, { 1, 8, 0 }, { 1, 9, 0 },
	{ 1, 10, 0 }, { 1, 11, 0 }, { 1, 12, 0 }, { 1, 13, 0 }, { 1, 14, 0 }, { 1, 15, 0 }, }
	};
	for (int i = 0; i < 8; i++)
	{

		CParticle::CREATEINFO cinfo;
		cinfo.ID = EFFECT_HIT;
		cinfo.pBuffer = &gHitSoundbuff;
		cinfo.State.Scale = CVector3(1, 1, 1);
		cinfo.State.Position = ZERO_VECTOR3;
		cinfo.ParameterBehavior.Motion = anim;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.UpdateBehavior.Color.a = -1 / 10.0f;
		cinfo.ParameterBehavior.Existence = 10;
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.pTexture = &gTexture;
		CEffectManager::Create(cinfo);
	}
}

void CGameScene::InitializeCamera()
{
	gCamera = CGraphicsUtilities::GetCamera();
	// カメラ初期化
	gCamera->SetViewPort();
	//                    カメラの位置、　ターゲットの位置　、上
	gCamera->LookAt(Vector3(0, 6.0f, -17.0f), Vector3(0, 0, -10), Vector3(0, 1, 0));
	gCamera->PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(gCamera);
}

void CGameScene::InitializeLight()
{
	// ライト初期化
	gLight.SetDirection(Vector3(-1, -2, 1.5f));
	gLight.SetDiffuse(MOF_COLOR_WHITE);
	gLight.SetAmbient(MOF_COLOR_HWHITE);
	gLight.SetSpeculer(MOF_COLOR_WHITE);
	CGraphicsUtilities::SetDirectionalLight(&gLight);
}
