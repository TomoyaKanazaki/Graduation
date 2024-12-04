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

	// �A�C�e���̎��
	enum ITEM_TYPE
	{
		TYPE_NONE = 0,		// �����Ȃ�
		TYPE_CROSS,			// �\����
		TYPE_BIBLE,			// ����
		TYPE_MAX
	};

	static CPlayer* Create(int PlayNumber);
	HRESULT Init(int PlayNumber) override;
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPlayNumber(int Number) { m_nPlayNumber = Number; }
	int GetPlayNumber(void) { return m_nPlayNumber; }

	bool GetJump(void) { return m_bJump; }
	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetModelDisp(bool Sst);
	void SetPartsDisp(int nParts, bool Set);
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

	
	void ChangeMoveState(CMoveState* pMoveState) override;		// �ړ���ԕύX
	D3DXVECTOR3 GetEggMove() override { return m_EggMove; }		// ���̈ړ��ʎ擾


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
	D3DXVECTOR3 m_rotDest;			//�����̖ړI�n
	D3DXVECTOR3 m_AutoMoveRot;		//�����ړ��̈ړ�����
	D3DXVECTOR3 m_AtkPos;			//�U���ʒu
	D3DXVECTOR3 m_CameraPos;		//�J�����ʒu�ʒu
	bool m_bJump;					//�W�����v���������ǂ���
	int m_nActionCount;				//�s���̃J�E���g
	int m_nStateCount;				//��ԊǗ��p�J�E���g

	float m_CollisionRot;			//�����蔻��p�̌���

	bool m_bGritCenter;				//�O���b�g�̒��S�ʒu�ɂ��邩�ǂ���

	int m_nLife;					//���C�t
	int m_nTime;
	
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

	CMoveState* m_pMoveState;		// �ړ����

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