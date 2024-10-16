//============================================
//
//	�f�r���z�[���̏��� [DevilHole.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _DEVILHOLE_H_
#define _DEVILHOLE_H_

#include "main.h"
#include "objectX.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�v���C���[�N���X
class CDevilHole : public CObjectX
{
public:

	CDevilHole(int nPriority = 3);
	~CDevilHole();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};

	static CDevilHole* Create(char* pModelName);

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

private:
	void StateManager(void);	//��ԊǗ�
	void CollisionOpen(void);	//��������

	int m_nIdxXModel;				//X���f���̔ԍ�
	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	bool m_bCollision;				//�����蔻��p�̍��W
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�ϐ�
	float m_Scaling;				//�傫��
	D3DXVECTOR3 m_pos;				//�ʒu	
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu	
	D3DXVECTOR3 m_move;				//�ړ���	
	D3DXVECTOR3 m_rot;				//����	
	float m_fColorA;				//�s�����x

	bool m_bSet[4];					//�㉺���E�̌������܂��Ă��邩�ǂ���
	CObjectX *m_pHoleKey[4];			//�㉺���E�̌��𖄂߂�|���S��
};
#endif