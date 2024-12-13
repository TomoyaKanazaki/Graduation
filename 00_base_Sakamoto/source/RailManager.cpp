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
	int nMax = m_GridPos.size();
	int nCnt = 0;

	while (nCnt < nMax)
	{ // �O���b�h�̐����܂킷

		// �����Ȃ���Ԃɂ���
		CRail::POSTYPE PosType[CRail::POSSTATE_MAX] = { CRail::POSTYPE::POSTYPE_NONE, CRail::POSTYPE::POSTYPE_NONE };

		//���f���̌����ݒ�
		SetRot(m_GridPos[nCnt], PosType[CRail::POSSTATE_FIRST], PosType[CRail::POSSTATE_SECOND]);

		// ���[���̐���
		CRail::Create(m_GridPos[nCnt], PosType[CRail::POSSTATE_FIRST], PosType[CRail::POSSTATE_SECOND]);

		nCnt++;
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
	CMapSystem::GRID nNumber[CRail::POSTYPE_MAX];	// 4�����̗אڂ���O���b�h

	CMapSystem::GRID gridTemp[CRail::POSTYPE_MAX];
	for (int i = 0; i < CRail::POSTYPE_MAX;++i)
	{
		// �אڃO���b�h�̐ݒ�
		nNumber[i] = CMapSystem::GRID(grid.x + (int)sinf(D3DX_PI * 0.5f * i), grid.z + (int)cosf(D3DX_PI * 0.5f * i));
		
		// �O���b�h�ԍ��̊ۂߍ���
		nNumber[i].x = useful::RangeNumber(MaxGrid.x, 0, nNumber[i].x);
		nNumber[i].z = useful::RangeNumber(MaxGrid.z, 0, nNumber[i].z);

		// �z�u���̍X�V
		m_bRail[i] = pMapSystem->GetRailGritBool(nNumber[i]);
	}

	// �אڂS�}�X�����[���z�u�\�����f����
	//m_bRail[CRail::POSTYPE_LEFT] = pMapSystem->GetRailGritBool(CMapSystem::GRID(nNumber[CRail::POSTYPE_LEFT], grid.z));
	//m_bRail[CRail::POSTYPE_RIGHT] = pMapSystem->GetRailGritBool(CMapSystem::GRID(nNumber[CRail::POSTYPE_RIGHT], grid.z));
	//m_bRail[CRail::POSTYPE_UP] = pMapSystem->GetRailGritBool(CMapSystem::GRID(grid.x, nNumber[CRail::POSTYPE_UP]));
	//m_bRail[CRail::POSTYPE_DOWN] = pMapSystem->GetRailGritBool(CMapSystem::GRID(grid.x, nNumber[CRail::POSTYPE_DOWN]));

	// ���[���̔z�u�ꏊ�ݒ�
	for (int nCnt = 0; nCnt < CRail::POSTYPE_MAX; nCnt++)
	{
		if (!m_bRail[nCnt]) { continue; }
		else if (m_bRail[nCnt])
		{ // �z�u�\�ȏꍇ

			// �ꏊ�ݒ�
			if (PosType0 == CRail::POSTYPE_NONE)
			{ // 1��
				PosType0 = (CRail::POSTYPE)nCnt;
			}
			else if (PosType1 == CRail::POSTYPE_NONE)
			{ // 2��
				PosType1 = (CRail::POSTYPE)nCnt;
			}
		}
	}
}