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

#include "AStar.h"
#include "CubeBlock.h"

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
	m_fGritSize = GRID_SIZE;
	m_MapPos = D3DXVECTOR3((((m_WightMax * 0.5f) * -100.0f) + m_fGritSize * 0.5f), 0.0f, (((m_HeightMax * 0.5f) * 100.0f) - m_fGritSize * 0.5f));
	m_InitPos = m_MapPos;
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

	m_MapPos = D3DXVECTOR3((((m_WightMax * 0.5f) * -100.0f) + m_fGritSize * 0.5f), 0.0f, (((m_HeightMax * 0.5f) * 100.0f) - m_fGritSize * 0.5f));
	m_InitPos = m_MapPos;

	// �o�H�T���p�̏����擾
	AStar::Generator::Create();
	auto generator = AStar::Generator::GetInstance();

	// �o�H�T���p���̐ݒ�
	generator->setWorldSize({ NUM_WIGHT, NUM_HEIGHT }); // ���E�̑傫��
	generator->setHeuristic(AStar::Heuristic::euclidean); // �ŒZ���[�g�v���̎��
	generator->setDiagonalMovement(false); // �΂߈ړ����I�t
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

	delete AStar::Generator::GetInstance();
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
D3DXVECTOR3 CMapSystem::GetGritPos(const GRID& grid)
{
	D3DXVECTOR3 Pos;

	D3DXVECTOR3 DevilPos = CGame::GetDevil()->GetDevilPos();

	// �O���b�g�ԍ����ő�l�ȏ��ŏ��l�ȉ��̎��A�͈͓��ɔ[�߂鏈��
	CMapSystem::GRID temp = grid;
	temp.x = useful::RangeNumber(m_WightMax, 0, grid.x);
	temp.z = useful::RangeNumber(m_HeightMax, 0, grid.z);

	// �O���b�g�̉��ԍ��̈ʒu��ݒ肷��
	Pos.x = m_MapPos.x + (temp.x * m_fGritSize);

	//���E���̊O���ɃO���b�g������ꍇ���Α��Ɉړ�������
	if (Pos.x > DevilPos.x + (m_MapSize.x))
	{
		Pos.x = Pos.x - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
	}

	// �����̈ʒu��ݒ肷��
	Pos.y = 0.0f;

	// �O���b�g�̏c�ԍ��̈ʒu��ݒ肷��
	Pos.z = m_MapPos.z - (temp.z * m_fGritSize);

	//���E���̊O���ɃO���b�g������ꍇ���Α��Ɉړ�������
	if (Pos.z < DevilPos.z - (m_MapSize.z))
	{
		Pos.z = Pos.z + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
	}

	return Pos;
}

