//============================================
//
//	�}�b�v�ړ����u�̏��� [ScrollDevice.cpp]
//	Author:sakai minato
//
//============================================
#include "ScrollDevice.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "game.h"

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

	//���[�h���Ƃɏ����l��ݒ�o����
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		break;

	case CScene::MODE_RESULT:
		break;
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
// ���f���֘A�̏���������
//====================================================================
HRESULT CScrollDevice::InitModel(const char* pModelNameScrollDevice, const char* pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameScrollDevice);

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
