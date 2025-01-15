//============================================
//
//	�X�����u�̏��� [SlopeDevice.cpp]
//	Author:sakai minato
//
//============================================
#include "SlopeDevice.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "game.h"

#include "objectcharacter.h"
#include "model.h"
#include "motion.h"

#include "MapMove.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 ASCENT_ADD = D3DXVECTOR3(0.0f, 30.0f, 0.0f);		// �㏸��
	const D3DXVECTOR3 DESCENT_DEST = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// ���~��

	const D3DXVECTOR3 MAX_POS_HEIGHT = D3DXVECTOR3(0.0f, 380.0f, 0.0f);		// �c�X���̍ő�㏸�l
	const D3DXVECTOR3 MAX_POS_WIDTH = D3DXVECTOR3(0.0f,550.0f,0.0f);		// ���X���̍ő�㏬�l

	const D3DXVECTOR3 MIN_POS_HEIGHT = D3DXVECTOR3(0.0f, -250.0f, 0.0f);	// �c�X���̍ŏ��㏸�l
	const D3DXVECTOR3 MIN_POS_WIDTH = D3DXVECTOR3(0.0f, -300.0f, 0.0f);		// ���X���̍ŏ��㏬�l

	const D3DXVECTOR3 MOVE_SPEED_HEIGHT = D3DXVECTOR3(0.0f, 1.6f, 0.0f);	// �c�X���̏��~�ړ���
	const D3DXVECTOR3 MOVE_SPEED_WIDTH = D3DXVECTOR3(0.0f, 2.25f, 0.0f);	// ���X���̏��~�ړ���

	const D3DXVECTOR3 MOVE_SPEED_HEIGHT_RETRO = D3DXVECTOR3(0.0f, 28.0f, 0.0f);	// �c�X���̏��~�ړ��ʁi���g����ԁj
	const D3DXVECTOR3 MOVE_SPEED_WIDTH_RETRO = D3DXVECTOR3(0.0f, 34.5f, 0.0f);	// ���X���̏��~�ړ��ʁi���g����ԁj

	const D3DXVECTOR3 LIVER_ROT_MOVE = D3DXVECTOR3(0.01f, 0.0f, 0.0f);		// ���o�[�̈ړ���
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CSlopeDevice::CSlopeDevice(int nPriority) : CObjectCharacter(nPriority)
{
	m_posTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posTargetDef = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_rotleverDef = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bleverMove = false;

	m_State = STATE(0);

	m_LocateWorldType = LOCATE_WORLD_TYPE(0);

	m_bUseRetroMove = false;
		
	m_pObjectCharacter = nullptr;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSlopeDevice::~CSlopeDevice()
{

}

//====================================================================
//��������
//====================================================================
CSlopeDevice* CSlopeDevice::Create(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy)
{
	// �I�u�W�F�N�g�̐�������
	CSlopeDevice* pInstance = new CSlopeDevice();

	// �I�u�W�F�N�g�̏���������
	if (FAILED(pInstance->Init()))
	{// ���������������s�����ꍇ
		return nullptr;
	}

	// ���f���֘A����������
	if (FAILED(pInstance->InitModel(pModelNameSlopeDevice, pModelNameEnemy)))
	{// ���������������s�����ꍇ
		return nullptr;
	}

	return pInstance;
}

//====================================================================
//����������
//====================================================================
HRESULT CSlopeDevice::Init(void)
{
	SetType(CObject::TYPE_DEVILHOLE);

	// �e��s�g�p�ɐݒ�
	SetShadow(false);

	// �L�����N�^�[�N���X�̏������i�p���j
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	if (m_pObjectCharacter == nullptr)
	{
		// �L�����N�^�[�i���_�}���p�j�̐�������
		m_pObjectCharacter = CObjectCharacter::Create(false);
	}

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CSlopeDevice>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CSlopeDevice::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �L�����N�^�[�N���X�̏I���i�p���j
	CObjectCharacter::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CSlopeDevice::Update(void)
{
	// �l���擾
	D3DXVECTOR3 posMy = GetPos();			// �ʒu
	D3DXVECTOR3 posOldMy = GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 rotMy = GetRot();			// ����
	D3DXVECTOR3 sizeMy = GetSize();			// �傫��

	// �ߋ��̈ʒu���L�^
	posOldMy = posMy;

	//��ԊǗ�
	StateManager();

	// �L�����N�^�[�N���X�̍X�V�i�p���j
	CObjectCharacter::Update();

	// �l�X�V
	SetPos(posMy);			// �ʒu
	SetPosOld(posOldMy);	// �O��̈ʒu
	SetRot(rotMy);			// ����
	SetSize(sizeMy);		// �傫��
}

//====================================================================
//�`�揈��
//====================================================================
void CSlopeDevice::Draw(void)
{
	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();
}

//====================================================================
// ��������i�X���j�̏�Ԑݒ菈��
//====================================================================
void CSlopeDevice::SetStateArrow(CScrollArrow::Arrow stateArrow)
{
	switch (stateArrow)
	{
	case CScrollArrow::STATE_UP:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			SetState(STATE_ASCENT);
		}
		else
		{
			SetState(STATE_DESCENT);
		}

		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT)
		{
			SetState(STATE_ASCENT);
		}
		else
		{
			SetState(STATE_DESCENT);
		}

		break;

	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT)
		{
			SetState(STATE_ASCENT);
		}
		else
		{
			SetState(STATE_DESCENT);
		}

		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			SetState(STATE_ASCENT);
		}
		else
		{
			SetState(STATE_DESCENT);
		}

		break;
	}

	// �}�b�v�ړ��擾����
	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();

	// ��Ԏ擾�p�ϐ�
	CMapMove::SCROLL_TYPE MoveType = CMapMove::SCROLL_TYPE_NORMAL;

	if (pMapMove != nullptr)
	{
		// �}�b�v�ړ����[�h�擾����
		MoveType = pMapMove->GetScrollType();
	}

	if (stateArrow == CScrollArrow::STATE_UP ||
		stateArrow == CScrollArrow::STATE_DOWN)
	{
		if (MoveType == CMapMove::SCROLL_TYPE_NORMAL)
		{
			// ���~�ړ��ʂ�ݒ�
			m_move = MOVE_SPEED_HEIGHT;
		}
		else if (MoveType == CMapMove::SCROLL_TYPE_RETRO)
		{
			// ���~�ړ��ʂ�ݒ�i���g���j
			m_move = MOVE_SPEED_HEIGHT_RETRO;
		}

		if (m_State == STATE_ASCENT)
		{
			// �ő�㏸�l����
			m_posTarget = MAX_POS_HEIGHT;
		}
		else if (m_State == STATE_DESCENT)
		{
			// �ŏ����~�l����
			m_posTarget = MIN_POS_HEIGHT;
		}
	}
	else if (stateArrow == CScrollArrow::STATE_LEFT ||
			 stateArrow == CScrollArrow::STATE_RIGHT)
	{
		if (MoveType == CMapMove::SCROLL_TYPE_NORMAL)
		{
			// ���~�ړ��ʂ�ݒ�
			m_move = MOVE_SPEED_WIDTH;
		}
		else if (MoveType == CMapMove::SCROLL_TYPE_RETRO)
		{
			// ���~�ړ��ʂ�ݒ�i���g���j
			m_move = MOVE_SPEED_WIDTH_RETRO;
		}

		if (m_State == STATE_ASCENT)
		{
			// �ő�㏸�l����
			m_posTarget = MAX_POS_WIDTH;
		}
		else if (m_State == STATE_DESCENT)
		{
			// �ŏ����~�l����
			m_posTarget = MIN_POS_WIDTH;
		}
	}
}

