//============================================
//
//	���o�p�̃��[�r�[ [EventMovie.cpp]
//	Author:sakamoto kai
//
//============================================
#include "EventMovie.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"
#include "player.h"
#include "game.h"
#include "Devil.h"
#include "camera.h"

//�ÓI�����o�ϐ��錾

//====================================================================
//�R���X�g���N�^
//====================================================================
CEventMovie::CEventMovie()
{
	m_State = STATE_START;
	m_nWave = 0;
	m_nCount = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEventMovie::~CEventMovie()
{

}

//====================================================================
//��������
//====================================================================
CEventMovie* CEventMovie::Create()
{
	CEventMovie* pPause = nullptr;

	if (pPause == nullptr)
	{
		//�G�̐���
		pPause = new CEventMovie();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pPause->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pPause;
}

//====================================================================
//����������
//====================================================================
HRESULT CEventMovie::Init(void)
{

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CEventMovie::Uninit(void)
{

}

//====================================================================
//�X�V����
//====================================================================
void CEventMovie::Update(void)
{
	switch (m_State)
	{
	case CEventMovie::STATE_START:
		//�J�n�����o
		StartMovie();
		break;

	case CEventMovie::STATE_END:
		break;
	default:
		assert(false);
		break;
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CEventMovie::Draw(void)
{

}

//====================================================================
//�J�n�����o
//====================================================================
void CEventMovie::StartMovie(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	D3DXVECTOR3 DevilPos;

	if (CGame::GetInstance()->GetDevil())
	{
		DevilPos = CGame::GetInstance()->GetDevil()->GetPos();
	}

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	float EventFinish = 0.0f;

	switch (m_nWave)
	{
	case 0:		//�����猩�Ă鎋�_�ɂ���
		pCamera->SetCameraMode(CCamera::CAMERAMODE_SIDEVIEW);
		pCamera->SetHomingSpeed(0.2f);

		m_nWave++;
		break;

	case 1:		//�v���C���[�����̋����܂ŕ���

		//if (m_pPlayer->GetPos().x <= 450.0f)
		//{//���̋����ɓ��B�����ꍇ��3�l�̎��_�ɕύX����

		//	pCamera->SetCameraMode(CCamera::CAMERAMODE_FOLLOW);
		//	pCamera->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		//	pCamera->SetHomingSpeed(0.02f);
		//	m_nWave++;

		//}
		break;

	default:

		//pCamera->SetCameraMode(CCamera::CAMERAMODE_FPS);
		//SetEvent(false);
		//m_pPlayer->SetModelDisp(false);
		//m_pBoss->EventJump(60);
		//m_nWave = 0;
		//m_nEventNumber++;
		break;
	}
}