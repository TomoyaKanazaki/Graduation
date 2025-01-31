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
#include "move.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
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
m_OldGrid(0, 0),
m_SaveGrid(0, 0),
m_pMoveState(nullptr)
{
	
}

//====================================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//====================================================================
CRailBlock::CRailBlock(int nPriority, CMapSystem::GRID gridCenter) : CObjectX(nPriority),
m_Grid(gridCenter),
m_OldGrid(gridCenter),
m_SaveGrid(gridCenter),
m_pMoveState(nullptr)
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
	SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

	// ����������
	CObjectX::Init(pModelName);

	// �ʒu�ݒ�
	SetPos(m_Grid.ToWorld());

	// ��ނ̐ݒ�
	SetType(TYPE_RAILBLOCK);

	// �ړ���Ԑݒ�
	if (m_pMoveState == nullptr)
	{ // �ړ���Ԑݒ�
		m_pMoveState = new CStateRoll();		// �]������
		m_pMoveState->SetRotState(CMoveState::ROTSTATE_MAX);		// �ړ������̏�Ԃ�ݒ�
	}

	if (m_pList == nullptr)
	{// ���X�g�}�l�[�W���[����
		m_pList = CListManager<CRailBlock>::Create();
		if (m_pList == nullptr) { assert(false); return E_FAIL; }
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// �X�N���[�����I���ɂ���
	SetMapScroll(true);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRailBlock::Uninit(void)
{
	// �ړ���Ԃ̔j��
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();		// �j��
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	if (m_pList != nullptr)
	{
		// ���X�g���玩�g�̃I�u�W�F�N�g���폜
		m_pList->DelList(m_iterator);

		if (m_pList->GetNumAll() == 0)
		{ // �I�u�W�F�N�g������Ȃ��ꍇ

			// ���X�g�}�l�[�W���[�̔j��
			m_pList->Release(m_pList);
		}
	}

	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CRailBlock::Update(void)
{
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 PosOld = Pos;

	// �O���b�h����ۑ�
	m_OldGrid = m_Grid;

	// �ړ�����
	Move(&Pos);

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
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �e�N���X�̕`��
	CObjectX::Draw();

	//�X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//�X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�̐ݒ� => ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 11);

	//�X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ� => 0xff(�S�Đ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	//�X�e���V���o�b�t�@�̔�r���@ => (�Q�ƒl => �X�e���V���o�b�t�@�̎Q�ƒl)�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//�X�e���V���e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// 2��ڂ̕`��
	CObjectX::Draw();

	//�X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//====================================================================
//�X�����̈ړ�����
//====================================================================
void CRailBlock::Move(D3DXVECTOR3* Pos)
{
	// �ړ�����
	m_pMoveState->Move(this, *Pos, INITVECTOR3);

	// �X�e�[�W�O�Ƃ̓����蔻��
	CollisionOut(*Pos);

	// �O���b�h����ݒ�
	m_Grid = CMapSystem::GetInstance()->CMapSystem::CalcGrid(*Pos);

	// A*�����ݒ�
	Coodinate();

	DebugProc::Print(DebugProc::POINT_RIGHT, "[���[���u���b�N] �ʒu %f : %f : %f\n", Pos->x, Pos->y, Pos->z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "[���[���u���b�N] �O���b�h�F %d : %d\n", m_Grid.x, m_Grid.z);

}

//====================================================================
// �X�e�[�W�O�Ƃ̓����蔻��
//====================================================================
void CRailBlock::CollisionOut(D3DXVECTOR3& pos)
{
	// �L���[�u�u���b�N�̃��X�g�\����������Δ�����
	if (CDevil::GetList() == nullptr) { return; }
	std::list<CDevil*> list = CDevil::GetList()->GetList();    // ���X�g���擾

	// �L���[�u�u���b�N���X�g�̒��g���m�F����
	for (CDevil* pDevil : list)
	{
		D3DXVECTOR3 MapSize = CMapSystem::GetInstance()->GetMapSize();
		float GritSize = CMapSystem::GetInstance()->GetGritSize() * 0.5f;

		DebugProc::Print(DebugProc::POINT_CENTER, "%f, %f\n\n", pos.x, pos.z);

		// z����+

		// �X�e�[�W�O�̓����蔻��
		if (MapSize.x + GritSize < pos.x) // �E
		{
			pos = m_OldGrid.ToWorld();
		}
		if (-MapSize.x - GritSize > pos.x) // ��
		{
			pos = m_OldGrid.ToWorld();
		}
		if (MapSize.z + GritSize < pos.z) // ��
		{
			pos = m_OldGrid.ToWorld();
		}
		if (-MapSize.z - GritSize > pos.z) // ��
		{
			pos = m_OldGrid.ToWorld();
		}
	}
}

//==========================================
//  A*�E�F�C�g�̕ύX����
//==========================================
void CRailBlock::Coodinate()
{
	// �O��̃O���b�h�ƍ���̃O���b�h����v���Ă���ꍇ�֐��𔲂���
	if (m_Grid == m_OldGrid) { return; }

	// �}�b�v�����擾
	CMapSystem* pMapSystem = CMapSystem::GetInstance();

	// �O��̃O���b�h���ړ��\�n�_�ɐݒ�
	pMapSystem->SetGritBool(m_OldGrid, false);

	// ���݂̃O���b�h���ړ��s�n�_�ɐݒ�
	pMapSystem->SetGritBool(m_Grid, true);
}

//==========================================
// �ړ���ԕύX����
//==========================================
void CRailBlock::ChangeMoveState(CMoveState* pMoveState)
{
	if (m_pMoveState != nullptr)
	{
		m_pMoveState->Release();
		delete m_pMoveState;
		m_pMoveState = nullptr;
	}

	m_pMoveState = pMoveState;
	m_pMoveState->Init();
}

//==========================================
// ���Z�b�g
//==========================================
void CRailBlock::Reset(void)
{
	// �O���b�h�̏�����
	m_Grid = m_SaveGrid;

	// �ʒu�ݒ�
	SetPos(m_Grid.ToWorld());
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CRailBlock>* CRailBlock::GetList(void)
{
	return m_pList;
}