//====================================================================
// ��������i�X���߂�j�̏�Ԑݒ菈��
//====================================================================
void CSlopeDevice::SetStateArrowBack(CScrollArrow::Arrow stateArrow)
{
	switch (stateArrow)
	{
	case CScrollArrow::STATE_UP:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			SetState(STATE_DESCENT);
		}
		else
		{
			SetState(STATE_ASCENT);
		}

		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT)
		{
			SetState(STATE_DESCENT);
		}
		else
		{
			SetState(STATE_ASCENT);
		}

		break;
	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT)
		{
			SetState(STATE_DESCENT);
		}
		else
		{
			SetState(STATE_ASCENT);
		}

		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			SetState(STATE_DESCENT);
		}
		else
		{
			SetState(STATE_ASCENT);
		}

		break;
	}

	if (stateArrow == CScrollArrow::STATE_UP ||
		stateArrow == CScrollArrow::STATE_DOWN)
	{
		// �ړI�ʒu��ʏ�ʒu�ɕύX
		m_posTarget = m_posTargetDef;
	}
	else if (stateArrow == CScrollArrow::STATE_LEFT ||
			 stateArrow == CScrollArrow::STATE_RIGHT)
	{
		// �ړI�ʒu��ʏ�ʒu�ɕύX
		m_posTarget = m_posTargetDef;
	}
}

