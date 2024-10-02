//============================================
//
//	�j�Ђ̏��� [debris.h]
//	Author:hashimoto kenta
//
//============================================
#ifndef _DEBRIS_H_
#define _DEBRIS_H_

#include "main.h"
#include "objectX.h"
#include "useful.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�v���C���[�N���X
class CDebris : public CObjectX
{
public:

	CDebris(int nPriority = 3);
	~CDebris();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};

	static CDebris* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ModelIdx);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ModelIdx);
	void Uninit();
	void Update();
	void TitleUpdate();
	void GameUpdate();
	void Draw();

	int GetIdxXModel() { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	void SetMove(D3DXVECTOR3 Move) { m_move = Move; }
	D3DXVECTOR3 GetMove() { return m_move; }

private:
	char* SetModelName();	//���f���̃p�X�����߂�
	void SetRamScaling();	//���f���̃T�C�Y�������_���Ō��߂�
	void SetRotMove();		//��]�̈ړ��ʂ�ݒ�
	void CollisionMapObject(D3DXVECTOR3 Mypos, D3DXVECTOR3 MyposOld, useful::COLLISION XYZ);	//�ǂƂ̓����蔻��

	int m_nIdxXModel;				//X���f���̔ԍ�
	float m_Scaling;				//�傫��
	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_rot;				//����
	D3DXVECTOR3 m_rotmove;			//��]�̈ړ���
};
#endif