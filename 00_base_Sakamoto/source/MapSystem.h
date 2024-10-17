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

class CObject2D;

#define NUM_WIGHT (16)
#define NUM_HEIGHT (12)

//�t�F�[�h�̃N���X�錾
class CMapSystem
{
public:
	CMapSystem();
	~CMapSystem();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetGritPos(int nWight, int nHeight) { m_nMapGrit[nWight][nHeight] = true; }

protected:

public:
	bool m_nMapGrit[NUM_WIGHT][NUM_HEIGHT];
	D3DXVECTOR3 m_MovePos;
};

#endif
