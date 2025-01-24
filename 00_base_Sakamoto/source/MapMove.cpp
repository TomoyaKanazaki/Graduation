//============================================
//
//	�}�b�v�̓��� [MapMove.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MapMove.h"
#include "object2D.h"
#include "Fade.h"
#include "sound.h"
#include "objmeshField.h"
#include "enemy.h"
#include "ScrollDevice.h"
#include "SlopeDevice.h"
#include "signal.h"
#include "effect.h"

#include "bowabowa.h"
#include "Cross.h"
#include "player.h"
#include "railblock.h"
#include "RollRock.h"
#include "tile.h"
#include "friedegg.h"
#include "fire.h"
#include "wall.h"
#include "camera.h"

//===========================================
// �萔��`
//===========================================
namespace
{
	const int SCROOL_TIME = 300;						// �X�N���[������

	const int SCROOL_MOVEGRID_01 = 3;					// �X�N���[���̈ړ��}�X��
	const float SCROOL_SPEED_01 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_01 / SCROOL_TIME);				// �X�N���[���̈ړ����x

	const int SCROOL_COUNT_02 = 12;						// �X�N���[���̈ړ���
	const int SCROOL_MOVEGRID_02 = 3;					// �X�N���[���̈ړ��}�X��
	const float SCROOL_SPEED_02 = (CMapSystem::GetGritSize() * SCROOL_MOVEGRID_02) / SCROOL_COUNT_02;			// �X�N���[���̈ړ����x

	const int SLOPE_TIME = 300;						// �X�����쎞��
	const int SLOPE_RAND = 50;						// �X�������m��
	float STAGE_ROT_LIMIT = D3DX_PI * 0.15f;		// �X���̊p�x����

	const float SLOPE_SPEED01 = 0.00075f;			// �X���̈ړ����x

	const float SLOPE_SPEED02 = 0.0125f;			// �X���̈ړ����x

	const int EFFECT_NUM = 3;			// ��x�ɐ�������G�t�F�N�g�̐�
	const float EFFECT_RANGE = 1.5f;	// �G�t�F�N�g�̐����Ԋu

	const float SLOPEUP_MAG = 1.5f;		// �X�����̈ړ����x�̔{��
	const float SLOPEDOWN_MAG = 0.5f;	// �X�����̈ړ����x�̔{��

	const int MAX_PRAB_MODE = 100;		// �ړ����@���I�̊m���̍ő�l
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CMapMove::CMapMove() : 
	m_fEffectTime(0.0f)
{
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_AutoMoveRot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_fActionCount = 0.0f;
	m_State = MOVE_WAIT;
	m_SlopeOld = MOVE_WAIT;
	m_nStateCount = 0;
	m_CollisionRot = 0.0f;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
	m_bSlope = false;
	m_MinGrid = CMapSystem::GRID(0, 0);
	m_MaxGrid = CMapSystem::GRID(NUM_WIGHT - 1, NUM_HEIGHT - 1);
	m_ScrollArrowOld = 0;
	m_SlopwArrowOld = 0;
	m_ScrollType = SCROLL_TYPE_NORMAL;
	m_nStateNum = 0;
	m_SlopeType = 0;
	m_bScrollOK = false;
	m_fScrollMove = 0.0f;
	m_fScrollEndLine = 0.0f;

	m_MoveMode = MOVEMODE_WAIT;		// �ړ����[�h
	m_RotType = ROTTYPE_MAX;			// �����̎��
	m_OldRotType = m_RotType;			// �O��̌����̎��
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMapMove::~CMapMove()
{

}

//====================================================================
//��������
//====================================================================
CMapMove* CMapMove::Create()
{
	CMapMove* pSystem = new CMapMove();

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSystem->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pSystem;
}

//====================================================================
//����������
//====================================================================
HRESULT CMapMove::Init(void)
{
	m_ScrollType = (SCROLL_TYPE)CManager::GetInstance()->GetScrollType();

	//��Ԋ֘A�̏�����
	m_State = MOVE_WAIT;
	m_SlopeOld = MOVE_WAIT;
	m_nStateCount = SCROOL_TIME * 0.5f;
	m_move = INITVECTOR3;
	m_MapDifference = INITVECTOR3;
	m_DevilRot = INITVECTOR3;
	m_bSlope = false;
	m_fScrollMove = 0.0f;
	m_fEffectTime = 0.0f;
	m_Objmove = INITVECTOR3;
	m_DevilPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MinGrid = CMapSystem::GRID(0, 0);
	m_MaxGrid = CMapSystem::GRID(NUM_WIGHT - 1, NUM_HEIGHT - 1);
	m_ScrollArrowOld = 0;
	m_SlopwArrowOld = 0;
	m_nStateNum = 0;
	m_SlopeType = 0;
	m_bScrollOK = false;
	m_fScrollEndLine = 0.0f;

	m_MoveMode = MOVEMODE_WAIT;		// �ړ����[�h
	m_RotType = ROTTYPE_MAX;			// �����̎��
	m_OldRotType = m_RotType;			// �O��̌����̎��

	//�J������U�������Ȃ�
	CManager::GetInstance()->GetCamera(0)->SetBib(false);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CMapMove::Uninit(void)
{
	// ���g���폜����
	delete this;
}

//====================================================================
//�X�V����
//====================================================================
void CMapMove::Update(void)
{
	// �ړ��ʂ����Z�b�g
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �}�b�v�̌X��
	m_DevilRot = CObjmeshField::GetListTop()->GetRot();

	//��Ԃ̊Ǘ�
	StateManager();

	//�X�e�[�W�O�ɂ���I�u�W�F�N�g�̏���
	CollisionOut();

	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	// �G�t�F�N�g�𐶐�
	switch (m_MoveMode)
	{
	case MOVEMODE_WAIT:

		// �������Ԃ����Z�b�g
		m_fEffectTime = EFFECT_RANGE;
		break;

	default:

		// �������Ԃ��X�V
		m_fEffectTime -= DeltaTime::Get();

		// �^�C�}�[
		if (m_fEffectTime > 0.0f)
		{
			break;
		}

		// �������Ԃ����Z�b�g
		m_fEffectTime = EFFECT_RANGE;

		// �O���b�h���̎擾
		CMapSystem* system = CMapSystem::GetInstance();
		CMapSystem::GRID maxGrid = system->GetMapGrid();
		D3DXMATRIX mat = *CObjmeshField::GetListTop()->GetMatrix();
		D3DXVECTOR3 rotEffect = useful::CalcMatrixToRot(mat);
		for (int i = 0; i < EFFECT_NUM; ++i)
		{
			// ���������Z�o
			CMapSystem::GRID grid = { rand() % maxGrid.x, rand() % maxGrid.z };

			// ����
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_QUAKE, false, useful::CalcMatrix(grid.ToWorld(), INITVECTOR3, mat), rotEffect);
		}

		break;
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CMapMove::Draw(void)
{
	// Do Nothing
}

//====================================================================
//��ԊǗ�
//====================================================================
#if 0	// ����R�[�h
void CMapMove::StateManager(void)
{
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

	switch (m_State)
	{
	case MOVE_WAIT: // �ҋ@��� -> ���Ɉړ����N����

		// ��ԏI����
		if (m_nStateCount <= 0)
		{
			// �T�E���h�̒�~
			StopSound();

			// ���̍s���𒊑I
			int nRand = rand() % 101;

			if (nRand <= SLOPE_RAND)
			{ // �X���̎w�聓�̎�

				m_MoveMode = MOVEMODE_SLOPE;	// �X�����[�h�ɂ���
			}
			else
			{// �X���̎w�聓����Ȃ���
				m_MoveMode = MOVEMODE_SCROLL;	// �X�N���[�����[�h�ɂ���
			}

			// ��Ԑݒ�
			switch (m_MoveMode)
			{
			case CMapMove::MOVEMODE_SCROLL:		// �X�N���[��

				// �X�N���[�����Ԑݒ�
				m_nStateCount = SCROOL_TIME * 2;

				// �X���̌����ݒ肷��
				SetSlopeRot();

				// �X�N���[�����(����)�ݒ�
				switch (m_DevilArrow + MOVE_SCROLL_UP)
				{
				case MOVE_SCROLL_UP:		// ��
					m_State = MOVE_SCROLL_UP;

					// �T�E���h�Đ�
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_UP);
					break;

				case MOVE_SCROLL_DOWN:		// ��
					m_State = MOVE_SCROLL_DOWN;

					// �T�E���h�Đ�
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_DOWN);
					break;

				case MOVE_SCROLL_LEFT:		// ��
					m_State = MOVE_SCROLL_LEFT;

					// �T�E���h�Đ�
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_LEFT);
					break;

				case MOVE_SCROLL_RIGHT:		// �E
					m_State = MOVE_SCROLL_RIGHT;

					// �T�E���h�Đ�
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_RIGHT);
					break;
				}

				// �}�b�v���u�̐ݒ�
				SetDeviceMap();

				m_bScrollOK = false;

				break;

			case CMapMove::MOVEMODE_SLOPE:		// �X��

				// �X���Ă����Ԃ��ǂ�����؂�ւ���
				m_bSlope = !m_bSlope;

				if (m_bSlope)
				{// �X����Ԃ̎�

					m_nStateCount = SLOPE_TIME;

					// �X���̌����ݒ肷��
					SetSlopeRot();

					// �X�����(����)�ݒ�
					switch (m_DevilArrow + MOVE_SLOPE_UP)
					{
					case MOVE_SLOPE_UP:		// ��
						m_State = MOVE_SLOPE_UP;

						// �T�E���h�Đ�
						CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_UP);
						break;
					case MOVE_SLOPE_DOWN:	// ��
						m_State = MOVE_SLOPE_DOWN;

						// �T�E���h�Đ�
						CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_DOWN);
						break;
					case MOVE_SLOPE_LEFT:	// ��
						m_State = MOVE_SLOPE_LEFT;

						// �T�E���h�Đ�
						CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_LEFT);
						break;
					case MOVE_SLOPE_RIGHT:	// �E
						m_State = MOVE_SLOPE_RIGHT;

						// �T�E���h�Đ�
						CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SIGN_RIGHT);
						break;
					}

					// �}�b�v���u�̐ݒ�
					SetDeviceMap();
				}
				else
				{// �X���߂���Ԃ̎�

					// �T�E���h�̒�~
					StopSound();

					// �X����߂��������{�̎��Ԃ��w�肵�A�߂�؂�����X����Ԃ��I���Ƃ���
					m_nStateCount = SLOPE_TIME * 2;

					m_State = m_SlopeOld;

					//�J������U��������
					CManager::GetInstance()->GetCamera(0)->SetBib(true);

					// �X�����u�̃��X�g�\����������Δ�����
					if (CSlopeDevice::GetList() == nullptr) { return; }
					std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // ���X�g���擾

					// �X�����u�̃��X�g�̒��g���m�F����
					for (CSlopeDevice* pSlopeDevice : list)
					{
						// �����̌X�����u���㏸��ԂɕύX
						pSlopeDevice->SetStateArrowBack((CScrollArrow::Arrow)m_SlopwArrowOld);
					}
				}


				break;
			
			default:
				assert(false);
				break;
			}
		}

		break;

	// �X�N���[�����̏�� -> �ҋ@��ԂɂȂ�
	case MOVE_SCROLL_UP:
	case MOVE_SCROLL_DOWN:
	case MOVE_SCROLL_LEFT:
	case MOVE_SCROLL_RIGHT:

		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:
			Move();
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

			if (m_nStateCount % (SCROOL_TIME / SCROOL_COUNT_02) == 0)
			{
				Move();
			}

			break;

		default:

			break;
		}

		if (m_bScrollOK == true)
		{
			m_State = MOVE_WAIT;
			m_nStateCount = 120;

			// �}�b�v�ړ����u�̃��X�g�\����������Δ�����
			if (CScrollDevice::GetList() == nullptr) { return; }
			std::list<CScrollDevice*> list = CScrollDevice::GetList()->GetList();    // ���X�g���擾

			// �}�b�v�ړ����u�̃��X�g�̒��g���m�F����
			for (CScrollDevice* pScrollDevice : list)
			{
				// �ʏ��ԂɕύX
				pScrollDevice->SetState(CScrollDevice::STATE_NORMAL);
			}
		}

		break;

	// �X�����̏�� -> �ҋ@��ԂɂȂ�
	case MOVE_SLOPE_UP:
	case MOVE_SLOPE_DOWN:
	case MOVE_SLOPE_LEFT:
	case MOVE_SLOPE_RIGHT:
		switch (m_ScrollType)
		{
		case CMapMove::SCROLL_TYPE_NORMAL:

			if (m_bSlope)
			{
				Slope();
			}
			else
			{
				BackSlope(); // ���݂̌X����Ԃ���ɖ߂��֐�
			}
			break;

		case CMapMove::SCROLL_TYPE_RETRO:

			if (m_nStateCount % 25 == 0)
			{
				if (m_bSlope)
				{
					Slope();
				}
				else
				{
					BackSlope();
				}

				// �X�����u�̃��X�g�\����������Δ�����
				if (CSlopeDevice::GetList() == nullptr) { return; }
				std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // ���X�g���擾

				// �X���ړ����u�̃��X�g�̒��g���m�F����
				for (CSlopeDevice* pSlopeDevice : list)
				{
					// ���g���p�̌X���ړ����I��
					pSlopeDevice->SetUseRetroMove(true);
				}
			}
			break;

		default:

			break;
		}

		if (m_nStateCount < 0)
		{
			m_State = MOVE_WAIT;
			m_nStateCount = 120;
		}

		break;
	}

	if (m_nStateCount >= 0)
	{
		m_nStateCount--;
	}

	DebugProc::Print(DebugProc::POINT_RIGHT, "�X�N���[����� : ");

	auto str = magic_enum::enum_name(m_State);

	DebugProc::Print(DebugProc::POINT_RIGHT, str.data());
	DebugProc::Print(DebugProc::POINT_RIGHT, "\n");

}


