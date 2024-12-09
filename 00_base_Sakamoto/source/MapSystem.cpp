//========================================================================================
//
// �}�b�v�V�X�e��[MapSystem.cpp]
// Author: ��{�@�ėB
//
//========================================================================================
#include "MapSystem.h"
#include "renderer.h"
#include "game.h"
#include "tutorial.h"
#include "Devil.h"

#include "AStar.h"
#include "CubeBlock.h"
#include "tile.h"
#include "wall.h"
#include "item.h"
#include "DevilHole.h"
#include "RailBlock.h"
#include "enemy.h"
#include "RollRock.h"
#include "objmeshField.h"

// �萔��`
namespace
{
	float GRID_SIZE = 100.0f;	// �O���b�h�̃T�C�Y
	D3DXVECTOR3 MAP_SIZE = D3DXVECTOR3(750.0f, 0.0f, 550.0f);		// ���̓����蔻��
	int BOWABOWA_RATE = 5; // �{���{���̐����� ( 0�ȉ��ŃG���[ )
}

//�ÓI�����o�ϐ��錾
CMapSystem* CMapSystem::m_pMapSystem = nullptr;
bool CMapSystem::m_nMapGrit[NUM_WIGHT][NUM_HEIGHT] = {false};
std::vector<std::tuple<>> CMapSystem::m_nData = {};	// �����̒l��ێ�
std::vector<CMapSystem::GRID> CMapSystem::m_PosPlayer = {};	// �v���C���[�̈ʒu��ێ�

//====================================================================
//�R���X�g���N�^
//====================================================================
CMapSystem::CMapSystem() : 
	m_mapCenter(0, 0)
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
	m_MapPos = D3DXVECTOR3((((NUM_WIGHT * 0.5f) * -GRID_SIZE) + GRID_SIZE * 0.5f), 0.0f, (((NUM_HEIGHT * 0.5f) * GRID_SIZE) - GRID_SIZE * 0.5f));
	m_InitPos = m_MapPos;
	m_MapSize = MAP_SIZE;
	m_MapSize = D3DXVECTOR3((NUM_WIGHT - 1) * 50.0f, 0.0f, (NUM_HEIGHT - 1) * 50.0f);
	//m_MapType = MAPTYPE_NONE;			// �}�b�v�I�u�W�F�N�g�̎��
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
	if (m_pMapSystem == nullptr)
	{
		m_pMapSystem = new CMapSystem;
		m_pMapSystem->Init();
	}
	return m_pMapSystem;
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
			m_pMapSystem->m_nMapGrit[nCntW][nCntH] = false;
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

	// ���S��ݒ�
	m_mapCenter = GRID(NUM_WIGHT / 2, NUM_HEIGHT / 2);
}

