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
#include "tutorial.h"
#include "objmeshField.h"
#include "Devil.h"
#include "debugproc.h"
#include "player.h"
#include "MapMove.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const float RAIL_WIGHT = 5.0f;	//���[����
}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CRailBlock>* CRailBlock::m_pList = nullptr; // �I�u�W�F�N�g���X�g

//====================================================================
//�R���X�g���N�^
//====================================================================
CRailBlock::CRailBlock(int nPriority) :CObjectX(nPriority),
m_Grid(0, 0),
m_OldGrid(0, 0)
{
	m_pTop = nullptr;		// �擪�̃��[���ւ̃|�C���^
	m_pCur = nullptr;		// �Ō���̃��[���ւ̃|�C���^

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		bMoveOK[nCnt] = false;
	}
}

//====================================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//====================================================================
CRailBlock::CRailBlock(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority),
m_Grid(gridCenter),
m_OldGrid(gridCenter)
{
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
CRailBlock* CRailBlock::Create(CMapSystem::GRID gridCenter)
{
	CRailBlock* pObjectBlock = nullptr;

	if (pObjectBlock == nullptr)
	{
		//�I�u�W�F�N�g3D�̐���
		pObjectBlock = new CRailBlock(3, gridCenter);
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pObjectBlock->Init("data\\MODEL\\00_railblock.x")))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pObjectBlock;
}

