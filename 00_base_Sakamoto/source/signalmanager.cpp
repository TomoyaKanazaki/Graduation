//============================================
//
//	�V�O�i���}�l�[�W���[ [signalmanager.cpp]
//	Author:morikawa shunya
//
//============================================
#include "signalmanager.h"
#include "useful.h"
#include "MapSystem.h"
#include "game.h"
#include "renderer.h"
#include "tutorial.h"
#include "objmeshField.h"
#include "devil.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float BASE_Y = 50.0f; // ����
}

//===========================================
// �ÓI�����o�ϐ��錾
//===========================================
CListManager<CSignalManager>* CSignalManager::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
// �R���X�g���N�^
//====================================================================
CSignalManager::CSignalManager(int nPriority)
{
	m_nStateCount = 0;			// ��ԊǗ�
	m_nStateNum = 0;
	m_fRot = 0.0f;
	m_pDevil = nullptr;
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CSignalManager::~CSignalManager()
{

}

//==========================================
//  ���X�g�̎擾
//==========================================
CListManager<CSignalManager>* CSignalManager::GetList(void)
{
	return m_pList;
}

//====================================================================
// ����
//====================================================================
CSignalManager* CSignalManager::Create()
{
	CSignalManager* pSignalmanager = nullptr;

	if (pSignalmanager == nullptr)
	{
		//�v���C���[�̐���
		pSignalmanager = new CSignalManager();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSignalmanager->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pSignalmanager;
}


//====================================================================
// ������
//====================================================================
HRESULT CSignalManager::Init(void)
{
	// ���X�g�}�l�[�W���[�̐���
	if (m_pList == nullptr)
	{
		m_pList = CListManager<CSignalManager>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	m_pDevil = CDevil::Create();

	return S_OK;
}

//====================================================================
// �I��
//====================================================================
void CSignalManager::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}
}

//====================================================================
// �X�V
//====================================================================
void CSignalManager::Update()
{

}

//====================================================================
// ���[�u�ݒ�
//====================================================================
void CSignalManager::SetMove(int nType)
{
	
}