//============================================
//
//	�J�����̏��� [camera.cpp]
//	Author:sakamoto kai
//
//============================================
#include "camera.h"
#include "manager.h"
#include "title.h"
#include "game.h"
#include "tutorial.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "model.h"
#include "sound.h"

//�}�N����`
#define CAMERA_DISTANCE (100.0f)							//���_�ƒ����_�̋���
#define CAMERA_DISTANCE_EVENT (75.0f)						//�C�x���g���̎��_�ƒ����_�̋���
#define MODEL_DISTANCE (10.0f)								//���f���ƒ����_�̋���
#define CAMERA_SPEED (9.0f)									//�J�����̈ړ��X�s�[�h
#define CAMERA_VR_SPEED (0.015f)							//�J�����̎��_�X�s�[�h
#define CAMERA_PAD_VR_SPEED (0.015f)						//�J�����̃p�b�h�̎��_�X�s�[�h
#define CAMERA_HOMING (0.2f)								//�J�����̒Ǐ]�X�s�[�h
#define POS_HOMING (0.9f)									//�ʒu�ւ̒Ǐ]�X�s�[�h
#define DOWNVIEW_POSV (D3DXVECTOR3(0.0f, 1500.0f, -200.0f))	//�����낵�̎��_
#define DOWNVIEW_POSR (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//�����낵�̒����_
#define SIDEVIEW_POSV (D3DXVECTOR3(0.0f, 350.0f, -1000.0f))	//2D�̎��_
#define SIDEVIEW_POSR (D3DXVECTOR3(0.0f, 300.0f, 0.0f))		//2D�̒����_

#define ULTIMATE_DISTANCE (150.0f)							//�p�������̎��_�ƒ����_����
#define ULTIMATE_DISTANCEu (150.0f)							//�p�������̍���
#define ULTIMATE_ROT (0.5f)								//�p�������̌���

#define TITLE_POS_V (D3DXVECTOR3(-1300.0f, 550.0f, 300.0f))	//�^�C�g���J�����̈ʒu(���_)
#define TITLE_POS_R (D3DXVECTOR3(-0.0f, 400.0f, 0.0f))		//�^�C�g���J�����̈ʒu(�����_)
#define TTTLE_DECREASE (0.05f)								//�v���C���[���_�܂ł̋����̌������x
#define TTTLE_LENGTH (0.01f)								//�v���C���[���_�܂ł̂��ǂ蒅��������

