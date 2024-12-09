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
#include "model.h"
#include "motion.h"
#include "effect.h"
#include "timer.h"

//�ÓI�����o�ϐ��錾

//====================================================================
//�R���X�g���N�^
//====================================================================
CEventMovie::CEventMovie()
{
	m_State = STATE_CHANGE;
	m_nWave = 0;
	m_nCount = 0;
	m_fSinFloat = 0.0f;
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
	// ���g���폜����
	delete this;
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

	case CEventMovie::STATE_CHANGE:
		//�J�n�����o
		StageChangeMovie();
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

	CDevil* pDevil = CDevil::GetListTop();

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	switch (m_nWave)
	{
	case 0:		//�����̋ʍ����[�V����
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
		pCamera->SetHomingSpeed(0.2f);
		
		pDevil->SetModelColor(CModel::COLORTYPE_TRUE_A, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		if (m_nCount >= 180)
		{
			pDevil->SetAction(CDevil::ACTION_KING, 0);
			pDevil->SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_nWave++;
		}

		break;

	case 1:		//�v���C���[�����̋����܂ŕ���

		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
		pCamera->SetHomingSpeed(0.2f);

		if (pDevil->GetMotion()->GetFinish())
		{
			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 120);
			m_nWave++;
		}

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

	m_nCount++;
}

//====================================================================
//�J�n�����o
//====================================================================
void CEventMovie::StageChangeMovie(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	CDevil* pDevil = CDevil::GetListTop();
	CPlayer* pPlayer = CGame::GetInstance()->GetPlayer(0);
	D3DXVECTOR3 PlayerPos = INITVECTOR3;

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	switch (m_nWave)
	{
	case 0:		//������

		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);	//�J�������C�x���g�p�ɕύX
		CGame::GetInstance()->GetTime()->SetStopTime(true);	//�^�C���̐i�s���~�߂�

		if (m_nCount >= 180)
		{
			pDevil->SetAction(CDevil::ACTION_SIGNAL_DOWN, 0);	//�{�胂�[�V�����ɕύX
			m_nWave++;
		}

		break;

	case 1:		//�{�胂�[�V����

		if (m_nCount >= 60)
		{
			pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);	//�J�������w��ʒu�Ɉړ�
			pCamera->SetCameraPosMode(D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y, -1000.0f), pDevil->GetPos());	//�J�����̎w��ʒu��ݒ�
			pCamera->SetHomingSpeed(0.01f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�
			pCamera->SetBib(true);	//�J������U����Ԃɐݒ�
			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 120);	//���[�V�������j���[�g�����ɕύX

			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;

	case 2:		//�J�����������Ă���


		if (m_nCount >= 200)
		{
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y - 100.0f, -1000.0f),
				D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y - 100.0f, pDevil->GetPos().z));	//�J�����̎w��ʒu��ݒ�
			pCamera->SetHomingSpeed(0.9f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�
			pCamera->SetBib(false);	//�J������U�����Ȃ���Ԃɐݒ�

			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;

	case 3:		//�}�b�v�𒵂ˏオ��

		if (m_nCount >= 5)
		{
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y, -1000.0f),
				D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y, pDevil->GetPos().z));	//�J�����̎w��ʒu��ݒ�
			pCamera->SetHomingSpeed(0.9f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�

			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;

	case 4:		//�}�b�v�����ɖ߂�

		pCamera->SetCameraPosMode(
			D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y, -1000.0f),
			D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y + 1000.0f, pDevil->GetPos().z));	//�J�����̎w��ʒu��ݒ�
		pCamera->SetHomingSpeed(0.05f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�

		m_nCount = 0;		//�J�E���g���Z�b�g
		m_nWave++;

		break;

	case 5:		//�J��������������A�I�u�W�F�N�g����ɔ��ł���

		PlayerPos = pPlayer->GetPos();
		PlayerPos.y += 50.0f;
		pPlayer->SetPos(PlayerPos);


		if (m_nCount >= 60)
		{
			//�v���C���[�̈ʒu�ݒ�
			PlayerPos = pPlayer->GetPos();
			PlayerPos = D3DXVECTOR3(0.0f, 1700.0f, -400.0f);
			pPlayer->SetPos(PlayerPos);

			//�J�����̎w��ʒu��ݒ�
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + 400.0f, -800.0f),
				D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + 400.0f, pPlayer->GetPos().z));
			pCamera->SetHomingSpeed(0.9f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�
			m_fSinFloat = pPlayer->GetPos().y;		//�T�C���J�[�u�p�̏����ʒu��

			//�����m�F�p(�f�o�b�O)===========================
			for (int TestX = 0; TestX < 10; TestX++)
			{
				for (int TestY = 0; TestY < 10; TestY++)
				{
					CEffect *pEffect = CEffect::Create();
					pEffect->SetPos(D3DXVECTOR3(PlayerPos.x - 500.0f + TestX * 100.0f, PlayerPos.y - 500.0f + TestY * 100.0f, PlayerPos.z));
					pEffect->SetRadius(50.0f);
					pEffect->SetDel(0.0f);
					pEffect->SetLife(500);
				}
			}
			//===============================================

			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;

	case 6:		//�v���C���[�ɃJ������������

		PlayerPos = pPlayer->GetPos();
		PlayerPos.y = m_fSinFloat + sinf(D3DX_PI * ((float)m_nCount / 120)) * 400.0f;
		pPlayer->SetPos(PlayerPos);
		pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (m_nCount >= 120)
		{
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + 400.0f, -800.0f),
				D3DXVECTOR3(INITVECTOR3));	//�J�����̎w��ʒu��ݒ�
			pCamera->SetHomingSpeed(0.05f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�

			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;


	case 7:		//�J���������Ɍ�����


		if (m_nCount >= 10)
		{
			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;

	default:

		CGame::GetInstance()->GetTime()->SetStopTime(false);	//�^�C���̐i�s��i�߂�
		pCamera->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
		CGame::GetInstance()->SetEvent(false);
		break;
	}

	m_nCount++;
}