//====================================================================
// �S�l���Z�b�g����
//====================================================================
void CSlopeDevice::ReSet(void)
{
	// ���~���ʂ̃��f���̎擾
	CModel* pModelElevatingParts = GetModel(SETUP_TYPE_ELEVATING_PART);
	CModel* pModelLever = GetModel(SETUP_TYPE_LIVER);

	if (pModelElevatingParts == nullptr && pModelLever == nullptr)
	{
		return;
	}

	// ���f���̈ʒu���擾
	D3DXVECTOR3 pos = pModelElevatingParts->GetStartPos();
	D3DXVECTOR3 rot = pModelLever->GetStartRot();

	// ���~�ʂ����Z
	pos = m_posTargetDef;
	rot = m_rotleverDef;

	// ���f���̈ʒu�X�V
	pModelElevatingParts->SetStartPos(pos);
	pModelLever->SetStartRot(rot);

	// �ʏ��ԂɕύX
	SetState(STATE_NORMAL);
}

//====================================================================
// �S�\���ݒ菈��
//====================================================================
void CSlopeDevice::SetAppearAll(bool bAppear)
{
	// �\���ݒ�
	SetAppear(bAppear);

	// �L�����N�^�[�̕\���ݒ�
	if (m_pObjectCharacter != nullptr)
	{
		m_pObjectCharacter->SetAppear(bAppear);
	}
}

//====================================================================
// ���f���֘A�̏���������
//====================================================================
HRESULT CSlopeDevice::InitModel(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameSlopeDevice, 0);

	// ���o�[�̏����������擾
	m_rotleverDef = GetModel(SETUP_TYPE_LIVER)->GetStartRot();

	// �f�t�H���g�ړI�ʒu���擾�E�ݒ�
	m_posTargetDef = GetModel(SETUP_TYPE_ELEVATING_PART)->GetStartPos();
	m_posTarget = m_posTargetDef;

	if (m_pObjectCharacter != nullptr)
	{
		// �L�����N�^�[�e�L�X�g�ǂݍ��ݏ����i���_�}���j
		m_pObjectCharacter->SetTxtCharacter(pModelNameEnemy, 0);

		// ���_�}���̐e��y��ɕύX
		m_pObjectCharacter->GetModel(0)->SetParent(GetModel(SETUP_TYPE_LIVER));
		D3DXVECTOR3 rotlever = GetModel(SETUP_TYPE_LIVER)->GetStartRot();

		// ���_�}���̈ʒu���擾
		D3DXVECTOR3 pos = m_pObjectCharacter->GetModel(0)->GetStartPos();
		D3DXVECTOR3 rot = m_pObjectCharacter->GetModel(0)->GetStartRot();
		D3DXVECTOR3 posAdd = D3DXVECTOR3(0.0f, 10.0f, 150.0f);
		D3DXVECTOR3 rotAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f) - rotlever;

		// ���_�}�������
		m_pObjectCharacter->GetModel(0)->SetStartPos(pos + posAdd);
		m_pObjectCharacter->GetModel(0)->SetStartRot(rot + rotAdd);
	}

	return S_OK;
}