//====================================================================
//�R���X�g���N�^
//====================================================================
CCamera::CCamera()
{
	m_bBib = false;
	m_fBibPowor = 0.0f;
	m_move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	m_posV = INITVECTOR3;
	m_posR = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_PlayerPos = INITVECTOR3;
	m_DelCameraPos = INITVECTOR3;
	ResetCamera();
	CameraMode = CAMERAMODE_CONTROL;
	m_fHomingSpeed = CAMERA_HOMING;

	m_viewport.X = 0;				// �`�悷���ʂ̍���X���W
	m_viewport.Y = 0;				// �`�悷���ʂ̍���Y���W
	m_viewport.Width = 1280;		// ��ʂ̕�
	m_viewport.Height = 720;		// ��ʂ̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	
#ifdef _DEBUG
	CameraMode = CAMERAMODE_CONTROL;
#endif

	m_FollowTime = 0;
	m_bFollowY = false;
	m_bAttention = false;
	m_fRotMove = INITVECTOR3;
	m_nCameraCount = -1;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CCamera::~CCamera()
{

}

//====================================================================
//�J�����̏���������
//====================================================================
HRESULT CCamera::Init(void)
{
	return S_OK;
}

//====================================================================
//�J�����̏I������
//====================================================================
void CCamera::Uninit(void)
{

}

//====================================================================
//�J�����̍X�V����
//====================================================================
void CCamera::Update(void)
{
#ifdef _DEBUG
	//�f�o�C�X�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	{
		if (CameraMode == CAMERAMODE_CONTROL)
		{
			CameraMode = CAMERAMODE_FPS;
		}
		else if (CameraMode == CAMERAMODE_FPS)
		{
			CameraMode = CAMERAMODE_CONTROL;
		}
		//else if (CameraMode == CAMERAMODE_FOLLOW)
		//{
		//	CameraMode = CAMERAMODE_DOWNVIEW;
		//}
		//else if (CameraMode == CAMERAMODE_DOWNVIEW)
		//{
		//	CameraMode = CAMERAMODE_SIDEVIEW;
		//}
		//else if (CameraMode == CAMERAMODE_SIDEVIEW)
		//{
		//	CameraMode = CAMERAMODE_CONTROL;
		//}
	}

#endif // _DEBUG

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:

		if (CameraMode == CAMERAMODE_SEAMLESS)
		{
			SeamlessModeChangeCamera();
		}
		else
		{
			// �^�C�g���J�����̈ʒu
			TitleCamera();
		}
		
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		switch (CameraMode)
		{
		case CAMERAMODE_CONTROL:	//����J�����̍X�V
			ControlCamera();
			break;
		case CAMERAMODE_FOLLOW:		//�Ǐ]�J�����̍X�V
			FollowCamera();
			break;
		case CAMERAMODE_FIXEDNOW:		//�Œ�J�����̍X�V
			FixedNow();
			break;
		case CAMERAMODE_BETWEEN:		//�Q�_�ԃJ�����̍X�V
			BetWeen();
			break;
		case CAMERAMODE_DOWNVIEW:	//�����낵�J�����̍X�V
			DownviewCamera();
			break;
		case CAMERAMODE_SIDEVIEW:	//2D�J�����̍X�V
			SideviewCamera();
			break;
		case CAMERAMODE_EVENTBOSS:	//�C�x���g���̃{�X���ڃJ�����̍X�V
			EventBossCamera();
			break;
		case CAMERAMODE_FPS:		//��l��
			FPSCamera();
			break;
		case CAMERAMODE_FPSCOMPLEMENT:		//��l�̕ύX���̕⊮�p�J����
			FPSComplementCamera();
			break;
		case CAMERAMODE_ULTIMATE:	//�p�������̃v���C���[�����낵���_
			UltimateCamera();
			break;
		case CAMERAMODE_SEAMLESS:
			SeamlessModeChangeCamera();
			break;
		}

		break;
	case CScene::MODE_RESULT:

		//���_�̏����o�͂���
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;

		break;
	}
	//�f�o�b�O�\���̎擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("[���_]%f:%f:%f\n", m_posV.x, m_posV.y, m_posV.z);
	pDebugProc->Print("[�����_]%f:%f:%f\n", m_posR.x, m_posR.y, m_posR.z);
}