#else	// �����������Ă�R�[�h
void CMapMove::StateManager(void)
{
	// ���ꂼ��̓���
	switch (m_MoveMode)
	{
	case CMapMove::MOVEMODE_SCROLL:		// �X�N���[��

		// �ړ�����
		Move();

		if (m_bScrollOK)
		{ // �������̈ړ������ŃX�N���[�������������ꍇ

			// �X�����u�̏���
			SetDeviceMap();

			m_MoveMode = MOVEMODE_WAIT;	// �ҋ@��Ԃɂ���
			m_nStateCount = 120;		// ��ԊǗ��J�E���^�[��������
		}
		else if(!m_bScrollOK)
		{ // �������̈ړ������ŃX�N���[�����������ĂȂ��ꍇ
			if (m_nStateCount >= 0)
			{
				m_nStateCount--;	// ��ԊǗ��J�E���^�[�����Z
			}
		}

		break;

	case CMapMove::MOVEMODE_SLOPE:		// �X��

		// �X���̉�]����
		MoveScroll();

		if (m_nStateCount < 0)
		{
			m_MoveMode = MOVEMODE_WAIT;		// �ҋ@��Ԃɂ���
			m_nStateCount = 120;
		}
		else
		{
			m_nStateCount--;		// ��ԊǗ��J�E���^�[�����Z
		}

		break;

	case CMapMove::MOVEMODE_WAIT:		// �ҋ@

		// �X�N���[�����X���̔��f
		if (m_nStateCount > 0)
		{ // ���Ԍo�߂��ĂȂ��ꍇ
			m_nStateCount--;		// ��ԕω������Ȃ�
		}
		else
		{ // ���Ԍo�߂����ꍇ

			// �T�E���h�̒�~
			StopSound();

			// ���̍s���𒊑I
			int nRand = rand() % MAX_PRAB_MODE + 1;

			if (nRand <= SLOPE_RAND)
			{ // �X���̎w�聓�̎�

				m_MoveMode = MOVEMODE_SLOPE;		// �X�����[�h�ɂ���
				m_bSlope = !m_bSlope;				// �X�����Ԃ��ǂ�����؂�ւ���

				if (m_bSlope)
				{// �X�����Ԃ̎�
					m_nStateCount = SCROOL_TIME;	// �X�N���[�����Ԑݒ�

					// �X���̐ݒ�
					SetSlope();
				}
				else if (!m_bSlope)
				{ // ���ɖ߂���

					// �X���߂��̐ݒ�
					SetBackSlope();
				}
			}
			else
			{// �X���̎w�聓����Ȃ���

				m_MoveMode = MOVEMODE_SCROLL;		// �X�N���[�����[�h�ɂ���
				m_nStateCount = SCROOL_TIME * 2;	// �X�N���[�����Ԑݒ�
				m_bScrollOK = false;				// �X�N���[���������������ǂ���

				// �X�N���[���̐ݒ�
				SetScroll();
			}
		}

		break;
	default:
		assert(false);
		break;
	}

//#if 1
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, "�X�N���[����� : ");
//
//	auto str = magic_enum::enum_name(m_State);
//
//	DebugProc::Print(DebugProc::POINT_RIGHT, str.data());
//	DebugProc::Print(DebugProc::POINT_RIGHT, "\n");
//#endif

}
#endif

