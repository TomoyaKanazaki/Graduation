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
#include "cross.h"
#include "GamePlayer.h"
#include "bgObjManager.h"

//�ÓI�����o�ϐ��錾

//====================================================================
//�R���X�g���N�^
//====================================================================
CEventMovie::CEventMovie()
{
	m_State = STATE_START;
	m_nWave = 0;
	m_nCount = 0;
	m_fSinFloat = 0.0f;
	m_pEventModel = nullptr;
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
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// �{�^�������ꂽ�牉�o�X�L�b�v
		if (pInputKeyboard->GetTrigger(DIK_RETURN) ||
			pInputJoypad->GetTrigger(CInputJoypad::BUTTON_START, nCnt))
		{
			CGame::GetInstance()->SetEvent(false);	// �X�L�b�v����
		}
	}

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
		//�I�������o
		EndMovie();
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
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	CDevil* pDevil = CDevil::GetListTop();
	D3DXVECTOR3 DevilPos = pDevil->GetPos();
	float CameraDistance = 0.0f;

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	if (CGame::GetInstance()->GetEvent() == false)
	{ // �C�x���g�X�L�b�v���ꂽ�ꍇ
		CGame::GetInstance()->SetEvent(true);	// �X�L�b�v���Ȃ�

		// �Ō�̃t�F�[�Y�ɂ���
		m_nWave = 5;
		m_nCount = 150;

		pDevil->SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �f�r���̐F���ɖ߂�

		// �֎q�������������
		if (m_pEventModel != nullptr)
		{
			m_pEventModel->Uninit();
			m_pEventModel = nullptr;
		}
	}

	switch (m_nWave)
	{
	case 0:		//������

		//�f�r���̔�\��
		pDevil->SetModelColor(CModel::COLORTYPE_TRUE_A, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		//�J�����̏����ʒu��ݒ�
		pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);
		pCamera->SetCameraPosMode(D3DXVECTOR3(0.0f, 1000.0f, -3000.0f), DevilPos);
		pCamera->SetHomingSpeed(0.1f);

		m_nCount = 0;	
		m_nWave++;

		break;

	case 1:		//�X�e�[�W�𒍖ڂ���

		pDevil->SetModelColor(CModel::COLORTYPE_TRUE_A, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		if (m_nCount >= 20)
		{
			//�J�����̏����ʒu��ݒ�
			pCamera->SetCameraMode(CCamera::CAMERAMODE_AROUND);
			pCamera->SetAroundRot((D3DX_PI * ((float)m_nCount / (float)180) + (D3DX_PI * 1.0f)));
			CameraDistance = sqrtf((3000.0f) * (3000.0f) + (1000.0f) * (1000.0f));
			pCamera->SetAroundDistance(CameraDistance);

			m_nCount = 0;
			m_nWave++;
		}

		break;

	case 2:		//������ƃX�e�[�W���������

		//�J�����̌����Ƌ�����ݒ�
		pCamera->SetAroundRot((D3DX_PI * ((float)(m_nCount * 1.8f) / (float)240) + (D3DX_PI * 1.0f)));
		CameraDistance = sqrtf((3000.0f) * (3000.0f) + (1000.0f) * (1000.0f));
		pCamera->SetAroundDistance(CameraDistance);

		if (m_nCount >= 240)
		{
			m_nCount = 0;
			m_nWave++;
		}

		break;

	case 3:		//�����̋ʍ����[�V����
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
		pCamera->SetHomingSpeed(0.05f);

		if (m_nCount == 5)
		{
			// �G�t�F�N�g�̐���
			CEffekseer* pEffect = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_SPAWN_DEVIL), false, DevilPos, pDevil->GetRot(), D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		}

		if (m_nCount >= 80)
		{
			pDevil->SetAction(CDevil::ACTION_KING, 0);
			pDevil->SetModelColor(CModel::COLORTYPE_FALSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_nWave++;
		}

		break;

	case 4:		//�{�X�̏o��

		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);
		pCamera->SetHomingSpeed(0.2f);

		if (m_pEventModel == nullptr)
		{
			m_pEventModel = CObjectX::Create("data\\MODEL\\01_enemy\\03_devil\\chair.x");
			m_pEventModel->SetPos(pDevil->GetPos());
		}

		if (pDevil->GetMotion()->GetFinish())
		{
			pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(0.0f, 1600.0f, -900.0f),
				D3DXVECTOR3(INITVECTOR3));	//�J�����̎w��ʒu��ݒ�
			pCamera->SetHomingSpeed(0.02f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�

			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 120);
			m_nWave++;
			m_nCount = 0;
		}

		break;

	case 5:		//

		if (m_pEventModel != nullptr)
		{
			m_pEventModel->SetScaling(D3DXVECTOR3(m_pEventModel->GetScaling().x - 0.015f, m_pEventModel->GetScaling().y - 0.015f, m_pEventModel->GetScaling().z - 0.015f));
			//m_pEventModel->SetPos(D3DXVECTOR3(m_pEventModel->GetPos().x, m_pEventModel->GetPos().y + 3.0f, m_pEventModel->GetPos().z + 5.0f));

			if (m_pEventModel->GetScaling().x <= 0.0f)
			{
				m_pEventModel->Uninit();
				m_pEventModel = nullptr;
			}
		}

		if (m_nCount >= 150)
		{
			pCamera->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);

			m_nCount = 0;
			m_nWave++;
		}

		break;

	default:

		CGame::GetInstance()->GetTime()->SetStopTime(false);	//�^�C���̐i�s��i�߂�
		CGame::GetInstance()->SetEvent(false);
		m_nWave = 0;
		break;
	}

	m_nCount++;
}

