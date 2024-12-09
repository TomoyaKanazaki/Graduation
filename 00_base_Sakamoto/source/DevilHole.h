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
#include "MapSystem.h"

//�O���錾
class CObjGauge2D;

//�I�u�W�F�N�g�v���C���[�N���X
class CDevilHole : public CObjectX
{
public:

	CDevilHole(int nPriority = 3);
	CDevilHole(int nPriority, CMapSystem::GRID gridCenter);
	~CDevilHole();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};

	static CDevilHole* Create(CMapSystem::GRID gridCenter);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	// �ÓI�����o�֐�
	static CListManager<CDevilHole>* GetList(void); // ���X�g�擾

private:
	void StateManager(void);	//��ԊǗ�
	void CollisionOpen(D3DXVECTOR3& pos);	//��������
	void ClearJudge(void);		//�N���A���菈��

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�

	int m_nIdxXModel;				//X���f���̔ԍ�
	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	bool m_bCollision;				//�����蔻��p�̍��W
	int m_nIdxTexture;				//�e�N�X�`���̔ԍ�
	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�ϐ�
	float m_Scaling;				//�傫��
	float m_fColorA;				//�s�����x

	bool m_bSet[4];					//�㉺���E�̌������܂��Ă��邩�ǂ���
	CObjectX *m_pHoleKey[4];			//�㉺���E�̌��𖄂߂�|���S��

	// �ÓI�����o�ϐ�
	static CListManager<CDevilHole>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CDevilHole>::AIterator m_iterator; // �C�e���[�^�[

};
#endif