//==========================================
//  �O���b�h���W�̎Z�o
//==========================================
CMapSystem::GRID CMapSystem::CalcGrid(const D3DXVECTOR3& pos)
{
	// ���Z�p�ϐ�
	GRID grid = GRID(-1, -1);
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	// x���W�̎Z�o
	for (int i = 0; i < m_WightMax; i++)
	{
		float fCountPosX = m_MapPos.x + (i * m_fGritSize);

		if (fCountPosX > DevilPos.x + (m_MapSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
		}

		if (pos.x < fCountPosX + (m_fGritSize * 0.5f) &&
			pos.x >= fCountPosX - (m_fGritSize * 0.5f))
		{
			grid.x = i;
		}
	}

	// z���W�̎Z�o
	for (int i = 0; i < m_HeightMax; i++)
	{
		float fCountPosZ = m_MapPos.z - (i * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (m_MapSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
		}

		if (pos.z < fCountPosZ + (m_fGritSize * 0.5f) &&
			pos.z >= fCountPosZ - (m_fGritSize * 0.5f))
		{
			grid.z = i;
		}
	}

	return grid;
}

//==========================================
//  �}�b�v���̓ǂݍ���
//==========================================
void CMapSystem::Load(const char* pFilename)
{
	// TODO : csv�Ή�������

	// �o�H�T���p�̏����擾
	auto generator = AStar::Generator::GetInstance();
	if (generator == nullptr)
	{
		assert(false);
		generator = AStar::Generator::Create();
	}

		//�t�@�C�����J��
	FILE* pFile = fopen(pFilename, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		char Getoff[32] = {};
		char boolLife[32] = {};
		char aString[128] = {};			//�S�~��
		char aStartMessage[32] = {};	//�X�^�[�g���b�Z�[�W
		char aSetMessage[32] = {};		//�Z�b�g���b�Z�[�W
		char aEndMessage[32] = {};		//�I�����b�Z�[�W

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			CMapSystem* pMapSystem = CMapSystem::GetInstance();
			D3DXVECTOR3 MapSystemPos = pMapSystem->GetMapPos();
			float MapSystemGritSize = pMapSystem->GetGritSize() * 0.5f;

			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETBLOCK") == 0)
				{
					int WightNumber, HeightNumber;

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &WightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%d", &HeightNumber);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%s", &aString[0]);

					pMapSystem->SetGritBool(WightNumber, HeightNumber, true);
					CCubeBlock* pBlock = CCubeBlock::Create();
					pBlock->SetWightNumber(WightNumber);
					pBlock->SetHeightNumber(HeightNumber);
					pBlock->SetPos(D3DXVECTOR3(MapSystemPos.x + WightNumber * 100.0f, 50.0f, MapSystemPos.z - HeightNumber * 100.0f));
					pBlock->SetSize(D3DXVECTOR3(MapSystemGritSize, MapSystemGritSize, MapSystemGritSize));
					pBlock->SetTexture(&aString[0]);

					// �o�H�T���p���̐ݒ�
					generator->addCollision({ WightNumber, HeightNumber }); // �ʉߕs�n�_��ǉ�

					fscanf(pFile, "%s", &aEndMessage[0]);
				}
				else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
				{
					break;
				}
			}
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}

}

//==========================================
//  �O���b�h���Z�o
//==========================================
int CMapSystem::CalcGridX(const float posX)
{
	// �Z�o�Ɏg�p����ϐ�
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	// x���W�̎Z�o
	for (int i = 0; i < m_WightMax; i++)
	{
		float fCountPosX = m_MapPos.x + (i * m_fGritSize);

		if (fCountPosX > DevilPos.x + (m_MapSize.x))
		{
			fCountPosX = fCountPosX - (DevilPos.x + (m_MapSize.x * 2.0f)) - m_fGritSize;
		}

		if (posX < fCountPosX + (m_fGritSize * 0.5f) &&
			posX >= fCountPosX - (m_fGritSize * 0.5f))
		{
			return i;
		}
	}

	// �O���b�g�O�Ȃ�-1��Ԃ�
	return -1;
}

//==========================================
//  �O���b�h���Z�o
//==========================================
int CMapSystem::CalcGridZ(const float posZ)
{
	// �Z�o�Ɏg�p����ϐ�
	CDevil* pDevil = CGame::GetDevil();
	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	for (int i = 0; i < m_HeightMax; i++)
	{
		float fCountPosZ = m_MapPos.z - (i * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (m_MapSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
		}

		if (posZ < fCountPosZ + (m_fGritSize * 0.5f) &&
			posZ >= fCountPosZ - (m_fGritSize * 0.5f))
		{
			return i;
		}
	}

	// �O���b�g�O�Ȃ�-1��Ԃ�
	return -1;
}

//==========================================
//  ������Z�q�̃I�y���[�^
//==========================================
CMapSystem::GRID& CMapSystem::GRID::operator=(const AStar::Vec2i& vec)
{
	x = vec.x;
	z = vec.y;
	return *this;
}

//==========================================
//  �O���b�h���m�̔�r���Z�q
//==========================================
bool CMapSystem::GRID::operator==(const GRID& grid)
{
	return (x == grid.x && z == grid.z);
}

//==========================================
//  �O���b�h���m�̔�r���Z�q
//==========================================
bool CMapSystem::GRID::operator!=(const GRID& grid)
{
	return (x != grid.x || z != grid.z);
}
