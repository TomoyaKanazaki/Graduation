//============================================
//
//	�v���C���[�̃��x���G�t�F�N�g���� [LevelModelEffect.cpp]
//	Author:sakamoto kai
//
//============================================
#include "LevelModelEffect.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "model.h"
#include "motion.h"
#include "input.h"
#include "sound.h"
#include "Effect.h"
#include "game.h"
#include "time.h"
#include "camera.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float PLAYER_ROT_SPEED = 0.2f;		//�v���C���[�̉�]�X�s�[�h
	const float PLAYER_SPEED = 10.0f;		//�v���C���[�̑���
	const float PLAYER_JAMPPOWER = 15.0f;	//�v���C���[�̃W�����v��
	const float PLAYER_JAMPWALL = 4.0f;		//�W�����v�܂ł̗͂̐U�蕝
	const int JAMP_ACTIONNOT = 4;			//�W�����v����s���o����܂�
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(40.0f, 80.0f, 40.0f);		//���̓����蔻��
}

//==========================================
// �ÓI�����o�ϐ��錾
//==========================================
CListManager<CLevelModelEffect>* CLevelModelEffect::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CLevelModelEffect::CLevelModelEffect(int nPriority) :CObject(nPriority)
{
	m_pos = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_Action = ACTION_WAIT;
	m_Color = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	m_fDel = 0.05f;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CLevelModelEffect::~CLevelModelEffect()
{

}

//====================================================================
//��������
//====================================================================
CLevelModelEffect* CLevelModelEffect::Create()
{
	CLevelModelEffect* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{
		//�v���C���[�̐���
		pPlayer = new CLevelModelEffect();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pPlayer->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pPlayer;
}

//====================================================================
//����������
//====================================================================
HRESULT CLevelModelEffect::Init(void)
{
	SetType(CObject::TYPE_PLAYEREFFECT);

	// �L�����N�^�[�N���X�̐���
	if (m_pCharacter == nullptr)
	{
		m_pCharacter->Create("data\\TXT\\motion_foot_light_spear.txt");
	}

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CLevelModelEffect>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CLevelModelEffect::Uninit(void)
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
void CLevelModelEffect::Update(void)
{
	m_Color.a -= m_fDel;

	if (m_pCharacter != nullptr)
	{
		// ���f�������擾
		int nNumModel = m_pCharacter->GetNumModel();

		for (int nCnt = 0; nCnt < nNumModel; nCnt++)
		{
			// ���f���̎擾����
			CModel* pModel = m_pCharacter->GetModel(nCnt);

			if (pModel != nullptr)
			{
				// �F�ݒ菈��
				pModel->SetColorType(CModel::COLORTYPE_TRUE_ALL);
				pModel->SetColor(m_Color);
			}
		}
	}

	if (m_Color.a < 0.0f)
	{
		Uninit();
	}

	////���[�V�����̍X�V
	//m_pMotion->Update();
}

//====================================================================
//�v���C���[�̎p���擾���ĕ`�悷��
//====================================================================
void CLevelModelEffect::SetAllPose(int nType, int nKey, float nCounter)
{
	if (m_pCharacter != nullptr)
	{
		// ���[�V�����̎擾����
		CMotion* pMotion = m_pCharacter->GetMotion();

		if (pMotion != nullptr)
		{
			pMotion->SetStopPose(nType, nKey, nCounter);
		}
	}
}

//====================================================================
//�v���C���[�̎p���擾���ĕ`�悷��
//====================================================================
void CLevelModelEffect::SetPose(int nType, int nKey, float nCounter, int nModelNumber)
{
	if (m_pCharacter != nullptr)
	{
		// ���f�������擾
		int nNumModel = m_pCharacter->GetNumModel();

		for (int nCnt = 0; nCnt < nNumModel; nCnt++)
		{
			// ���[�V�����̎擾����
			CModel* pModel = m_pCharacter->GetModel(nCnt);

			if (pModel != nullptr)
			{
				// �F�ݒ菈��
				pModel->SetDisp(false);
			}
		}

		// ���[�V�����̎擾����
		CMotion* pMotion = m_pCharacter->GetMotion();

		if (pMotion != nullptr)
		{
			pMotion->SetStopPose(nType, nKey, nCounter);
		}
	}

}

//====================================================================
//�`�揈��
//====================================================================
void CLevelModelEffect::Draw(void)
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

	//���[���h�}�g���b�N�X�̐ݒ�
	m_pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �L�����N�^�[�̕`��
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CLevelModelEffect>* CLevelModelEffect::GetList(void)
{
	return m_pList;
}
