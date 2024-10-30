//============================================
//
//	���[���u���b�N�̏��� [RailBlock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RAILBLOCK_H_
#define _RAILBLOCK_H_

#include "main.h"
#include "CubeBlock.h"

class CRail;

//�I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CRailBlock : public CCubeBlock
{
public:
	CRailBlock(int nPriority = 3);
	~CRailBlock();

	static CRailBlock* Create(int nMapWight, int nMapHeight);

	HRESULT Init(int nMapWight, int nMapHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:

private:

	void Move(D3DXVECTOR3 *Pos);
	void RailCheck();
	void RailSet();
	void CollisionPlayer(useful::COLLISION XYZ);	//���[���u���b�N�Ƃ̓����蔻��

	int StartWightNumber;
	int StartHeightNumber;

	bool bMoveOK[4];
	CRail* m_pTop;		// �擪�̃��[���ւ̃|�C���^
	CRail* m_pCur;		// �Ō���̃��[���ւ̃|�C���^

};
#endif