//====================================================================
//�}�b�v�V�X�e���̏I������
//====================================================================
void CMapSystem::Uninit(void)
{
	if (m_pMapSystem != nullptr)
	{
		delete m_pMapSystem;
		m_pMapSystem = nullptr;
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
	D3DXVECTOR3 DevilPos;

	DevilPos = CDevil::GetListTop()->GetDevilPos();

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

	CDevil* pDevil = nullptr;

	pDevil = CDevil::GetListTop();

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
//  �P�}�X�̃T�C�Y���擾
//==========================================
float CMapSystem::GetGritSize(void)
{
	return GRID_SIZE;
}

//==========================================
// �v���C���[�̈ʒu�擾
//==========================================
D3DXVECTOR3 CMapSystem::GetPlayerPos(unsigned int PlayNumber)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu

	// �ʒu
	if (PlayNumber < m_PosPlayer.size())
	{ // �ǂݍ��񂾐��ȓ���������
		
		// ���W�����
		pos = m_PosPlayer[PlayNumber].ToWorld();
	}
	return pos;
}

//==========================================
//  �}�b�v���̓ǂݍ���
//==========================================
void CMapSystem::Load(const char* pFilename)
{
	// �o�H�T���p�̏����擾
	auto generator = AStar::Generator::GetInstance();
	if (generator == nullptr)
	{
		assert(false);
		generator = AStar::Generator::Create();
	}

	// �}�b�v�V�X�e���̏��
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	float fMapSystemGritSize = pMapSystem->GetGritSize() * 0.5f;

	// �ǂݍ��ݗp
	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �O���b�h�����ʒu
	D3DXVECTOR3 posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O���b�h�J�n�ʒu
	D3DXVECTOR2 charOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O���b�h�̃I�t�Z�b�g
	D3DXVECTOR3 size = D3DXVECTOR3(fMapSystemGritSize, 0.0f, fMapSystemGritSize);		// �O���b�h�T�C�Y
	GRID MaxGrid;		// �O���b�h�̍ő吔

	// �O���b�h�ݒ�̔���
	bool bGridSet = false;

	// �t�@�C�����J��
	std::ifstream file(pFilename);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�u���b�N�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return;
	}

	// �t�@�C����Ǎ�
	std::string str;	// �Ǎ�������
	while (std::getline(file, str))
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		// �J���}��؂育�ƂɃf�[�^��Ǎ�
		std::istringstream iss(str);	// ������X�g���[��
		while (std::getline(iss, str, ','))
		{
			if (str == "START_POS")
			{
				// �J�n�ʒu��Ǎ�
				iss >> posStart.x >> posStart.y >> posStart.z;

				// �J�n�ʒu�𐶐��ʒu�ɐݒ�
				posOffset = posStart;
				pMapSystem->m_MapPos = posStart;		// �}�b�v�̈ʒu�ɐݒ�
			}
			else if (str == "NUM_GRID")
			{
				// �O���b�h�̍s�񐔂�ǂݍ���
				iss >> MaxGrid.x >> MaxGrid.z;

				CObjmeshField* map = nullptr;
				//���̐���
				if (CScene::GetMode() == CScene::MODE_GAME)
				{
					CGame::GetInstance()->SetMapField(CObjmeshField::Create(MaxGrid));
					map = CGame::GetInstance()->GetMapField();
				}
				else if (CScene::GetMode() == CScene::MODE_TUTORIAL)
				{
					CTutorial::GetInstance()->SetMapField(CObjmeshField::Create(MaxGrid));
					map = CTutorial::GetInstance()->GetMapField();
				}
				map->SetPos(INITVECTOR3);
				map->SetAlpha(0.0f); // �`����I�t

				// �o�H�T���p���̐ݒ�
				generator->setWorldSize(MaxGrid.ToAStar()); // ���E�̑傫��
			}

			else if (str == "STARTSETSTAGE")
			{
				while (std::getline(file, str))
				{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

					// �I�[�̏ꍇ�X�e�[�W�����𔲂���
					if (str == "ENDSETSTAGE") { break; }

					for (int nCntHeight = 0; nCntHeight < MaxGrid.z; nCntHeight++)
					{ // ��J�E���g

						// ����s���̔z����g��
						m_nData.emplace_back();

						// �J���}��؂育�ƂɃf�[�^��Ǎ�
						std::istringstream issChar(str);	// ������X�g���[��

						for (int nCntWidth = 0; nCntWidth < MaxGrid.x; nCntWidth++)
						{ // �s�J�E���g

							// 1�s���ǂݍ���
							std::getline(issChar, str, ',');

							// �s�񐔐ݒ�
							GRID grid = GRID(nCntWidth, nCntHeight);
							if (str == "") { continue; }	// �󔒂͖�������
							else if (str == "1")
							{ // �ǂ̏ꍇ

								// �ǃ��f���̐���
								CWall::Create(grid);

								// �O���b�h����̐ݒ�
								pMapSystem->SetGritBool(grid.x, grid.z, true);

								// �o�H�T���p���̐ݒ�
								generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�

								continue;
							}

							// �����f���̐���
							CTile::Create(grid);

							// �O���b�h�ݒ�̔���
							bGridSet = false;

							// �I�u�W�F�N�g��ݒu
							if (str == "2")
							{ // �\����

								// �\���˂̐���
								CItem::Create(CItem::TYPE_CROSS, grid);
							}
							else if (str == "3")
							{ // �f�r���z�[���̐����͈�

								// �O���b�h�ݒ�̔���
								bGridSet = true;

								// �o�H�T���p���̐ݒ�
								generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�
							}
							else if (str == "4")
							{ // �f�r���z�[��

								// �f�r���z�[���̐���
								CDevilHole::Create(grid);

								// �}�b�v�̒��S�ɐݒ�
								pMapSystem->m_mapCenter = grid;

								// �O���b�h�ݒ�̔���
								bGridSet = true;

								// �o�H�T���p���̐ݒ�
								generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�
							}
							else if (str == "5")
							{ // ���[���u���b�N

								// ���[���u���b�N�̐���
								CRailBlock::Create(grid);

								// �O���b�h�ݒ�̔���
								bGridSet = true;

								// �o�H�T���p���̐ݒ�
								generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�
							}
							else if (str == "6")
							{ // �v���C���[

								// �v���C���[�̃O���b�h�ʒu
								pMapSystem->m_PosPlayer.push_back(grid);
							}
							else if (str == "7")
							{ // �G(���_�}��)

								// ���_�}������
								CEnemy::Create(CEnemy::ENEMY_MEDAMAN, grid);

							}
							else if (str == "8")
							{ // �G(�{���{��)

								// �{���{������
								CEnemy::Create(CEnemy::ENEMY_BONBON, grid);

							}
							else if (str == "9")
							{ // �G(�q�f�r��)

								// �q�f�r������
								CEnemy::Create(CEnemy::ENEMY_LITTLEDEVIL, grid);

							}
							else if (str == "10")
							{ // �]�����

								// �]����␶��
								CRollRock::Create(grid);

								// �O���b�h�ݒ�̔���
								bGridSet = true;

								// �o�H�T���p���̐ݒ�
								generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�
							}
							else
							{ // �{���{���̐���
								if (BOWABOWA_RATE <= 0) { assert(false); }

								// �����_������
								if (!(rand() % BOWABOWA_RATE))
								{
									CItem::Create(CItem::TYPE_BOWABOWA, grid);
								}
							}

							// �O���b�h����̐ݒ�
							pMapSystem->SetGritBool(grid, bGridSet);

						}

						// ���̍s�ǂݍ���
						std::getline(file, str);
					}

				}
			}
		}
	}

	// �t�@�C�������
	file.close();
}

