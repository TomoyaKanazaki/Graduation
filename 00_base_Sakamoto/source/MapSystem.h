//========================================================================================
//
// �}�b�v�V�X�e��[MapSystem.h]
// Author: ��{�@�ėB
//
//========================================================================================
#ifndef _MAPSYSTEM_H_
#define _MAPSYSTEM_H_

#include "main.h"
#include "manager.h"
#include "AStar.h"

class CObject2D;

//�}�N����`
#define NUM_WIGHT (23)
#define NUM_HEIGHT (15)

//�t�F�[�h�̃N���X�錾
class CMapSystem
{
public:

	// �}�b�v�̃I�u�W�F�N�g�̎��
	enum MAPTYPE
	{
		MAPTYPE_NONE = 0,		// ���ł��Ȃ�(��)
		MAPTYPE_WALL,			// ��
		MAPTYPE_CROSS,			// �\����
		MAPTYPE_DEVILHOLLRANGE,	// �f�r���z�[���̐����͈�
		MAPTYPE_DEVILHOLL,
		MAPTYPE_MAX
	};

	// ����ȍ��W�n���Ǘ�����\����
	struct GRID
	{
		// �R���X�g���N�^
		GRID() {};
		GRID(int X, int Z) { x = X; z = Z; };

		// �f�X�g���N�^
		~GRID() {};

		// �v�Z�֐�
		D3DXVECTOR3 ToWorld();
		AStar::Vec2i ToAStar();

		int x;
		int z;

		// �I�y���[�^�[
		GRID& operator = (const AStar::Vec2i& vec); // A*���C�u�����Ƃ̌݊��p
		bool operator == (const GRID& grid); // �O���b�h���m�̔�r
		bool operator != (const GRID& grid); // �O���b�h���m�̔�r
	};

	CMapSystem();
	~CMapSystem();

	static CMapSystem* GetInstance();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	// �O���b�g�ʒu�̎擾
	D3DXVECTOR3 GetStartGritPos(float Wight, float Height);
	D3DXVECTOR3 GetGritPos(const GRID& grid);
	int CalcGridX(const float posX);
	int CalcGridZ(const float posZ);
	GRID CalcGrid(const D3DXVECTOR3& pos);
	GRID GetCenter() { return m_gridCenter; };

	void SetGritBool(int nWight, int nHeight, bool Set) { m_nMapGrit[nWight][nHeight] = Set; }
	void SetGritBool(const GRID& grid, bool Set) { m_nMapGrit[grid.x][grid.z] = Set; }
	bool GetGritBool(int nWight, int nHeight) { return m_nMapGrit[nWight][nHeight]; }
	bool GetGritBool(const GRID& grid) { return m_nMapGrit[grid.x][grid.z]; }

	void SetMapPos(D3DXVECTOR3 pos) { m_MapPos = pos; }
	D3DXVECTOR3 GetMapPos(void) { return m_MapPos; }
	D3DXVECTOR3 GetInitPos(void) { return m_InitPos; }
	int GetWightMax(void) { return m_WightMax; }
	int GetHeightMax(void) { return m_HeightMax; }
	float GetGritSize(void) { return m_fGritSize; }

	void SetMapSize(D3DXVECTOR3 size) { m_MapSize = size; }
	D3DXVECTOR3 GetMapSize(void) { return m_MapSize; }

	static void Load(const char* pFilename); // �}�b�v���̓ǂݍ���

public:

	static CMapSystem* m_pMapSystem;
	static std::vector<std::tuple<>> m_nData;	// �����̒l��ێ�

	static bool m_nMapGrit[NUM_WIGHT][NUM_HEIGHT];	//�O���b�g�Ƀu���b�N�����݂��Ă�����true
	GRID m_gridCenter;
	D3DXVECTOR3 m_InitPos;
	D3DXVECTOR3 m_MapPos;
	int m_WightMax;
	int m_HeightMax;
	float m_fGritSize;
	D3DXVECTOR3 m_MapSize;		//�}�b�v�̋��E���̑傫��

	//MAPTYPE m_MapType;			// �}�b�v�I�u�W�F�N�g�̎��

};

#endif
