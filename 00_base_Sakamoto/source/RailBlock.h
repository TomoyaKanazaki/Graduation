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

//�I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CRailBlock : public CCubeBlock
{
public:
	CRailBlock(int nPriority = 3);
	~CRailBlock();

	static CRailBlock* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:

private:

};
#endif