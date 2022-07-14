#include	"Particle.h"

/**
* コンストラクタ
*
*/
CParticle::CParticle() :
m_Time(0.0f) ,
m_State() ,
m_InitializeState() ,
m_UpdateBehavior() ,
m_ParameterBehavior() ,
m_EnvironmentBehavior() ,
m_pTexture(NULL) ,
m_pMesh(NULL),
m_ViewPosition() {
}

/**
* デストラクタ
*
*/
CParticle::~CParticle(){
	Release();
}

/**
* 生成情報からパーティクルを初期化する
*
 * 引数
 * [in]			info			生成情報
*/
void CParticle::Initialize(CREATEINFO& info){
	m_Time = 0;
	m_InitializeState = m_State = info.State;
	m_UpdateBehavior = info.UpdateBehavior;
	m_ParameterBehavior = info.ParameterBehavior;
	m_EnvironmentBehavior = info.EnvironmentBehavior;
	m_pTexture = info.pTexture;
	m_pMesh = info.pMesh;
}

/**
* 更新
*
*/
void CParticle::Update(void){
	//存在時間の更新
	m_Time++;
	//終了判定
	if (m_Time >= m_ParameterBehavior.Existence)
	{
		return;
	}

	//挙動にあわせて更新
	m_State.Position += m_UpdateBehavior.Move + m_EnvironmentBehavior.Gravity * m_Time;
	m_State.Scale += m_UpdateBehavior.Scale;
	m_State.Rotation += m_UpdateBehavior.Rotation;
	m_State.Color += m_UpdateBehavior.Color;

	//行列の更新
	m_State.World.RotationZXY(m_State.Rotation);
	m_State.World.Scaling(m_State.Scale, m_State.World);
	m_State.World.SetTranslation(m_State.Position);
}

/**
* 描画
*
*/
void CParticle::Render(void){
	if (m_Time >= m_ParameterBehavior.Existence)
	{
		return;
	}
	if (m_State.Color.w <= 0.0f)
	{
		return;
	}
	g_pGraphics->SetBlending(m_ParameterBehavior.Blending);
	//描画用マトリクスの宣言
	CMatrix44 rMat = m_State.World;
	//ビルボードマトリクスをワールドマトリクスに適用する
	if (m_ParameterBehavior.Flag & BILLBOARD)
	{
		//カメラを取得する
		LPCamera pCam = CGraphicsUtilities::GetCamera();
		rMat.Multiply3x3(pCam->GetBillBoardMatrix());
	}
	if (m_pMesh)
	{
		m_pMesh->Render(rMat, m_State.Color);
	}
	if (m_pTexture)
	{
		m_pTexture->Render(rMat, m_State.Color.ToU32Color());
	}
}

/**
* 解放
*
*/
void CParticle::Release(void){
}