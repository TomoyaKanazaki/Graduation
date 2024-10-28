//============================================
//
//	���[���u���b�N�̏��� [RailBlock.cpp]
//	Author:sakamoto kai
//
//============================================
#include "RailBlock.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "Rail.h"
#include "game.h"
#include "Devil.h"
#include "MapSystem.h"

//==========================================
//  �萔��`
//==========================================
namespace
{

}

//====================================================================
//�R���X�g���N�^
//====================================================================
CRailBlock::CRailBlock(int nPriority) :CCubeBlock(nPriority)
{
	m_pTop = nullptr;		// �擪�̃��[���ւ̃|�C���^
	m_pCur = nullptr;		// �Ō���̃��[���ւ̃|�C���^

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		bMoveOK[nCnt] = false;
	}
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CRailBlock::~CRailBlock()
{

}

//====================================================================
//��������
//====================================================================
CRailBlock* CRailBlock::Create(int nMapWight, int nMapHeight)
{
	CRailBlock* pObjectBlock = nullptr;

	if (pObjectBlock == nullptr)
	{
		//�I�u�W�F�N�g3D�̐���
		pObjectBlock = new CRailBlock();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pObjectBlock->Init(nMapWight, nMapHeight)))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pObjectBlock;
}

//====================================================================
//����������
//====================================================================
HRESULT CRailBlock::Init(int nMapWight, int nMapHeight)
{
	SetWightNumber(nMapWight);
	SetHeightNumber(nMapHeight);

	StartWightNumber = nMapWight;
	StartHeightNumber = nMapHeight;

	CCubeBlock::Init();

	SetTexture("data\\TEXTURE\\Wood001.png");

	SetType(TYPE_RAILBLOCK);

	RailSet();

	RailCheck();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRailBlock::Uninit(void)
{
	CCubeBlock::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CRailBlock::Update(void)
{
	D3DXVECTOR3 Pos = GetPos();

	Move(&Pos);

	if (GetWightNumber() != CMapSystem::GetInstance()->GetGritWightNumber(Pos.x))
	{
		SetWightNumber(CMapSystem::GetInstance()->GetGritWightNumber(Pos.x));
		RailCheck();
	}

	if (GetHeightNumber() != CMapSystem::GetInstance()->GetGritHeightNumber(Pos.z))
	{
		SetHeightNumber(CMapSystem::GetInstance()->GetGritHeightNumber(Pos.z));
		RailCheck();
	}

	SetPos(Pos);

	CCubeBlock::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CRailBlock::Draw(void)
{
	CCubeBlock::Draw();
}

//====================================================================
//�X�����̈ړ�����
//====================================================================
void CRailBlock::Move(D3DXVECTOR3* Pos)
{
	D3DXVECTOR3 SlopeMove = INITVECTOR3;
	D3DXVECTOR3 SlopeRot = CGame::GetDevil()->GetDevilRot();
	D3DXVECTOR3 GritPos = CMapSystem::GetInstance()->GetGritPos(GetWightNumber(), GetHeightNumber());

	SlopeMove.x = SlopeRot.x * 10.0f;
	SlopeMove.z = SlopeRot.z * 10.0f;

	Pos->x -= SlopeMove.z;
	Pos->z += SlopeMove.x;

	// ��
	if (!bMoveOK[0])
	{
		if (Pos->z > GritPos.z)
		{
			Pos->z = GritPos.z;
		}
	}

	// ��
	if (!bMoveOK[1])
	{
		if (Pos->z < GritPos.z)
		{
			Pos->z = GritPos.z;
		}
	}

	// ��
	if (!bMoveOK[2])
	{
		if (Pos->x < GritPos.x)
		{
			Pos->x = GritPos.x;
		}
	}

	// �E
	if (!bMoveOK[3])
	{
		if (Pos->x > GritPos.x)
		{
			Pos->x = GritPos.x;
		}
	}
}

//====================================================================
//���[���̗L����ݒ肷��
//====================================================================
void CRailBlock::RailCheck(void)
{
	CRail* pRail = m_pTop;
	int WightNumber = GetWightNumber();
	int HeightNumber = GetHeightNumber();

	while (1)
	{
		if (WightNumber == pRail->GetWightNumber() &&
			HeightNumber == pRail->GetHeightNumber())
		{
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				bMoveOK[nCnt] = pRail->GetRailOK(nCnt);
			}
			return;
		}

		if (pRail == m_pCur)
		{
			break;
		}

		pRail = pRail->GetNextRail();
	}
}

//====================================================================
//���[���̐ݒu����
//====================================================================
void CRailBlock::RailSet(void)
{
	m_pTop = CRail::Create();
	m_pTop->SetWightNumber(GetWightNumber());
	m_pTop->SetHeightNumber(GetHeightNumber());
	m_pTop->NextSet(CRail::RAIL_POS_RIGHT);

	int nMax = 4;
	int nData[4];

	nData[0] = 0;
	nData[1] = 3;
	nData[2] = 1;
	nData[3] = 3;

	CRail* pRail = m_pTop->GetNextRail();

	for (int nCnt = 0; nCnt < nMax; nCnt++)
	{
		pRail->NextSet((CRail::RAIL_POS)nData[nCnt]);

		pRail = pRail->GetNextRail();
	}

	m_pCur = pRail;
}