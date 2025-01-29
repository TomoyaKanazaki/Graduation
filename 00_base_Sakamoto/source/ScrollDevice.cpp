//============================================
//
//	�}�b�v�ړ����u�̏��� [ScrollDevice.cpp]
//	Author:sakai minato
//
//============================================
#include "ScrollDevice.h"
#include "renderer.h"
#include "manager.h"

#include "objectcharacter.h"
#include "model.h"
#include "motion.h"

#include "objmeshField.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 ROTATE_ADD_TOP = D3DXVECTOR3(0.0f, 0.0f, -0.05f);		// ��
	const D3DXVECTOR3 ROTATE_ADD_BOTTOM = D3DXVECTOR3(0.0f, 0.0f, 0.05f);	// ��
	const D3DXVECTOR3 ROTATE_ADD_LEFT = D3DXVECTOR3(0.0f, 0.0f, 0.05f);		// ��
	const D3DXVECTOR3 ROTATE_ADD_RIGHT = D3DXVECTOR3(0.0f, 0.0f, -0.05f);	// �E

	const D3DXVECTOR3 MEDAMAN_ROT_TOP_NOR = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_BOTTOM_NOR = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_LEFT_NOR = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_RIGHT_NOR = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);

	const D3DXVECTOR3 MEDAMAN_ROT_TOP = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_BOTTOM = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_LEFT = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	const D3DXVECTOR3 MEDAMAN_ROT_RIGHT = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CScrollDevice>* CScrollDevice::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CScrollDevice::CScrollDevice(int nPriority) : CObjectCharacter(nPriority)
{
	m_State = STATE(0);
	m_nStateCount = 0;

	m_LocateWorldType = LOCATE_WORLD_TYPE(0);

	m_rotMove = D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_pObjectCharacter = nullptr;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CScrollDevice::~CScrollDevice()
{

}

//====================================================================
//��������
//====================================================================
CScrollDevice* CScrollDevice::Create(const std::string pModelNameSlopeDevice, const std::string pModelNameEnemy)
{
	// �I�u�W�F�N�g�̐�������
	CScrollDevice* pInstance = new CScrollDevice();

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
HRESULT CScrollDevice::Init(void)
{
	SetType(CObject::TYPE_DEVILHOLE);

	// �e��s�g�p�ɐݒ�
	SetShadow(false);

	// �L�����N�^�[�N���X�̏������i�p���j
	if (FAILED(CObjectCharacter::Init())) { assert(false); }

	// �}�b�v�}�g���b�N�X�ƕR�Â�
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	if (m_pObjectCharacter == nullptr)
	{
		// �L�����N�^�[�i���_�}���p�j�̐�������
		if (FAILED(m_pObjectCharacter = CObjectCharacter::Create(false))) { assert(false); }

		// �}�b�v�}�g���b�N�X�ƕR�Â�
		m_pObjectCharacter->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
	}

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CScrollDevice>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CScrollDevice::Uninit(void)
{
	if (m_pObjectCharacter != nullptr)
	{
		m_pObjectCharacter->Uninit();
		m_pObjectCharacter = nullptr;
	}

	if (m_pList != nullptr)
	{
		// ���X�g���玩�g�̃I�u�W�F�N�g���폜
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // �I�u�W�F�N�g������Ȃ��ꍇ

			// ���X�g�}�l�[�W���[�̔j��
			m_pList->Release(m_pList);
		}
	}
	// �L�����N�^�[�N���X�̏I���i�p���j
	CObjectCharacter::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CScrollDevice::Update(void)
{
	// �l���擾
	D3DXVECTOR3 posMy = GetPos();			// �ʒu
	D3DXVECTOR3 posOldMy = GetPosOld();		// �O��̈ʒu
	D3DXVECTOR3 rotMy = GetRot();			// ����
	D3DXVECTOR3 sizeMy = GetSize();			// �傫��

	// �ߋ��̈ʒu���L�^
	posOldMy = posMy;

	//��ԊǗ�
	StateManager(rotMy);

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
void CScrollDevice::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();

	//�X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//�X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�̐ݒ� => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 103);

	//�X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//�X�e���V���o�b�t�@�̔�r���@ => (�Q�ƒl => �X�e���V���o�b�t�@�̎Q�ƒl)�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//�X�e���V���e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();

	//�X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
// ��Ԑݒ菈��
//====================================================================
void CScrollDevice::SetState(STATE state)
{
	// ��Ԑݒ�
	m_State = state;

	switch (m_State)
	{
	case CScrollDevice::STATE_NORMAL:

		if (m_pObjectCharacter != nullptr)
		{
			// ���[�V�����ݒ菈��
			m_pObjectCharacter->GetMotion()->Set(0, 5);
		}

		// ���_�}���̌����ݒ�
		SetRotMedaman();

		break;
	case CScrollDevice::STATE_ROTATE:

		if (m_pObjectCharacter != nullptr)
		{
			// ���[�V�����ݒ菈��
			m_pObjectCharacter->GetMotion()->Set(1, 5);
		}

		break;
	}
}

//====================================================================
// ��������i�X���j�̏�Ԑݒ菈��
//====================================================================
void CScrollDevice::SetStateArrow(CScrollArrow::Arrow stateArrow)
{
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (stateArrow)
	{
	case CScrollArrow::STATE_UP:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM)
		{
			// ��ړ����
			m_rotMove = ROTATE_ADD_TOP;

			// ����
			SetState(STATE_ROTATE);

			// ���_�}���̌����ݒ�
			SetRotMedaman(stateArrow);
		}

		break;
	case CScrollArrow::STATE_DOWN:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_TOP ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_BOTTOM)
		{
			// ���ړ����
			m_rotMove = ROTATE_ADD_BOTTOM;

			// ����
			SetState(STATE_ROTATE);

			// ���_�}���̌����ݒ�
			SetRotMedaman(stateArrow);
		}

		break;

	case CScrollArrow::STATE_LEFT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_RIGHT)
		{
			// ���ړ��ʑ��
			m_rotMove = ROTATE_ADD_LEFT;

			// ����
			SetState(STATE_ROTATE);

			// ���_�}���̌����ݒ�
			SetRotMedaman(stateArrow);
		}

		break;
	case CScrollArrow::STATE_RIGHT:

		if (m_LocateWorldType == LOCATE_WORLD_TYPE_LEFT ||
			m_LocateWorldType == LOCATE_WORLD_TYPE_RIGHT)
		{
			// �E�ړ��ʑ��
			m_rotMove = ROTATE_ADD_RIGHT;

			// ����
			SetState(STATE_ROTATE);

			// ���_�}���̌����ݒ�
			SetRotMedaman(stateArrow);
		}

		break;
	}
}

//====================================================================
// �S�\���ݒ菈��
//====================================================================
void CScrollDevice::SetAppearAll(bool bAppear)
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
HRESULT CScrollDevice::InitModel(const std::string pModelNameScrollDevice, const std::string pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameScrollDevice);

	if (m_pObjectCharacter != nullptr)
	{
		// �L�����N�^�[�e�L�X�g�ǂݍ��ݏ����i���_�}���j
		m_pObjectCharacter->SetTxtCharacter(pModelNameEnemy);

		// ���_�}���̐e��y��ɕύX
		m_pObjectCharacter->GetModel(0)->SetParent(GetModel(SETUP_TYPE_FOUNDATION));

		// ���_�}���̈ʒu���擾
		D3DXVECTOR3 pos = m_pObjectCharacter->GetModel(0)->GetStartPos();
		D3DXVECTOR3 rot = m_pObjectCharacter->GetModel(0)->GetStartRot();
		D3DXVECTOR3 posAdd = D3DXVECTOR3(0.0f, 75.0f, 0.0f);

		// ���_�}�������
		m_pObjectCharacter->GetModel(0)->SetStartPos(pos + posAdd);
	}

	return S_OK;
}

