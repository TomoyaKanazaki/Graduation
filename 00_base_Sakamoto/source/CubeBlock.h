//============================================
//
//	�L���[�u�u���b�N [CubeBlock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CUBEBLOCK_H_
#define _CUBEBLOCK_H_

#include "main.h"
#include "objmeshCube.h"

//�I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CCubeBlock : public CObjmeshCube
{
public:
	CCubeBlock(int nPriority = 3);
	~CCubeBlock();

	static CCubeBlock* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ);

	void SetWightNumber(int Number) { m_nMapWightNumber = Number; }
	int GetWightNumber(void) { return m_nMapWightNumber; }
	void SetHeightNumber(int Number) { m_nMapHeightNumber = Number; }
	int GetHeightNumber(void) { return m_nMapHeightNumber; }
protected:

private:

	int m_nMapWightNumber;
	int m_nMapHeightNumber;

};
#endif