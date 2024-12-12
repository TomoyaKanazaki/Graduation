//============================================
//
//	���[���}�l�[�W���[ [RailManager.cpp]
//	Author:Satone Shion
//
//============================================
#include "RailManager.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* FILE_PASS = "data\\MODEL\\fireball.x"; // ���f���p�X
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CRailManager>* CRailManager::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CRailManager::CRailManager()
{
	m_pRailManagerModel[0] = nullptr;
	m_pRailManagerModel[1] = nullptr;

	m_pPrev = nullptr;		// �O�̃��[���ւ̃|�C���^
	m_pNext = nullptr;		// ���̃��[���ւ̃|�C���^
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CRailManager::~CRailManager()
{

}

//====================================================================
//��������
//====================================================================
CRailManager* CRailManager::Create()
{
	CRailManager* pSample = nullptr;

	if (pSample == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CRailManager();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pSample;
}

//====================================================================
//����������
//====================================================================
HRESULT CRailManager::Init()
{

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CRailManager>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRailManager::Uninit(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailManagerModel[nCnt] != nullptr)
		{
			m_pRailManagerModel[nCnt]->SetDeathFlag(true);
		}
	}

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}
}

//====================================================================
//�I������
//====================================================================
void CRailManager::SetNULL(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailManagerModel[nCnt] != nullptr)
		{
			m_pRailManagerModel[nCnt] = nullptr;
		}
	}
}

//====================================================================
//�X�V����
//====================================================================
void CRailManager::Update(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailManagerModel[nCnt] != nullptr)
		{
			m_pRailManagerModel[nCnt]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CRailManager::Draw(void)
{

}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CRailManager>* CRailManager::GetList(void)
{
	return m_pList;
}
