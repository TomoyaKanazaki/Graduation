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

protected:
};

#endif
