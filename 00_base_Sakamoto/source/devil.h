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

//�O���錾

class CObject2D;
class CObject3D;
class CObjGauge2D;
class CUltimate;
class CSlowManager;
class CObjectBillboard;
class CNumber;
class CScrollArrow;
class CSignal;

class CEnemy;
class CBoss;
class CPlayer;
class CSignalManager;
class CMapMove;

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
		ACTION_GOMI00,				// �S�~�f�[�^00
		ACTION_SIGNAL_DOWN,			// �X���M���u���v
		ACTION_SIGNAL_LEFT,			// �X���M���u���v
		ACTION_SIGNAL_RIGHT,		// �X���M���u�E�v
		ACTION_SIGNAL_UP,			// �X���M���u��v
		ACTION_RUN,					// ������
		ACTION_KING,				// �ʍ�
		ACTION_MAX,					// �ő�
	};

	enum SLOPE_TYPE
	{
		SLOPE_TYPE_FRONT = 0,
		SLOPE_TYPE_BACK,
		SLOPE_TYPE_RIGHT,
		SLOPE_TYPE_LEFT,
		SLOPE_TYPE_MAX,
	};

	static CDevil* Create();
	HRESULT Init(void) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�����z���J�n

	void SetDevilPos(D3DXVECTOR3 size) { m_DevilPos = size; }
	D3DXVECTOR3 GetDevilPos(void) { return m_DevilPos; }
	void SetDifference(D3DXVECTOR3 size) { m_MapDifference = size; }
	D3DXVECTOR3 GetDifference(void) { return m_MapDifference; }
	void SetDevilRot(D3DXVECTOR3 Rot) { m_DevilRot = Rot; }
	D3DXVECTOR3 GetDevilRot(void) { return m_DevilRot; }

	//�����z���I��

	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	CMapMove* GetMove(void) { return m_pMapMove; }

	void SetModelDisp(bool Sst);

	// �ÓI�����o�֐�
	static CListManager<CDevil>* GetList(void); // ���X�g�擾
	static CDevil* GetListTop(void); // ���X�g�擾

private:
	void ActionState(void);		//���[�V�����Ə�Ԃ̊Ǘ�

	//�����z���J�n

	void CollisionPressPlayer(CPlayer* pPlayer, D3DXVECTOR3 pos, D3DXVECTOR3 Size);	//�v���C���[���ׂ���鎞�̏���

	//�����z���I��

	ACTION_TYPE m_AtkAction;		//�U����ԋL�^�p�ϐ�
	CMapMove* m_pMapMove;

	//�����z���J�n
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rotDest;			//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;		//�����ړ��̈ړ�����
	D3DXVECTOR3 m_DevilPos;			//�f�r���p���[���}�b�v�̃u���b�N�Ƀf�r���X�N���[������ʒu�A�e�X�g�p
	D3DXVECTOR3 m_MapDifference;	//�}�b�v�𓮂��������̍���
	float m_fActionCount;			//�s���̃J�E���g
	bool m_bSlope;					//�X����Ԃ��ǂ���

	CMapSystem::GRID m_MinGrid;		//�}�b�v�ň�ԍ���ɂ���u���b�N�̔ԍ�
	CMapSystem::GRID m_MaxGrid;		//�}�b�v�ň�ԉE���ɂ���u���b�N�̔ԍ�

	CSignal* m_pSignal[4];			//��󃂃f���̃|�C���^

	float m_CollisionRot;			//�����蔻��p�̌���

	CSignalManager* m_pSignalManager;	// �V�O�i���}�l�[�W���[
	D3DXVECTOR3 m_DevilRot;			// �f�r���p���[�ɂ���ČX���l
	int m_DevilArrow;				// �f�r���p���[�̕���
	int m_ScrollArrowOld;			// �ߋ��̃X�N���[���̕���
	int m_SlopwArrowOld;			// �ߋ��̌X���̕���
	int m_SlopeType;				// �X���[�v�̎��
	//�����z���I��

	ACTION_TYPE m_Action;
	int m_nStateCount;				//��ԊǗ��p�J�E���g
	int m_nStateNum;				//��ԃJ�E���g

	CSlowManager* m_pSlow;			// �X���[

	// �ÓI�����o�ϐ�
	static CListManager<CDevil>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CDevil>::AIterator m_iterator; // �C�e���[�^�[

};

#endif