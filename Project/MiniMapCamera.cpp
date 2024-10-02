//============================================
//
//	�~�j�}�b�v�J�����̏��� [MiniMapCamera.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MiniMapCamera.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
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

#define ULTIMATE_DISTANCE (300.0f)							//�p�������̎��_�ƒ����_����
#define ULTIMATE_DISTANCEu (300.0f)							//�p�������̍���
#define ULTIMATE_ROT (0.85f)								//�p�������̌���

//====================================================================
//�R���X�g���N�^
//====================================================================
CMiniMapCamera::CMiniMapCamera()
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
	CameraMode = CAMERAMODE_DOWNVIEW;
	m_fHomingSpeed = CAMERA_HOMING;

	m_viewport.X = 10;			// �`�悷���ʂ̍���X���W
	m_viewport.Y = 510;				// �`�悷���ʂ̍���Y���W
	m_viewport.Width = 200;			// ��ʂ̕�
	m_viewport.Height = 200;		// ��ʂ̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 0.0f;

	m_FollowTime = 0;
	m_bFollowY = false;
	m_bAttention = false;
	m_fRotMove = INITVECTOR3;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMiniMapCamera::~CMiniMapCamera()
{

}

//====================================================================
//�J�����̏���������
//====================================================================
HRESULT CMiniMapCamera::Init(void)
{
	return S_OK;
}

//====================================================================
//�J�����̏I������
//====================================================================
void CMiniMapCamera::Uninit(void)
{

}

//====================================================================
//�J�����̍X�V����
//====================================================================
void CMiniMapCamera::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:

		ControlCamera();

		//���_�̏����o�͂���
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;

		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		switch (CameraMode)
		{
		case CAMERAMODE_CONTROL:	//����J�����̍X�V
			ControlCamera();
			break;
		case CAMERAMODE_DOWNVIEW:	//�����낵�J�����̍X�V
			DownviewCamera();
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
	////�f�o�b�O�\���̎擾
	//CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	//pDebugProc->Print("%f:%f:%f\n", m_posV.x, m_posV.y, m_posV.z);
	//pDebugProc->Print("%f:%f:%f\n", m_posR.x, m_posR.y, m_posR.z);
}

//====================================================================
//����J�����̍X�V����
//====================================================================
void CMiniMapCamera::ControlCamera(void)
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

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		//���_�̏����o�͂���
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	}
}

//====================================================================
//�����낵�J�����̍X�V����
//====================================================================
void CMiniMapCamera::DownviewCamera(void)
{
	CPlayer* pPlayer = nullptr;

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
		assert(("�v���C���[�J�����̎擾�Ɏ��s", false));
	}

	D3DXVECTOR3 Pos = pPlayer->GetPos();
	D3DXVECTOR3 Rot = pPlayer->GetRot();

	m_rot.y = 0.0f;
	m_posV.x = Pos.x + sinf(Rot.y) * 10.0f;
	m_posV.y = Pos.y + 1000.0f;
	m_posV.z = Pos.z + cosf(Rot.y) * 10.0f;
	m_posR.x = Pos.x;
	m_posR.y = Pos.y;
	m_posR.z = Pos.z;
}

//====================================================================
//�J�����̐ݒ菈��
//====================================================================
void CMiniMapCamera::SetCamera(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�r���[�|�[�g�̐ݒ�
	m_pDevice->SetViewport(&m_viewport);

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
void CMiniMapCamera::ResetCamera(void)
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