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
#include "effect.h"
#include "game.h"

#include "character.h"
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
CListManager<CSlopeDevice>* CSlopeDevice::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CSlopeDevice::CSlopeDevice(int nPriority) : CObject(nPriority)
{
	m_pos = INITVECTOR3;
	m_posOld = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = INITVECTOR3;

	m_State = STATE(0);
	m_nStateCount = 0;

	m_pCharacter = nullptr;
	m_bMultiMatrix = false;
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

	// �L�����N�^�[�̏I������
	if (m_pCharacter != nullptr)
	{
		// �L�����N�^�[�̔j��
		m_pCharacter->Uninit();
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CSlopeDevice::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
	case CScene::MODE_TUTORIAL:

		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CSlopeDevice::TitleUpdate(void)
{
	if (m_pCharacter != nullptr)
	{
		// �L�����N�^�[�̍X�V
		m_pCharacter->Update();
	}
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CSlopeDevice::GameUpdate(void)
{
	//��ԊǗ�
	StateManager();

	if (m_pCharacter != nullptr)
	{
		// �L�����N�^�[�̍X�V
		m_pCharacter->Update();
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CSlopeDevice::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 m_pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bMultiMatrix)
	{
		SetUseMultiMatrix(CGame::GetMapField()->GetMatrix());

		//�Z�o�����}�g���N�X���������킹��
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_UseMultiMatrix);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���̕`��(�S�p�[�c)
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
}

//====================================================================
// ���f���֘A�̏���������
//====================================================================
HRESULT CSlopeDevice::InitModel(const char* pModelNameSlopeDevice, const char* pModelNameEnemy)
{
	if (m_pCharacter == nullptr)
	{
		m_pCharacter = CCharacter::Create(pModelNameSlopeDevice);

		if (m_pCharacter == nullptr)
		{
			return E_FAIL;
		}
	}

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
	case STATE_ROTATE:

		// ���[���[�Ɖ񂵎ԉ�]����
		Rotate(SETUP_TYPE_ROLLRE, ROTATE_ADD);
		Rotate(SETUP_TYPE_MAWASIGURMA, ROTATE_ADD);
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
void CSlopeDevice::Rotate(int nNldxModel,D3DXVECTOR3 rotate)
{
	if (m_pCharacter == nullptr)
	{
		// �A�T�[�g
		assert(("�L�����N�^�[�N���X���Ȃ���", false));
		return;
	}

	// ���f���̎擾
	CModel* pModel = m_pCharacter->GetModel(nNldxModel);

	if (pModel == nullptr)
	{
		return;
	}

	// ���f���̉�]�����擾
	D3DXVECTOR3 rot = pModel->GetStartRot();
	
	if (m_rot.y == 0.0f)
	{
		// ��]�ʌ��Z
		rot += rotate;
	}
	else if (m_rot.y == D3DX_PI)
	{
		// ��]�ʉ��Z
		rot -= rotate;
	}

	// ���f���̉�]�ݒ�
	pModel->SetStartRot(rot);
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CSlopeDevice>* CSlopeDevice::GetList(void)
{
	return m_pList;
}
