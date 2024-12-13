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
#include "objectX.h"
#include "MapSystem.h"
#include "MapMove.h"
#include "game.h"
#include "objmeshField.h"

//==========================================
//  �萔��`
//==========================================
namespace
{
	const D3DXVECTOR3 SAMPLE_SIZE = D3DXVECTOR3(20.0f, 20.0f, 20.0f);		//�����蔻��
	const char* FILE_PASS = "data\\MODEL\\rail.x"; // ���f���p�X
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
	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		m_pRailModel[nCnt] = nullptr;				// ���[�����f��
		m_PosType[nCnt] = POSTYPE_NONE;				// ���[���ʒu�̎��

	}

	// ���[���̕���
	for (int nCnt = 0; nCnt < POSTYPE_MAX; nCnt++)
	{
		m_bRail[nCnt] = false;
	}

	m_pPrev = nullptr;		// �O�̃��[���ւ̃|�C���^
	m_pNext = nullptr;		// ���̃��[���ւ̃|�C���^

	// �O���b�h
	m_Grid.x = 0;
	m_Grid.z = 0;
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
CRail* CRail::Create(CMapSystem::GRID grid, POSTYPE PosType0, POSTYPE PosType1)
{
	CRail* pRail = nullptr;

	if (pRail == nullptr)
	{
		//���[���̐���
		pRail = new CRail();
	}

	pRail->m_Grid = grid;

	// ���[���̈ʒu���
	pRail->m_PosType[POSSTATE_FIRST] = PosType0;		// 1�Ԗ�
	pRail->m_PosType[POSSTATE_SECOND] = PosType1;		// 2�Ԗ�

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
	D3DXVECTOR3 pos = m_Grid.ToWorld();
	D3DXVECTOR3 rot = INITVECTOR3;

	// ���[�����f��
	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		if (m_PosType[nCnt] == POSTYPE_NONE) { continue; }		// ���[���Ɍ����������ꍇ�������Ȃ�
		else if (m_PosType[nCnt] != POSTYPE_NONE)
		{ // ���[���Ɍ���������ꍇ

			m_pRailModel[nCnt] = CObjectX::Create(FILE_PASS);	// ��������
			m_pRailModel[nCnt]->SetGrid(m_Grid);				// �O���b�h�ݒ�
			m_pRailModel[nCnt]->SetPos(pos);					// �ʒu�ݒ�
			m_pRailModel[nCnt]->SetPosOld(pos);					// �O��̈ʒu�ݒ�
			m_pRailModel[nCnt]->SetType(CObject::TYPE_RAIL);
			m_pRailModel[nCnt]->SetUseMultiMatrix(CObjmeshField::GetListTop()->GetMatrix());

			// �ݒu�������
			rot.y = -D3DX_PI * 0.5f * m_PosType[nCnt];

			// �����ݒ�
			m_pRailModel[nCnt]->SetRot(rot);
			rot = INITVECTOR3;
		}
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
	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			m_pRailModel[nCnt]->SetDeathFlag(true);
			m_pRailModel[nCnt]->Uninit();
			m_pRailModel[nCnt] = nullptr;

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
	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
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
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;			// ����

	D3DXVECTOR3 SlopeRot = CMapSystem::GetInstance()->GetMove()->GetDevilRot();		// �}�b�v�̌X��
	

	for (int nCnt = 0; nCnt < POSSTATE_MAX; nCnt++)
	{
		if (m_pRailModel[nCnt] != nullptr)
		{
			//// �擾
			//pos = m_pRailModel[nCnt]->GetPos();					// �ʒu
			//posOld = m_pRailModel[nCnt]->GetPosOld();			// �O��̈ʒu
			rot = m_pRailModel[nCnt]->GetRot();					// ����

			//// �X���ɂ��ړ��ʐݒ�
			//move.x = -SlopeRot.z;
			//move.z = SlopeRot.x;

			//pos.x += move.x;
			//pos.z += move.z;

			m_pRailModel[nCnt]->SetPos(m_Grid.ToWorld());		// �ʒu�ݒ�
			m_pRailModel[nCnt]->SetRot(rot);					// �����ݒ�

			//m_pRailModel[nCnt]->SetPosOld(posOld);				// �ʒu
			m_pRailModel[nCnt]->Update();
		}
	}

	DebugProc::Print(DebugProc::POINT_CENTER, "Rail : %d, %d\n", m_Grid.x, m_Grid.z);
}

//====================================================================
//�`�揈��
//====================================================================
void CRail::Draw(void)
{
	
}

//====================================================================
//���X�g�擾
//====================================================================
CListManager<CRail>* CRail::GetList(void)
{
	return m_pList;
}