//====================================================================
//���`�ԕω����o
//====================================================================
void CEventMovie::StageChangeMovie(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);

	CDevil* pDevil = CDevil::GetListTop();
	CPlayer* pPlayer = CGame::GetInstance()->GetPlayer(0);
	D3DXVECTOR3 PlayerPos = INITVECTOR3;

	CPlayer* pPlayer2 = nullptr;
	D3DXVECTOR3 PlayerPos2 = INITVECTOR3;

	if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
	{
		pPlayer2 = CGame::GetInstance()->GetPlayer(1);
	}

	if (CGame::GetInstance()->GetEvent() == false)
	{ // �C�x���g�X�L�b�v���ꂽ�ꍇ
		m_nWave = 6;
		m_nCount = 120;
		CGame::GetInstance()->SetEvent(true);	// �X�L�b�v���Ȃ�
	}

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	switch (m_nWave)
	{
	case 0:		//������
		pCamera->SetBib(false);	//�J������U����Ԃɐݒ�
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);	//�J�������C�x���g�p�ɕύX
		CGame::GetInstance()->GetTime()->SetStopTime(true);	//�^�C���̐i�s���~�߂�

		pDevil->SetAction(CDevil::ACTION_NEUTRAL, 60);	//���[�V�������j���[�g�����ɕύX

		pPlayer->PlayerNumberDisp(false);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			pPlayer2->PlayerNumberDisp(false);
		}

		if (m_nCount >= 180)
		{
			pDevil->SetAction(CDevil::ACTION_ANGER, 60);	//�{�胂�[�V�����ɕύX
			m_nWave++;
		}

		// ���u�n�w�i���f���̔�\������
		CBgObjManager::GetInstance()->SetDeviceAppear(false);

		break;

	case 1:		//�{�胂�[�V����

		if (m_nCount >= 60)
		{
			pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);	//�J�������w��ʒu�Ɉړ�
			pCamera->SetCameraPosMode(D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y + 200.0f, -1000.0f), pDevil->GetPos());	//�J�����̎w��ʒu��ݒ�
			pCamera->SetHomingSpeed(0.01f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�
			pCamera->SetBib(true);	//�J������U����Ԃɐݒ�

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

		//�v���C���[��ړ�
		PlayerPos = pPlayer->GetPos();
		PlayerPos.y += 50.0f;
		pPlayer->SetPos(PlayerPos);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			PlayerPos2 = pPlayer2->GetPos();
			PlayerPos2.y += 50.0f;
			pPlayer2->SetPos(PlayerPos2);
		}

		//�N���X��ړ�
		ShootUpCross();

		if (m_nCount >= 60)
		{
			//�v���C���[�̈ʒu�ݒ�
			PlayerPos = pPlayer->GetPos();
			PlayerPos = D3DXVECTOR3(0.0f, 1700.0f, -400.0f);
			pPlayer->SetPos(PlayerPos);

			if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
			{
				//�v���C���[�̈ʒu�ݒ�
				PlayerPos2 = pPlayer2->GetPos();
				PlayerPos2 = D3DXVECTOR3(200.0f, 1900.0f, 100.0f);
				pPlayer2->SetPos(PlayerPos2);
			}

			//�\���˂̈ʒu�ݒ�
			SetPosEventCross();

			//�J�����̎w��ʒu��ݒ�
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pPlayer->GetPos().x + 50.0f, pPlayer->GetPos().y + 200.0f, -550.0f),
				D3DXVECTOR3(pPlayer->GetPos().x + 50.0f, pPlayer->GetPos().y + 200.0f, pPlayer->GetPos().z));
			pCamera->SetHomingSpeed(0.9f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�
			m_fSinFloat = pPlayer->GetPos().y;		//�T�C���J�[�u�p�̏����ʒu��

			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;

	case 6:		//�v���C���[�ɃJ������������
		CMapSystem::GetInstance()->ResetMap();

		PlayerPos = pPlayer->GetPos();
		PlayerPos.y = m_fSinFloat + sinf(D3DX_PI * ((float)m_nCount / 120)) * 200.0f;
		pPlayer->SetPos(PlayerPos);
		pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			PlayerPos2 = pPlayer2->GetPos();
			PlayerPos2.y = (m_fSinFloat + sinf(D3DX_PI * ((float)m_nCount / 120)) * 200.0f) + 200.0f;
			pPlayer2->SetPos(PlayerPos2);
			pPlayer2->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		SetPosCross(PlayerPos);

		if (m_nCount >= 120)
		{
			pCamera->SetCameraPosMode(
				D3DXVECTOR3(pPlayer->GetPos().x, pPlayer->GetPos().y + 400.0f, -800.0f),
				D3DXVECTOR3(INITVECTOR3));	//�J�����̎w��ʒu��ݒ�
			pCamera->SetHomingSpeed(0.05f);	//�J�����̖ڕW�܂ł̃z�[�~���O���x��ݒ�

			//pPlayer->SetState(CPlayer::STATE_EGG);
			pPlayer->Reivel(PlayerPos);
			pPlayer->SetPos(PlayerPos);

			if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
			{
				pPlayer2->Reivel(PlayerPos2);
				pPlayer2->SetPos(PlayerPos2);
			}

			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;


	case 7:		//�J���������Ɍ�����

		// ���u�n�w�i���f���̔�\������
		CBgObjManager::GetInstance()->SetDeviceAppear(true);

		if (m_nCount >= 10)
		{
			m_nCount = 0;		//�J�E���g���Z�b�g
			m_nWave++;
		}

		break;

	default:

		pPlayer->PlayerNumberDisp(true);

		if (CManager::GetInstance()->GetGameMode() == CManager::GAME_MODE::MODE_MULTI)
		{
			pPlayer2->PlayerNumberDisp(true);
		}

		CGame::GetInstance()->NextStage();
		CGame::GetInstance()->GetTime()->SetStopTime(false);	//�^�C���̐i�s��i�߂�
		pCamera->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
		CGame::GetInstance()->SetEvent(false);
		break;
	}

	m_nCount++;
}

