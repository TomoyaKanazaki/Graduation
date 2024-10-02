//============================================
//
//	�����蔻��L���[�u [CubeColl.h]
//	Author:���� �쏟
//
//============================================
#ifndef _CUBECOLL_H_
#define _CUBECOLL_H_

#include "main.h"
#include "objmeshCube.h"

//�I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CCubeColl : public CObjmeshCube
{
public:
	CCubeColl(int nPriority = 3);
	~CCubeColl();

	static CCubeColl* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ);

private:

};
#endif