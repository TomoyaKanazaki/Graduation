//============================================
//
//	�v���C���[�̏��� [Player.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"
#include "useful.h"

//�O���錾
class CModel;
class CMotion;
class CObject2D;
class CObject3D;
class CObjGauge2D;
class CUltimate;
class CSlowManager;
class CObjectBillboard;
class CNumber;
class CMoveHelpUI;
class CParameterUI;

class CEnemy;
class CBoss;
class CLifeUi;
class CFire;

//�I�u�W�F�N�g�v���C���[�N���X
class CPlayer : public CObject
{
private:
	static const int ENCOUNT_MAX = 128;

public:
	CPlayer(int nPriority = 2);
	~CPlayer();

	//�v���C���[�̃��[�V����
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,		//�ҋ@
		ACTION_MOVE,			//�ړ�
		ACTION_EGG,				//�����(�������O�O
		ACTION_DEATH,			//���S���(�������O�P
		ACTION_MAX,				//�ő�
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
	static CPlayer* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void TutorialUpdate(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	STATE GetState(void) { return m_State; }
	bool GetJump(void) { return m_bJump; }
	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
	void SetAction(ACTION_TYPE Action, float BlendTime);
	ACTION_TYPE GetAction(void) { return m_Action; }
	void SetModelDisp(bool Sst);
	void SetPartsDisp(int nParts, bool Set);
	void SetMultiMatrix(bool Set) { m_bMultiMatrix = Set; }
	bool GetMultiMatrix(void) { return m_bMultiMatrix; }
	void SetUseMultiMatrix(D3DXMATRIX Set) { m_UseMultiMatrix = Set; }
	D3DXMATRIX GetUseMultiMatrix(void) { return m_UseMultiMatrix; }

	void SetUseItem(bool bUse) { m_UseItem = bUse; }
	bool GetbUseItem() { return m_UseItem; }

	int GetWightNumber() { return m_nMapWight; }
	int GetHeightNumber() { return m_nMapHeight; }
	bool GetGritCenter() { return m_bGritCenter; }

	void Death(void);
	bool SortObject(D3DXVECTOR3 pos);					// �I�u�W�F�N�g�Ƃ̃\�[�g����

	void SetItemType(ITEM_TYPE eType);
	ITEM_TYPE GetItemType() { return m_eItemType; }		// �A�C�e���̎�ގ擾

	// �ÓI�����o�֐�
	static CListManager<CPlayer>* GetList(void); // ���X�g�擾

private:
	void MyObjCreate(void);							//�������ێ�����I�u�W�F�N�g�̐���
	void ActionState(void);							//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);						//��ԊǗ�
	void Move(void);								//�ړ�����
	void Rot(void);									//�ړ���������
	void Attack(void);								//�U������
	void CollisionWall(useful::COLLISION XYZ);		//�ǂƂ̓����蔻��
	void SearchWall(void);							//�ǂ̃T�[�`����
	void CollisionDevilHole(useful::COLLISION XYZ);	//�f�r���z�[���Ƃ̓����蔻��
	void CollisionEnemy(void);						// �G�Ƃ̓����蔻��
	void CollisionStageOut(void);					// �X�e�[�W�O�̓����蔻��
	void MapSystemNumber(void);						// �v���C���[���}�b�v�̂ǂ̃}�X�ɑ��݂��Ă��邩�ݒ肷��

	void CameraPosUpdate(void);	//�J�����ʒu�X�V����
	float MoveSlopeX(void);		//�X�����̈ړ��ʕϓ�
	float MoveSlopeZ(void);		//�X�����̈ړ��ʕϓ�
	void PosUpdate(void);		//�ʒu�X�V����
	void RotUpdate(void);		//�����X�V����

	void DebugKey(void);		//�f�o�b�O�L�[

	void LoadLevelData(const char* pFilename);

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

	int m_nMapWight;			//�}�b�v�̉��ԍ�
	int m_nMapHeight;			//�}�b�v�̏c�ԍ�
	bool m_bGritCenter;			//�O���b�g�̒��S�ʒu�ɂ��邩�ǂ���

	int m_nLife;				//���C�t
	bool m_OKL;					//���ւ̐i�s��������邩�ǂ���
	bool m_OKR;					//�E�ւ̐i�s��������邩�ǂ���
	bool m_OKU;					//��ւ̐i�s��������邩�ǂ���
	bool m_OKD;					//���ւ̐i�s��������邩�ǂ���
	bool m_bInput;				//���͂��s�������ǂ���

	bool m_UseItem;				//�A�C�e�����g�p�\���ǂ���

	CSlowManager* m_pSlow;		// �X���[

	ITEM_TYPE m_eItemType;		// �����Ă�A�C�e���̎��

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
	CLifeUi* m_pLifeUi;

	//�}�b�v�Ƃ̃}�g���b�N�X���
	bool m_bMultiMatrix;					//�}�g���b�N�X�̊|�����킹�����邩�ǂ���
	D3DXMATRIX m_UseMultiMatrix;			//�|�����킹��}�g���b�N�X

	// �ÓI�����o�ϐ�
	static CListManager<CPlayer>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator; // �C�e���[�^�[
};

#endif