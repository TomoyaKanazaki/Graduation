//============================================
//
//	���[���̏��� [Rail.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Rail.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "objectX.h"
#include "MapSystem.h"
#include "game.h"
#include "objmeshField.h"
#include "objectX.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* FILE_PASS = "data\\MODEL\\fireball.x"; // ���f���p�X
	const int MAX_RAIL = 2;		// 1�}�X�������[���̐�
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CRail>* CRail::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CRail::CRail(int nPriority) : CObject(nPriority)
{
	// ���[�����f��
	for (int nCnt = 0; nCnt < MAX_RAIL; nCnt++)
	{
		m_pRailModel[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < RAIL_POS_MAX; nCnt++)
	{
		m_bRail[nCnt] = false;
	}

	m_pPrev = nullptr;		// �O�̃��[���ւ̃|�C���^
	m_pNext = nullptr;		// ���̃��[���ւ̃|�C���^
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CRail::~CRail()
{

}

//====================================================================
//��������
//====================================================================
CRail* CRail::Create()
{
	CRail* pRail = nullptr;

	if (pRail == nullptr)
	{
		//���[���̐���
		pRail = new CRail();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pRail->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pRail;
}

//====================================================================
//����������
//====================================================================
HRESULT CRail::Init()
{

	// ����������
	for (int nCnt = 0; nCnt < MAX_RAIL; nCnt++)
	{
		m_pRailModel[nCnt]->Init(FILE_PASS);
	}

	SetType(CObject::TYPE_RAIL);

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CRail>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRail::Uninit(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			m_pRailModel[nCnt]->SetDeathFlag(true);
			m_pRailModel[nCnt]->Uninit();

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
}

//====================================================================
//�I������
//====================================================================
void CRail::SetNULL(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			m_pRailModel[nCnt] = nullptr;
		}
	}
}

//====================================================================
//�X�V����
//====================================================================
void CRail::Update(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			m_pRailModel[nCnt]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CRail::Draw(void)
{

}

//====================================================================
//�O�̃��f���̐ݒ�
//====================================================================
void CRail::PrevSet(RAIL_POS Set)
{
	//�����Őݒ肵�������Ƀ��[����u��
	m_bRail[Set] = true;

	//�^�񒆂���̃��[����ݒu����
	if (m_pRailModel[0] == nullptr)
	{
		m_pRailModel[0] = CObjectX::Create("data\\MODEL\\TestRail.x");
		m_pRailModel[0]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		m_pRailModel[0]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		//m_pRailModel[0]->SetMultiMatrix(true);

		switch (Set)
		{
		case CRail::RAIL_POS_UP:	// ��
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			break;
		case CRail::RAIL_POS_DOWN:	// ��
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			break;
		case CRail::RAIL_POS_LEFT:	// ��
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			break;
		case CRail::RAIL_POS_RIGHT:	// �E
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			break;
		default:
			break;
		}
	}
}

//====================================================================
//���̃��f���̐ݒ�
//====================================================================
void CRail::NextSet(RAIL_POS Set)
{
	//�����Őݒ肵�������Ƀ��[����u��
	m_bRail[Set] = true;
	m_bNextNumber = Set;

	//�^�񒆂܂ł̃��[����ݒu����
	if (m_pRailModel[1] == nullptr)
	{
		m_pRailModel[1] = CObjectX::Create("data\\MODEL\\TestRail.x");
		m_pRailModel[1]->SetPos(CMapSystem::GRID(m_nMapWidth, m_nMapHeight).ToWorld());
		m_pRailModel[1]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());
		//m_pRailModel[1]->SetMultiMatrix(true);

		//�L�΂��O�̃��[���̈ʒu���擾����
		int nMapWight = GetWightNumber();
		int nMapHeight = GetHeightNumber();

		//�����Őݒ肵�������Ƀ��[���̌����A�ԍ���ݒ�
		switch (Set)
		{
		case CRail::RAIL_POS_UP:	// ��
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			Set = RAIL_POS_DOWN;
			nMapHeight = GetHeightNumber() - 1;
			break;

		case CRail::RAIL_POS_DOWN:	// ��
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			Set = RAIL_POS_UP;
			nMapHeight = GetHeightNumber() + 1;
			break;

		case CRail::RAIL_POS_LEFT:	// ��
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			Set = RAIL_POS_RIGHT;
			nMapWight = GetWightNumber() - 1;
			break;

		case CRail::RAIL_POS_RIGHT:	// �E
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			Set = RAIL_POS_LEFT;
			nMapWight = GetWightNumber() + 1;
			break;

		default:
			break;
		}

		//���̃��[����ݒ肷��
		m_pNext = CRail::Create();
		m_pNext->SetWightNumber(nMapWight);
		m_pNext->SetHeightNumber(nMapHeight);
		m_pNext->SetPrevRail(this);
		m_pNext->PrevSet(Set);
	}
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CRail>* CRail::GetList(void)
{
	return m_pList;
}