//====================================================================
// �X�N���[���̐ݒ�
//====================================================================
void CMapMove::SetScroll(void)
{
	// �X���̌����ݒ肷��
	SetSlopeRot();

	// �}�b�v���u�̐ݒ�
	SetDeviceMap();

	// SE�Đ�����
	CMapMove::PlaySound();
}

//====================================================================
// �X���̐ݒ�
//====================================================================
void CMapMove::SetSlope(void)
{
	m_nStateCount = SLOPE_TIME;

	// �X���̌����ݒ肷��
	SetSlopeRot();

	// �}�b�v���u�̐ݒ�
	SetDeviceMap();

	// SE�Đ�����
	CMapMove::PlaySound();
}

//====================================================================
// ���ɖ߂��X���̐ݒ�
//====================================================================
void CMapMove::SetBackSlope(void)
{
	// �T�E���h�̒�~
	StopSound();

	// �X����߂��������{�̎��Ԃ��w�肵�A�߂�؂�����X����Ԃ��I���Ƃ���
	m_nStateCount = SLOPE_TIME * 2;

	m_RotType = m_OldRotType;

	//�J������U��������
	CManager::GetInstance()->GetCamera(0)->SetBib(true);

	// �X�����u�̃��X�g�\����������Δ�����
	if (CSlopeDevice::GetList() == nullptr) { return; }
	std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // ���X�g���擾

	// �X�����u�̃��X�g�̒��g���m�F����
	for (CSlopeDevice* pSlopeDevice : list)
	{
		// �����̌X�����u���㏸��ԂɕύX
		pSlopeDevice->SetStateArrowBack((CScrollArrow::Arrow)m_SlopwArrowOld); // TODO�F�����̕ϐ��ς��ā`yamete~
	}
}