//====================================================================
// �ʏ��Ԃ̏�ԊǗ�
//====================================================================
void CSlopeDevice::StateManager(void)
{
	// �}�b�v�ړ��擾����
	CMapMove* pMapMove = CMapSystem::GetInstance()->GetMove();

	// ��Ԏ擾�p�ϐ�
	CMapMove::SCROLL_TYPE MoveType = CMapMove::SCROLL_TYPE_NORMAL;
	CMapMove::MOVE MoveState = CMapMove::MOVE_WAIT;	

	if (pMapMove != nullptr)
	{
		// �}�b�v�ړ����[�h�擾����
		MoveType = pMapMove->GetScrollType();

		// �}�b�v��Ԏ擾
		MoveState = pMapMove->GetState();
	}

	if (MoveType == CMapMove::SCROLL_TYPE_RETRO &&
		m_bUseRetroMove == false)
	{
		return;
	}
	
	// ��Ԕ���
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

		// �㏸���
	case STATE_ASCENT:

		// �ړ���Ԕ���i�� or ���j
		if (MoveState == CMapMove::MOVE_SLOPE_UP ||
			MoveState == CMapMove::MOVE_SLOPE_DOWN)
		{
			// �㏸����
			Ascent(SETUP_TYPE_ELEVATING_PART);
			
			// ���o�[���쏈��
			ActiveLever(SETUP_TYPE_LIVER);
		}
		// �ړ���Ԕ���i�E or ���j
		else if (MoveState == CMapMove::MOVE_SLOPE_RIGHT ||
				 MoveState == CMapMove::MOVE_SLOPE_LEFT)
		{
			// �㏸����
			Ascent(SETUP_TYPE_ELEVATING_PART);

			// ���o�[���쏈��
			ActiveLever(SETUP_TYPE_LIVER);
		}

		break;
	case STATE_DESCENT:

		// �ړ���Ԕ���i�� or ���j
		if (MoveState == CMapMove::MOVE_SLOPE_UP ||
			MoveState == CMapMove::MOVE_SLOPE_DOWN)
		{
			// ���~����
			Descent(SETUP_TYPE_ELEVATING_PART);

			// ���o�[���쏈��
			ActiveLever(SETUP_TYPE_LIVER);
		}
		// �ړ���Ԕ���i�E or ���j
		else if (MoveState == CMapMove::MOVE_SLOPE_RIGHT ||
				 MoveState == CMapMove::MOVE_SLOPE_LEFT)
		{
			// ���~����
			Descent(SETUP_TYPE_ELEVATING_PART);

			// ���o�[���쏈��
			ActiveLever(SETUP_TYPE_LIVER);
		}

		break;
	}

	if (m_bUseRetroMove == true)
	{
		m_bUseRetroMove = false;
	}
}

//====================================================================
// �㏸����
//====================================================================
void CSlopeDevice::Ascent(int nNldxModel)
{
	// ���f���̎擾
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// ���f���̈ʒu���擾
	D3DXVECTOR3 pos = pModel->GetStartPos();

	// �㏸�ő�l����
	if (pos.y < m_posTarget.y)
	{
		// �㏸�ʂ����Z
		pos += m_move;
	}
	else
	{
		// �ʏ��ԂɕύX
		SetState(STATE_NORMAL);
	}

	// ���f���̈ʒu�X�V
	pModel->SetStartPos(pos);
}

//====================================================================
// ���~����
//====================================================================
void CSlopeDevice::Descent(int nNldxModel)
{
	// ���f���̎擾
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// ���f���̈ʒu���擾
	D3DXVECTOR3 pos = pModel->GetStartPos();

	// ���~�ŏ��l����
	if (pos.y > m_posTarget.y)
	{
		// ���~�ʂ����Z
		pos -= m_move;
	}
	else
	{
		// �ʏ��ԂɕύX
		SetState(STATE_NORMAL);
	}

	// ���f���̈ʒu�X�V
	pModel->SetStartPos(pos);
}

//====================================================================
// ���o�[�����ԏ���
//====================================================================
void CSlopeDevice::ActiveLever(int nNldxModel)
{
	// ���f���̎擾
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// ���f���̈ʒu���擾
	D3DXVECTOR3 rot = pModel->GetStartRot();

	if (m_bleverMove)
	{
		// ���~�ʂ����Z
		rot -= LIVER_ROT_MOVE;

		// ���~�ŏ��l����
		if (rot.x < m_rotleverDef.x)
		{
			m_bleverMove = false;
		}
	}
	else
	{
		// �㏸�ʂ����Z
		rot += LIVER_ROT_MOVE;

		// ���~�ŏ��l����
		if (rot.x > 0.0f)
		{
			m_bleverMove = true;
		}
	}


	// ���f���̈ʒu�X�V
	pModel->SetStartRot(rot);
}

//====================================================================
// ��Ԑݒ菈��
//====================================================================
void CSlopeDevice::SetState(STATE state)
{
	m_State = state;

	switch (m_State)
	{
	case STATE_NORMAL:

		if (m_pObjectCharacter != nullptr)
		{
			// ���[�V�����ݒ菈��
			m_pObjectCharacter->GetMotion()->Set(0, 5);
		}

		break;
	case STATE_ASCENT:

		if (m_pObjectCharacter != nullptr)
		{
			// ���[�V�����ݒ菈��
			m_pObjectCharacter->GetMotion()->Set(1, 5);
		}

		break;

	case STATE_DESCENT:

		if (m_pObjectCharacter != nullptr)
		{
			// ���[�V�����ݒ菈��
			m_pObjectCharacter->GetMotion()->Set(1, 5);
		}

		break;
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::GetList(void)
{
	return m_pList;
}
