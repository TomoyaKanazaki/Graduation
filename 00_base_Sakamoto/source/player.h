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
class CObjectBillboard;

//�I�u�W�F�N�g�v���C���[�N���X
class CPlayer : public CObjectCharacter
{
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

	// �i�s�󋵂��Ǘ�����\����
	struct PROGGRESS
	{

	};

	static CPlayer* Create(int PlayNumber);
	HRESULT Init(int PlayNumber) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPlayNumber(int Number) { m_nPlayNumber = Number; }
	int GetPlayNumber(void) { return m_nPlayNumber; }

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetMoveX(float moveX) { m_move.x = moveX; }
	void SetMoveZ(float moveZ) { m_move.z = moveZ; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetObjMoveX(float move) { m_Objmove.x = move; }
	void SetObjMoveZ(float move) { m_Objmove.z = move; }
	D3DXVECTOR3 GetObjMove(void) { return m_Objmove; }
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

	// �ړ���ԃN���X�p
	void SetEggMove(D3DXVECTOR3 EggMove) { m_EggMove = EggMove; }	// ���̈ړ��ʐݒ�
	D3DXVECTOR3 GetEggMove() { return m_EggMove; }			// ���̈ړ��ʎ擾
	float GetSpeed();		// �v���C���[�̈ړ����x�擾
	MOVE_STATE GetMoveState() { return m_MoveState; }		// �v���C���[�̈ړ������̏��
	void SetInput(bool bInput) { m_bInput = bInput; }		// ���͔���̐ݒ�
	bool IsInput() { return m_bInput; }		// �v���C���[�̓��͔���
	void SetInvincible(bool bInvincible) { m_bInvincible = bInvincible; }		// ���G���ǂ���
	void SetInvincibleCount(int nInvincibleCount) { m_nInvincibleCount = nInvincibleCount; }		// ���G����

	// �}�b�v�ԍ��̐ݒ�
	virtual void SetGrid(const CMapSystem::GRID& pos) { m_Grid = pos; }
	CMapSystem::GRID GetGrid(void) { return m_Grid; }

	virtual void Death(void);

	void SetItemType(ITEM_TYPE eType);
	ITEM_TYPE GetItemType() { return m_eItemType; }		// �A�C�e���̎�ގ擾

	void ResetCrossTimer() { m_fCrossTimer = 0.0f; } // �\���ˏ������Ԃ̏�����

	// �ÓI�����o�֐�
	static CListManager<CPlayer>* GetList(void);	// ���X�g�擾

private:

	void ActionState(void);							//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(D3DXVECTOR3& posMy, D3DXVECTOR3& rotThis);			//��ԊǗ�
	void Move(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy);								//�ړ�����
	D3DXVECTOR3 MoveInputKey(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy,D3DXVECTOR3 Move);		//�ړ����̓L�[�{�[�h
	D3DXVECTOR3 MoveInputPadStick(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move);//�ړ����̓p�b�h�X�e�B�b�N
	D3DXVECTOR3 MoveInputPadKey(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy, D3DXVECTOR3 Move);	//�ړ����̓p�b�h�L�[
	void Rot(D3DXVECTOR3& rotMy);									//�ړ���������
	void Attack(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy);			//�U������
	void CollisionWall(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);		//�ǂƂ̓����蔻��
	void CollisionPressWall(D3DXVECTOR3& posMy, D3DXVECTOR3& sizeMy,useful::COLLISION XYZ);	//�ǂƂ̈�������
	void CollisionWaitRailBlock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//�~�܂��Ă��郌�[���u���b�N�Ƃ̓����蔻��
	void CollisionMoveRailBlock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//�����Ă��郌�[���u���b�N�Ƃ̓����蔻��
	void CollisionWaitRock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//�~�܂��Ă����Ƃ̓����蔻��
	void CollisionMoveRock(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//�����Ă����Ƃ̓����蔻��
	void SearchWall(D3DXVECTOR3& posMy);							//�ǂ̃T�[�`����
	void CollisionDevilHole(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy, useful::COLLISION XYZ);	//�f�r���z�[���Ƃ̓����蔻��
	void CollisionEnemy(D3DXVECTOR3& posMy);						// �G�Ƃ̓����蔻��
	void CollisionStageOut(D3DXVECTOR3& posMy);					// �X�e�[�W�O�̓����蔻��
	bool CollisionStageIn(D3DXVECTOR3& posMy);					// �X�e�[�W���ɂ��邩�ǂ���
	void CollisionPressStageOut(D3DXVECTOR3& posMy);				// �X�e�[�W�O�̈�������

	void CameraPosUpdate(D3DXVECTOR3& posMy);	//�J�����ʒu�X�V����
	void PosUpdate(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy);		//�ʒu�X�V����
	void ObjPosUpdate(D3DXVECTOR3& posMy, D3DXVECTOR3& posOldMy, D3DXVECTOR3& sizeMy);	//�I�u�W�F�N�g�ɂ��ʒu�X�V����
	void RotUpdate(D3DXVECTOR3& rotMy);		//�����X�V����
	void EggMove(D3DXVECTOR3& posMy, D3DXVECTOR3& rotMy);			//���̓���

	void UI_Create(void);			//��������UI�̐���
	void UI_Init(void);				//��������UI�̏�����

	void ControlEffect(CEffekseer* pTarget, const D3DXVECTOR3* pPos = nullptr); // �G�t�F�N�g�̑���
	void RotationEffect(CEffekseer* pTarget); // �G�t�F�N�g�̑���
	void MoveEffect(CEffekseer* pTarget, const D3DXVECTOR3* pPos = nullptr); // �G�t�F�N�g�̑���

	void DebugKey(void);			//�f�o�b�O�L�[

	int m_nPlayNumber;				//�v���C�p�ԍ�
	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;		//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_Objmove;			//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rotDest;			//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;		//�����ړ��̈ړ�����
	D3DXVECTOR3 m_AtkPos;			//�U���ʒu
	D3DXVECTOR3 m_CameraPos;		//�J�����ʒu�ʒu
	bool m_bJump;					//�W�����v���������ǂ���
	int m_nActionCount;				//�s���̃J�E���g
	STATE m_State;					//���
	MOVE_STATE m_MoveState;			//�ړ�����
	int m_nStateCount;				//��ԊǗ��p�J�E���g

	float m_CollisionRot;			//�����蔻��p�̌���

	CMapSystem::GRID m_Grid;		//�O���b�g�ԍ�
	bool m_bGritCenter;				//�O���b�g�̒��S�ʒu�ɂ��邩�ǂ���

	int m_nLife;					//���C�t
	int m_nTime;
	bool m_OKL;						//���ւ̐i�s��������邩�ǂ���
	bool m_OKR;						//�E�ւ̐i�s��������邩�ǂ���
	bool m_OKU;						//��ւ̐i�s��������邩�ǂ���
	bool m_OKD;						//���ւ̐i�s��������邩�ǂ���
	bool m_bInput;					//���͂��s�������ǂ���
	bool m_bPressObj;				//�I�u�W�F�N�g�ɉ�����Ă��邩�ǂ���
	bool m_bInvincible;				//���G���ǂ���
	int m_nInvincibleCount;			//���G����

	bool m_UseItem;					//�A�C�e�����g�p�\���ǂ���

	CSlowManager* m_pSlow;			// �X���[

	ITEM_TYPE m_eItemType;			// �����Ă�A�C�e���̎��
	CScore* m_pScore;				// �X�R�A�̃|�C���^

	CObjectX* m_pUpEgg;				//�����f���̏�
	CObjectX* m_pDownEgg;			//�����f���̉�
	D3DXVECTOR3 m_EggMove;			//���̓���

	CLifeUi* m_pLifeUi;				//�̗�UI
	CObjectBillboard* m_pP_NumUI;	//�v���C���[�ԍ�UI		

	CEffekseer* m_pEffectEgg;		// ���̃G�t�F�N�g
	CEffekseer* m_pEffectSpeed;		// �������̃G�t�F�N�g
	CEffekseer* m_pEffectGuide; // �����ʒu�̃G�t�F�N�g
	CEffekseer* m_pEffectItem; // �A�C�e�������G�t�F�N�g

	// �ÓI�����o�ϐ�
	static CListManager<CPlayer>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator; // �C�e���[�^�[
	float m_fCrossTimer; // �\���˂������Ă����鎞��
};

#endif