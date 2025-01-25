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
#include "devil.h"
#include "texture.h"

#include "AStar.h"
#include "tile.h"
#include "wall.h"
#include "item.h"
#include "DevilHole.h"
#include "RailBlock.h"
#include "enemy.h"
#include "RollRock.h"
#include "objmeshField.h"
#include "MapMove.h"
#include "RailManager.h"

#ifdef _DEBUG
#include "objmeshField.h"
#endif

// �萔��`
namespace
{
	float GRID_SIZE = 100.0f;	// �O���b�h�̃T�C�Y
	D3DXVECTOR3 MAP_SIZE = D3DXVECTOR3(750.0f, 0.0f, 550.0f);		// ���̓����蔻��
	int BOWABOWA_RATE = 5; // �{���{���̐����� ( 0�ȉ��ŃG���[ )
	const D3DXVECTOR3 EFFECT_SIZE = { 103.5f, 25.0f, 67.5f }; // �G�t�F�N�g�̔{��
	const char* MAP_FILE = "data\\TXT\\STAGE\\stage.txt";		// �}�b�v�t�@�C��

}

//�ÓI�����o�ϐ��錾
CMapSystem* CMapSystem::m_pMapSystem = nullptr;
bool CMapSystem::m_bMapGrit[NUM_WIGHT][NUM_HEIGHT] = {false};
bool CMapSystem::m_bMapRailGrit[NUM_WIGHT][NUM_HEIGHT] = { false };

std::vector<std::tuple<>> CMapSystem::m_nData = {};	// �����̒l��ێ�
//std::vector<CMapSystem::GRID> CMapSystem::m_PosPlayer = {};	// �v���C���[�̈ʒu��ێ�
std::vector<CMapSystem::MapInfo> CMapSystem::m_MapInfo = {};	// �}�b�v�̏��

//====================================================================
//�R���X�g���N�^
//====================================================================
CMapSystem::CMapSystem() : 
	m_mapCenter(0, 0),
	m_mtxStage(nullptr)
{
	for (int nCntW = 0; nCntW < NUM_WIGHT; nCntW++)
	{
		for (int nCntH = 0; nCntH < NUM_HEIGHT; nCntH++)
		{
			m_bMapGrit[nCntW][nCntH] = false;
		}
	}
	m_WightMax = NUM_WIGHT;
	m_HeightMax = NUM_HEIGHT;
	m_fGritSize = GRID_SIZE;
	m_MapPos = D3DXVECTOR3((((NUM_WIGHT * 0.5f) * -GRID_SIZE) + GRID_SIZE * 0.5f), 0.0f, (((NUM_HEIGHT * 0.5f) * GRID_SIZE) - GRID_SIZE * 0.5f));
	m_InitPos = m_MapPos;
	m_MapSize = MAP_SIZE;
	m_MapSize = D3DXVECTOR3((NUM_WIGHT - 1) * 50.0f, 0.0f, (NUM_HEIGHT - 1) * 50.0f);
	m_pMapMove = nullptr;
	m_nNumMap = -1;
	m_nSelectMap = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMapSystem::~CMapSystem()
{
	m_MapInfo.clear();
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
			m_pMapSystem->m_bMapGrit[nCntW][nCntH] = false;
		}
	}

	if (m_pMapMove == nullptr)
	{
		m_pMapMove = CMapMove::Create();
	}

	/*m_MapPos = D3DXVECTOR3((((m_WightMax * 0.5f) * -100.0f) + m_fGritSize * 0.5f), 0.0f, (((m_HeightMax * 0.5f) * 100.0f) - m_fGritSize * 0.5f));
	m_InitPos = m_MapPos;*/

	m_MapPos = m_InitPos;

	// �o�H�T���p�̏����擾
	auto generator = AStar::Generator::Create();

	// �o�H�T���p���̐ݒ�
	generator->setWorldSize({ NUM_WIGHT, NUM_HEIGHT }); // ���E�̑傫��

	// ���S��ݒ�
	m_mapCenter = GRID(NUM_WIGHT / 2, NUM_HEIGHT / 2);

	// ���̃}�g���b�N�X���擾
	m_mtxStage = CObjmeshField::GetListTop()->GetMatrix();
}

