//============================================
//
//	�]�����̏��� [RollRock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _ROLLROCK_H_
#define _ROLLROCK_H_

#include "main.h"
#include "objectX.h"
#include "MapSystem.h"

//�O���錾
class CObjGauge2D;
class CMoveState;

//�]�����N���X
class CRollRock : public CObjectX
{
public:

	CRollRock(int nPriority = 3);
	CRollRock(int nPriority, CMapSystem::GRID gridCenter);
	~CRollRock();

	static CRollRock* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Reset(void);		// ���Z�b�g

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }

	// �}�b�v�ԍ��̐ݒ�
	void SetGrid(const CMapSystem::GRID& pos) override { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) override { return m_Grid; }

	void ChangeMoveState(CMoveState* pMoveState) override;   // �ړ���ԕύX

	// �ÓI�����o�֐�
	static CListManager<CRollRock>* GetList(void); // ���X�g�擾

private:
	void Move(D3DXVECTOR3& pos, D3DXVECTOR3& rot);
	void CollisionOut(D3DXVECTOR3& pos);							//�X�e�[�W�O�Ƃ̓����蔻��
	void CollisionPlayer(void);

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�
	CMapSystem::GRID m_OldGrid;	//�O���b�g�ԍ�
	CMapSystem::GRID m_SaveGrid;//�O���b�g�ԍ�(�ۑ��p)

	int m_nIdxXModel;				//X���f���̔ԍ�
	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	bool m_bCollision;				//�����蔻��p�̍��W
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	int m_nStateCount;				//��ԊǗ��p�ϐ�
	float m_Scaling;				//�傫��
	float m_fColorA;				//�s�����x

	CEffekseer* m_pEffect; // �G�t�F�N�g
	CMoveState* m_pMoveState;		// �ړ����
	CShadow* m_pShadow;				// �e

	// �ÓI�����o�ϐ�
	static CListManager<CRollRock>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CRollRock>::AIterator m_iterator; // �C�e���[�^�[
};
#endif