//==========================================
//  �O���b�h���Z�o
//==========================================
int CMapSystem::CalcGridX(const float posX)
{
	// �Z�o�Ɏg�p����ϐ�
	CDevil* pDevil = nullptr;

	pDevil = CDevil::GetListTop();

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
	CDevil* pDevil = nullptr;

	pDevil = CDevil::GetListTop();

	D3DXVECTOR3 DevilPos = pDevil->GetDevilPos();

	for (int i = 0; i < m_HeightMax; i++)
	{
		float fCountPosZ = m_MapPos.z - (i * m_fGritSize);

		if (fCountPosZ < DevilPos.z - (m_MapSize.z))
		{
			fCountPosZ = fCountPosZ + (DevilPos.z + (m_MapSize.z * 2.0f)) + m_fGritSize;
		}

		if (posZ <= fCountPosZ + (m_fGritSize * 0.5f) &&
			posZ > fCountPosZ - (m_fGritSize * 0.5f))
		{
			return i;
		}
	}

	// �O���b�g�O�Ȃ�-1��Ԃ�
	return -1;
}

//==========================================
//  �O���b�h���W�𐢊E�̍��W�ɕϊ�����
//==========================================
D3DXVECTOR3 CMapSystem::GRID::ToWorld()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 DevilPos = CDevil::GetListTop()->GetDevilPos();
	CMapSystem* map = GetInstance();

	// �O���b�g�ԍ����ő�l�ȏ��ŏ��l�ȉ��̎��A�͈͓��ɔ[�߂鏈��
	CMapSystem::GRID temp = GRID(x, z);
	temp.x = useful::RangeNumber(map->m_WightMax, 0, x);
	temp.z = useful::RangeNumber(map->m_HeightMax, 0, z);

	// �O���b�g�̉��ԍ��̈ʒu��ݒ肷��
	pos.x = map->m_MapPos.x + (temp.x * map->m_fGritSize);

	//���E���̊O���ɃO���b�g������ꍇ���Α��Ɉړ�������
	if (pos.x > DevilPos.x + (map->m_MapSize.x))
	{
		pos.x = pos.x - (DevilPos.x + (map->m_MapSize.x * 2.0f)) - map->m_fGritSize;
	}

	// �����̈ʒu��ݒ肷��
	pos.y = 0.0f;

	// �O���b�g�̏c�ԍ��̈ʒu��ݒ肷��
	pos.z = map->m_MapPos.z - (temp.z * map->m_fGritSize);

	//���E���̊O���ɃO���b�g������ꍇ���Α��Ɉړ�������
	if (pos.z < DevilPos.z - (map->m_MapSize.z))
	{
		pos.z = pos.z + (DevilPos.z + (map->m_MapSize.z * 2.0f)) + map->m_fGritSize;
	}

	return pos;
}

//==========================================
//  A*�p�̍��W�n�ɕϊ�����
//==========================================
AStar::Vec2i CMapSystem::GRID::ToAStar()
{
	// �ϐ��錾
	AStar::Vec2i vec;

	// �l����
	vec.x = x;
	vec.y = z;

	return vec;
}

//==========================================
//  �O���b�h���W�ɕϊ�����
//==========================================
CMapSystem::GRID CMapSystem::GRID::ToGrid(const D3DXVECTOR3& pos)
{
	return CMapSystem::GetInstance()->CalcGrid(pos);
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