//====================================================================
//�I�����o
//====================================================================
void CEventMovie::EndMovie(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	CDevil* pDevil = CDevil::GetListTop();
	D3DXVECTOR3 DevilPos = pDevil->GetPos();
	float CameraDistance = 0.0f;
	float a;

	//float fDistance = sqrtf((PlayerPos.x - DevilPos.x) * (PlayerPos.x - DevilPos.x) + (PlayerPos.z - DevilPos.z) * (PlayerPos.z - DevilPos.z));
	//float fAngle = atan2f(PlayerPos.z - DevilPos.z, DevilPos.x - PlayerPos.x) + D3DX_PI * 0.5f;

	if (CGame::GetInstance()->GetEvent() == false)
	{ // �C�x���g�X�L�b�v���ꂽ�ꍇ
		m_nWave = 4;
		m_nCount = 120;
		CGame::GetInstance()->SetEvent(true);	// �X�L�b�v���Ȃ�
	}

	switch (m_nWave)
	{
	case 0:		//������

		//�J�����̏����ʒu��ݒ�
		pCamera->SetBib(true);									//�J������U����Ԃɐݒ�
		pCamera->SetCameraMode(CCamera::CAMERAMODE_EVENTBOSS);	//�J�������C�x���g�p�ɕύX
		pCamera->SetCameraPosMode(D3DXVECTOR3(0.0f, 1000.0f, -3000.0f), DevilPos);
		pCamera->SetHomingSpeed(0.1f);

		m_nCount = 0;
		m_nWave++;

		break;

	case 1:		//

		if (m_nCount >= 120)
		{
			pCamera->SetBib(false);									//�J������U����Ԃɐݒ�
			pCamera->SetCameraMode(CCamera::CAMERAMODE_SETPOS);
			pCamera->SetCameraPosMode(pCamera->GetPosV(), DevilPos);
			pCamera->SetHomingSpeed(0.5f);

			m_fSinFloat = DevilPos.y;

			//�f�r���̃��[�V�����ύX
			pDevil->SetAction(CDevil::ACTION_RUN, 0);

			// �G�t�F�N�g�̐���
			D3DXMATRIX mat = {};
			D3DXVECTOR3 ef = useful::CalcMatrix(DevilPos, pDevil->GetRot(), mat);
			CEffekseer* pEffect = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_SPAWN_DEVIL), false, DevilPos, pDevil->GetRot(), D3DXVECTOR3(50.0f, 50.0f, 50.0f));

			m_nCount = 0;
			m_nWave++;
		}

		break;

	case 2:		//

		a = m_fSinFloat + (sinf(D3DX_PI * (float)m_nCount / 120) * 800.0f);

		pDevil->SetRot(D3DXVECTOR3(pDevil->GetRot().x + 0.2f, pDevil->GetRot().y, pDevil->GetRot().z));
		pDevil->SetPos(D3DXVECTOR3(pDevil->GetPos().x, a, pDevil->GetPos().z));
		pCamera->SetCameraPosMode(pCamera->GetPosV(), DevilPos);

		if (m_nCount >= 120)
		{
			pDevil->SetRot(D3DXVECTOR3(D3DX_PI * 1.0f, 0.0f, 0.0f));
			pDevil->SetPos(D3DXVECTOR3(pDevil->GetPos().x, pDevil->GetPos().y + 100.0f, pDevil->GetPos().z));

			//�f�r���̔�\��
			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 0);

			m_nCount = 0;
			m_nWave++;
		}

		break;


	case 3:		//

		if (m_nCount >= 120)
		{
			pDevil->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

			//�f�r���̔�\��
			pDevil->SetAction(CDevil::ACTION_RUN, 0);

			m_nCount = 0;
			m_nWave++;

			// �G�t�F�N�g�̐���
			D3DXMATRIX mat = {};
			D3DXVECTOR3 ef = useful::CalcMatrix(DevilPos, pDevil->GetRot(), mat);
			CEffekseer* pEffect = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_SPAWN_DEVIL), false, DevilPos, pDevil->GetRot(), D3DXVECTOR3(25.0f, 25.0f, 25.0f));
		}

		break;

	case 4:		//
		
		pDevil->SetPos(D3DXVECTOR3(pDevil->GetPos().x + 10.0f, pDevil->GetPos().y, pDevil->GetPos().z));
		pCamera->SetCameraPosMode(pCamera->GetPosV(), DevilPos);

		if (m_nCount >= 120)
		{
			//�f�r���̔�\��
			pDevil->SetAction(CDevil::ACTION_NEUTRAL, 0);

			CEffekseer* pEffect = MyEffekseer::EffectCreate((CMyEffekseer::TYPE)((int)CMyEffekseer::TYPE_SPAWN_DEVIL), false, DevilPos, pDevil->GetRot(), D3DXVECTOR3(50.0f, 50.0f, 50.0f));

			m_nCount = 0;
			m_nWave++;
		}

		break;

	default:

		//CGame::GetInstance()->GetTime()->SetStopTime(false);	//�^�C���̐i�s��i�߂�
		//CGame::GetInstance()->SetEvent(false);
		//m_nWave = 0;
		break;
	}

	m_nCount++;
}

