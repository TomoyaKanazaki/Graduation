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

// �}�N����`
#define GRIT_SIZE (100.0f)

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
	m_fGritSize = GRIT_SIZE;
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
// �O���b�g�ʒu�̎擾
//====================================================================
D3DXVECTOR3 CMapSystem::GetGritPos(int Wight, int Height)
{
	D3DXVECTOR3 Pos;

	D3DXVECTOR3 DevilPos = CGame::GetDevil()->GetDevilPos();
	D3DXVECTOR3 DevilSize = CGame::GetDevil()->GetDevilSize();

	if (Wight < 0 || Wight >= m_WightMax ||
		Height < 0 || Height >= m_HeightMax)
	{
		return Pos;
	}

	//�@�O���b�g�̈ʒu�ɃG�t�F�N�g��\��
	Pos.x = m_MapPos.x + (Wight * m_fGritSize);

	if (Pos.x > DevilPos.x + (DevilSize.x))
	{
		Pos.x = Pos.x - (DevilPos.x + (DevilSize.x * 2.0f)) - m_fGritSize;
	}

	Pos.z = m_MapPos.z - (Height * m_fGritSize);

	if (Pos.z < DevilPos.z - (DevilSize.z))
	{
		Pos.z = Pos.z + (DevilPos.z + (DevilSize.z * 2.0f)) + m_fGritSize;
	}

	return Pos;
}