//====================================================================
//�}�b�v�V�X�e���̏I������
//====================================================================
void CMapSystem::Uninit(void)
{
	if (m_pMapMove != nullptr)
	{
		m_pMapMove->Uninit();
		m_pMapMove = nullptr;
	}

	if (m_pMapSystem != nullptr)
	{
		delete m_pMapSystem;
		m_pMapSystem = nullptr;
	}

	AStar::Generator::GetInstance()->Uninit();
}

//====================================================================
//�}�b�v�V�X�e���̍X�V����
//====================================================================
void CMapSystem::Update(void)
{
	if (CManager::GetInstance()->GetPause() == false)
	{
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			if (CGame::GetInstance()->GetEvent() == false &&
				CGame::GetInstance()->GetTrans() == false &&
				m_pMapMove != nullptr)
			{
				//�}�b�v�̓����ݒ�
				m_pMapMove->Update();

				//���̑��I�u�W�F�N�g�̃X�N���[��
				CObject::ScrollAll();
			}
		}
		else if (CScene::GetMode() == CScene::MODE_TUTORIAL)
		{
			if (m_pMapMove != nullptr)
			{
				//�}�b�v�̓����ݒ�
				m_pMapMove->Update();

				//���̑��I�u�W�F�N�g�̃X�N���[��
				CObject::ScrollAll();
			}
		}
	}

#ifdef _DEBUG
#if 0

	// �f�o�b�O�\��
	for (int i = 0; i < m_MapGrid.x; ++i)
	{
		for (int j = 0; j < m_MapGrid.z; ++j)
		{
			if (!m_bMapGrit[i][j]) { continue; }
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_TRUE, false, useful::CalcMatrix(GRID(i, j).ToWorld(), INITVECTOR3, *CObjmeshField::GetListTop()->GetMatrix()), INITVECTOR3, {10.0f, 10.0f, 10.0f});
		}
	}

#endif // 0 or 1
#endif // _DEBUG
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

	DevilPos = CMapSystem::GetInstance()->GetMove()->GetDevilPos();

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
//  �O���b�h�̈ړ��\�t���O��ݒ�
//==========================================
void CMapSystem::SetGritBool(int nWight, int nHeight, bool Set)
{
	if (nWight < 0 || nHeight < 0) { return; }
	if (nWight >= m_MapGrid.x || nHeight >= m_MapGrid.z) { return; }
	m_bMapGrit[nWight][nHeight] = Set;
}

//==========================================
//  �O���b�h�̈ړ��\�t���O��ݒ�
//==========================================
void CMapSystem::SetGritBool(const GRID& grid, bool Set)
{
	if (grid.x < 0 || grid.z < 0) { return; }
	if (grid.x >= m_MapGrid.x || grid.z >= m_MapGrid.z) { return; }
	m_bMapGrit[grid.x][grid.z] = Set;
}

//==========================================
//  �O���b�h�̈ړ��\�t���O���擾
//==========================================
bool CMapSystem::GetGritBool(int nWight, int nHeight)
{
	if (nWight < 0 || nHeight < 0) { return false; }
	if (nWight >= m_MapGrid.x || nHeight >= m_MapGrid.z) { return false; }
	return m_bMapGrit[nWight][nHeight];
}

//==========================================
//  �O���b�h�̈ړ��\�t���O���擾
//==========================================
bool CMapSystem::GetGritBool(const GRID& grid)
{
	if (grid.x < 0 || grid.z < 0) { return false; }
	if (grid.x >= m_MapGrid.x || grid.z >= m_MapGrid.z) { return false; }
	return m_bMapGrit[grid.x][grid.z];
}

