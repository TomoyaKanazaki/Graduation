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
#include "MapSystem.h"

//�I�u�W�F�N�g�L���[�u�u���b�N�N���X
class CCubeBlock : public CObjmeshCube
{
public:
	CCubeBlock(int nPriority = 3);
	CCubeBlock(int nPriority, CMapSystem::GRID gridCenter, D3DXVECTOR3 GridSize);
	~CCubeBlock();

	static CCubeBlock* Create(CMapSystem::GRID gridCenter, D3DXVECTOR3 GridSize);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ);

	void SetWightNumber(int Number) { m_nMapWidthNumber = Number; }
	int GetWightNumber(void) { return m_nMapWidthNumber; }
	void SetHeightNumber(int Number) { m_nMapHeightNumber = Number; }
	int GetHeightNumber(void) { return m_nMapHeightNumber; }

	// �ÓI�����o�֐�
	static CListManager<CCubeBlock>* GetList(void); // ���X�g�擾

protected:

private:

	int m_nMapWidthNumber;
	int m_nMapHeightNumber;

	D3DXVECTOR3 m_GritSize;		// �}�b�v�̃O���b�h�T�C�Y

	// �ÓI�����o�ϐ�
	static CListManager<CCubeBlock>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CCubeBlock>::AIterator m_iterator; // �C�e���[�^�[

};
#endif