//====================================================================
// �X���̌�����ݒ�
//====================================================================
void CMapMove::SetSlopeRot(void)
{
	//�X�������w�菈��
	m_RotType = (ROTTYPE)(rand() % 2);

	if (m_SlopwArrowOld == ROTTYPE_UP || m_SlopwArrowOld == ROTTYPE_DOWN)
	{// �O��̌X���������㉺�������ꍇ

		// ����̌X�������͍��E�ɂ���
		if (m_RotType == ROTTYPE_UP)
		{ // ��̏ꍇ

			m_RotType = ROTTYPE_LEFT;		// ��
		}
		else if (m_RotType == ROTTYPE_DOWN)
		{ // ���̏ꍇ

			m_RotType = ROTTYPE_RIGHT;		// �E
		}
	}

	// ����̌X���������L�^����
	m_SlopwArrowOld = m_RotType;
	m_nStateNum = m_RotType;
}

//====================================================================
// �}�b�v���u�̐ݒ�
//====================================================================
void CMapMove::SetDeviceMap(void)
{
	// �ړ���Ԑݒ�
	switch (m_MoveMode)
	{
	case CMapMove::MOVEMODE_SCROLL:		// �X�N���[��
	{
		// �}�b�v�ړ����u�̃��X�g�\����������Δ�����
		if (CScrollDevice::GetList() == nullptr) { return; }
		std::list<CScrollDevice*> list = CScrollDevice::GetList()->GetList();    // ���X�g���擾

		// �}�b�v�ړ����u�̃��X�g�̒��g���m�F����
		for (CScrollDevice* pScrollDevice : list)
		{
			// �����w���Ԑݒ�i��v���Ɖ�]�j
			pScrollDevice->SetStateArrow((CScrollArrow::Arrow)m_RotType);
		}
	}
		break;
	case CMapMove::MOVEMODE_SLOPE:		// �X��
	{
		//�J������U��������
		CManager::GetInstance()->GetCamera(0)->SetBib(true);

		// �X�����u�̃��X�g�\����������Δ�����
		if (CSlopeDevice::GetList() == nullptr) { return; }
		std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // ���X�g���擾

		// �X�����u�̃��X�g�̒��g���m�F����
		for (CSlopeDevice* pSlopeDevice : list)
		{
			// �����̌X�����u���㏸��ԂɕύX
			pSlopeDevice->SetStateArrow((CScrollArrow::Arrow)m_RotType);
		}
	}
		break;
	
	default:
		assert(false);
		break;
	}
	
}