//====================================================================
//��ԊǗ�
//====================================================================
void CScrollDevice::StateManager(D3DXVECTOR3& rotMy)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ROTATE:

		// ���[���[�Ɖ񂵎ԉ�]����
		Rotate(rotMy,SETUP_TYPE_ROLLRE);
		Rotate(rotMy,SETUP_TYPE_MAWASIGURMA);
		break;
	}
}

//====================================================================
// ��]����
//====================================================================
void CScrollDevice::Rotate(D3DXVECTOR3& rotMy,int nNldxModel)
{
	// ���f���̎擾
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// ���f���̉�]�����擾
	D3DXVECTOR3 rot = pModel->GetStartRot();
	
	// ��]�ʉ��Z
	rot += m_rotMove;

	// �p�x�̐��K��
	useful::NormalizeAngle(&rot);

	// ���f���̉�]�ݒ�
	pModel->SetStartRot(rot);
}

//====================================================================
// ��Ԑݒ菈��
//====================================================================
void CScrollDevice::SetRotMedaman(CScrollArrow::Arrow stateArrow)
{
	if (m_pObjectCharacter == nullptr)
	{
		return;
	}

	switch (m_State)
	{
	case CScrollDevice::STATE_NORMAL:

		switch (m_LocateWorldType)
		{
		case CScrollDevice::LOCATE_WORLD_TYPE_TOP:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_TOP_NOR);

			break;
		case CScrollDevice::LOCATE_WORLD_TYPE_BOTTOM:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_BOTTOM_NOR);

			break;
		case CScrollDevice::LOCATE_WORLD_TYPE_LEFT:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_LEFT_NOR);

			break;
		case CScrollDevice::LOCATE_WORLD_TYPE_RIGHT:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_RIGHT_NOR);

			break;
		}

		break;

	case CScrollDevice::STATE_ROTATE:

		switch (stateArrow)
		{
		case CScrollArrow::STATE_UP:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_TOP);

			break;
		case CScrollArrow::STATE_DOWN:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_BOTTOM);

			break;
		case CScrollArrow::STATE_LEFT:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_LEFT);

			break;
		case CScrollArrow::STATE_RIGHT:

			m_pObjectCharacter->GetModel(0)->SetStartRot(MEDAMAN_ROT_RIGHT);

			break;
		}

		break;
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CScrollDevice>* CScrollDevice::GetList(void)
{
	return m_pList;
}