//====================================================================
//����J�����̍X�V����
//====================================================================
void CCamera::ControlCamera(void)
{
	//�f�o�C�X�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (m_rot.x <= D3DX_PI * 0.5f && m_rot.x >= -(D3DX_PI * 0.5f))
	{//����
		m_rotOld.x = m_rot.x;

		//�L�[�{�[�h
		if (pInputKeyboard->GetPress(DIK_I) == true)
		{
			m_rot.x += CAMERA_VR_SPEED;
		}
		if (pInputKeyboard->GetPress(DIK_K) == true)
		{
			m_rot.x -= CAMERA_VR_SPEED;
		}
	}

	if (fabsf(m_rot.x) > fabsf(D3DX_PI * 0.5f))
	{//����ɒB�������P�t���[���O��rot�ɂ��ǂ�
		m_rot.x = m_rotOld.x;
	}

	//�L�[�{�[�h
	if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		m_rot.y -= CAMERA_VR_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{
		m_rot.y += CAMERA_VR_SPEED;
	}

	//����������̌����̕␳
	if (m_rot.y > D3DX_PI * 1.0f)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI * 1.0f)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}


	//�L�[�{�[�h
	//if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
	//{
	//	m_posV.y += CAMERA_SPEED;
	//	m_posR.y += CAMERA_SPEED;

	//}
	//if (pInputKeyboard->GetPress(DIK_LCONTROL) == true)
	//{
	//	m_posV.y -= CAMERA_SPEED;
	//	m_posR.y -= CAMERA_SPEED;
	//}
	if (pInputKeyboard->GetPress(DIK_RSHIFT) == true)
	{
		m_posV.y += CAMERA_SPEED;
		m_posR.y += CAMERA_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_RCONTROL) == true)
	{
		m_posV.y -= CAMERA_SPEED;
		m_posR.y -= CAMERA_SPEED;
	}

	//if (pInputKeyboard->GetTrigger(DIK_4) == true)
	//{
	//	m_CameraDistance -= 500.0f;
	//}

	//if (pInputKeyboard->GetTrigger(DIK_5) == true)
	//{
	//	m_CameraDistance += 500.0f;
	//}

	//�L�[�{�[�h
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{
		m_posR.z += CAMERA_SPEED * cosf(m_rot.y);
		m_posR.x += CAMERA_SPEED * sinf(m_rot.y);

		m_posV.z += CAMERA_SPEED * cosf(m_rot.y);
		m_posV.x += CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_G) == true)
	{
		m_posR.z += -CAMERA_SPEED * cosf(m_rot.y);
		m_posR.x += -CAMERA_SPEED * sinf(m_rot.y);

		m_posV.z += -CAMERA_SPEED * cosf(m_rot.y);
		m_posV.x += -CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_F) == true)
	{
		m_posR.x += -CAMERA_SPEED * cosf(m_rot.y);
		m_posR.z -= -CAMERA_SPEED * sinf(m_rot.y);

		m_posV.x += -CAMERA_SPEED * cosf(m_rot.y);
		m_posV.z -= -CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_H) == true)
	{
		m_posR.x += CAMERA_SPEED * cosf(m_rot.y);
		m_posR.z -= CAMERA_SPEED * sinf(m_rot.y);

		m_posV.x += CAMERA_SPEED * cosf(m_rot.y);
		m_posV.z -= CAMERA_SPEED * sinf(m_rot.y);
	}

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME ||
		CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
	{
		//���_�̏����o�͂���
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	}
}

