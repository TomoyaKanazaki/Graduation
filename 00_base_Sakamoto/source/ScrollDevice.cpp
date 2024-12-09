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
	const D3DXVECTOR3 ROTATE_ADD = D3DXVECTOR3(0.0f, 0.0f, 0.05f);
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
CScrollDevice* CScrollDevice::Create(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
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
	// �L�����N�^�[�N���X�̕`��i�p���j
	CObjectCharacter::Draw();
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
// ���f���֘A�̏���������
//====================================================================
HRESULT CScrollDevice::InitModel(const char* pModelNameScrollDevice, const char* pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameScrollDevice);

	if (m_pObjectCharacter != nullptr)
	{
		// �L�����N�^�[�e�L�X�g�ǂݍ��ݏ����i���_�}���j
		m_pObjectCharacter->SetTxtCharacter(pModelNameEnemy);

		// ���_�}���̐e��y��ɕύX
		m_pObjectCharacter->GetModel(0)->SetParent(GetModel(SETUP_TYPE_FOUNDATION));

		// ���_�}���̈ʒu���擾
		D3DXVECTOR3 pos = m_pObjectCharacter->GetModel(2)->GetStartPos();
		D3DXVECTOR3 rot = m_pObjectCharacter->GetModel(2)->GetStartRot();
		D3DXVECTOR3 posAdd = D3DXVECTOR3(-50.0f, 375.0f, 500.0f);
		D3DXVECTOR3 rotAdd = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

		// ���_�}�������
		m_pObjectCharacter->GetModel(0)->SetStartPos(pos + posAdd);
		m_pObjectCharacter->GetModel(0)->SetStartRot(rot + rotAdd);
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
		Rotate(rotMy,SETUP_TYPE_ROLLRE, ROTATE_ADD);
		Rotate(rotMy,SETUP_TYPE_MAWASIGURMA, ROTATE_ADD);
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
// ��]����
//====================================================================
void CScrollDevice::Rotate(D3DXVECTOR3& rotMy,int nNldxModel,D3DXVECTOR3 rotate)
{
	// ���f���̎擾
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// ���f���̉�]�����擾
	D3DXVECTOR3 rot = pModel->GetStartRot();
	
	// �{�̂̌�������
	if (rotMy.y == 0.0f)
	{
		// ��]�ʉ��Z
		rot += rotate;
	}
	else if (rotMy.y == D3DX_PI)
	{
		// ��]�ʌ��Z
		rot -= rotate;
	}

	// �p�x�̐��K��
	useful::NormalizeAngle(&rot);

	// ���f���̉�]�ݒ�
	pModel->SetStartRot(rot);
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CScrollDevice>* CScrollDevice::GetList(void)
{
	return m_pList;
}
