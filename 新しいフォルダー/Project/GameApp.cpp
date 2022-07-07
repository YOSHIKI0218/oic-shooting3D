/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Particle.h"
#include	"Emitter.h"

//テクスチャクラスの宣言

CTexture				gTexture;

//パーティクルの宣言

std::list< CParticle* >		gParticleList;

CEmitter<MINMAXPARAMETER>	gEmitter;

//カメラ
CCamera					gCamera;
//カメラ位置
CVector3				gCPos(0, 0, -5);
//カメラ注視点位置
CVector3				gTPos(0, 0, 0);
//カメラの上方ベクトル
CVector3				gCUp(0, 1, 0);
/**
 * カメラ更新
 */
void UpdateCamera(){
	if (g_pInput->IsKeyHold(MOFKEY_LEFT))
	{
		CVector3 vv = gCPos - gTPos;
		vv.RotationY(0.01f);
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	else if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		CVector3 vv = gCPos - gTPos;
		vv.RotationY(-0.01f);
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	if (g_pInput->IsKeyHold(MOFKEY_UP))
	{
		CVector3 vv = gCPos - gTPos;
		float d = sqrt(vv.x * vv.x + vv.z * vv.z);
		MofFloat sina = MOF_SIN(-0.01f);
		MofFloat cosa = MOF_COS(-0.01f);
		MofFloat tmpy = vv.y * cosa + d * -sina;
		MofFloat tmpxz = vv.y * sina + d * cosa;
		vv.y = tmpy;
		vv.x = (vv.x / d) * tmpxz;
		vv.z = (vv.z / d) * tmpxz;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	else if (g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		CVector3 vv = gCPos - gTPos;
		float d = sqrt(vv.x * vv.x + vv.z * vv.z);
		MofFloat sina = MOF_SIN(0.01f);
		MofFloat cosa = MOF_COS(0.01f);
		MofFloat tmpy = vv.y * cosa + d * -sina;
		MofFloat tmpxz = vv.y * sina + d * cosa;
		vv.y = tmpy;
		vv.x = (vv.x / d) * tmpxz;
		vv.z = (vv.z / d) * tmpxz;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}

	//マウス入力でカメラ操作
	CVector3 mv;
	g_pInput->GetMouseMove(mv);
	//ホイールによるカメラのズーム調整
	if (mv.z != 0)
	{
		CVector3 vv = gCPos - gTPos;
		vv *= 1.0f - mv.z * 0.0001f;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	else if (g_pInput->IsMouseKeyHold(2))
	{
		CVector3 vv = gCPos - gTPos;
		vv *= 1.0f + mv.y * 0.01f;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	//左クリックによるカメラ移動
	if (g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON))
	{
		CVector3 sv;
		CVector3 uv;
		CVector3 vv = gTPos - gCPos;
		vv.Normal(vv);
		vv.Cross(gCUp, sv);
		sv.Normal(sv);
		gCPos += sv * mv.x * 0.01f;
		gTPos += sv * mv.x * 0.01f;
		sv.Cross(vv, uv);
		uv.Normal(uv);
		gCPos += uv * mv.y * 0.01f;
		gTPos += uv * mv.y * 0.01f;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	//右クリックによるカメラ回転
	if (g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON))
	{
		CVector3 vv = gCPos - gTPos;
		vv.RotationY(mv.x * 0.01f);
		float d = sqrt(vv.x * vv.x + vv.z * vv.z);
		MofFloat sina = MOF_SIN(-mv.y * 0.01f);
		MofFloat cosa = MOF_COS(-mv.y * 0.01f);
		MofFloat tmpy = vv.y * cosa + d * -sina;
		MofFloat tmpxz = vv.y * sina + d * cosa;
		vv.y = tmpy;
		vv.x = (vv.x / d) * tmpxz;
		vv.z = (vv.z / d) * tmpxz;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
}

//光源
CDirectionalLight		gLight;

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");

	//カメラの設定
	gCamera.SetViewPort();
	gCamera.LookAt(gCPos, gTPos, gCUp);
	gCamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	gCamera.Update();
	CGraphicsUtilities::SetCamera(&gCamera);

	//基本光源の設定
	gLight.SetDirection(Vector3(-1, -1, 1));
	gLight.SetDiffuse(MOF_COLOR_WHITE);
	gLight.SetAmbient(MOF_COLOR_HWHITE);
	CGraphicsUtilities::SetDirectionalLight(&gLight);

	//テクスチャの読み込み
	gTexture.Load("Flash.dds");

	CEmitter<MINMAXPARAMETER>::CREATEINFO cinfo;
	cinfo.UpdateBehavior.Color.SetDefaultParameter(Vector4(0, 0, 0, -1 / 60.0f));

	cinfo.ParameterBehavior.Existence.SetDefaultParameter(60);
	cinfo.ParameterBehavior.Blending = BLEND_ADD;
	cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
	cinfo.ParameterBehavior.pTexture = &gTexture;

	cinfo.CreateBehavior.Position.Min.SetValue(-5, -5, -5);
	cinfo.CreateBehavior.Position.Max.SetValue(5, 5, 5);

	cinfo.CreateBehavior.Color.SetDefaultParameter(Vector4(1, 1, 1, 1));
	cinfo.CreateBehavior.Scale.SetDefaultParameter(Vector3(1, 1, 1));

	cinfo.CreateBehavior.Time = 30;
	cinfo.CreateBehavior.Count = 10;
	gEmitter.Initialize(cinfo);

	

	//パーティクルの動作パラメーター設定
	/*CParticle::CREATEINFO cinfo;
	cinfo.UpdateBehavior.Color.a = -1/60.0f;
	cinfo.ParameterBehavior.Existence = 60;
	cinfo.ParameterBehavior.Blending = BLEND_ADD;
	cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
	cinfo.pTexture = &gTexture;
	gParticle.Initialize(cinfo);*/
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();

	//キー入力でカメラ操作
	UpdateCamera();

	//パーティクルの更新
	gEmitter.Update(&gParticleList);
	for (std::list<CParticle*>::iterator it = gParticleList.begin(); it != gParticleList.end();) {
		(*it)->Update();
		if ((*it)->IsEnd()) {
			delete (*it);
			it = gParticleList.erase(it);
			continue;
		}
		++it;
	}
	//終了しているならば再生成
	/*if (gParticle.IsEnd()) {
		CParticle::CREATEINFO cinfo;
		cinfo.UpdateBehavior.Color.a = -1 / 60.0f;
		cinfo.ParameterBehavior.Existence = 60;
		cinfo.ParameterBehavior.Blending = BLEND_ADD;
		cinfo.ParameterBehavior.Flag = CParticle::BILLBOARD;
		cinfo.pTexture = &gTexture;
		gParticle.Initialize(cinfo);
	}*/
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f,0.0f,1.0f,0.0f,1.0f,0);

	//深度バッファを有効にする
	g_pGraphics->SetDepthEnable(TRUE);

	//グリッドを描画する
	CGraphicsUtilities::RenderGrid(2, 20, MOF_COLOR_WHITE, PLANEAXIS_ALL);

	//パーティクルの描画
	for (std::list<CParticle*>::iterator it = gParticleList.begin(); it != gParticleList.end(); ++it) {
		(*it)->Render();
	}

	//描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void){
	//パーティクルの解放
	for (std::list<CParticle*>::iterator it = gParticleList.begin(); it != gParticleList.end();) {
		delete (*it);
		it = gParticleList.erase(it);
	}
	//テクスチャの解放
	gTexture.Release();
	return TRUE;
}