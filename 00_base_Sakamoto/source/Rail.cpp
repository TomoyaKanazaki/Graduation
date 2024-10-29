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

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
}

//====================================================================
//�R���X�g���N�^
//====================================================================
CRail::CRail(int nPriority) : CObject(nPriority)
{
	m_pRailModel[0] = nullptr;
	m_pRailModel[1] = nullptr;

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
	CRail* pSample = nullptr;

	if (pSample == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CRail();
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
HRESULT CRail::Init()
{
	SetType(CObject::TYPE_RAIL);

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


	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRail::Uninit(void)
{

}

//====================================================================
//�X�V����
//====================================================================
void CRail::Update(void)
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
void CRail::TitleUpdate(void)
{

}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CRail::GameUpdate(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			m_pRailModel[nCnt]->SetPos(CMapSystem::GetInstance()->GetGritPos(m_nMapWidth, m_nMapHeight));
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
	m_bRail[Set] = true;

	if (m_pRailModel[0] == nullptr)
	{
		m_pRailModel[0] = CObjectX::Create("data\\MODEL\\TestRail.x");
		m_pRailModel[0]->SetPos(CMapSystem::GetInstance()->GetGritPos(m_nMapWidth, m_nMapHeight));
		m_pRailModel[0]->SetMultiMatrix(true);

		switch (Set)
		{
		case CRail::RAIL_POS_UP:
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			break;
		case CRail::RAIL_POS_DOWN:
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			break;
		case CRail::RAIL_POS_LEFT:
			m_pRailModel[0]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			break;
		case CRail::RAIL_POS_RIGHT:
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
	m_bRail[Set] = true;

	if (m_pRailModel[1] == nullptr)
	{
		m_pRailModel[1] = CObjectX::Create("data\\MODEL\\TestRail.x");
		m_pRailModel[1]->SetPos(CMapSystem::GetInstance()->GetGritPos(m_nMapWidth, m_nMapHeight));
		m_pRailModel[1]->SetMultiMatrix(true);

		int nMapWight = GetWightNumber();
		int nMapHeight = GetHeightNumber();

		switch (Set)
		{
		case CRail::RAIL_POS_UP:
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
			Set = RAIL_POS_DOWN;
			nMapHeight = GetHeightNumber() - 1;
			break;

		case CRail::RAIL_POS_DOWN:
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f));
			Set = RAIL_POS_UP;
			nMapHeight = GetHeightNumber() + 1;
			break;

		case CRail::RAIL_POS_LEFT:
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
			Set = RAIL_POS_RIGHT;
			nMapWight = GetWightNumber() - 1;
			break;

		case CRail::RAIL_POS_RIGHT:
			m_pRailModel[1]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
			Set = RAIL_POS_LEFT;
			nMapWight = GetWightNumber() + 1;
			break;

		default:
			break;
		}

		m_pNext = CRail::Create();
		m_pNext->SetWightNumber(nMapWight);
		m_pNext->SetHeightNumber(nMapHeight);
		m_pNext->SetPrevRail(this);
		m_pNext->PrevSet(Set);
	}
}