//====================================================================
// �X���̉�]����
//====================================================================
void CMapMove::MoveScroll(void)
{
	// �X������
	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:		// �m�[�}��

		if (m_bSlope)
		{ // �X����Ԃ̂Ƃ�
			Slope();
		}
		else
		{ // ���ɖ߂���Ԃ̂Ƃ�
			BackSlope(); // ���݂̌X����Ԃ���ɖ߂��֐�
		}
		break;

	case CMapMove::SCROLL_TYPE_RETRO:		// ���g��

		if (m_nStateCount % 25 == 0)
		{
			if (m_bSlope)
			{
				Slope();
			}
			else
			{
				BackSlope();
			}

			// �X�����u�̃��X�g�\����������Δ�����
			if (CSlopeDevice::GetList() == nullptr) { return; }
			std::list<CSlopeDevice*> list = CSlopeDevice::GetList()->GetList();    // ���X�g���擾

			// �X���ړ����u�̃��X�g�̒��g���m�F����
			for (CSlopeDevice* pSlopeDevice : list)
			{
				// ���g���p�̌X���ړ����I��
				pSlopeDevice->SetUseRetroMove(true);
			}
		}
		break;

	default:
		assert(false);

		break;
	}
}

//====================================================================
//�ړ�����
//====================================================================
void CMapMove::Move()
{
	// ���[�J���ϐ�
	bool bMoveScroll = false;

	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:		// �m�[�}��
		switch (m_RotType)
		{
		case ROTTYPE_UP:		// ��
			m_move.z = SCROOL_SPEED_01;
			break;
		case ROTTYPE_DOWN:		// ��
			m_move.z = -SCROOL_SPEED_01;
			break;
		case ROTTYPE_LEFT:		// ��
			m_move.x = -SCROOL_SPEED_01;
			break;
		case ROTTYPE_RIGHT:		// �E
			m_move.x = SCROOL_SPEED_01;
			break;
		}

		m_fScrollMove += SCROOL_SPEED_01;
		if (m_fScrollMove >= (SCROOL_MOVEGRID_01 * 100))
		{ // �w�肳�ꂽ�X�N���[�����ړ������ꍇ
			bMoveScroll = true;		// �X�N���[��������Ԃɂ���
		}
		break;

	case CMapMove::SCROLL_TYPE_RETRO:		// ���g��

		if (m_nStateCount % (SCROOL_TIME / SCROOL_COUNT_02) == 0)
		{ // ��莞�Ԃ��ƂɈړ�
			switch (m_RotType)
			{
			case ROTTYPE_UP:		// ��
				m_move.z = SCROOL_SPEED_02;
				break;
			case ROTTYPE_DOWN:		// ��
				m_move.z = -SCROOL_SPEED_02;
				break;
			case ROTTYPE_LEFT:		// ��
				m_move.x = -SCROOL_SPEED_02;
				break;
			case ROTTYPE_RIGHT:		// �E
				m_move.x = SCROOL_SPEED_02;
				break;
			}

			m_fScrollMove += SCROOL_SPEED_02;
			if (m_fScrollMove > (SCROOL_MOVEGRID_02 * 100))
			{ // �w�肳�ꂽ�X�N���[�����ړ������ꍇ
				bMoveScroll = true;		// �X�N���[��������Ԃɂ���
			}
		}
		break;

	default:
		assert(false);
		break;
	}

	if (bMoveScroll)
	{ // �X�N���[�����I������ꍇ
		D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();

		MapPos.x = useful::RoundUp2(MapPos.x);
		MapPos.z = useful::RoundUp2(MapPos.z);

		// �}�b�v�̈ʒu�ݒ�
		CMapSystem::GetInstance()->SetMapPos(MapPos);

		m_bScrollOK = true;		// �X�N���[������������Ԃɂ���
		m_fScrollMove = 0.0f;	// �X�N���[���ړ��ʏ�����
		m_move = INITVECTOR3;	// �ړ��ʏ�����
	}

	// �}�b�v�𓮂���
	GritScroll();
}

