//============================================
//
//	�f�r���̏���(��) [devil.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _DEVIL_H_
#define _DEVIL_H_

#include "objectcharacter.h"
#include "useful.h"
#include "MapSystem.h"

#define SCROLL_ID 0

//�O���錾

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
class CDevil : public CObjectCharacter
{
public:
	CDevil(int nPriority = 2);
	~CDevil();

	// ���[�V����
	enum ACTION_TYPE
	{
		ACTION_NEUTRAL = 0,			// �ҋ@
		ACTION_DAMAGE = 0,			// ���炭�_���[�W
		ACTION_SIGNAL_UP,			// �X���M���u��v
		ACTION_SIGNAL_DOWN,			// �X���M���u���v
		ACTION_SIGNAL_LEFT,			// �X���M���u���v
		ACTION_SIGNAL_RIGHT,		// �X���M���u�E�v
		ACTION_KING,				// �ʍ�
		ACTION_MAX,					// �ő�
	};

	// ���[�V����
	enum SCROLL_TYPE
	{
		SCROLL_TYPE_NORMAL = 0,	// �X���[�Y�ɃX�N���[������
		SCROLL_TYPE_RETRO,		// �J�N�J�N�ƃX�N���[������
		SCROLL_TYPE_MAX,		// �ő�
	};

	//�f�r���̏��
	enum STATE
	{
		STATE_WAIT = 0,		//�ҋ@
		STATE_SCROLL,		//�X�N���[�����
		STATE_SLOPE,		//�X�����
		STATE_MAX,			//�ő�
	};

	static CDevil* Create();
	HRESULT Init(void) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float MoveSlopeX(float Move);		//�X�����̈ړ��ʕϓ�
	float MoveSlopeZ(float Move);		//�X�����̈ړ��ʕϓ�

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	STATE GetState(void) { return m_State; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetDevilRot(D3DXVECTOR3 Rot) { m_DevilRot = Rot; }
	D3DXVECTOR3 GetDevilRot(void) { return m_DevilRot; }

	void SetScrollType(SCROLL_TYPE Rot) { m_ScrollType = Rot; }
	SCROLL_TYPE GetScrollType(void) { return m_ScrollType; }

	void SetModelDisp(bool Sst);

	void SetStateNumber(int nNum) { m_nStateNum = nNum; }
	int GetStateNumber(void) { return m_nStateNum; }

	// �ÓI�����o�֐�
	static CListManager<CDevil>* GetList(void); // ���X�g�擾
	static CDevil* GetListTop(void); // ���X�g�擾

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
	void FireScroll(D3DXVECTOR3 Move, float GritSize);	// �t�@�C�A�{�[���̃X�N���[��
	void TileScroll(D3DXVECTOR3 Move, float GritSize);	// ���̃X�N���[��
	void WallScroll(D3DXVECTOR3 Move, float GritSize);	// �ǂ̃X�N���[��

	void GritScroll(D3DXVECTOR3 Move);		//�O���b�g�̃X�N���[��
	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//�v���C���[���ׂ���鎞�̏���

	void DebugKey(void);		//�f�o�b�O�L�[

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;		//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rotDest;			//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;		//�����ړ��̈ړ�����
	D3DXVECTOR3 m_DevilPos;			//�f�r���p���[���}�b�v�̃u���b�N�Ƀf�r���X�N���[������ʒu�A�e�X�g�p
	D3DXVECTOR3 m_MapDifference;	//�}�b�v�𓮂��������̍���
	float m_fActionCount;			//�s���̃J�E���g
	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�J�E���g
	int m_nStateNum;				//��Ԕԍ�
	bool m_bSlope;					//�X����Ԃ��ǂ���

	CMapSystem::GRID m_MinGrid;		//�}�b�v�ň�ԍ���ɂ���u���b�N�̔ԍ�
	CMapSystem::GRID m_MaxGrid;		//�}�b�v�ň�ԉE���ɂ���u���b�N�̔ԍ�

	float m_CollisionRot;			//�����蔻��p�̌���

	CSlowManager* m_pSlow;			// �X���[
	D3DXVECTOR3 m_DevilRot;			// �f�r���p���[�ɂ���ČX���l
	int m_DevilArrow;				// �f�r���p���[�̕���
	int m_ScrollArrowOld;			// �ߋ��̃X�N���[���̕���
	int m_SlopwArrowOld;			// �ߋ��̌X���̕���
	CScrollArrow* m_ScrollArrow[2];	// �X�N���[��������UI 
	SCROLL_TYPE m_ScrollType;		// �X�N���[���̎��

	// �ÓI�����o�ϐ�
	static CListManager<CDevil>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CDevil>::AIterator m_iterator; // �C�e���[�^�[

};

#endif