//====================================================================
//�Ǐ]�J�����̍X�V����
//====================================================================
void CCamera::FollowCamera(void)
{
	//�f�o�C�X�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�v���C���[�̎擾
	CPlayer* pPlayer = CGame::GetPlayer();
	//�{�X�̎擾
	CBoss* pBoss = CGame::GetBoss();

	m_fRotMove = m_fRotMove * 0.5f;

	if (pPlayer->GetJump() == false)
	{
		if (m_rot.x <= D3DX_PI * 0.5f && m_rot.x >= -(D3DX_PI * 0.5f))
		{//����
			m_rotOld.x = m_rot.x;

			////�L�[�{�[�h
			//if (pInputKeyboard->GetPress(DIK_I) == true)
			//{
			//	m_fRotMove.x += CAMERA_VR_SPEED;
			//}
			//if (pInputKeyboard->GetPress(DIK_K) == true)
			//{
			//	m_fRotMove.x -= CAMERA_VR_SPEED;
			//}

			////�E�X�e�B�b�N�̏㉺���_�ړ�����
			//m_fRotMove.x += pInputJoypad->Get_Stick_Right(0).y * CAMERA_PAD_VR_SPEED;

			//m_fRotMove.x -= pInputMouse->GetMouseMove().y * CAMERA_VR_SPEED;

			m_rot.x += m_fRotMove.x;
		}
	}

	if (m_rot.x > D3DX_PI * 0.05f || m_rot.x < -(D3DX_PI * 0.45f))
	{//����ɒB�������P�t���[���O��rot�ɂ��ǂ�
		m_rot.x = m_rotOld.x;
	}

	////�L�[�{�[�h
	//if (pInputKeyboard->GetPress(DIK_J) == true)
	//{
	//	m_fRotMove.y -= CAMERA_VR_SPEED;

	//}
	//if (pInputKeyboard->GetPress(DIK_L) == true)
	//{
	//	m_fRotMove.y += CAMERA_VR_SPEED;
	//}

	////�E�X�e�B�b�N�̍��E���_�ړ�����
	//m_fRotMove.y += pInputJoypad->Get_Stick_Right(0).x * CAMERA_PAD_VR_SPEED;

	//m_fRotMove.y += pInputMouse->GetMouseMove().x * CAMERA_VR_SPEED;

	m_rot.y += m_fRotMove.y;

	//����������̌����̕␳
	if (m_rot.y > D3DX_PI * 1.0f)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI * 1.0f)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posRDest.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;

	m_posVDest.x = pPlayer->GetCameraPos().x + sinf(-pPlayer->GetRot().y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posVDest.z = pPlayer->GetCameraPos().z + cosf(-pPlayer->GetRot().y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posVDest.y = pPlayer->GetCameraPos().y + 50.0f + sinf(-m_rot.x) * CAMERA_DISTANCE;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.4f;

		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;
		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);

		if (pPlayer->GetJump() == false)
		{
			m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed;
			m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed;
		}
	}
	else
	{
		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed * 0.05f;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed * 0.05f;
		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 1.0f;
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 1.0f;

		if (pPlayer->GetJump() == false)
		{
			m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed;
			m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed;
		}
	}


	//���ڂ̐؂�ւ�
	if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::BUTTON_L, 0) == true)
	{
		//�Q�[����SE���Đ�����
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ATTENTION);

		m_bAttention = m_bAttention ? false : true;
	}

	if (m_bAttention == true)
	{
		m_FollowTime = 180;
	}

	////���f�����~�܂������ɐ��ʂ���������
	//float fRotMove, fRotDest, fRotDiff;

	//if (m_FollowTime > 0)
	//{
	//	m_FollowTime--;

	//	fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));	//���݂̌���
	//	fRotDest = atan2f(pBoss->GetPos().x - pPlayer->GetPos().x, pBoss->GetPos().z - pPlayer->GetPos().z);	//�ړI�̌���
	//	//fRotDest = atan2f(sinf(pPlayer->GetRot().y + D3DX_PI), cosf(pPlayer->GetRot().y + D3DX_PI));	//�ړI�̌���
	//	fRotDiff = fRotDest - fRotMove;									 //����

	//	if (fRotDiff > D3DX_PI * 1.0f)
	//	{
	//		fRotDiff -= D3DX_PI * 2.0f;
	//	}
	//	else if (fRotDiff < -D3DX_PI * 1.0f)
	//	{
	//		fRotDiff += D3DX_PI * 2.0f;
	//	}

	//	fRotMove += fRotDiff * 0.075f;

	//	if (fRotMove > D3DX_PI * 1.0f)
	//	{
	//		fRotMove -= D3DX_PI * 2.0f;
	//	}
	//	else if (fRotMove < -D3DX_PI * 1.0f)
	//	{
	//		fRotMove += D3DX_PI * 2.0f;
	//	}
	//	m_rot.y = fRotMove;
	//}
}

//====================================================================
//���݂̈ʒu�ŌŒ�
//====================================================================
void CCamera::FixedNow(void)
{

}

//====================================================================
//�Q�_�ԃJ����(Boss�EPlayer)
//====================================================================
void CCamera::BetWeen(void)
{
	//�v���C���[�̎擾
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//�{�X�̎擾
	D3DXVECTOR3 BossPos = INITVECTOR3;

	D3DXVECTOR3 BetWeenPos = PlayerPos + ((BossPos - PlayerPos) * 0.5f);

	//����������̌����̕␳
	if (m_rot.y > D3DX_PI * 1.0f)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI * 1.0f)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * m_CameraDistance;
	m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * m_CameraDistance;
	m_posRDest.y = m_posV.y + sinf(m_rot.x) * m_CameraDistance;

	m_posVDest.x = BetWeenPos.x + sinf(m_rot.y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	m_posVDest.z = BetWeenPos.z + cosf(m_rot.y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	m_posVDest.y = BetWeenPos.y + 50.0f + sinf(-m_rot.x) * m_CameraDistance;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.4f;

		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;
		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
	}
	else
	{
		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed * 1.0f;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed * 1.0f;
		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 1.0f;
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 1.0f;
	}

	if (m_bAttention == true)
	{
		m_FollowTime = 180;
	}
}

