//========================================================================================
//
// �}�b�v�V�X�e��[MapSystem.cpp]
// Author: ��{�@�ėB
//
//========================================================================================
#include "MapSystem.h"
#include "renderer.h"
#include "game.h"
#include "Devil.h"

// �萔��`
namespace
{
	float GRID_SIZE = 100.0f;	// �O���b�h�̃T�C�Y
	D3DXVECTOR3 MAP_SIZE = D3DXVECTOR3(750.0f, 0.0f, 550.0f);		// ���̓����蔻��
}

//�ÓI�����o�ϐ��錾
CMapSystem* CMapSystem::pMapSystem = nullptr;
bool CMapSystem::m_nMapGrit[NUM_WIGHT][NUM_HEIGHT] = {false};

//====================================================================
//�R���X�g���N�^
//====================================================================
CMapSystem::CMapSystem()
{
	for (int nCntW = 0; nCntW < NUM_WIGHT; nCntW++)
	{
		for (int nCntH = 0; nCntH < NUM_HEIGHT; nCntH++)
		{
			m_nMapGrit[nCntW][nCntH] = false;
		}
	}
	m_WightMax = NUM_WIGHT;
	m_HeightMax = NUM_HEIGHT;
	m_MapPos = D3DXVECTOR3((m_WightMax * 0.5f) * -100.0f, 0.0f, (m_HeightMax * 0.5f) * 100.0f);
	m_InitPos = m_MapPos;
	m_fGritSize = GRID_SIZE;
	m_MapSize = MAP_SIZE;
	m_MapSize = D3DXVECTOR3((NUM_WIGHT - 1) * 50.0f, 0.0f, (NUM_HEIGHT - 1) * 50.0f);
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMapSystem::~CMapSystem()
{

}

//====================================================================
//�C���X�^���X�擾
//====================================================================
CMapSystem* CMapSystem::GetInstance(void)
{
	if (pMapSystem == nullptr)
	{
		pMapSystem = new CMapSystem;
	}
	return pMapSystem;
}

//====================================================================
//�}�b�v�V�X�e���̏���������
//====================================================================
void CMapSystem::Init()
{
	for (int nCntW = 0; nCntW < NUM_WIGHT; nCntW++)
	{
		for (int nCntH = 0; nCntH < NUM_HEIGHT; nCntH++)
		{
			pMapSystem->m_nMapGrit[nCntW][nCntH] = false;
		}
	}

	m_MapPos = D3DXVECTOR3((m_WightMax * 0.5f) * -100.0f, 0.0f, (m_HeightMax * 0.5f) * 100.0f);
	m_InitPos = m_MapPos;
}

//====================================================================
//�}�b�v�V�X�e���̏I������
//====================================================================
void CMapSystem::Uninit(void)
{
	if (pMapSystem != nullptr)
	{
		delete pMapSystem;
		pMapSystem = nullptr;
	}
}

//====================================================================
//�}�b�v�V�X�e���̍X�V����
//====================================================================
void CMapSystem::Update(void)
{

}

//====================================================================
//�}�b�v�V�X�e���̕`�揈��
//====================================================================
void CMapSystem::Draw(void)
{

}

//====================================================================
// �O���b�g�̊J�n�ʒu�擾
//====================================================================
D3DXVECTOR3 CMapSystem::GetStartGritPos(float Wight, float Height)
{
	D3DXVECTOR3 Pos;

	if (Wight < 0 || Wight >= (float)m_WightMax ||
		Height < 0 || Height >= (float)m_HeightMax)
	{
		return Pos;
	}

	//�@�O���b�g�̈ʒu�ɃG�t�F�N�g��\��
	Pos.x = m_InitPos.x + (Wight * m_fGritSize);
	Pos.y = 0.0f;
	Pos.z = m_InitPos.z - (Height * m_fGritSize);

	return Pos;
}

//====================================================================
// �O���b�g�̈ʒu�擾
//====================================================================
D3DXVECTOR3 CMapSystem::GetGritPos(int Wight, int Height)
{
	D3DXVECTOR3 Pos;

	D3DXVECTOR3 DevilPos = CGame::GetDevil()->GetDevilPos();

	//�O���b�g�ԍ����ő�l�ȏ��ŏ��l�ȉ��̎��A�͈͓��ɔ[�߂鏈��
	Wight = useful::RangeNumber(m_WightMax, 0, Wight);
	Height = useful::RangeNumber(m_HeightMax, 0, Height);

	//�@�O���b�g�̈ʒu�ɃG�t�F�N�g��\��
	Pos.x = m_MapPos.x + (Wight * m_fGritSize);

	if (Pos.x > DevilPos.x + (m_MapSize.x))
	{
		Pos.x = Pos.x - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
	}

	Pos.y = 0.0f;

	Pos.z = m_MapPos.z - (Height * m_fGritSize);

	if (Pos.z < DevilPos.z - (m_MapSize.z))
	{
		Pos.z = Pos.z + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
	}

	return Pos;
}

//====================================================================
// �O���b�g�̉��ԍ��擾
//====================================================================
int CMapSystem::GetGritWightNumber(float PosX)
{
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	for (int nCntW = 0; nCntW < m_WightMax; nCntW++)
	{
		float fCountPosX = m_MapPos.x + (nCntW * m_fGritSize);

		if (fCountPosX > DevilPos.x + (m_MapSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
		}

		if (PosX < fCountPosX + (m_fGritSize * 0.5f) &&
			PosX >= fCountPosX - (m_fGritSize * 0.5f))
		{
			return nCntW;
		}
	}

	return -1;
}

//====================================================================
// �O���b�g�̏c�ԍ��擾
//====================================================================
int CMapSystem::GetGritHeightNumber(float PosZ)
{
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	for (int nCntH = 0; nCntH < m_HeightMax; nCntH++)
	{
		float fCountPosZ = m_MapPos.z - (nCntH * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (m_MapSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
		}

		if (PosZ < fCountPosZ + (m_fGritSize * 0.5f) &&
			PosZ >= fCountPosZ - (m_fGritSize * 0.5f))
		{
			return nCntH;
		}
	}
	return -1;
}