//====================================================================
//�ړ���������(�X���߂�)
//====================================================================
void CMapMove::BackSlope(void)
{
	// ���[�J���ϐ��錾
	CObjmeshField* pMapField = CObjmeshField::GetListTop(); // �}�b�v�̏�
	D3DXVECTOR3 MapRot = pMapField->GetRot(); // �}�b�v�̌X��
	bool bReset = false; // �X���̃��Z�b�g�����������t���O
	float fSlopeRate = 0.0f; // �X���̑��x

	// �X�N���[���^�C�v�ɂ���Ďg�p����萔��ݒ�
	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:
		fSlopeRate = SLOPE_SPEED01;
		break;

	case CMapMove::SCROLL_TYPE_RETRO:
		fSlopeRate = SLOPE_SPEED02;
		break;

	default: // �X�N���[����Ԃ��ݒ肳��Ă��Ȃ��ꍇ
		assert(false);
		break;
	}

	// x�����ɌX���Ă���ꍇ
	if (MapRot.x > 0.0f)
	{
		MapRot.x -= D3DX_PI * fSlopeRate;

		// �߂�߂��̕␳
		if (MapRot.x <= 0.0f)
		{
			bReset = true;
		}
	}

	// x�����ɌX���Ă���ꍇ
	if (MapRot.x < 0.0f)
	{
		MapRot.x += D3DX_PI * fSlopeRate;
		
		// �߂�߂��̕␳
		if (MapRot.x >= 0.0f)
		{
			bReset = true;
		}
	}
	
	// z�����ɌX���Ă���ꍇ
	if (MapRot.z > 0.0f)
	{
		MapRot.z -= D3DX_PI * fSlopeRate;

		// �߂�߂��̕␳
		if (MapRot.z <= 0.0f)
		{
			bReset = true;
		}
	}

	// z�����ɌX���Ă���ꍇ
	if (MapRot.z < 0.0f)
	{
		MapRot.z += D3DX_PI * fSlopeRate;

		// �߂�߂��̕␳
		if (MapRot.z >= 0.0f)
		{
			bReset = true;
		}
	}

	// �X���̓������I������ꍇ
	if (bReset)
	{
		//�J�����̐U�����I�t�ɂ���
		CManager::GetInstance()->GetCamera(0)->SetBib(false);

		// �J�E���^�[�����Z�b�g���� TODO : �}�W�b�N�i���o�[
		m_nStateCount = 60;

		// �X����0�ɂ���
		MapRot.x = MapRot.z = 0.0f;
	}

	// �X����ݒ�
	pMapField->SetRot(MapRot);
}

