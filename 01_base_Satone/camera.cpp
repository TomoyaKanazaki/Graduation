//==============================================================
//
//�J��������[camera.h]
//Author:����������
//
//==============================================================
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"

//�}�N����`
#define MAX_CAMERA			(1)			//�J�����̍ő吔
#define CAMERA_LEN			(10.0f)		//����
#define CAMERA_PLEN			(0.0f)		//�J�����̎��_�ƒ����_�̋���
#define CURVE_RL			(0.5f)		//���E�̊p�x
#define CURVE_UP			(0.0f)		//��̊p�x
#define CURVE_DOWN			(1.0f)		//���̊p�x
#define LENGRH				(1300.0f)	//���_�ƒ����_�̒���
#define FOLLOW				(0.5f)		//�J�����̈ړ���

//==============================================================
//�R���X�g���N�^
//==============================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 189.0f, -500.0f);	//���_�̏�����
	m_posR = D3DXVECTOR3(0.0f, 168.0f, -500.0f);	//�����_�̏�����
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//������x�N�g���̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����̏�����
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړI�̎��_�̏�����
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړI�̒����_�̏�����
	m_moveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���_�̈ړ��ʂ̏�����
	m_moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����_�̈ړ��ʂ̏�����
	m_fRotMove = 0.0f;								//���݂̕���(�p�x)
	m_fRotDest = 0.0f;								//�ڕW�̕���(�p�x)
	m_fRotDiff = 0.0f;								//�ڕW�̕����܂ł̍���
	//m_fLength = LENGRH;								//����
	m_bProject = true;			//�������e�̏�Ԃɂ���
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CCamera::~CCamera()
{

}

//==============================================================
//�J�����̏���������
//==============================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -500.0f);	//���_�̏�����
	m_posR = D3DXVECTOR3(0.0f, 0.0f, -500.0f);	//�����_�̏�����
	
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//������x�N�g���̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����̏�����
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړI�̎��_�̏�����
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړI�̒����_�̏�����
	m_moveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���_�̈ړ��ʂ̏�����
	m_moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����_�̈ړ��ʂ̏�����
	m_fRotMove = 0.0f;								//���݂̕���(�p�x)
	m_fRotDest = 0.0f;								//�ڕW�̕���(�p�x)
	m_fRotDiff = 0.0f;								//�ڕW�̕����܂ł̍���
	//m_fLength = LENGRH;								//����
	m_bProject = true;			//�������e�̏�Ԃɂ���

	//�����n�_�̏�����
	m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * LENGRH;
	m_posV.y = (m_posR.y + CAMERA_PLEN) + sinf(m_rot.x) * LENGRH;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * LENGRH;

	m_posR.x = m_posV.x + sinf(m_rot.y) * -cosf(m_rot.x) * -LENGRH;
	m_posR.y = (m_posV.y - CAMERA_PLEN) + sinf(m_rot.x) * -LENGRH;
	m_posR.z = m_posV.z + cosf(m_rot.y) * -cosf(m_rot.x) * -LENGRH;

	return S_OK;
}

//==============================================================
//�J�����̏I������
//==============================================================
void CCamera::Uninit(void)
{
	
}

//==============================================================
//�J�����̍X�V����
//==============================================================
void CCamera::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();		//�f�o�b�O�\���̏��擾

	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME)
	{//�Q�[����ʂ̂Ƃ�

		//�J�����̑���
		CCamera::Control();

	}
	else
	{//����ȊO(���U���g)�̂Ƃ�

		m_posV = D3DXVECTOR3(-4680, 1271.0f, 300.0f);	//���_�̏�����
		m_posR = D3DXVECTOR3(0.0f, 1250.0f, 300.0f);	//�����_�̏�����
	}

	//�ړ�����
	m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * LENGRH;
	m_posV.y = (m_posR.y + CAMERA_PLEN) + sinf(m_rot.x) * LENGRH;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * LENGRH;

	m_posR.x = m_posV.x + sinf(m_rot.y) * -cosf(m_rot.x) * -LENGRH;
	m_posR.y = (m_posV.y - CAMERA_PLEN) + sinf(m_rot.x) * -LENGRH;
	m_posR.z = m_posV.z + cosf(m_rot.y) * -cosf(m_rot.x) * -LENGRH;

	//�J�����̉�荞��
	/*if (pPlayer->move.x <= 0.0001f && pPlayer->move.x >= -0.0001f && pPlayer->move.z <= 0.0001f && pPlayer->move.z >= -0.0001f)
	{
		m_fRotDest = pPlayer->rot.y + D3DX_PI;
	}*/

	//�����̕␳
	CCamera::RotCorrection();

	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME)
	{//�Q�[����ʂ̂Ƃ�

		//�����Ǐ]
		/*m_posR.x += (m_posRDest.x - m_posR.x) * FOLLOW;
		m_posV.x += (m_posVDest.x - m_posV.x) * FOLLOW;
		m_posR.y += (m_posRDest.y - m_posR.y) * FOLLOW;
		m_posV.y += (m_posVDest.y - m_posV.y) * FOLLOW;
		m_posR.z += (m_posRDest.z - m_posR.z) * FOLLOW;
		m_posV.z += (m_posVDest.z - m_posV.z) * FOLLOW;*/
	}

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	pDebugProc->Print("[���_]  (%f,%f,%f)\n", m_posV.x, m_posV.y, m_posV.z);
	pDebugProc->Print("[�����_](%f,%f,%f)\n", m_posR.x, m_posR.y, m_posR.z);
}

