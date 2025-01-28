//============================================
//
//	�}�b�v�̓��� [MapMove.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MAPMOVE_H_
#define _MAPMOVE_H_

#include "useful.h"
#include "MapSystem.h"

//�O���錾
class CObject2D;
class CSignal;
class CPlayer;

//�}�b�v���[�u�N���X
class CMapMove
{
public:
	CMapMove();
	~CMapMove();

	// �X�N���[���^�C�v
	enum SCROLL_TYPE
	{
		SCROLL_TYPE_NORMAL = 0,	// �X���[�Y�ɃX�N���[������
		SCROLL_TYPE_RETRO,		// �J�N�J�N�ƃX�N���[������
		SCROLL_TYPE_MAX,		// �ő�
	};

	// ����
	enum ROTTYPE
	{
		ROTTYPE_UP = 0,		// ��
		ROTTYPE_DOWN,		// ��
		ROTTYPE_LEFT,		// ��
		ROTTYPE_RIGHT,		// �E
		ROTTYPE_MAX
	};

	// �ړ����[�h
	enum MOVEMODE
	{
		MOVEMODE_SCROLL = 0,	// �X�N���[��
		MOVEMODE_SLOPE,			// �X��
		MOVEMODE_WAIT,			// �ҋ@
		MOVEMODE_MAX
	};

	//���������
	enum SPEED
	{
		SPEED_NONE = 0,		//�ҋ@
		SPEED_UP,			//����
		SPEED_DOWN,			//����
		SPEED_MAX,			//�ő�
	};



	//�ړ��̏��----------------------------------------------------------���O��������S�Ă̌���
	//enum MOVE
	//{
	//	MOVE_WAIT = 0,		//�ҋ@
	//	MOVE_SCROLL_UP,		//�X�N���[��[��]
	//	MOVE_SCROLL_DOWN,	//�X�N���[��[��]
	//	MOVE_SCROLL_LEFT,	//�X�N���[��[��]
	//	MOVE_SCROLL_RIGHT,	//�X�N���[��[�E]

	//	MOVE_SLOPE_UP,		//�X�����[��]
	//	MOVE_SLOPE_DOWN,	//�X�����[��]
	//	MOVE_SLOPE_LEFT,	//�X�����[��]
	//	MOVE_SLOPE_RIGHT,	//�X�����[�E]

	//	MOVE_MAX,			//�ő�
	//};

	static CMapMove* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float MoveSlopeX(float Move, SPEED& Speed);		//�X�����̈ړ��ʕϓ�
	float MoveSlopeZ(float Move, SPEED& Speed);		//�X�����̈ړ��ʕϓ�

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetDevilRot(D3DXVECTOR3 Rot) { m_DevilRot = Rot; }
	D3DXVECTOR3 GetDevilRot(void) { return m_DevilRot; }

	void SetScrollType(SCROLL_TYPE Rot) { m_ScrollType = Rot; }
	SCROLL_TYPE GetScrollType(void) { return m_ScrollType; }

	CMapSystem::GRID GetMinGrid(void) { return m_MinGrid; }	//�}�b�v�ň�ԍ���ɂ���u���b�N�̔ԍ�
	CMapSystem::GRID GetMaxGrid(void) { return m_MaxGrid; }	//�}�b�v�ň�ԉE���ɂ���u���b�N�̔ԍ�

	void SetStateCount(int Set) { m_nStateCount = Set; }
	int GetStateCount(void) { return m_nStateCount; }

	ROTTYPE GetRotState(void) { return m_RotType; }			// �������
	MOVEMODE GetMoveMode(void) { return m_MoveMode; }		// �ړ����

	void FollowScroll(D3DXVECTOR3& pos) { pos += m_move; }

private:

	// �ړ��p�֐�
	void StateManager(void);	//��ԊǗ�
	void Move();				//�ړ�����
	void BackSlope(void);		//�X������
	void Slope();				//�X������
	void CollisionOut();		//�X�e�[�W�O�ɂ���I�u�W�F�N�g�̏���

	// �ړ��֌W�̐ݒ�
	void SetScroll(void);		// �X�N���[���̐ݒ�
	void SetSlope(void);		// �X���̐ݒ�
	void SetBackSlope(void);	// ���ɖ߂��X���̐ݒ�
	void SetSlopeRot(ROTTYPE& RotType);		// �X���̌�����ݒ�
	void SetDeviceMap(void);	// �}�b�v���u�̐ݒ�
	void MoveScroll(void);		// �X���̉�]����

	// ���̉�
	void PlaySound();	// ���点��
	void StopSound();	// �ق点��

	void ScrollReset();		//�X�N���[���̂���C��
	void GritScroll();		//�O���b�g�̃X�N���[��

	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rotDest;			//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;		//�����ړ��̈ړ�����
	D3DXVECTOR3 m_DevilPos;			//�f�r���p���[���}�b�v�̃u���b�N�Ƀf�r���X�N���[������ʒu�A�e�X�g�p
	D3DXVECTOR3 m_MapDifference;	//�}�b�v�𓮂��������̍���
	float m_fActionCount;			//�s���̃J�E���g
	int m_nStateCount;				//��ԊǗ��p�J�E���g
	bool m_bSlope;					//�X����Ԃ��ǂ���

	float m_fScrollMove;			//�X�N���[�����ړ�������
	float m_fScrollEndLine;			//�X�N���[���I���ʒu
	bool m_bScrollOK;				//�X�N���[���������������ǂ���

	CMapSystem::GRID m_MinGrid;		//�}�b�v�ň�ԍ���ɂ���u���b�N�̔ԍ�
	CMapSystem::GRID m_MaxGrid;		//�}�b�v�ň�ԉE���ɂ���u���b�N�̔ԍ�

	float m_CollisionRot;			//�����蔻��p�̌���

	D3DXVECTOR3 m_DevilRot;			// �f�r���p���[�ɂ���ČX���l
	SCROLL_TYPE m_ScrollType;		// �X�N���[���̎��
	int m_SlopeType;				// �X���[�v�̎��
	float m_fEffectTime;			// �G�t�F�N�g�����^�C�}�[


	// �V�����ǉ������ϐ�(������)
	MOVEMODE m_MoveMode;			// �ړ����[�h
	ROTTYPE m_RotType;				// �ړ������̎��
	ROTTYPE m_OldScrollRotType;		// �O��̈ړ������̎��(�X�N���[��)
	ROTTYPE m_OldSlopeRotType;		// �O��̈ړ������̎��(�X��)

};
#endif