//====================================================================
//����������
//====================================================================
HRESULT CRailBlock::Init(char* pModelName)
{
	/*SetWightNumber(nMapWight);
	SetHeightNumber(nMapHeight);

	m_StartGrid.x = nMapWight;
	m_StartGrid.z = nMapHeight;*/

	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// ����������
	CObjectX::Init(pModelName);

	// �ʒu�ݒ�
	SetPos(m_Grid.ToWorld());

	// ��ނ̐ݒ�
	SetType(TYPE_RAILBLOCK);

	/*if (Edit == false)
	{
		RailSet(Max, nMove);

		RailCheck();
	}*/

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CRailBlock>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRailBlock::Uninit(void)
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	//���g���������郌�[����S�č폜����
	/*CRail* pRail = m_pTop;
	while (1)
	{
		if (pRail != nullptr)
		{
			pRail->Uninit();

			pRail = pRail->GetNextRail();
		}
		else
		{
			break;
		}
	}
	m_pTop = nullptr;
	m_pCur = nullptr;*/

	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CRailBlock::Update(void)
{
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PosOld = GetPos();

	Move(&Pos);

	// ���ԍ����O��ƈ�v���Ȃ����ɃO���b�g�̃u���b�N�̗L����؂�ւ���
	//if (GetWightNumber() != CMapSystem::GetInstance()->CalcGridX(Pos.x))
	//{
	//	CMapSystem::GetInstance()->SetGritBool(GetWightNumber(), GetHeightNumber(), false);
	//	SetWightNumber(CMapSystem::GetInstance()->CalcGridX(Pos.x));
	//	CMapSystem::GetInstance()->SetGritBool(GetWightNumber(), GetHeightNumber(), true);

	//	RailCheck();
	//}

	//// �c�ԍ����O��ƈ�v���Ȃ����ɃO���b�g�̃u���b�N�̗L����؂�ւ���
	//if (GetHeightNumber() != CMapSystem::GetInstance()->CalcGridZ(Pos.z))
	//{
	//	CMapSystem::GetInstance()->SetGritBool(GetWightNumber(), GetHeightNumber(), false);
	//	SetHeightNumber(CMapSystem::GetInstance()->CalcGridZ(Pos.z));
	//	CMapSystem::GetInstance()->SetGritBool(GetWightNumber(), GetHeightNumber(), true);

	//	RailCheck();
	//}

	// �X�N���[���ɍ��킹�Ĉړ�����
	CMapSystem::GetInstance()->GetMove()->FollowScroll(Pos);

	// �ʒu�ݒ�
	SetPos(Pos);
	SetPosOld(PosOld);

	CObjectX::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CRailBlock::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//�X�����̈ړ�����
//====================================================================
void CRailBlock::Move(D3DXVECTOR3* Pos)
{
	// �O���b�h����ۑ�
	m_OldGrid = m_Grid;

	D3DXVECTOR3 TestPos = INITVECTOR3;

	D3DXVECTOR3 SlopeMove = INITVECTOR3;
	D3DXVECTOR3 SlopeRot = INITVECTOR3;
	SlopeRot = CMapMove::GetListTop()->GetDevilRot();
	//D3DXVECTOR3 GritPos = CMapSystem::GRID(GetWightNumber(), GetHeightNumber()).ToWorld();
	//D3DXVECTOR3 GritDistance = *Pos - GritPos;	//�O���b�g�̒��S�Ƃ������̑��Έʒu�A����

	//�X���ɂ��ړ��ʐݒ�
	SlopeMove.x = -SlopeRot.z * 10.0f;
	SlopeMove.z = SlopeRot.x * 10.0f;

	//if (useful::CollisionCircle(GritPos, D3DXVECTOR3(Pos->x, GritPos.y, Pos->z), RAIL_WIGHT) == true)
	//{// �u���b�N�̒��S�ɂ��鎞�ɏ㉺�����E�̂ǂ��炩�ɂȂ�܂łɈړ�����

	//	Pos->x += SlopeMove.x;
	//	Pos->z += SlopeMove.z;
	//}
	//else
	//{// �u���b�N�̒��S�ɂȂ��Ƃ�

	//	//�㉺�ړ�
	//	if (GritPos.x - Pos->x >= -5.0f && GritPos.x - Pos->x <= RAIL_WIGHT)
	//	{
	//		Pos->z += SlopeMove.z;
	//	}
	//	else
	//	{
	//		Pos->z = GritPos.z;
	//	}

	//	//���E�ړ�
	//	if (GritPos.z - Pos->z >= -5.0f && GritPos.z - Pos->z <= RAIL_WIGHT)
	//	{
	//		Pos->x += SlopeMove.x;
	//	}
	//	else
	//	{
	//		Pos->x = GritPos.x;
	//	}
	//}

	// ��
	//if (!bMoveOK[0])
	//{
	//	if (Pos->z > GritPos.z)
	//	{
	//		Pos->z = GritPos.z;
	//	}
	//}

	//// ��
	//if (!bMoveOK[1])
	//{
	//	if (Pos->z < GritPos.z)
	//	{
	//		Pos->z = GritPos.z;
	//	}
	//}

	//// ��
	//if (!bMoveOK[2])
	//{
	//	if (Pos->x < GritPos.x)
	//	{
	//		Pos->x = GritPos.x;
	//	}
	//}

	//// �E
	//if (!bMoveOK[3])
	//{
	//	if (Pos->x > GritPos.x)
	//	{
	//		Pos->x = GritPos.x;
	//	}
	//}

	// ���E�̃O���b�g�̔ԍ����G���[�ԍ�(�}�b�v�O)�ɔ�яo�Ă鎞
	//if (CMapSystem::GetInstance()->CalcGridX(Pos->x) == -1)
	//{
	//	if (Pos->x > 0.0f)	//�E�̃O���b�g�O�ɏo���Ƃ�
	//	{
	//		if (SlopeMove.x > 0.0f && bMoveOK[3])	//X���̓�����[+]�ŉE�Ƀ��[�������݂��鎞
	//		{
	//			//�P�E�̃O���b�g�Ɉړ�
	//			GritPos = CMapSystem::GRID(GetWightNumber() + 1, GetHeightNumber()).ToWorld();
	//		}
	//		else
	//		{
	//			// ���݂̃O���b�g�Ɉړ�
	//			GritPos = CMapSystem::GRID(GetWightNumber(), GetHeightNumber()).ToWorld();
	//		}
	//	}
	//	else if (Pos->x < 0.0f)	//���̃O���b�g�O�ɏo���Ƃ�
	//	{
	//		if (SlopeMove.x < 0.0f && bMoveOK[2])	//X���̓�����[-]�ō��Ƀ��[�������݂��鎞
	//		{
	//			//�P�E�̃O���b�g�Ɉړ�
	//			GritPos = CMapSystem::GRID(GetWightNumber() - 1, GetHeightNumber()).ToWorld();
	//		}
	//		else
	//		{
	//			// ���݂̃O���b�g�Ɉړ�
	//			GritPos = CMapSystem::GRID(GetWightNumber(), GetHeightNumber()).ToWorld();
	//		}
	//	}
	//		Pos->x = GritPos.x;
	//}

	//// �㉺�̃O���b�g�̔ԍ����G���[�ԍ�(�}�b�v�O)�ɔ�яo�Ă鎞
	//if (CMapSystem::GetInstance()->CalcGridZ(Pos->z) == -1)
	//{
	//	if (Pos->z > 0.0f)	//�E�̃O���b�g�O�ɏo���Ƃ�
	//	{
	//		if (SlopeMove.z > 0.0f && bMoveOK[0])	//Z���̓�����[+]�ŉE�Ƀ��[�������݂��鎞
	//		{
	//			//�P�E�̃O���b�g�Ɉړ�
	//			GritPos = CMapSystem::GRID(GetWightNumber(), GetHeightNumber() - 1).ToWorld();
	//		}
	//		else
	//		{
	//			// ���݂̃O���b�g�Ɉړ�
	//			GritPos = CMapSystem::GRID(GetWightNumber(), GetHeightNumber()).ToWorld();
	//		}
	//	}
	//	else if (Pos->z < 0.0f)	//���̃O���b�g�O�ɏo���Ƃ�
	//	{
	//		if (SlopeMove.z < 0.0f && bMoveOK[1])	//Z���̓�����[-]�ō��Ƀ��[�������݂��鎞
	//		{
	//			//�P�E�̃O���b�g�Ɉړ�
	//			GritPos = CMapSystem::GRID(GetWightNumber(), GetHeightNumber() + 1).ToWorld();
	//		}
	//		else
	//		{
	//			// ���݂̃O���b�g�Ɉړ�
	//			GritPos = CMapSystem::GRID(GetWightNumber(), GetHeightNumber()).ToWorld();
	//		}
	//	}
	//	Pos->z = GritPos.z;
	//}

	// �O���b�h����ݒ�
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(*Pos);

	// A*�����ݒ�
	Coodinate();

	DebugProc::Print(DebugProc::POINT_LEFT, "[���[���u���b�N]�ʒu %f : %f : %f\n", Pos->x, Pos->y, Pos->z);
	/*DebugProc::Print(DebugProc::POINT_LEFT, "[���[���u���b�N]���ԍ� %d \n", GetWightNumber());
	DebugProc::Print(DebugProc::POINT_LEFT, "[���[���u���b�N]�c�ԍ� %d \n", GetHeightNumber());
	DebugProc::Print(DebugProc::POINT_LEFT, "[���[���u���b�N]���� %f : %f \n", GritDistance.x, GritDistance.z);*/
	DebugProc::Print(DebugProc::POINT_LEFT, "[�e�X�g�u���b�N]���� %f : %f \n", TestPos.x, TestPos.z);
}

//====================================================================
//���[���̗L����ݒ肷��
//====================================================================
void CRailBlock::RailCheck(void)
{
	CRail* pRail = m_pTop;
	//int WightNumber = GetWightNumber();
	//int HeightNumber = GetHeightNumber();

	////�u���b�N���̃��X�g���񂵂ău���b�N�ƃO���b�g�ԍ�����v���郌�[���̏㉺���E�̗L��������
	//while (1)
	//{
	//	if (WightNumber == pRail->GetWightNumber() &&
	//		HeightNumber == pRail->GetHeightNumber())
	//	{
	//		for (int nCnt = 0; nCnt < 4; nCnt++)
	//		{
	//			bMoveOK[nCnt] = pRail->GetRailOK(nCnt);
	//		}
	//		return;
	//	}

	//	if (pRail == m_pCur)
	//	{
	//		break;
	//	}

	//	pRail = pRail->GetNextRail();
	//}
}

//====================================================================
//�ݒu�������[���ƒu���Ă��郌�[���̈ʒu�������Ƃ��Ɉړ��\������ǉ�����
//====================================================================
void CRailBlock::RailAddWrite(void)
{
	//CRail* pCheckRail = m_pTop;

	////�u���b�N���̃��X�g���񂵂ău���b�N�ƃO���b�g�ԍ�����v���郌�[���̏㉺���E�̗L��������
	//while (1)
	//{
	//	if (pRail->GetWightNumber() == pCheckRail->GetWightNumber() &&
	//		pRail->GetHeightNumber() == pCheckRail->GetHeightNumber())
	//	{
	//		switch (nMove[nCnt])
	//		{
	//		case 0:
	//			pCheckRail->SetRailOK(1, true);
	//			break;

	//		case 1:
	//			pCheckRail->SetRailOK(0, true);
	//			break;

	//		case 2:
	//			pCheckRail->SetRailOK(3, true);
	//			break;

	//		case 3:
	//			pCheckRail->SetRailOK(2, true);
	//			break;
	//		}
	//	}

	//	if (pRail == m_pCur)
	//	{
	//		break;
	//	}

	//	pRail = pRail->GetNextRail();
	//}
}

//====================================================================
//���[���̐ݒu����
//====================================================================
void CRailBlock::RailSet(int Max, int* nMove)
{
	//m_nMax = Max;		//���[����

	//// ���O�ɐݒ肵�����[���̐ݒu���s��
	//m_pTop = CRail::Create();
	//m_pTop->SetWightNumber(GetWightNumber());
	//m_pTop->SetHeightNumber(GetHeightNumber());
	//m_pTop->NextSet((CRail::RAIL_POS)nMove[0]);
	//m_nMove[0] = nMove[0];

	//// ���[���ݒu
	//CRail* pRail = m_pTop->GetNextRail();

	//for (int nCnt = 1; nCnt < m_nMax; nCnt++)
	//{
	//	m_nMove[nCnt] = nMove[nCnt];
	//	pRail->NextSet((CRail::RAIL_POS)nMove[nCnt]);

	//	//�ݒu�������[���ƒu���Ă��郌�[���̈ʒu�������Ƃ��Ɉړ��\������ǉ�����
	//	CRail* pCheckRail = m_pTop;
	//	while (1)
	//	{
	//		if (pRail->GetWightNumber() == pCheckRail->GetWightNumber() &&
	//			pRail->GetHeightNumber() == pCheckRail->GetHeightNumber() &&
	//			pCheckRail != m_pCur)
	//		{
	//			pCheckRail->SetRailOK(nMove[nCnt], true);
	//		}

	//		pCheckRail = pCheckRail->GetNextRail();

	//		if (pCheckRail == nullptr)
	//		{
	//			break;
	//		}
	//	}
	//	//=======================

	//	pRail = pRail->GetNextRail();

	//	m_pCur = pRail;

	//	//�ݒu�������[���ƒu���Ă��郌�[���̈ʒu�������Ƃ��Ɉړ��\������ǉ�����
	//	pCheckRail = m_pTop;
	//	while (1)
	//	{
	//		if (pRail->GetWightNumber() == pCheckRail->GetWightNumber() &&
	//			pRail->GetHeightNumber() == pCheckRail->GetHeightNumber() &&
	//			pCheckRail != m_pCur)
	//		{
	//			switch (nMove[nCnt])
	//			{
	//			case 0:
	//				pCheckRail->SetRailOK(1, true);
	//				break;

	//			case 1:
	//				pCheckRail->SetRailOK(0, true);
	//				break;

	//			case 2:
	//				pCheckRail->SetRailOK(3, true);
	//				break;

	//			case 3:
	//				pCheckRail->SetRailOK(2, true);
	//				break;
	//			}
	//		}

	//		if (pCheckRail == m_pCur)
	//		{
	//			break;
	//		}

	//		pCheckRail = pCheckRail->GetNextRail();
	//	}
	//	//=======================
	//}
}

//==========================================
//  A*�E�F�C�g�̕ύX����
//==========================================
void CRailBlock::Coodinate()
{
	// �O��̃O���b�h�ƍ���̃O���b�h����v���Ă���ꍇ�֐��𔲂���
	if (m_Grid == m_OldGrid) { return; }

	// �o�H�T���p�̏����擾
	auto generator = AStar::Generator::GetInstance();
	if (generator == nullptr)
	{
		assert(false);
		generator = AStar::Generator::Create();
	}

	// �}�b�v�����擾
	CMapSystem* pMapSystem = CMapSystem::GetInstance();

	// �O��̃O���b�h���ړ��\�n�_�ɐݒ�
	generator->removeCollision(m_OldGrid.ToAStar());
	pMapSystem->SetGritBool(m_OldGrid, false);

	// ���݂̃O���b�h���ړ��s�n�_�ɐݒ�
	generator->addCollision(m_Grid.ToAStar());
	pMapSystem->SetGritBool(m_Grid, true);
}

//====================================================================
//�G�f�B�b�g�p�̃��[���̐ݒu����
//====================================================================
void CRailBlock::EditRailSet(int Number)
{
	/*int nWightNumber = GetWightNumber();
	int nHeightNumber = GetHeightNumber();*/

	//if (m_pTop == nullptr)
	//{
	//	// ���O�ɐݒ肵�����[���̐ݒu���s��
	//	m_pTop = CRail::Create();
	//	m_pTop->SetWightNumber(nWightNumber);
	//	m_pTop->SetHeightNumber(nHeightNumber);
	//	m_pTop->NextSet((CRail::RAIL_POS)Number);

	//	m_pCur = m_pTop->GetNextRail();
	//}
	//else
	//{
	//	// ���[���ݒu
	//	CRail* pRail = m_pCur;

	//	nWightNumber = pRail->GetWightNumber();
	//	nHeightNumber = pRail->GetHeightNumber();

	//	pRail->SetWightNumber(nWightNumber);
	//	pRail->SetHeightNumber(nHeightNumber);
	//	pRail->NextSet((CRail::RAIL_POS)Number);

	//	m_pCur = pRail->GetNextRail();
	//}
}

//====================================================================
//�G�f�B�b�g�p�̃��[���̐ݒu����
//====================================================================
void CRailBlock::EditRailUpdate(void)
{
	CRail* pRail = m_pTop;
	//int WightNumber = GetWightNumber();
	//int HeightNumber = GetHeightNumber();

	//if (pRail == nullptr)
	//{
	//	return;
	//}


	////�u���b�N���̃��X�g���񂵂ău���b�N�ƃO���b�g�ԍ�����v���郌�[���̏㉺���E�̗L��������
	//while (1)
	//{
	//	if (pRail == m_pTop)
	//	{
	//		pRail->SetWightNumber(WightNumber);
	//		pRail->SetHeightNumber(HeightNumber);
	//	}
	//	else
	//	{
	//		switch (pRail->GetPrevRail()->GetNextNumber())
	//		{
	//		case 0:
	//			pRail->SetWightNumber(WightNumber);
	//			pRail->SetHeightNumber(HeightNumber - 1);
	//			break;

	//		case 1:
	//			pRail->SetWightNumber(WightNumber);
	//			pRail->SetHeightNumber(HeightNumber + 1);
	//			break;

	//		case 2:
	//			pRail->SetWightNumber(WightNumber - 1);
	//			pRail->SetHeightNumber(HeightNumber);
	//			break;

	//		case 3:
	//			pRail->SetWightNumber(WightNumber + 1);
	//			pRail->SetHeightNumber(HeightNumber);
	//			break;
	//		}
	//	}

	//	WightNumber = pRail->GetWightNumber();
	//	HeightNumber = pRail->GetHeightNumber();

	//	if (pRail == m_pCur)
	//	{
	//		break;
	//	}

	//	pRail = pRail->GetNextRail();
	//	if (pRail == nullptr)
	//	{
	//		pRail = m_pCur;
	//	}
	//}
}

//====================================================================
//�I������
//====================================================================
void CRailBlock::RailDelete(void)
{
	//���g���������郌�[����S�č폜����
	CRail* pRail = m_pTop;
	while (1)
	{
		if (pRail != nullptr)
		{
			pRail->Uninit();

			pRail = pRail->GetNextRail();
		}
		else
		{
			break;
		}
	}
	m_pTop = nullptr;
	m_pCur = nullptr;
}

//====================================================================
//�I������
//====================================================================
int CRailBlock::GetRailMove(int nCnt)
{
	return m_nMove[nCnt];
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CRailBlock>* CRailBlock::GetList(void)
{
	return m_pList;
}