//==========================================
//  �O���b�h�̃��[���t���O��ݒ�
//==========================================
void CMapSystem::SetRailGritBool(const GRID& grid, bool Set)
{
	if (grid.x < 0 || grid.z < 0) { return; }
	if (grid.x >= m_MapGrid.x || grid.z >= m_MapGrid.z) { return; }
	m_bMapRailGrit[grid.x][grid.z] = Set;
}

//==========================================
//  �O���b�h�̃��[���t���O���擾
//==========================================
bool CMapSystem::GetRailGritBool(const GRID& grid)
{
	if (grid.x < 0 || grid.z < 0) { return false; }
	if (grid.x >= m_MapGrid.x || grid.z >= m_MapGrid.z) { return false; }
	return m_bMapRailGrit[grid.x][grid.z];
}

//==========================================
//  �P�}�X�̃T�C�Y���擾
//==========================================
float CMapSystem::GetGritSize(void)
{
	return GRID_SIZE;
}

#if 0
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
// �v���C���[�̃O���b�h�擾
//==========================================
CMapSystem::GRID CMapSystem::GetPlayerGrid(unsigned int PlayNumber)
{
	// �ʒu
	if (PlayNumber < m_PosPlayer.size())
	{ // �ǂݍ��񂾐��ȓ���������

		// ���W�����
		return m_PosPlayer[PlayNumber];
	}

	// �ςȂ��Ƃ����
	assert(false);
	return GRID();
}
#endif

//==========================================
// �v���C���[�̈ʒu�擾
//==========================================
D3DXVECTOR3 CMapSystem::GetPlayerPos(unsigned int PlayNumber, unsigned int nNumMap)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu

	// �ʒu
	if (PlayNumber < m_MapInfo[nNumMap].posPlayer.size())
	{ // �ǂݍ��񂾐��ȓ���������

		// ���W�����
		pos = m_MapInfo[nNumMap].posPlayer[PlayNumber].ToWorld();
	}
	return pos;
}

//==========================================
// �v���C���[�̃O���b�h�擾
//==========================================
CMapSystem::GRID CMapSystem::GetPlayerGrid(unsigned int PlayNumber, unsigned int nNumMap)
{
	// �ʒu
	if (PlayNumber < m_MapInfo[nNumMap].posPlayer.size())
	{ // �ǂݍ��񂾐��ȓ���������

		// ���W�����
		return m_MapInfo[nNumMap].posPlayer[PlayNumber];
	}

	// �ςȂ��Ƃ����
	assert(false);
	return GRID();
}

//==========================================
// �S�Ẵ}�b�v���̓ǂݍ���
//==========================================
HRESULT CMapSystem::LoadAll()
{
	// �t�@�C�����J��
	std::ifstream file(MAP_FILE);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�u���b�N�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �t�@�C���ǂݍ��ݗp�ϐ�
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	std::string FileName;	// �Ǎ�������
	int nNum = -1;			// �}�b�v��
	int nNumTex = 0;		// �e�N�X�`����

	// �t�@�C����Ǎ�
	std::string str, comment;	// �Ǎ�������
	while (std::getline(file, str))
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		// �J���}��؂育�ƂɃf�[�^��Ǎ�
		std::istringstream iss(str);	// ������X�g���[��
		while (std::getline(iss, str, ' '))
		{
			if (str == "NUM_MAP")
			{ // �}�b�v��

				// �}�b�v����Ǎ�
				iss >> comment >> nNum;
				pMapSystem->m_nNumMap = nNum;

			}
			else if (str == "SETFILENAME")
			{ // �}�b�v�t�@�C����

				// ���̍s�ǂݍ���
				std::getline(file, str);

				for (int nCnt = 0; nCnt < nNum; nCnt++)
				{
					std::istringstream issMapFile(str);	// ������X�g���[��
					// �}�b�v�t�@�C���̃p�X��Ǎ�
					issMapFile >> FileName;

					// �}�b�v���̓ǂݍ���
					LoadMap(FileName.c_str());

					// ���̍s�ǂݍ���
					std::getline(file, str);
				}
			}
			else if (str == "SETTEXTURE")
			{ // �}�b�v�e�N�X�`����

				// ���̍s�ǂݍ���
				std::getline(file, str);

				while (str != "END_SETTEXTURE")
				{ // �e�N�X�`���ǂݍ��݂��I���܂�
					std::istringstream issTexFile(str);	// ������X�g���[��

					// �}�b�v�t�@�C���̃p�X��Ǎ�
					issTexFile >> FileName;

					m_MapInfo[nNumTex].texture = FileName.c_str();

					// �e�N�X�`���̓ǂݍ���
					//pTexture->Regist(FileName.c_str());

					// ���̍s�ǂݍ���
					std::getline(file, str);

					nNumTex++;
				}
			}
		}
	}

	// �t�@�C�������
	file.close();

	return S_OK;
}

