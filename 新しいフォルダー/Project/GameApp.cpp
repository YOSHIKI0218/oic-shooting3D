/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Particle.h"
#include	"Emitter.h"

//�e�N�X�`���N���X�̐錾

CTexture				gTexture;

//�p�[�e�B�N���̐錾

std::list< CParticle* >		gParticleList;

CEmitter<MINMAXPARAMETER>	gEmitter;

//�J����
CCamera					gCamera;
//�J�����ʒu
CVector3				gCPos(0, 0, -5);
//�J���������_�ʒu
CVector3				gTPos(0, 0, 0);
//�J�����̏���x�N�g��
CVector3				gCUp(0, 1, 0);
/**
 * �J�����X�V
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

	//�}�E�X���͂ŃJ��������
	CVector3 mv;
	g_pInput->GetMouseMove(mv);
	//�z�C�[���ɂ��J�����̃Y�[������
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
	//���N���b�N�ɂ��J�����ړ�
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
	//�E�N���b�N�ɂ��J������]
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

//����
CDirectionalLight		gLight;

/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");

	//�J�����̐ݒ�
	gCamera.SetViewPort();
	gCamera.LookAt(gCPos, gTPos, gCUp);
	gCamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
	gCamera.Update();
	CGraphicsUtilities::SetCamera(&gCamera);

	//��{�����̐ݒ�
	gLight.SetDirection(Vector3(-1, -1, 1));
	gLight.SetDiffuse(MOF_COLOR_WHITE);
	gLight.SetAmbient(MOF_COLOR_HWHITE);
	CGraphicsUtilities::SetDirectionalLight(&gLight);

	//�e�N�X�`���̓ǂݍ���
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

	

	//�p�[�e�B�N���̓���p�����[�^�[�ݒ�
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
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();

	//�L�[���͂ŃJ��������
	UpdateCamera();

	//�p�[�e�B�N���̍X�V
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
	//�I�����Ă���Ȃ�΍Đ���
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
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`��J�n
	g_pGraphics->RenderStart();
	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f,0.0f,1.0f,0.0f,1.0f,0);

	//�[�x�o�b�t�@��L���ɂ���
	g_pGraphics->SetDepthEnable(TRUE);

	//�O���b�h��`�悷��
	CGraphicsUtilities::RenderGrid(2, 20, MOF_COLOR_WHITE, PLANEAXIS_ALL);

	//�p�[�e�B�N���̕`��
	for (std::list<CParticle*>::iterator it = gParticleList.begin(); it != gParticleList.end(); ++it) {
		(*it)->Render();
	}

	//�`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){
	//�p�[�e�B�N���̉��
	for (std::list<CParticle*>::iterator it = gParticleList.begin(); it != gParticleList.end();) {
		delete (*it);
		it = gParticleList.erase(it);
	}
	//�e�N�X�`���̉��
	gTexture.Release();
	return TRUE;
}