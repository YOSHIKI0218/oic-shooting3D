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

//�O���Q��(���̂�GameApp.cpp)
extern int				gChangeSceneNo;

//���݂̃X�e�[�W�ԍ�
int						gStageNo;
int						gPrevStageNo;

LPCamera gCamera;

// ���C�g
CDirectionalLight	gLight;
// �v���C���[
CPlayer				gPlayer;

CEnemy				gEnemyArray[ENEMY_COUNT];

CItemBase*			gItemArray[1];

//�X�e�[�W
CStageBase*			gpStage1;
CStageBase*			gpStage2;
CStageBase*			gpStage3;

#define				ENEMYSHOT_COUNT	(200)

CEnemyShot			gShotArray[ENEMYSHOT_COUNT];
CMeshContainer		gEnemyShotMesh;

CGameUI				gUI;

// �f�o�b�O�\���t���O
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
	//�X�e�[�W�N���X�̃C���X�^���X�̐���
	gpStage1 = new CStage();
	gpStage2 = new CStage2();
	gpStage3 = new CStage3();
	//���\�[�X�Ǎ�
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

	// �v���C���[�̑f�ޓǂݍ���
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
		//����! ������SetMesh�����Ȃ��Ƃ��ƂŃ������A�N�Z�X�ᔽ�ɂȂ�B
		gShotArray[i].SetMesh(&gEnemyShotMesh);

	}
	//����! WinMain��MOF_FRAMEWORK_SOUNDINITIALIZE�t���O���g���ăT�E���h�̏����������Ȃ��ƃT�E���h�͎g�p�s�\�B
	
	//UI�Ƀv���C���[�ƃ{�X1�̃|�C���^��n��
	gUI.SetPlayerPtr(&gPlayer);
	
	//�����l��ݒ�
	Reset();
}

void CGameScene::Update()
{
	// �v���C���[�̍X�V
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

	//�A�C�e���X�V
	for (int i = 0; i < 1; i++)
	{
		if (gItemArray[i])
		{
			gItemArray[i]->Update();
		}
	}

	//�v���C���[�̍X�V
	gPlayer.Update();
	gPlayer.CollisionItems();



	//�X�e�[�W�X�V
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

	//�X�e�[�W3���J�n����Ή񕜃A�C�e���o��
	if (gStageNo != gPrevStageNo && gStageNo == 3)
	{
		gItemArray[0]->Start(FRONT_VECTOR3 * CGameDefine::GetFiledHalfZ() * 0.25f);
	}

	//�G���X�V
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].SetTargetPos(gPlayer.GetPosition());
		gEnemyArray[i].Update(gShotArray, ENEMYSHOT_COUNT);
		gPlayer.CollisionEnemy(gEnemyArray[i]);
	}

	//�G�e�X�V
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Update();
		gShotArray[i].SetPlayerPos(gPlayer.GetPosition());
		gPlayer.CollisioEnemyShot(gShotArray[i]);
	}

	// �f�o�b�O�\���̐؂�ւ�
	if (g_pInput->IsKeyPush(MOFKEY_F2))
	{
		gbDebug = !gbDebug;
	}
	

	//�X�e�[�W�ԍ���4�Ȃ�Q�[���N���A
	if (!gbClear && gStageNo == 4)
	{
		gbClear = true;
	}
	//Shift+Enter�ōŏ�����
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && g_pInput->IsKeyHold(MOFKEY_LSHIFT))
		Reset();

	//�������̍X�V����
	CEffectManager::Update();
	gUI.UpdatePlayerHpSlider();
	gUI.UpdateBoss1Slider();
	UpdateCamera();

	gPrevStageNo = gStageNo;
}

void CGameScene::Render()
{
	// �[�x�o�b�t�@�L����
	g_pGraphics->SetDepthEnable(TRUE);

	CBackground::Instance().Render();
	// �v���C���[�`��
	gPlayer.Render();
	//�X�e�[�W
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

	//�A�C�e��
	for (int i = 0; i < 1; i++)
	{
		if (gItemArray[i])
		{
			gItemArray[i]->Render();
		}
	}

	//�G
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		gEnemyArray[i].Render();
	}
	//�G�t�F�N�g
	CEffectManager::Render();
	//�e
	for (int i = 0; i < ENEMYSHOT_COUNT; i++)
	{
		gShotArray[i].Render();
		if (gbDebug)
			gShotArray[i].RenderDebug();
	}

	// 3D�f�o�b�O�`��
	if (gbDebug)
	{
		// �ړ��\�͈͂̕\��
		CMatrix44 matWorld;
		matWorld.Scaling(CGameDefine::GetFiledHalfX() * 2, 1, CGameDefine::GetFiledHalfZ() * 2);
		CGraphicsUtilities::RenderPlane(matWorld, Vector4(1, 1, 1, 0.4f));
	}

	// �[�x�o�b�t�@������
	g_pGraphics->SetDepthEnable(FALSE);
	gUI.Render();
	// 2D�f�o�b�O�`��
	if (gbDebug)
	{
		// �v���C���[�̃f�o�b�O�����`��
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

	//�A�C�e��
	for (int i = 0; i < 1; i++)
	{
		if (gItemArray[i])
		{
			gItemArray[i]->Release();
			delete gItemArray[i];
		}
	}

	//�X�e�[�W���
	gpStage1->Release();
	gpStage2->Release();
	gpStage3->Release();

	//�C���X�^���X�̔j��
	delete gpStage1;
	delete gpStage2;
	delete gpStage3;
}

void CGameScene::UpdateCamera()
{
	// �v���C���[�̓����ɍ��킹�ăJ�����𓮂���
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
	//���񎞂ɔ�����8�쐬
	if (!m_bFirstInialize)
		return;

	SpriteAnimationCreate anim = {
"�G�t�F�N�g",
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
"�G�t�F�N�g",
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
	// �J����������
	gCamera->SetViewPort();
	//                    �J�����̈ʒu�A�@�^�[�Q�b�g�̈ʒu�@�A��
	gCamera->LookAt(Vector3(0, 6.0f, -17.0f), Vector3(0, 0, -10), Vector3(0, 1, 0));
	gCamera->PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	CGraphicsUtilities::SetCamera(gCamera);
}

void CGameScene::InitializeLight()
{
	// ���C�g������
	gLight.SetDirection(Vector3(-1, -2, 1.5f));
	gLight.SetDiffuse(MOF_COLOR_WHITE);
	gLight.SetAmbient(MOF_COLOR_HWHITE);
	gLight.SetSpeculer(MOF_COLOR_WHITE);
	CGraphicsUtilities::SetDirectionalLight(&gLight);
}
