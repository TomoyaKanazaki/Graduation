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

#include "objmeshField.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 ASCENT_ADD = D3DXVECTOR3(0.0f, 1.5f, 0.0f);	// �㏸��
	const D3DXVECTOR3 DESCENT_DEST = D3DXVECTOR3(0.0f, 1.5f, 0.0f);	// ���~��
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
	m_State = STATE(1);
	m_nStateCount = 0;
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
// ���f���֘A�̏���������
//====================================================================
HRESULT CSlopeDevice::InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
{
	CObjectCharacter::SetTxtCharacter(pModelNameSlopeDevice);

	return S_OK;
}

//====================================================================
//��ԊǗ�
//====================================================================
void CSlopeDevice::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ASCENT:

		Ascent(SETUP_TYPE_ELEVATING_PART, ASCENT_ADD,D3DXVECTOR3(0.0f,200.0f,0.0f));

		break;
	case STATE_DESCENT:

		Descent(SETUP_TYPE_ELEVATING_PART, DESCENT_DEST, D3DXVECTOR3(0.0f, 80.0f, 0.0f));

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
void CSlopeDevice::Ascent(int nNldxModel, D3DXVECTOR3 ascent, D3DXVECTOR3 ascentPosMax)
{
	// ���f���̎擾
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// ���f���̈ʒu���擾
	D3DXVECTOR3 pos = pModel->GetStartPos();

	// �ő�㏸�ʒu����
	if (pos.y <= ascentPosMax.y)
	{
		// �㏸�ʂ����Z
		pos += ascent;
	}
	else
	{
		m_State = STATE(2);
	}

	// ���f���̈ʒu�X�V
	pModel->SetStartPos(pos);
}

//====================================================================
// ���~����
//====================================================================
void CSlopeDevice::Descent(int nNldxModel, D3DXVECTOR3 descent, D3DXVECTOR3 descentPosMin)
{
	// ���f���̎擾
	CModel* pModel = GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// ���f���̈ʒu���擾
	D3DXVECTOR3 pos = pModel->GetStartPos();

	// �ő剺�~�ʒu����
	if (pos.y >= descentPosMin.y)
	{
		// ���~�ʂ����Z
		pos -= descent;
	}
	else
	{
		m_State = STATE(1);
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