//====================================================================
//�ړ���������(�X����)
//====================================================================
void CMapMove::Slope()
{
	CObjmeshField* pMapField = CObjmeshField::GetListTop();
	if (pMapField == nullptr) { assert(false); }

	D3DXVECTOR3 MapRot = pMapField->GetRot();

	float fSlopeRate = 0.0f; // �X�N���[���̑��x

	// �X�N���[���^�C�v�ɂ���Ďg�p����萔��ݒ�
	switch (m_ScrollType)
	{
	case CMapMove::SCROLL_TYPE_NORMAL:		// �m�[�}��
		fSlopeRate = SLOPE_SPEED01;
		break;

	case CMapMove::SCROLL_TYPE_RETRO:		// ���g��
		fSlopeRate = SLOPE_SPEED02;
		break;

	default: // �X�N���[����Ԃ��ݒ肳��Ă��Ȃ��ꍇ
		assert(false);
		break;
	}

	// ��]������
	switch (m_RotType)
	{
	case ROTTYPE_UP:		// ��

		MapRot.x += D3DX_PI * fSlopeRate;

		if (MapRot.x > STAGE_ROT_LIMIT)
		{ // �p�x���ڕW�𒴂�����
			MapRot.x = STAGE_ROT_LIMIT;
			CManager::GetInstance()->GetCamera(0)->SetBib(false);	//�J�����̐U�����I�t�ɂ���
		}
		break;

	case ROTTYPE_DOWN:		// ��

		MapRot.x -= D3DX_PI * fSlopeRate;

		if (MapRot.x < -STAGE_ROT_LIMIT)
		{ // �p�x���ڕW�𒴂�����
			MapRot.x = -STAGE_ROT_LIMIT;
			CManager::GetInstance()->GetCamera(0)->SetBib(false);	//�J�����̐U�����I�t�ɂ���
		}
		break;

	case ROTTYPE_LEFT:		// ��

		MapRot.z += D3DX_PI * fSlopeRate;

		if (MapRot.z > STAGE_ROT_LIMIT)
		{ // �p�x���ڕW�𒴂�����
			MapRot.z = STAGE_ROT_LIMIT;
			CManager::GetInstance()->GetCamera(0)->SetBib(false);	//�J�����̐U�����I�t�ɂ���
		}
		break;

	case ROTTYPE_RIGHT:		// �E

		MapRot.z -= D3DX_PI * fSlopeRate;

		if (MapRot.z < -STAGE_ROT_LIMIT)
		{ // �p�x���ڕW�𒴂�����
			MapRot.z = -STAGE_ROT_LIMIT;
			CManager::GetInstance()->GetCamera(0)->SetBib(false);	//�J�����̐U�����I�t�ɂ���
		}
		break;
	}

	m_OldRotType = m_RotType;		// �O��̌��������

	pMapField->SetRot(MapRot);		// �p�x�����
}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CMapMove::CollisionOut()
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CEnemy::GetList() == nullptr) { return; }
	std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CEnemy* pEnemy : list)
	{
		CMapSystem::GRID EnemyGrid = pEnemy->GetGrid();
		D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
		D3DXVECTOR3 EnemyMove = pEnemy->GetMove();
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//��
		m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//��
		m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//�E
		m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//��

		if (EnemyGrid.x == -1)
		{
			if (EnemyPos.x > 0.0f)
			{
				m_MinGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x - MapSize.x - GritSize);	//��
				EnemyPos = CMapSystem::GRID(m_MinGrid.x, pEnemy->GetGrid().z).ToWorld();
			}
			else
			{
				m_MaxGrid.x = CMapSystem::GetInstance()->CalcGridX(m_DevilPos.x + MapSize.x - GritSize);	//�E
				EnemyPos = CMapSystem::GRID(m_MaxGrid.x, pEnemy->GetGrid().z).ToWorld();
			}
		}
		else if (EnemyGrid.z == -1)
		{
			if (EnemyPos.z < 0.0f)
			{
				m_MinGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z + MapSize.z + GritSize);	//��
				EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MinGrid.z).ToWorld();
			}
			else
			{
				m_MaxGrid.z = CMapSystem::GetInstance()->CalcGridZ(m_DevilPos.z - MapSize.z + GritSize);	//��
				EnemyPos = CMapSystem::GRID(pEnemy->GetGrid().x, m_MaxGrid.z).ToWorld();
			}
		}

		pEnemy->SetPos(EnemyPos);
	}
}

//==========================================
// ���点�鏈��
//==========================================
void CMapMove::PlaySound()
{
	// ���[�J���ϐ�
	CSound::SOUND_LABEL SoundLabel;

	// SE�ݒ�
	switch (m_RotType)
	{
	case ROTTYPE_UP:		// ��
		SoundLabel = CSound::SOUND_LABEL_SE_SIGN_UP;
		break;

	case ROTTYPE_DOWN:		// ��
		SoundLabel = CSound::SOUND_LABEL_SE_SIGN_DOWN;
		break;

	case ROTTYPE_LEFT:		// ��
		SoundLabel = CSound::SOUND_LABEL_SE_SIGN_LEFT;
		break;

	case ROTTYPE_RIGHT:		// �E
		SoundLabel = CSound::SOUND_LABEL_SE_SIGN_RIGHT;
		break;
	}

	// �T�E���h�Đ�
	CManager::GetInstance()->GetSound()->PlaySound(SoundLabel);
}