//==============================================================
//�����̕␳����
//==============================================================
void CCamera::RotCorrection(void)
{
	//�������v�Z
	m_fRotDiff = m_fRotDest - m_fRotMove;

	//�ڕW�̕����܂ł̍������C��
	if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * 2;
	}
	else if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff -= D3DX_PI * 2;
	}

	//���݂̈ړ������ɍ����𑫂�
	m_fRotMove += m_fRotDiff * 0.1f;

	//���݂̕����C��
	if (m_fRotMove > D3DX_PI)
	{
		m_fRotMove -= D3DX_PI * 2;
	}
	else if (m_fRotMove < -D3DX_PI)
	{
		m_fRotMove += D3DX_PI * 2;
	}
}

//==============================================================
//�J�����̑���
//==============================================================
void CCamera::Control()
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CPlayer *pPlayer = CGame::GetPlayer();			//�v���C���[�̏��擾

#if 0

	//�J����
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{//�J������
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//�J��������
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * 0.25f);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * 0.25f);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * 0.25f);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//�J��������
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * 0.75f);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * 0.75f);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * 0.75f);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * 0.75f);
		}
		else
		{
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * CURVE_RL);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * CURVE_RL);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * CURVE_RL);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * CURVE_RL);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{//�J�����E
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//�J��������
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * 0.25f);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * 0.25f);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * 0.25f);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//�J��������
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * 0.75f);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * 0.75f);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * 0.75f);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * 0.75f);
		}
		else
		{
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_RL);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_RL);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_RL);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_RL);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{//�J������
		m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_UP);
		m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_UP);
		m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_UP);
		m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_UP);
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{//�J������
		m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_DOWN);
		m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_DOWN);
		m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_DOWN);
		m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_DOWN);
	}
#endif

	////�ړI�̒����_��ݒ�
	//m_posRDest.x = pPlayer->GetPosition().x + sinf(pPlayer->GetRotation().y + D3DX_PI) * 0.0f;
	//m_posRDest.y = pPlayer->GetPosition().y;
	//m_posRDest.z = pPlayer->GetPosition().z + cosf(pPlayer->GetRotation().y + D3DX_PI) * 0.0f;

	////�ړI�̎��_��ݒ�
	//m_posVDest.x = pPlayer->GetPosition().x + sinf(GetRotation().y + D3DX_PI) * LENGRH;
	//m_posVDest.y = pPlayer->GetPosition().y;
	//m_posVDest.z = pPlayer->GetPosition().z + cosf(GetRotation().y + D3DX_PI) * LENGRH;

#if _DEBUG
	//���_
	if (pInputKeyboard->GetPress(DIK_C) == true)
	{//���_�E
		m_rot.y -= 0.05f;
	}
	if (pInputKeyboard->GetPress(DIK_Z) == true)
	{//���_��
		m_rot.y += 0.05f;
	}
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{//���_��
		m_rot.x += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_N) == true)
	{//���_��
		m_rot.x -= 0.01f;
	}

	//�����_
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{//�����_�E
		m_rot.y -= 0.05f;
	}
	if (pInputKeyboard->GetPress(DIK_E) == true)
	{//�����_��
		m_rot.y += 0.05f;
	}
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{//���_��
		m_rot.x -= 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_B) == true)
	{//���_��
		m_rot.x += 0.01f;
	}
#endif
	
#if _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	{//F3�L�[�������ꂽ��

		//���e�؂�ւ�
		m_bProject = m_bProject ? false : true;
	}

#endif
}

//==============================================================
//�J�����̐ݒ菈��
//==============================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//if (m_bProject == false)
	{
		//�v���W�F�N�V�����}�g���b�N�X���쐬(�������e)
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f, 7000.0f);
	}
	//else if (m_bProject == true)
	//{
	//	//�v���W�F�N�V�����}�g���b�N�X���쐬(���s���e)
	//	D3DXMatrixOrthoLH(&m_mtxProjection,		//�v���W�F�N�V�����}�g���b�N�X
	//		(float)SCREEN_WIDTH * 1.2f/* * 0.5f*/,			//��ʂ̕�
	//		(float)SCREEN_HEIGHT * 1.2f/* * 0.5f*/,		//��ʂ̍���
	//		-1000.0f,					//Z�l�̍ŏ��l
	//		7000.0f);					//Z�l�̍ő�l
	//}

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

////==============================================================
////�����J�����̐ݒ菈��
////==============================================================
////void SetCamera(int nIdex)
////{
////	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
////
////	//�r���[�|�[�g�̐ݒ�
////	pDevice->SetViewport(&g_camera[nIdex].viewport);
////
////	//�v���W�F�N�V�����}�g���b�N�X�̏�����
////	D3DXMatrixIdentity(&g_camera[nIdex].mtxProjection);
////
////	//�v���W�F�N�V�����}�g���b�N�X���쐬
////	D3DXMatrixPerspectiveFovLH(&g_camera[nIdex].mtxProjection,
////		D3DXToRadian(45.0f),
////		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
////		10.0f, 5000.0f);
////
////	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
////	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nIdex].mtxProjection);
////
////	//�r���[�}�g���b�N�X�̏�����
////	D3DXMatrixIdentity(&g_camera[nIdex].mtxView);
////
////	//�r���[�}�g���b�N�X�̍쐬
////	D3DXMatrixLookAtLH(&g_camera[nIdex].mtxView, &g_camera[nIdex].posV, &g_camera[nIdex].posR, &g_camera[nIdex].vecU);
////
////	//�r���[�}�g���b�N�X�̐ݒ�
////	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nIdex].mtxView);
////}
//
////==============================================================
////�J�����̎擾
////==============================================================
//Camera *GetCamera(void)
//{
//	return &g_camera;		//�J�����̏��̐擪�A�h���X��n��
//}