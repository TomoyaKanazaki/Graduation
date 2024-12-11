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

	const D3DXVECTOR3 MAX_POS_HEIGHT = D3DXVECTOR3(0.0f, 500.0f, 0.0f);		// �c�X���̍ő�㏸�l
	const D3DXVECTOR3 MAX_POS_WIDTH = D3DXVECTOR3(0.0f,500.0f,0.0f);		// ���X���̍ő�㏬�l

	const D3DXVECTOR3 MIN_POS_HEIGHT = D3DXVECTOR3(0.0f, -200.0f, 0.0f);	// �c�X���̍ŏ��㏸�l
	const D3DXVECTOR3 MIN_POS_WIDTH = D3DXVECTOR3(0.0f, -200.0f, 0.0f);		// ���X���̍ŏ��㏬�l

	const D3DXVECTOR3 MOVE_SPEED_HEIGHT = D3DXVECTOR3(0.0f, 1.75f, 0.0f);	// �c�X���̏��~�ړ���
	const D3DXVECTOR3 MOVE_SPEED_WIDTH = D3DXVECTOR3(0.0f, 2.25f, 0.0f);	// ���X���̏��~�ړ���
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

	m_State = STATE(0);
	m_nStateCount = 0;

	m_LocateWorldType = LOCATE_WORLD_TYPE(0);

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
CSlopeDevice* CSlopeDevice::Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
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
			m_State = STATE_ASCENT;
		}
		else
		{
			m_State = STATE_DESCENT;
		}

		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT)
		{
			m_State = STATE_ASCENT;
		}
		else
		{
			m_State = STATE_DESCENT;
		}

		break;

	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT)
		{
			m_State = STATE_ASCENT;
		}
		else
		{
			m_State = STATE_DESCENT;
		}

		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			m_State = STATE_ASCENT;
		}
		else
		{
			m_State = STATE_DESCENT;
		}

		break;
	}

	if (stateArrow == CScrollArrow::STATE_UP ||
		stateArrow == CScrollArrow::STATE_DOWN)
	{
		// ���~�ړ��ʂ�ݒ�
		m_move = MOVE_SPEED_HEIGHT;

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
		// ���~�ړ��ʂ�ݒ�
		m_move = MOVE_SPEED_WIDTH;

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
			m_State = STATE_DESCENT;
		}
		else
		{
			m_State = STATE_ASCENT;
		}

		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT)
		{
			m_State = STATE_DESCENT;
		}
		else
		{
			m_State = STATE_ASCENT;
		}

		break;
	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_LEFT)
		{
			m_State = STATE_DESCENT;
		}
		else
		{
			m_State = STATE_ASCENT;
		}

		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP_RIGHT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM_RIGHT)
		{
			m_State = STATE_DESCENT;
		}
		else
		{
			m_State = STATE_ASCENT;
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
// ���f���֘A�̏���������
//====================================================================
HRESULT CSlopeDevice::InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameSlopeDevice, 0);

	// �f�t�H���g�ړI�ʒu���擾�E�ݒ�
	m_posTargetDef = GetModel(SETUP_TYPE_ELEVATING_PART)->GetStartPos();
	m_posTarget = m_posTargetDef;

	if (m_pObjectCharacter != nullptr)
	{
		// �L�����N�^�[�e�L�X�g�ǂݍ��ݏ����i���_�}���j
		m_pObjectCharacter->SetTxtCharacter(pModelNameEnemy, 0);

		// ���_�}���̐e��y��ɕύX
		m_pObjectCharacter->GetModel(0)->SetParent(GetModel(SETUP_TYPE_JACK));

		// ���_�}���̈ʒu���擾
		D3DXVECTOR3 pos = m_pObjectCharacter->GetModel(0)->GetStartPos();
		D3DXVECTOR3 rot = m_pObjectCharacter->GetModel(0)->GetStartRot();
		D3DXVECTOR3 posAdd = D3DXVECTOR3(0.0f, 0.0f, 400.0f);
		D3DXVECTOR3 rotAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ���_�}�������
		m_pObjectCharacter->GetModel(0)->SetStartPos(pos + posAdd);
		m_pObjectCharacter->GetModel(0)->SetStartRot(rot + rotAdd);
	}

	return S_OK;
}

//====================================================================
//��ԊǗ�
//====================================================================
void CSlopeDevice::StateManager(void)
{
	CMapMove* pMapMove = CMapMove::GetListTop();
	CMapMove::MOVE MoveState = CMapMove::MOVE_WAIT;

	if (pMapMove != nullptr)
	{
		MoveState = pMapMove->GetState();
	}

	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ASCENT:

		if (MoveState == CMapMove::MOVE_SLOPE_UP ||
			MoveState == CMapMove::MOVE_SLOPE_DOWN)
		{
			Ascent(SETUP_TYPE_ELEVATING_PART);
		}
		else if (MoveState == CMapMove::MOVE_SLOPE_RIGHT ||
				 MoveState == CMapMove::MOVE_SLOPE_LEFT)
		{
			Ascent(SETUP_TYPE_ELEVATING_PART);
		}

		break;
	case STATE_DESCENT:

		if (MoveState == CMapMove::MOVE_SLOPE_UP ||
			MoveState == CMapMove::MOVE_SLOPE_DOWN)
		{
			Descent(SETUP_TYPE_ELEVATING_PART);
		}
		else if (MoveState == CMapMove::MOVE_SLOPE_RIGHT ||
				 MoveState == CMapMove::MOVE_SLOPE_LEFT)
		{
			Descent(SETUP_TYPE_ELEVATING_PART);
		}

		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
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

	// ���f���̈ʒu�X�V
	pModel->SetStartPos(pos);
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::GetList(void)
{
	return m_pList;
}