//====================================================================
//�\���˂�ł��グ�鏈��
//====================================================================
void CEventMovie::ShootUpCross(void)
{
	// �G�̃��X�g�\����������Δ�����
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // ���X�g���擾

	// �G�̃��X�g�̒��g���m�F����
	for (CCross* pCorss : list)
	{
		D3DXVECTOR3 pos = pCorss->GetPos();
		pos.y += 40.0f;
		pCorss->SetPos(pos);
	}
}

//====================================================================
//�\���˂̈ʒu�ݒ�
//====================================================================
void CEventMovie::SetPosCross(D3DXVECTOR3 Pos)
{
	// �G�̃��X�g�\����������Δ�����
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // ���X�g���擾

	// �G�̃��X�g�̒��g���m�F����
	for (CCross* pCorss : list)
	{
		D3DXVECTOR3 MyEventpos = pCorss->GetEventPos();
		pCorss->SetPos(D3DXVECTOR3(MyEventpos + Pos));
	}
}

//====================================================================
//�\���˂̈ʒu�ݒ�
//====================================================================
void CEventMovie::SetPosEventCross()
{
	// �G�̃��X�g�\����������Δ�����
	if (CCross::GetList() == nullptr) { return; }
	std::list<CCross*> list = CCross::GetList()->GetList();    // ���X�g���擾

	// �G�̃��X�g�̒��g���m�F����
	for (CCross* pCorss : list)
	{
		pCorss->SetEventPos(D3DXVECTOR3(
			((float)(rand() % 400)), 
			(float)(rand() % 500), 
			(+400.0f + (float)(rand() % 600))));
	}
}