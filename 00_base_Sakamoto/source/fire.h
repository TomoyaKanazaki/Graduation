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

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�v���C���[�N���X
class CFire : public CObjectX
{
public:

	CFire(int nPriority = 3);
	~CFire();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};

	static CFire* Create(char* pModelName, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void CollisionEnemy();

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:
	void StateManager(void);		// ��ԊǗ�

	STATE m_State;					// ���

	int m_nIdxXModel;				// X���f���̔ԍ�
	int m_nIdxTexture;				// �e�N�X�`���̔ԍ�
	int m_nStateCount;				// ��ԊǗ��p�ϐ�
	int m_nLife;					// �̗�

	float m_Scaling;				// �傫��
	float m_fColorA;				// �s�����x

	bool m_bCollision;				// �����蔻��p�̍��W

	D3DXVECTOR3 m_CollisionPos;		// �����蔻��p�̍��W
	D3DXVECTOR3 m_pos;				// �ʒu	
	D3DXVECTOR3 m_posOld;			// �ߋ��̈ʒu	
	D3DXVECTOR3 m_move;				// �ړ���	
	D3DXVECTOR3 m_rot;				// ����	
};
#endif