//====================================================================
//�����낵�J�����̍X�V����
//====================================================================
void CCamera::DownviewCamera(void)
{
	m_rot.y = 0.0f;
	m_posV.x += (DOWNVIEW_POSV.x - m_posV.x) * m_fHomingSpeed;
	m_posV.y += (DOWNVIEW_POSV.y - m_posV.y) * m_fHomingSpeed;
	m_posV.z += (DOWNVIEW_POSV.z - m_posV.z) * m_fHomingSpeed;
	m_posR.x += (DOWNVIEW_POSR.x - m_posR.x) * m_fHomingSpeed;
	m_posR.y += (DOWNVIEW_POSR.y - m_posR.y) * m_fHomingSpeed;
	m_posR.z += (DOWNVIEW_POSR.z - m_posR.z) * m_fHomingSpeed;
}

//====================================================================
//2D�J�����̍X�V����
//====================================================================
void CCamera::SideviewCamera(void)
{
	m_rot.y = 0.0f;
	m_posV.x += (SIDEVIEW_POSV.x - m_posV.x) * m_fHomingSpeed;
	m_posV.y += (SIDEVIEW_POSV.y - m_posV.y) * m_fHomingSpeed;
	m_posV.z += (SIDEVIEW_POSV.z - m_posV.z) * m_fHomingSpeed;
	m_posR.x += (SIDEVIEW_POSR.x - m_posR.x) * m_fHomingSpeed;
	m_posR.y += (SIDEVIEW_POSR.y - m_posR.y) * m_fHomingSpeed;
	m_posR.z += (SIDEVIEW_POSR.z - m_posR.z) * m_fHomingSpeed;
}

