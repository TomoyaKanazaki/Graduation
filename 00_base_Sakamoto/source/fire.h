//============================================
//
//	�I�u�W�F�N�g�w���f���̃T���v�� [SampleObjX.h]
//	Author:morikawa shunya
//
//============================================
#ifndef _FIRE_H_
#define _FIRE_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//�I�u�W�F�N�g�t�@�C�A�N���X
class CFire : public CObjectX
{
public:

	CFire(int nPriority = 3);
	~CFire();

	static CFire* Create(char* pModelName, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionEnemy();
	void CollisionWall();

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	// �ÓI�����o�֐�
	static CListManager<CFire>* GetList(void); // ���X�g�擾

private:

	// �ÓI�����o�ϐ�
	static CListManager<CFire>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CFire>::AIterator m_iterator; // �C�e���[�^�[
	int m_nLife;					// �̗�
	float m_Scaling;				// �傫��
	float m_fColorA;				// �s�����x
	D3DXVECTOR3 m_move;				// �ړ���	
	CMapSystem::GRID m_Grid;		// �}�b�v�ԍ�
	CEffekseer* m_pEffect;			// �G�t�F�N�g

};
#endif