//==========================================
//  �}�b�v���̓ǂݍ���
//==========================================
HRESULT CMapSystem::LoadMap(const char* pFilename)
{
	// �o�H�T���p�̏����擾
	auto generator = AStar::Generator::Create();

	// �}�b�v�V�X�e���̏��
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	float fMapSystemGritSize = pMapSystem->GetGritSize() * 0.5f;

	// �ǂݍ��ݗp
	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O���b�h�����ʒu
	D3DXVECTOR3 posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O���b�h�J�n�ʒu
	D3DXVECTOR2 charOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O���b�h�̃I�t�Z�b�g
	D3DXVECTOR3 size = D3DXVECTOR3(fMapSystemGritSize, 0.0f, fMapSystemGritSize);		// �O���b�h�T�C�Y
	MAPTYPE mapType;

	// �t�@�C�����J��
	std::ifstream file(pFilename);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�u���b�N�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �t�@�C����Ǎ�
	std::string str = {};		// �Ǎ�������
	m_MapInfo.emplace_back();	// �z����g��
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
				pMapSystem->m_InitPos = posStart;		// �J�n�n�_�̐ݒ�
			}
			else if (str == "NUM_GRID")
			{
				// �O���b�h�̍s�񐔂�ǂݍ���
				iss >> pMapSystem->m_MapGrid.x >> pMapSystem->m_MapGrid.z;

				// �o�H�T���p���̐ݒ�
				generator->setWorldSize(pMapSystem->m_MapGrid.ToAStar()); // ���E�̑傫��
			}

			else if (str == "STARTSETSTAGE")
			{
				while (std::getline(file, str))
				{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

					// �I�[�̏ꍇ�X�e�[�W�����𔲂���
					if (str == "ENDSETSTAGE") { break; }

					for (int nCntHeight = 0; nCntHeight < pMapSystem->m_MapGrid.z; nCntHeight++)
					{ // ��J�E���g

						// ����s���̔z����g��
						m_nData.emplace_back();

						// �J���}��؂育�ƂɃf�[�^��Ǎ�
						std::istringstream issChar(str);	// ������X�g���[��

						for (int nCntWidth = 0; nCntWidth < pMapSystem->m_MapGrid.x; nCntWidth++)
						{ // �s�J�E���g

							// 1�s���ǂݍ���
							std::getline(issChar, str, ',');

							// �s�񐔐ݒ�
							GRID grid = GRID(nCntWidth, nCntHeight);
							if (str == "") { continue; }	// �󔒂͖�������
							else if (str == "1")
							{ // �ǂ̏ꍇ

								// ��ޕۑ�
								m_MapInfo.back().type.push_back(MAPTYPE_WALL);
								m_MapInfo.back().grid.push_back(grid);

								continue;
							}

							// ��
							mapType = MAPTYPE_NONE;

							// �I�u�W�F�N�g��ݒu
							switch (std::stoi(str))
							{
							case MAPTYPE_CROSS:		// �\����
								// ��ޕۑ�
								mapType = MAPTYPE_CROSS;
								break;

							case MAPTYPE_DEVILHOLLRANGE:		// �f�r���z�[���͈̔�
								// ��ޕۑ�
								mapType = MAPTYPE_DEVILHOLLRANGE;
								break;

							case MAPTYPE_DEVILHOLL:				// �f�r���z�[��
								// ��ޕۑ�
								mapType = MAPTYPE_DEVILHOLL;
								break;

							case MAPTYPE_RAILBLOCK:				// ���[���u���b�N
								// ��ޕۑ�
								mapType = MAPTYPE_RAILBLOCK;
								break;

							case MAPTYPE_PLAYER:				// �v���C���[
								// ��ޕۑ�
								mapType = MAPTYPE_PLAYER;

								// �v���C���[�̃O���b�h�ʒu
								m_MapInfo.back().posPlayer.push_back(grid);
								break;

							case MAPTYPE_MEDAMAN:				// ���_�}��
								// ��ޕۑ�
								mapType = MAPTYPE_MEDAMAN;
								break;

							case MAPTYPE_BONBON:				// �{���{��
								// ��ޕۑ�
								mapType = MAPTYPE_BONBON;
								break;

							case MAPTYPE_LITTLEDEVIL:			// �q�f�r��
								// ��ޕۑ�
								mapType = MAPTYPE_LITTLEDEVIL;
								break;

							case MAPTYPE_ROLLROCK:				// �]�����
								// ��ޕۑ�
								mapType = MAPTYPE_ROLLROCK;
								break;

							case MAPTYPE_RAIL:					// ���[��
								// ��ޕۑ�
								mapType = MAPTYPE_RAIL;
								break;

							default:
								break;
							}

							// ��ޕۑ�
							m_MapInfo.back().type.push_back(mapType);

							// �O���b�h�ۑ�
							m_MapInfo.back().grid.push_back(grid);
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

	return S_OK;
}

//==========================================
// �w�肳�ꂽ�}�b�v�̐���
//==========================================
HRESULT CMapSystem::CreateMap(unsigned int nSelect)
{
	// ��̏ꍇ�Ԃ�
	if (m_MapInfo.empty()) { return E_FAIL; }

	m_MapPos = m_InitPos;		// �}�b�v�̈ʒu������

	// �}�b�v�V�X�e���̏��
	CMapSystem* pMapSystem = CMapSystem::GetInstance();
	CObjmeshField* map = nullptr;
	CRailManager* pRailManager = new CRailManager();		// ���[���}�l�[�W���[�𐶐�

	// �O���b�h�ݒ�̔���
	bool bGridSet = false;
	bool bRailGridSet = false;		// ���[��

	// �o�H�T���p�̏����擾
	auto generator = AStar::Generator::Create();

	switch (CScene::GetMode())
	{// ���̐���
	case CScene::MODE_GAME:
		CGame::GetInstance()->SetMapField(CObjmeshField::Create(pMapSystem->m_MapGrid));
		map = CGame::GetInstance()->GetMapField();
		break;

	case CScene::MODE_TUTORIAL:
		CTutorial::GetInstance()->SetMapField(CObjmeshField::Create(pMapSystem->m_MapGrid));
		map = CTutorial::GetInstance()->GetMapField();
		break;

	default:
		break;
	}

	map->SetPos(INITVECTOR3);
	map->SetDisp(false); // �`����I�t

	for (int nCntHeight = 0; nCntHeight < m_MapGrid.z; nCntHeight++)
	{ // ��J�E���g
		for (int nCntWidth = 0; nCntWidth < m_MapGrid.x; nCntWidth++)
		{ // �s�J�E���g

			int nNumGrid = nCntWidth + (nCntHeight * m_MapGrid.x);
			GRID grid = m_MapInfo[nSelect].grid[nNumGrid];

			// �O���b�h����̏�����
			pMapSystem->SetGritBool(grid, false);

			// �O���b�h�̃��[������̏�����
			pMapSystem->SetRailGritBool(grid, false);

			if (m_MapInfo[nSelect].type[nNumGrid] == MAPTYPE_WALL)
			{ // �ǂ̏ꍇ
				CWall::Create(grid);
				// �O���b�h����̐ݒ�
				pMapSystem->SetGritBool(grid.x, grid.z, true);

				// �o�H�T���p���̐ݒ�
				generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�

				continue;
			}

			// ���̐���
			CTile::Create(grid);

			// �I�u�W�F�N�g�𐶐�
			switch (m_MapInfo[nSelect].type[nNumGrid])
			{
			case MAPTYPE_CROSS:		// �\����
				CItem::Create(CItem::TYPE_CROSS, grid);
				break;

			case MAPTYPE_DEVILHOLLRANGE:		// �f�r���z�[���͈̔�
				// �O���b�h�ݒ�̔���
				bGridSet = true;

				// �o�H�T���p���̐ݒ�
				generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�
				break;

			case MAPTYPE_DEVILHOLL:				// �f�r���z�[��
				// �}�b�v�̒��S�ɐݒ�
				pMapSystem->m_mapCenter = grid;
				CDevilHole::Create(grid);

				// �O���b�h�ݒ�̔���
				bGridSet = true;

				// �o�H�T���p���̐ݒ�
				generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�
				break;

			case MAPTYPE_RAILBLOCK:				// ���[���u���b�N
				CRailBlock::Create(grid);
				pRailManager->Set(grid);
				// �O���b�h�ݒ�̔���
				bGridSet = true;
				bRailGridSet = true;

				// �o�H�T���p���̐ݒ�
				generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�
				break;

			case MAPTYPE_PLAYER:				// �v���C���[
				break;

			case MAPTYPE_MEDAMAN:				// ���_�}��
				CEnemy::Create(CEnemy::ENEMY_MEDAMAN, grid);
				break;

			case MAPTYPE_BONBON:				// �{���{��
				CEnemy::Create(CEnemy::ENEMY_BONBON, grid);
				break;

			case MAPTYPE_LITTLEDEVIL:			// �q�f�r��
				CEnemy::Create(CEnemy::ENEMY_LITTLEDEVIL, grid);
				break;

			case MAPTYPE_ROLLROCK:				// ��
				CRollRock::Create(grid);
				// �O���b�h�ݒ�̔���
				bGridSet = true;

				// �o�H�T���p���̐ݒ�
				generator->addCollision(grid.ToAStar()); // �ʉߕs�n�_��ǉ�
				break;

			case MAPTYPE_RAIL:					// ���[��
				pRailManager->Set(grid);
				bRailGridSet = true;

				break;

			default:
				// �{���{���̐���
				if (BOWABOWA_RATE <= 0) { assert(false); }

				// �����_������
				if (!(rand() % BOWABOWA_RATE))
				{
					CItem::Create(CItem::TYPE_BOWABOWA, grid);
				}
				break;
			}

			// �O���b�h����̐ݒ�
			pMapSystem->SetGritBool(grid, bGridSet);

			// �O���b�h�̃��[������̐ݒ�
			pMapSystem->SetRailGritBool(grid, bRailGridSet);

			// �O���b�h�ݒ�̔���
			bGridSet = false;
			bRailGridSet = false;

		}
	}

	// ���[���̌����ݒ�
	pRailManager->Init();

	// ���[���}�l�[�W���[�̔j��
	if (pRailManager != nullptr)
	{
		pRailManager->Uninit();
		delete pRailManager;
		pRailManager = nullptr;
	}

	return S_OK;
}

//==========================================
// �}�b�v�̃��Z�b�g
//==========================================
void CMapSystem::ResetMap(void)
{
	// �ʒu��������
	m_MapPos = m_InitPos;

	bool bGridSet = false;
	bool bRailGridSet = false;

	// �O���b�h����̍Đݒ�
	for (int nCntHeight = 0; nCntHeight < m_MapGrid.z; nCntHeight++)
	{ // ��J�E���g
		for (int nCntWidth = 0; nCntWidth < m_MapGrid.x; nCntWidth++)
		{ // �s�J�E���g
			int nNumGrid = nCntWidth + (nCntHeight * m_MapGrid.x);
			GRID grid = m_MapInfo[m_nSelectMap].grid[nNumGrid];

			// �O���b�h����̏�����
			SetGritBool(grid, false);

			// �O���b�h�̃��[������̏�����
			SetRailGritBool(grid, false);

			if (m_MapInfo[m_nSelectMap].type[nNumGrid] == MAPTYPE_WALL)
			{ // �ǂ̏ꍇ
				// �O���b�h����̐ݒ�
				SetGritBool(grid.x, grid.z, true);
				continue;
			}

			// �I�u�W�F�N�g�𐶐�
			switch (m_MapInfo[m_nSelectMap].type[nNumGrid])
			{
			case MAPTYPE_DEVILHOLLRANGE:		// �f�r���z�[���͈̔�
				// �O���b�h�ݒ�̔���
				bGridSet = true;
				break;

			case MAPTYPE_DEVILHOLL:				// �f�r���z�[��
				// �}�b�v�̒��S�ɐݒ�
				m_mapCenter = grid;
				// �O���b�h�ݒ�̔���
				bGridSet = true;

				break;

			case MAPTYPE_RAILBLOCK:				// ���[���u���b�N
				// �O���b�h�ݒ�̔���
				bGridSet = true;
				bRailGridSet = true;

				break;

			case MAPTYPE_PLAYER:				// �v���C���[
				break;

			case MAPTYPE_ROLLROCK:				// ��
				// �O���b�h�ݒ�̔���
				bGridSet = true;

				break;

			case MAPTYPE_RAIL:					// ���[��
				bRailGridSet = true;

				break;

			default:
				
				break;
			}

			// �O���b�h����̐ݒ�
			SetGritBool(grid, bGridSet);

			// �O���b�h�̃��[������̐ݒ�
			SetRailGritBool(grid, bRailGridSet);

			// �O���b�h�ݒ�̔���
			bGridSet = false;
			bRailGridSet = false;
		}
	}

	// ��̃��X�g�\����������Δ�����
	if (CRollRock::GetList() == nullptr) { /*return;*/ }
	else
	{
		std::list<CRollRock*> list = CRollRock::GetList()->GetList();    // ���X�g���擾

		// ��̃��X�g�̒��g���m�F����
		for (CRollRock* pRollRock : list)
		{
			// ��̃��Z�b�g
			pRollRock->Reset();
		}
	}

	// ���[���u���b�N�̃��X�g�\����������Δ�����
	if (CRailBlock::GetList() == nullptr) { /*return;*/ }
	else
	{
		std::list<CRailBlock*> list = CRailBlock::GetList()->GetList();    // ���X�g���擾

		// ���[���u���b�N�̃��X�g�̒��g���m�F����
		for (CRailBlock* pRailBlock : list)
		{
			// ���[���u���b�N�̃��Z�b�g
			pRailBlock->Reset();
		}
	}

	// �G�̃��X�g�\����������Δ�����
	if (CEnemy::GetList() == nullptr) { }
	else
	{
		std::list<CEnemy*> list = CEnemy::GetList()->GetList();    // ���X�g���擾

		// �G�̃��X�g�̒��g���m�F����
		for (CEnemy* pEnemy : list)
		{
			// �G�̃��Z�b�g
			pEnemy->Reset();
		}
	}
}

//==========================================
//  �}�b�v�̍폜
//==========================================
void CMapSystem::MapDelete()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::OBJECT_TYPE type = pObj->GetType();			//��ނ��擾

			if (type == CObject::TYPE_TILE ||
				type == CObject::TYPE_CROSS ||
				type == CObject::TYPE_DEVILHOLE ||
				type == CObject::TYPE_RAILBLOCK ||
				type == CObject::TYPE_RAIL ||
				type == CObject::TYPE_ENEMY3D ||
				type == CObject::TYPE_ROLLROCK
				)
			{//��ނ��}�b�v�֘A�̎�
				pObj->Uninit();
			}

			pObj = pObjNext;
		}
	}
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
	D3DXVECTOR3 DevilPos = CMapSystem::GetInstance()->GetMove()->GetDevilPos();
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
