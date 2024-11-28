//============================================
//
//	�v���C���[�̏��� [Player.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "objectcharacter.h"
#include "useful.h"
#include "Model.h"
#include "MapSystem.h"

//�O���錾
class CSlowManager;
class CLifeUi;
class CObjectX;
class CScore;
class CObjmeshField;
class CDevil;

//�I�u�W�F�N�g�v���C���[�N���X
class CPlayer : public CObjectCharacter
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CPlayer(int nPriority = 2);
	~CPlayer();

	//�v���C���[�̃��[�V����
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,		// �ҋ@
		ACTION_MOVE,			// �ړ�
		ACTION_EGG,				// �����(�������O�O
		ACTION_DEATH,			// ���S���(�������O�Q
		ACTION_MAX,				// �ő�
		ACTION_NONE
	};

	//�v���C���[�̏��
	enum STATE
	{
		STATE_WAIT = 0,		//�ҋ@
		STATE_WALK,			//����
		STATE_DEATH,		//���S
		STATE_EGG,			//��
		STATE_ATTACK,		//�U��
		STATE_MAX,			//�ő�
		STATE_NONE
	};

	// �v���C���[�̈ړ�����
	enum MOVE_STATE
	{
		MOVE_STATE_WAIT = 0,	// �ҋ@
		MOVE_STATE_LEFT,		// ������
		MOVE_STATE_RIGHT,		// �E����
		MOVE_STATE_UP,			// �����
		MOVE_STATE_DOWN,		// ������
		MOVE_STATE_MAX,			// �ő�
		MOVE_STATE_NONE
	};

	// �A�C�e���̎��
	enum ITEM_TYPE
	{
		TYPE_NONE = 0,		// �����Ȃ�
		TYPE_CROSS,			// �\����
		TYPE_BIBLE,			// ����
		TYPE_MAX
	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CPlayer* Create(int PlayNumber);
	HRESULT Init(int PlayNumber) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPlayNumber(int Number) { m_nPlayNumber = Number; }
	int GetPlayNumber(void) { return m_nPlayNumber; }

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetMoveX(float moveX) { m_move.x = moveX; }
	void SetMoveZ(float moveZ) { m_move.z = moveZ; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetObjMoveX(float move) { m_Objmove.x = move; }
	void SetObjMoveZ(float move) { m_Objmove.z = move; }
	D3DXVECTOR3 GetObjMove(void) { return m_Objmove; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	float GetHeight(void) { return m_size.y; }
	void SetHeight(float fHeight) { m_size.y = fHeight; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }
	bool GetJump(void) { return m_bJump; }
	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetModelDisp(bool Sst);
	void SetPartsDisp(int nParts, bool Set);
	void SetModelColor(CModel::COLORTYPE Type, D3DXCOLOR Col);
	void SetUseMultiMatrix(D3DXMATRIX *Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX *GetUseMultiMatrix(void) { return m_UseMultiMatrix; }
	void SetPressObj(bool Set) { m_bPressObj = Set; }
	bool GetPressObj(void) { return m_bPressObj; }
	CScore* GetScore(void) { return m_pScore; }

	void SetLife(int Set) { m_nLife = Set; }
	int GetLife(void) { return m_nLife; }

	void SetStateCount(int Set) { m_nStateCount = Set; }
	int GetStateCount(void) { return m_nStateCount; }

	CLifeUi* GetLifeUI(void) { return m_pLifeUi; }

	void SetUseItem(bool bUse) { m_UseItem = bUse; }
	bool GetbUseItem() { return m_UseItem; }

	bool GetGritCenter() { return m_bGritCenter; }

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	virtual void Death(void);
	bool SortObject(D3DXVECTOR3 pos);					// �I�u�W�F�N�g�Ƃ̃\�[�g����

	void SetItemType(ITEM_TYPE eType);
	ITEM_TYPE GetItemType() { return m_eItemType; }		// �A�C�e���̎�ގ擾

	void ResetCrossTimer() { m_fCrossTimer = 0.0f; } // �\���ˏ������Ԃ̏�����

	// �ÓI�����o�֐�
	static CListManager<CPlayer>* GetList(void);	// ���X�g�擾

private:

	void ActionState(void);							//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);						//��ԊǗ�
	void Move(void);								//�ړ�����
	D3DXVECTOR3 MoveInputKey(D3DXVECTOR3 Move);		//�ړ����̓L�[�{�[�h
	D3DXVECTOR3 MoveInputPadStick(D3DXVECTOR3 Move);//�ړ����̓p�b�h�X�e�B�b�N
	D3DXVECTOR3 MoveInputPadKey(D3DXVECTOR3 Move);	//�ړ����̓p�b�h�L�[
	void Rot(void);									//�ړ���������
	void Attack(void);								//�U������
	void CollisionWall(useful::COLLISION XYZ);		//�ǂƂ̓����蔻��
	void CollisionPressWall(useful::COLLISION XYZ);	//�ǂƂ̈�������
	void CollisionWaitRailBlock(useful::COLLISION XYZ);	//�~�܂��Ă��郌�[���u���b�N�Ƃ̓����蔻��
	void CollisionMoveRailBlock(useful::COLLISION XYZ);	//�����Ă��郌�[���u���b�N�Ƃ̓����蔻��
	void CollisionWaitRock(useful::COLLISION XYZ);	//�~�܂��Ă����Ƃ̓����蔻��
	void CollisionMoveRock(useful::COLLISION XYZ);	//�����Ă����Ƃ̓����蔻��
	void SearchWall(void);							//�ǂ̃T�[�`����
	void CollisionDevilHole(useful::COLLISION XYZ);	//�f�r���z�[���Ƃ̓����蔻��
	void CollisionEnemy(void);						// �G�Ƃ̓����蔻��
	void CollisionStageOut(void);					// �X�e�[�W�O�̓����蔻��
	bool CollisionStageIn(void);					// �X�e�[�W���ɂ��邩�ǂ���
	void CollisionPressStageOut(void);				// �X�e�[�W�O�̈�������

	void CameraPosUpdate(void);	//�J�����ʒu�X�V����
	void PosUpdate(void);		//�ʒu�X�V����
	void ObjPosUpdate(void);	//�I�u�W�F�N�g�ɂ��ʒu�X�V����
	void RotUpdate(void);		//�����X�V����
	void EggMove(void);			//���̓���

	void DebugKey(void);		//�f�o�b�O�L�[

	int m_nPlayNumber;			//�v���C�p�ԍ�
	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_Objmove;		//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;	//�����ړ��̈ړ�����
	D3DXVECTOR3 m_size;			//�傫��
	D3DXVECTOR3 m_AtkPos;		//�U���ʒu
	D3DXVECTOR3 m_CameraPos;	//�J�����ʒu�ʒu
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	bool m_bJump;				//�W�����v���������ǂ���
	int m_nActionCount;			//�s���̃J�E���g
	STATE m_State;				//���
	MOVE_STATE m_MoveState;		//�ړ�����
	int m_nStateCount;			//��ԊǗ��p�J�E���g

	float m_CollisionRot;		//�����蔻��p�̌���

	CMapSystem::GRID m_Grid;	//�O���b�g�ԍ�
	bool m_bGritCenter;			//�O���b�g�̒��S�ʒu�ɂ��邩�ǂ���

	int m_nLife;				//���C�t
	int m_nTime;
	bool m_OKL;					//���ւ̐i�s��������邩�ǂ���
	bool m_OKR;					//�E�ւ̐i�s��������邩�ǂ���
	bool m_OKU;					//��ւ̐i�s��������邩�ǂ���
	bool m_OKD;					//���ւ̐i�s��������邩�ǂ���
	bool m_bInput;				//���͂��s�������ǂ���
	bool m_bPressObj;			//�I�u�W�F�N�g�ɉ�����Ă��邩�ǂ���
	bool m_bInvincible;			//���G���ǂ���
	int m_nInvincibleCount;		//���G����

	bool m_UseItem;				//�A�C�e�����g�p�\���ǂ���

	CSlowManager* m_pSlow;		// �X���[

	ITEM_TYPE m_eItemType;		// �����Ă�A�C�e���̎��
	CScore* m_pScore;			// �X�R�A�̃|�C���^

	CObjectX* m_pUpEgg;			//�����f���̏�
	CObjectX* m_pDownEgg;		//�����f���̉�
	D3DXVECTOR3 m_EggMove;		//���̓���

	CLifeUi* m_pLifeUi;

	CShadow* m_pShadow;

	//�}�b�v�Ƃ̃}�g���b�N�X���
	D3DXMATRIX *m_UseMultiMatrix;			//�|�����킹��}�g���b�N�X

	// �ÓI�����o�ϐ�
	static CListManager<CPlayer>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator; // �C�e���[�^�[
	float m_fCrossTimer; // �\���˂������Ă����鎞��
};

#endif