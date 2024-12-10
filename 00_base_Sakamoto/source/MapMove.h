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
class CSignalManager;
class CSignal;
class CPlayer;

//�}�b�v���[�u�N���X
class CMapMove
{
public:
	CMapMove();
	~CMapMove();

	// ���[�V����
	enum SCROLL_TYPE
	{
		SCROLL_TYPE_NORMAL = 0,	// �X���[�Y�ɃX�N���[������
		SCROLL_TYPE_RETRO,		// �J�N�J�N�ƃX�N���[������
		SCROLL_TYPE_MAX,		// �ő�
	};

	//�f�r���̏��
	enum MOVE
	{
		MOVE_WAIT = 0,		//�ҋ@
		MOVE_SCROLL_UP,		//�X�N���[��[��]
		MOVE_SCROLL_DOWN,	//�X�N���[��[��]
		MOVE_SCROLL_RIGHT,	//�X�N���[��[�E]
		MOVE_SCROLL_LEFT,	//�X�N���[��[��]
		MOVE_SLOPE_UP,		//�X�����[��]
		MOVE_SLOPE_DOWN,	//�X�����[��]
		MOVE_SLOPE_RIGHT,	//�X�����[�E]
		MOVE_SLOPE_LEFT,	//�X�����[��]
		MOVE_MAX,			//�ő�
	};

	static CMapMove* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float MoveSlopeX(float Move);		//�X�����̈ړ��ʕϓ�
	float MoveSlopeZ(float Move);		//�X�����̈ړ��ʕϓ�

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	MOVE GetState(void) { return m_State; }
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

	void FollowScroll(D3DXVECTOR3& pos) { pos += m_move; }

	// �ÓI�����o�֐�
	static CListManager<CMapMove>* GetList(void); // ���X�g�擾
	static CMapMove* GetListTop(void); // ���X�g�擾

private:

	void StateManager(void);	//��ԊǗ�
	void Move(int Arroow);		//�ړ�����
	void BackSlope(void);		//�X������
	void Slope(int Arroow);		//�X������
	void CollisionOut();		//�X�e�[�W�O�ɂ���I�u�W�F�N�g�̏���

	void SignalCreate(void);
	void SignalManager(void);

	void GritScroll();		//�O���b�g�̃X�N���[��
	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//�v���C���[���ׂ���鎞�̏���

	void DebugKey(void);		//�f�o�b�O�L�[

	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rotDest;			//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;		//�����ړ��̈ړ�����
	D3DXVECTOR3 m_DevilPos;			//�f�r���p���[���}�b�v�̃u���b�N�Ƀf�r���X�N���[������ʒu�A�e�X�g�p
	D3DXVECTOR3 m_MapDifference;	//�}�b�v�𓮂��������̍���
	float m_fActionCount;			//�s���̃J�E���g
	MOVE m_State;					//���
	MOVE m_SlopeOld;				//��O�̌X�����
	int m_nStateCount;				//��ԊǗ��p�J�E���g
	int m_nStateNum;				//��ԃJ�E���g
	bool m_bSlope;					//�X����Ԃ��ǂ���

	CMapSystem::GRID m_MinGrid;		//�}�b�v�ň�ԍ���ɂ���u���b�N�̔ԍ�
	CMapSystem::GRID m_MaxGrid;		//�}�b�v�ň�ԉE���ɂ���u���b�N�̔ԍ�

	CSignal* m_pSignal[4];			//��󃂃f���̃|�C���^

	float m_CollisionRot;			//�����蔻��p�̌���

	CSignalManager* m_pSignalManager;	// �V�O�i���}�l�[�W���[
	D3DXVECTOR3 m_DevilRot;			// �f�r���p���[�ɂ���ČX���l
	int m_DevilArrow;				// ����[0:��][1:��][2:��][3:�E]
	int m_ScrollArrowOld;			// �ߋ��̃X�N���[���̕���
	int m_SlopwArrowOld;			// �ߋ��̌X���̕���
	SCROLL_TYPE m_ScrollType;		// �X�N���[���̎��
	int m_SlopeType;				// �X���[�v�̎��
	float m_fEffectTime;			// �G�t�F�N�g�����^�C�}�[

	// �ÓI�����o�ϐ�
	static CListManager<CMapMove>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CMapMove>::AIterator m_iterator; // �C�e���[�^�[
};
#endif