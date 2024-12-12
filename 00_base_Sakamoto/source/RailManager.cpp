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
#include "MapSystem.h"
#include "Rail.h"

//==========================================
//  �萔��`
//==========================================
namespace
{

}

//====================================================================
//�ÓI�����o�ϐ��錾
//====================================================================
CListManager<CRailManager>* CRailManager::m_pList = nullptr;	// �I�u�W�F�N�g���X�g
std::vector<CMapSystem::GRID> CRailManager::m_GridPos = {};		// ���[���̈ʒu��ێ�

//====================================================================
//�R���X�g���N�^
//====================================================================
CRailManager::CRailManager()
{
	
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CRailManager::~CRailManager()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CRailManager::Init(CMapSystem::GRID& grid)
{
	// ���[���̈ʒu��ێ�
	m_GridPos.push_back(grid);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRailManager::Uninit(void)
{
	
}

//====================================================================
//�X�V����
//====================================================================
void CRailManager::Update(void)
{
	
}

//====================================================================
//�`�揈��
//====================================================================
void CRailManager::Draw(void)
{

}

//====================================================================
// ���[���̐ݒ�
//====================================================================
void CRailManager::Set(void)
{
	// �����Ȃ���Ԃɂ���
	CRail::POSTYPE PosType[CRail::POSSTATE_MAX];

	for (int nCnt = 0; nCnt < CRail::POSSTATE_MAX; nCnt++)
	{
		PosType[nCnt] = CRail::POSTYPE_NONE;
	}

	int nMax = m_GridPos.size();
	int nCnt = 0;

	while (nCnt <= nMax)
	{ // �O���b�h�̐����܂킷

		//���f���̌����ݒ�
		SetRot(m_GridPos[nCnt], PosType[CRail::POSSTATE_FIRST], PosType[CRail::POSSTATE_SECOND]);

		// ���[���̐���
		CRail::Create(m_GridPos[nCnt], PosType[CRail::POSSTATE_FIRST], PosType[CRail::POSSTATE_SECOND]);
	}
}

//====================================================================
// ���f���̌����̐ݒ�
//====================================================================
void CRailManager::SetRot(CMapSystem::GRID& grid, CRail::POSTYPE& PosType0, CRail::POSTYPE& PosType1)
{
	CMapSystem* pMapSystem = CMapSystem::GetInstance(); // �}�b�v�V�X�e���̃C���X�^���X���擾
	CMapSystem::GRID MaxGrid;
	MaxGrid.x = pMapSystem->GetWightMax();	// �}�b�v�̉���
	MaxGrid.z = pMapSystem->GetHeightMax(); // �}�b�v�̗���

	/* ���g�̗אڂS�}�X�̃O���b�h */
	int nNumber[CRail::POSTYPE_MAX];	// 4�����̗אڂ���O���b�h
	nNumber[CRail::POSTYPE_LEFT] = grid.x - 1;		// ��
	nNumber[CRail::POSTYPE_RIGHT] = grid.x + 1;		// �E
	nNumber[CRail::POSTYPE_UP] = grid.z - 1;		// ��
	nNumber[CRail::POSTYPE_DOWN] = grid.z + 1;		// ��

	// �O���b�h�ʒu�̕␳
	nNumber[CRail::POSTYPE_LEFT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[CRail::POSTYPE_LEFT]);
	nNumber[CRail::POSTYPE_RIGHT] = useful::RangeNumber(MaxGrid.x, 0, nNumber[CRail::POSTYPE_RIGHT]);
	nNumber[CRail::POSTYPE_UP] = useful::RangeNumber(MaxGrid.z, 0, nNumber[CRail::POSTYPE_UP]);
	nNumber[CRail::POSTYPE_DOWN] = useful::RangeNumber(MaxGrid.z, 0, nNumber[CRail::POSTYPE_DOWN]);

	// �אڂS�}�X�����[���z�u�\�����f����
	m_bRail[CRail::POSTYPE_LEFT] = pMapSystem->GetRailGritBool(CMapSystem::GRID(nNumber[CRail::POSTYPE_LEFT], grid.z));
	m_bRail[CRail::POSTYPE_RIGHT] = pMapSystem->GetRailGritBool(CMapSystem::GRID(nNumber[CRail::POSTYPE_RIGHT], grid.z));
	m_bRail[CRail::POSTYPE_UP] = pMapSystem->GetRailGritBool(CMapSystem::GRID(grid.x, nNumber[CRail::POSTYPE_UP]));
	m_bRail[CRail::POSTYPE_DOWN] = pMapSystem->GetRailGritBool(CMapSystem::GRID(grid.x, nNumber[CRail::POSTYPE_DOWN]));

	// ���[���̔z�u�ꏊ�ݒ�
	for (int nCnt = CRail::POSTYPE_UP; nCnt < CRail::POSTYPE_MAX; nCnt++)
	{
		if (m_bRail[nCnt])
		{ // �z�u�\�ȏꍇ

			// �ꏊ�ݒ�
			if (PosType0 == CRail::POSTYPE_NONE)
			{ // 1��
				switch (nCnt)
				{
				case CRail::POSTYPE_UP:		// ��
					PosType0 = CRail::POSTYPE_UP;
					break;

				case CRail::POSTYPE_DOWN:	// ��
					PosType0 = CRail::POSTYPE_DOWN;
					break;

				case CRail::POSTYPE_LEFT:	// ��
					PosType0 = CRail::POSTYPE_LEFT;
					break;

				case CRail::POSTYPE_RIGHT:	// �E
					PosType0 = CRail::POSTYPE_RIGHT;
					break;

				default:
					break;
				}

			}
			else if (PosType1 == CRail::POSTYPE_NONE)
			{ // 2��
				switch (nCnt)
				{
				case CRail::POSTYPE_UP:		// ��
					PosType1 = CRail::POSTYPE_UP;
					break;

				case CRail::POSTYPE_DOWN:	// ��
					PosType1 = CRail::POSTYPE_DOWN;
					break;

				case CRail::POSTYPE_LEFT:	// ��
					PosType1 = CRail::POSTYPE_LEFT;
					break;

				case CRail::POSTYPE_RIGHT:	// �E
					PosType1 = CRail::POSTYPE_RIGHT;
					break;

				default:
					break;
				}
			}

		}
	}
}