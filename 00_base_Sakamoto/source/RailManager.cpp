//============================================
//
//	���[���}�l�[�W���[ [RailManager.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RailManager.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "objectX.h"
#include "MapSystem.h"
#include "game.h"
#include "objmeshField.h"

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
CRailManager::CRailManager(int nPriority) : CObjectX(nPriority)
{
	m_pRailManagerModel[0] = nullptr;
	m_pRailManagerModel[1] = nullptr;

	for (int nCnt = 0; nCnt < RAIL_POS_MAX; nCnt++)
	{
		m_bRailManager[nCnt] = false;
	}

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
	CObjectX::Init(FILE_PASS);

	SetType(CObject::TYPE_RAIL);

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

	SetDeathFlag(true);

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	CObjectX::Uninit();
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
//�O�̃��f���̐ݒ�
//====================================================================
void CRailManager::PrevSet(RAIL_POS Set)
{
	//�����Őݒ肵�������Ƀ��[����u��
	m_bRailManager[Set] = true;

	//�^�񒆂���̃��[����ݒu����
	if (m_pRailManagerModel[0] == nullptr)
	{
		m_pRailManagerModel[0] = CObjectX::Create("data\\MODEL\\TestRailManager.x");
		m_pRailManagerModel[0]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		m_pRailManagerModel[0]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		//m_pRailManagerModel[0]->SetMultiMatrix(true);

		switch (Set)
		{
		case CRailManager::RAIL_POS_UP:	// ��
			m_pRailManagerModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			break;
		case CRailManager::RAIL_POS_DOWN:	// ��
			m_pRailManagerModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			break;
		case CRailManager::RAIL_POS_LEFT:	// ��
			m_pRailManagerModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			break;
		case CRailManager::RAIL_POS_RIGHT:	// �E
			m_pRailManagerModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			break;
		default:
			break;
		}
	}
}

//====================================================================
//���̃��f���̐ݒ�
//====================================================================
void CRailManager::NextSet(RAIL_POS Set)
{
	//�����Őݒ肵�������Ƀ��[����u��
	m_bRailManager[Set] = true;
	m_bNextNumber = Set;

	//�^�񒆂܂ł̃��[����ݒu����
	if (m_pRailManagerModel[1] == nullptr)
	{
		m_pRailManagerModel[1] = CObjectX::Create("data\\MODEL\\TestRailManager.x");
		m_pRailManagerModel[1]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		m_pRailManagerModel[1]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		//m_pRailManagerModel[1]->SetMultiMatrix(true);

		//�L�΂��O�̃��[���̈ʒu���擾����
		int nMapWight = GetWightNumber();
		int nMapHeight = GetHeightNumber();

		//�����Őݒ肵�������Ƀ��[���̌����A�ԍ���ݒ�
		switch (Set)
		{
		case CRailManager::RAIL_POS_UP:	// ��
			m_pRailManagerModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			Set = RAIL_POS_DOWN;
			nMapHeight = GetHeightNumber() - 1;
			break;

		case CRailManager::RAIL_POS_DOWN:	// ��
			m_pRailManagerModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			Set = RAIL_POS_UP;
			nMapHeight = GetHeightNumber() + 1;
			break;

		case CRailManager::RAIL_POS_LEFT:	// ��
			m_pRailManagerModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			Set = RAIL_POS_RIGHT;
			nMapWight = GetWightNumber() - 1;
			break;

		case CRailManager::RAIL_POS_RIGHT:	// �E
			m_pRailManagerModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			Set = RAIL_POS_LEFT;
			nMapWight = GetWightNumber() + 1;
			break;

		default:
			break;
		}

		//���̃��[����ݒ肷��
		m_pNext = CRailManager::Create();
		m_pNext->SetWightNumber(nMapWight);
		m_pNext->SetHeightNumber(nMapHeight);
		m_pNext->SetPrevRailManager(this);
		m_pNext->PrevSet(Set);
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CRailManager>* CRailManager::GetList(void)
{
	return m_pList;
}