//====================================================================
//�C�x���g���̃{�X���ڃJ�����̍X�V����
//====================================================================
void CCamera::EventBossCamera(void)
{
	//�{�X�̎擾
	CBoss* pBoss = CGame::GetBoss();

	m_posVDest.x = CGame::GetEventPos().x + sinf(m_rot.y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posVDest.z = CGame::GetEventPos().z + cosf(m_rot.y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posVDest.y = CGame::GetEventPos().y + sinf(-m_rot.x) * CAMERA_DISTANCE_EVENT + 50.0f;

	m_posRDest.x = m_posVDest.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posRDest.z = m_posVDest.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posRDest.y = m_posVDest.y + sinf(m_rot.x) * CAMERA_DISTANCE_EVENT + 10.0f;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.8f;

		//m_posR.y += (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);

		////���_�̏����o�͂���
		//m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		//m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		//m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;

		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed * 5.0f;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;

		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
	}
	else
	{
		m_posR.x += (m_posRDest.x - m_posR.x) * m_fHomingSpeed;
		m_posR.y += (m_posRDest.y - m_posR.y) * m_fHomingSpeed;
		m_posR.z += (m_posRDest.z - m_posR.z) * m_fHomingSpeed;

		m_posV.x += (m_posVDest.x - m_posV.x) * m_fHomingSpeed;
		m_posV.y += (m_posVDest.y - m_posV.y) * m_fHomingSpeed;
		m_posV.z += (m_posVDest.z - m_posV.z) * m_fHomingSpeed;
	}
}

//====================================================================
//��l�̃J�����̍X�V����
//====================================================================
void CCamera::FPSCamera(void)
{
	CPlayer* pPlayer = nullptr;

	//�v���C���[�̎擾
	switch (CScene::GetMode())
	{
	case CScene::MODE_GAME:

		pPlayer = CGame::GetPlayer();

		break;

	case CScene::MODE_TUTORIAL:

		pPlayer = CTutorial::GetPlayer();

		break;
	}

	m_posV.x = pPlayer->GetPos().x;
	m_posV.y = pPlayer->GetPos().y + 50.0f;
	m_posV.z = pPlayer->GetPos().z;
	m_rot.x = pPlayer->GetCameraHeightRot();
	m_rot.y = pPlayer->GetRot().y + D3DX_PI;

	//�����_�̏����o�͂���
	m_posR.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posR.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;
}

//====================================================================
//��l�̕ύX���̕⊮�p�J�����̍X�V����
//====================================================================
void CCamera::FPSComplementCamera(void)
{
	CPlayer* pPlayer = nullptr;

	//�v���C���[�̎擾
	switch (CScene::GetMode())
	{
	case CScene::MODE_GAME:

		pPlayer = CGame::GetPlayer();

		break;

	case CScene::MODE_TUTORIAL:

		pPlayer = CTutorial::GetPlayer();

		break;
	}


	m_posVDest.x = pPlayer->GetPos().x;
	m_posVDest.y = pPlayer->GetPos().y + 50.0f;
	m_posVDest.z = pPlayer->GetPos().z;
	m_rot.x = pPlayer->GetCameraHeightRot();
	m_rot.y = pPlayer->GetRot().y + D3DX_PI;

	//�����_�̏����o�͂���
	m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posRDest.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;

	m_posR.x += (m_posRDest.x - m_posR.x) * 0.6f;
	m_posR.y += (m_posRDest.y - m_posR.y) * 0.6f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.6f;

	m_posV.x += (m_posVDest.x - m_posV.x) * 0.6f;
	m_posV.y += (m_posVDest.y - m_posV.y) * 0.6f;
	m_posV.z += (m_posVDest.z - m_posV.z) * 0.6f;

	if (m_nCameraCount > 0)
	{
		m_nCameraCount--;
	}
	else
	{
		CameraMode = CAMERAMODE_FPS;
	}
}

//====================================================================
//�p�������̃J����
//====================================================================
void CCamera::UltimateCamera(void)
{
	//�v���C���[�̎擾
	CPlayer* pPlayer = nullptr;

	//�v���C���[�̎擾
	switch (CScene::GetMode())
	{
	case CScene::MODE_GAME:

		pPlayer = CGame::GetPlayer();

		break;

	case CScene::MODE_TUTORIAL:

		pPlayer = CTutorial::GetPlayer();

		break;
	}

	if (pPlayer == nullptr)
	{
		return;
	}

	// �ϐ��錾
	float fRotDest = 0.0f;	// �����ۑ��p
	float fRotDiff = 0.0f;	// �����ۑ��p

	// �������v�Z
	fRotDest = (pPlayer->GetRot().y + D3DX_PI * ULTIMATE_ROT);
	// �ڕW�̊p�x�܂ł̍��������߂�
	fRotDiff = fRotDest - m_rot.y;

	/*if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= (D3DX_PI * 2);
	}
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += (D3DX_PI * 2);
	}*/

	// ���_�̐ݒ�
	m_posV.x = m_posR.x + sinf(fRotDiff) * cosf(m_rot.x) * ULTIMATE_DISTANCE;
	m_posV.z = m_posR.z + cosf(fRotDiff) * cosf(m_rot.x) * ULTIMATE_DISTANCE;
	m_posV.y = pPlayer->GetPos().y + ULTIMATE_DISTANCEu;

	// �����_�̐ݒ�
	m_posR = pPlayer->GetPos();
}

//====================================================================
//�^�C�g���J����
//====================================================================
void CCamera::TitleCamera(void)
{
	// ���_
	m_posV = TITLE_POS_V;
	// �����_
	m_posR = TITLE_POS_R;
}

//====================================================================
//���[�h�؂�ւ����̃V�[�����X(�q���ڂ̂Ȃ�)�ȃJ����
//====================================================================
void CCamera::SeamlessModeChangeCamera(void)
{
	//CPlayer* pPlayer = nullptr;

	////�v���C���[�̎擾
	//switch (CScene::GetMode())
	//{
	//case CScene::MODE_TITLE:

	//	pPlayer = CTitle::GetPlayer();

	//	break;

	//case CScene::MODE_GAME:

	//	pPlayer = CGame::GetPlayer();

	//	break;

	//case CScene::MODE_TUTORIAL:

	//	pPlayer = CTutorial::GetPlayer();

	//	break;
	//}

	//if (!pPlayer)
	//{
	//	return;
	//}

	//D3DXVECTOR3 posDiffv = {};	// ���_�̍���
	//D3DXVECTOR3 posDiffr = {};	// �����_�̍���
	//D3DXVECTOR3 rotDiff = {};	// �����̍���

	//D3DXVECTOR3 posPlayer = {	// �v���C���[�ʒu
	//	pPlayer->GetPos().x, pPlayer->GetPos().y + 50.0f, pPlayer->GetPos().z
	//};

	//// �����̐ݒ�
	//m_rot.x = pPlayer->GetCameraHeightRot();
	//m_rot.y = pPlayer->GetRot().y + D3DX_PI;

	//D3DXVECTOR3 posPlayerR = {	// �v���C���[�����_
	//	posPlayer.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE,
	//	posPlayer.y + sinf(m_rot.x) * CAMERA_DISTANCE,
	//	posPlayer.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE,
	//};

	//// �v���C���[(�ڕW)�Ǝ��_�̍������߂�
	//posDiffv.x = posPlayer.x - m_posV.x;
	//posDiffv.y = posPlayer.y - m_posV.y;
	//posDiffv.z = posPlayer.z - m_posV.z;

	//// �v���C���[(�ڕW)�ƒ����_�̍������߂�
	//posDiffr.x = posPlayerR.x - m_posR.x;
	//posDiffr.y = posPlayerR.y - m_posR.y;
	//posDiffr.z = posPlayerR.z - m_posR.z;

	//// ���_�̍X�V
	//m_posV.x += posDiffv.x * TTTLE_DECREASE;
	//m_posV.y += posDiffv.y * TTTLE_DECREASE;
	//m_posV.z += posDiffv.z * TTTLE_DECREASE;

	//// ���_�̍X�V
	//m_posR.x += posDiffr.x * TTTLE_DECREASE;
	//m_posR.y += posDiffr.y * TTTLE_DECREASE;
	//m_posR.z += posDiffr.z * TTTLE_DECREASE;

	//// �������v�Z
	//float fLength = sqrtf((posPlayer.x - m_posV.x) * (posPlayer.x - m_posV.x) + (posPlayer.y - m_posV.y) * (posPlayer.y - m_posV.y));

	//if (fLength <= TTTLE_LENGTH)
	//{// ���l�ȉ��Ȃ烂�[�h���`���[�g���A����
	//	CScene::SetMode(CScene::MODE_TUTORIAL);
	//}
}

//====================================================================
//�J�����̐ݒ菈��
//====================================================================
void CCamera::SetCamera(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�r���[�|�[�g�̐ݒ�
	m_pDevice->SetViewport(&m_viewport);

	CManager::GetInstance()->SetGameMtxView(m_mtxView);
	CManager::GetInstance()->SetGamemtxProjection(m_mtxProjection);
	CManager::GetInstance()->SetGameViewport(m_viewport);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)m_viewport.Width / (float)m_viewport.Height,
		10.0f,
		12000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//====================================================================
//�J�����̐ݒ菈��
//====================================================================
void CCamera::ResetCamera(void)
{
	m_posV = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CameraDistance = CAMERA_DISTANCE;
	m_EventPosY = 0.0f;

	if (CScene::MODE_GAME == CScene::GetMode())
	{
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}