//==========================================
//  �ق点�鏈��
//==========================================
void CMapMove::StopSound()
{
	// �T�E���h�̎擾
	CSound* sound = CManager::GetInstance()->GetSound();

	// �T�E���h�̒�~
	sound->Stop(CSound::SOUND_LABEL_SE_SIGN_UP);
	sound->Stop(CSound::SOUND_LABEL_SE_SIGN_DOWN);
	sound->Stop(CSound::SOUND_LABEL_SE_SIGN_RIGHT);
	sound->Stop(CSound::SOUND_LABEL_SE_SIGN_LEFT);
}

//====================================================================
// �X�N���[���̂���C��
//====================================================================
void CMapMove::ScrollReset()
{

}

//====================================================================
// �O���b�g�̃X�N���[��
//====================================================================
void CMapMove::GritScroll()
{
	D3DXVECTOR3 InitPos = CMapSystem::GetInstance()->GetInitPos();
	D3DXVECTOR3 MapPos = CMapSystem::GetInstance()->GetMapPos();
	float MapGrit = CMapSystem::GetInstance()->GetGritSize();
	MapPos += m_move;
	m_MapDifference = -(InitPos - MapPos);
	D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();

	if ((InitPos.x - MapPos.x) > 0.0f)
	{// ���͈�
		MapPos.x = InitPos.x + (MapSize.x * 2.0f) + MapGrit + m_move.x;
	}
	if ((InitPos.x - MapPos.x + MapGrit) < (-MapSize.x * 2.0f))
	{// �E�͈�
		MapPos.x = InitPos.x + m_move.x;
	}

	if ((InitPos.z - MapPos.z) < 0.0f)
	{// ��͈�
		MapPos.z = InitPos.z + (-MapSize.z * 2.0f) - MapGrit + m_move.z;
	}
	if ((InitPos.z - MapPos.z - MapGrit) > (MapSize.z * 2.0f))
	{// ���͈�
		MapPos.z = InitPos.z + m_move.z;
	}

	CMapSystem::GetInstance()->SetMapPos(MapPos);
}

//====================================================================
//�X�����̈ړ��ʕϓ�
//====================================================================
float CMapMove::MoveSlopeX(float Move, SPEED& Speed)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = m_DevilRot;

	if (Move > 0.0f)
	{// �E���Ɉړ�
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z));

		if (DevilRot.z < 0.0f)
		{// ���ɌX�������̉���
			fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z)) * SLOPEUP_MAG;
		}
		else if (DevilRot.z > 0.0f)
		{// �E�ɌX�������̌���
			fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z)) * SLOPEDOWN_MAG;
		}
	}
	else if (Move < 0.0f)
	{// �����Ɉړ�
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z));

		if (DevilRot.z > 0.0f)
		{// �E�ɌX�������̉���
			fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.z)) * SLOPEUP_MAG;
		}
		else if (DevilRot.z < 0.0f)
		{// ���ɌX�������̌���
			fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.z)) * SLOPEDOWN_MAG;
		}
	}

	if (fSlopeMove > 1.0f)
	{
		Speed = SPEED_UP;
	}
	else if (fSlopeMove < 1.0f)
	{
		Speed = SPEED_DOWN;
	}
	else
	{
		Speed = SPEED_NONE;
	}

	return fSlopeMove;
}

//====================================================================
//�X�����̈ړ��ʕϓ�
//====================================================================
float CMapMove::MoveSlopeZ(float Move, SPEED& Speed)
{
	float fSlopeMove = 1.0f;

	D3DXVECTOR3 DevilRot = m_DevilRot;

	if (Move > 0.0f)
	{// �����Ɉړ�
		fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x));

		if (DevilRot.x < 0.0f)
		{// ��O�ɌX�������̌���
			fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x)) * SLOPEDOWN_MAG;
		}
		else if (DevilRot.x > 0.0f)
		{// ���ɌX�������̉���
			fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x)) * SLOPEUP_MAG;
		}
	}
	else if (Move < 0.0f)
	{// ��O���Ɉړ�
		fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x));

		if (DevilRot.x > 0.0f)
		{// ���ɌX�������̌���
			fSlopeMove = (D3DX_PI / (D3DX_PI + DevilRot.x)) * SLOPEDOWN_MAG;
		}
		else if (DevilRot.x < 0.0f)
		{// ��O�ɌX�������̉���
			fSlopeMove = (D3DX_PI / (D3DX_PI - DevilRot.x)) * SLOPEUP_MAG;
		}
	}

	if (fSlopeMove > 1.0f)
	{
		Speed = SPEED_UP;
	}
	else if (fSlopeMove < 1.0f)
	{
		Speed = SPEED_DOWN;
	}
	else
	{
		Speed = SPEED_NONE;
	}

	return fSlopeMove;
}