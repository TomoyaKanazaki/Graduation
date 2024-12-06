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
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 pos) { m_move = pos; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// �ÓI�����o�֐�
	static CListManager<CRollRock>* GetList(void); // ���X�g�擾

private:
	void Move(void);
	void CollisionWall(useful::COLLISION XYZ);		//�ǂƂ̓����蔻��
	void CollisionOut();							//�X�e�[�W�O�Ƃ̓����蔻��
	void Coodinate();

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�
	CMapSystem::GRID m_OldGrid;	//�O���b�g�ԍ�
	int m_nIdxXModel;				//X���f���̔ԍ�
	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	bool m_bCollision;				//�����蔻��p�̍��W
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	int m_nStateCount;				//��ԊǗ��p�ϐ�
	float m_Scaling;				//�傫��
	D3DXVECTOR3 m_pos;				//�ʒu	
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu	
	D3DXVECTOR3 m_move;				//�ړ���	
	D3DXVECTOR3 m_rot;				//����	
	float m_fColorA;				//�s�����x

	bool m_OKL;					//���ւ̐i�s��������邩�ǂ���
	bool m_OKR;					//�E�ւ̐i�s��������邩�ǂ���
	bool m_OKU;					//��ւ̐i�s��������邩�ǂ���
	bool m_OKD;					//���ւ̐i�s��������邩�ǂ���

	CEffekseer* m_pEffect; // �G�t�F�N�g

		// �ÓI�����o�ϐ�
	static CListManager<CRollRock>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CRollRock>::AIterator m_iterator; // �C�e���[�^�[
};
#endif