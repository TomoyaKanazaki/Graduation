//============================================
//
//	�f�r���̏���(��) [devil.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _DEVIL_H_
#define _DEVIL_H_

#include "object.h"
#include "useful.h"
#include "MapSystem.h"

#define SCROLL_ID 1

//�O���錾

class CCharacter;

class CObject2D;
class CObject3D;
class CObjGauge2D;
class CUltimate;
class CSlowManager;
class CObjectBillboard;
class CNumber;
class CScrollArrow;

class CEnemy;
class CBoss;
class CPlayer;

//�I�u�W�F�N�g�v���C���[�N���X
class CDevil : public CObject
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CDevil(int nPriority = 2);
	~CDevil();

	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,			// �ҋ@
		ACTION_SIGNAL_UP,			// �X���M���u��v
		ACTION_SIGNAL_DOWN,			// �X���M���u���v
		ACTION_SIGNAL_LEFT,			// �X���M���u���v
		ACTION_SIGNAL_RIGHT,		// �X���M���u�E�v
		ACTION_MAX,					// �ő�
	};

	//�f�r���̏��
	enum STATE
	{
		STATE_WAIT = 0,		//�ҋ@
		STATE_SCROLL,		//�X�N���[�����
		STATE_SLOPE,		//�X�����
		STATE_MAX,			//�ő�
	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CDevil* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void TutorialUpdate(void);
	void Draw(void);

	float MoveSlopeX(float Move);		//�X�����̈ړ��ʕϓ�
	float MoveSlopeZ(float Move);		//�X�����̈ړ��ʕϓ�

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	STATE GetState(void) { return m_State; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetDevilRot(D3DXVECTOR3 Rot) { m_DevilRot = Rot; }
	D3DXVECTOR3 GetDevilRot(void) { return m_DevilRot; }
	void SetModelDisp(bool Sst);

	// �ÓI�����o�֐�
	static CListManager<CDevil>* GetList(void); // ���X�g�擾

private:
	void ActionState(void);		//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);	//��ԊǗ�
	void Move(int Arroow);		//�ړ�����
	void BackSlope(void);		//�X������
	void Slope(int Arroow);		//�X������
	void CollisionOut();		//�X�e�[�W�O�ɂ���I�u�W�F�N�g�̏���

	void ObjectScroll(D3DXVECTOR3 Move);	//�I�u�W�F�N�g�̃X�N���[��
	void CrossScroll(D3DXVECTOR3 Move, float GritSize);		// �\���˂̃X�N���[��
	void BowabowaScroll(D3DXVECTOR3 Move, float GritSize);	// �{���{���̃X�N���[��
	void EnemyScroll(D3DXVECTOR3 Move, float GritSize);		// �G�̃X�N���[��
	void RailBlockScroll(D3DXVECTOR3 Move, float GritSize);	// ���[���u���b�N�̃X�N���[��
	void RollRockScroll(D3DXVECTOR3 Move, float GritSize);	// �]�����̃X�N���[��
	void PlayerScroll(D3DXVECTOR3 Move, float GritSize);	// �v���C���[�̃X�N���[��

	void GritScroll(D3DXVECTOR3 Move);		//�O���b�g�̃X�N���[��
	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//�v���C���[���ׂ���鎞�̏���

	void DebugKey(void);		//�f�o�b�O�L�[

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;		//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;				//����
	D3DXVECTOR3 m_rotDest;			//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;		//�����ړ��̈ړ�����
	D3DXVECTOR3 m_size;				//�傫��
	D3DXVECTOR3 m_DevilPos;			//�f�r���p���[���}�b�v�̃u���b�N�Ƀf�r���X�N���[������ʒu�A�e�X�g�p
	D3DXVECTOR3 m_MapDifference;	//�}�b�v�𓮂��������̍���
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X
	float m_fActionCount;				//�s���̃J�E���g
	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�J�E���g
	bool m_bSlope;					//�X����Ԃ��ǂ���

	CMapSystem::GRID m_MinGrid;		//�}�b�v�ň�ԍ���ɂ���u���b�N�̔ԍ�
	CMapSystem::GRID m_MaxGrid;		//�}�b�v�ň�ԉE���ɂ���u���b�N�̔ԍ�

	float m_CollisionRot;			//�����蔻��p�̌���

	CSlowManager* m_pSlow;			// �X���[
	D3DXVECTOR3 m_DevilRot;			// �f�r���p���[�ɂ���ČX���l
	int m_DevilArrow;				// �f�r���p���[�̕���
	int m_DevilArrowOld;			// �ߋ��̃f�r���p���[�̕���
	CScrollArrow* m_ScrollArrow[2];	// �X�N���[��������UI 

	CCharacter* m_pCharacter;		// �L�����N�^�[�̃N���X

	// �ÓI�����o�ϐ�
	static CListManager<CDevil>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CDevil>::AIterator m_iterator; // �C�e���[�^�[

};

#endif