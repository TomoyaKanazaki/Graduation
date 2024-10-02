//============================================
//
//	�}�b�v���f�� [MapModel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MAPMODEL_H_
#define _MAPMODEL_H_

#include "main.h"
#include "objectX.h"

//�O���錾
class CObjGauge2D;
class CCubeEffect;

//==========================
// �萔��`
//==========================
namespace mapmodel_const
{
	const int SCREENPOS_NUM = 8;	//�X�N���[�����W�p���W��
}

//==============================
//�I�u�W�F�N�g�v���C���[�N���X
//==============================
class CMapModel : public CObjectX
{
public:

	CMapModel(int nPriority = 3);
	~CMapModel();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};

	static CMapModel* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void SetCollision(bool Set) { m_bCollision = Set; }
	bool GetCollision(void) { return m_bCollision; }
	void CreateBlock(void);		//�f�o�b�O�p�u���b�N�̐���
	void UpdateBlock(void);		//�f�o�b�O�p�u���b�N�̍X�V
	void SetEditIdx(int Idx) { m_EditIndex = Idx; }
	int GetEditIdx(void) { return m_EditIndex; }
	D3DXVECTOR3* GetScreenPos(void) { return &m_ScreenPos[0]; }
	void SetCollisionSize(D3DXVECTOR3 size) { collisionSize = size; }
	D3DXVECTOR3 GetCollisionSize() { return collisionSize; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove() { return m_move; }
	void SetSpin(D3DXVECTOR3 spin) { m_Spin = spin; }
	D3DXVECTOR3 GetSpin() { return m_Spin; }
	void SetDebugBlock(CCubeEffect* pCubeEffect) { m_pDebugBlock = pCubeEffect; }
	CCubeEffect*GetDebugBlock() { return m_pDebugBlock; }
	void SetBreakable(bool set) { m_bBreakable = set; }
	bool GetBreakable() { return m_bBreakable; }
	void SetDebris(bool set) { m_bDebris = set; }
	bool GetDebris() { return m_bDebris; }
	void SetBreakEffect(bool set) { m_bBreakEffect = set; }
	bool GetBreakEffect() { return m_bBreakEffect; }

	virtual void Break(D3DXVECTOR3 *pos);	//�U���ɂ���ĉ󂳂ꂽ�Ƃ�
	void ScreenCollision();

private:
	void StateManager(void);	//��ԊǗ�

	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	bool m_bCollision;				//�����蔻�肪���邩�ǂ���
	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�ϐ�
	float m_Scaling;				//�傫��
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu	
	D3DXVECTOR3 m_move;				//�ړ���	
	D3DXVECTOR3 m_Spin;				//��]��	
	float m_fColorA;				//�s�����x
	int m_EditIndex;				//�G�f�B�b�g���̃C���f�b�N�X
	D3DXVECTOR3 m_ScreenPos[mapmodel_const::SCREENPOS_NUM];	//�X�N���[�����W
	D3DXVECTOR3 collisionSize;					//��`�̓����蔻��T�C�Y
	bool m_bBreakable;		//���鑶�݂�
	bool m_bDebris;			//�j�Ђ��o�邩�ǂ���
	bool m_bBreakEffect;	//�j��G�t�F�N�g�̏�Ԃ��ǂ���
	int m_bBreakBound;		//�j��G�t�F�N�g��ԂŃo�E���h������

	CCubeEffect* m_pDebugBlock;		//�f�o�b�O�p�u���b�N
};
#endif